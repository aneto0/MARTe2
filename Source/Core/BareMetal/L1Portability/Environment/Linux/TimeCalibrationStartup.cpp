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
#include <math.h>
#include <sys/time.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "TimeCalibration.h"
#include "CompositeErrorManagement.h"
#include "StartupManager.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
namespace TimeCalibration {

INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(TimeCalibration,("TimeCalibrated",emptyString),("HeapManager",emptyString))

ErrorManagement::ErrorType CalibrateFrequency(){
	ErrorManagement::ErrorType ret;

    FILE *f = NULL;
    const uint32 LINUX_CPUINFO_BUFFER_SIZE = 1023u;
    char8 buffer[LINUX_CPUINFO_BUFFER_SIZE + 1u];
    if (ret ) {
        memset(&buffer[0], 0, LINUX_CPUINFO_BUFFER_SIZE + 1u);

        f = fopen("/proc/cpuinfo", "r");
        ret.OSError = (f == NULL);
        REPORT_ERROR(ret, "fopen(/proc/cpuinfo) failed");
    }

    size_t size = LINUX_CPUINFO_BUFFER_SIZE;
    if (ret){
        size = fread(&buffer[0], size, static_cast<size_t>(1u), f);
        fclose(f);

        ret.fatalError  = (size == 0);
        REPORT_ERROR(ret, "fread(/proc/cpuinfo) returned 0");
    }

    CCString MHzS;
    if (ret){
        CCString bufferString(&buffer[0]);
        MHzS = bufferString.FindPatternString("MHz");
        ret.fatalError = (MHzS.GetSize()==0);
        COMPOSITE_REPORT_ERROR(ret, "cannot find MHz in cpuinfo string: ",bufferString);
    }

    float64 freqMHz = 0.0;
    if (ret){
    	MHzS = MHzS.FindPatternString(":");
    	MHzS++;
        freqMHz = strtof(MHzS.GetList(), static_cast<char8 **>(0));

        ret.fatalError = (freqMHz <= 0.);
        COMPOSITE_REPORT_ERROR(ret, "freqMHz is 0 or less ",freqMHz);

    }

    if (ret){
        float64 frequencyF = freqMHz * 1.0e6;
        period = 1.0 / frequencyF;
        frequency = static_cast<uint64>(frequencyF);
        COMPOSITE_REPORT_ERROR(ErrorManagement::Information, "frequency is ",frequencyF, "Hz period is ",(period*1.0e12), "ps");

    }

    osMinSleepUsec = 10;
    osSleepUsec    = 100;
    osSleepTicks   = frequency / (1000000 / osSleepUsec);

    return ret;
}

ErrorManagement::ErrorType TimeCalibrationStartup::Init(){

	ErrorManagement::ErrorType ret;

    initialTicks = HighResolutionTimer::Counter();
    frequency = 0u;
    period = 0.;

    struct timeval initTime;
    ret.OSError = gettimeofday(&initTime, static_cast<struct timezone *>(NULL));
    REPORT_ERROR(ret, "gettimeofday failed");

    initialSecs = initTime.tv_sec;
    initialUSecs = initTime.tv_usec;

    if (ret){
    	ret = CalibrateFrequency();
    }


	return ret;
}

ErrorManagement::ErrorType TimeCalibrationStartup::Finish(){
	ErrorManagement::ErrorType ret;

	return ret;
}


}
}
