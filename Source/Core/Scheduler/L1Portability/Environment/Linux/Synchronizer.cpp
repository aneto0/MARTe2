/**
 * @file Synchronizer.cpp
 * @brief Header file for class AnyType
 * @date 25 Aug 2019
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

#include <sys/eventfd.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>

#include "Synchronizer.h"
#include "TypeCharacteristics.h"


namespace MARTe{

/**
 *
 */
Synchronizer::Synchronizer(){
	pfd.fd = -1;
}

/**
 *
 */
Synchronizer::~Synchronizer(){
	if (pfd.fd != -1){
		Close();
	}
}


EventSource Synchronizer::GetEventSource(){
	EventSource es;
	EventSourceData *esd = es.GetData();
	if (esd != NULL_PTR(EventSourceData)){
		esd->pfd.fd = data.eventfd;
	}
	return es;
}


/**
 * Creates the waitable handle
 */
ErrorManagement::ErrorType Synchronizer::Open(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (pfd.fd >= 0);
	REPORT_ERROR(ret,"Synchronyzer::Open() already opened");

	if (ret){
		pfd.fd = eventfd(0UL, EFD_SEMAPHORE | EFD_NONBLOCK);
		ret.OSError = (pfd.fd < 0);
		REPORT_ERROR(ret,"Synchronyzer::Open failed");
	}

	return ret;
}

/**
 * Closes the waitable handle
 */
ErrorManagement::ErrorType Synchronizer::Close(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (pfd.fd < 0);
	REPORT_ERROR(ret,"Synchronyzer::Close() not opened");

	if (ret){
		ret.OSError = (close(pfd.fd) < 0);
		REPORT_ERROR(ret,"Synchronyzer::Close() failed");
		pfd.fd = -1;
	}

	return ret;
}

/**
 * Opens the lock
 */
ErrorManagement::ErrorType Synchronizer::Post(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (pfd.fd < 0);
	REPORT_ERROR(ret,"Synchronyzer::Post() not opened");

	if (ret){
		uint64 number = 1UL;
		ret.OSError = (write(pfd.fd,&number,sizeof(number)) < 0);
		REPORT_ERROR(ret,"Synchronyzer::Post() failed");
	}

	return ret;
}

/**
 * Closes the lock
 */
ErrorManagement::ErrorType Synchronizer::Reset(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (pfd.fd < 0);
	REPORT_ERROR(ret,"Synchronyzer::Post() not opened");

	if (ret){
		uint64 number = 1UL;
		int readRet = read(pfd.fd,&number,sizeof(number));
		if (readRet < 0){
			ret.OSError = (errno != EAGAIN);
			REPORT_ERROR(ret,"Synchronyzer::Reset() failed");
		}
	}

	return ret;
}

/**
 * Waits for the lock to open
 */
ErrorManagement::ErrorType Synchronizer::Wait(MilliSeconds timeout){
	ErrorManagement::ErrorType ret;
	int time = 0;
	if (timeout.IsValid()){
		uint32 tRaw = timeout.GetTimeRaw();
		if (tRaw >= static_cast<uint32>(TypeCharacteristics<int>::MaxValue())){
			time = TypeCharacteristics<int>::MaxValue();
		} else {
			time = static_cast<int>(tRaw);
		}
	} else {
		if (timeout.IsInfinite()){
			time = -1;
		}
	}

	ret.invalidOperation = (pfd.fd < 0);
	REPORT_ERROR(ret,"Synchronyzer::Post() not opened");

	if (ret){
//		struct pollfd pfd;
//		pfd.fd = pfd.fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		int wret = poll(&pfd, 1, time);

	    ret.OSError = (wret < 0);
		REPORT_ERROR(ret,"Synchronyzer::Wait() failed");

		ret.timeout =  (wret == 0) ;
	}

	return ret;

}



} //MARTe
