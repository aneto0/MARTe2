
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

#include <stdio.h>

#include "Semaphore.h"
#include "CompositeErrorManagement.h"
#include "Atomic.h"
#include "Threads.h"

namespace MARTe{

Semaphore::Semaphore(){
	mode = Closed;
	status = 0;
	owner = 0;
	waiters = 0;
}

Semaphore::~Semaphore(){
	Close();
}

int32 Semaphore::Status() const{
	return status;
}

int32 Semaphore::Waiters() const{
	return waiters;
}

Semaphore::SemaphoreMode Semaphore::Mode() const{
	return mode;
}

ErrorManagement::ErrorType Semaphore::Open(SemaphoreMode mode){
	ErrorManagement::ErrorType ret;

	ret.illegalOperation =  (this->mode != Closed);
	REPORT_ERROR(ret,"Open(): Semaphore already opened ");

	if (ret){
		ret = lockHev.Open();
		REPORT_ERROR(ret,"Open(): failed opening lockHev");
	}

	if (ret){
		ret = this->Synchronizer::Open();
		REPORT_ERROR(ret,"Open(): failed opening main Synchronizer");
	}

	if (ret){
		this->mode = mode;

		if (mode == Mutex){
			status = 1;  // no taken
			owner  = 0;
		}
	}

	return ret;
}

/**
 * @brief Closes the semaphore.
 * @return true if the operating system call returns with no errors.
 */
ErrorManagement::ErrorType Semaphore::Close(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = ((mode == Invalid) || (mode == Closed));

	if (ret){

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

		if (ret){
			mode = Closed;
		} else {
			mode = Invalid;
		}
	}

	return ret;

}


ErrorManagement::ErrorType Semaphore::Lock(MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;

	// in case the lock was cleared just before we reset the lockHev
	// we perform the Reset Wait in two steps
	bool resetStage = true;
	while (!lock.FastTryLock() && ret){
		if (resetStage){
			lockHev.Reset();
			resetStage = false;
		} else {
			ret = lockHev.WaitUpdate(timeout);
			resetStage = true;
		}
	}
	return ret;
}


ErrorManagement::ErrorType Semaphore::UnLock(){
	ErrorManagement::ErrorType ret;

	lock.FastUnLock();
	ret = lockHev.Post();

	return ret;
}

ErrorManagement::ErrorType Semaphore::Take(const MilliSeconds &timeout){
	ErrorManagement::ErrorType ret;
	Atomic::Increment(&waiters);

	MilliSeconds timeoutCopy(timeout);
	int32 sampledStatus = 0;
	do {
		sampledStatus = 0;

		ret = Lock(timeoutCopy);
		REPORT_ERROR(ret,"Lock failed ");

		bool wasLocked = (status <= 0);

		if (ret){
			switch (mode){
			case MultiLock:
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

		if (ret){
			ret = UpdateLockHev(wasLocked);
		}

		if (ret && (sampledStatus <= 0)){
//			ret = this->Synchronizer::Reset();
//			REPORT_ERROR(ret,"Synchronizer::Reset failed ");

			if (ret){
				ret = UnLock();
				REPORT_ERROR(ret,"UnLock failed ");
			}

			if (ret){
				ret = this->Synchronizer::WaitUpdate(timeoutCopy);
				if (ret.timeout == false){
					REPORT_ERROR(ret,"Synchronizer::Wait failed ");
				}
			}
		}

	} while(ret && (sampledStatus <= 0));

	if (lock.Locked()){
		ret = UnLock();
	}

	Atomic::Decrement(&waiters);

	return ret;
}

ErrorManagement::ErrorType Semaphore::UpdateLockHev(bool wasLocked){
	ErrorManagement::ErrorType ret;

	bool isLocked = (status <= 0);
	if (isLocked != wasLocked){
		if (isLocked){
			ret = this->Synchronizer::Reset();
			REPORT_ERROR(ret,"Synchronizer::Reset failed ");
		} else {
			ret = this->Post();
			REPORT_ERROR(ret,"Synchronizer Post failed");
		}
	}

	return ret;
}


ErrorManagement::ErrorType Semaphore::Reset(uint32 count){
	ErrorManagement::ErrorType ret;
	MilliSeconds timeoutCopy(MilliSeconds::Infinite);

	ret = Lock(timeoutCopy);
	REPORT_ERROR(ret,"Lock failed ");

	// to avoid Resetting before waiting
	bool wasLocked = (status <= 0);

	if (ret){
		if (mode == MultiLock){
			status = 1 - static_cast<int32>(count);
		} else
		if (mode == Mutex){
			ret.invalidOperation = true;
			REPORT_ERROR(ret,"Reset is invalid for Mutex");
		} else {
			status = 0;
		}
	}

#if 1
	if (ret){
		ret = UpdateLockHev(wasLocked);
	}
#else
	if (ret && (waiters > 0) && (status > 0)){
		ret = this->Post();
		REPORT_ERROR(ret,"Synchronizer Post failed");
	}
#endif

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

	// to avoid Resetting before waiting
	bool wasLocked = (status <= 0);

	if (ret){
		switch (mode){
		case Latching:{
			status = 1;
		}break;
		case AutoResetting:{
			status = 1;
		}break;
		case MultiLock:{
			if (status <= 0){
				status++;
			}
		}break;
		case Counting:{
			status += count;
		}break;
		case Mutex:{
			ret.illegalOperation = (Threads::Id() != owner);
			REPORT_ERROR(ret,"ReSet is invalid for non-owner of mutex");

			if (ret){
				if (status <= 0){
					status++;
				}
				if (status == 1){
					owner = 0;
				}
			}
		}break;
		default:{
			ret.unsupportedFeature = true;
			COMPOSITE_REPORT_ERROR(ret,"mode has an unsupported value",mode);
		}
		}
	}

#if 1
	if (ret){
		ret = UpdateLockHev(wasLocked);
	}
#else
	if (ret && (waiters > 0) && (status > 0)){
		ret = this->Post();
		REPORT_ERROR(ret,"Synchronizer Post failed");
	}
#endif

	if (lock.Locked()){
		ret = UnLock();
		REPORT_ERROR(ret,"Unlock failed");
	}

	return ret;
}


} //MARTe
