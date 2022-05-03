/**
 * @file HighResolutionTimer.h
 * @brief Source file for class HighResolutionTimer
 * @date 26/08/2021
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

#ifndef HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_
#define HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "xtime_l.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    class HighResolutionTimerCalibrator {
        public:
            /**
            * @brief Unlocks the timer and initially computes the timer frequency and period.
            */
            HighResolutionTimerCalibrator();

            /**
            * @brief Destructs the calibrator and eventually unlocks the timer.
            */
            ~HighResolutionTimerCalibrator();

            /**
            * @brief Returns underlying timer frequency
            */
            uint64 GetFrequency();

            /**
            * @brief Returns underlying timer period, as inverse of the frequency
            */
            float64 GetPeriod();

            /**
            * @brief Tells whether the timer instance is valid
            */
            bool IsInstanceValid();

            /**
            * @brief Calls the underlying GetClockTicks, which returns a fine grained consecutive tick counter in us
            */
            uint64 GetTicks();

        private:
            /**
            * @brief Timer calibrator instance validity
            */
            bool isInstanceValid;

            /**
            * @brief Holds the timer frequency, which is read at construction stage
            */
            uint64 frequency;

            /**
            * @brief Holds the timer period, which is computed as frequency inverse and thus computed at construction stage
            */
            float64 period;
    };

}

#endif /* HIGHRESOLUTIONTIMERCALIBRATOR_ENV_H_ */