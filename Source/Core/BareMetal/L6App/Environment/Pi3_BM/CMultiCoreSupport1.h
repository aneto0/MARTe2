/**
 * @file CMultiCoreSupport1.h
 * @brief Header file for class CMultiCoreSupport1
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

 * @details This header file contains the declaration of the class CMultiCoreSupport1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PI3_CIRCLE_STARTUP_CMULTICORESUPPORT1_H_
#define 		PI3_CIRCLE_STARTUP_CMULTICORESUPPORT1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSchedulerI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class CMultiCoreSupport1: public CMultiCoreSupport {
public:

    CMultiCoreSupport1(CMemorySystem *pMemorySystem);

    virtual ~CMultiCoreSupport1();

    virtual void Run(unsigned nCore);

    static void SetScheduledThreads(MARTe::ScheduledThread *threadsIn, unsigned int &numberOfThreadsIn);

    static unsigned sem;

    static MARTe::ScheduledThread *threads;

    static unsigned numberOfThreads;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PI3_CIRCLE_STARTUP_CMULTICORESUPPORT1_H_ */

