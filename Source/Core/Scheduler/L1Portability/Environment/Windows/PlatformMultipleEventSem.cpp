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

ErrorManagement::ErrorType PlaformMultipleEventSem::AddEvent(const EventSource &event){
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = (handles.GetSize() >= MAXIMUM_WAIT_OBJECTS);
	COMPOSITE_REPORT_ERROR(ret,"AddEvent supports up to ",MAXIMUM_WAIT_OBJECTS, " event sources");

	if (ret){
		handles.Add(event.handle);
	}

	return ret;
}

ErrorManagement::ErrorType PlaformMultipleEventSem::Wait(const MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;
	DWORD dwMilliseconds = timeout.GetTimeRaw();
	uint32 index = 0;

	if (timeout.IsInfinite()){
		dwMilliseconds = INFINITE;
	} else {
		ret.parametersError = !timeout.IsValid();
		REPORT_ERROR(ret,"timeout is neither positive infinite nor a finite number ");
	}

	if (ret){
		DWORD reason = WaitForMultipleObjectsEx(handles.GetSize(),handles.GetAllocatedMemoryConst(),FALSE,dwMilliseconds,FALSE);

		if (reason > WAIT_OBJECT_0 ){
			index = reason - WAIT_OBJECT_0;

			ret.internalSetupError = (index >=handles.GetSize());
			COMPOSITE_REPORT_ERROR(ret,"WaitForMultipleObjectsEx returned too large index into event sources: ",index, " greater than ", handles.GetSize());

		} else {
			if (reason == WAIT_TIMEOUT){
				ret.timeout = true;
			}
			if (reason == WAIT_FAILED){
				ret.fatalError = true;
				REPORT_ERROR(ret,"WaitForMultipleObjectsEx returned WAIT_FAILED");
			} else {
				ret.internalSetupError = true;
				COMPOSITE_REPORT_ERROR(ret,"WaitForMultipleObjectsEx returned unexpected value: ",static_cast<uint32>(reason));
			}
		}
	}

	if (ret){
		ret.SetNonErrorCode(index);
	}

	return ret;
}


} //MARTe
