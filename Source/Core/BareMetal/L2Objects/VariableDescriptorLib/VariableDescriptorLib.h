/**
 * @file VariableDescriptorLib.h
 * @brief Header file for class AnyType
 * @date 3 Oct 2018
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

#ifndef VARIABLEDESCRIPTORHELPER_H_
#define VARIABLEDESCRIPTORHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//#include "VariableDescriptor.h"
#include "VD_Dimension.h"
#include "VD_Variable.h"
//#include "TypeConversionOperatorI.h"
//class Marte::TypeConversionOperatorI;

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class TypeConversionOperatorI;

namespace VariableDescriptorLib{

/**
 * @brief parses a modifierString extracting the top modifier and associated number,
 * modifierString is modified to point to the next modifier
 * at the end of the string it returns 'O' to indicate the termination
 */
DLL_API void GetLayerInfo(CCString &modifierString,char8 &modifier,uint32 &size );

/**
 *  Converts a type designator character to size.
 *  if c is 0 then the size of the type described by tdIn is returned
 */
DLL_API uint32 Type2Size(char8 c,const TypeDescriptor &tdIn);

/**
 * returns the TypeDescriptor of a given type
 * returns tidIn if c == 'O'
 */
DLL_API TypeDescriptor Type2TypeDescriptor(char8 c, const TypeDescriptor &tdIn);

/**
 * @brief takes a modifierString and calculates the size of the top layer.
 * a layer terminates with a pointer or a pointer like structure (Vector,Array,ZTA)
 */
DLL_API DimensionSize LayerSize(CCString modifierString,TypeDescriptor td);

/**
 * check pointer and redirects
 * if allowNULL is true allows redirecting to a NULL pointer
 */
DLL_API ErrorManagement::ErrorType RedirectP(const uint8* &ptr,bool allowNULL= false);


#if 0
/**
 * Compares the two variable descriptors first and second to see whether they have compatible dimensions
 * 1) The number of dimensions must be the same
 * 2) each dimension must have equal size
 * 3) or one of them is indeterminate
 * 4) and no overflow
 */
DLL_API ErrorManagement::ErrorType HasSameDimensionsAs(const Variable &first,const Variable &second);
#endif
/**
 *
 */
DLL_API ErrorManagement::ErrorType CopyToRecursive(
		const Dimension 				*sourceDimensions,
		const uint8* 					sourcePtr,
		const Dimension 				*destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI 	&	op,
		bool                            isCopy
		);

#if 0
/**
 * copy something to a non const Vector<BasicType>
 * Vector will be resized to match
 */
DLL_API ErrorManagement::ErrorType VectorCopy(
		uint32 							level,
		VariableDescriptorLib::Variable &sourceDimensions,
		const uint8* 					sourcePtr,
		VariableDescriptorLib::Variable &destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		);

/**
 * copy something to a non const Matrix<BasicType>
 * Vector will be resized to match
 */
DLL_API ErrorManagement::ErrorType MatrixCopy(
		uint32 							level,
		VariableDescriptorLib::Variable &sourceDimensions,
		const uint8* 					sourcePtr,
		VariableDescriptorLib::Variable &destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		);
#endif
/**
 * Operates recursively
 */
DLL_API ErrorManagement::ErrorType GetSizeRecursive(
		const Dimension 	            *dimension,
		const uint8* 					pointer,
		uint64 							&dataSize,
		uint64 							&auxSize
		);

/**
 *
 */
DLL_API ErrorManagement::ErrorType ToString(
		CCString 						modifiers,
		const TypeDescriptor &			typeDescriptor,
		DynamicCString &				string,
		CCString 						modifierString,
		bool 							start,
		int8 &							priority);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} //VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORHELPER_PRIVATE_H_ */
