/**
 * @file HighResolutionTimer.h
 * @brief Header file for module HighResolutionTimer
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

 * @details This header file contains the declaration of the module HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMER_ENV_H_
#define HIGHRESOLUTIONTIMER_ENV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "../../HighResolutionTimer.h"
#include "HighResolutionTimerCalibrator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe{


extern HighResolutionTimerCalibrator calibratedHighResolutionTimer;

namespace HighResolutionTimer {


inline int64 Frequency() {
    return calibratedHighResolutionTimer.GetFrequency();
}

inline float64 Period() {
    return calibratedHighResolutionTimer.GetPeriod();
}

inline float64 TicksToTime(const int64 tStop,
                           const int64 tStart) {
    int64 dT = tStop - tStart;
    return static_cast<float64>(dT) * Period();
}

inline bool GetTimeStamp(TimeStamp &date) {
    return calibratedHighResolutionTimer.GetTimeStamp(date);
}

}

}
#endif /* HIGHRESOLUTIONTIMER_ENV_H_ */
