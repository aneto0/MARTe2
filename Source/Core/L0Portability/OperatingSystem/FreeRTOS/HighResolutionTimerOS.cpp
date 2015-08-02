/**
 * @file HighResolutionTimerOS.cpp
 * @brief Source file for class HighResolutionTimerOS
 * @date 1 Aug 2015
 * @author andre
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
 * the class HighResolutionTimerOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "../../HighResolutionTimer.h"
#include "HighResolutionTimerCalibratorOS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static HighResolutionTimerCalibratorOS calibratedHighResolutionTimer;

int64 HighResolutionTimer::Frequency(){
    return calibratedHighResolutionTimer.GetFrequency();
}

float64 HighResolutionTimer::Period(){
    return calibratedHighResolutionTimer.GetPeriod();
}

float64 HighResolutionTimer::TicksToTime(int64 tStop, int64 tStart){
    int64 dT = tStop - tStart;
    return dT * Period();
}

bool HighResolutionTimer::GetTimeStamp(TimeValues &date){
    return calibratedHighResolutionTimer.GetTimeStamp(date);
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
