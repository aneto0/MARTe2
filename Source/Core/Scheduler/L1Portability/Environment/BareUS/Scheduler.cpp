/**
 * @file Scheduler.cpp
 * @brief Source file for class Scheduler
 * @date 24 apr 2019
 * @author pc
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
 * the class Scheduler (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Scheduler.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Scheduler::Scheduler(void) {
}

Scheduler::~Scheduler(void) {
}

void Scheduler::Yield(void) {
}

void Scheduler::Sleep(unsigned nSeconds) {
}

void Scheduler::MsSleep(unsigned nMilliSeconds) {
}

void Scheduler::usSleep(unsigned nMicroSeconds) {
}

Task *Scheduler::GetCurrentTask(void) {
    return NULL;
}

void Scheduler::RegisterTaskSwitchHandler(SchedulerTaskHandler *pHandler) {
}

void Scheduler::RegisterTaskTerminationHandler(SchedulerTaskHandler *pHandler) {
}

unsigned int Scheduler::AddTask(Task *pTask) {
    return 0;
}

void Scheduler::RemoveTask(Task *pTask) {
}

void Scheduler::BlockTask(Task **ppTask) {
}

void Scheduler::WakeTask(Task **ppTask) {
}

unsigned Scheduler::GetNextTask(void) {
    return 0;
}

Scheduler *Scheduler::Get(void) {
    return NULL;
}

bool Scheduler::IsAlive(unsigned int id) {
    return false;
}

Task *Scheduler::GetTask(unsigned int id) {
    Task *ret = NULL;
    return ret;
}

unsigned int Scheduler::Id(Task* task) {
    return 0;
}

unsigned int Scheduler::NumberOfThreads() {
    return 1;
}