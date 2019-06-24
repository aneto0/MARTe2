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

bool ZeroTerminatedArrayStaticTools::ZTAIsZero(const uint8 *pointer, uint32 elSize){
    bool isZero = false;
    if (pointer == NULL_PTR(const uint8 *)){
    	isZero = true;
    } else {
    	if (pointer[0] == 0){
    		uint32 ix = 1;
    		while ((ix < elSize) && (pointer[ix] == 0)){
    			ix++;
    		}
    		isZero = (ix == elSize);
    	}
    }
	return isZero;
}

void ZeroTerminatedArrayStaticTools::ZTAZero(uint8 *pointer, uint32 elSize){
    if (pointer != NULL_PTR(const uint8 *)){
    	while(elSize > 0){
    		*pointer++ = 0;
    		elSize--;
    	}
    }
}

bool ZeroTerminatedArrayStaticTools::ZTAIsSame(const uint8 *pointer,const uint8 *data, uint32 elSize){
	bool IsSame = true;
    if ((pointer != NULL_PTR(const uint8 *)) && (data != NULL_PTR(const uint8 *))){
    	while((elSize > 3)&& IsSame){
    		IsSame = (*((uint32 *)pointer) == *((uint32 *)data));
    		elSize-=4;
    		pointer+=4;
    		data+=4;
    	}
    	while((elSize > 0)&& IsSame){
    		IsSame = (*pointer == *data);
    		elSize--;
    		pointer++;
    		data++;
    	}
    }
	return IsSame;
}

/*******************************************************************
 *
 *   ZTAISSameArray
 *
 ********************************************************************/
#if 0
template <typename T>
static inline bool ZTAIsSameArrayLT(const T *pointer,const T *array, uint32 limit){
    bool same = true;
    if (pointer != array){
    	if ((pointer != NULL_PTR(uint8 *))&&(array != NULL_PTR(uint8 *))) {
            while ((pointer[0]!=0U) && same && (limit > 0)) {
                same = (*pointer++ == *array++);
                limit--;
            }
            // if sane and limit > 0 it means we reached a terminator on listP. Check list2P
            if (same && (limit > 0)){
            	same = (array[0]==0U);
            }

    	} else {
    		same = false;
    	}
    }
    return same;
}

bool ZeroTerminatedArrayStaticTools::ZTAIsSameArray2(const uint16 *pointer,const uint16 *array, uint32 limit){
	return ZTAIsSameArrayLT<uint16>(pointer,array, limit);
}

bool ZeroTerminatedArrayStaticTools::ZTAIsSameArray4(const uint32 *pointer,const uint32 *array, uint32 limit){
	return ZTAIsSameArrayLT<uint32>(pointer,array, limit);
}

bool ZeroTerminatedArrayStaticTools::ZTAIsSameArray8(const uint64 *pointer,const uint64 *array, uint32 limit){
	return ZTAIsSameArrayLT<uint64>(pointer,array, limit);
}

bool ZeroTerminatedArrayStaticTools::ZTAIsSameArray(const uint8 *pointer,const uint8 *array, uint32 elSize,uint32 limit){
    bool same = true;
    if ((pointer != NULL_PTR(uint8 *))&&(array != NULL_PTR(uint8 *))) {
        while (!ZTAIsZero(pointer,elSize) && same && (limit > 0)) {
            same = ZTAIsSame(pointer,array,elSize);
            pointer+=elSize;
            array+=elSize;
            limit--;
        }
        // if sane and limit > 0 it means we reached a terminator on listP. Check list2P
        if (same && (limit > 0)){
        	same = ZTAIsZero(array,elSize);
        }
    }
    return same;
}
#endif

/*******************************************************************
 *
 *   xxx
 *
 ********************************************************************/



// calculates size of a generic ZeroTermArray
uint32 ZeroTerminatedArrayStaticTools::ZTAGetSize(const uint8 *pointer, uint32 elSize){
	uint32 size = 0;
	bool end = false;
    if (pointer != NULL_PTR(const uint8 *)){
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
    }
	return size;
};



uint32 ZeroTerminatedArrayStaticTools::ZTAFind(const uint8 *pointer,const uint8 *data, uint32 elSize){
    uint32 pos = 0xFFFFFFFFu;
    if (pointer != NULL_PTR(const uint8 *)) {
        uint32 index = 0;
        while (!ZTAIsZero(pointer,elSize) && (pos == 0xFFFFFFFFu)) {
        	if (ZTAIsSame(data,pointer,elSize)){
        		pos = index;
        	}
            index++;
            pointer+=elSize;
        }
    }
    return pos;
}








} //MARTe
