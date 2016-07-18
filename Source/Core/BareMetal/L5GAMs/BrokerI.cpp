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

#include "AdvancedErrorManagement.h"
#include "BrokerI.h"
#include "DataSourceI.h"
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
BrokerI::BrokerI() {

    numberOfCopies = 0u;
    functionSignalPointers = NULL_PTR(void **);
    copyByteSize = NULL_PTR(uint32 *);
    copyOffset = NULL_PTR(uint32 *);
#if 0
    application = NULL_PTR(RealTimeApplication *);
    finalised = false;
#endif
}

BrokerI::~BrokerI() {
    if (functionSignalPointers != NULL_PTR(void **)) {
        delete[] functionSignalPointers;
    }
    if (copyByteSize != NULL_PTR(uint32 *)) {
        delete[] copyByteSize;
    }
    if (copyOffset != NULL_PTR(uint32 *)) {
        delete[] copyOffset;
    }

#if 0
    uint32 nOfSignalsWithIndexBlocks = nIndexBlocksPerSignal.GetSize();
    uint32 ** element = NULL_PTR(uint32 **);

    for (uint32 i = 0u; i < nOfSignalsWithIndexBlocks; i++) {
        if (indexBlocksList.Peek(i, element)) {
            if (element != NULL) {
                uint32 nIndexBlocks = 0u;
                if (nIndexBlocksPerSignal.Peek(i, nIndexBlocks)) {
                    for (uint32 k = 0u; k < nIndexBlocks; k++) {
                        delete[] element[k];
                    }
                }
                delete[] element;
            }
        }
    }
    uint32 nOfSignalsWithSampleBlocks = nSamplesBlocksPerSignal.GetSize();
    for (uint32 i = 0u; i < nOfSignalsWithSampleBlocks; i++) {
        if (samplesBlocksList.Peek(i, element)) {
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
#endif
}

#if 0
void BrokerI::SetApplication(RealTimeApplication &rtApp) {
    application = &rtApp;
}

bool BrokerI::Finalise() {
    SetFinalised(true);
    return IsFinalised();
}

void BrokerI::SetFinalised(bool isFinalised) {
    finalised = isFinalised;
}

bool BrokerI::AddSignalPrivate(ReferenceT<GAMSignalI> gamSignal,
        void *gamSignalMemory,
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
            const Introspection *intro = NULL_PTR(Introspection *);
            ret = (item != NULL);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                intro = item->GetIntrospection();
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
                                        ret = AddSignalPrivate(subDef, gamSignalMemory, initialOffset, introEntry.GetMemberByteOffset());
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

        ReferenceT<DataSourceSignal> dataSourceSignal;
        // same code for structured and basic
        if ((ret) && (numberOfMembers == 0u)) {
            // the size is changed by this function (* #samples * #cycles * #dimension)
            dataSourceSignal = GetDataSourceSignal(gamSignal);
        }
        if (ret) {
            ret = (StringHelper::Compare(gamSignal->GetType(), dataSourceSignal->GetType()) == 0);
        }
        if (ret) {
            ret = SetBlockParameters(gamSignal, dataSourceSignal, varSize);
        }
        if (ret) {
            //ret = dataSourcesVars.Add(dataSourceSignal.operator ->());
            void *dsPointer0 = dataSourceSignal->GetDataSourcePointer(0u);
            void *dsPointer1 = dataSourceSignal->GetDataSourcePointer(1u);

            ret = ((dsPointer0 != NULL) && (dsPointer1 != NULL));
            if (ret) {
                // add the data source pointer
                ret = dataSourceSignalPointers[0].Add(dsPointer0);
            }
            if (ret) {
                ret = dataSourceSignalPointers[1].Add(dsPointer1);
            }
            if (ret) {
                // add the GAM pointer
                uint32 finalOffset = offset + initialOffset;
                ret = gamSignalPointers.Add(&reinterpret_cast<char8 *>(gamSignalMemory)[finalOffset]);
                //printf("\nAdded offset %d rel offset %d of %s in %s\n", finalOffset, offset, gamSignal->GetName(), gamSignal->GetPath());
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

        ret = AddSignalPrivate(gamSignalIn, ptr, initialOffset, 0u);
        if (ret) {
            ret = gamSignalList.Insert(gamSignalIn);
        }
        else {
            // Error ! Remove eventual added things to be consistent
            uint32 last = 0u;
            bool ok = true;
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
            while ((indexBlocksList.GetSize() > last) && (ok)) {
                if (!indexBlocksList.Remove(indexBlocksList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from indexBlocksList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((samplesBlocksList.GetSize() > last) && (ok)) {
                if (!samplesBlocksList.Remove(samplesBlocksList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                            "Failed to remove from samplesBlocksList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((nIndexBlocksPerSignal.GetSize() > last) && (ok)) {
                if (!nIndexBlocksPerSignal.Remove(nIndexBlocksPerSignal.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                            "Failed to remove from nIndexBlocksPerSignal. This will lead to inconsistencies in the broker memory");
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
            while ((nSamplesPerGAMSignalList.GetSize() > last) && (ok)) {
                if (!nSamplesPerGAMSignalList.Remove(nSamplesPerGAMSignalList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                            "Failed to remove from nSamplesPerGAMSignalList. This will lead to inconsistencies in the broker memory");
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

ReferenceT<DataSourceSignal> BrokerI::GetDataSourceSignal(ReferenceT<GAMSignalI> gamSignal) {
    ReferenceT<DataSourceSignal> dataSourceSignal;
    const char8 *dataSourceSignalPath = NULL_PTR(const char8 *);
    bool ok = (application != NULL);
    if (ok) {
        ok = gamSignal.IsValid();
    }
    if (ok) {
        dataSourceSignalPath = gamSignal->GetPath();
        ok = (dataSourceSignalPath != NULL);
    }
    if (ok) {
// find the data source signal
        StreamString dataSourceSignalFullPath = "Data.";
        dataSourceSignalFullPath += dataSourceSignalPath;
        dataSourceSignal = application->Find(dataSourceSignalFullPath.Buffer());

    }
    return dataSourceSignal;
}

bool BrokerI::SetBlockParameters(ReferenceT<GAMSignalI> gamSignal,
        ReferenceT<DataSourceSignal> dataSourceSignal,
        uint32 &typeSize) {

    bool ret = true;

    uint32 nOfDSElements = dataSourceSignal->GetNumberOfElements();
    uint32 nOfGAMElements = 1u;

//uint32 numberOfDSSamples = dataSourceSignal->GetNumberOfSamples();
    uint32 numberOfGAMSamples = 1u;

// the number of data source elements can be different than the gam signal one
    ReferenceT<GAMSampledSignal> gamSampledSignal = gamSignal;
    uint32 **samplesM = NULL_PTR(uint32**);
    uint32 **indexM = NULL_PTR(uint32**);
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

        ConfigurationDatabase cdb;
        if (hasSamplesDefined) {
            confSamples.Seek(0ULL);
            StandardParser parser(confSamples, cdb);
            hasSamplesDefined = parser.Parse();
        }
        if (hasSamplesDefined) {
            // if the node is read
            AnyType at = cdb.GetType("SignalSamples");
            ret = (at.GetDataPointer() != NULL);
            uint32 nCols = 0u;
            if (ret) {
                nCols = at.GetNumberOfElements(0u);
                // the number of columns must be 3
                ret = (nCols == 3);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed parsing the \"Samples\" field of %s. GetDataPointer() == NULL",
                        gamSignal->GetName())
            }
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
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                        "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with a<=b<=c. Found number of columns != 3",
                        gamSignal->GetName())
            }
            if (ret) {
                // save begin index and size
                numberOfGAMSamples = 0u;
                //For each of the defined sample block with format {a, b, c} do:
                for (uint32 i = 0u; (i < nSamplesBlocks) && (ret); i++) {
                    //check that the oldest index a is smaller than the total number of samples
                    ret = (samplesM[i][0] < samplesM[i][2]);
                    if (ret) {
                        //check that the newest index b is smaller than the total number of samples index
                        ret = (samplesM[i][1] < samplesM[i][2]);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with a<=b<c. Found a > c",
                                gamSignal->GetName())
                    }
                    if (ret) {
                        //check that the oldest index a is smaller than the total number of samples index
                        ret = (samplesM[i][0] < samplesM[i][1]);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with a<=b<c. Found b > c",
                                gamSignal->GetName())
                    }
                    if (ret) {
                        // check that the newest index is greater than the oldest index
                        ret = (samplesM[i][1] >= samplesM[i][0]);
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with a<=b<=c. Found a > b",
                                gamSignal->GetName())
                    }
                    if (ret) {
                        // the size of the sample block in bytes
                        samplesM[i][1] *= typeSize * nOfDSElements;
                        // the begin index in bytes
                        samplesM[i][0] *= samplesM[i][1];
                        numberOfGAMSamples += samplesM[i][2];
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c. Found b > a",
                                gamSignal->GetName())
                    }
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c",
                        gamSignal->GetName())
            }
        }
        else {
            // take the last sample by default
            samplesM = new uint32*;
            samplesM[0] = new uint32[3];
            // the begin index
            // read only the last
            samplesM[0][1] = 1u;
            samplesM[0][2] = (typeSize * nOfDSElements);
            nSamplesBlocks = 1u;
            numberOfGAMSamples = 1u;
        }
    }
    if (ret) {
// calculate the number of elements to be read in GAM
        for (uint32 k = 0u; k < gamSignal->GetNumberOfDimensions(); k++) {
            nOfGAMElements *= gamSignal->GetNumberOfElements(k);
        }

        StreamString confString = "SignalIndex=";
        confString += gamSignal->GetOperation();
        bool hasIndexDefined = (confString != "SignalIndex=");
        ConfigurationDatabase cdb;
        if (hasIndexDefined) {
            // a set of blocks to be read
            confString.Seek(0ULL);

            StandardParser parser(confString, cdb);
            hasIndexDefined = parser.Parse();
        }
        // the field exists
        if (hasIndexDefined) {

            AnyType at = cdb.GetType("SignalIndex");
            ret = (at.GetDataPointer() != NULL);
            uint32 nCols = 0u;
            if (ret) {
                nCols = at.GetNumberOfElements(0u);
                ret = (nCols == 2);
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failed parsing the field \"Index\" of %s GetDataPointer() == NULL", gamSignal->GetName())
            }

            if (ret) {
                nIndexBlocks = at.GetNumberOfElements(1u);
                indexM = new uint32*[nIndexBlocks];
                for (uint32 i = 0u; i < nIndexBlocks; i++) {
                    indexM[i] = new uint32[nCols];
                }
                Matrix<uint32> paramListMatrix(indexM, nIndexBlocks, nCols);
                ret = (cdb.Read("SignalIndex", paramListMatrix));
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                        "The field \"Index\" of %s must declare a nx2 matrix {{a,b}...}. Found number of columns != 2.", gamSignal->GetName())
            }
            uint32 testSize = 0u;
            if (ret) {
                // checks if the size in the GAM variable matches with the
                // sum of the block sizes
                for (uint32 i = 0u; (i < nIndexBlocks) && (ret); i++) {
                    uint32 beg = indexM[i][0];
                    uint32 end = indexM[i][1];
                    ret = (end >= beg);
                    if (ret) {
                        // begin and end must be into the DS memory
                        ret = ((beg < nOfDSElements) && (end < nOfDSElements));
                        if (ret) {
                            uint32 size = (end - beg) + 1u;
                            // as char8* pointer
                            // the begin index
                            indexM[i][0] *= typeSize;
                            // the size to be copied
                            indexM[i][1] = (size * typeSize);
                            testSize += size;
                        }
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Defined indexes do not fit in the DataSourceSignal memory!.",
                                gamSignal->GetName())
                    }
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                        "The field \"Index\" of %s must declare a nx2 matrix {{a,b}...}. Failed to parse \"Index\" field.",
                        gamSignal->GetName())
            }
            if (ret) {
                ret = (testSize <= nOfGAMElements);
                if (testSize != nOfGAMElements) {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "The GAM signal %s has more elements than the the effective read-write ones",
                            gamSignal->GetName())
                }
            }
            else {
                // def can be added to another broker?
                REPORT_ERROR_PARAMETERS(ErrorManagement::RecoverableError,
                        "The field \"Index\" of %s must declare a nx2 matrix {{a,b}...} with a,b<DataSourceSignal::GetNumberOfElements(*)",
                        gamSignal->GetName())

            }
        }
        else {
            // if there is not the index field read all the block
            indexM = new uint32*;
            indexM[0] = new uint32[2];

            //begin =0 if #samples=1
            indexM[0][0] = 0u;

            // checks that the GAM signal has enough space
            ret = (nOfGAMElements <= nOfDSElements);
            if (ret) {
                //the size is the one defined
                indexM[0][1] = (typeSize * nOfGAMElements);
                nIndexBlocks = 1u;
            }

        }
    }
    if (ret) {
        ret = indexBlocksList.Add(indexM);
        if (ret) {
            ret = nIndexBlocksPerSignal.Add(nIndexBlocks);
        }
        if (ret) {
            ret = samplesBlocksList.Add(samplesM);
        }
        if (ret) {
            ret = nSamplesBlocksPerSignal.Add(nSamplesBlocks);
        }
    }

// how many consecutive reads
    int32 nCycles = gamSignal->GetCycles();
    if (ret) {
        ret = nCyclesPerGAMSignalList.Add(nCycles);
    }
    if (ret) {
        ret = nSamplesPerGAMSignalList.Add(numberOfGAMSamples);
// change dimension accordingly to #cycles and #samples
        if (nCycles < 1) {
            nCycles = 1;
        }
        typeSize *= ((nCycles * numberOfGAMSamples) * nOfGAMElements);
    }
    if (!ret) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Failure of StaticList::Add for GAM signal %s", gamSignal->GetName())

        if (indexM != NULL) {
            for (uint32 i = 0u; i < nIndexBlocks; i++) {
                if (indexM[i] != NULL) {
                    delete[] indexM[i];
                    indexM[i] = NULL_PTR(uint32 *);
                }
            }
            delete[] indexM;
            indexM = NULL_PTR(uint32 **);
        }
        if (samplesM != NULL) {
            for (uint32 i = 0u; i < nSamplesBlocks; i++) {
                if (samplesM[i] != NULL) {
                    delete[] samplesM[i];
                    samplesM[i] = NULL_PTR(uint32 *);
                }
            }
            delete[] samplesM;
            samplesM = NULL_PTR(uint32 **);
        }
    }
    return ret;
}

uint32 BrokerI::GetNumberOfSamplesBlocks(uint32 signalIndex) {
    uint32 ret;
    if (!nSamplesBlocksPerSignal.Peek(signalIndex, ret)) {
        ret = 0;
    }
    return ret;
}

uint32 BrokerI::GetNumberOfIndexBlocks(uint32 signalIndex) {
    uint32 ret;
    if (!nIndexBlocksPerSignal.Peek(signalIndex, ret)) {
        ret = 0;
    }
    return ret;
}

bool BrokerI::GetIndexBlockStartIndex(uint32 signalIndex,
        uint32 blockIndex,
        uint32 &startIndex) {
    bool ok = false;
    uint32 nIndexBlocksSignal;
    if (!nIndexBlocksPerSignal.Peek(signalIndex, nIndexBlocksSignal)) {
        if (blockIndex < nIndexBlocksSignal) {
            uint32 **signalIndexBlock = indexBlocksList[signalIndex];
            startIndex = signalIndexBlock[blockIndex][0];
            ok = true;
        }
    }
    return ok;
}

bool BrokerI::GetIndexBlockSize(uint32 signalIndex,
        uint32 blockIndex,
        uint32 &size) {
    bool ok = false;
    uint32 nIndexBlocksSignal;
    if (!nIndexBlocksPerSignal.Peek(signalIndex, nIndexBlocksSignal)) {
        if (blockIndex < nIndexBlocksSignal) {
            uint32 **signalIndexBlock = indexBlocksList[signalIndex];
            size = signalIndexBlock[blockIndex][1];
            ok = true;
        }
    }
    return ok;
}

bool BrokerI::GetSamplesBlockStartIndex(uint32 signalIndex,
        uint32 blockIndex,
        uint32 &startIndex) {
    bool ok = false;
    uint32 nSamplesBlocksSignal;
    if (!nSamplesBlocksPerSignal.Peek(signalIndex, nSamplesBlocksSignal)) {
        if (blockIndex < nSamplesBlocksSignal) {
            uint32 **signalIndexBlock = samplesBlocksList[signalIndex];
            startIndex = signalIndexBlock[blockIndex][0];
            ok = true;
        }
    }
    return ok;
}

bool BrokerI::GetSamplesBlockSize(uint32 signalIndex,
        uint32 blockIndex,
        uint32 &size) {
    bool ok = false;
    uint32 nSamplesBlocksSignal;
    if (!nSamplesBlocksPerSignal.Peek(signalIndex, nSamplesBlocksSignal)) {
        if (blockIndex < nSamplesBlocksSignal) {
            uint32 **signalIndexBlock = samplesBlocksList[signalIndex];
            size = signalIndexBlock[blockIndex][1];
            ok = true;
        }
    }
    return ok;
}

uint32 BrokerI::GetNumberOfSignals() {
    return gamSignalPointers.GetSize();
}

void *BrokerI::GetDataSourcePointer(uint32 signalIndex,
        uint32 buffer) {
    return (dataSourceSignalPointers[buffer])[signalIndex];
}

void *BrokerI::GetGAMSignalPointer(uint32 signalIndex) {
    void *ret = NULL_PTR(void*);

    if (gamSignalPointers.GetSize() > 0u) {
        uint32 numberOfStructures = signalIndexOfFirstMemberInStructure.GetSize();
        uint32 ptrIndex = 0u;

        bool go = true;
        // get the index of the structure where the offset in the nth position belongs
        for (uint32 i = 1u; (i < numberOfStructures) && (go); i++) {
            uint32 structIndex = 0u;
            go = (signalIndexOfFirstMemberInStructure.Peek(i, structIndex));
            if (go) {
                if (structIndex > signalIndex) {
                    go = false;
                }
                else {
                    ptrIndex++;
                }
            }
        }

        void *beginPtr = NULL_PTR(void*);
        if (gamSignalPointers.Peek(ptrIndex, beginPtr)) {
            uint32 offset = 0u;
            if (signalOffsetInsideGAMSignalI.Peek(signalIndex, offset)) {
                ret = &reinterpret_cast<char8 *>(beginPtr)[offset];
            }
        }
    }
    return ret;

}

uint32 BrokerI::GetTotalNumberOfSampleBlocks() {
    uint32 total = 0u;
    uint32 i = 0u;
    for (i = 0; i < nSamplesBlocksPerSignal.GetSize(); i++) {
        total += nSamplesBlocksPerSignal[i];
    }
    return total;
}

uint32 BrokerI::GetTotalNumberOfIndexBlocks() {
    uint32 total = 0u;
    uint32 i = 0u;
    for (i = 0; i < nIndexBlocksPerSignal.GetSize(); i++) {
        total += nIndexBlocksPerSignal[i];
    }
    return total;
}

void *BrokerI::GetGAMSignalPointer(uint32 signalIndex,
        uint32 sampleBlockNumber,
        uint32 indexBlockNumber) {
    uint32 indexBlockSize = 0u;
    void *gamSignalPointer = NULL_PTR(void *);
    bool ok = GetIndexBlockSize(signalIndex, indexBlockNumber, indexBlockSize);
    if (ok) {
        gamSignalPointer = GetGAMSignalPointer(signalIndex);
        gamSignalPointer += sampleBlockNumber * indexBlockSize;
    }

    return gamSignalPointer;
}

void *BrokerI::GetDataSourceSignalPointer(uint32 signalIndex,
        uint32 sampleBlockNumber,
        uint32 indexBlockNumber,
        uint32 buffer) {
    uint32 indexBlockStart = 0u;
    uint32 sampleBlockSize = 0u;

    void *dataSourceSignalPointer = NULL_PTR(void *);

    bool ok = GetIndexBlockStartIndex(signalIndex, indexBlockNumber, indexBlockStart);
    if (ok) {
        ok = GetSamplesBlockSize(signalIndex, sampleBlockNumber, sampleBlockSize);
    }
    if (ok) {
        dataSourceSignalPointer = GetDataSourcePointer(signalIndex, buffer);
        dataSourceSignalPointer += sampleBlockNumber * sampleBlockSize;
        dataSourceSignalPointer += indexBlockStart;
    }

    return dataSourceSignalPointer;
}

void *BrokerI::GetSignalByName(const char8 * name,
        uint32 &index) {
    uint32 numberOfSignals = Size();
    bool found = false;
    void* ret = NULL_PTR(void*);
    for (uint32 i = 0u; (i < numberOfSignals) && (!found); i++) {
        ReferenceT<GAMSignalI> gamSignal = gamSignalList.Get(i);
        if (gamSignal.IsValid()) {
            found = (StringHelper::Compare(name, gamSignal->GetName()) == 0);
            if (found) {
                ret = GetGAMSignalPointer(i);
                index = i;
            }
        }
    }
    return ret;
}
#endif

uint32 BrokerI::GetNumberOfCopies() {
    return numberOfCopies;
}

uint32 BrokerI::GetCopyByteSize(uint32 copyIdx) {
    return copyByteSize[copyIdx];
}

uint32 BrokerI::GetCopyOffset(uint32 copyIdx) {
    return copyOffset[copyIdx];
}

void *BrokerI::GetFunctionPointer(uint32 copyIdx) {
    return functionSignalPointers[copyIdx];
}

bool BrokerI::InitFunctionPointers(SignalDirection direction,
                                   ReferenceT<DataSourceI> dataSource,
                                   const char8 * const functionName) {
    //Find the function
    uint32 functionIdx;
    bool ret = dataSource->GetFunctionIndex(functionIdx, functionName);
    //For this function, how many signals in this direction
    uint32 functionNumberOfSignals = 0u;
    if (ret) {
        ret = dataSource->GetFunctionNumberOfSignals(direction, functionIdx, functionNumberOfSignals);
    }
    numberOfCopies = 0u;
    uint32 numberOfByteOffsets = 0u;
    uint32 n;
    for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
        if (ret) {
            ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
        }
        if (ret) {
            //One copy for each signal but each signal might want to copy several pieces (i.e. offsets)
            numberOfCopies += numberOfByteOffsets;
        }
    }
    if (numberOfCopies > 0u) {
        functionSignalPointers = new void*[numberOfCopies];
        copyByteSize = new uint32[numberOfCopies];
        copyOffset = new uint32[numberOfCopies];

        uint32 memoryOffset = 0u;
        uint32 c = 0u;
        //The same signal can be copied from different ranges. A CopyTableEntry is added for each signal range.
        for (n = 0u; (n < functionNumberOfSignals) && (ret); n++) {
            void *gamMemoryAddress = NULL_PTR(void *);
            ret = dataSource->GetFunctionSignalsAddress(direction, functionIdx, gamMemoryAddress);
            if (ret) {
                ret = (gamMemoryAddress != NULL_PTR(void *));
            }
            if (ret) {
                ret = dataSource->GetFunctionSignalNumberOfByteOffsets(direction, functionIdx, n, numberOfByteOffsets);
            }

            uint32 bo;
            //Take into account  different ranges for the same signal
            for (bo = 0u; (bo < numberOfByteOffsets) && (ret); bo++) {
                uint32 offsetStart;
                uint32 copySize;
                ret = dataSource->GetFunctionSignalByteOffsetInfo(direction, functionIdx, n, bo, offsetStart, copySize);
                if (ret) {
                    copyByteSize[c] = copySize;
                    copyOffset[c] = offsetStart;

                    char8 *gamMemoryCharAddress = reinterpret_cast<char8 *>(gamMemoryAddress);
                    gamMemoryCharAddress += memoryOffset;
                    functionSignalPointers[c] = reinterpret_cast<void *>(gamMemoryCharAddress);
                    memoryOffset += copySize;
                }
                c++;
            }
        }
    }
    return ret;
}

}
