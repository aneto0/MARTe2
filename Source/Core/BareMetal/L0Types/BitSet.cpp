/**
 * @file BitSet.cpp
 * @brief Source file for class BitSet
 * @date 04/02/2021
 * @author Martino Ferrari
 *
 * @copyright Copyright 2021 ITER.
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
 * the class BitSet (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BitSet.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static BitSet Longer(const BitSet a, const BitSet b) {
    BitSet result = a;
    if (b.GetNumberOfElements() > a.GetNumberOfElements()) {
        result = b;
    }
    return result;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BitSet::BitSet() {
    nElements = 1u;
    array = new uint32[1u];
    array[0u] = 0u;
}

BitSet::BitSet(const uint32 value) {
    nElements = 1u;
    array = new uint32[1u];
    array[0u] = value;
}

BitSet::BitSet(const uint64 value) {
    nElements = 2u;
    array = new uint32[nElements];
    array[0u] = static_cast<uint32>(value);
    array[1u] = static_cast<uint32>(value >> 32u);
}

BitSet::BitSet(const BitSet &other) {
    array = new uint32[other.nElements];
    nElements = other.nElements;
    for (uint32 i = 0u; i < nElements; i++) {
        array[i] = other.array[i];
    }
}

BitSet::BitSet(const uint32 marray[], const uint32 size) {
    nElements = size;
    array = new uint32[size];
    for (uint32 i = 0u; i < size; i++) {
        array[i] = marray[i];
    }
}

BitSet::~BitSet() {
    delete[] array;
}

void BitSet::Resize(const uint32 size) {
    uint32 * newArray = new uint32[size];
    uint32 commonSize = TypeCharacteristics<uint32>::Min(size, nElements);
    for (uint32 i = 0u; i < commonSize; i++) {
        newArray[i] = array[i];
    }
    for (uint32 i = commonSize; i < size; i++) {
        newArray[i] = 0u;
    }
    delete[] array;
    array = newArray;
    nElements = size;
}

uint32 BitSet::GetNumberOfElements() const {
    return nElements;
}

bool BitSet::Bit(const uint32 index) const {
    uint32 byteIndex = index / 32u;
    bool result = false;
    if (byteIndex < nElements) {
        uint32 localIndex = index - (32u * byteIndex);
        uint32 byte = array[byteIndex];
        result = (byte & (TypeCharacteristics<uint32>::One() << localIndex)) != 0u;
    }
    return result;
}

void BitSet::Set(const uint32 index, const bool value) {
    uint32 byteIndex = index / 32u;
    if (byteIndex >= nElements) {
        Resize(byteIndex + 1u);
    }
    uint32 localIndex = index - (32u * byteIndex);
    uint32 bitmask = TypeCharacteristics<uint32>::One() << localIndex;
    if (value) {
        if (byteIndex < nElements) {
            array[byteIndex] |= bitmask;
        }
    }
    else {
        if (byteIndex < nElements) {
            array[byteIndex] &= ~bitmask;
        }
    }
}

BitSet& BitSet::operator=(const BitSet& other) {
    if (&other != this) {
        if (array != NULL) {
            delete[] array;
        }
        nElements = other.nElements;
        array = new uint32[other.nElements];

        for (uint32 i = 0u; i < other.nElements; i++) {
            array[i] = other.array[i];
        }
    }
    return *this;
}

BitSet& BitSet::operator=(const uint32& other) {
    if (array != NULL) {
        delete[] array;
    }
    nElements = 1u;
    array = new uint32[nElements];
    array[0] = other;
    return *this;
}

BitSet& BitSet::operator=(const uint64& other) {
    if (array != NULL) {
        delete[] array;
    }
    nElements = 2u;
    array = new uint32[nElements];
    array[0] = static_cast<uint32>(other);
    array[1] = static_cast<uint32>(other >> 32u);
    return *this;
}

BitSet::operator uint32() const {
    uint32 value = 0u;
    if (nElements > 0u) {
        value = array[0u];
    }
    return value;
}

BitSet::operator uint64() const {
    uint64 v = 0u;
    if (nElements > 0u) {
        v = array[0u];
    }
    if (nElements == 2u) {
        v += static_cast<uint64>(array[1u]) << 32u;
    }
    return v;
}

/*lint -e{772} -e{9135} newArray correctly initialized and it is not an unary operator!*/
BitSet BitSet::operator&(const BitSet& rhm) const {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 * newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] & rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = 0u;
    }

    BitSet res(newArray, size);
    delete[] newArray;
    return res;
}

/*lint -e{772} newArray correctly initialized*/
BitSet BitSet::operator|(const BitSet& rhm) const {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // nElements : rhm.nElements;
    uint32 * newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] | rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = Longer(*this, rhm).array[i];
    }
    BitSet res(newArray, size);
    delete[] newArray;
    return res;
}

/*lint -e{772} newArray correctly initialized*/
BitSet BitSet::operator^(const BitSet& rhm) const {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // nElements : rhm.nElements;
    uint32 * newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] ^ rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = Longer(*this, rhm).array[i];
    }
    BitSet res(newArray, size);
    delete[] newArray;
    return res;
}

/*lint -e{772} newArray correctly initialized*/
BitSet BitSet::operator~() const {
    uint32 * newArray = new uint32[nElements];
    for (uint32 i = 0u; i < nElements; i++) {
        newArray[i] = ~array[i];
    }
    BitSet res(newArray, nElements);
    delete[] newArray;
    return res;
}

/*lint -e{771} newArray is correctly initialized*/
BitSet BitSet::operator<<(const uint32& rhm) const {
    uint32 newSize = nElements + (rhm / 32u) + 1u;
    uint32 * newArray = new uint32[newSize];
    for (uint32 i = 0u; i < newSize; i++) {
        newArray[i] = 0u;
    }
    uint32 bitsize = 32u * nElements;
    for (uint32 i = 0u; i < bitsize; i++) {
        if (Bit(i)) {
            uint32 byteIndex = (i + rhm) / 32u;
            uint32 bitIndex = (i + rhm) - (byteIndex * 32u);
            if (byteIndex < newSize) {
                newArray[byteIndex] |= TypeCharacteristics<uint32>::One() << bitIndex;
            }
        }
    }
    BitSet bs(newArray, newSize);
    delete[] newArray;
    return bs;
}

/*lint -e{771} newArray is correctly initialized*/
BitSet BitSet::operator>>(const uint32& rhm) const {
    uint32 * newArray = new uint32[nElements];
    uint32 bitsize = 32u * nElements;
    for (uint32 i = 0u; i < nElements; i++) {
        newArray[i] = 0u;
    }
    for (uint32 i = rhm; i < bitsize; i++) {
        if (Bit(i)) {
            uint32 byteIndex = (i - rhm) / 32u;
            uint32 bitIndex = (i - rhm) - (byteIndex * 32u);
            if (byteIndex < nElements) {
                newArray[byteIndex] |= TypeCharacteristics<uint32>::One() << bitIndex;
            }
        }
    }
    BitSet bs(newArray, nElements);
    delete[] newArray;
    return bs;
}

bool BitSet::operator==(const BitSet& rhm) const {
    uint32 common = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 maxLength = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    bool result = true;
    for (uint32 i = 0u; i < common; i++) {
        if (rhm.array[i] != array[i]) {
            result = false;
        }
    }
    if (result && (common < maxLength)) {
        for (uint32 i = common; i < maxLength; i++) {
            if (Longer(*this, rhm).array[i] != 0u) {
                result = false;
            }
        }
    }
    return result;
}

bool BitSet::operator==(const uint32& rhm) const {
    bool result = true;
    if (nElements == 0u) {
        result = rhm == 0u;
    }
    else if (array[0] != rhm) {
        result = false;
    }
    else {
        for (uint32 i = 1u; i < nElements; i++) {
            if (array[i] != 0u) {
                result = false;
                break;
            }
        }
    }
    return result;
}

bool BitSet::operator==(const uint64& rhm) const {
    bool result = true;
    if (nElements == 0u) {
        result = rhm == 0u;
    }
    else if (((nElements == 1u) && ((rhm >> 32) != 0u)) || (array[0] != (rhm & 0xffffffffu)) || (array[1] != (rhm >> 32u))) {
        result = false;
    }
    else {
        for (uint32 i = 2u; i < nElements; i++) {
            /*lint -e{661} the size of array is always guaranteed to be < nElements*/
            result = result && (array[i] == 0u);
        }
    }
    return result;
}

bool BitSet::operator!=(const BitSet& rhm) const {
    return !(*this == rhm);
}

bool BitSet::operator!=(const uint32& rhm) const {
    return !(*this == rhm);
}

bool BitSet::operator!=(const uint64& rhm) const {
    return !(*this == rhm);
}

BitSet & BitSet::operator|=(const BitSet& rhm) {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 * newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] | rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = Longer(*this, rhm).array[i];
    }
    delete[] array;
    array = newArray;
    nElements = size;
    return *this;
}
BitSet & BitSet::operator&=(const BitSet& rhm) {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 *newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] & rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = 0u;
    }
    delete[] array;
    array = newArray;
    nElements = size;
    return *this;
}

BitSet & BitSet::operator^=(const BitSet& rhm) {
    uint32 msize = TypeCharacteristics<uint32>::Min(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 size = TypeCharacteristics<uint32>::Max(nElements, rhm.nElements); // ? nElements : rhm.nElements;
    uint32 *newArray = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        newArray[i] = array[i] ^ rhm.array[i];
    }
    for (uint32 i = msize; i < size; i++) {
        newArray[i] = Longer(*this, rhm).array[i];
    }
    delete[] array;
    array = newArray;
    nElements = size;
    return *this;
}

uint32 *BitSet::GetArray() {
    return array;
}


}
