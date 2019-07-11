/**
 * @file VD_Dimension.h
 * @brief Header file for class AnyType
 * @date 7 Oct 2018
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

#ifndef VD_DIMENSION_H_
#define VD_DIMENSION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "SaturatedInteger.h"
#include "ErrorManagement.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace VariableDescriptorLib{

/**
 * use a SaturatedInteger<uint32> to describes number of elements in the structures
 */
typedef  SaturatedInteger<uint32> DimensionSize;

/**
 * @brief description of a dimension.
 */
class DLL_API Dimension{

public:
	/**
	 * constructor
	 */
	Dimension();

	/**
	 * virtual destructor
	 */
	virtual ~Dimension();

	/**
	 *
	 */
	static const uint8 isConstant = 0x1;

	/**
	 * memory can be reallocated
	 */
	static const uint8 isDynamic = 0x2;

	/**
	 *  this is a layer that ends all the stacks
	 */
	static const uint8 is2D = 0x4;

	/**
	 *  this is a layer that ends the current stack
	 *  a stack is made of layers that can occupy contiguous memory
	 */
	static const uint8 isBreak = 0x40;

	/**
	 *  this is a layer that ends all the stacks
	 *  isFinal layers are also iSBreak
	 */
	static const uint8 isFinal = 0x80;

	/**
	 * @brief returns the size of the typeChar.
	 * Works also for 'O' final type.
	 */
	virtual uint32 TypeSize() const =0;

	/**
	 * converts to a TypeDescriptor
	 */
	virtual TypeDescriptor GetTypeDescriptor() const ;

	/**
	 * @brief gets the char corresponding to the type
	 */
	virtual char8 TypeChar() const = 0;

	/**
	 * @brief 1 for all but 2 for Matrix
	 */
	inline uint8 NOfDimensions() const;

	/**
	 * @brief is it a layer with redirection or a straight dimension?
	 */
	inline bool IsBreak() const;

	/**
	 * @brief has the next layer a modifiable size?
	 */
	inline bool IsDynamic() const;
	/**
	 * @brief indicates that this is either the last layer (the type) or one above.
	 * So equivalent to a vector<T> or matrix<T> or T  or T[]
	 */
	bool IsFinalLayer() const;

	/**
	 * @browses the layers looking for next break;
	 * if this is a break returns this
	 */
	inline const Dimension* GetNextBreak() const;

	/**
	 * @brief indicates that this is the last stack end.
	 * So equivalent to T
	 */
	inline bool IsFinal() const;

	/**
	 * @brief access the next dimension
	 */
	inline Dimension *Next() const;

	/**
	 * @brief allows adding an element to the end
	 */
	static void AddToEnd(Dimension *&rootPtr,Dimension *newDimension);

	/**
	 * @brief process a dimension
	 * dereference pointer if necessary
	 * calculates dimensions
	 */
	virtual ErrorManagement::ErrorType UpdatePointerAndSizeEx(
				const uint8 *&	ptr,
				uint32 &		numberOfColumns,
				uint32 &		numberOfRows) const = 0;

	/**
	 * @brief returns the elements in a dimension
	 * only valid for F and A dimensions
	 */
	virtual ErrorManagement::ErrorType GetNumberOfElements(uint32 &numberOfElements) const;

	/**
	 * @brief process a dimension
	 * dereference pointer if necessary
	 * try to set the dimensions.
	 * On success returns the pointer to the actual data as in UpdatePointerAndSizeEx
	 */
	virtual ErrorManagement::ErrorType ReSize(
				uint8 *&		ptr,
				uint32 &		numberOfColumns,
				uint32 &		numberOfRows) const;

	/**
	 * @brief initialises the current layer stack
	 * it will recurse through all the 'A' levels and compute the total n of elements
	 * it will stop at the first stack break which will initialise the whole memory
	 * if the stack break is 'P','F' the memory will be all NULL. V and M will be handled similarly
	 * for the final level 'O' it will depend on the actual type. Basic Types and structures are set to 0
	 *
	 */
	virtual ErrorManagement::ErrorType InitStack(
				uint8 * 		ptr,
				DimensionSize	numberOfElements) const = 0;

	/**
	 * loops through layers stopping at a stack break.
	 * calculates combined size of all elements on this stack
	 * size of stack end * nOfEl each layer
	 */
	virtual DimensionSize GetLayerCombinedElementSize() const =0;

	/**
	 * takes the size of the element multiplied by this dimension
	 * multiplies the dimension sizes of all dimensions from the next
	 * until a break is encountered. the size of the dimension break element is last multiplied
	 */
	inline ErrorManagement::ErrorType GetNextLayerElementSize(uint32 &destElementSize) const ;
	inline ErrorManagement::ErrorType GetNextLayerElementSize(DimensionSize &destElementSize) const;

	/*
	 * how much is wasted in pointers and or terminators.
	 * 0 for arrays
	 * sizeof(Vector)
	 * for ZTA is the pointer and the data terminator
	 */
	virtual uint32 GetOverHead() const ;

protected:
	/*
	 *
	 */
	uint8 flags;

	/**
	 * points to next dimension in the variable
	 */
	Dimension *next;

	/**
	 * points to next dimension in the variable
	 */
	Dimension *endStack;

	/**
	 * recursively called
	 * add to end
	 * fixes endStack
	 */
	void Add(Dimension *newDimension);

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


Dimension *Dimension::Next() const{
	return next;
}

uint8 Dimension::NOfDimensions() const{
	uint8 ret = 1;
	if (flags & is2D){
		ret = 2;
	}
	return ret;
}

bool Dimension::IsBreak() const{
	return ((flags & isBreak)!=0);
}

bool Dimension::IsDynamic() const{
	return (((flags & isDynamic)!=0) && ((flags & isConstant)==0));
}

bool Dimension::IsFinal() const{
	return ((flags & isFinal) != 0);
}


ErrorManagement::ErrorType Dimension::GetNextLayerElementSize(DimensionSize &destElementSize) const {
	ErrorManagement::ErrorType ret;

	if ((flags & isFinal)==0){
		ret.internalSetupError = (next == NULL_PTR(Dimension *));
		REPORT_ERROR(ret,"next is NULL ");

		if (ret){
			destElementSize = next->GetLayerCombinedElementSize();
		}
	} else {
		destElementSize = DimensionSize(this->TypeSize());
	}
	return ret;
}

ErrorManagement::ErrorType Dimension::GetNextLayerElementSize(uint32 &destElementSize) const{
	ErrorManagement::ErrorType ret;
	DimensionSize di;
	ret = GetNextLayerElementSize(di);

	if (ret){
		ret = di.ToNumber(destElementSize);
		REPORT_ERROR(ret,"overflow in dimensions");
	}
	return ret;
}

const Dimension* Dimension::GetNextBreak() const{
	const Dimension* d = this;
	while ((!d->IsBreak()) && (d != NULL_PTR(Dimension *))){
		d = d->Next();
	}
	return d;
}


} //VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORLIB_VD_DIMENSION_H_ */
