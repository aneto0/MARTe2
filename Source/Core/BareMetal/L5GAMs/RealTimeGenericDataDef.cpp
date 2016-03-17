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
#include "AdvancedErrorManagement.h"

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
    bool ret = (type != "");
    // if type is not empty
    if (ret) {
        // if the type is basic return true (nothing to check)
        // the type is structured
        if (TypeDescriptor::GetTypeDescriptorFromTypeName(type.Buffer()) == InvalidType) {
            // in this case it must be registered
            const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(type.Buffer());
            ret = (item != NULL);
            if (ret) {
                const Introspection *intro = item->GetIntrospection();
                ret = (intro != NULL);
                if (ret) {
                    // not final
                    if (Size() > 0u) {
                        uint32 numberOfMembers = Size();
                        ret = (numberOfMembers == intro->GetNumberOfMembers());
                        if (ret) {
                            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                                const IntrospectionEntry introEntry = (*intro)[i];
                                bool found = false;
                                ReferenceT<RealTimeGenericDataDef> element;
                                for (uint32 j = 0u; (j < numberOfMembers) && (!found) && (ret); j++) {
                                    element = Get(j);
                                    ret = element.IsValid();
                                    if (ret) {
                                        const char8 *defMemberName = element->GetName();
                                        ret = defMemberName != NULL;
                                        if (ret) {
                                            // compare the name
                                            // skip the + or $ at the beginning
                                            found = (StringHelper::Compare(&(defMemberName[1]), introEntry.GetMemberName()) == 0);
                                            if (found) {
                                                // compare the type
                                                ret = (StringHelper::Compare(element->GetType(), introEntry.GetMemberTypeName()) == 0);

                                                if (ret) {
                                                    // go recursively
                                                    // verify the sub type
                                                    ret = element->Verify();
                                                }
                                                else {
                                                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                                            "Type mismatch for the member %s defined in introspection as %s",
                                                                            introEntry.GetMemberName(), introEntry.GetMemberTypeName())
                                                }
                                            }
                                        }
                                    }
                                }
                                if (!found) {
                                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Member %s not found in introspection", introEntry.GetMemberName())
                                    ret = false;
                                }
                            }
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The number of members does not coincide with %d defined in introspection",
                                                    numberOfMembers)
                        }
                    }
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type %s not introspectable", type.Buffer())
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Type %s not registered", type.Buffer())
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "The type cannot be empty");
    }

    return ret;
}

bool RealTimeGenericDataDef::Initialise(StructuredDataI& data) {

    bool ret = RealTimeDataDefI::Initialise(data);
    if (ret) {
        StreamString isFinal;
        if (data.Read("IsFinal", isFinal)) {
            final = (isFinal == "true");
        }
    }
    return ret;
}

bool RealTimeGenericDataDef::ToStructuredData(StructuredDataI& data) {

    const char8 * name = GetName();
    bool ret = data.CreateRelative(name);
    if (ret) {
        ret = data.Write("Class", "RealTimeGenericDataDef");
        if (ret) {
            if (type != "") {
                ret = data.Write("Type", type);
            }
        }
        if (ret) {
            if (path != "") {
                ret = data.Write("Path", path);
            }
        }
        if (ret) {
            if (defaultValue != "") {
                data.Write("Default", defaultValue);
            }
        }

        if (ret) {
            if (numberOfDimensions > 0u) {
                ret = data.Write("NumberOfDimensions", numberOfDimensions);
                if (ret) {
                    ret = data.Write("NumberOfElements", numberOfElements);
                }
            }
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
        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }

    return ret;
}

bool RealTimeGenericDataDef::MergeWithLocal(StructuredDataI & localData) {

    bool ret = (!final);
    // merge if both are not final, return false in other cases
    if (ret) {
        // if type and path are different, take the globals without returning error
        if (type == "") {
            if (!localData.Read("Type", type)) {
            }
        }
        // the same with the path
        if (path == "") {
            if (!localData.Read("Path", path)) {
            }
        }

        // the same with the default value
        if (defaultValue == "") {
            if (!localData.Read("Default", defaultValue)) {
            }
        }

        if (modifiers == "") {
            if (localData.Read("Modifiers", modifiers)) {
                // use introspection entry to parse the modifiers
                IntrospectionEntry entry("", "", modifiers.Buffer(), "", 0u, 0u);
                numberOfDimensions = entry.GetNumberOfDimensions();
                for (uint32 i = 0u; i < 3u; i++) {
                    numberOfElements[i] = entry.GetNumberOfElements(i);
                }
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
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Trying to merge a final definition with local configuration data");
    }
    return ret;
}

CLASS_REGISTER(RealTimeGenericDataDef, "1.0")

}
