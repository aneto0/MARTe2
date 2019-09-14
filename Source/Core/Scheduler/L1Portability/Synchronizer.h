/**
 * @file Synchronizer.h
 * @brief Header file for class AnyType
 * @date 23 Aug 2019
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

#ifndef SYNCHRONIZER_H_
#define SYNCHRONIZER_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "EventSource.h"
#include "ErrorType.h"
#include "ErrorManagement.h"
#include "Ticks.h"
#include "HighResolutionTimer.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,SynchronizerData.h)

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/* @details The Synchronizer offer the possibility to block any number of threads in
* a barrier. This barrier is lowered by calling the Reset method and raised
* by the Post method. Threads are blocked in the barrier by calling one of the Wait methods.
* Once the barrier is raised all the threads are allowed to concurrently proceed.
* */
class DLL_API Synchronizer: public EventSource{
public:
	/**
	 * Creates the waitable handle
	 */
	ErrorManagement::ErrorType Open();

	/**
	 * Closes the waitable handle
	 */
	ErrorManagement::ErrorType Close();

	/**
	 * Opens the lock
	 */
	ErrorManagement::ErrorType Post();

	/**
	 * Closes the lock
	 */
	ErrorManagement::ErrorType Reset();

	/**
	 * Waits for the lock to open
	 */
	inline ErrorManagement::ErrorType WaitUpdate(MilliSeconds &timeout);

	/**
	 * Waits for the lock to open
	 */
	ErrorManagement::ErrorType Wait(MilliSeconds timeout);

	/**
	 *
	 */
	Synchronizer();

	/**
	 *
	 */
	~Synchronizer();
private:
	/**
	 * platform specifc
	 */
	SynchronizerData data;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
ErrorManagement::ErrorType Synchronizer::WaitUpdate(MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;

	if (timeout.IsInfinite()){
		ret = Wait(timeout);
	} else {
		Ticks tickStart = HighResolutionTimer::GetTicks();
		ret = Wait(timeout);
		Ticks ticksEnd = HighResolutionTimer::GetTicks();
		Ticks elapsed = ticksEnd - tickStart;
		MilliSeconds elapsedMs = elapsed;
//printf("(%i)",elapsedMs.GetTimeRaw());

		if (elapsedMs < timeout){
			timeout = timeout - elapsedMs;
		} else {
			timeout = MilliSeconds(0,Units::ms);
		}
	}
	return ret;
}



} // MARTe

#endif /* SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_SYNCHRONIZER_H_ */
