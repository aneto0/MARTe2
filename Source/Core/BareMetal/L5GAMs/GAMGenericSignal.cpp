/**
 * @file GAMGenericSignal.cpp
 * @brief Source file for class GAMGenericSignal
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
 * the class GAMGenericSignal (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMGenericSignal.h"
#include "ReferenceT.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

GAMGenericSignal::GAMGenericSignal() :
        GAMSignalI() {
    final = false;
}

bool GAMGenericSignal::Verify() {
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
                /*lint -e{613} NULL pointer checking done before entering here */
                const Introspection *intro = item->GetIntrospection();
                ret = (intro != NULL);
                if (ret) {
                    // not final
                    if (Size() > 0u) {
                        uint32 numberOfMembers = Size();
                        /*lint -e{613} NULL pointer checking done before entering here */
                        ret = (numberOfMembers == intro->GetNumberOfMembers());
                        if (ret) {
                            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                                /*lint -e{613} NULL pointer checking done before entering here */
                                const IntrospectionEntry introEntry = (*intro)[i];
                                bool found = false;
                                ReferenceT<GAMGenericSignal> element;
                                for (uint32 j = 0u; (j < numberOfMembers) && (!found) && (ret); j++) {
                                    element = Get(j);
                                    ret = element.IsValid();
                                    if (ret) {
                                        const char8 *defMemberName = element->GetName();
                                        ret = (defMemberName != NULL);
                                        if (ret) {
                                            // compare the name
                                            // skip the + or $ at the beginning
                                            /*lint -e{613} NULL pointer checking done before entering here */
                                            found = (StringHelper::Compare(defMemberName, introEntry.GetMemberName()) == 0);
                                            if (found) {
                                                // compare the type
                                                ret = (StringHelper::Compare(element->GetType(), introEntry.GetMemberTypeName()) == 0);

                                                // check the number of dimensions
                                                if (ret) {
                                                    uint32 elementNumDimensions = element->GetNumberOfDimensions();
                                                    ret = (element->GetNumberOfDimensions() == introEntry.GetNumberOfDimensions());
                                                    if (ret) {
                                                        for (uint32 k = 0u; (k < elementNumDimensions) && (ret); k++) {
                                                            ret = (element->GetNumberOfElements(k) == introEntry.GetNumberOfElements(k));
                                                        }
                                                        if (!ret) {
                                                            REPORT_ERROR_PARAMETERS(
                                                                    ErrorManagement::FatalError,
                                                                    "Number of elements in dimensions not compatibles for %s defined in introspection as %s",
                                                                    introEntry.GetMemberName(), introEntry.GetMemberTypeName())

                                                        }
                                                    }
                                                    else {
                                                        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                                                "Number of dimensions not compatibles for %s defined in introspection as %s",
                                                                                introEntry.GetMemberName(), introEntry.GetMemberTypeName())
                                                    }
                                                }
                                                else {
                                                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
                                                                            "Type mismatch for the member %s defined in introspection as %s",
                                                                            introEntry.GetMemberName(), introEntry.GetMemberTypeName())
                                                }
                                                if (ret) {
                                                    // go recursively
                                                    // verify the sub type
                                                    ret = element->Verify();
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
                    else {
                        // unsupported multi-dimensional structures
                        ret = (numberOfDimensions == 0u);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Unsupported multi-dimensional structures for %s defined with type %s",
                                                    GetName(), GetType())

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

bool GAMGenericSignal::Initialise(StructuredDataI& data) {

    bool ret = GAMSignalI::Initialise(data);
    if (ret) {
        StreamString isFinal;
        if (data.Read("IsFinal", isFinal)) {
            final = (isFinal == "true");
        }
    }
    return ret;
}

bool GAMGenericSignal::ToStructuredData(StructuredDataI& data) {

    const char8 * objName = GetName();
    StreamString objNameToPrint=(IsDomain())?("$"):("+");
    objNameToPrint+=objName;
    bool ret = data.CreateRelative(objNameToPrint.Buffer());
    if (ret) {
        ret = data.Write("Class", "GAMGenericSignal");
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
                ret = data.Write("Default", defaultValue);
            }
        }

        if (ret) {
            if (dimensions != "") {
                ret = data.Write("Dimensions", dimensions);
            }
        }

        if (ret) {
            if (operation != "") {
                ret = data.Write("Operation", operation);
            }
        }

        if (ret) {
            if (cycles != 0) {
                ret = data.Write("Cycles", cycles);
            }
        }

        if (ret) {
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; i < numberOfChildren; i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    if (ret) {
                        ret = child->ExportData(data);
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

bool GAMGenericSignal::MergeWithLocal(StructuredDataI & localData) {

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

        // the same with the cycles
        if (cycles == 0) {
            if (!localData.Read("Cycles", cycles)) {
            }
        }

        if (dimensions == "") {

            if (localData.Read("Dimensions", dimensions)) {
                // use introspection entry to parse the dimensions
                IntrospectionEntry entry("", "", dimensions.Buffer(), "", 0u, 0u);
                numberOfDimensions = entry.GetNumberOfDimensions();
                for (uint32 i = 0u; i < 3u; i++) {
                    numberOfElements[i] = entry.GetNumberOfElements(i);
                }

            }
        }

        if (operation == "") {
            if (localData.Read("Operation", operation)) {

            }
        }

        uint32 newItemsNumber = localData.GetNumberOfChildren();
        for (uint32 i = 0u; (i < newItemsNumber) && (ret); i++) {
            const char8 * newItemName = localData.GetChildName(i);
            if ((newItemName[0] == '+') || (newItemName[0] == '$')) {
                uint32 itemsNumber = Size();
                bool found = false;
                for (uint32 j = 0u; (j < itemsNumber) && (ret) && (!found); j++) {
                    ReferenceT<GAMGenericSignal> item = Get(j);
                    ret = item.IsValid();
                    if (ret) {
                        if (StringHelper::Compare(item->GetName(), &newItemName[1]) == 0) {
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
                        ReferenceT<GAMGenericSignal> newItem;
                        ret = newItem.Initialise(localData, false);
                        if (ret) {
                            ret = (newItem.IsValid());
                            if (ret) {
                                newItem->SetName(&newItemName[1]);
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
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Trying to merge a final definition with local configuration data");
    }
    return ret;
}

CLASS_REGISTER(GAMGenericSignal, "1.0")

}
