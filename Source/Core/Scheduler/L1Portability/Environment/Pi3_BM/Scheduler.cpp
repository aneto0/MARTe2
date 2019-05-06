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
#include <circle/sched/scheduler.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <assert.h>

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

static const char FromScheduler[] = "sched";

static Scheduler *Scheduler::p_this = 0;

Scheduler::Scheduler(void) {
    if (p_this == 0) {
        p_this = this;
    }
}

Scheduler::~Scheduler(void) {
}

void Scheduler::Yield(void) {
    CScheduler::Get()->Yield();
}

void Scheduler::Sleep(unsigned nSeconds) {
    CScheduler::Get()->Sleep(nSeconds);
}

void Scheduler::MsSleep(unsigned nMilliSeconds) {
    CScheduler::Get()->MsSleep(nMilliSeconds);

}

void Scheduler::usSleep(unsigned nMicroSeconds) {
    CScheduler::Get()->usSleep(nMicroSeconds);
}

Task *Scheduler::GetCurrentTask(void) {
    return CScheduler::Get()->GetCurrentTask();
}

void Scheduler::RegisterTaskSwitchHandler(SchedulerTaskHandler *pHandler) {
    CScheduler::Get()->RegisterTaskSwitchHandler(pHandler);
}

void Scheduler::RegisterTaskTerminationHandler(SchedulerTaskHandler *pHandler) {
    CScheduler::Get()->RegisterTaskTerminationHandler(pHandler);

}

unsigned int Scheduler::AddTask(Task *pTask) {
    assert(pTask != 0);

    unsigned i;
    for (i = 0; i < CScheduler::Get()->m_nTasks; i++) {
        if (CScheduler::Get()->m_pTask[i] == 0) {
            CScheduler::Get()->m_pTask[i] = pTask;

            return i;
        }
    }

    if (CScheduler::Get()->m_nTasks >= MAX_TASKS) {
        CLogger::Get()->Write(FromScheduler, LogPanic, "System limit of tasks exceeded");
    }

    CScheduler::Get()->m_pTask[CScheduler::Get()->m_nTasks++] = pTask;
    return (CScheduler::Get()->m_nTasks - 1);
}

void Scheduler::RemoveTask(Task *pTask) {
    CScheduler::Get()->RemoveTask(pTask);
}

void Scheduler::BlockTask(Task **ppTask) {
    CScheduler::Get()->BlockTask(ppTask);

}

void Scheduler::WakeTask(Task **ppTask) {
    CScheduler::Get()->WakeTask(ppTask);
}

unsigned Scheduler::GetNextTask(void) {
    CScheduler::Get()->GetNextTask();
}

Scheduler *Scheduler::Get(void) {
    return p_this;
}

bool Scheduler::IsAlive(unsigned int id) {
    bool ret = (id < MAX_TASKS);
    if (ret) {
        ret = (CScheduler::Get()->m_pTask[id] != 0);
    }
    return ret;
}

Task *Scheduler::GetTask(unsigned int id) {
    Task *ret = NULL;
    if(id < MAX_TASKS) {
        ret=CScheduler::Get()->m_pTask[id];
    }
    return ret;
}

unsigned int Scheduler::Id(Task* task) {
    for (unsigned i = 0; i < CScheduler::Get()->m_nTasks; i++) {
        if (CScheduler::Get()->m_pTask[i] == task) {
            return i;
        }
    }
    return 0xFFFFFFFF;
}

unsigned int Scheduler::NumberOfThreads() {
    return CScheduler::Get()->m_nTasks;
}

