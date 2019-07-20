/**
 * @file HeapStartup.cpp
 * @brief Header file for class AnyType
 * @date 3 Jan 2019
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

 * @details This header file contains the declaration of the class HeapStartup
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StartupManager.h"
#include "HeapManager.h"
#include "StandardHeap.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(HeapManager,("HeapManager",emptyString),(emptyString))

StandardHeap heaps[3];

ErrorManagement::ErrorType HeapManagerStartup::Init(){
	ErrorManagement::ErrorType ret;

	//TODO combine return codes
	ret = HeapManager::InstallAllocator(&heaps[0],HeapManager::standardHeapId);
	ret = HeapManager::InstallAllocator(&heaps[1],HeapManager::internalsHeapId);
	ret = HeapManager::InstallAllocator(&heaps[2],HeapManager::stringsHeapId);

	return ret;
}

ErrorManagement::ErrorType HeapManagerStartup::Finish(){
	ErrorManagement::ErrorType ret;

	//TODO combine return codes
	ret = HeapManager::RemoveAllocator(HeapManager::standardHeapId);
	ret = HeapManager::RemoveAllocator(HeapManager::internalsHeapId);
	ret = HeapManager::RemoveAllocator(HeapManager::stringsHeapId);

	return ret;
}


} //MARTe

