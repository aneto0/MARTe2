/**
 * @file BrokerI.cpp
 * @brief Source file for class BrokerI
 * @date 17/05/2016
 * @author Andre Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class BrokerI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BrokerI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
BrokerI::BrokerI() {
    application = NULL_PTR(RealTimeApplication *);
    synchronized = false;
    finalised = false;
    syncIndex = 0u;
}

BrokerI::~BrokerI() {
    uint32 nOfSignalsWithIndexingBlocks = nIndexingBlocksPerSignal.GetSize();
    uint32 ** element = NULL_PTR(uint32 **);

    for (uint32 i = 0u; i < nOfSignalsWithIndexingBlocks; i++) {
        if (nIndexingBlocksPerSignal.Peek(i, element)) {
            if (element != NULL) {
                uint32 nIndexingBlocks = 0u;
                if (nIndexingBlocksPerSignal.Peek(i, nIndexingBlocks)) {
                    for (uint32 k = 0u; k < nIndexingBlocks; k++) {
                        delete[] element[k];
                    }
                }
                delete[] element;
            }
        }
    }
    uint32 nOfSignalsWithSampleBlocks = nSamplesBlocksPerSignal.GetSize();
    for (uint32 i = 0u; i < nOfSignalsWithSampleBlocks; i++) {
        if (nSamplesBlocksPerSignal.Peek(i, element)) {
            if (element != NULL) {
                uint32 nSampleBlocks = 0u;
                if (nSamplesBlocksPerSignal.Peek(i, nSampleBlocks)) {
                    for (uint32 k = 0u; k < nSampleBlocks; k++) {
                        delete[] element[k];
                    }
                }
                delete[] element;
            }
        }
    }
}

void BrokerI::SetApplication(RealTimeApplication &rtApp) {
    application = &rtApp;
}

bool BrokerI::AddSignalPrivate(ReferenceT<GAMSignalI> gamSignal,
                               uint32 initialOffset,
                               uint32 offset) {

    bool ret = (application != NULL);
    if (ret) {
        ret = gamSignal.IsValid();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
    }

    if (ret) {
        const char8* typeName = gamSignal->GetType();

        // case structured
        TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);

        uint32 varSize = 0u;
        uint32 numberOfMembers = gamSignal->Size();

        if (typeDes == InvalidType) {

            const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
            ret = (item != NULL);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                const Introspection *intro = item->GetIntrospection();
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", gamSignal->GetType())
            }

            ret = (intro != NULL);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                varSize = intro->GetClassSize();
                // unsupported multi-dimensional
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not introspectable", gamSignal->GetType())
            }

            ret = (gamSignal->GetNumberOfDimensions() == 0u);
            if (ret) {
                // final type
                if (numberOfMembers > 0u) {
                    /*lint -e{613} NULL pointer checking done before entering here */
                    uint32 numberOfIntroMembers = intro->GetNumberOfMembers();

                    // the variables will be added in the same order of the introspection
                    for (uint32 i = 0u; (i < numberOfIntroMembers) && (ret); i++) {
                        bool found = false;
                        /*lint -e{613} NULL pointer checking done before entering here */
                        const IntrospectionEntry introEntry = (*intro)[i];
                        for (uint32 j = 0u; (j < numberOfMembers) && (ret) && (!found); j++) {
                            ReferenceT<GAMGenericSignal> subDef = gamSignal->Get(j);
                            ret = subDef.IsValid();
                            if (ret) {
                                // if the member name matches
                                const char8 *defMemberName = subDef->GetName();
                                ret = (defMemberName != NULL);
                                if (ret) {
                                    /*lint -e{613} NULL pointer checking done before entering here */
                                    if (StringHelper::Compare(introEntry.GetMemberName(), defMemberName) == 0) {
                                        /*lint -e{613} if (pointablePtr == NULL) it just returns the offset*/
                                        ret = AddSignalPrivate(subDef, initialOffset, introEntry.GetMemberByteOffset());
                                        found = true;
                                    }
                                }
                            }
                        }
                        // this should never happen if we call before GAMSignalI.Verify()
                        if (!found) {
                            ret = false;
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Member %s of %s  with type %s does not matches with the introspection data",
                                                    introEntry.GetMemberName(), gamSignal->GetName(), gamSignal->GetType())
                        }
                    }
                }
            }
            else {
                // this should never happen if we call before DataSource.Allocate()
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Unsupported multi-dimensional structure %s", gamSignal->GetName())
            }
        }
        // basic type
        else {
            varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
        }

        // same code for structured and basic
        if ((ret) && (numberOfMembers == 0u)) {

            // the size is changed by this function (* #samples * #cycles * #dimension)
            ReferenceT<DataSource> dataSource;
            ReferenceT<DataSourceSignalI> dataSourceSignal;
            ret = Verify(gamSignalIn, varSize, dataSource, dataSourceSignal);
            if (ret) {
                ret = dataSource.IsValid();
            }
            if (ret) {
                ret = dataSourceSignal.IsValid();
            }
            if (ret) {
                //ret = dataSourcesVars.Add(dataSourceSignal.operator ->());
                void **dsPointer0 = dataSource->GetDataSourcePointer(dataSourceSignal, 0u);
                void **dsPointer1 = dataSource->GetDataSourcePointer(dataSourceSignal, 1u);

                ret = ((dsPointer0 != NULL) && (dsPointer1 != NULL));
                if (ret) {
                    // add the data source pointer
                    ret = DSPointers[0].Add(dsPointer0);
                }
                if (ret) {
                    ret = DSPointers[1].Add(dsPointer1);
                }
                if (ret) {
                    // add the GAM pointer
                    uint32 finalOffset = offset + initialOffset;
                    ret = GAMOffsets.Add(finalOffset);
                    printf("\nAdded offset %d rel offset %d of %s in %s\n", finalOffset, offset, gamSignal->GetName(), gamSignal->GetPath());
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Invalid gamSignal");
    }

    return ret;

}

bool BrokerI::AddSignal(ReferenceT<GAMSignalI> gamSignalIn,
                        void * const ptr) {

    bool ret = (ptr != NULL);
    if (ret) {
        uint32 currentSignalIndex = 0u;
        uint32 initialOffset = 0u;

        currentSignalIndex = structuredSignalOffset.GetSize();
        ret = AddSignalPrivate(gamSignalIn, initialOffset, 0u);
        if (ret) {
            //Note that the call to AddSignalPrivate might have added several structure members =>
            //structuredSignalOffset.GetSize() will now be > currentSignalIndex
            ret = uniqueSignalIndex.Add(currentSignalIndex);
            if (ret) {
                // adds the memory start pointer
                ret = gamMemoryAreaPointer.Add(ptr);
                if (ret) {
                    ret = Insert(gamSignalIn);
                }
            }
        }
        else {
            // Error ! Remove eventual added things to be consistent
            uint32 last = 0u;
            bool ok = true;
            if (uniqueSignalIndex.GetSize() > 0u) {
                if (uniqueSignalIndex.Peek((uniqueSignalIndex.GetSize() - 1u), last)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to chunkIndex.Peek. This will lead to inconsistencies in the broker memory");
                }
            }
            while ((structuredSignalOffset.GetSize() > last) && (ok)) {
                if (!structuredSignalOffset.Remove(structuredSignalOffset.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from structuredSignalOffset. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((dataSourceSignalPointers[0].GetSize() > last) && (ok)) {
                if (!dataSourceSignalPointers[0].Remove(dataSourceSignalPointers[0].GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from dataSourceMemory[0]. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((dataSourceSignalPointers[1].GetSize() > last) && (ok)) {
                if (!dataSourceSignalPointers[1].Remove(dataSourceSignalPointers[1].GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from dataSourceMemory[1]. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((blockParamList.GetSize() > last) && (ok)) {
                if (!blockParamList.Remove(blockParamList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from blockParamList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((samplesParamList.GetSize() > last) && (ok)) {
                if (!samplesParamList.Remove(samplesParamList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from samplesParamList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((nIndexingBlocksPerSignal.GetSize() > last) && (ok)) {
                if (!nIndexingBlocksPerSignal.Remove(nIndexingBlocksPerSignal.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from nIndexingBlocksPerSignal. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((nSamplesBlocksPerSignal.GetSize() > last) && (ok)) {
                if (!nSamplesBlocksPerSignal.Remove(nSamplesBlocksPerSignal.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from nSamplesBlocksPerSignal. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((nSamplesList.GetSize() > last) && (ok)) {
                if (!nSamplesList.Remove(nSamplesList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from nSamplesList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Memory cannot be NULL");
    }
    return ret;
}

bool BrokerI::SetBlockParameters(ReferenceT<GAMSignalI> gamSignal,
                                 ReferenceT<DataSourceSignal> dataSourceSignal,
                                 uint32 &typeSize) {

    bool ret = true;

    uint32 nOfDSElements = dataSourceSignal->GetNumberOfElements();
    uint32 numberOfDSSamples = dataSourceSignal->GetNumberOfSamples();
    uint32 numberOfGAMSamples = 1u;

    // the number of data source elements can be different than the gam signal one
    if (ret) {
        ReferenceT<GAMSampledSignal> gamSampledSignal = gamSignal;
        uint32 **samplesM = NULL_PTR(uint32**);
        uint32 **indexingM = NULL_PTR(uint32**);
        uint32 nIndexBlocks = 0u;
        uint32 nSamplesBlocks = 0u;

        bool isGAMSampledSignal = gamSampledSignal.IsValid();

        // check if the gam wants to read samples
        if (isGAMSampledSignal) {

            StreamString confSamples = "SignalSamples=";
            //Small trick to parse configuration automatically from gamSampledSignal->GetSamples()
            confSamples += gamSampledSignal->GetSamples();
            //If gamSampledSignal->GetSamples() == "" => hasSamplesDefined = false
            bool hasSamplesDefined = (confSamples != "SignalSamples=");

            if (hasSamplesDefined) {
                confSamples.Seek(0ULL);
                ConfigurationDatabase cdb;
                StandardParser parser(confSamples, cdb);
                hasSamplesDefined = parser.Parse();
                // if not get the last sample by default
                if (hasSamplesDefined) {
                    // if the node is read
                    AnyType at = cdb.GetType("SignalSamples");
                    ret = (at.GetDataPointer() != NULL);
                    if (ret) {
                        uint32 nCols = at.GetNumberOfElements(0u);
                        ret = (nCols == 3);
                        // the number of columns must be 3
                        if (ret) {
                            nSamplesBlocks = at.GetNumberOfElements(1u);
                            samplesM = new uint32*[nSamplesBlocks];
                            //Add one row samplesM for each i < nSamplesBlocks
                            for (uint32 i = 0u; i < nSamplesBlocks; i++) {
                                samplesM[i] = new uint32[nCols];
                            }
                            Matrix<uint32> paramListMatrix(samplesM, nSamplesBlocks, nCols);
                            //Read SignalSamples into samplesM
                            ret = (cdb.Read("SignalSamples", paramListMatrix));
                            if (ret) {
                                // save begin index and size
                                numberOfGAMSamples = 0u;
                                uint32 maxSamplesIndex = (numberOfDSSamples - 1u);
                                //For each of the defined sample block with format {a, b, c} do:
                                for (uint32 i = 0u; (i < nSamplesBlocks) && (ret); i++) {
                                    ret = (samplesM[i][2] > 0u);
                                    if (ret) {
                                        //check that the last index a is smaller than the total number of samples index
                                        ret = (samplesM[i][0] <= samplesM[i][2]);
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(
                                                ErrorManagement::FatalError,
                                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c. Found c < 0",
                                                gamSignal->GetName())
                                    }
                                    if (ret) {
                                        //check that the oldest index a is smaller than the total number of samples index
                                        ret = (samplesM[i][1] <= samplesM[i][2]);
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(
                                                ErrorManagement::FatalError,
                                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c. Found a > c",
                                                gamSignal->GetName())
                                    }

                                    if (ret) {
                                        // check that the oldest index is greater than the newest index (remember ascending order with zero being the newest)
                                        ret = (samplesM[i][1] <= samplesM[i][0]);
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(
                                                ErrorManagement::FatalError,
                                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c. Found b > c",
                                                gamSignal->GetName())
                                    }
                                    if (ret) {
                                        // fractional indexing
                                        // the end index
                                        uint32 end = maxSamplesIndex - ((samplesM[i][1] * maxSamplesIndex) / samplesM[i][2]);
                                        // the begin index
                                        samplesM[i][0] = maxSamplesIndex - ((samplesM[i][0] * maxSamplesIndex) / samplesM[i][2]);
                                        // the number of samples
                                        samplesM[i][1] = (end - samplesM[i][0]) + 1u;
                                        // the offset in byte of each sample
                                        samplesM[i][2] = typeSize * nOfDSElements;
                                        // store directly the offset in byte
                                        samplesM[i][0] *= samplesM[i][2];
                                        printf("\nsample beg=%d\n", samplesM[i][0]);
                                        numberOfGAMSamples += samplesM[i][1];
                                    }
                                    else {
                                        REPORT_ERROR_PARAMETERS(
                                                ErrorManagement::FatalError,
                                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c. Found b > a",
                                                gamSignal->GetName())
                                    }
                                }
                            }
                        }
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                    "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c",
                                                    gamSignal->GetName())
                        }
                    }
                }
            }
        }
        if (!isGAMSampledSignal) {
            // take the last sample by default
            samplesM = new uint32*;
            samplesM[0] = new uint32[3];
            // the begin index
            samplesM[0][0] = numberOfSamples - 1u;
            // read only the last
            samplesM[0][1] = 1u;
            samplesM[0][2] = (typeSize * nOfDSElements);
            samplesRows = 1u;
            numberOfGAMSamples = 1u;
        }

        if (ret) {
            // calculate the number of elements to be read in GAM
            uint32 nOfGAMElements = 1u;
            for (uint32 k = 0u; k < gamSignal->GetNumberOfDimensions(); k++) {
                nOfGAMElements *= gamSignal->GetNumberOfElements(k);
            }

            StreamString confString = "SignalBlocks=";
            confString += gamSignal->GetOperation();

            init = (confString != "SignalBlocks=");
            if (init) {
                // a set of blocks to be read
                confString.Seek(0ULL);
                ConfigurationDatabase cdb;

                StandardParser parser(confString, cdb);
                init = parser.Parse();
                // the field exists
                if (init) {

                    AnyType at = cdb.GetType("SignalBlocks");
                    ret = (at.GetDataPointer() != NULL);
                    if (ret) {
                        uint32 nCols = at.GetNumberOfElements(0u);
                        ret = (nCols == 2);
                        if (ret) {
                            nBlockRows = at.GetNumberOfElements(1u);
                            blocksM = new uint32*[nBlockRows];
                            for (uint32 i = 0u; i < nBlockRows; i++) {
                                blocksM[i] = new uint32[nCols];
                            }
                            Matrix<uint32> paramListMatrix(blocksM, nBlockRows, nCols);
                            ret = (cdb.Read("SignalBlocks", paramListMatrix));
                            if (ret) {
                                // checks if the size in the GAM variable matches with the
                                // sum of the block sizes
                                uint32 testSize = 0u;
                                for (uint32 i = 0u; (i < nBlockRows) && (ret); i++) {
                                    uint32 beg = blocksM[i][0];
                                    uint32 end = blocksM[i][1];
                                    ret = (end >= beg);
                                    if (ret) {
                                        // begin and end must be into the DS memory
                                        ret = ((beg < nOfDSElements) && (end < nOfDSElements));
                                        if (ret) {
                                            uint32 size = (end - beg) + 1u;
                                            // as char8* pointer
                                            // the begin index
                                            blocksM[i][0] *= typeSize;
                                            // the size to be copied
                                            blocksM[i][1] = (size * typeSize);
                                            testSize += size;
                                        }
                                    }
                                }

                                if (ret) {
                                    ret = (testSize <= nOfGAMElements);
                                    if (testSize != nOfGAMElements) {
                                        REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
                                                                "The GAM signal %s has more elements than the the effective read-write ones",
                                                                gamSignal->GetName())
                                    }
                                }
                            }
                        }
                        else {
                            // def can be added to another broker?
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::RecoverableError,
                                    "The field \"Operation\" of %s must declare a nx2 matrix {{a,b}...} with a,b<DataSourceSignal::GetNumberOfElements(*)",
                                    gamSignal->GetName())
                        }
                    }
                }
            }

            if (!init) {
                // if there is not the block field read all the block
                if (ret) {

                    blocksM = new uint32*;
                    blocksM[0] = new uint32[2];

                    //begin =0 if #samples=1
                    blocksM[0][0] = 0u;

                    // checks that the GAM signal has enough space
                    ret = (nOfGAMElements <= nOfDSElements);
                    if (ret) {
                        //the size is the one defined
                        blocksM[0][1] = (typeSize * nOfGAMElements);
                        nBlockRows = 1u;
                    }
                }
            }
            if (ret) {
                ret = blockParamList.Add(blocksM);
                if (ret) {
                    ret = blockParamRows.Add(nBlockRows);
                }
                if (ret) {
                    ret = samplesParamList.Add(samplesM);
                }
                if (ret) {
                    ret = samplesParamRows.Add(samplesRows);
                }

                // how many consecutive reads
                if (ret) {
                    ret = numberOfCyclesPerVar.Add(numberOfCycles);
                }
                if (ret) {
                    ret = nSamplesList.Add(numberOfGAMSamples);
                }
                if (!ret) {
                    // def can be added to another broker?
                    REPORT_ERROR_PARAMETERS(ErrorManagement::RecoverableError, "Failure of StaticList::Add for GAM signal %s", gamSignal->GetName())
                }

                printf("\n%d %d %d\n", typeSize, numberOfGAMSamples, nOfGAMElements);

                // change dimension accordingly to #cycles and #samples
                typeSize *= ((numberOfCycles * numberOfGAMSamples) * nOfGAMElements);
            }
            // delete resources in case of error
            else {
                if (blocksM != NULL) {
                    for (uint32 i = 0u; i < nBlockRows; i++) {
                        if (blocksM[i] != NULL) {
                            delete[] blocksM[i];
                            blocksM[i] = NULL_PTR(uint32 *);
                        }
                    }
                    delete[] blocksM;
                    blocksM = NULL_PTR(uint32 **);
                }
                if (samplesM != NULL) {
                    for (uint32 i = 0u; i < samplesRows; i++) {
                        if (samplesM[i] != NULL) {
                            delete[] samplesM[i];
                            samplesM[i] = NULL_PTR(uint32 *);
                        }
                    }
                    delete[] samplesM;
                    samplesM = NULL_PTR(uint32 **);
                }
            }

        }
    }
    else {
// def can be added to another broker?
        REPORT_ERROR_PARAMETERS(ErrorManagement::RecoverableError, "Trying to add the sync GAMSignal %s on a broker which already contains a sync GAMSignal",
                                gamSignal->GetName())
    }
    return ret;
}

}
