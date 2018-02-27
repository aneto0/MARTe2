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
#include "CompositeErrorManagement.h"
#include "StaticList.h"
#include "VariableDescriptor.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * Special size code to indicate that the size is variable
 */
const uint32 variableSize32			= 0xFFFFFFFF;

/**
 * Special size code to indicate that the size is not defined
 */
const uint32 indeterminateSize32	= 0xFFFFFFFE;

/**
 * Special size code to indicate that the size is variable
 */
const uint32 tooLarge32				= 0xFFFFFFF0;


class  DimensionSize{
public:
	inline DimensionSize(uint32 x=0,bool force=false){
		if ((!force )&&(x >= tooLarge32)){
			x = tooLarge32;
		}
		value = x;
	}

	inline DimensionSize operator= (const DimensionSize &x){
		value = x.value;
		return *this;
	}

	inline bool operator== (const DimensionSize &x){
		return (value == x.value);
	}

	inline bool operator!= (const DimensionSize &x){
		return (value != x.value);
	}

	inline DimensionSize operator*(const DimensionSize &m) const{
		DimensionSize d;
		if (( m.value == tooLarge32 ) || ( value == tooLarge32 )){
			d.value = tooLarge32;
		} else
		if (( m.value == indeterminateSize32 ) || ( value == indeterminateSize32 )){
			d.value = indeterminateSize32;
		} else
		if (( m.value == variableSize32 ) || ( value == variableSize32 )){
			d.value = variableSize32;
		} else {
			uint64 o = m.value * value;
			if (o >= tooLarge32){
				d.value = tooLarge32;
			} else {
				d.value = o;
			}
		}
		return d;
	}

	inline DimensionSize operator+(const DimensionSize &m) const{
		DimensionSize d;
		if (( m.value == tooLarge32 ) || ( value == tooLarge32 )){
			d.value = tooLarge32;
		} else
		if (( m.value == indeterminateSize32 ) || ( value == indeterminateSize32 )){
			d.value = indeterminateSize32;
		} else
		if (( m.value == variableSize32 ) || ( value == variableSize32 )){
			d.value = variableSize32;
		} else {
			uint64 o = m.value + value;
			if (o >= tooLarge32){
				d.value = tooLarge32;
			} else {
				d.value = o;
			}
		}
		return d;
	}

	inline bool isTooLarge() const{
		return (value == tooLarge32);
	}

	inline bool isVariableSize() const{
		return (value == variableSize32);
	}

	inline bool isIndeterminateSize() const{
		return (value == indeterminateSize32);
	}

	inline bool isNotANumber() const{
		return (value >= tooLarge32);
	}

	ErrorManagement::ErrorType toUint32(uint32 &x) const{
		ErrorManagement::ErrorType ret;
		if (isNotANumber()){
			ret.outOfRange = true;
		} else {
			x = value;
		}
		return ret;
	}

	uint32 toUint32Unchecked() const{
		return value;
	}
private:
	uint32 value;

	operator uint32(){ return value;}
};

/**
 * Special size code to indicate that the size is variable
 */
const DimensionSize variableSize(variableSize32,true);

/**
 * Special size code to indicate that the size is not defined
 */
const DimensionSize indeterminateSize(indeterminateSize32,true);

/**
 * Special size code to indicate that the size is variable
 */
const DimensionSize tooLarge(tooLarge32,true);

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
	 * product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 */
	DimensionSize elementSize;

	DimensionInfoElement(char8 typeIn,DimensionSize numberOfElementsIn){
		type = typeIn;
		numberOfElements = numberOfElementsIn;
		layerEnd = '\0';
		elementSize = 0;
		outputType = type;
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
	DimensionSize Type2Size(char8 c) const;

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
inline  DimensionInfoElement &DimensionHandler::operator[](uint32 index) {
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
