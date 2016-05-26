/**
 * @file MemoryMapDataSourceBroker.cpp
 * @brief Source file for class MemoryMapDataSourceBroker
 * @date 11/apr/2016
 * @author pc
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

#include "MemoryMapDataSourceBroker.h"
#include "DataSourceContainer.h"
#include "AdvancedErrorManagement.h"
#include "GAMGenericSignal.h"
#include "GAMSampledSignal.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapDataSourceBroker::MemoryMapDataSourceBroker() :
        DataSourceBrokerI() {
    application = NULL_PTR(RealTimeApplication *);
    synchronized = false;
    finalised = false;
    syncIndex = 0u;
}

MemoryMapDataSourceBroker::~MemoryMapDataSourceBroker() {
    uint32 numberOfBlocks = blockParamList.GetSize();
    uint32 ** element = NULL_PTR(uint32 **);

    for (uint32 i = 0u; i < numberOfBlocks; i++) {
        if (blockParamList.Peek(i, element)) {
            if (element != NULL) {
                uint32 nRows = 0u;
                if (blockParamRows.Peek(i, nRows)) {
                    for (uint32 k = 0u; k < nRows; k++) {
                        delete[] element[k];
                    }
                }
                delete[] element;
            }
        }
    }
    numberOfBlocks = samplesParamList.GetSize();
    for (uint32 i = 0u; i < numberOfBlocks; i++) {
        if (samplesParamList.Peek(i, element)) {
            if (element != NULL) {
                uint32 nRows = 0u;
                if (samplesParamRows.Peek(i, nRows)) {
                    for (uint32 k = 0u; k < nRows; k++) {
                        delete[] element[k];
                    }
                }
                delete[] element;
            }
        }
    }
}

void MemoryMapDataSourceBroker::SetApplication(RealTimeApplication &rtApp) {
    application = &rtApp;
}

bool MemoryMapDataSourceBroker::AddSignal(Reference def,
                                          void * const ptr) {

    //REPORT_ERROR(ErrorManagement::FatalError, "Adding Signal To MMap");

    bool ret = true;
    bool allocate = true;
    uint32 currentSignalIndex = 0u;
    uint32 initialOffset = 0u;
    if (ptr != NULL) {
        allocate = false;
    }
    else {
        // we do not know yet
        initialOffset = memory.GetMemorySize();
    }
    if (ret) {
        if (ret) {
            currentSignalIndex = GAMOffsets.GetSize();
            ret = AddSignalPrivate(def, initialOffset, 0u, allocate);
        }
    }
    if (ret) {
        ret = chunkIndex.Add(currentSignalIndex);
        //printf("\nsignal index = %d\n", currentSignalIndex);
        if (ret) {
            // adds the memory start pointer
            ret = beginPointers.Add(ptr);
            if (ret) {
                ret = Insert(def);
            }
        }
    }
    else {
        // error ! remove eventual added things to be consistent
        uint32 last = 0u;
        if (chunkIndex.GetSize() > 0u) {
            if (chunkIndex.Peek((chunkIndex.GetSize() - 1u), last)) {
                //impossible failure (come on...)
            }
        }

        while (dataSourcesVars.GetSize() > last) {
            if (!dataSourcesVars.Remove(dataSourcesVars.GetSize() - 1u)) {
                //impossible failure (come on...)
            }
        }
        while (GAMOffsets.GetSize() > last) {
            if (!GAMOffsets.Remove(GAMOffsets.GetSize() - 1u)) {

            }
        }
        while (DSPointers[0].GetSize() > last) {
            if (!DSPointers[0].Remove(DSPointers[0].GetSize() - 1u)) {

            }
        }
        while (DSPointers[1].GetSize() > last) {
            if (!DSPointers[1].Remove(DSPointers[1].GetSize() - 1u)) {

            }
        }
        while (numberOfCyclesPerVar.GetSize() > last) {
            if (!numberOfCyclesPerVar.Remove(numberOfCyclesPerVar.GetSize() - 1u)) {

            }
        }
        while (blockParamList.GetSize() > last) {
            if (!blockParamList.Remove(blockParamList.GetSize() - 1u)) {

            }
        }
        while (samplesParamList.GetSize() > last) {
            if (!samplesParamList.Remove(samplesParamList.GetSize() - 1u)) {

            }
        }
        while (blockParamRows.GetSize() > last) {
            if (!blockParamRows.Remove(blockParamRows.GetSize() - 1u)) {

            }
        }
        while (samplesParamRows.GetSize() > last) {
            if (!samplesParamRows.Remove(samplesParamRows.GetSize() - 1u)) {

            }
        }
        while (nSamplesList.GetSize() > last) {
            if (!nSamplesList.Remove(nSamplesList.GetSize() - 1u)) {

            }
        }

        while (gamSignalSizes.GetSize() > last) {
            if (!gamSignalSizes.Remove(gamSignalSizes.GetSize() - 1u)) {

            }
        }

    }
  /*  if (ret) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Added Signal %s To MMap", def->GetName())
    }
*/
    return ret;
}

bool MemoryMapDataSourceBroker::AddSignalPrivate(Reference defIn,
                                                 uint32 initialOffset,
                                                 uint32 offset,
                                                 bool allocate) {

    bool ret = (application != NULL);

    if (ret) {
        ReferenceT < GAMSignalI > def = defIn;
        ret = def.IsValid();
        if (ret) {
            const char8* typeName = def->GetType();

            // case structured
            TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);

            uint32 varSize = 0u;
            uint32 numberOfMembers = def->Size();

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
                        ret = (def->GetNumberOfDimensions() == 0u);

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
                                        ReferenceT < GAMGenericSignal > subDef = def->Get(j);
                                        ret = subDef.IsValid();
                                        if (ret) {
                                            // if the member name matches
                                            const char8 *defMemberName = subDef->GetName();
                                            ret = (defMemberName != NULL);
                                            if (ret) {
                                                /*lint -e{613} NULL pointer checking done before entering here */
                                                if (StringHelper::Compare(introEntry.GetMemberName(), defMemberName) == 0) {
                                                    /*lint -e{613} if (pointablePtr == NULL) it just returns the offset*/
                                                    ret = AddSignalPrivate(subDef, initialOffset, introEntry.GetMemberByteOffset(), false);
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
                                                                introEntry.GetMemberName(), def->GetName(), def->GetType())
                                    }
                                }
                            }
                        }
                        else {
                            // this should never happen if we call before DataSource.Allocate()
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Unsupported multi-dimensional structure %s", def->GetName())
                        }
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not introspectable", def->GetType())
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", def->GetType())
                }

            }
            // basic type
            else {
                varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
            }

            // same code for structured and basic
            if ((ret) && (numberOfMembers == 0u)) {

                // the size is changed by this function (* #samples * #cycles * #dimension)
                ReferenceT < DataSourceSignalI > dsDef = Verify(defIn, varSize);
                ret = dsDef.IsValid();
              //  REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Verify = %d, varSize = %d", ret, varSize)

                if (ret) {
                    ret = dataSourcesVars.Add(dsDef.operator ->());
                    if (ret) {
                        void **dsPointer0 = dsDef->GetDataSourcePointer(0u);
                        void **dsPointer1 = dsDef->GetDataSourcePointer(1u);

                        ret = ((dsPointer0 != NULL) && (dsPointer1 != NULL));
                        if (ret) {
                            // add the data source pointer
                            ret = DSPointers[0].Add(dsPointer0);
                            if (ret) {
                                ret = DSPointers[1].Add(dsPointer1);
                                if (ret) {
                                    // add the GAM pointer
                                    uint32 finalOffset = offset + initialOffset;
                                    ret = GAMOffsets.Add(finalOffset);
                                    /*   REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Added offset %d rel offset %d of %s in %s", finalOffset, offset,
                                     def->GetName(), def->GetPath())
                                     */
                                    //printf("\nAdded offset %d rel offset %d of %s in %s\n", finalOffset, offset, def->GetName(), def->GetPath());
                                }
                            }
                        }
                    }
                }
            }

            // only in the first recursion level
            if (allocate) {
                //              REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Allocating variable %s size=%d of %s", def->GetName(), varSize, def->GetPath())

                // allocate the memory
                // it must be allocated only if ptr in input is NULL
                if (ret) {

                    // the last thing to be done
                    ret = memory.Add(varSize, offset);

                    if (ret) {
                        ret=gamSignalSizes.Add(varSize);
                    }
                    //   REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Allocated variable %s size=%d of %s", def->GetName(), varSize, def->GetPath())
                    //printf("\nallocated variable %s size=%d of %s\n", def->GetName(), varSize, def->GetPath());
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

void *MemoryMapDataSourceBroker::GetSignal(const uint32 n) {
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

void *MemoryMapDataSourceBroker::GetSignalByName(const char8 * name,
                                                 uint32 &index) {
    uint32 numberOfSignals = Size();
    bool found = false;
    void* ret = NULL_PTR(void*);
    for (uint32 i = 0u; (i < numberOfSignals) && (!found); i++) {
        ReferenceT < GAMSignalI > def = Get(i);
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

uint32 MemoryMapDataSourceBroker::GetSignalNumberOfSamples(const uint32 n) {
    uint32 nSamples = 0u;
    if (nSamplesList.Peek(n, nSamples)) {

    }
    return nSamples;
}

uint32 MemoryMapDataSourceBroker::GetSignalSize(const uint32 n){

    uint32 signalSize = 0u;
    if (gamSignalSizes.Peek(n, signalSize)) {

    }
    return signalSize;
}



void *MemoryMapDataSourceBroker::GetMemoryPointerPrivate(const uint32 n) const {

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

bool MemoryMapDataSourceBroker::Finalise() {

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

bool MemoryMapDataSourceBroker::IsSync() const {
    return synchronized;
}

Reference MemoryMapDataSourceBroker::Verify(Reference defIn,
                                            uint32 &typeSize) {

    ReferenceT < GAMSignalI > def = defIn;
    ReferenceT < DataSourceSignalI > ret;

    const char8 *path = def->GetPath();
    if (path != NULL) {
        bool ok = (application != NULL);
        if (ok) {
            // find the data source signal
            StreamString allPath = "Data.";
            allPath += path;
            ReferenceT < DataSourceSignalI > dsDef = application->Find(allPath.Buffer());
            ok = dsDef.IsValid();
            if (ok) {
                ok = dsDef->IsSupportedBroker(*this);
                if (ok) {
                    // check type compatibility
                    ok = (StringHelper::Compare(dsDef->GetType(), def->GetType()) == 0);

                    if (ok) {
                        if (SetBlockParams(def, dsDef, typeSize)) {
                            ret = dsDef;
                        }
                        else {
                            REPORT_ERROR(ErrorManagement::FatalError, "Failed SetBlockParams");
                        }
                    }
                    else {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "%s %s type mismatch", dsDef->GetType(), def->GetType())

                    }
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "%s not found", allPath.Buffer())
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "NULL App");

        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "NULL path");

    }

    return ret;

}

bool MemoryMapDataSourceBroker::SetBlockParams(Reference defIn,
                                               Reference dsDefIn,
                                               uint32 &typeSize) {

    // this function is called internally (no segmentation faults should happen)
    ReferenceT < GAMSignalI > def = defIn;
    ReferenceT < DataSourceSignalI > dsDef = dsDefIn;
    bool ret = true;

    uint32 nOfDSElements = 0u;
    nOfDSElements += dsDef->GetNumberOfElements();

    uint32 numberOfSamples = dsDef->GetNumberOfSamples();
    int32 numberOfCycles = def->GetCycles();
    //printf("\n#cycles=%d\n", numberOfCycles);
    uint32 numberOfGAMSamples = 1u;
    // checks if it is already sync
    if (synchronized) {
        ret = (numberOfCycles == 0);
    }
    else {
        synchronized = (numberOfCycles > 0);
    }

    // the number of ds elements can be different than the gam signal one
    if (ret) {
        // the minimum is one
        if (numberOfCycles == 0u) {
            numberOfCycles++;
        }

        ReferenceT < GAMSampledSignal > sampledDef = def;
        uint32 **samplesM = NULL_PTR(uint32**);
        uint32 **blocksM = NULL_PTR(uint32**);
        uint32 nBlockRows = 0u;
        uint32 samplesRows = 0u;

        bool init = sampledDef.IsValid();

        // check if the gam wants to read samples
        if (init) {

            StreamString confSamples = "SignalSamples=";
            confSamples += sampledDef->GetSamples();
            init = (confSamples != "SignalSamples=");

            // if there is the field
            if (init) {
                confSamples.Seek(0ULL);
                ConfigurationDatabase cdb;
                StandardParser parser(confSamples, cdb);
                init = parser.Parse();
                // if not get the last sample by default
                if (init) {

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
                            Matrix < uint32 > paramListMatrix(samplesM, samplesRows, nCols);
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
                                                    //printf("\nsample beg=%d\n", samplesM[i][0]);
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
                                                    "The field \"Samples\" of %s must declare a nx3 matrix {{a,b,c}...} with b<=a<=c", def->GetName())
                        }
                    }
                }
            }
        }
        if (!init) {
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
            for (uint32 k = 0u; k < def->GetNumberOfDimensions(); k++) {
                nOfGAMElements *= def->GetNumberOfElements(k);
            }

            StreamString confString = "SignalBlocks=";
            confString += def->GetOperation();

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
                            Matrix < uint32 > paramListMatrix(blocksM, nBlockRows, nCols);
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
                                    if (testSize < nOfGAMElements) {
                                        REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
                                                                "The GAM signal %s has more elements than the the effective read-write ones", def->GetName())
                                    }
                                }
                            }
                        }
                        else {
                            // def can be added to another broker?
                            REPORT_ERROR_PARAMETERS(
                                    ErrorManagement::RecoverableError,
                                    "The field \"Operation\" of %s must declare a nx2 matrix {{a,b}...} with a,b<DataSourceSignal::GetNumberOfElements(*)",
                                    def->GetName())
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
                    REPORT_ERROR_PARAMETERS(ErrorManagement::RecoverableError, "Failure of StaticList::Add for GAM signal %s", def->GetName())
                }

                //printf("\n%d %d %d\n", typeSize, numberOfGAMSamples, nOfGAMElements);

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
                                def->GetName())
    }
    return ret;
}

uint32 MemoryMapDataSourceBroker::GetNumberOfSignals() {
    return Size();
}

bool MemoryMapDataSourceBroker::Read(const uint8 activeDataSourceBuffer,
                                     const TimeoutType &timeout) {
    return false;
}

bool MemoryMapDataSourceBroker::Write(const uint8 activeDataSourceBuffer,
                                      const TimeoutType &timeout) {
    return false;
}

}
