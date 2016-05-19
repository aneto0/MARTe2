/**
 * @file MemoryMapDataSourceBroker.cpp
 * @brief Source file for class MemoryMapDataSourceBroker
 * @date 11/04/2016
 * @author Giuseppe Ferrò
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
 * the class MemoryMapDataSourceBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"
#include "MemoryMapBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapBroker::MemoryMapBroker() :
        BrokerI() {
    application = NULL_PTR(RealTimeApplication *);
    synchronized = false;
    finalised = false;
    syncIndex = 0u;
}

MemoryMapBroker::~MemoryMapBroker() {

}

bool MemoryMapBroker::AddSignal(ReferenceT<GAMSignalI> gamSignalIn,
                                void * const ptr) {

    bool ret = (ptr != NULL);
    if (ret) {
        uint32 currentSignalIndex = 0u;
        uint32 initialOffset = 0u;

        currentSignalIndex = GAMOffsets.GetSize();
        ret = AddSignalPrivate(gamSignalIn, initialOffset, 0u);
        if (ret) {
            ret = chunkIndex.Add(currentSignalIndex);
            if (ret) {
                // adds the memory start pointer
                ret = beginPointers.Add(ptr);
                if (ret) {
                    ret = Insert(gamSignalIn);
                }
            }
        }
        else {
            // error ! remove eventual added things to be consistent
            uint32 last = 0u;
            bool ok = true;
            if (chunkIndex.GetSize() > 0u) {
                if (chunkIndex.Peek((chunkIndex.GetSize() - 1u), last)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to chunkIndex.Peek. This will lead to inconsistencies in the broker memory");
                }
            }

            while ((dataSourcesVars.GetSize() > last) && (ok)) {
                if (!dataSourcesVars.Remove(dataSourcesVars.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from dataSourcesVars. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((GAMOffsets.GetSize() > last) && (ok)) {
                if (!GAMOffsets.Remove(GAMOffsets.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from GAMOffsets. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((DSPointers[0].GetSize() > last) && (ok)) {
                if (!DSPointers[0].Remove(DSPointers[0].GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from DSPointers[0]. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((DSPointers[1].GetSize() > last) && (ok)) {
                if (!DSPointers[1].Remove(DSPointers[1].GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from DSPointers[1]. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            while ((numberOfCyclesPerVar.GetSize() > last) && (ok)) {
                if (!numberOfCyclesPerVar.Remove(numberOfCyclesPerVar.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError,
                                 "Failed to remove from numberOfCyclesPerVar. This will lead to inconsistencies in the broker memory");
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
            while ((blockParamRows.GetSize() > last) && (ok)) {
                if (!blockParamRows.Remove(blockParamRows.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from blockParamRows. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
            /*while ((samplesParamRows.GetSize() > last) && (ok)) {
                if (!samplesParamRows.Remove(samplesParamRows.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from samplesParamRows. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }*/
            while ((nSamplesList.GetSize() > last) && (ok)) {
                if (!nSamplesList.Remove(nSamplesList.GetSize() - 1u)) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to remove from nSamplesList. This will lead to inconsistencies in the broker memory");
                    ok = false;
                }
            }
        }
    }
    return ret;
}

bool MemoryMapBroker::AddSignalPrivate(ReferenceT<GAMSignalI> gamSignalIn,
                                       uint32 initialOffset,
                                       uint32 offset) {

    bool ret = (application != NULL);

    if (ret) {
        ReferenceT<GAMSignalI> gamSignal = gamSignalIn;
        ret = gamSignal.IsValid();
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
                    ret = (intro != NULL);
                    if (ret) {
                        /*lint -e{613} NULL pointer checking done before entering here */
                        varSize = intro->GetClassSize();
                        // unsupported multi-dimensional
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
                                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                                "Member %s of %s  with type %s does not matches with the introspection data",
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
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not introspectable", gamSignal->GetType())
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", gamSignal->GetType())
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
                    ret = dataSourcesVars.Add(dataSourceSignal.operator ->());
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
            REPORT_ERROR(ErrorManagement::FatalError, "Invalid definition in input");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Application not set");
    }

    return ret;

}

void *MemoryMapBroker::GetSignal(const uint32 n) {
    void *ret = NULL_PTR(void*);
    void* beginPtr = NULL_PTR(void*);
    if (finalised) {
        if (beginPointers.Peek(n, beginPtr)) {
            uint32 index = 0u;
            if (chunkIndex.Peek(n, index)) {
                uint32 offset = 0u;
                if (GAMOffsets.Peek(index, offset)) {
                    ret = &reinterpret_cast<char8 *>(beginPtr)[offset];
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The object definition is not finalised");
    }
    return ret;
}

void *MemoryMapBroker::GetSignalByName(const char8 * name,
                                       uint32 &index) {
    uint32 numberOfSignals = Size();
    bool found = false;
    void* ret = NULL_PTR(void*);
    for (uint32 i = 0u; (i < numberOfSignals) && (!found); i++) {
        ReferenceT<GAMSignalI> def = Get(i);
        if (def.IsValid()) {
            found = (StringHelper::Compare(name, def->GetName()) == 0);
            if (found) {
                ret = GetSignal(i);
                index = i;
            }
        }
    }
    return ret;
}

uint32 MemoryMapBroker::GetSignalNumberOfSamples(const uint32 n) {
    uint32 nSamples = 0u;
    if (nSamplesList.Peek(n, nSamples)) {

    }
    return nSamples;
}

void *MemoryMapBroker::GetMemoryPointerPrivate(const uint32 n) const {

    void *ret = NULL_PTR(void*);

    if (beginPointers.GetSize() > 0u) {
        uint32 numberOfStructures = chunkIndex.GetSize();
        uint32 ptrIndex = 0u;

        bool go = true;
        // get the index of the structure where the offset in the nth position belongs
        for (uint32 i = 1u; (i < numberOfStructures) && (go); i++) {
            uint32 structIndex = 0u;
            go = (chunkIndex.Peek(i, structIndex));
            if (go) {
                if (structIndex > n) {
                    go = false;
                }
                else {
                    ptrIndex++;
                }
            }
        }

        void *beginPtr = NULL_PTR(void*);
        if (beginPointers.Peek(ptrIndex, beginPtr)) {
            uint32 offset = 0u;
            if (GAMOffsets.Peek(n, offset)) {
                ret = &reinterpret_cast<char8 *>(beginPtr)[offset];
            }
        }
    }
    return ret;
}

bool MemoryMapBroker::Finalise() {

    bool ret = true;

// refresh the pointers
    uint32 numberOfStructures = beginPointers.GetSize();
    for (uint32 i = 0u; (i < numberOfStructures) && (ret); i++) {
        void* ptr = NULL_PTR(void*);
        ret = (beginPointers.Peek(i, ptr));
        if (ret) {
            if (ptr == NULL) {
                ptr = memory.GetMemoryStart();
                ret = beginPointers.Set(i, ptr);
            }
        }
    }

    if (ret) {
        // fill the gam pointers
        uint32 numberOfPointers = GAMOffsets.GetSize();
        for (uint32 i = 0u; (i < numberOfPointers) && (ret); i++) {
            void* ptr = GetMemoryPointerPrivate(i);
            ret = (ptr != NULL);
            if (ret) {
                ret = GAMPointers.Add(ptr);
            }
        }
    }

// set as finalised
    finalised = ret;
    return ret;
}

bool MemoryMapBroker::IsSync() const {
    return synchronized;
}

bool MemoryMapBroker::Verify(ReferenceT<GAMSignalI> gamSignal,
                             uint32 &typeSize,
                             ReferenceT<DataSource> &dataSource,
                             ReferenceT<DataSourceSignal> &dataSourceSignal) {

    const char8 *path = NULL_PTR(char8 *);

    bool ok = gamSignal.IsValid();
    if (ok) {
        path = gamSignal->GetPath();
        ok = (path != NULL);
    }

    if (ok) {
        ok = (application != NULL);
    }
    if (ok) {
        // find the data source signal
        StreamString allPath = "Data.";
        StreamString pathNoData = path;
        allPath += pathNoData;
        dataSourceSignal = application->Find(allPath.Buffer());
        ok = dataSourceSignal.IsValid();
    }
    if (ok) {
        // check type compatibility
        ok = (StringHelper::Compare(dataSourceSignal->GetType(), gamSignal->GetType()) == 0);
    }

    if (ok) {
        StreamString dataSourcePath;
        //The DataSource nodes are declared just under Data.
        char8 ignored;
        pathNoData.GetToken(dataSourcePath, ".", ignored, "");
        dataSourcePath = "Data." + dataSourcePath;
        dataSource = application->Find(dataSourcePath.Buffer());
        ok = dataSource.IsValid();
    }

    if (ok) {
        ok = dataSource->IsSupportedBroker(*this);
    }

    if (ok) {
        ok = (SetBlockParams(gamSignal, dataSourceSignal, typeSize));
    }

    return ok;

}

bool MemoryMapBroker::SetBlockParams(ReferenceT<GAMSignalI> gamSignal,
                                     ReferenceT<DataSourceSignal> dataSourceSignal,
                                     uint32 &typeSize) {

    bool ret = true;

    uint32 nOfDSElements = dataSourceSignal->GetNumberOfElements();

    uint32 numberOfSamples = dataSourceSignal->GetNumberOfSamples();
    int32 numberOfCycles = gamSignal->GetCycles();

    printf("\n#cycles=%d\n", numberOfCycles);
    uint32 numberOfGAMSamples = 1u;
    // checks if it is already sync
    if (synchronized) {
        ret = (numberOfCycles == 0);
    }
    else {
        synchronized = (numberOfCycles > 0);
    }

    // the number of data source elements can be different than the gam signal one
    if (ret) {
        // the minimum is one
        if (numberOfCycles == 0u) {
            numberOfCycles++;
        }

        ReferenceT<GAMSampledSignal> gamSampledSignal = gamSignal;
        uint32 **samplesM = NULL_PTR(uint32**);
        uint32 **blocksM = NULL_PTR(uint32**);
        uint32 nBlockRows = 0u;
        uint32 samplesRows = 0u;

        bool isGAMSampledSignal = gamSampledSignal.IsValid();

        // check if the gam wants to read samples
        if (isGAMSampledSignal) {

            StreamString confSamples = "SignalSamples=";
            confSamples += gamSampledSignal->GetSamples();
            bool hasSamplesDefined = (confSamples != "SignalSamples=");

            // if there is the field
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
                            samplesRows = at.GetNumberOfElements(1u);
                            samplesM = new uint32*[samplesRows];
                            for (uint32 i = 0u; i < samplesRows; i++) {
                                samplesM[i] = new uint32[nCols];
                            }
                            Matrix<uint32> paramListMatrix(samplesM, samplesRows, nCols);
                            ret = (cdb.Read("SignalSamples", paramListMatrix));
                            if (ret) {

                                // save begin index and size
                                numberOfGAMSamples = 0u;
                                uint32 maxSamplesIndex = (numberOfSamples - 1u);
                                for (uint32 i = 0u; (i < samplesRows) && (ret); i++) {
                                    ret = (samplesM[i][2] > 0u);
                                    if (ret) {
                                        // check that the first is minor than the third
                                        ret = (samplesM[i][0] <= samplesM[i][2]);
                                        if (ret) {
                                            // check that the second is minor than the third
                                            ret = (samplesM[i][1] <= samplesM[i][2]);
                                            if (ret) {
                                                // check that the first is greater than the second
                                                ret = (samplesM[i][1] <= samplesM[i][0]);
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
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                    "The field \"Samples\" of %s must be declared as a nx3 matrix {{a,b,c}...} with b<=a<=c", gamSignal->GetName())
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

uint32 MemoryMapBroker::GetNumberOfSignals() {
    return Size();
}

}
