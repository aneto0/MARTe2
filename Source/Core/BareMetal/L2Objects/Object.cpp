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

#include "Object.h"
#include "AnyType.h"
#include "StringHelper.h"
#include "HeapI.h"
#include "ReferenceContainer.h"
#include "MemoryOperationsHelper.h"
#include "Atomic.h"
#include "ClassMethodCaller.h"
#include "ClassRegistryDatabase.h"
#include "IntrospectionTools.h"
#include "CCString.h"
#include "CString.h"


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

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName,
                                                        ReferenceContainer &parameters) {
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

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName,
                                                        StructuredDataI &parameters) {
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

ErrorManagement::ErrorType Object::CallRegisteredMethod(const CCString &methodName,
                                                        StreamI &stream) {
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


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Object::Object() {
    referenceCounter = 0;
    objectName = NULL_PTR(char8 *);
    isDomain = false;
}

Object::Object(const Object &copy) {
    referenceCounter = 0;
    SetName(copy.GetName());
    isDomain = false;
}

/*lint -e{1551} the destructor must guarantee that the named is freed. No exception should be
 * thrown given that name always points to a valid memory address and thus Memory::Free
 * should not raise exceptions.*/
Object::~Object() {
    if (!objectName.IsNullPtr()) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(objectName));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Object: Failed HeapManager::Free() in destructor");
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

CCString Object::GetName() const {
    return objectName.GetList();
}

static inline char8 toHex(uint8 value){
    uint8 hexValue;
    if (value < 10){
        hexValue =  48u+value;
    } else {
        hexValue =  55u+value;
    }

    return static_cast<char8>(hexValue);
}

bool Object::GetUniqueName(StreamI &nameStream) const {

    /*lint -e{9091} -e{923} the casting from pointer type to integer type is required in order to be able to get a
     * numeric address of the pointer.*/
    uintp ptrHex = reinterpret_cast<uintp>(this);
    uint32 ptrNum = static_cast<uint32>(ptrHex);

    //Each byte in the hexadecimal representation of the pointer is described by two chars, (e.g. 1 byte = 0xFF)
    const uint32 nOfPtrChars = static_cast<uint32>(sizeof(void *) * 2u);

    bool ret = true;
    {
        // 16 chars for the 64 bit hex + x + terminator;
        char8 buffer[nOfPtrChars+4];
        buffer[0] = 'x';
        char8 *pb = &buffer[1];

        // fill the buffer
        uint32 i;
        for (i = nOfPtrChars-1;i >=0;i-- ){
            uint32 masked = (ptrNum >> (i * 4)) & 0xF;
            *pb++ = toHex(masked);
        }
        // terminate
        *pb++ = ':';
        *pb++ = ':';
        *pb = 0;

        uint32 size = nOfPtrChars+3;
        ret = nameStream.Write(buffer,size);
    }

    if (ret){
        CCString name = GetName();
        uint32 size = name.GetSize();
        ret = nameStream.Write(name.GetList(),size);
    }

    return ret;
}

void Object::SetName(CCString const newName) {
    if (!objectName.IsNullPtr()) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(objectName));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::FatalError, "Object: Failed HeapManager::Free()");
        }
    }

    uint32 size = newName.GetSize()+1;
    if (size > 0){
        objectName = reinterpret_cast<char8 *>(HeapManager::Duplicate(newName.GetList(),size));
    }

}
#if 0
bool Object::ExportData(StructuredDataI & data) {
    return ConvertObjectToStructuredData(*this,data);
}

bool Object::ExportMetadata(StructuredDataI & data,const int32 level) {

  return ConvertObjectMetadataToStructuredData(*this,data,level);

}

#endif

void Object::SetDomain(const bool isDomainFlag) {
    isDomain = isDomainFlag;
}

bool Object::IsDomain() const {
    return isDomain;
}

void Object::ToAnyType(AnyType &at){
	ClassRegistryItem *cri = GetClassRegistryItem();
	//TODO This only works if Object is the single root class
	//TODO Better would be to add a static function to convert Object *this to T*
	at = AnyType (cri->GetTypeDescriptor(),this);
}


CLASS_REGISTER(Object, "1.0")

}
