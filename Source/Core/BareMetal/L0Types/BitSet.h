/**
 * @file BitSet.h
 * @brief Header file for class BitSet
 * @date 04/02/2021
 * @author Martino Ferrari
 *
 * @copyright ITER | International Thermonuclear Experimental Reactor.
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class BitSet
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPESDEV_BITSET_H_
#define L0TYPESDEV_BITSET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *  @brief A variable size bit set compatible with standard bitwise operations.
 *  @details Using this type allows to create bitmask of albitrary size.
 */
class BitSet {
public:
    BitSet();
    BitSet(uint32);
    BitSet(uint64);
    BitSet(const BitSet &other);
    BitSet(uint32* bytes, uint32 size);

    ~BitSet();

    uint32 GetByteSize();

    bool Bit(uint32 index);
    void Set(uint32 index, bool value);
    
    BitSet & operator=(const BitSet& other);
    BitSet & operator=(const uint32& other);
    BitSet & operator=(const uint64& other);

    operator uint32() const;
    operator uint64() const;

    BitSet operator&(const BitSet& rhm);
    BitSet operator|(const BitSet& rhm);
    BitSet operator^(const BitSet& rhm);
    BitSet operator~();

    BitSet operator<<(const int& rhf);
    BitSet operator>>(const int& rhm);

    bool operator==(const BitSet& rhm);
    bool operator==(const uint32& rhm);
    bool operator!=(const BitSet& rhm);
    bool operator!=(const uint32& rhm);

    BitSet & operator|=(const BitSet& rhm);
    BitSet & operator&=(const BitSet& rhm);
    BitSet & operator^=(const BitSet& rhm);
private:
    uint32 bytesSize;
    uint32 * bytes;

};

/*---------------------------------------------------------------------------*/
/*                        Methods Implementation                             */
/*---------------------------------------------------------------------------*/

BitSet::BitSet() {
    this->bytesSize = 1;
    this->bytes = new uint32[1];
    this->bytes[0] = 0;
}

BitSet::BitSet(uint32 value){
    this->bytesSize = 1;
    this->bytes = new uint32[1];
    this->bytes[0] = value;
}

BitSet::BitSet(uint64 value) {
    this->bytesSize = 2;
    this->bytes = new uint32[2];
    this->bytes[0] = value;
    this->bytes[1] = value >> 32;
}

BitSet::BitSet(const BitSet &other) {
    this->bytes = new uint32[other.bytesSize];
    this->bytesSize = other.bytesSize;
    for (uint32 i = 0; i < other.bytesSize; i++) {
        this->bytes[i] = other.bytes[i];
    }
}

BitSet::BitSet(uint32 *bytes, uint32 size){
    this->bytesSize = size;
    this->bytes = new uint32[size];
    for (uint32 i = 0; i < size; i++) {
        this->bytes[i] = bytes[i];
    }
}


BitSet::~BitSet(){
    delete[] this->bytes;
}

uint32 BitSet::GetByteSize() {
    return this->bytesSize;
}

bool BitSet::Bit(uint32 index) {
    uint32 byte_index = index / 32;
    if (byte_index < this->bytesSize) {
        uint32 local_index = index - (32 * byte_index);
        uint32 byte =  this->bytes[byte_index];
        return byte & ((uint32) 1 << local_index);
    }
    return false;
}

void BitSet::Set(uint32 index, bool value) {
    uint32 byte_index = index / 32;
    if (byte_index >= this->bytesSize) {
        this->bytes = (uint32*)realloc(this->bytes, sizeof(uint32) * (byte_index + 1));
        for (uint32 i = this->bytesSize; i <= byte_index; i++) {
            this->bytes[i] = 0;
        }
        this->bytesSize = byte_index + 1;
    }
    uint32 local_index = index - (32 * byte_index);
    if (value) {
        this->bytes[byte_index] |= 1u << local_index;
    } else {
        this->bytes[byte_index] &= ~ ((uint32)1 << local_index);
    }
}

BitSet& BitSet::operator=(const BitSet& other) {
    this->bytesSize = other.bytesSize;
    this->bytes = new uint32[other.bytesSize];
    
    for (uint32 i = 0; i < other.bytesSize; i++) {
        this->bytes[i] = other.bytes[i];
    }
    return *this;
}

BitSet& BitSet::operator=(const uint32& other) {
    this->bytesSize = 1;
    this->bytes = new uint32[1];
    this->bytes[0] = other;
    return *this;
}

BitSet& BitSet::operator=(const uint64& other) {
    this->bytesSize = 2;
    this->bytes = new uint32[2];
    this->bytes[0] = other;
    this->bytes[1] = other >> 32;
    return *this;
}

BitSet::operator uint32() const{
    if (this->bytesSize) {
        return this->bytes[0];
    }
    return 0;
}

BitSet::operator uint64() const{
    uint64 v = 0;
    if (this->bytesSize){
        v = this->bytes[0];
    }
    if (this->bytesSize == 2) {
        v += ((uint64)this->bytes[1]) << 32;
    }
    return v;
}

BitSet longer(BitSet a, BitSet b){
    if (a.GetByteSize() > b.GetByteSize()) {
        return a;
    }
    return b;
}

BitSet BitSet::operator&(const BitSet& rhm){
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 bytes[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] & rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = 0;
    }

    return BitSet(bytes, size);
}

BitSet BitSet::operator|(const BitSet& rhm){
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 bytes[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] | rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    return BitSet(bytes, size);
}
    
BitSet BitSet::operator^(const BitSet& rhm){
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 bytes[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] ^ rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    return BitSet(bytes, size);
}

BitSet BitSet::operator~(){
    uint32 bytes[this->bytesSize];
    for (uint32 i = 0; i < this->bytesSize; i++) {
        bytes[i] = ~this->bytes[i];
    }
    return BitSet(bytes, this->bytesSize);
}

BitSet BitSet::operator<<(const int& rhb){
    uint32 new_size = this->bytesSize + rhb/ 32;
    uint32 bytes[new_size];
    BitSet bs(bytes, new_size);
    for (uint32 i = 0; i < 32 * this->bytesSize; i++) {
        bs.Set(i+rhb, this->Bit(i));
    }
    return bs;
}

BitSet BitSet::operator>>(const int& rhb) {
    uint32 bytes[this->bytesSize];
    BitSet bs(bytes, this->bytesSize);
    for (uint32 i = rhb; i < 32 * this->bytesSize; i++) {
        bs.Set(i-rhb, this->Bit(i));
    }
    return bs;
}


bool BitSet::operator==(const BitSet& rhm){
    uint32 common = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 max_length = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;

    for (uint32 i = 0; i < common; i++){
        if (rhm.bytes[i] != this->bytes[i]){
            return false;
        }
    }
    if (common < max_length) {
        for (uint32 i = common; i < max_length; i++){
            if (longer(*this, rhm).bytes[i] != 0){
                return false;
            }
        }
    }
    return true;
}

bool BitSet::operator==(const uint32& rhm){
    if (this->bytesSize == 0 && rhm != 0) return false;
    if (this->bytes[0] != rhm) return false;
    for (uint32 i = 1; i < this->bytesSize; i++) {
        if (this->bytes[i] != 0) return false;
    }
    return true;
}

bool BitSet::operator!=(const BitSet& rhm){
    return !(*this == rhm);
}

bool BitSet::operator!=(const uint32& rhm){
    return !(*this == rhm);
}

BitSet & BitSet::operator|=(const BitSet& rhm){
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 * bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] | rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] this->bytes;
    this->bytes = bytes;
    this->bytesSize = size;
    return *this;
}
BitSet & BitSet::operator&=(const BitSet& rhm){
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 *bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] & rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = 0;
    }
    delete[] this->bytes;
    this->bytes = bytes;
    this->bytesSize = size;
    return *this;
}

BitSet & BitSet::operator^=(const BitSet& rhm) {
    uint32 msize = this->bytesSize < rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 size = this->bytesSize > rhm.bytesSize ? this->bytesSize : rhm.bytesSize;
    uint32 *bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] ^ rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] this->bytes;
    this->bytes = bytes;
    this->bytesSize = size;
    return *this;
}

}
#endif // L0TYPESDEV_BITSET_H_
