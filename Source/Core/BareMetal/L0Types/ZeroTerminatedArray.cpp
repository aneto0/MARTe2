/**
 * @file ZeroTerminatedArray.cpp
 * @brief Header file for class AnyType
 * @date 28 Sep 2017
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

#include "ZeroTerminatedArray.h"

namespace MARTe{

bool ZeroTerminatedArrayIsZero(const uint8 *pointer, uint32 elSize){
    bool isZero = false;
	if (pointer[0] == 0){
		uint32 ix = 1;
		while ((ix < elSize) && (pointer[ix] == 0)){
			ix++;
		}
		isZero = (ix == elSize);
	}
	return isZero;
}

void ZeroTerminatedArrayZero(uint8 *pointer, uint32 elSize){
	while(elSize > 0){
		*pointer++ = 0;
	}
}


bool ZeroTerminatedArrayIsSame(const uint8 *pointer,const uint8 *data, uint32 elSize){
	bool isSame = true;
	while((elSize > 3)&& isSame){
		isSame = (*((uint32 *)pointer) == *((uint32 *)data));
		elSize-=4;
		pointer+=4;
		data+=4;
	}
	while((elSize > 0)&& isSame){
		isSame = (*pointer == *data);
		elSize--;
		pointer++;
		data++;
	}
	return isSame;
}


// calculates size of a generic ZeroTermArray
uint32 ZeroTerminatedArrayGetSize(const uint8 *pointer, uint32 elSize){
	uint32 size = 0;
	bool end = false;
	while (!end){
		if (pointer[0] == 0){
			uint32 ix = 1;
			while ((ix < elSize) && (pointer[ix] == 0)){
				ix++;
			}
			end = (ix == elSize);
		}

		if (!end){
			pointer+= elSize;
			size++;
		}
	}
	return size;
};

uint32 ZeroTerminatedArrayFind(const uint8 *pointer,const uint8 *data, uint32 elSize){
    uint32 pos = 0xFFFFFFFFu;
    if (pointer != NULL_PTR(const uint8 *)) {
        uint32 index = 0;
        while (!ZeroTerminatedArrayIsZero(pointer,elSize) && (pos == 0xFFFFFFFFu)) {
        	if (ZeroTerminatedArrayIsSame(data,pointer,elSize)){
        		pos = index;
        	}
            index++;
            pointer+=elSize;
        }
    }
    return pos;

}


} //MARTe
