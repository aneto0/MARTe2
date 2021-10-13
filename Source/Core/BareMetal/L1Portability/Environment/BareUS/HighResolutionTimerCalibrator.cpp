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
#include "xparameters.h"
#include "xtime_l.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"
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
            
            /* COUNTS_PER_SECOND is defined in the xtime_l.h */
            frequency = COUNTS_PER_SECOND;
            period = 1.0 / COUNTS_PER_SECOND;

            XTime tempTicks = 0ULL;

            /* The timer is initialized only if it is disabled.
             * If the timer is already running, this function does not perform any operation.
             * See ug1137-zynq-ultrascale-mpsoc-swdev.pdf 
             * Zynq UltraScale+ MPSoC: Software Developers Guide
             * Chapter Cortex A53 64-bit Mode Time Functions
             */
            XTime_StartTimer();
            XTime_GetTime(&tempTicks);

            if(tempTicks != 0) {
                isInstanceValid = true;
            }
            else {
                isInstanceValid = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "HighResolutionTimerCalibrator: failure in XTime_GetTime()");
            }
        }

        HighResolutionTimerCalibrator::~HighResolutionTimerCalibrator() {
            isInstanceValid = false;            
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
            XTime tempTime = 0ULL;
            XTime_GetTime(&tempTime);

            return static_cast<uint64>(tempTime);
        }

}
