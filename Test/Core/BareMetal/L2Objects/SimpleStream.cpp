/**
 * @file SimpleStream.cpp
 * @brief Header file for class AnyType
 * @date 24 Jun 2019
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

#include "SimpleStream.h"

namespace MARTe{

SimpleStream::SimpleStream(CCString init):DynamicCString(init){
	index = 0;
}
void SimpleStream::Init(CCString init){
	index = 0;
	*this = init;
}
bool SimpleStream::CanRead() const{
	return true;
}
bool SimpleStream::CanSeek() const{
	return true;
}
bool SimpleStream::CanWrite() const{
	return true;
}
bool SimpleStream::Read(char8 * const output,uint32 & size){
	char8* array = GetList();
	uint32 amount = 0;
	while ((amount < size) && (array[index] != 0)){
		output[amount++] = array[index++];
	}
	size = amount;
	return true;
}

bool SimpleStream::Seek(uint64 pos){
	index = static_cast<uint32>(pos);
	return true;
}

bool SimpleStream::Write(const char8 * const input, uint32 & size){
	index = 0;
	uint32 pos = 0;
	while (pos < size){
    	(*this)().Append(input[pos++]);
	}
	size -= pos;
	return true;
}

bool SimpleStream::SetSize(uint64 size){
	if (size == 0){
		index = 0;
		(*this)=="";
    	return true;
	}
	return false;
}

} //MARTe
