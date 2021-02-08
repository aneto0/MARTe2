
#include "BitSet.h"


namespace MARTe{

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


bool BitSet::operator==(const BitSet& rhm) const{
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

bool BitSet::operator==(const uint32& rhm) const {
    if (bytesSize == 0) {
        return rhm == 0;
    }
    if (bytes[0] != rhm) return false;
    for (uint32 i = 1; i < bytesSize; i++) {
        if (bytes[i] != 0) {
            return false;
        }
    }
    return true;
}


bool BitSet::operator!=(const BitSet& rhm) const{
    return !(*this == rhm);
}

bool BitSet::operator!=(const uint32& rhm) const{
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
