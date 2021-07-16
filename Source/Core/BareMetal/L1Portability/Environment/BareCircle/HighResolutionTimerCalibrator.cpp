/**
 * @file HighResolutionTimer.cpp
 * @brief Source file for class HighResolutionTimer
 * @date 19/04/2021
 * @author Giuseppe Avon
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
#include <circle/timer.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "HighResolutionTimerCalibrator.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

        HighResolutionTimerCalibrator calibratedHighResolutionTimer;

        HighResolutionTimerCalibrator::HighResolutionTimerCalibrator() {
            isInstanceValid = true;
            frequency = CLOCKHZ;
            period = 1.0 / CLOCKHZ;
        }

        HighResolutionTimerCalibrator::~HighResolutionTimerCalibrator() {

        }

        uint64 HighResolutionTimerCalibrator::GetFrequency() {
            return frequency;
        }

        float64 HighResolutionTimerCalibrator::GetPeriod() {
            return period;
        }

        bool HighResolutionTimerCalibrator::IsInstanceValid() {
            return isInstanceValid;
        }

        uint64 HighResolutionTimerCalibrator::GetTicks() {
            return CTimer::GetClockTicks();
        }

}
