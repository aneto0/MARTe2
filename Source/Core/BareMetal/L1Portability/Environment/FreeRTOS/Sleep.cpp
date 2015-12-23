/**
 * @file SleepOS.cpp
 * @brief Source file for class SleepOS
 * @date 31/07/2015
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
 * the class SleepOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "FreeRTOS.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../HighResolutionTimer.h"
#include "../../Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
extern "C" {

void HAL_Delay(volatile uint32_t Delay);
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Sleep {

void AtLeast(float64 sec) {
#ifdef USE_FREERTOS
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        vTaskDelay((msec * configTICK_RATE_HZ) / 1000L);
    }
#else
    HAL_Delay(sec * 1000);
#endif
}

void NoMore(float64 sec) {
    int64 secCounts = static_cast<int64>(sec) * HighResolutionTimer::Frequency();

    int64 start = HighResolutionTimer::Counter();
    if (sec > 0.) {
#ifdef USE_FREERTOS
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
            vTaskDelay(sec * 1000 / portTICK_PERIOD_MS);
        }
#else
        HAL_Delay(sec * 1000);
#endif
    }

    int64 sleepUntil = secCounts + start;
    while (HighResolutionTimer::Counter() < sleepUntil) {
    }

}

void Sec(float64 sec) {
#ifdef USE_FREERTOS
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        vTaskDelay(sec * configTICK_RATE_HZ);

    }
#else
    HAL_Delay(sec * 1000);
#endif
}

void MSec(int32 msec) {
#ifdef USE_FREERTOS
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        vTaskDelay((msec * configTICK_RATE_HZ) / 1000L);
    }
#else
    HAL_Delay(msec);
#endif

}

void SemiBusy(float64 totalSleepSec,
              float64 nonBusySleepSec) {
    int64 startCounter = HighResolutionTimer::Counter();
    float64 endCounterF = totalSleepSec * static_cast<float64>(HighResolutionTimer::Frequency());
    int64 sleepUntilCounter = startCounter + static_cast<int64>(endCounterF);

    if ((nonBusySleepSec < totalSleepSec) && (nonBusySleepSec > 0.0)) {
#ifdef USE_FREERTOS
        if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
            vTaskDelay(nonBusySleepSec * 1000 / portTICK_PERIOD_MS);
        }
#else
        HAL_Delay(nonBusySleepSec * 1000);
#endif

    }
    while (HighResolutionTimer::Counter() < sleepUntilCounter) {
    }
}

int32 GetDateSeconds() {
    //TODO
    return 0;
}

}

}
