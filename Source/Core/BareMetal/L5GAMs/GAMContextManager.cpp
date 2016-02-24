/**
 * @file GAMContextManager.cpp
 * @brief Source file for class GAMContextManager
 * @date 23/02/2016
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
 * the class GAMContextManager (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMContextManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

const uint32 contextArrayGranularity = 16u;
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMContextManager::GAMContextManager() {
    items = NULL;
    numberOfItems=0u;
    contexts=NULL;
    numberOfContexts=0u;
}

// triggered by the scheduler
void GAMContextManager::SwitchToNext() {

}

bool GAMContextManager::AddContext(ReferenceT<GAMContext> newContext) {
    bool ret = true;
    if ((numberOfContexts % contextArrayGranularity) == 0u) {
        Reference<GAM>* temp = reinterpret_cast<Reference<GAM>*>(HeapManager::Realloc(
                contexts, sizeof(ReferenceT<GAMContext> ) * (numberOfContexts + contextArrayGranularity)));
        ret = (temp != NULL);
        if (ret) {
            contexts = temp;
        }
        else {
            //TODO Error in reallocation
        }
    }

    if (ret) {
        contexts[numberOfContexts] = newContext;
        numberOfContexts++;
    }

    return ret;

}

bool GAMContextManager::Validate() {
    bool ret = false;
    ReferenceT<GAMContextContainer> contextContainer = ObjectRegistryDatabase::Instance()->Find(contextContainerPath.Buffer());
    if (contextContainer.IsValid()) {
        ret = contextContainer->Validate();
    }
    return ret;
}

bool GAMContextManager::Initialise(StructuredDataI & data) {

    bool ret = false;
    uint32 numberOfItems = data.GetNumberOfChildren();
    items = new NamedPointer[numberOfItems];
    for (uint32 i = 0u; i < numberOfItems; i++) {

        const char8* childName = data.GetChildName(i);
        AnyType leaf = data.GetType(childName);
        // case structured
        if (leaf.GetDataPointer() == NULL) {
            data.MoveRelative(childName);
            StreamString className;
            if(data.Read("Class", className)) {
                const ClassRegistryItem *item=ClassRegistryDatabase::Instance()->Find(className.Buffer());
                // the struct must be registered
                if(item!=NULL) {
                    const ClassProperties *properties=item->GetClassProperties();
                    if(properties!=NULL) {
                        uint32 classSize=properties->GetSize();
                        void* structPtr=contextMemory.Add(classSize);
                        const Introspection *introspection=item->GetIntrospection();
                        if(introspection != NULL) {
                            if(structPtr!=NULL) {
                                TypeDescriptor structDescriptor(false, properties->GetUniqueIdentifier());
                                AnyType structAT(structDescriptor, 0u, structPtr);
                                // it is possible initialise the structure
                                if(!TypeConvert(structAT,data)) {
                                    //TODO Warning??
                                }
                            }
                            else {
                                //TODO
                            }
                        }
                        items[i].pointer=structPtr;
                        items[i].type=className;
                        items[i].name=childName;
                        items[i].size=classSize;
                    }
                }
            }
            if (!data.MoveToAncestor(1u)) {
                //TODO
            }
        }
        // case simple leaf
        else {
            // manage the case of pointers... in this case copy only the pointer (const char, pointer, ecc)
            uint32 leafSize=leaf.GetByteSize();
            for(uint32 j=0u; j<3u; j++) {
                leafSize*=leaf.GetNumberOfElements(j);
            }
            void* leafPointer=leaf.GetDataPointer();
            TypeDescriptor leafDescriptor=leaf.GetTypeDescriptor();
            bool isCString=(leafDescriptor==ConstCharString);
            bool isPointer=(leafDescriptor==PointerType);
            void *basicPtr=NULL;
            if(isCString || isPointer) {
                //Copy only the space of the pointers not the pointed memory
                basicPtr=contextMemory.Add(leafSize);
            }
            else {
                basicPtr=contextMemory.Add(leafPointer, leafSize);
            }
            if(basicPtr!=NULL) {
                items[i].name=childName;
                items[i].type=TypeDescriptor::GetTypeNameFromTypeDescriptor(leafDescriptor);
                items[i].pointer=basicPtr;
                items[i].size=leafSize;
            }
        }

    }
    // read the container path
    if (!data.Read("ContextContainerPath", contextContainerPath)) {
        ret = false;
    }

    return ret;

}

}
