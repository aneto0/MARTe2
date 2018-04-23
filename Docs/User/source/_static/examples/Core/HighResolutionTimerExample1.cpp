/**
 * @file HighResolutionTimerExample1.cpp
 * @brief Source file for class HighResolutionTimerExample1
 * @date 23/04/2018
 * @author Andre Neto
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
 * the class HighResolutionTimerExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorLoggerExample.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


int main(int argc, char *argv[]) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    float64 sleepSeconds = 1.1;
    float64 countUntilF = sleepSeconds * HighResolutionTimer::Frequency();
    uint64 countUntil = static_cast<uint64>(countUntilF);

    uint64 countStart = HighResolutionTimer::Counter();
    uint64 countEnd = countStart + countUntil;
    while (HighResolutionTimer::Counter() < countEnd);
    float64 countDiff = static_cast<float64>(HighResolutionTimer::Counter() - countStart);
    float64 sleptSeconds = countDiff * HighResolutionTimer::Period();
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Was asked to wait for %e and waited for %e", sleepSeconds, sleptSeconds);

    return 0;
}



