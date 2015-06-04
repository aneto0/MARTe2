/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Thread functions implementation in Linux using pthread library and signals. 
 */
#ifndef THREADS_OS_H
#define THREADS_OS_H 

//include stm sources
#include "cmsis_os.h"
#include "../../GeneralDefinitions.h"

#define __thread_decl 
/**
 * @brief Callback thread function as defined in Linux
 */
typedef void *(*StandardThreadFunction)(void *args);


osPriority ConvertToPriority(uint32 priorityLevel){
	//seven priority levels
	osPriority taskPriority;

	if(priorityLevel==0) taskPriority=osPriorityIdle;
	if(priorityLevel==1) taskPriority=osPriorityLow;
	if(priorityLevel==2) taskPriority=osPriorityBelowNormal;
	if(priorityLevel==3) taskPriority=osPriorityNormal;
	if(priorityLevel==4) taskPriority=osPriorityAboveNormal;
	if(priorityLevel==5) taskPriority=osPriorityHigh;
	if(priorityLevel>=6) taskPriority=osPriorityRealtime;

	return taskPriority;
}



void ThreadsOSEndThread() {
}

/**
 * @brief Not implemented in Linux
 * @param threadid is the thread identifier.
 * @return the thread state.
 */
uint32 ThreadsOSGetState(TID threadId) {
}

/** @see Threads::GetCPUs */
int32 ThreadsOSGetCPUs(TID threadId) {
	return 1;
}


TID ThreadsOSId() {
    return osThreadGetId();
}


/**
 * @see Threads::SetPriorityLevel.
 *
 * In linux the priority will vary between 33, i.e. priorityClass = IDLE_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_IDLE and 99, i.e. priorityClass = REAL_TIME_PRIORITY_CLASS
 * and priorityLevel = PRIORITY_TIME_CRITICAL
 */
void ThreadsOSSetPriorityLevel(TID threadId, uint32 priorityClass,
                               uint32 priorityLevel) {


	osPriority taskPriority=ConvertToPriority(priorityLevel);	
	osThreadSetPriority(threadId, taskPriority);
}

/** @see Threads::Kill
  *
  * A thread cannot be deleted if it locks a mutex semaphore. */
bool ThreadsOSKill(TID threadId) {
	return osThreadTerminate(threadId)==0;
}

/** @see Threads::IsAlive
  * 
  * A signal is used to know if the other thread is alive.  */
bool ThreadsOSIsAlive(TID threadId) {
	return True;
}

/** @see Threads::BeginThread */
TID ThreadsOSBeginThread(StandardThreadFunction function,
                         ThreadInformation *threadInfo, uint32 stacksize=configMINIMAL_STACK_SIZE,
                         ProcessorType runOnCPUs=0xf) {

	//the thread must have a name!
	os_pthread threadFunction=(os_pthread)function;
	osPriority threadPriority=ConvertToPriority(threadInfo->priorityLevel);
	uint32 instances=0;
	const osThreadDef_t threadDefinition={(char*)threadInfo->ThreadName(),threadFunction, threadPriority, instances , stacksize};
	
 	return osThreadCreate(&threadDefinition, threadInfo);
}
#endif

