/**
 * @file Object.cpp
 * @brief Source file for class Object
 * @date 04/08/2015
 * @author Andre' Neto
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
 *
 * @details This source file contains the definition of all the methods for
 * the class Object (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemT.h"
#include "Object.h"
#include "StringHelper.h"
#include "HeapI.h"
#include "Introspection.h"
#include "ReferenceContainer.h"
#include "MemoryOperationsHelper.h"
#include "Atomic.h"
#include "ClassMethodCaller.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName) {
    ErrorManagement::ErrorType err;

    ClassRegistryItem * cri = GetClassRegistryItem();
    ClassMethodCaller *caller = NULL_PTR(ClassMethodCaller *);

    err.fatalError = (cri == NULL_PTR(ClassRegistryItem *));

    if (err.ErrorsCleared()) {
        caller = cri->FindMethod(methodName);
        err.unsupportedFeature = (caller == NULL_PTR(ClassMethodCaller *));
    }

    if (err.ErrorsCleared()) {
        /*lint -e{613} err.unsupportedFeature protects from using caller = NULL*/
        err = caller->Call(this);
    }

    return err;
}

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName, ReferenceContainer &parameters) {
    ErrorManagement::ErrorType err;

    ClassRegistryItem * cri = GetClassRegistryItem();
    ClassMethodCaller *caller = NULL_PTR(ClassMethodCaller *);

    err.fatalError = (cri == NULL_PTR(ClassRegistryItem *));

    if (err.ErrorsCleared()) {
        caller = cri->FindMethod(methodName);
        err.unsupportedFeature = (caller == NULL_PTR(ClassMethodCaller *));
    }

    if (err.ErrorsCleared()) {
        /*lint -e{613} err.unsupportedFeature protects from using caller = NULL*/
        err = caller->Call(this, parameters);
    }

    return err;
}

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName, StructuredDataI &parameters) {
    ErrorManagement::ErrorType err;

    ClassRegistryItem * cri = GetClassRegistryItem();
    ClassMethodCaller *caller = NULL_PTR(ClassMethodCaller *);

    err.fatalError = (cri == NULL_PTR(ClassRegistryItem *));

    if (err.ErrorsCleared()) {
        caller = cri->FindMethod(methodName);
        err.unsupportedFeature = (caller == NULL_PTR(ClassMethodCaller *));
    }

    if (err.ErrorsCleared()) {
        /*lint -e{613} err.unsupportedFeature protects from using caller = NULL*/
        err = caller->Call(this, parameters);
    }

    return err;
}

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName, StreamI &stream) {
    ErrorManagement::ErrorType err;

    ClassRegistryItem * cri = GetClassRegistryItem();
    ClassMethodCaller *caller = NULL_PTR(ClassMethodCaller *);

    err.fatalError = (cri == NULL_PTR(ClassRegistryItem *));

    if (err.ErrorsCleared()) {
        caller = cri->FindMethod(methodName);
        err.unsupportedFeature = (caller == NULL_PTR(ClassMethodCaller *));
    }

    if (err.ErrorsCleared()) {
        /*lint -e{613} err.unsupportedFeature protects from using caller = NULL*/
        err = caller->Call(this, stream);
    }

    return err;
}

bool Object::ConvertDataToStructuredData(void* const ptr, const char8* const className, StructuredDataI& data, const char8* const objName) {
    bool ret = false;

    const ClassRegistryItem* sourceItem = ClassRegistryDatabase::Instance()->Find(className);

    if (sourceItem != NULL) {
        const Introspection *sourceIntrospection = sourceItem->GetIntrospection();
        if (GetName() != NULL_PTR(const char8 *)) {
            ret = data.Write("Name", GetName());
        }
        else {
            ret = data.Write("Name", "?");
        }
        if (ret) {
            ret = data.Write("Class", sourceItem->GetClassProperties()->GetName());
        }
        if ((sourceIntrospection != NULL) && (ret)) {
            if (objName != NULL) {
                ret = data.CreateRelative(objName);
            }
            if ((data.Write("Class", sourceItem->GetClassProperties()->GetName())) && (ret)) {
                uint32 numberOfMembers = sourceIntrospection->GetNumberOfMembers();

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
                            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertDataToStructuredData: Number of dimensions greater than 0 not supported.");
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
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertDataToStructuredData: Class not registered");
    }
    return ret;
}

bool Object::ConvertMetadataToStructuredData(void * const ptr, const char8 * const className, StructuredDataI &data, const int32 recursionLevel) {
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
                            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                            ret = false;
                        }
                        if (ret) {
                            const char8* memberModifiers = sourceMemberIntrospection.GetMemberModifiers();
                            if (!data.Write("modifiers", memberModifiers)) {
                                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            const char8* memberAttributes = sourceMemberIntrospection.GetMemberAttributes();
                            if (!data.Write("attributes", memberAttributes)) {
                                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            uint32 memberSize = sourceMemberIntrospection.GetMemberSize();
                            if (!data.Write("size", memberSize)) {
                                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
                                ret = false;
                            }
                        }
                        if (ret) {
                            uint32 memberOffset = sourceMemberIntrospection.GetMemberByteOffset();
                            /*lint -e{9091} -e{923} the casting from pointer type to integer type is
                             * required in order to be able to get a numeric address of the pointer.*/
                            if (!data.Write("pointer", (reinterpret_cast<uintp>(ptr) + memberOffset))) {
                                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Error when writing a leaf on the StructuredDataI object");
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
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Introspection not found for the specified class");
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ConvertMetadataToStructuredData: Class not registered");
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Object::Object() {
    referenceCounter = 0;
    thisObjName = NULL_PTR(char8 *);
    isDomain = false;
}

Object::Object(const Object &copy) {
    referenceCounter = 0;
    if (copy.thisObjName != NULL_PTR(char8 *)) {
        thisObjName = StringHelper::StringDup(copy.thisObjName);
    }
    else {
        thisObjName = NULL_PTR(char8 *);
    }
    isDomain = false;
}

/*lint -e{1551} the destructor must guarantee that the named is freed. No exception should be
 * thrown given that name always points to a valid memory address and thus Memory::Free
 * should not raise exceptions.*/
Object::~Object() {
    if (thisObjName != NULL_PTR(char8 *)) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(thisObjName));
        if (!ok) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Object: Failed HeapManager::Free() in destructor");
        }
    }
}

uint32 Object::DecrementReferences() {
    Atomic::Decrement(&referenceCounter);
    uint32 ret = static_cast<uint32>(referenceCounter);
    return ret;
}

void Object::IncrementReferences() {
    Atomic::Increment(&referenceCounter);

}

Object *Object::Clone() const {
    return NULL_PTR(Object *);
}

uint32 Object::NumberOfReferences() const {
    return static_cast<uint32>(referenceCounter);
}

/*lint -e{715} data is not used as this is not implemented on purpose*/
bool Object::Initialise(StructuredDataI &data) {
    return true;
}

//LCOV_EXCL_START
/*lint -e{715} size is not used as this is not implemented on purpose*/
void *Object::operator new(const osulong size) throw () {
    return NULL_PTR(Object *);
}
//LCOV_EXCL_STOP

const char8 * const Object::GetName() const {
    return thisObjName;
}

void Object::GetUniqueName(char8 * const destination, const uint32 &size) const {
    if (!MemoryOperationsHelper::Set(destination, '\0', size)) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Failed initialization of the object name in output");
    }
    /*lint -e{9091} -e{923} the casting from pointer type to integer type is required in order to be able to get a
     * numeric address of the pointer.*/
    uintp ptrHex = reinterpret_cast<uintp>(this);
//Each byte in the hexadecimal representation of the pointer is described by two chars, (e.g. 1 byte = 0xFF)
    uint32 nOfPtrChars = static_cast<uint32>(sizeof(void *) * 2u);
    uint32 shiftBits = 0u;

//Count the number of leading zeros
    uint32 nLeadingOfZeros = 0u;
    uint32 i;
    for (i = 0u; i < nOfPtrChars; i++) {
        //First character in destination is the MSB
        shiftBits = nOfPtrChars * 4u;
        shiftBits -= 4u * (i + 1u);
        uint32 hexValue = static_cast<uint32>(ptrHex >> shiftBits);
        hexValue &= 0xFu;
        if (hexValue == 0u) {
            nLeadingOfZeros++;
        }
        else {
            break;
        }
    }
    if (size != 0u) {
        destination[0] = 'x';
    }
    for (i = 1u; (i < size) && (i < ((nOfPtrChars - nLeadingOfZeros) + 1u)); i++) {
        //First character in destination is the MSB
        shiftBits = nOfPtrChars * 4u;
        shiftBits -= 4u * (i + nLeadingOfZeros); //Notice that i already starts at 1
        uint32 hexValue = static_cast<uint32>(ptrHex >> shiftBits);
        hexValue &= 0xFu;
        if (hexValue < 0xAu) {
            hexValue += 48u;
            destination[i] = static_cast<char8>(hexValue);
        }
        else {
            hexValue += 55u;
            destination[i] = static_cast<char8>(hexValue);
        }
    }
    if (i < size) {
        destination[i] = ':';
        i++;
    }
    if (i < size) {
        destination[i] = ':';
        i++;
    }
//If there is no space to even write \0 don't even try
    if (i < size) {
        if (GetName() != NULL) {
            // TODO check memory allocation
            if (!StringHelper::ConcatenateN(destination, GetName(), size - i)) {
                destination[i] = '\0';
            }
        }
        else {
            destination[i] = '\0';
        }
    }
}

void Object::SetName(const char8 * const newName) {
    if (thisObjName != NULL_PTR(char8 *)) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(thisObjName));
        if (!ok) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Object: Failed HeapManager::Free()");
        }
    }
    thisObjName = StringHelper::StringDup(newName);
}

bool Object::ExportData(StructuredDataI & data) {
    bool ret = false;

    const ClassProperties *myProperties = GetClassProperties();
    if (myProperties != NULL) {
        ret = ConvertDataToStructuredData(reinterpret_cast<void*>(this), myProperties->GetName(), data, GetName());
    }
    return ret;
}

bool Object::ExportMetadata(StructuredDataI & data, const int32 level) {
    bool ret = false;

    const ClassProperties *myProperties = GetClassProperties();
    if (myProperties != NULL) {
        const char8* className = myProperties->GetName();

        ret = ConvertMetadataToStructuredData(reinterpret_cast<void*>(this), className, data, level);
    }
    return ret;
}

const ClassProperties *Object::GetClassProperties() const {
    const ClassProperties *cp = NULL_PTR(ClassProperties *);
    ClassRegistryItem * cri = GetClassRegistryItem();

    if (cri != NULL_PTR(ClassRegistryItem *)) {
        cp = cri->GetClassProperties();
    }

    return cp;
}

void Object::SetDomain(const bool isDomainFlag) {
    isDomain = isDomainFlag;
}

bool Object::IsDomain() const {
    return isDomain;
}

/*lint -e{715} purgeList is not used in the default implementation of the method*/
void Object::Purge(ReferenceContainer &purgeList) {

}

bool Object::IsReferenceContainer() const {
    return false;
}

CLASS_REGISTER(Object, "1.0")

}
