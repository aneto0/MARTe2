/**
 * @file RealTimeDataDefContainer.cpp
 * @brief Source file for class RealTimeDataDefContainer
 * @date 25/feb/2016
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
 * the class RealTimeDataDefContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataDefContainer.h"
#include "ReferenceT.h"
#include "RealTimeDataDefI.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

RealTimeDataDefContainer::RealTimeDataDefContainer() {
    final = false;
    isInput = false;
    isOutput = false;

}

bool RealTimeDataDefContainer::Verify() {

    bool ret = true;
    uint32 itemsNumber = Size();
    for (uint32 i = 0u; (i < itemsNumber) && (ret); i++) {
        ReferenceT<RealTimeDataDefI> item = Get(i);
        if (item.IsValid()) {
            ret = item->Verify();
        }
    }

    return ret;
}

bool RealTimeDataDefContainer::MergeWithLocal(StructuredDataI & localData) {

    StreamString isLocalDefFinal;

    bool ret = true;
    uint32 newItemsNumber = localData.GetNumberOfChildren();

    // check the IO definitions
    // false is by default, so enable if it is defined true in the local

    StreamString isLocalInputStr;
    bool isLocalInput = isInput;
    if (localData.Read("IsInput", isLocalInputStr)) {
        if ((!isInput) && (isLocalInputStr == "true")) {
            isInput = true;
        }
    }

    StreamString isLocalOutputStr;
    if (localData.Read("IsOutput", isLocalOutputStr)) {
        if ((!isOutput) && (isLocalOutputStr == "true")) {
            isOutput = true;
        }
    }

    for (uint32 i = 0u; (i < newItemsNumber) & (ret); i++) {
        const char8 * newItemName = localData.GetChildName(i);
        // take only the objects
        if ((newItemName[0] == '+') || (newItemName[0] == '$')) {
            uint32 itemsNumber = Size();
            bool found = false;
            for (uint32 j = 0u; (j < itemsNumber) && (ret) && (!found); j++) {
                ReferenceT<RealTimeDataDefI> item = Get(j);
                // if the name are equal go inside the definition
                if (StringHelper::Compare(item->GetName(), newItemName) == 0) {
                    if (localData.MoveRelative(newItemName)) {
                        ret = (item->MergeWithLocal(localData));
                        if (!localData.MoveToAncestor(1u)) {
                            ret = false;
                        }
                    }
                    found = true;
                }
            }
            // if does not exist yest and it is not final add the definition
            if ((ret) && (!found)) {
                if (!final) {
                    if (localData.MoveRelative(newItemName)) {
                        ReferenceT<RealTimeDataDefI> newItem;
                        newItem.Initialise(localData, false);
                        ret = (newItem.IsValid());
                        if (ret) {
                            newItem->SetName(newItemName);
                            ret = Insert(newItem);
                            if (!localData.MoveToAncestor(1u)) {
                                ret = false;
                            }
                        }
                    }
                }
                else {
                    //TODO Cannot add if one of them is declared final
                    ret = false;
                }
            }

        }
    }
    return ret;
}

bool RealTimeDataDefContainer::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        StreamString isFinal;
        ret = (data.Read("IsFinal", isFinal));
        if (ret) {
            final = (isFinal == "true");
        }
        StreamString isInput;
        if (data.Read("IsInput", isInput)) {
            isInput = (isInput == "true");
        }
        StreamString isOutput;
        if (data.Read("IsOutput", isOutput)) {
            isInput = (isOutput == "true");
        }
    }
    return ret;
}

bool RealTimeDataDefContainer::IsFinal() const {
    return final;
}

void RealTimeDataDefContainer::SetInput(bool isInputPar) {
    isInput = isInputPar;
}

void RealTimeDataDefContainer::SetOutput(bool isOutputPar) {
    isOutput = isOutputPar;
}

bool RealTimeDataDefContainer::IsInput() const {
    return isInput;
}

bool RealTimeDataDefContainer::IsOutput() const {
    return isOutput;
}

CLASS_REGISTER(RealTimeDataDefContainer, "1.0")

}

