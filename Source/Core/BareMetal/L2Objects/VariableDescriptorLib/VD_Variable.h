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
class  DLL_API Variable: protected StaticList<Dimension>{
public:
	/**
	 * @brief prepare a table of DimensionInfoElement
	 * Stops modifier parsing when a pure pointer is encountered: P\0 or PP or PAAA\0 or PV or PM
	 *
	 * stopAtFirstTerminal stops parsing at the first non A modifier
	 */
	Variable(CCString modifiers,TypeDescriptor tdIn,bool stopAtFirstTerminal=false);

	/**
	 * Allow read only access to all DimensionInfoElement
	 */
	inline const Dimension 		&operator[](uint32 index) const ;

	/**
	 * Allow access to all DimensionInfoElement
	 */
	inline Dimension 			&Access(uint32 index) ;

	/**
	 * How many DimensionInfoElement
	 */
	inline uint32 				NDimensions() const ;

	/**
	 * access type
	 * This is either the original tdIn or a synthetic pointer type
	 */
	inline TypeDescriptor 		GetTypeDescriptor() const ;

private:

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

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TypeDescriptor Variable::GetTypeDescriptor() const{
	return td;
}

uint32 Variable::NDimensions() const {
	return GetSize();
}

const Dimension &Variable::operator[](uint32 index) const{
	return StaticList<Dimension>::operator[](index);
}

Dimension &Variable::Access(uint32 index) {
	return StaticList<Dimension>::Access(index);
}

Variable::Variable(const Variable &){

}

void Variable::operator=(const Variable &){

}


} // VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORLIB_VD_VARIABLE_H_ */
