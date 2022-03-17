/**
 * @file Scheduler.h
 * @brief Header file for class Scheduler
 * @date 24/04/2019
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

 * @details This header file contains the declaration of the class Scheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_US_SCHEDULER_H_
#define SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_US_SCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Task.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

typedef void SchedulerTaskHandler(Task *pTask);

class Scheduler // simple cooperative (non-preemtive) scheduler
{
public:

    Scheduler(void);
    ~Scheduler(void);


    void Yield(void); // switch to next task

    void Sleep(unsigned nSeconds);
    void MsSleep(unsigned nMilliSeconds);
    void usSleep(unsigned nMicroSeconds);

    Task *GetTask(unsigned int id);

    Task *GetCurrentTask(void);

    static Scheduler *Get(void);

    bool IsAlive(unsigned int id);

    unsigned int Id(Task* task);

    void RegisterTaskSwitchHandler (SchedulerTaskHandler *pHandler);
    void RegisterTaskTerminationHandler (SchedulerTaskHandler *pHandler);

    unsigned int AddTask(Task *pTask);

    void BlockTask(Task **ppTask);
    void WakeTask(Task **ppTask); // can be called from interrupt context
    friend class CSynchronizationEvent;

    void RemoveTask(Task *pTask);
    unsigned GetNextTask(void); // returns index into m_pTask or MAX_TASKS if no task was found

    unsigned int NumberOfThreads();

private:

    static Scheduler *p_this;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_US_SCHEDULER_H_ */

