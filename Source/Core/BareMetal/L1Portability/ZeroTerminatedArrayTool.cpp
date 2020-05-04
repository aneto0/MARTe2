/**
 * @file ZeroTerminatedArrayAppendTool.cpp
 * @brief Header file for class AnyType
 * @date 11 Dec 2018
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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "ZeroTerminatedArrayTool.h"
#include "AllocationPointer.h"
#include "ZeroTerminatedArray.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * When appending to a string the DZTA will add this amount of memory
 */
#define DZTA_APPEND_GRANULARITY 32u

#include <stdio.h>
void ZeroTerminatedArrayTool::Truncate(uint32 newIndex){
	if (dataPointer != NULL){
//printf("T0{%p %p %i %i %i}\n",dataPointer,dataPointerAddress,index,maxIndex,ret.format_as_integer);  //TODO
	    if (ret){
		    ret.invalidOperation = (newIndex > index);
		}
	    if (ret){
			/// shrink storage if too large
			if ((static_cast<uint32>(maxIndex) > (newIndex + DZTA_APPEND_GRANULARITY)) && (maxIndex > 0)){
		    	/// check for DynamicZTA
		    	if (dataPointerAddress != NULL){
		    		maxIndex = static_cast<int32>(newIndex);
		        	HeapManager::AllocationPointer p(dataPointer);
		        	ret = p.Realloc((maxIndex + 1)*elementSize);
		        	*dataPointerAddress = p;
		        	dataPointer = p;
		            if (!ret){
		            	maxIndex = 0;
//printf("failed:");   //TODO
		            }
		    	}
//printf("T{%p %p %i %i %i}\n",dataPointer,dataPointerAddress,index,maxIndex,ret.format_as_integer); TODO
			}
	    	Terminate(newIndex);
	    	index = newIndex;
	    }
	}
}

void ZeroTerminatedArrayTool::Append(const uint8 *data,uint32 dataSize){
//printf("{A}:");
	if ((data != NULL_PTR(const uint8 *))&& (dataSize > 0) && ret){

		uint32 temp = index + dataSize;
		int32 finalIndex = static_cast<int32>(temp);
		if (temp >= static_cast<uint32>(TypeCharacteristics<int32>::MaxValue())){
			finalIndex  = TypeCharacteristics<int32>::MaxValue()-1;
		}

		/** check if we need to allocate more memory */
		if (finalIndex >= maxIndex){

			/** can we change memory allocation */
			if (dataPointerAddress != NULL){
				/** add some margin to reduce reallocation frequency*/
				maxIndex = static_cast<int32>(finalIndex + DZTA_APPEND_GRANULARITY);
//printf("A{%p %p %i %i %i}\n",dataPointer,dataPointerAddress,index,maxIndex,ret.format_as_integer); TODO

				/** malloc or realloc ??*/
				if (dataPointer != NULL_PTR(uint8 *)){
					HeapManager::AllocationPointer p(dataPointer);
					ret = p.Realloc((maxIndex + 1)*elementSize);
					*dataPointerAddress = p;
					dataPointer = p;
				} else {
					HeapManager::AllocationPointer p;
					ret = p.Malloc((maxIndex + 1)*elementSize,HeapManager::stringsHeapId);
					*dataPointerAddress = p;
					dataPointer = p;
				}

				if (!ret){
					maxIndex = 0;
				}
			} else {
				ret.outOfRange = true;
			}
		}

		if (ret){
			uint8 *start = dataPointer + index * elementSize;
			memcpy(start,data,dataSize * elementSize);

			Terminate(static_cast<uint32>(finalIndex));
			index = static_cast<uint32>(finalIndex);
		}
	}
}

void ZeroTerminatedArrayTool::Remove(uint32 elements){

    if (ret){
	    ret.invalidOperation = (elements > index);
	}

    if (ret){
    	uint32 start = elements * elementSize;
    	uint32 i2 = 0;
    	uint32 last = index * elementSize;
    	for (uint32 i = start; i <= last;i++,i2++){
    		dataPointer[i2] = dataPointer[i];
    	}
    	index = index - elements;

    	if (dataPointerAddress != NULL){
    		if ((static_cast<uint32>(maxIndex) > (index + DZTA_APPEND_GRANULARITY)) && (maxIndex > 0)){
            	maxIndex = static_cast<int32>(index);
            	HeapManager::AllocationPointer p(dataPointer);
            	ret = p.Realloc((maxIndex + 1) * elementSize);
            	*dataPointerAddress = p;
            	dataPointer = p;
            	if (!ret){
                	maxIndex = 0;
            	}
        	}
    	}
    }
}


} //MARTe
