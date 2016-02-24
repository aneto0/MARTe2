/**
 * @file GAMContext.cpp
 * @brief Source file for class GAMContext
 * @date 22/02/2016
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
 * the class GAMContext (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMContext.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMContext::GAMContext() {
    items = NULL_PTR(NamedPointer*);
    numberOfItems = 0u;
}
GAMContext::~GAMContext() {
    if (items != NULL) {
        delete [] items;
    }
    numberOfItems=0u;
}

bool GAMContext::Read(const char8* name,
                      void * output) {

    bool ret = false;
    if (items != NULL) {
        for(uint32 i=0u; i<numberOfItems; i++) {
            // name found
            if(StringHelper::Compare(items[i].name.Buffer(), name)==0) {
                if(MemoryOperationsHelper::Copy(output, items[i].pointer, items[i].size)) {
                    ret=true;
                }
            }
        }
    }
    return ret;
}

bool GAMContext::Write(const char8* name,
                       void * input) {
    bool ret = false;
    if (items != NULL) {
        for(uint32 i=0u; i<numberOfItems; i++) {
            // name found
            if(StringHelper::Compare(items[i].name.Buffer(), name)==0) {
                if(MemoryOperationsHelper::Copy(items[i].pointer, input,items[i].size)) {
                    ret=true;
                }
            }
        }
    }
    return ret;
}

bool GAMContext::Validate(GAMContextContainer& contextContainer) {
    bool ret = true;
    const char8 * managerPath = contextContainer.GetContextManagerPath();
    ReferenceT<GAMContextManager> manager = ObjectRegistryDatabase::Instance()->Find(managerPath);
    if (manager.IsValid()) {
        for (uint32 i = 0u; (i < numberOfItems) && (ret); i++) {
            NamedPointer* itemRet = manager->GetItem(items[i].name);
            if (itemRet != NULL) {
                items[i].pointer=itemRet->pointer;
                items[i].type=itemRet->type;
                items[i].size=itemRet->size;
            }
            else {
                //TODO
                ret=false;
            }
        }
        manager->AddContext(ReferenceT<GAMContext>(this));
    }
    return ret;
}

bool GAMContext::Initialise(StructuredDataI &data) {
    AnyType itemsAT = data.GetType("Items");

    if (itemsAT != NULL) {
        numberOfItems=itemsAT.GetNumberOfElements(0u);
        StreamString *itemNames=new StreamString[numberOfItems];
        items=new NamedPointer[numberOfItems];
        if(data.Read("Items", itemNames)) {
            for(uint32 i=0u; i<numberOfItems; i++) {
                items[i].name=itemNames;
            }
        }
        delete [] itemNames;
    }
    return true;
}

}
