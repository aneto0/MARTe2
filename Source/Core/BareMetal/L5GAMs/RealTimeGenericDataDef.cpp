/**
 * @file RealTimeGenericDataDef.cpp
 * @brief Source file for class RealTimeGenericDataDef
 * @date 25/02/2016
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
#include "stdio.h"
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
                const Introspection *intro = item->GetIntrospection();
                if (intro != NULL) {
                    // not final
                    if (Size() > 0u) {
                        uint32 numberOfMembers = Size();
                        if (numberOfMembers == intro->GetNumberOfMembers()) {
                            ret = true;
                            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                                const IntrospectionEntry introEntry = (*intro)[i];
                                bool found = false;
                                ReferenceT<RealTimeGenericDataDef> element;
                                for (uint32 j = 0u; (j < numberOfMembers) && (!found) && (ret); j++) {
                                    element = Get(j);
                                    ret = element.IsValid();
                                    if (ret) {
                                        // compare the name
                                        // skip the + or $ at the beginning
                                        found = (StringHelper::Compare(&(element->GetName()[1]), introEntry.GetMemberName()) == 0);
                                        if (found) {
                                            // compare the type
                                            ret = (StringHelper::Compare(element->GetType(), introEntry.GetMemberTypeName()) == 0);

                                            if (ret) {
                                                // go recursively
                                                // verify the sub type
                                                ret = element->Verify();
                                            }
                                            else {
                                                //TODO type mismatch
                                            }
                                        }
                                    }
                                }
                                if (!found) {
                                    //TODO Invalid element
                                    ret = false;
                                }
                            }

                        }
                        else {
                            //TODO #members mismatch
                        }
                    }
                    else {
                        if (path == "") {
                            //TODO Final type without address
                        }
                        else {
                            // the type exists and it is complete
                            ret = true;
                        }
                    }
                }
                else {
                    //TODO undefined type
                }
            }
            else {
                //TODO undefined type
            }
        }
    }

    return ret;
}

bool RealTimeGenericDataDef::Initialise(StructuredDataI& data) {

    bool ret = RealTimeDataDefI::Initialise(data);
    if (ret) {
        if (!data.Read("Default", defaultValue)) {
            //TODO
        }
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

bool RealTimeGenericDataDef::ToStructuredData(StructuredDataI& data) {

    const char8 * name = GetName();
    bool ret = data.CreateRelative(name);
    ret = data.Write("Class", "RealTimeGenericDataDef");
    if (ret) {
        ret = data.Write("Type", type);
    }
    if (ret) {
        ret = data.Write("Path", path);
    }
    if (ret) {
        data.Write("Default", defaultValue);
    }
    if (ret) {
        uint32 numberOfChildren = Size();
        for (uint32 i = 0u; i < numberOfChildren; i++) {
            Reference child = Get(i);
            ret = child.IsValid();
            if (ret) {
                if (ret) {
                    ret = child->ToStructuredData(data);
                }
            }
        }
    }
    if (data.MoveToAncestor(1u)) {
        ret = false;
    }

    return ret;
}

bool RealTimeGenericDataDef::MergeWithLocal(StructuredDataI & localData) {

    StreamString isLocalDefFinal;
    bool localFinal = false;
    if (localData.Read("IsFinal", isLocalDefFinal)) {
        localFinal = (isLocalDefFinal == "true");
    }

    bool ret = (!final) && (!localFinal);
    // merge if both are not final, return false in other cases
    if (ret) {
        // if type and path are different, take the globals without returning error
        if (StringHelper::Compare(GetType(), "") == 0) {
            StreamString localType;
            if (localData.Read("Type", localType)) {
                SetType(localType.Buffer());
            }
        }
        // the same with the path
        if (StringHelper::Compare(GetPath(), "") == 0) {
            StreamString localPath;
            if (localData.Read("Path", localPath)) {
                SetPath(localPath.Buffer());
            }
        }

        uint32 newItemsNumber = localData.GetNumberOfChildren();
        for (uint32 i = 0u; (i < newItemsNumber) & (ret); i++) {
            const char8 * newItemName = localData.GetChildName(i);
            if ((newItemName[0] == '+') || (newItemName[0] == '$')) {
                uint32 itemsNumber = Size();
                bool found = false;
                for (uint32 j = 0u; (j < itemsNumber) && (ret) && (!found); j++) {
                    ReferenceT<RealTimeDataDefI> item = Get(j);
                    ret = item.IsValid();
                    if (ret) {
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
                }

                if ((!found) && (ret)) {
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
CLASS_REGISTER(RealTimeGenericDataDef, "1.0")

}
