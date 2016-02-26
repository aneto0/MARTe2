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
#include "ReferenceT.h"
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
    // verify myself
    bool ret = true;
    // if type is not empty
    if (type != "") {
        // if the type is basic return true (nothing to check)
        // the type is structured
        if (TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer()) == InvalidType) {
            ret = false;
            // in this case it must be registered
            const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
            if (item != NULL) {
                const Introspection *intro=item->GetIntrospection();
                if(intro!=NULL) {
                    // not final
                    if(path=="") {
                        uint32 numberOfMembers=Size();
                        if(numberOfMembers==intro->GetNumberOfMembers()) {
                            ret=true;
                            for(uint32 i=0u; (i<numberOfMembers) && (ret); i++) {
                                const IntrospectionEntry introEntry=(*intro)[i];
                                ReferenceT<RealTimeGenericDataDef> element=Get(i);
                                if(element.IsValid()) {
                                    // compare the type
                                    ret=(StringHelper::Compare(element->GetType(),introEntry.GetMemberTypeName())==0);
                                    if(ret){
                                        // compare the name
                                        ret=(StringHelper::Compare(element->GetName(),introEntry.GetMemberName())==0);
                                    }
                                    if(ret) {
                                        // go recursively
                                        // verify the sub type
                                        ret=element->Verify();
                                    }
                                    else{
                                        //TODO type mismatch
                                    }
                                }
                                else{
                                    //TODO invalid element
                                }
                            }

                        }
                        else{
                            //TODO #members mismatch
                        }
                    }
                    else{
                        // the type exists and it is complete
                        ret=true;
                    }
                }
                else{
                    //TODO undefined type
                }
            }
            else{
               //TODO undefined type
            }
        }
    }

    return ret;
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


    StreamString isLocalDefFinal;
    bool ret = localData.Read("IsFinal", isLocalDefFinal);

    if (ret) {
        bool localFinal = (isLocalDefFinal == "true");

        ret = (!final) && (!localFinal);

        // merge if both are not final, return false in other cases
        if (ret) {

            uint32 newItemsNumber = localData.GetNumberOfChildren();
            for (uint32 i = 0u; (i < newItemsNumber) & (ret); i++) {
                const char8 * newItemName = localData.GetChildName(i);
                uint32 itemsNumber=Size();
                for (uint32 j = 0u; (j < itemsNumber) && (ret); j++) {
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
                            ReferenceT<RealTimeGenericDataDef> newItem;
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

bool RealTimeGenericDataDef::IsFinal() const {
    return final;
}

}
