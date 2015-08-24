/**
 * @file HighResolutionTimerOS.h
 * @brief Header file for class HighResolutionTimerOS
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

 * @details This header file contains the declaration of the class HighResolutionTimerOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMEROS_H_
#define HIGHRESOLUTIONTIMEROS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "HighResolutionTimerCalibratorOS.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

extern HighResolutionTimerCalibratorOS calibratedHighResolutionTimer;


namespace HighResolutionTimer {


inline int64 Frequency() {
    return calibratedHighResolutionTimer.GetFrequency();
}

inline float64 Period() {
    return calibratedHighResolutionTimer.GetPeriod();
}

inline float64 TicksToTime(int64 tStop,
                           int64 tStart) {
    int64 dT = tStop - tStart;
    return dT * Period();
}

inline bool GetTimeStamp(TimeValues &date) {
    return calibratedHighResolutionTimer.GetTimeStamp(date);
}

}

#endif /* HIGHRESOLUTIONTIMEROS_H_ */
