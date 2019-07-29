/**
 * @file ThreadsTest.cpp
 * @brief Source file for class ThreadsTest
 * @date 25/06/2015
 * @author Giuseppe Ferr�
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
 * the class ThreadsTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ThreadsTest.h"
#include "Atomic.h"
#include "Sleep.h"
#include "ThreadInformation.h"
#include "Processor.h"
#include "Vector.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
//extern uint32 nOfEntries=0;
#define exitStatusWait 1
#define exitStatusGo 0



class ThreadTestEngine{
public:
	/* A shared variable used for synchronization. */
    int32 exitStatus;

    /* A shared variable used for synchronization */
    int32 actionCounter;

    /* A variable used to save a thread identifier. */
    ThreadIdentifier tidTest;

    /* A boolean to store the return value. */
    bool retValue;

    /* How many threads to start in this test */
    int32 nOfThreads;

    /* The thread ids */
    Vector<ThreadIdentifier> tids;

    /* the thread names */
    Vector<DynamicCString> tidN;

	uint32 stackSize;

public:
    ThreadTestEngine(int32 nOfThreadsIn){
        tids.SetSize(static_cast<uint32>(nOfThreadsIn));
        tidN.SetSize(static_cast<uint32>(nOfThreadsIn));
        exitStatus = exitStatusWait;
        actionCounter = 0;
        tidTest = 0;
        retValue = true;
        nOfThreads = nOfThreadsIn;
    	stackSize = THREADS_DEFAULT_STACKSIZE;
    }

    virtual ~ThreadTestEngine(){}

    void InitCounters(){
    	exitStatus = exitStatusWait;
    	actionCounter = 0;
    }

    // signal all threads that they can stop
    void StopThreads(){
    	exitStatus = exitStatusGo;
    	actionCounter = 0;
    }

    // wait until nOfThreads have signalled to have finished
    bool WaitCounter(int32 nOfThreads=1){
    	int32 j=0;
    	while (actionCounter < nOfThreads) {
        	if (j++ > 200) {
        	    return false;
        	}
    	Sleep::Short(10,Units::ms);\
    	}
    	return true;
    }

    /**
     * starts n Threads
     * check for correct startup
     * execute user check
     * initiates threads shutdown
     * checks for correct shutdown
     */
    bool StandardTest();

    virtual bool UserCheckFunction(){
    	return true;
    }

    virtual void UserStartThread(uint32 index);

};

//  thread function that increments the actionCounter on start and on exit. Will exit after exitStatus is 0
static void DummyFunction(ThreadTestEngine *tt) {
    //tells to the main process that the thread begins
    Atomic::Increment(&tt->actionCounter);
    //waits the permission of the main process to exit
    while (tt->exitStatus == exitStatusWait) {
        Sleep::Short(1,Units::ms);
    }
    Atomic::Increment(&tt->actionCounter);
    Threads::EndThread();
}

void ThreadTestEngine::UserStartThread(uint32 index){
    tids[index] = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, stackSize, tidN[index]);
}

/**
 * starts n Threads
 * check for correct startup
 * execute user check
 * initiates threads shutdown
 * checks for correct shutdown
 */
bool ThreadTestEngine::StandardTest(){
	bool ret = true;

    if (Threads::NumberOfThreads() != 0) {
    	REPORT(ErrorManagement::FatalError,"Other threads running in the system");
        return false;
    }

    bool failed = false;

	InitCounters();

	//Calls the thread callback.
    for (int32 i = 0; (i < nOfThreads) ; i++) {
    	UserStartThread(static_cast<uint32>(i));
    }

    if (!WaitCounter(nOfThreads)){
    	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"failed starting threads: ",actionCounter, " out of ", nOfThreads);
        ret = false;
    }

    if (ret){
    	failed = !UserCheckFunction();
    }

    StopThreads();

    if (!WaitCounter(nOfThreads)){
        ret = false;
    	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"failed stopping threads nicely: ",actionCounter, " out of ", nOfThreads);
    }

    if (!ret){
    	REPORT(ErrorManagement::FatalError,"Killing threads");
        for (int32 i = 0; i <= nOfThreads; i++) {
            Threads::Kill(tids[static_cast<uint32>(i)]);
        }
    }

    int32 counter = 10;
    while ((Threads::NumberOfThreads() != 0) && (counter > 0)){
    	counter--;
    	Sleep::Short(10000,Units::us);
    }

    if (Threads::NumberOfThreads() != 0) {
    	REPORT(ErrorManagement::FatalError,"Threads still running in the system");
        ret = false;
    }

    if (failed) {
    	REPORT(ErrorManagement::FatalError,"User Test failed");
    	ret = false;
    }

    if (Threads::NumberOfThreads() != 0) {
    	REPORT(ErrorManagement::FatalError,"Still threads running?");
    	ret = false;
    }

    return ret;
}


class TBT_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TBT_ThreadTestEngine(){}
	TBT_ThreadTestEngine(int32 numberOfThreads,CCString baseName,uint32 stackSizeIn): ThreadTestEngine(numberOfThreads){
		stackSize = stackSizeIn;
		for (int32 i=0;i<numberOfThreads;i++){
			DynamicCString name;
			name().Append(baseName).Append(i);
			tidN[static_cast<uint32>(i)] = name;
		}
	}
};


bool ThreadsTest::TestBeginThread(CCString name,
                                  uint32 stackSize,
                                  int32 nOfThreads) {

	TBT_ThreadTestEngine tte(nOfThreads,name,stackSize);

	return tte.StandardTest();
}


bool ThreadsTest::TestBeginThreadNullFunction(CCString name) {
//try to launch a thread with null function. The thread is created
//but the function is not launched because is null (see ThreadInformation::UserThreadFunction)
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) NULL, NULL, THREADS_DEFAULT_STACKSIZE, name);

    return tid != (ThreadIdentifier) 0;
}

class TIA_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TIA_ThreadTestEngine(){}
	TIA_ThreadTestEngine(int32 numberOfThreads): ThreadTestEngine(numberOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool success = true;
    	for (int32 i =0;i<nOfThreads;i++){
            if (!Threads::IsAlive(tids[static_cast<uint32>(i)])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," is not alive")
                success = false;
            }
    	}
    	return success;
    }
};

bool ThreadsTest::TestIsAlive(int32 nOfThreads) {
	TIA_ThreadTestEngine tte(nOfThreads);
	return tte.StandardTest();
}

class TIAK_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TIAK_ThreadTestEngine(){}
	TIAK_ThreadTestEngine(int32 numberOfThreads): ThreadTestEngine(numberOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool success = true;
    	for (int32 i =0;i<nOfThreads;i++){
            Threads::Kill(tids[static_cast<uint32>(i)]);
    	}
    	for (int32 i =0;i<nOfThreads;i++){
    		if (Threads::IsAlive(tids[static_cast<uint32>(i)])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," is alive after Kill")
                success = false;
            }
    	}
    	if (success) nOfThreads = 0;
    	return success;
    }
};

bool ThreadsTest::TestIsAliveAfterkill(int32 nOfThreads) {
	TIAK_ThreadTestEngine tte(nOfThreads);
	return tte.StandardTest();
}

bool ThreadsTest::TestKillInvalidID() {
    //try to kill an invalid thread
    return !(Threads::Kill(InvalidThreadIdentifier));
}

class TK_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TK_ThreadTestEngine(){}
	TK_ThreadTestEngine(int32 numberOfThreads): ThreadTestEngine(numberOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool success = true;
    	for (int i =0;i<nOfThreads;i++){
            if (!Threads::Kill(tids[static_cast<uint32>(i)])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," Kill failed")
            	success = false;
            }
    	}
    	for (int i =0;i<nOfThreads;i++){
            if (Threads::Kill(tids[static_cast<uint32>(i)])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," Kill succeeded after Kill")
            	success = false;
            }
    	}
    	for (int i =0;i<nOfThreads;i++){
    		if (Threads::IsAlive(tids[static_cast<uint32>(i)])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," is alive after double Kill")
                success = false;
            }
    	}
    	if (success) nOfThreads = 0;
    	return success;
    }
};

bool ThreadsTest::TestKill(int32 nOfThreads) {
	TK_ThreadTestEngine tte(nOfThreads);
	return tte.StandardTest();
}

class TP_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TP_ThreadTestEngine(){}
	TP_ThreadTestEngine(): ThreadTestEngine(64){
	}
    virtual bool UserCheckFunction(){
        //Available priority classes
        Threads::PriorityClassType allPrioClassTypes[] = {
        		Threads::UnknownPriorityClass,
    			Threads::IdlePriorityClass,
    			Threads::NormalPriorityClass,
                Threads::RealTimePriorityClass
        };

        bool success = true;
    	for (int i =0;i<nOfThreads;i++){
    		uint32 pc = static_cast<uint32>(i) /16;
    		uint32 pl = static_cast<uint32>(i) %16;
            //set priority level and class
            Threads::SetPriority(tids[static_cast<uint32>(i)], allPrioClassTypes[pc], pl);

            //gets priority level and class
            Threads::PriorityClassType prio =  Threads::GetPriorityClass(tids[static_cast<uint32>(i)]);
            if (pc!=0 && (prio != allPrioClassTypes[pc])) {
                success = false;
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," GetPriorityClass mismatch: ", prio, " != ",allPrioClassTypes[pc]);
            }

            uint8 level = Threads::GetPriorityLevel(tids[static_cast<uint32>(i)]);
            if (level != pl) {
                success = false;
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," GetPriorityLevel mismatch: ",level, " != " ,pl )
            }
    	}
    	return success;
    }
};

bool ThreadsTest::TestPriority() {

    ThreadIdentifier fakeTid = (ThreadIdentifier) 0;
    if (Threads::GetPriorityClass(fakeTid) != Threads::UnknownPriorityClass || Threads::GetPriorityLevel(fakeTid) != 0) {
        return false;
    }
    TP_ThreadTestEngine tte;

    return tte.StandardTest();
}

class GS_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~GS_ThreadTestEngine(){}
	GS_ThreadTestEngine(): ThreadTestEngine(1){
	}
    virtual bool UserCheckFunction(){
        return (Threads::GetState(tids[0]) == Threads::UnknownThreadStateType) ;
    }
};

bool ThreadsTest::TestGetState() {
	GS_ThreadTestEngine tte;

	return tte.StandardTest();
}


//  thread function that increments the actionCounter on start and on exit. Will exit after exitStatus is 0
static void TestIdFunction(ThreadTestEngine *tt) {
    //tells to the main process that the thread begins
    Atomic::Increment(&tt->actionCounter);

    tt->retValue = (Threads::Id() == tt->tids[0]);
    if (!tt->retValue){
    	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"Threads read id = ",static_cast<uint32>(Threads::Id())," instead of ",static_cast<uint32>(tt->tids[0]));
    }

    //waits the permission of the main process to exit
    while (tt->exitStatus == exitStatusWait) {
        Sleep::Short(1,Units::ms);
    }
    Atomic::Increment(&tt->actionCounter);
    Threads::EndThread();
}

class TI_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TI_ThreadTestEngine(){}
	TI_ThreadTestEngine(): ThreadTestEngine(1){
	}
    void UserStartThread(uint32 index){
        tids[static_cast<uint32>(index)] = Threads::BeginThread((ThreadFunctionType) TestIdFunction, this);
    }
};

bool ThreadsTest::TestId(int32 nOfThreads) {
	bool ret = true;
	for (int i =0;i<nOfThreads;i++){
		TI_ThreadTestEngine tte;
		if (!tte.StandardTest()){
			ret = false;
			REPORT(ErrorManagement::FatalError,"Threads::Id does not match");
		}
	}
	return ret;
}

class GC_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~GC_ThreadTestEngine(){}
	GC_ThreadTestEngine(): ThreadTestEngine(Processor::Available()){
	}
    virtual void UserStartThread(uint32 index){
    	ProcessorType pt(0);
    	pt.AddCPU(static_cast<uint32>(index+1));
        tids[static_cast<uint32>(index)] = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, emptyString, ExceptionHandler::NotHandled, pt);
    }
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	for (int i =0;i<nOfThreads;i++){
            ProcessorType pt2(Threads::GetCPUs(tids[static_cast<uint32>(i)]));
            ProcessorType pt(0);
            pt.AddCPU(static_cast<uint32>(i+1));
            if (pt != pt2){
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," GetCPU mismatch: ",pt.GetProcessorMask(),"!=",pt2.GetProcessorMask());
            	failed = true;
            }
    	}
    	return !failed;
    }
};
bool ThreadsTest::TestGetCPUs() {
	GC_ThreadTestEngine tte;
	return tte.StandardTest();
}


class TN_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~TN_ThreadTestEngine(){}
	TN_ThreadTestEngine(int32 nOfThreads): ThreadTestEngine(nOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	for (int i =0;i<nOfThreads;i++){
    		CCString name = Threads::Name(tids[static_cast<uint32>(i)]);
            if (tidN[i].GetSize() == 0){
                if (!name.IsSameAs("Unknown")){
                	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," expecting Unknown instead", name);
                	failed = true;
                }
            } else {
                if (!tidN[i].IsSameAs(name)){
                	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," expecting ",tidN[i], " instead ",name)
                	failed = true;
                }
            }
    	}
    	return !failed;
    }
};
bool ThreadsTest::TestName(CCString name,int32 nOfThreads) {
	TN_ThreadTestEngine tte(nOfThreads);
	if (name.GetSize()>0){
		for (int i=0;i<nOfThreads;i++ ){
			tte.tidN[i]().Append(name).Append(i);
		}
	}
	return tte.StandardTest();
}
bool ThreadsTest::TestNameNull() {
	return TestName(emptyString,1);
}

class NT_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~NT_ThreadTestEngine(){}
	NT_ThreadTestEngine(int32 nOfThreads): ThreadTestEngine(nOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	uint32 N = Threads::NumberOfThreads();
        if (N != static_cast<uint32>(nOfThreads)){
        	failed = true;
        	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"NumberOfThreads() returns ",N," expecting ", nOfThreads);
        }
    	return !failed;
    }
};
bool ThreadsTest::TestNumberOfThreads(int32 nOfThreads) {
	ThreadTestEngine tte(nOfThreads);

	return tte.StandardTest();
}


class FBI_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~FBI_ThreadTestEngine(){}
	FBI_ThreadTestEngine(): ThreadTestEngine(THREADS_DATABASE_GRANULARITY){
	}
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	for (int32 i = 0;i < nOfThreads;i++){
    		ThreadIdentifier t1 = Threads::FindByIndex(static_cast<uint32>(i));
    		ThreadIdentifier t2 = tids[i];
            if (t1 != t2) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"FindByIndex(",i,") returns ",static_cast<uint64>(t1)," expecting ", static_cast<uint64>(t2));
                failed = true;
            }
    	}

    	if (!failed){
            //removes the first thread
            Threads::Kill(tids[0]);
            Sleep::Short(10,Units::ms);
            Atomic::Decrement(&actionCounter);

            //removes the last thread
            Threads::Kill(tids[nOfThreads-1]);
            Sleep::Short(10,Units::ms);
            Atomic::Decrement(&actionCounter);

            //adds another thread
            tids[nOfThreads-1] = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

            tids[0] = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

            if (!WaitCounter(nOfThreads)){
                failed = true;
            	REPORT(ErrorManagement::FatalError,"Failed starting replacement threads");
            }

            //the next position is at the end but there are THREADS_DATABASE_GRANULARITY - 1 threads!
            if (Threads::FindByIndex(nOfThreads-1) != tids[nOfThreads-1]) {
            	REPORT(ErrorManagement::FatalError,"replacement threads 1 is not at last position");
                failed = true;
            }

            //the next position is at the beginning!
            if (Threads::FindByIndex(0) != tids[0]) {
            	REPORT(ErrorManagement::FatalError,"replacement threads 2 is not at first position");
                failed = true;
            }

    	}
    	return !failed;
    }
};
bool ThreadsTest::TestFindByIndex() {
	FBI_ThreadTestEngine tte;
	return tte.StandardTest();
}

class GTI_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~GTI_ThreadTestEngine(){}
	GTI_ThreadTestEngine(int32 nOfThreads): ThreadTestEngine(nOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	for (int32 i = 0;i < nOfThreads;i++){
            ThreadInformation ti;
    		CCString name = tidN[i];

            if (!Threads::GetThreadInfoCopy(ti, tids[i])){
            	failed =  true;
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," GetThreadInfoCopy failed ");
            }
            if (!name.IsSameAs(ti.ThreadName())) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," expecting ",name,"instead", ti.ThreadName());
            	failed =  true;
            }
            if (ti.GetThreadIdentifier() != tids[i]) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," tid mismatch ");
            	failed =  true;
            }
            if (ti.GetPriorityClass() != Threads::NormalPriorityClass) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," priority class not Normal: ", ti.GetPriorityClass());
            	failed =  true;
            }
            if (ti.GetPriorityLevel() != 0) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," priority level mismatch ");
            	failed =  true;
            }

            Threads::SetPriority(tids[i], Threads::IdlePriorityClass, 15);
            Threads::GetThreadInfoCopy(ti, tids[i]);
            if (ti.GetPriorityClass() != Threads::IdlePriorityClass) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," modified priority class mismatch ");
            	failed =  true;
            }
            if (ti.GetPriorityLevel() != 15) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," modified priority level mismatch ");
            	failed =  true;
            }

            //A priority greater than 16 should be set to 15
            Threads::SetPriority(tids[i], Threads::NormalPriorityClass, 16);
            Threads::GetThreadInfoCopy(ti, tids[i]);
            if (ti.GetPriorityLevel() != 15) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," modified priority level mismatch ");
            	failed =  true;
            }

            Threads::SetPriority(tids[i], Threads::NormalPriorityClass, 15);

            //if the index is >=0 the function search by index in the database
            ThreadInformation tiCopy;
            ThreadInformation tiCopy2;
            if (!Threads::GetThreadInfoCopy(tiCopy, tids[i])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," read copy 1 failed");
            	failed =  true;
            }
            if (!Threads::GetThreadInfoCopy(tiCopy2, static_cast<uint32>(i))) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," read copy 2 failed");
            	failed =  true;
            }
            if ( !name.IsSameAs(tiCopy.ThreadName()) || !name.IsSameAs(tiCopy2.ThreadName()) ) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," name mismatch with copies");
            	failed =  true;
            }
            if ((tiCopy.GetThreadIdentifier() != tids[i]) || (tiCopy2.GetThreadIdentifier() != tids[i])) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," tid mismatch with copies");
            	failed =  true;
            }
            if ((tiCopy.GetPriorityClass() != Threads::NormalPriorityClass) || (tiCopy2.GetPriorityClass() != Threads::NormalPriorityClass)) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," priority class mismatch with copies");
            	failed =  true;
            }
            if ((tiCopy.GetPriorityLevel() != 15) || (tiCopy2.GetPriorityLevel() != 15)) {
            	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i," priority level mismatch with copies");
            	failed =  true;
            }
    	}
    	return !failed;
    }
};

bool ThreadsTest::TestGetThreadInfoCopy(int32 nOfThreads,CCString name) {
	GTI_ThreadTestEngine tte(nOfThreads);
	if (name.GetSize()>0){
		for (int i=0;i<nOfThreads;i++ ){
			tte.tidN[i]().Append(name).Append(i);
		}
	}
	return tte.StandardTest();
}

bool ThreadsTest::TestGetThreadInfoCopyInvalidID() {
    ThreadInformation ti;
    return !Threads::GetThreadInfoCopy(ti, (ThreadIdentifier) 0);
}

class FBN_ThreadTestEngine: public ThreadTestEngine{
public:
	virtual ~FBN_ThreadTestEngine(){}
	FBN_ThreadTestEngine(int32 nOfThreads): ThreadTestEngine(nOfThreads){
	}
    virtual bool UserCheckFunction(){
    	bool failed = false;
    	for (int32 i = 0;i < nOfThreads;i++){
            ThreadInformation ti;
    		CCString name = tidN[i];
    		if (name.GetSize()>0){
                if (tids[i] != Threads::FindByName(name)) {
                	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"thread number ",i,"of name ",name, "cannot be found correctly by FindByName");
                    failed = true;
                }
    		}
    	}
        if (tids[0] != Threads::FindByName("Unknown")) {
        	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"FindByName(Unknown) should return the first thread");
            failed = true;
        }

        if (Threads::FindByName("") != InvalidThreadIdentifier){
        	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"FindByName() should fail");
            failed = true;
        }

    	return !failed;
    }
};


bool ThreadsTest::TestFindByName(int32 nOfThreads,
                                 CCString name,
                                 int32 position) {
	FBN_ThreadTestEngine tte(nOfThreads);
    tte.tidN[position] = name;
    return tte.StandardTest();
}
