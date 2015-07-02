/**
 * @file Threads.cpp
 * @brief Source file for class Threads
 * @date 17/06/2015
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
 * the class Threads (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Threads.h"
#include "ThreadsDatabase.h"

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,ThreadsOS.h)

TID ThreadsBeginThread(ThreadFunctionType function, void *parameters,
        uint32 stacksize, const char *name,
        uint32 exceptionHandlerBehaviour,
        ProcessorType runOnCPUs) {

    return ThreadsOS::BeginThread(
            function, parameters,
            stacksize, name, exceptionHandlerBehaviour, runOnCPUs);

}

TID ThreadsId() {
    return ThreadsOS::Id();
}

bool ThreadsIsAlive(TID threadId) {

    return ThreadsOS::IsAlive(threadId);

}

bool ThreadsKill(TID threadId) {

    return ThreadsOS::Kill(threadId);
}

void ThreadsSetPriorityLevel(TID threadId,
                             ThreadPriorityType priorityLevel) {

    ThreadsOS::SetPriorityLevel(threadId, PRIORITY_CLASS_UNKNOWN, priorityLevel);

}

void ThreadsSetPriorityClass(TID threadId,
                             PriorityClassType priorityClass) {

    ThreadsOS::SetPriorityLevel(threadId, priorityClass, PRIORITY_UNKNOWN);

}

int32 ThreadsGetCPUs(TID threadId) {
    return ThreadsOS::GetCPUs(threadId);
}

ThreadPriorityType ThreadsGetPriorityLevel(TID threadId) {
    return ThreadsOS::GetPriorityLevel(threadId);
}

PriorityClassType ThreadsGetPriorityClass(TID threadId) {
    return ThreadsOS::GetPriorityClass(threadId);
}

ThreadStateType ThreadsGetState(TID threadId) {
    return ThreadsOS::GetState(threadId);
}

void ThreadsEndThread() {

    ThreadsOS::EndThread();
}

const char *ThreadsName(TID threadId) {
    return ThreadsOS::Name(threadId);
}

bool ThreadProtectedExecute(ThreadFunctionType userFunction,
                            void *userData,
                            ExceptionHandler *ehi){
    return ThreadsOS::ProtectedExecute(userFunction, userData, ehi);
}

TID ThreadsFindByIndex(uint32 n) {
    return ThreadsOS::FindByIndex(n);
}

uint32 ThreadsNumberOfThreads() {
    return ThreadsOS::NumberOfThreads();
}

bool ThreadsGetThreadInfoCopy(ThreadInformation &copy,
                              int32 n,
                              TID tid) {
    return ThreadsOS::GetThreadInfoCopy(copy, n, tid);
}

TID ThreadsFindByName(const char* name) {
    return ThreadsOS::FindByName(name);
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void Threads::SetPriorityLevel(TID tid,
                               ThreadPriorityType level) {
    ThreadsSetPriorityLevel(tid, level);
}

void Threads::SetPriorityClass(TID tid,
                               PriorityClassType priorityClass) {
    ThreadsSetPriorityClass(tid, priorityClass);
}

void Threads::EndThread() {
    ThreadsEndThread();
}

TID Threads::BeginThread(ThreadFunctionType function,
                         void *parameters,
                         uint32 stacksize,
                         const char *name,
                         uint32 exceptionHandlerBehaviour,
                         ProcessorType runOnCPUs) {
    return ThreadsBeginThread(function, parameters, stacksize, name, exceptionHandlerBehaviour, runOnCPUs);
}

TID Threads::Id() {
    return ThreadsId();
}

bool Threads::Kill(TID tid) {
    return ThreadsKill(tid);
}

bool Threads::IsAlive(TID tid) {
    return ThreadsIsAlive(tid);
}

const char *Threads::Name(TID tid) {
    return ThreadsName(tid);
}

ThreadStateType Threads::GetState(TID tid) {
    return ThreadsGetState(tid);
}

ThreadPriorityType Threads::GetPriorityLevel(TID tid) {
    return ThreadsGetPriorityLevel(tid);
}

PriorityClassType Threads::GetPriorityClass(TID tid) {
    return ThreadsGetPriorityClass(tid);
}

int32 Threads::GetCPUs(TID tid) {
    return ThreadsGetCPUs(tid);
}

bool Threads::ProtectedExecute(ThreadFunctionType userFunction,
                            void *userData,
                            ExceptionHandler *ehi){

    return ThreadProtectedExecute(userFunction, userData, ehi);

}
TID Threads::FindByIndex(uint32 n) {
    return ThreadsFindByIndex(n);
}

uint32 Threads::NumberOfThreads() {
    return ThreadsNumberOfThreads();
}

bool Threads::GetThreadInfoCopy(ThreadInformation &copy,
                                int32 n,
                                TID tid) {
    return ThreadsGetThreadInfoCopy(copy, n, tid);

}

TID Threads::FindByName(const char* name) {
    return ThreadsFindByName(name);
}

