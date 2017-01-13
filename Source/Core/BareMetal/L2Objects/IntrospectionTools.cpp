/**
 * @file IntrospectionTools.cpp
 * @brief Source file for class IntrospectionTools
 * @date Jan 11, 2017
 * @author fsartori
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
 * the class IntrospectionTools (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionTools.h"
#include "ErrorManagement.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

bool ConvertObjectToStructuredData(Object const &object,StructuredDataI& data){
    bool ret = true;

    ret = data.CreateRelative(object.GetName());

    // successfully created node
    if (ret){

        const ClassRegistryItem* cri = object.GetClassRegistryItem();
        if (cri == NULL){
            REPORT_ERROR(ErrorManagement::FatalError, "ConvertDataToStructuredData: Introspection not found for the specified class");
            ret = false;
        }  else {
            CCString className = cri->GetClassName();
            ret = data.Write("Class", className);

            if (ret){

                ret = ConvertDataToStructuredData(static_cast< void const *>(&object),cri,data);

            }
        }

        // move back in any case
        ret = ret && data.MoveToAncestor(1u);

    } // created node

    return ret;
}


bool ConvertDataToStructuredData(void const  * const ptr,
                                 const ClassRegistryItem *cri,
                                 StructuredDataI& data){

#if 0
    bool ret = false;

    if (cri == NULL) {
        REPORT_ERROR(ErrorManagement::ParametersError, "ConvertDataToStructuredData: ClassRegistryItem cannot be NULL");
        ret = false;
    }

    if (ret){
        uint32 memberNo= 0;
        const ClassMember * memberDesc = cri->FindMember(memberNo++);
        while (memberDesc != NULL){

            AnyType at(memberDesc->GetDescriptor().GetTypeDescriptor(),memberDesc->GetOffset());




            memberDesc = cri->FindMember(memberNo++);
        }
    }



//        const VariableDescriptor * varDesc = cri->FindMember(0);


                for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                    IntrospectionEntry sourceMemberIntrospection = (*sourceIntrospection)[i];

                    TypeDescriptor sourceMemberDescriptor = sourceMemberIntrospection.GetMemberTypeDescriptor();

                    TypeDescriptor newSourceDescriptor = sourceMemberDescriptor;
                    // source is a pointer!
                    if (sourceMemberIntrospection.GetMemberPointerLevel() > 0u) {
                        newSourceDescriptor = TypeDescriptor(false, UnsignedInteger, static_cast<uint8>(sizeof(void*)) * 8u);
                    }

                    char8* sourceMemberDataPointer = &(reinterpret_cast<char8*>(ptr)[sourceMemberIntrospection.GetMemberByteOffset()]);
                    AnyType newSource(newSourceDescriptor, 0u, sourceMemberDataPointer);
                    // special case char* string because is a pointer
                    if (newSourceDescriptor == CharString) {
                        if (sourceMemberIntrospection.GetNumberOfDimensions() == 0u) {
                            newSource = AnyType(*reinterpret_cast<char8**>(sourceMemberDataPointer));
                        }
                    }
                    for (uint32 j = 0u; j < 3u; j++) {
                        newSource.SetNumberOfElements(j, sourceMemberIntrospection.GetNumberOfElements(j));
                    }
                    newSource.SetNumberOfDimensions(sourceMemberIntrospection.GetNumberOfDimensions());

                    bool isNewSourceStructured = newSourceDescriptor.isStructuredData;
                    if (isNewSourceStructured) {
                        if (newSource.GetNumberOfDimensions() > 0u) {
                            REPORT_ERROR(ErrorManagement::FatalError, "ConvertDataToStructuredData: Number of dimensions greater than 0 not supported.");
                        }
                        else {
                            // structured data again! Create a node and go recursively
                            ret = data.CreateRelative(sourceMemberIntrospection.GetMemberName());
                            if (ret) {
                                ret = ConvertDataToStructuredData(newSource.GetDataPointer(), sourceMemberIntrospection.GetMemberTypeName(), data);
                                if (!data.MoveToAncestor(1u)) {
                                    ret = false;
                                }
                            }
                        }
                    }
                    else {
                        // in this case only write
                        ret = data.Write(sourceMemberIntrospection.GetMemberName(), newSource);
                    }
                }
            }
            if (objName != NULL) {
                ret = data.MoveToAncestor(1u);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "ConvertDataToStructuredData: Introspection not found for the specified class");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ConvertDataToStructuredData: Class not registered");

    }
    return ret;
#else
    return false;

#endif

}

bool ConvertObjectMetadataToStructuredData(Object const & object,
                                           StructuredDataI& data,
                                           const int32 recursionLevel )
{
    return false;
}


bool ConvertMetadataToStructuredData(    void const  * const ptr,
                                         const ClassRegistryItem *cri,
                                         StructuredDataI& data,
                                         const int32 recursionLevel) {
#if 0

    bool ret = false;
    const ClassRegistryItem *sourceItem = ClassRegistryDatabase::Instance()->Find(className);

    if (sourceItem != NULL) {
        const Introspection *sourceIntrospection = sourceItem->GetIntrospection();
        if (sourceIntrospection != NULL) {
            // create the class node
            if (data.CreateRelative(className)) {
                uint32 numberOfMembers = sourceIntrospection->GetNumberOfMembers();
                ret = true;
                for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                    IntrospectionEntry sourceMemberIntrospection = (*sourceIntrospection)[i];
                    // create the member node
                    if (data.CreateRelative(sourceMemberIntrospection.GetMemberName())) {
                        const char8* memberTypeName = sourceMemberIntrospection.GetMemberTypeName();
                        // write the type name
                        if (!data.Write("type", memberTypeName)) {
                            REPORT_ERROR(ErrorManagement::FatalError,
                                         "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                            ret = false;
                        }
                        if (ret) {
                            const char8* memberModifiers = sourceMemberIntrospection.GetMemberModifiers();
                            if (!data.Write("modifiers", memberModifiers)) {
                                REPORT_ERROR(ErrorManagement::FatalError,
                                             "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            const char8* memberAttributes = sourceMemberIntrospection.GetMemberAttributes();
                            if (!data.Write("attributes", memberAttributes)) {
                                REPORT_ERROR(ErrorManagement::FatalError,
                                             "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            uint32 memberSize = sourceMemberIntrospection.GetMemberSize();
                            if (!data.Write("size", memberSize)) {
                                REPORT_ERROR(ErrorManagement::FatalError,
                                             "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            uint32 memberOffset = sourceMemberIntrospection.GetMemberByteOffset();
                            /*lint -e{9091} -e{923} the casting from pointer type to integer type is
                             * required in order to be able to get a numeric address of the pointer.*/
                            if (!data.Write("pointer", (reinterpret_cast<uintp>(ptr) + memberOffset))) {
                                REPORT_ERROR(ErrorManagement::FatalError,
                                             "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }

                        if (recursionLevel != 0) {
                            bool isNewSourceStructured = sourceMemberIntrospection.GetMemberTypeDescriptor().isStructuredData;
                            if ((isNewSourceStructured) && (ret)) {
                                int32 newRecursionLevel = recursionLevel;
                                uint32 nPointers = sourceMemberIntrospection.GetMemberPointerLevel();
                                char8* sourceMemberDataPointer = &(reinterpret_cast<char8*>(ptr)[sourceMemberIntrospection.GetMemberByteOffset()]);
                                void* newSource = reinterpret_cast<void *>(sourceMemberDataPointer);
                                // take the pointer to the real object
                                for (uint32 j = 0u; j < nPointers; j++) {
                                    void** temp = reinterpret_cast<void**>(newSource);
                                    newSource = *temp;
                                }
                                if (newRecursionLevel > 0) {
                                    newRecursionLevel--;
                                }
                                ret = ConvertMetadataToStructuredData(newSource, memberTypeName, data, newRecursionLevel);
                            }
                        }
                        // move up to write the next member
                        if (ret) {
                            if (!data.MoveToAncestor(1u)) {
                                ret = false;
                            }
                        }

                    }
                }
                if (!data.MoveToAncestor(1u)) {
                    ret = false;
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Introspection not found for the specified class");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Class not registered");
    }

    return ret;
#endif
    return false;
}


}

	
