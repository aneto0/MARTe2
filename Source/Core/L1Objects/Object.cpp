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

 * @details This source file contains the definition of all the methods for
 * the class Object (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "FastPollingMutexSem.h"
#include "StringHelper.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Object::Object() {
    referenceCounter = 0u;
    name = NULL_PTR(char8 *);
}

/*lint -e{1551} the destructor must guarantee that the named is freed. No exception should be
 * thrown given that name always points to a valid memory address and thus Memory::Free
 * should not raise exceptions.*/
Object::~Object() {
    if (name != NULL_PTR(char8 *)) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(name));
        if (!ok) {
            //TODO Error message here
        }
    }
}

/*lint -e{9141} global declaration but only used to support the class implementation.
 * The symbol is not exported (static). This could also be replaced by an anonymous namespace.
 */
static FastPollingMutexSem refMux;

uint32 Object::DecrementReferences() {
    uint32 ret = 0u;
    if (refMux.FastLock() == NoError) {
        --referenceCounter;
        ret = referenceCounter;
    }
    refMux.FastUnLock();
    return ret;
}

void Object::IncrementReferences() {
    if (refMux.FastLock() == NoError) {
        ++referenceCounter;
    }
    refMux.FastUnLock();
}

Object *Object::Clone() const {
    return NULL_PTR(Object *);
}

uint32 Object::NumberOfReferences() const {
    return referenceCounter;
}

/*lint -e{715} data is not used as this is not implemented on purpose*/
bool Object::Initialise(const StructuredData &data) {
    return false;
}

//LCOV_EXCL_START
/*lint -e{715} size is not used as this is not implemented on purpose*/
void *Object::operator new(const osulong size) throw () {
    return NULL_PTR(Object *);
}
//LCOV_EXCL_STOP

void Object::GetIntrospectionCopy(Introspection &destination) const {
    destination = introspection;
}

const char8 * const Object::GetName() const {
    return name;
}

void Object::GetUniqueName(char8 * const destination,
                           const uint32 &size) const {
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
        shiftBits = nOfPtrChars * 8u;
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
        shiftBits = nOfPtrChars * 8u;
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
    if (name != NULL_PTR(char8 *)) {
        /*lint -e{929} cast required to be able to use Memory::Free interface.*/
        bool ok = HeapManager::Free(reinterpret_cast<void *&>(name));
        if (!ok) {
            //TODO error here.
        }
    }
    name = StringHelper::StringDup(newName);
}

CLASS_REGISTER(Object, "1.0")
