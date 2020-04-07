/**
 * @file EventInterface.h
 * @brief Header file for class AnyType
 * @date 3 Jan 2020
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

#ifndef EVENTINTERFACE_H_
#define EVENTINTERFACE_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSource.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * Interface for classes that allow synchronisation on a MultipleEventSem
 */
class EventInterface {

public:

	/**
	 * The event that one would like to capture
	 */
	class Event{
	public:
		/**
		 * Initialise with the event code
		 */
		inline Event(uint64 code=0){
			this->code = code;
		}
		/**
		 * Copy constructor
		 */
		inline Event(const Event &ev){
			this->code = ev.code;
		}
		/**
		 * To allow comparison
		 */
		inline bool operator==(const Event &ev){
			return (code == ev.code);
		}
		/**
		 * To allow mask handling
		 */
		inline bool In(const Event &evMask){
			return ((evMask.code & code) != 0);
		}


	private:
		uint64 code;
	};

	/**
	 * No specific event
	 */
	static const Event noEvent;

	/**
	 *
	 */
	EventInterface();

	/**
	 *
	 */
	virtual ~EventInterface();

	/**
	 *
	 */
	virtual EventSource GetEventSource(EventInterface::Event eventMask= noEvent)const =0 ;

private:
	/**
	 * Disallow copy
	 */
	void operator=(EventInterface & ev);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_EVENTINTERFACE_H_ */
