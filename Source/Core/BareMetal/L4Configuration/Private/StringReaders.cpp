/**
 * @file StringReaders.cpp
 * @brief Header file for class AnyType
 * @date Apr 22, 2020
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

#include "StringReaders.h"

namespace MARTe{

StreamReader::StreamReader(uint32 streamSizeIn):streamSize(streamSizeIn){

}

StreamReader::~StreamReader(){

}

CCString StreamReader::GetToken(const uint8 *ptr,uint32 index){
	const CCString separators=" \r\n\t";
	bool ok = true;

	buffer().Truncate(0);
	if ((index == 0) || (streamSize > 0)) {
		uint8 *ptr8 = const_cast<uint8 *>(ptr);
		StreamI *stream = reinterpret_cast<StreamI *>(ptr8+streamSize*index);
		buffer().Truncate(0);
		if (stream != NULL){
			char8 c = 0;
			uint32 size =1 ;
			ok = stream->Read(&c,size);
			while ( ok && (size == 1) && (separators.In(c))){
				size = 1;
				ok = stream->Read(&c,size);
			}
            while ( ok && (size == 1) && (!separators.In(c))){
                    buffer().Append(c);
                    size = 1;
                    ok = stream->Read(&c,size);
			}
		}
	}
	return CCString(buffer.GetList());
}

CCString SStringReader::GetToken(const uint8 *ptr,uint32 index){
	uint8 *ptrc = const_cast<uint8 *>(ptr);
	StreamString *ss = reinterpret_cast<StreamString *>(ptrc);
	return CCString(ss[index].Buffer());
}

CCString CCStringReader::GetToken(const uint8 *ptr,uint32 index){
	const CCString *ccs = reinterpret_cast<const CCString *>(ptr);
	return ccs[index];
}


} //MARTe
