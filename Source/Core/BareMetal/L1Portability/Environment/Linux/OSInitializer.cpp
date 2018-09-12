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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

//#include "HighResolutionTimerCalibrator.h"
//#include "HighResolutionTimer.h"
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

    const uint64 LINUX_CPUINFO_BUFFER_SIZE = 1023u;
    initialTicks = HighResolutionTimer::Counter();
    frequency = 0u;
    period = 0.;

    struct timeval initTime;
    int32 ret = gettimeofday(&initTime, static_cast<struct timezone *>(NULL));

    initialSecs = initTime.tv_sec;
    initialUSecs = initTime.tv_usec;

    if (ret == 0) {
        char8 buffer[LINUX_CPUINFO_BUFFER_SIZE + 1u];
        memset(&buffer[0], 0, LINUX_CPUINFO_BUFFER_SIZE + 1u);

        FILE *f = fopen("/proc/cpuinfo", "r");
        size_t size = LINUX_CPUINFO_BUFFER_SIZE;
        if (f != NULL) {
            size = fread(&buffer[0], size, static_cast<size_t>(1u), f);
            fclose(f);
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "HighResolutionTimerCalibrator: fopen()");
        }
 
        if (size > 0u) {
            CCString pattern("MHz");
            CCString bufferString(&buffer[0]);
            CCString p = StringHelper::SearchString(bufferString, pattern);
            if (!p.IsNullPtr()) {
                p = StringHelper::SearchString(p, ":");
                p.Skip(); // p++;
                float64 freqMHz = strtof(p.GetList(), static_cast<char8 **>(0));
                if (freqMHz > 0.) {
                    float64 frequencyF = freqMHz *= 1.0e6;
                    period = 1.0 / frequencyF;
                    frequency = static_cast<uint64>(frequencyF);
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::OSError, "HighResolutionTimerCalibrator: fread()");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::OSError, "HighResolutionTimerCalibrator: gettimeofday()");
    }

    osMinSleepUsec = 10;
    osSleepUsec    = 100;
    osSleepTicks   = frequency / (1000000 / osSleepUsec);
}



}
