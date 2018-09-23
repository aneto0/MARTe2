/**
 * @file HighResolutionTimer.cpp
 * @brief Source file for class HighResolutionTimer
 * @date 05/07/2015
 * @author André Neto
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
 * the class HighResolutionTimerCalibrator (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include <sys/time.h>
#include <math.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

//#include "HighResolutionTimerCalibrator.h"
#include "HighResolutionTimer.h"
#include "StringHelper.h"
#include "../../OSInitializer.h"
#include "../../ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

OSInitializer::OSInitializer(){
    struct timeval initialTime;
    time((time_t *) &initialTime.tv_sec);

    //The precision is at the millisecond!
    SYSTEMTIME forMs;
    GetSystemTime(&forMs);
    initialSecs = initialTime.tv_sec;
    initialUSecs = forMs.wMilliseconds * 1000;

    initialTicks = HighResolutionTimer::Counter();

    LARGE_INTEGER tt0, tt1, tt2, tt3, tt4, tt5;
    uint64 dTa, dTb;
    dTa = 0;
    dTb = 0;
    for (int i = 0; i < 50; i++) {
        tt2.QuadPart = HighResolutionTimer::Counter();
        QueryPerformanceCounter(&tt0);
        tt3.QuadPart = HighResolutionTimer::Counter();
        tt4 = tt3;
        while ((tt4.QuadPart - tt3.QuadPart) < 100000)
            tt4.QuadPart = HighResolutionTimer::Counter(); // .5 ms at 200 Mhz

        QueryPerformanceCounter(&tt1);
        tt5.QuadPart = HighResolutionTimer::Counter();
        dTa += (tt1.QuadPart - tt0.QuadPart);
        dTb += ((tt5.QuadPart + tt4.QuadPart) - (tt3.QuadPart + tt2.QuadPart)) / 2;
    }
    QueryPerformanceFrequency(&tt0);
    frequency = tt0.QuadPart;
    frequency *= dTb;
    frequency /= dTa;

    frequency += 999999;
    frequency /= 2000000;
    frequency *= 2000000;

    period = 1.0 / frequency;

    //
    // Assumes a granularity of 10ms
    //
    osMinSleepUsec = 1000;
    osSleepUsec    = 10000;
    osSleepTicks   = frequency / (1000000 / osSleepUsec);

}


uint64 OSInitializer::frequency ;

/**
 * Time between two ticks in seconds
 */
float64 OSInitializer::period;

/**
 * Stores the seconds (counting from the epoch) at which a framework instance was executed.
 */
oslong OSInitializer::initialSecs;

/**
 * Stores the microseconds (counting from the epoch) at which a framework instance was executed.
 */
oslong OSInitializer::initialUSecs;

/**
 * Number of elapsed ticks at the time at which a framework instance was executed.
 */
uint64 OSInitializer::initialTicks;

/**
 * typical worst case number of ticks the OS will consume during a sleep if CPU not used.
 */
uint64 OSInitializer::osSleepTicks;

/**
 * typical worst case number of usec the OS will consume during a sleep if CPU not used.
 */
uint32 OSInitializer::osSleepUsec;

/**
 * minimum value o be used in a sleep call to guarantee some sleep is actually performed.
 */
uint32 OSInitializer::osMinSleepUsec;



}
