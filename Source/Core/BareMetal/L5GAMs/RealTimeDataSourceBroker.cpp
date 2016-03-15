/**
 * @file RealTimeDataSourceBroker.cpp
 * @brief Source file for class RealTimeDataSourceBroker
 * @date 09/mar/2016
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
 * the class RealTimeDataSourceBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceBroker.h"
#include "RealTimeDataSource.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataSourceBroker::RealTimeDataSourceBroker() {

    finalised = false;
}

void RealTimeDataSourceBroker::SetApplication(ReferenceT<RealTimeApplication> rtApp) {
    application = rtApp;
}

bool RealTimeDataSourceBroker::AddVariable(ReferenceT<RealTimeDataDefI> def,
                                           void* ptr) {

    bool ret = false;
    void *memStart = NULL;
    if (ptr != NULL) {
        // ??
        ret = beginPointers.Add(ptr);
        memStart = ptr;
        // printf("\nadd static pointer %llx\n", (uintp) ptr);
    }
    else {
        // we do not know yet
        memStart = memory.GetMemoryStart();
        ret = beginPointers.Add(NULL);
    }
    if (ret) {
        ret = chunkIndex.Add(GAMOffsets.GetSize());
        if (ret) {
            ret = AddVariablePrivate(def, ptr, memStart);
        }
    }

    return ret;
}

bool RealTimeDataSourceBroker::AddVariablePrivate(ReferenceT<RealTimeDataDefI> def,
                                                  void* ptr,
                                                  void* memStart) {

    bool ret = application.IsValid();
    if (ret) {
        ReferenceT<RealTimeDataSource> data;

        ret = false;
        for (uint32 i = 0u; (i < application->Size()) && (!ret); i++) {
            data = application->Get(i);
            if (data.IsValid()) {
                if (StringHelper::Compare(data->GetName(), "+Data") == 0) {
                    ret = true;
                }
            }
        }

        if (ret) {
            ret = def.IsValid();
            if (ret) {
                const char8* typeName = def->GetType();

                // case structured
                TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);

                // offset used in recursion
                uint32 offset = 0u;
                if (ptr != NULL) {
                    offset = static_cast<uint32>((uintp) ptr - (uintp) memStart);
                }

                uint32 varSize = 0u;
                uint32 numberOfMembers = def->Size();

                if (typeDes == InvalidType) {

                    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
                    ret = (item != NULL);
                    if (ret) {
                        const Introspection *intro = item->GetIntrospection();
                        ret = (intro != NULL);
                        if (ret) {
                            varSize = intro->GetClassSize();
                            // unsupported multi-dimensional
                            ret = (def->GetNumberOfDimensions() == 0u);
                            if (ret) {
                                if (ptr == NULL) {
                                    ret = memory.Add(varSize, offset);
                                    ptr = memory.GetPointer(offset);
                                    memStart = memory.GetMemoryStart();
                                }
                            }
                            else{
                                //TODO Warning unsupported multidimensional for structures
                            }

                            if (ret) {
                                // final type
                                if (numberOfMembers > 0u) {
                                    uint32 numberOfIntroMembers = intro->GetNumberOfMembers();

                                    // the variables will be added in the same order of the introspection
                                    for (uint32 i = 0u; (i < numberOfIntroMembers) && (ret); i++) {
                                        bool found = false;
                                        for (uint32 j = 0u; (j < numberOfMembers) && (ret) && (!found); j++) {
                                            ReferenceT<RealTimeDataDefI> subDef = def->Get(j);
                                            ret = def.IsValid();
                                            if (ret) {
                                                // if the member name matches
                                                const char8 *defMemberName = subDef->GetName();
                                                ret = (defMemberName != NULL);
                                                if (ret) {
                                                    if (StringHelper::Compare((*intro)[i].GetMemberName(), &defMemberName[1]) == 0) {
                                                        printf("\n insert %s path = %s\n", &defMemberName[1], subDef->GetPath());
                                                        char8 *pointablePtr = reinterpret_cast<char8*>(ptr);
                                                        ret = AddVariablePrivate(subDef, &pointablePtr[(*intro)[i].GetMemberByteOffset()], memStart);
                                                        found = true;
                                                    }
                                                }
                                            }
                                        }
                                        if (!found) {
                                            ret = false;
                                            //TODO not found
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            //TODO not introspectable
                        }
                    }
                    else {
                        //TODO not registered
                    }

                }
                // basic type
                else {
                    varSize = (typeDes.numberOfBits + 7u) / 8u;
                    // consider multi-dimensional
                    uint8 numberOfDimensions = def->GetNumberOfDimensions();
                    for (uint32 k = 0u; k < numberOfDimensions; k++) {
                        varSize *= def->GetNumberOfElements(k);
                    }
                    printf("\noffset is %d size is %d\n", offset, varSize);
                    if (ptr == NULL) {
                        ret = memory.Add(varSize, offset);
                    }
                }

                // same code for structured and basic
                if ((ret) && (numberOfMembers == 0u)) {
                    const char8* path = def->GetPath();
                    printf("\npath is %s name is %s\n", def->GetPath(), def->GetName());
                    ReferenceT<RealTimeDataSourceDef> dsDef = data->Find(path);
                    ret = dsDef.IsValid();
                    if (ret) {
                        void **dsPointer0 = dsDef->GetDataSourcePointer(0);
                        void **dsPointer1 = dsDef->GetDataSourcePointer(1);

                        ret = ((dsPointer0 != NULL) && (dsPointer1 != NULL));
                        if (ret) {
                            // add the data source pointer
                            ret = DSPointers[0].Add(dsPointer0);
                            if (ret) {
                                ret = DSPointers[1].Add(dsPointer1);
                            }
                        }
                        if (ret) {
                            // add the GAM pointer
                            ret = GAMOffsets.Add(offset);
                        }
                        if (ret) {

                            ret = sizes.Add(varSize);
                        }

                        printf("\ntest...%d %d\n", *(uint32*) (*dsPointer0), *(uint32*) (*dsPointer1));
                    }
                }
            }
            else {
                //TODO invalid def
            }
        }
        else {
            //TODO invalid data
        }
    }
    else {
        //TODO invalid application
    }
    return ret;

}

void *RealTimeDataSourceBroker::GetData(uint32 i) {
    void *ret = NULL;
    void* beginPtr = NULL;
    if (finalised) {
        if (beginPointers.Peek(i, beginPtr)) {
            uint32 index = 0u;
            if (chunkIndex.Peek(i, index)) {
                uint32 offset = 0u;
                if (GAMOffsets.Peek(index, offset)) {
                    printf("Get Data: offset=%d", offset);
                    ret = &reinterpret_cast<char8 *>(beginPtr)[offset];
                    //printf("\nget data %llx offset=%d, chunkIndex=%d ?? %d\n", (uintp) ret, offset, index, GAMOffsets.GetSize());
                }
            }
        }
    }
    else {
        //TODO Not Finalised!!!
    }
    return ret;
}

void *RealTimeDataSourceBroker::GetMemoryPointer(uint32 n) {
    void *ret = NULL;

    if (finalised) {
        if (!GAMPointers.Peek(n, ret)) {
            //TODO
        }
    }
    else {
        //TODO Not Finalised!
    }
    return ret;
}

void *RealTimeDataSourceBroker::GetMemoryPointerPrivate(uint32 n) {

    void *ret = NULL;

    if (beginPointers.GetSize() > 0u) {
        uint32 numberOfStructures = chunkIndex.GetSize();
        uint32 ptrIndex = 0u;

        bool go = true;
        // get the index of the structure where the offset in the nth position belongs
        for (uint32 i = 1u; (i < numberOfStructures) && (go); i++) {
            uint32 structIndex = 0u;
            go = (chunkIndex.Peek(i, structIndex));
            if (go) {
                printf("\nstruct nPointers = %d\n", structIndex);
                if (structIndex > n) {
                    go = false;
                }
                else {
                    ptrIndex++;
                }
            }
        }

        printf("\n%s: begIndex=%d nStructs=%d\n", GetName(), ptrIndex, numberOfStructures);
        void *beginPtr = NULL;
        if (beginPointers.Peek(ptrIndex, beginPtr)) {
            uint32 offset = 0u;
            if (GAMOffsets.Peek(n, offset)) {
                ret = &reinterpret_cast<char8 *>(beginPtr)[offset];
            }
        }
    }
    return ret;
}

bool RealTimeDataSourceBroker::Finalise() {

    bool ret = true;

    printf("\nFinalising %s\n", GetName());

    // refresh the pointers
    uint32 numberOfStructures = beginPointers.GetSize();
    for (uint32 i = 0u; (i < numberOfStructures) && (ret); i++) {
        void* ptr = NULL;
        ret = (beginPointers.Peek(i, ptr));
        if (ret) {
            if (ptr == NULL) {
                printf("\nset memory area\n");
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

}
