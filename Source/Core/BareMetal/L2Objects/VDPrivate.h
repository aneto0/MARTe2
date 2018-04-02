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
#include "CompositeErrorManagement.h"
#include "StaticList.h"
#include "VariableDescriptor.h"
#include "SaturatedInteger.h"
#include "MemoryCheck.h"



/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * use a SaturatedInteger<uint32> to describes number of elements in the structures
 */
typedef  SaturatedInteger<uint32> DimensionSize;
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
	 * @brief used to mark the type used for the output
	 */
	char8 outputType;

	/**
	 * @brief number of elements in this dimension
	 * DynamicSize for variable size (ZSDMV)
	 */
	DimensionSize numberOfElements;

	/**
	 * @brief product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 */
	DimensionSize elementSize;

	/**
	 * @brief constructor
	 */
	DimensionInfoElement(char8 typeIn,const DimensionSize &numberOfElementsIn){
		type = typeIn;
		numberOfElements = numberOfElementsIn;
		layerEnd = '\0';
		elementSize = 0;
		outputType = type;
	}

};

static inline ErrorManagement::ErrorType RedirectP(const uint8* &ptr,bool allowNULL){
	ErrorManagement::ErrorType ret;
	const uint8 **pp = (const uint8 **)(ptr);
	const uint8 *p = *pp;
	if ((p == NULL) && (allowNULL)){
		ptr = p;
	} else
	if ((p == NULL) || (!MemoryCheck::Check(p))){
		ret.exception = true;
		DynamicCString errM;
		errM.Append("bad pointer (");
		errM.AppendHex(reinterpret_cast<uint64>(p));
		errM.Append(") at (");
		errM.AppendHex(reinterpret_cast<uint64>(pp));
		errM.Append(')');
        REPORT_ERROR(ret, errM.GetList());
	} else {
		ptr = p;
	}
	return ret;
}


/**
 * A more expanded and useful version of VariableDescriptor
 */
class DimensionHandler{
public:
	/**
	 * @brief prepare a table of DimensionInfoElement s
	 */
	DimensionHandler(CCString modifiers,TypeDescriptor tdIn);

	/**
	 * Allow access to all DimensionInfoElement
	 */
	inline DimensionInfoElement &operator[](uint32 index);

	/**
	 * How many DimensionInfoElement
	 */
	inline uint32 				NDimensions() const ;

	/**
	 * Compares two DimensionHandlers
	 */
	ErrorManagement::ErrorType 	HasSameDimensionsAs(const DimensionHandler &other) const ;

	/**
	 * access type
	 * This is either the original tdIn or a synthetic pointer type
	 */
	inline TypeDescriptor 		GetTypeDescriptor() const ;

	/**
	 * Creates a VariableDescriptor using the outputType
	 */
	void 						GetOutputModifiers(DynamicCString &dc) const ;

	/**
	 * @brief allows redirecting to next layer and at the same time obtain updated number of elements
	 * Note that elementSize refers to next level.
	 * Only exception is for the last level where it is the same
	 */
	ErrorManagement::ErrorType 	UpdatePointerAndSize(
			uint32 			layerIndex,
			const uint8 *&	pointer,
			uint32 &		numberOfElements,
			uint32 &		nextElementSize,
			uint32 &		overHead)  ;

private:

	/**
	 *	@brief constructor
	 */
	DimensionHandler( const DimensionHandler &){}

	/**
	 * @brief copy operator
	 */
	void operator=(const DimensionHandler &){}

	/**
	 * @brief size of a dimension code
	 */
	uint32  Type2Size(char8 c) const;

	/**
	 * @brief the dimensions
	 */
	StaticList<DimensionInfoElement> dimensions;

	/**
	 *
	 */
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

inline  DimensionInfoElement &DimensionHandler::operator[](uint32 index) {
	DimensionInfoElement &ret = dimensions.Access(index);
	return ret;
}



} // MARTe


#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORDIMENSIONSINFO_H_ */
