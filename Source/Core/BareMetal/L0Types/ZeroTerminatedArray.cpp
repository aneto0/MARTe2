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
		}
	}
}


} //MARTe
