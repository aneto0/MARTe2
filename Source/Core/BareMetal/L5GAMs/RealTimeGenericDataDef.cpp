/**
 * @file RealTimeGenericDataDef.cpp
 * @brief Source file for class RealTimeGenericDataDef
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
 * the class RealTimeGenericDataDef (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeGenericDataDef.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeGenericDataDef::RealTimeGenericDataDef() {
    final = false;
}

bool RealTimeGenericDataDef::Verify() {
    //TODO
    return true;
}

bool RealTimeGenericDataDef::Initialise(StructuredDataI& data) {

    bool ret = RealTimeDataDefI::Initialise(data);
    if (ret) {
        ret = data.Read("Default", defaultValue);
    }
    if (ret) {
        StreamString isFinal;
        ret = data.Read("IsFinal", isFinal);
        if (ret) {
            final = (isFinal == "true");
        }
    }
    return ret;
}

bool RealTimeGenericDataDef::MergeWithLocal(StructuredDataI & localData) {

    uint32 newItemsNumber = localData.GetNumberOfChildren();

    StreamString isLocalDefFinal;
    bool ret = localData.Read("IsFinal", isLocalDefFinal);

    if (ret) {
        bool localFinal = (isLocalDefFinal == "true");

        // no need to merge if both definitions are final, return true
        ret = ((final) && (localFinal));

        // merge if both are not final, return false in other cases
        if ((!final) && (!localFinal)) {
            //ret = (newItemsNumber > 0u);
            for (uint32 i = 0u; (i < newItemsNumber) & (ret); i++) {
                const char8 * newItemName = localData.GetChildName(i);
                for (uint32 j = 0u; j < Size(); j++) {
                    ReferenceT<RealTimeGenericDataDef> item = Get(i);
                    if (item.IsValid()) {
                        if (StringHelper::Compare(item->GetName(), newItemName) == 0) {
                            // if the path is the same go inside
                            if (localData.MoveRelative(newItemName)) {
                                ret = item->MergeWithLocal(localData);
                                if (localData.MoveToAncestor(1u)) {
                                    ret = false;
                                }
                            }
                        }
                        else {
                            ReferenceT<RealTimeDataDefI> newItem;
                            newItem.Initialise(localData, false);
                            if (newItem.IsValid()) {
                                ret = Insert(newItem);
                            }
                            else {
                                ret = false;
                            }
                        }
                    }
                }
            }
        }
    }
    return ret;
}



const char8 *RealTimeGenericDataDef::GetDefaultValue() {
    return defaultValue.Buffer();
}

bool RealTimeGenericDataDef::IsFinal() const{
    return final;
}



}
