
#include "BitSet.h"


namespace MARTe{

BitSet::BitSet() {
    bytesSize = 1;
    bytes = new uint32[1];
    bytes[0] = 0;
}

BitSet::BitSet(uint32 value){
    bytesSize = 1;
    bytes = new uint32[1];
    bytes[0] = value;
}

BitSet::BitSet(uint64 value) {
    bytesSize = 2;
    bytes = new uint32[2];
    bytes[0] = value;
    bytes[1] = value >> 32;
}

BitSet::BitSet(const BitSet &other) {
    bytes = new uint32[other.bytesSize];
    bytesSize = other.bytesSize;
    for (uint32 i = 0; i < bytesSize; i++) {
        bytes[i] = other.bytes[i];
    }
}

BitSet::BitSet(uint32 *bytes, uint32 size){
    bytesSize = size;
    this->bytes = new uint32[size];
    for (uint32 i = 0; i < size; i++) {
        this->bytes[i] = bytes[i];
    }
}


BitSet::~BitSet(){
    delete[] bytes;
}

uint32 BitSet::GetByteSize() {
    return bytesSize;
}

bool BitSet::Bit(uint32 index) {
    uint32 byte_index = index / 32;
    if (byte_index < bytesSize) {
        uint32 local_index = index - (32 * byte_index);
        uint32 byte =  bytes[byte_index];
        return byte & ((uint32) 1 << local_index);
    }
    return false;
}

void BitSet::Set(uint32 index, bool value) {
    uint32 byte_index = index / 32;
    if (byte_index >= bytesSize) {
        bytes = (uint32*)realloc(bytes, sizeof(uint32) * (byte_index + 1));
        for (uint32 i = bytesSize; i <= byte_index; i++) {
            bytes[i] = 0;
        }
        bytesSize = byte_index + 1;
    }
    uint32 local_index = index - (32 * byte_index);
    if (value) {
        bytes[byte_index] |= 1u << local_index;
    } else {
        bytes[byte_index] &= ~ ((uint32)1 << local_index);
    }
}

BitSet& BitSet::operator=(const BitSet& other) {
    if (bytes != NULL) delete[] bytes;
    bytesSize = other.bytesSize;
    bytes = new uint32[other.bytesSize];
    
    for (uint32 i = 0; i < other.bytesSize; i++) {
        bytes[i] = other.bytes[i];
    }
    return *this;
}

BitSet& BitSet::operator=(const uint32& other) {
    if (bytes != NULL) delete[] bytes;
    bytesSize = 1;
    bytes = new uint32[1];
    bytes[0] = other;
    return *this;
}

BitSet& BitSet::operator=(const uint64& other) {
    if (bytes != NULL) delete[] bytes;
    bytesSize = 2;
    bytes = new uint32[2];
    bytes[0] = other;
    bytes[1] = other >> 32;
    return *this;
}

BitSet::operator uint32() const{
    if (bytesSize) {
        return bytes[0];
    }
    return 0;
}

BitSet::operator uint64() const{
    uint64 v = 0;
    if (bytesSize){
        v = bytes[0];
    }
    if (bytesSize == 2) {
        v += ((uint64)bytes[1]) << 32;
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
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
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
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
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
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
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
    uint32 bytes[bytesSize];
    for (uint32 i = 0; i < bytesSize; i++) {
        bytes[i] = ~bytes[i];
    }
    return BitSet(bytes, bytesSize);
}

BitSet BitSet::operator<<(const int& rhb){
    uint32 new_size = bytesSize + rhb/ 32;
    uint32 bytes[new_size];
    BitSet bs(bytes, new_size);
    for (uint32 i = 0; i < 32 * bytesSize; i++) {
        bs.Set(i+rhb, Bit(i));
    }
    return bs;
}

BitSet BitSet::operator>>(const int& rhb) {
    uint32 bytes[bytesSize];
    BitSet bs(bytes, bytesSize);
    for (uint32 i = rhb; i < 32 * bytesSize; i++) {
        bs.Set(i-rhb, Bit(i));
    }
    return bs;
}


bool BitSet::operator==(const BitSet& rhm) const{
    uint32 common = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 max_length = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;

    for (uint32 i = 0; i < common; i++){
        if (rhm.bytes[i] != bytes[i]){
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

bool BitSet::operator==(const uint64& rhm) const {
    if (bytesSize == 0) {
        return rhm == 0;
    }
    if (bytesSize == 1 && (rhm >> 32) != 0) return false;
    if (bytes[0] != (rhm & 0xffffffffu)) return false;
    if (bytes[1] != rhm >> 32) return false;
    for (uint32 i = 2; i < bytesSize; i++) {
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

bool BitSet::operator!=(const uint64& rhm) const{
    return !(*this == rhm);
}

BitSet & BitSet::operator|=(const BitSet& rhm){
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 * bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] | rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] this->bytes;
    this->bytes = bytes;
    bytesSize = size;
    return *this;
}
BitSet & BitSet::operator&=(const BitSet& rhm){
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 *bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] & rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = 0;
    }
    delete[] this->bytes;
    this->bytes = bytes;
    bytesSize = size;
    return *this;
}

BitSet & BitSet::operator^=(const BitSet& rhm) {
    uint32 msize = bytesSize < rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 size = bytesSize > rhm.bytesSize ? bytesSize : rhm.bytesSize;
    uint32 *bytes = new uint32[size];
    for (uint32 i = 0; i < msize; i++) {
        bytes[i] = this->bytes[i] ^ rhm.bytes[i];
    }      
    for (uint32 i = msize; i < size; i++) {
        bytes[i] = longer(*this, rhm).bytes[i];
    }
    delete[] this->bytes;
    this->bytes = bytes;
    bytesSize = size;
    return *this;
}

}
