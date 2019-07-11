/**
 * @file VariableCloner.h
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

#ifndef VARIABLECLONER_H_
#define VARIABLECLONER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "VariableDescriptorLib.h"
//#include "MemoryPageFile.h"
/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

	class MemoryPageFile;

namespace VariableDescriptorLib{

/**
 * Internal class to support Clone function
 */
class DLL_API VariableCloner{
public:
	/**
	 * @brief constructor: only initialises handler
	 */
	VariableCloner(Variable &handlerIn,MemoryPageFile &pageFileIn);

	/**
	 * Recursive function
	 * @param level is the current level of variable level indirection
	 * @param inputPointer is the pointer to the data to be copied
	 * @param startOfOutput
	 * @param addressOfOutput address of the block created in the current level
	 * @param sizeOfOutput  --> number of elements
	 */
	ErrorManagement::ErrorType DoCreateR(
				uint32 							level,
				const uint8* 					inputPointer,
				uint8 *&						addressOfOutput,
				DimensionSize 					numberOfElementsD
		);

	/**
	 * @brief Note that one has to create the modifier list from the Handler as some of the dimension might have been removed.
	 */
	void GetOutputModifiers(DynamicCString &dc) const ;

public:
	/**
	 *
	 */
	Variable &              		handler;

	/**
	 * used to store data
	 */
	MemoryPageFile &				pageFile;

	/**
	 * Data is finally encoded as zero term strings
	 */
	bool							isString;

	void operator=(VariableCloner &data){}
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} //VariableDescriptorLib
} // MARTe

#endif /* SVARIABLECLONER_H_ */
