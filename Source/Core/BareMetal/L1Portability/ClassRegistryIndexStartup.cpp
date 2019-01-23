/**
 * @file ClassRegistryIndexLoader.cpp
 * @brief Header file for class AnyType
 * @date 21 Dec 2018
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



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryIndex.h"
#include "StartupManager.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(ClassRegistryIndex,("CRIDataBaseCore",emptyString),("HeapManager",emptyString))


/**
	Singleton based startup and memory allocation
	Managed shutdown
*/
ErrorManagement::ErrorType ClassRegistryIndexStartup::Init(){
	ErrorManagement::ErrorType ret;
	ClassRegistryIndex *pcri = ClassRegistryIndex::Instance();

	ret.fatalError = (pcri == NULL_PTR(ClassRegistryIndex *));
	return ret;
}

ErrorManagement::ErrorType ClassRegistryIndexStartup::Finish(){
	ErrorManagement::ErrorType ret;
	ClassRegistryIndex *pcri = ClassRegistryIndex::Instance();

	// cannot be used anymore after this!!!
	delete pcri;

	return ret;
}

} //MARTe
