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

static BitSet longer(const BitSet a, const BitSet b) {
    BitSet result = a;
    if (b.GetByteSize() > a.GetByteSize()) {
        result = b;
    }
    return result;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BitSet::BitSet() {
    bytesSize = 1u;
    bytes = new uint32[1u];
    bytes[0u] = 0u;
}

BitSet::BitSet(const uint32 value) {
    bytesSize = 1u;
    bytes = new uint32[1u];
    bytes[0u] = value;
}

BitSet::BitSet(const uint64 value) {
    bytesSize = 2u;
    bytes = new uint32[bytesSize];
    bytes[0u] = static_cast<uint32>(value);
    bytes[1u] = static_cast<uint32>(value >> 32u);
}

BitSet::BitSet(const BitSet &other) {
    bytes = new uint32[other.bytesSize];
    bytesSize = other.bytesSize;
    for (uint32 i = 0u; i < bytesSize; i++) {
        bytes[i] = other.bytes[i];
    }
}

BitSet::BitSet(const uint32 mbytes[], const uint32 size) {
    bytesSize = size;
    bytes = new uint32[size];
    for (uint32 i = 0u; i < size; i++) {
        bytes[i] = mbytes[i];
    }
}

BitSet::~BitSet() {
    delete[] bytes;
}

void BitSet::resize(const uint32 size) {
    uint32 * new_bytes = new uint32[size];
    uint32 common_size = MIN(size, bytesSize);
    for (uint32 i = 0u; i < common_size; i++) {
        new_bytes[i] = bytes[i];
    }
    for (uint32 i = common_size; i < size; i++) {
        new_bytes[i] = 0u;
    }
    delete[] bytes;
    bytes = new_bytes;
    bytesSize = size;
}

uint32 BitSet::GetByteSize() const {
    return bytesSize;
}

bool BitSet::Bit(const uint32 index) const {
    uint32 byte_index = index / 32u;
    bool result = false;
    if (byte_index < bytesSize) {
        uint32 local_index = index - (32u * byte_index);
        uint32 byte = bytes[byte_index];
        result = (byte & (ONE << local_index)) != 0u;
    }
    return result;
}

void BitSet::Set(const uint32 index, const bool value) {
    uint32 byte_index = index / 32u;
    if (byte_index >= bytesSize) {
        resize(byte_index + 1u);
    }
    uint32 local_index = index - (32u * byte_index);
    uint32 bitmask = ONE << local_index;
    if (value) {
        if (byte_index < bytesSize) {
            bytes[byte_index] |= bitmask;
        }
    }
    else {
        if (byte_index < bytesSize) {
            bytes[byte_index] &= ~bitmask;
        }
    }
}

BitSet& BitSet::operator=(const BitSet& other) {
    if (&other != this) {
        if (bytes != NULL) {
            delete[] bytes;
        }
        bytesSize = other.bytesSize;
        bytes = new uint32[other.bytesSize];

        for (uint32 i = 0u; i < other.bytesSize; i++) {
            bytes[i] = other.bytes[i];
        }
    }
    return *this;
}

BitSet& BitSet::operator=(const uint32& other) {
    if (bytes != NULL) {
        delete[] bytes;
    }
    bytesSize = 1u;
    bytes = new uint32[bytesSize];
    bytes[0] = other;
    return *this;
}

BitSet& BitSet::operator=(const uint64& other) {
    if (bytes != NULL) {
        delete[] bytes;
    }
    bytesSize = 2u;
    bytes = new uint32[bytesSize];
    bytes[0] = static_cast<uint32>(other);
    bytes[1] = static_cast<uint32>(other >> 32u);
    return *this;
}

BitSet::operator uint32() const {
    uint32 value = 0u;
    if (bytesSize > 0u) {
        value = bytes[0u];
    }
    return value;
}

BitSet::operator uint64() const {
    uint64 v = 0u;
    if (bytesSize > 0u) {
        v = bytes[0u];
    }
    if (bytesSize == 2u) {
        v += static_cast<uint64>(bytes[1u]) << 32u;
    }
    return v;
}

/*lint -e772 -e9135 new_bytes correctly initialized and it is not an unary operator!*/
BitSet BitSet::operator&(const BitSet& rhm) const {
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 * new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] & rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = 0u;
    }

    BitSet res(new_bytes, size);
    delete[] new_bytes;
    return res;
}

/*lint -e772 new_bytes correctly initialized*/
BitSet BitSet::operator|(const BitSet& rhm) const {
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // bytesSize : rhm.bytesSize;
    uint32 * new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] | rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = longer(*this, rhm).bytes[i];
    }
    BitSet res(new_bytes, size);
    delete[] new_bytes;
    return res;
}

/*lint -e772 new_bytes correctly initialized*/
BitSet BitSet::operator^(const BitSet& rhm) const {
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // bytesSize : rhm.bytesSize;
    uint32 * new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] ^ rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = longer(*this, rhm).bytes[i];
    }
    BitSet res(new_bytes, size);
    delete[] new_bytes;
    return res;
}

/*lint -e772 new_bytes correctly initialized*/
BitSet BitSet::operator~() const {
    uint32 * new_bytes = new uint32[bytesSize];
    for (uint32 i = 0u; i < bytesSize; i++) {
        new_bytes[i] = ~bytes[i];
    }
    BitSet res(new_bytes, bytesSize);
    delete[] new_bytes;
    return res;
}

/*lint -e771 new_bytes is correctly initialized*/
BitSet BitSet::operator<<(const uint32& rhm) const {
    uint32 new_size = bytesSize + (rhm / 32u) + 1u;
    uint32 * new_bytes = new uint32[new_size];
    for (uint32 i = 0u; i < new_size; i++) {
        new_bytes[i] = 0u;
    }
    uint32 bitsize = 32u * bytesSize;
    for (uint32 i = 0u; i < bitsize; i++) {
        if (Bit(i)) {
            uint32 byte_ind = (i + rhm) / 32u;
            uint32 bit_ind = (i + rhm) - (byte_ind * 32u);
            if (byte_ind < new_size) {
                new_bytes[byte_ind] |= ONE << bit_ind;
            }
        }
    }
    BitSet bs(new_bytes, new_size);
    delete[] new_bytes;
    return bs;
}

/*lint -e771 new_bytes is correctly initialized*/
BitSet BitSet::operator>>(const uint32& rhm) const {
    uint32 * new_bytes = new uint32[bytesSize];
    uint32 bitsize = 32u * bytesSize;
    for (uint32 i = 0u; i < bytesSize; i++) {
        new_bytes[i] = 0u;
    }
    for (uint32 i = rhm; i < bitsize; i++) {
        if (Bit(i)) {
            uint32 byte_ind = (i - rhm) / 32u;
            uint32 bit_ind = (i - rhm) - (byte_ind * 32u);
            if (byte_ind < bytesSize) {
                new_bytes[byte_ind] |= ONE << bit_ind;
            }
        }
    }
    BitSet bs(new_bytes, bytesSize);
    delete[] new_bytes;
    return bs;
}

bool BitSet::operator==(const BitSet& rhm) const {
    uint32 common = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 max_length = MAX(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    bool result = true;
    for (uint32 i = 0u; i < common; i++) {
        if (rhm.bytes[i] != bytes[i]) {
            result = false;
        }
    }
    if (result && (common < max_length)) {
        for (uint32 i = common; i < max_length; i++) {
            if (longer(*this, rhm).bytes[i] != 0u) {
                result = false;
            }
        }
    }
    return result;
}

bool BitSet::operator==(const uint32& rhm) const {
    bool result = true;
    if (bytesSize == 0u) {
        result = rhm == 0u;
    }
    else if (bytes[0] != rhm) {
        result = false;
    }
    else {
        for (uint32 i = 1u; i < bytesSize; i++) {
            if (bytes[i] != 0u) {
                result = false;
                break;
            }
        }
    }
    return result;
}

bool BitSet::operator==(const uint64& rhm) const {
    bool result = true;
    if (bytesSize == 0u) {
        result = rhm == 0u;
    }
    else if (((bytesSize == 1u) && ((rhm >> 32) != 0u)) || (bytes[0] != (rhm & 0xffffffffu)) || (bytes[1] != (rhm >> 32u))) {
        result = false;
    }
    else {
        for (uint32 i = 2u; i < bytesSize; i++) {
            /*lint -e{661} the size of bytes is always guaranteed to be < bytesSize*/
            result = result && (bytes[i] != 0u);
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
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 * new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] | rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] bytes;
    bytes = new_bytes;
    bytesSize = size;
    return *this;
}
BitSet & BitSet::operator&=(const BitSet& rhm) {
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 *new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] & rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = 0u;
    }
    delete[] bytes;
    bytes = new_bytes;
    bytesSize = size;
    return *this;
}

BitSet & BitSet::operator^=(const BitSet& rhm) {
    uint32 msize = MIN(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 size = MAX(bytesSize, rhm.bytesSize); // ? bytesSize : rhm.bytesSize;
    uint32 *new_bytes = new uint32[size];
    for (uint32 i = 0u; i < msize; i++) {
        new_bytes[i] = bytes[i] ^ rhm.bytes[i];
    }
    for (uint32 i = msize; i < size; i++) {
        new_bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] bytes;
    bytes = new_bytes;
    bytesSize = size;
    return *this;
}

}