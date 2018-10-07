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
struct DLL_API Dimension{
	/**
	 * @brief A for arrays F/f for pointers to arrays, ZSDzsd for pointers to ZTA, MVmv for matrix & vectors /0 final
	 */
	char8  type;

	/**
	 * @brief the type that ends this layer. A layer is made of a sequence of As AAP  AAZ etc...
	 */
	char8  endType;

	/**
	 * @brief number of elements in this dimension
	 * NonDet for pointers, Vectors, Arrays,ZTAs etc..
	 */
	DimensionSize numberOfElements;

	/**
	 * @brief product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 * exclude the actual multiplier of the Vector/Matrix/ZTA
	 */
	DimensionSize elementSize;

	/**
	 * @brief constructor
	 */
	inline Dimension(char8 typeIn,const DimensionSize &numberOfElementsIn);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

Dimension::Dimension(char8 typeIn,const DimensionSize &numberOfElementsIn){
	type = typeIn;
	endType = typeIn;
	numberOfElements = numberOfElementsIn;
	elementSize = 0;
}


} //VariableDescriptorLib
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_VARIABLEDESCRIPTORLIB_VD_DIMENSION_H_ */
