/**
 * @file EventSource.h
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

#ifndef EVENTSOURCE_DATA_H_
#define EVENTSOURCE_DATA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"
#include "Atomic.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,PlatformEventSource.h)

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class  EventSourceData;

/**
 *
 */
class EventSource{
public:
	/**
	 * builds an empty EventSource
	 */
	EventSource(){
		data = new EventSourceData ;
		data->counter = 1;
	}

	/**
	 * copy constructor. Allows sharing the object
	 */
	EventSource(const EventSource & sourceIn){
		*this = sourceIn;
	}

	/**
	 * copy operator. Allows sharing the object
	 */
	EventSource operator= (const EventSource & sourceIn){
		data = sourceIn.data;
		if (data != NULL){
			Atomic::Increment(&data->counter);
		}
		return *this;
	}

	/**
	 * destroys the referenced object only if no references are left
	 */
	~EventSource(){
		if (Atomic::Decrement(&data->counter) == 0){
			delete data;
		}
	}

	/**
	 * Allows access to the important data
	 */
	EventSourceData *GetData() const{
		return data;
	}
private:
	/**
	 * The actual information to help event handling
	 */
	EventSourceData *data;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_EVENTSOURCE_H_ */
