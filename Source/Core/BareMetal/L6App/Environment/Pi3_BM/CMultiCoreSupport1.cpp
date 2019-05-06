/**
 * @file CMultiCoreSupport1.cpp
 * @brief Source file for class CMultiCoreSupport1
 * @date 05 mag 2019
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
 * the class CMultiCoreSupport1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <circle/actled.h>
#include <circle/logger.h>
#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/dwhcidevice.h>
#include <circle/sched/scheduler.h>
#include <circle/net/netsubsystem.h>
#include <circle/types.h>
#include <circle/multicore.h>
#include <circle/string.h>
#include <circle/startup.h>
#include <circle/gpioclock.h>
#include <circle/gpiopin.h>
#include <circle/pwmoutput.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CMultiCoreSupport1.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



unsigned CMultiCoreSupport1::sem = 0u;
static MARTe::ScheduledThread *CMultiCoreSupport1::threads = NULL;

static unsigned CMultiCoreSupport1::numberOfThreads = 0u;

CMultiCoreSupport1::CMultiCoreSupport1(CMemorySystem *pMemorySystem) :
        CMultiCoreSupport(pMemorySystem) {
}
CMultiCoreSupport1::~CMultiCoreSupport1() {

}

void CMultiCoreSupport1::SetScheduledThreads(MARTe::ScheduledThread *threadsIn,
                                             unsigned int &numberOfThreadsIn) {
    threads = threadsIn;
    numberOfThreads = numberOfThreadsIn;
}

void CMultiCoreSupport1::Run(unsigned nCore) {
    using namespace MARTe;
    CLogger::Get()->Write("ciao", LogDebug, "CPU core %u started", ThisCore());
    while (sem == 0u)
        ;
    uint64 lastCycleTimeStamp = 0ull;
    uint32 coreNumber = ThisCore();
    CLogger::Get()->Write("ciao", LogDebug, "nThreads=%d", numberOfThreads);

    if (coreNumber < numberOfThreads) {
        CLogger::Get()->Write("ciao", LogDebug, "nExec=%d", threads[coreNumber].numberOfExecutables);

        if (threads != NULL) {
            while (1) {

                bool ok = true;
                uint64 absTicks = HighResolutionTimer::Counter();
                for (uint32 i = 0u; (i < threads[coreNumber].numberOfExecutables) && (ok); i++) {
                    // save the time before
                    // execute the gam
                    ok = threads[coreNumber].executables[i]->Execute();
                    uint64 tmp = (HighResolutionTimer::Counter() - absTicks);
                    float64 ticksToTime = (static_cast<float64>(tmp) * HighResolutionTimer::Period()) * 1e6;
                    uint32 absTTime = static_cast<uint32>(ticksToTime);  //us
                    if (ok) {
                        uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
                        ok = MemoryOperationsHelper::Copy(threads[coreNumber].executables[i]->GetTimingSignalAddress(), &absTTime, sizeToCopy);
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to ExecuteSingleCycle() %d", i);

                    }
                }

                if (!ok) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to ExecuteSingleCycle().");

                }
                uint32 absTime = 0u;
                if (lastCycleTimeStamp != 0u) {
                    uint64 tmp = (HighResolutionTimer::Counter() - lastCycleTimeStamp);
                    float64 ticksToTime = (static_cast<float64>(tmp) * HighResolutionTimer::Period()) * 1e6;
                    absTime = static_cast<uint32>(ticksToTime);  //us
                }
                uint32 sizeToCopy = static_cast<uint32>(sizeof(uint32));
                if (!MemoryOperationsHelper::Copy(threads[nCore].cycleTime, &absTime, sizeToCopy)) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not copy cycle time information.");
                }
                lastCycleTimeStamp = HighResolutionTimer::Counter();
            }
        }
    }
}

	
