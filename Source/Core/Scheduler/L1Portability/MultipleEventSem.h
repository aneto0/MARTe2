/**
 * @file MultipleEventSem.h
 * @brief Header file for class AnyType
 * @date 21 Aug 2019
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

#ifndef MULTIPLEEVENTSEM_H_
#define MULTIPLEEVENTSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,MultipleEventSemData.h)
#include "EventSource.h"
#include "MilliSeconds.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * @brief this object allows waiting on a list of EventSources.
 *
 * @details It is implemented with WaitForMultipleObjectEx or poll()
 * It support sockets, event sems, mutex sems, console input...
 *
 * */
class MultipleEventSem{
public:
	/**
	 *
	 */
	MultipleEventSem();

	/**
	 *
	 */
	~MultipleEventSem();

    /**
     * @brief Waits for an event, limited by the timeout time.
     * while the list is waited upon, the APIS of this objects are disabled --> return ErrorAccessDenied
     * @return on success the index of the event in the internal list is encoded in the ErrorrType. use GetNonErrorCode to retrieve it
     * Note that the call is not multi-thread safe. Use a mutex to allow multiple threads to add events
     */
    ErrorManagement::ErrorType Wait(const MilliSeconds &timeout);

    /**
     * @briefs adds the passed event to the list of events to wait for.
     * @return on success the index of the event in the internal list is encoded in the ErrorrType. use GetNonErrorCode to retrieve it
     * Note that the call is not multi-thread safe. Use a mutex to allow multiple threads to add events
     * Note the EventSource information is copied in an internal structure and any resource associated with the EventSource is deallocated at object destruction.
     */
    ErrorManagement::ErrorType AddEvent(EventSource event);

    /**
     * @briefs clears all history of events not yet reported. (effective only under linux)
     */
    ErrorManagement::ErrorType Reset();

private:

    /**
     *
     */
    MultipleEventSemData data;

    /**
     * cannot be used
     */
    void operator=(const MultipleEventSem &data){}

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



} // MARTe

#endif /* SOURCE_CORE_SCHEDULER_L1PORTABILITY_MULTIPLEEVENTSEM_H_ */
