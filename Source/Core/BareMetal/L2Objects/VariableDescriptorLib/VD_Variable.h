/**
 * @file VD_Variable.h
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

#ifndef VD_VARIABLE_H_
#define VD_VARIABLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "VD_Dimension.h"
#include "StaticList.h"
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
 *
 * A more expanded and useful version of VariableDescriptor
 */
class  DLL_API Variable/*: protected StaticList<Dimension>*/{
public:
	/**
	 * @brief prepare a table of DimensionInfoElement
	 * Stops modifier parsing when a pure pointer is encountered: P\0 or PP or PAAA\0 or PV or PM
	 *
	 */
	Variable(CCString modifiers,TypeDescriptor tdIn);

	/**
	 *  @brief destructor
	 */
	inline virtual ~Variable();

	/**
	 * How many DimensionInfoElement
	 */
	inline uint32 				NDimensions() const ;

	/**
	 * Allow read only access to all DimensionInfoElement
	 */
	const Dimension 			&operator[](uint32 index) const ;

	/**
	 * access type
	 * This is either the original tdIn or a synthetic pointer type
	 */
	inline TypeDescriptor 		GetTypeDescriptor() const ;
#if 0
	/**
	 *
	 */
	inline ErrorManagement::ErrorType UpdatePointerAndSize(
				uint32 			layerIndex,
				const uint8 *&	ptr,
				uint32 &		numberOfElementsIO,
				uint32 &		nextElementSize,
				uint32 &		overHead);
#endif
private:

	/**
	 * Allow access to all DimensionInfoElement
	 */
	Dimension 					&Access(uint32 index) ;


	/**
	 * adds one dimension at the end
	 */
	void Add(Dimension *dimension);

	/**
	 *	@brief disable copy constructor
	 */
	inline Variable(const Variable &);

	/**
	 * @brief disable copy operator
	 */
	inline void operator=(const Variable &);

	/**
	 * @Brief Normally a copy of the TdIn used in initialisation. It can become a Pointer in case a pointer modifier is encountered
	 *
	 */
	TypeDescriptor td;

	/**
	 *
	 */
	Dimension *firstDimension;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

Variable::~Variable() {
	if (firstDimension != NULL){
		delete firstDimension;
	}
}

TypeDescriptor Variable::GetTypeDescriptor() const{
	return td;
}

uint32 Variable::NDimensions() const {
	const Dimension *p = firstDimension;
	uint32 count = 0;
	while (p != NULL){
		count++;
		p = p->Next();
	}
	return count;
}

Variable::Variable(const Variable &){
	firstDimension = NULL;
}

void Variable::operator=(const Variable &){
	firstDimension = NULL;
}

/*
ErrorManagement::ErrorType Variable::UpdatePointerAndSize(
		uint32 			layerIndex,
		const uint8 *&	ptr,
		uint32 &		numberOfElementsIO,
		uint32 &		nextElementSize,
		uint32 &		overHead){
	return (*this)[layerIndex].UpdatePointerAndSize(ptr,numberOfElementsIO,nextElementSize,overHead);
}
*/


} // VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORLIB_VD_VARIABLE_H_ */
