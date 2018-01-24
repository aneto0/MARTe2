/**
 * @file ProgressiveCStringArrayCreator.h
 * @brief Header file for class AnyType
 * @date 11 Jan 2018
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

#ifndef PROGRESSIVECSTRINGARRAYCREATOR_H_
#define PROGRESSIVECSTRINGARRAYCREATOR_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ProgressiveFixedSizeTypeCreator.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


class DLL_API ProgressiveCStringArrayCreator: public ProgressiveFixedSizeTypeCreator{

public:

	/**
	 * @brief Creates the object selecting the type to convert to and the default PageSize
	 */
	inline ProgressiveCStringArrayCreator(uint32 pageSizeIn );

	/**
	 * @adds an element. It will convert the string to the specified type
	 */
	ErrorManagement::ErrorType AddElement(CCString typeStringRepresentation);

	ErrorManagement::ErrorType GetReference(ReferenceT<AnyObjectI> &x);

	/**
	 * @brief first call - sets the output type
	 */
	inline ErrorManagement::ErrorType Start();

private:

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ProgressiveCStringArrayCreator::ProgressiveCStringArrayCreator(uint32 pageSizeIn ):ProgressiveFixedSizeTypeCreator(pageSizeIn){
}

ErrorManagement::ErrorType ProgressiveCStringArrayCreator::Start(){
	return ProgressiveFixedSizeTypeCreator::Start(ConstCharString(sizeof(CCString)));

}


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PROGRESSIVECSTRINGARRAYCREATOR_H_ */
