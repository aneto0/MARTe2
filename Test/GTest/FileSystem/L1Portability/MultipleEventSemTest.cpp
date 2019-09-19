/**
 * @file MultipleEventSemTest.cpp
 * @brief Source file for class MultipleEventSemTest
 * @date 02/07/2015
 * @author Giuseppe Ferro
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

 * @details This source file contains the definition of all the methods for
 * the class MultipleEventSemTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>



#include "MutexSem.h"
#include "Threads.h"
#include "Sleep.h"
#include "HighResolutionTimer.h"
#include "Seconds.h"
#include "ErrorManagement.h"
#include "CompositeErrorManagement.h"
#include "MultipleEventSemTest.h"
#include "Vector.h"
#include "StaticCString.h"
#include "CString.h"
#include "MultipleEventSem.h"
#include "Semaphore.h"
#include "BasicUDPSocket.h"



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


class LocalSharedData{
public:
	volatile int32 sharedVariable;
	Semaphore sem;
	Semaphore sem2;
	Semaphore sem3;
	Semaphore sem4;
	MultipleEventSem msem;
	Semaphore *semP;

	MilliSeconds timeout;

	LocalSharedData(){
		semP = NULL;
	}
private:
	void operator=(const LocalSharedData &lsd){}
};


#define RTEventLoggerChars 16
class RTEventLogger{
	struct EventRecord{
		uint32 actorId;
		CCString constantMessage;
		char8 buffer[RTEventLoggerChars];
		MicroSeconds delay;

	};

public:

	RTEventLogger(int32 nEventsIn):buffer((uint32)nEventsIn){
		maxEvents = nEventsIn;
//		buffer = static_cast<EventRecord*>(HeapManager::Malloc(nEventsIn*sizeof(EventRecord)));
		Start();
	}
	~RTEventLogger(){
//		HeapManager::Free((void *&)buffer);
	}

	void Start(){
		lastRecord = -1;
		startTime = HighResolutionTimer::GetTicks();
	}
	/**
	 * note message must be persistent
	 */
	void RecordEvent(uint32 id,CCString constantMessage){
		if (lastRecord < maxEvents){
			int32 record = Atomic::Increment(&lastRecord);
			if (record < maxEvents){
				Ticks eventTime = HighResolutionTimer::GetTicks();
				Ticks deltaT = eventTime - startTime;
				buffer[record].delay = deltaT;
				buffer[record].actorId = id;
				buffer[record].constantMessage = constantMessage;
				buffer[record].buffer[0] = '\0';
			}
		}
	}

	CStringTool GetEventBuffer(uint32 id){
		if (lastRecord < maxEvents){
			int32 record = Atomic::Increment(&lastRecord);
			if (record < maxEvents){
				Ticks eventTime = HighResolutionTimer::GetTicks();
				Ticks deltaT = eventTime - startTime;
				buffer[record].delay = deltaT;
				buffer[record].actorId = id;
				buffer[record].constantMessage = &buffer[record].buffer[0];
				StaticCString<RTEventLoggerChars> message(buffer[record].buffer);
				return message();
			}
		}
		static CString dummy;
		return dummy();
	}

	void OutputReport(FILE *f){
		for (int i = 0;(i<=lastRecord) && (i < maxEvents);i++){

			fprintf (f,"@%i(us) actor%i %s\n",buffer[i].delay.GetTimeRaw(),buffer[i].actorId,buffer[i].constantMessage.GetList());
		}
	}
private:
	Ticks startTime;
	volatile int32 lastRecord;
	int32 maxEvents;
	Vector<EventRecord> buffer;
};

static RTEventLogger logger(4096);


class SyncTestObject {
public:

	virtual ~SyncTestObject(){}

	/**
	 * initialises the object.
	 * after this EventSource is ready
	 */
	virtual ErrorManagement::ErrorType Initialise()=0;

	/**
	 * creates an event into the event source
	 */
	virtual ErrorManagement::ErrorType CreateEvent()=0;

	/**
	 * removes events from the event source
	 */
	virtual ErrorManagement::ErrorType ResetEvent()=0;

	/**
	 * allows retrieving the handle to the event
	 */
	virtual operator EventSource();

private:
};


class Semaphore_STO: public SyncTestObject{
public:

	Semaphore_STO(){
	}


	virtual ~Semaphore_STO(){
	}

	/**
	 * initialises the object.
	 * after this EventSource is ready
	 */
	virtual ErrorManagement::ErrorType Initialise(){
		ErrorManagement::ErrorType ret;

		ret = sem.Open(Semaphore::Latching);

		REPORT_ERROR(ret," Semaphore_STO error sem.Open");

		if (ret){
			ret = sem.Reset();
			REPORT_ERROR(ret," Semaphore_STO error sem.Reset");
		}

		return ret;
	}

	/**
	 * creates an event into the event source
	 */
	virtual ErrorManagement::ErrorType CreateEvent(){
		ErrorManagement::ErrorType ret;

		ret = sem.Set();
		REPORT_ERROR(ret," Semaphore_STO error sem.Set");

		return ret;
	}

	/**
	 * removes events from the event source
	 */
	virtual ErrorManagement::ErrorType ResetEvent(){
		ErrorManagement::ErrorType ret;

		ret = sem.Reset();
		REPORT_ERROR(ret," Semaphore_STO error sem.Reset");

		return ret;
	}

	/**
	 * allows retrieving the handle to the event
	 */
	virtual operator EventSource(){
		return sem;
	}

private:

	Semaphore sem;
};



class UDPSocket_STO: public SyncTestObject{

	const uint32 testingPort;
public:

	UDPSocket_STO(uint32 port=49152): testingPort(port){
	}


	virtual ~UDPSocket_STO(){
	}

	/**
	 * initialises the object.
	 * after this EventSource is ready
	 */
	virtual ErrorManagement::ErrorType Initialise(){
		ErrorManagement::ErrorType ret;

		ret.OSError = !socketRecv.Open();
		REPORT_ERROR(ret," UDPSocket_STO error socketRecv.Open");

		if (ret){
		    ret.OSError = !socketRecv.Listen(testingPort);
			REPORT_ERROR(ret," Semaphore_STO error socketRecv.Listen");
		}

		if (ret){
		    ret.OSError = !socketSend.Open();
			REPORT_ERROR(ret," Semaphore_STO error socketSend.Open");
		}

		if (ret){
		    ret.OSError = !socketSend.Connect("127.0.0.1",testingPort);
			REPORT_ERROR(ret," Semaphore_STO error socketSend.Open");
		}

		return ret;
	}

	/**
	 * creates an event into the event source
	 */
	virtual ErrorManagement::ErrorType CreateEvent(){
		ErrorManagement::ErrorType ret;

		uint32 size = 3;
    	ret.OSError = !socketSend.Write("Hey", size);
		REPORT_ERROR(ret," socketSend error socketSend.Write");

		return ret;
	}

	/**
	 * removes events from the event source
	 */
	virtual ErrorManagement::ErrorType ResetEvent(){
		ErrorManagement::ErrorType ret;

		char buffer[4];
		uint32 size = 3;
    	ret.OSError = !socketRecv.Read(&buffer[0], size);
		REPORT_ERROR(ret," Semaphore_STO error socketRecv.Read");

		return ret;
	}

	/**
	 * allows retrieving the handle to the event
	 */
	virtual operator EventSource(){
		return socketSend.GetReadEvent();
	}

private:

	BasicUDPSocket socketRecv;
	BasicUDPSocket socketSend;
};


static void ThreadMultiWait(LocalSharedData *tt) {
	ErrorManagement::ErrorType ret;
	// NOTIFY of start completed

	Semaphore localSem;
	ret = localSem.Open(Semaphore::Latching);
	REPORT_ERROR(ret," ThreadMultiWait error localSem.Open");

	if (ret){
		ret = localSem.Reset();
		REPORT_ERROR(ret," ThreadMultiWait error localSem.Open");
	}

	if (ret){
		ret = tt->msem.AddEvent(localSem);
		REPORT_ERROR(ret," ThreadMultiWait error msem.AddEvent(localSem)");
	}

	uint32 id = (uint32)Atomic::Increment(&tt->sharedVariable);

	// wake up main thread
	if (ret){
	    ret = tt->sem2.Set();
		logger.RecordEvent(id," sem2.Set() done");
		REPORT_ERROR(ret," ThreadMultiWait error sem2.Set");
	}

	// wait go trigger!
	if (ret){
		logger.RecordEvent(id," sem4.Take...");
	    ret = tt->sem4.Take(tt->timeout);
		logger.RecordEvent(id," sem4.Taken");
    	REPORT_ERROR(ret," ThreadMultiWait failed sem4.Take");
	}

	tt->sharedVariable = (int32)id;
	tt->semP = &localSem;
//	Sleep::Short(10,Units::ms);

	// wake up main thread
	if (ret){
	    ret = localSem.Set();
		logger.RecordEvent(id," localSem Set");
    	REPORT_ERROR(ret," ThreadMultiWait localSem.Set()");
	}

	// wait end trigger
	if (ret){
	    ret = tt->sem.Take(tt->timeout);
		logger.RecordEvent(id," sem taken");
    	REPORT_ERROR(ret," ThreadMultiWait timeout sem.Take");
	}

	// notify termination
    if (ret){
        ret = tt->sem2.Set();
        logger.RecordEvent(id,"sem2.Set done");
    	REPORT_ERROR(ret," ThreadMultiWait failed sem3.Set");
    }

	Threads::EndThread();
    logger.RecordEvent(id," end of thread");
}


bool MultipleEventSemTest::TestMultiWait_Threads(uint32 nOfThreads,MilliSeconds timeout,FILE *errorDetails) {
//return false;
	ErrorManagement::ErrorType ret;
	LocalSharedData shared;
	shared.sharedVariable = 0;
	shared.timeout = timeout;

	Vector<ThreadIdentifier> tid(nOfThreads);

	for (uint32 i = 0U;i < nOfThreads;i++){
		tid[i] = InvalidThreadIdentifier;
	}
    ret = shared.sem.Open(Semaphore::Latching);
    if (ret){
    	ret = shared.sem.Reset();
    	REPORT_ERROR(ret,"shared.sem.Reset() failed");
    }
    if (ret){
    	ret = shared.sem4.Open(Semaphore::AutoResetting);
    	REPORT_ERROR(ret,"shared.sem4.Open(Semaphore::AutoResetting) failed");
    }
    if (ret){
    	ret = shared.sem4.Reset();
    	REPORT_ERROR(ret,"shared.sem4.Reset() failed");
    }
    if (ret){
    	ret = shared.sem2.Open(Semaphore::MultiLock);
    	REPORT_ERROR(ret,"shared.sem2.Open(Semaphore::MultiLock) failed");
    }
    if (ret){
    	ret = shared.sem2.Reset(nOfThreads);
    	REPORT_ERROR(ret,"shared.sem2.Reset() failed");
    }
    if (ret){
    	logger.Start();
    	logger.RecordEvent(0,"starting threads");
    	for (uint32 i = 0U;i < nOfThreads;i++){
    		tid[i] = Threads::BeginThread((ThreadFunctionType) ThreadMultiWait, &shared);
    	}
    	logger.RecordEvent(0,"threads started - waiting on sem2");
    	ret = shared.sem2.Take(timeout);
    	logger.RecordEvent(0,"sem2 taken");
    	COMPOSITE_REPORT_ERROR(ret,"shared.sem2.Take(",timeout.GetTimeRaw(),") failed");

    	if (ret){
        	ret.fatalError = (shared.sharedVariable != static_cast<int32>(nOfThreads));
        	COMPOSITE_REPORT_ERROR(ret,"sharedVariable should have reached the value ",nOfThreads," instead it's value is ",shared.sharedVariable );
    	}
    }

    for (uint32 i = 0U;(i < nOfThreads) && ret;i++){
    	// trigger random thread
        if (ret){
        	logger.RecordEvent(0,"sem4 setting now ....");
        	ret = shared.sem4.Set();
        	logger.RecordEvent(0,"sem4 set()");
        	REPORT_ERROR(ret,"shared.sem4.Set() failed");
        }

        // wait thread
        if (ret){
        	logger.RecordEvent(0,"msem wait()");
        	ret = shared.msem.Wait(timeout);
        	if (ret){
        		int32 triggered = (int32)ret.GetNonErrorCode();
            	logger.GetEventBuffer(0).Append("triggered=").Append(triggered);

            	ret.fatalError = (triggered != shared.sharedVariable);
            	COMPOSITE_REPORT_ERROR(ret,"mismatch between triggering thread:",shared.sharedVariable," and triggered semaphore:",triggered);
        	} else {
            	logger.GetEventBuffer(0).Append("timeout <=").Append(shared.sharedVariable);
            	REPORT_ERROR(ret,"msem.Wait failed");
        	}
        }

        // reset thread semaphore
        if (ret){
        	shared.semP->Reset();
        }
    }

    if (ret){
    	ret = shared.sem2.Reset(nOfThreads);
    	REPORT_ERROR(ret,"shared.sem2.Reset() failed");
    }

    if (ret){
    	logger.RecordEvent(0,"sem setting now ....");
    	ret = shared.sem.Set();
    	logger.RecordEvent(0,"sem set()");
    	REPORT_ERROR(ret,"shared.sem.Set() failed");
    }

    if (ret){
    	ret = shared.sem2.Take(timeout);
    	logger.RecordEvent(0,"sem2 Taken");
    	COMPOSITE_REPORT_ERROR(ret,"shared.sem2.Take(",timeout.GetTimeRaw(),") failed");
    }

    if (ret){
    	printf("OK! ");
    } else {
    	printf("NO! ");
    	logger.OutputReport(errorDetails);
    }

    Sleep::Short(50,Units::ms);;

	for (uint32 i = 0;i < nOfThreads;i++){
		if (tid[i] != InvalidThreadIdentifier){
	        if (Threads::IsAlive(tid[i])) {
                Threads::Kill(tid[i]);
	        }
		}
	}

    shared.sem.Close();
    return ret;
}
