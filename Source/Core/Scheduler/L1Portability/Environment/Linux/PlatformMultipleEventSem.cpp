/**
 * @file PlaformMultipleEventSem.cpp
 * @brief
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

#include "PlatformMultipleEventSem.h"
#include "CompositeErrorManagement.h"

namespace MARTe{

PlaformMultipleEventSem::PlaformMultipleEventSem(){
	pending = 0;
}


ErrorManagement::ErrorType PlaformMultipleEventSem::AddEvent(const EventSource &event){
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = (handles.GetSize() >= RLIMIT_NOFILE );
	COMPOSITE_REPORT_ERROR(ret,"AddEvent supports up to ",RLIMIT_NOFILE," event sources");

	if (ret){
		handles.Add(event.pfd);
	}

	return ret;
}

ErrorManagement::ErrorType PlaformMultipleEventSem::Wait(const MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;

	if (pending <= 0){
		uint32 index = 0;
		uint32 temp = timeout.GetTimeRaw();
		int waitTime = 0;

		if (temp <  TypeCharacteristics<int>::MaxValue()){
			waitTime = temp;
		} else {
			ret.parametersError = !timeout.IsValid();
			REPORT_ERROR(ret,"timeout is neither positive infinite nor a finite number ");

			if (ret){
				waitTime = -1;
			}
		}

		if (ret){
			int nEvents = poll(handles.GetAllocatedMemoryConst(),handles.GetSize(),timeout);

			ret.OSError = (nEvents < 0);
			COMPOSITE_REPORT_ERROR(ret,"poll() returned error ",errno);

			if (ret){
				ret.timeout = (nEvents == 0);
			}

			if (ret){
				pending = nEvents;
			}
		}
	}

	// assuming there are pending events and no error
	if (ret && (pending > 0)){
		uint32 max = handles.GetSize();
		bool found = false;
		uint32 index = 0;
		for (; (index < max) && !found; index++){
			found = (handles[index]->revents != 0);
		}

		if (found){
			ret.SetNonErrorCode(index);
		}


	}

	return ret;
}


} //MARTe
