/**
 * @file VariableDescriptorDimensionsInfo.h
 * @brief Header file for class AnyType
 * @date 7 Feb 2018
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

#ifndef VDPRIVATE_H_
#define VDPRIVATE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"
#include "StaticList.h"
#include "ErrorManagement.h"
#include "TypeDescriptor.h"
#include "ErrorManagement.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{



/**
 * @brief description of a dimension.
 */
struct DimensionInfoElement{
	/**
	 * @brief A for arrays F/f for pointers to arrays, ZSDzsd for pointers to ZTA, MVmv for matrix & vectors /0 final
	 */
	char8  type;
	/**
	 * @brief P/p for pointer F/f for pointers to arrays, ZSDzsd for pointers to ZTA, MVmv for matrix & vectors /0 final
	 */
	char8  layerEnd;
	/**
	 * @brief number of elements in this dimension
	 * 0 for variable size (ZSDMV)
	 */
	uint32 numberOfElements;

	/**
	 * product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 */
	uint64 elementSize;

	/**
	 * @brief Size of each element. Product of size of layers below up to a redirection
	 * 0 for variable size
	 */
//	uint64 dimensionsProduct;


	DimensionInfoElement(char8 typeIn,uint32 numberOfElementsIn){
		type = typeIn;
		numberOfElements = numberOfElementsIn;
		layerEnd = '\0';
//		dimensionsProduct = numberOfElements;
		elementSize = 0;
	}

};


class DimensionHandler{
public:
	/**
	 * prepare a table of DimensionInfoElement s
	 */
	DimensionHandler(CCString modifiers,TypeDescriptor tdIn);


	/**
	 * Allow access to all DimensionInfoElement
	 */
	inline const DimensionInfoElement &operator[](uint32 index) const;

	/**
	 * How many DimensionInfoElement
	 */
	inline uint32 NDimensions() const ;

	/**
	 * Compares two DimensionHandlers
	 */
	ErrorManagement::ErrorType HasSameDimensionsAs(const DimensionHandler &other) const ;

	/**
	 * access type
	 * This is either the original tdIn or a synthetic pointer type
	 */
	inline TypeDescriptor GetTypeDescriptor() const ;

	/**
	 * allows redirecting to next layer and at the same time obtain updated number of elements
	 */
	ErrorManagement::ErrorType UpdatePointerAndSize(uint32 layerIndex, const uint8 *&pointer,uint32 &numberOfElements,uint64 &elementSize,uint32 &overHead) const ;

private:

	/**
	 *
	 */
	DimensionHandler( const DimensionHandler &){}

	/**
	 *
	 */
	void operator=(const DimensionHandler &){}

	/**
	 * size of a dimension code
	 */
	uint32 DimensionSize(char8 c) const;

	/**
	 *
	 */
	StaticList<DimensionInfoElement> dimensions;

	TypeDescriptor td;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

uint32 DimensionHandler::NDimensions() const {
	return dimensions.GetSize();
}

TypeDescriptor DimensionHandler::GetTypeDescriptor() const{
	return td;
}
//#include <stdio.h>
inline const DimensionInfoElement &DimensionHandler::operator[](uint32 index) const{
	static DimensionInfoElement dummy('\0',0);
	DimensionInfoElement &ret = dummy;
	if (index < dimensions.GetSize()){
//printf("[%i %c %c %i]",index,dimensions[0].type,dimensions[1].type,dimensions[index].type);

		ret = dimensions[index];
	}
	return ret;
}



} // MARTe


#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORDIMENSIONSINFO_H_ */
