
/**
 * @file Semaphore.cpp
 * @brief Header file for class AnyType
 * @date 22 Aug 2019
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
#define DLL_API


//#include <intrin.h>
#include "Semaphore.h"
#include "CompositeErrorManagement.h"
#include "Atomic.h"
#include "Threads.h"



namespace MARTe{

Semaphore::Semaphore(){
	mode = Invalid;
	status = 0;
	owner = 0;
}

Semaphore::~Semaphore(){
	Close();
}

ErrorManagement::ErrorType Semaphore::Open(SemaphoreMode mode){
	ErrorManagement::ErrorType ret;

	ret = lockHev.Open();

	if (ret){
		ret = this->Synchronizer::Open();
	}

	if (ret){
		this->mode = mode;
	}

	return ret;

}

/**
 * @brief Closes the semaphore.
 * @return true if the operating system call returns with no errors.
 */
ErrorManagement::ErrorType Semaphore::Close(){
	ErrorManagement::ErrorType ret;

	status = 0xFFFF;
	mode = Exit;
	ret = this->Synchronizer::Post();
	REPORT_ERROR(ret,"Close(): Synchronizer::Post failed ");

	ret = lockHev.Post();
	REPORT_ERROR(ret,"Close(): lockHev.Post failed ");

	//	todo check on waiters == 0

	ret = this->Synchronizer::Close();
	REPORT_ERROR(ret,"Close(): Synchronizer::Close failed ");

	ret = lockHev.Close();
	REPORT_ERROR(ret,"Close(): lockHev.Close failed ");

	return ret;

}


ErrorManagement::ErrorType Semaphore::Lock(MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;

	Atomic::Increment(&waiters);

	while (!lock.FastTryLock() && ret){
		ret = lockHev.Wait(timeout);
	}

	Atomic::Decrement(&waiters);
	return ret;
}


ErrorManagement::ErrorType Semaphore::UnLock(){
	ErrorManagement::ErrorType ret;

	lock.FastUnLock();
	ret = lockHev.Post();

	return ret;
}

ErrorManagement::ErrorType Semaphore::Take(const MilliSeconds &timeout,bool resetBeforeWait){
	ErrorManagement::ErrorType ret;
	MilliSeconds timeoutCopy(timeout);
	Atomic::Increment(&waiters);

	int32 sampledStatus = 0;

	do {
		ret = Lock(timeoutCopy);
		REPORT_ERROR(ret,"Lock failed ");

		if (ret){
			switch (mode){
			case Latching:{
				sampledStatus = status;
			}break;
			case AutoResetting:{
				if (status > 0){
					sampledStatus = 1;
					status = 0;
				}
			}break;
			case Counting:{
				if (status > 0){
					sampledStatus = 1;
					status--;
				}
			}break;
			case Mutex:{
				// locked
				if (status <= 0){
					if (Threads::Id() == owner){
						sampledStatus = 1;
						status--;
					}
				} else {
					sampledStatus = 1;
					status = 0;
					owner = Threads::Id();
				}
			}break;
			case Exit:{
				sampledStatus = 1;
			}break;
			default:{
				ret.unsupportedFeature = true;
				COMPOSITE_REPORT_ERROR(ret,"mode has an unsupported value",mode);
			}
			}
		}

		if (ret && (sampledStatus == 0)){

			ret = this->Synchronizer::Reset();
			REPORT_ERROR(ret,"Synchronizer::Reset failed ");

			if (ret){
				ret = UnLock();
				REPORT_ERROR(ret,"UnLock failed ");
			}

			if (ret){
				ret = this->Synchronizer::WaitUpdate(timeoutCopy);
				REPORT_ERROR(ret,"Synchronizer::Wait failed ");
			}
		}

	} while(ret && (sampledStatus == 0));

	if (lock.Locked()){
		ret = UnLock();
	}

	Atomic::Decrement(&waiters);

	return ret;
}


ErrorManagement::ErrorType Semaphore::Reset(){
	ErrorManagement::ErrorType ret;
	MilliSeconds timeoutCopy(MilliSeconds::Infinite);

	ret = Lock(timeoutCopy);
	REPORT_ERROR(ret,"Lock failed ");

	if (ret){
		if (mode == Mutex){
			ret.illegalOperation = (Threads::Id() != owner);
			REPORT_ERROR(ret,"ReSet is invalid for non-owner of mutex");

			if (ret){
				if (status > 0){
					status--;
				}
			}
		} else {
			status = 0;
		}
	}

	if (lock.Locked()){
		ret = UnLock();
	}

	return ret;

}

ErrorManagement::ErrorType Semaphore::Set(uint32 count){
	ErrorManagement::ErrorType ret;
	MilliSeconds timeoutCopy(MilliSeconds::Infinite);

	ret = Lock(timeoutCopy);
	REPORT_ERROR(ret,"Lock failed ");

	if (ret){
		switch (mode){
		case Latching:{
			status = 1;
		}break;
		case AutoResetting:{
			status = 1;
		}break;
		case Counting:{
			status += count;
		}break;
		case Mutex:{
			ret.invalidOperation = true;
			REPORT_ERROR(ret,"Set is invalid for Mutex");
		}break;
		default:{
			ret.unsupportedFeature = true;
			COMPOSITE_REPORT_ERROR(ret,"mode has an unsupported value",mode);
		}
		}
	}

	if (lock.Locked()){
		ret = UnLock();
	}

	return ret;

}


} //MARTe
