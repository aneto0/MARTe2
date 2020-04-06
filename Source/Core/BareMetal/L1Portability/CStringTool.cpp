/**
 * @file CStringTool.cpp
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

#include "CStringTool.h"
#include "CCString.h"

namespace MARTe{

bool CStringTool::CanWrite() const {
	return true;
}

bool CStringTool::Write(const char8 * const input, uint32 & size){
	CCString inputS(input);
	uint32 oldSize = CStringTool::GetSize();
	CStringTool::Append(inputS,size);
	size = CStringTool::GetSize() - oldSize;
	return (size > 0);
}

bool CStringTool::Write(const char8 * const input,uint32 & size,const MilliSeconds &timeout){
	return Write(input,size);
}

bool CStringTool::SetSize(uint64 size){
	return Truncate(size).ret;
}


/*
bool CStringTool::StreamOutTo(StreamI &stream) const{
	uint32 size = this->index * this->elementSize;
	const char8 *ptr = reinterpret_cast<char8 *>(dataPointer);
	return stream.Write(ptr,size);
}*/

} //MARTe
