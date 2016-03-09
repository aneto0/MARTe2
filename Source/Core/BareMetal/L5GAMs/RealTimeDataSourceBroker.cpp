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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

RealTimeDataSourceBroker::RealTimeDataSourceBroker(){

}

void RealTimeDataSourceBroker::SetApplication(ReferenceT<RealTimeApplication> rtApp) {
    application = rtApp;
}

bool RealTimeDataSourceBroker::AddVariable(ReferenceT<RealTimeDataDefI> def,
                                                 void* ptr) {

    bool ret = application.IsValid();
    if(ret) {
        ReferenceT<RealTimeDataSource> data;

        for(uint32 i=0u; i<application->Size(); i++) {
            data=application->Get(i);
            if(StringHelper::Compare(data->GetName(),"+Data")==0) {
                break;
            }
        }

        ret=data.IsValid();
        if(ret) {
            const char8* typeName = def->GetType();

            // case structured
            TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(typeName);
            if (typeDes == InvalidType) {

                const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(typeName);
                ret=(item != NULL);
                if (ret) {
                    const Introspection *intro = item->GetIntrospection();
                    ret=(intro != NULL);
                    if (ret) {
                        uint32 varSize=intro->GetClassSize();
                        if(ptr==NULL) {
                            ptr=memory.Add(varSize);
                        }

                        uint32 numberOfMembers = def->Size();
                        // final type
                        if (numberOfMembers == 0u) {
                            const char8* path=def->GetPath();
                            ReferenceT<RealTimeDataSourceDef> dsDef=data->Find(path);
                            ret=dsDef.IsValid();
                            if(ret) {
                                void **dsPointer=dsDef->GetDataSourcePointer();
                                ret=(dsPointer!=NULL);
                                if(ret) {
                                    // add the data source pointer
                                    ret=DSPointers.Add(dsPointer);
                                }
                                if(ret) {
                                    // add the GAM pointer
                                    ret=GAMPointers.Add(ptr);
                                }
                                if(ret) {
                                    ret=sizes.Add(varSize);
                                }
                            }
                            else {
                                //TODO data source not found
                            }
                        }
                        else {
                            uint32 numberOfIntroMembers=intro->GetNumberOfMembers();
                            // the variables will be added in the same order of the introspection
                            for (uint32 i = 0u; (i < numberOfIntroMembers) && (ret); i++) {
                                for(uint32 j=0u; (j<numberOfMembers) && (ret); j++) {
                                    ReferenceT<RealTimeDataDefI> subDef = def->Get(j);
                                    // if the member name matches
                                    if(StringHelper::Compare((*intro)[i].GetMemberName(), def->GetName())==0) {
                                        char8 *pointablePtr=reinterpret_cast<char8*>(ptr);
                                        ret=AddVariable(subDef, &pointablePtr[(*intro)[i].GetMemberByteOffset()]);
                                        break;
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
                uint32 varSize=(typeDes.numberOfBits+7u)/8u;
                char8 *ptr=reinterpret_cast<char8*>(memory.Add(varSize));
                // add the pointer
                ret=GAMPointers.Add(ptr);
                if(ret) {
                    sizes.Add(varSize);
                }
            }

            if(ret) {
                ret=chunkIndex.Add(GAMPointers.GetSize());
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
    uint32 index=0u;
    if(chunkIndex.Peek(i, index)) {
        if(!GAMPointers.Peek(index , ret)) {
            //TODO
        }
    }
    return ret;
}
}
