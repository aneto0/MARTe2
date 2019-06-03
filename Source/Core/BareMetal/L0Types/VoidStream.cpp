/**
 * @file VoidStream.cpp
 * @brief Header file for class AnyType
 * @date 3 Jun 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#define DLL_API
#include "VoidStream.h"

namespace MARTe{

VoidStream::VoidStream(){

}

VoidStream::~VoidStream(){

}

bool VoidStream::CanWrite() const{
	return false;
}


bool VoidStream::CanRead() const{
	return false;
}

bool VoidStream::CanSeek() const{
	return false;
}

bool VoidStream::Read(char8 * const output,uint32 & size){
	return false;
}

bool VoidStream::Write(const char8 * const input, uint32 & size){
	return false;
}

bool VoidStream::Read(char8 * const output,
        uint32 & size,
        const MilliSeconds &timeout){
	return false;
}

bool VoidStream::Write(const char8 * const input,
        uint32 & size,
        const MilliSeconds &timeout){
	return false;
}

uint64 VoidStream::Size() const{
	return 0;
}

bool VoidStream::Seek(uint64 pos){
	return false;
}

bool VoidStream::RelativeSeek(const int64 deltaPos){
	return false;
}

uint64 VoidStream::Position() const{
	return 0;
}

bool VoidStream::SetSize(uint64 size){
	return false;
}

} //MARTe
