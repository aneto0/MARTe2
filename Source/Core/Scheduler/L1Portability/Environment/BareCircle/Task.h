/**
 * @file Task.h
 * @brief Header file for class Task
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

 * @details This header file contains the declaration of the class Task
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_PI3_BM_TASK_H_
#define SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_PI3_BM_TASK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <circle/sched/task.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ThreadInformation.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
class Task: public CTask {
public:
    Task(unsigned nStackSize = TASK_STACK_SIZE); // nStackSize = 0 for main task
    virtual ~Task(void);

    virtual void Run(void);

    void SetThreadInformation(ThreadInformation &threadInfoIn);
private:

    ThreadInformation *threadInfo;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_SCHEDULER_L1PORTABILITY_ENVIRONMENT_PI3_BM_TASK_H_ */

