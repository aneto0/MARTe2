/**
 * @file ZeroTerminatedArrayAppendTool.h
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

 * @details This header file contains the declaration of the class ZeroTerminatedArrayAppendTool
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ZEROTERMINATEDARRAYAPPENDTOOL_H_
#define ZEROTERMINATEDARRAYAPPENDTOOL_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ErrorType.h"
#include "ZeroTerminatedArrayStaticTools.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 *	This is a set of values to be loaded in the stack to allow efficient manipulation of strings
 *	It is all wrapped into a class that is obtained from CString, StaticCString and DynamicCString
  */
class ZeroTerminatedArrayTool: public ZeroTerminatedArrayStaticTools{
public:

	/**
	 * @brief constrct this tool taking information from a ZTA
	 */
	inline ZeroTerminatedArrayTool(uint8 **dataPointerAddressIn,uint8 *dataPointerIn,uint32 sizeOfBufferIn,uint32 elementSizeIn);

protected:

	/**
	 *	@brief put a terminator at the given position. adjusts index
	 */
	inline void Terminate(uint32 index);

	/**
	 * @brief adds dataSize elements to the end
	 * @pre data is not NULL and dataSize <= length of data
	 */
	void Append(const uint8 *data,uint32 dataSize);

	/**
	 * @brief shortens the ZTA and its allocation size
	 */
	void Truncate(uint32 newIndex);

	/**
	 * @briefs removes elements from the top
	 */
	void  Remove(uint32 elements);

	/**
	 * @brief pointer to the ZTA variable. NULL if not modifiable like in DynamicZTAs
	 */
	uint8 **dataPointerAddress;

	/**
	 * @brief copy of the ZTA pointer.
	 */
	uint8 *dataPointer;

	/**
	 * @brief size of one element (1 for char *)
	 */
	uint32 elementSize;

	/**
	 * @brief current end of string
	 */
	uint32 index;

	/**
	 * limit for index: index < maxIndex; . For a 2 bytes buffer it is 1: buffer[0] = c; (buffer[1] = \0);
	 * can be negative
	 */
	int32  maxIndex;

	/**
	 * maintain the error state across a series of operations
	 */
	ErrorManagement::ErrorType ret;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ZeroTerminatedArrayTool::ZeroTerminatedArrayTool(
		uint8 **dataPointerAddressIn,
		uint8 *dataPointerIn,
		uint32 sizeOfBufferIn,
		uint32 elementSizeIn
		): dataPointerAddress(dataPointerAddressIn),
		   dataPointer(dataPointerIn),
		   elementSize(elementSizeIn){
	if (dataPointer == NULL_PTR(uint8 *)){
		index = 0;
		maxIndex = 0;
	} else {
        maxIndex = static_cast<int32>(sizeOfBufferIn);
        maxIndex = maxIndex - 1;
		index = ZTAGetSize(dataPointer,elementSize);
	}
}

void ZeroTerminatedArrayTool::Terminate(uint32 index){
	ZTAZero(dataPointer+index*elementSize,elementSize);
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ZEROTERMINATEDARRAYAPPENDTOOL_H_ */
