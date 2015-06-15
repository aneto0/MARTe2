/**
 * @file TimeoutType.h
 * @brief Header file for class TimeoutType
 * @date 15/06/2015
 * @author Giuseppe Ferrò
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
 *
 * @details This header file contains the declaration of the class TimeoutType
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

#ifndef TIMEOUTTYPE_H_
#define 		TIMEOUTTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/** max value for the delay that is treated  */
const uint32 TTMaxDelay = 0xFFFF0000;

/** type to indicate specific wait in the timeout parameters */

/**
 * @brief Functions for timeout definition.
 *
 * @details These methods define the timeout object which is simply an integer which represent a time in milliseconds.
 * Furthermore here are defined flags for different types of timeout.
 *
 * @details This class is used to implement semaphores based on spinlocks with timed locks and in applications which needs
 * timeouts.
 */
class TimeoutType {
public:
    /** how many msecs to wait */
    uint32 msecTimeout;
public:
    /**
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     */
    TimeoutType(uint32 msecs = (uint32) 0xFFFFFFFF) {
        msecTimeout = msecs;
    }

    /**
     * @brief Set timeout from float.
     * @param[in] secs is the time in seconds.
     */
    void SetTimeOutSec(double secs) {
        msecTimeout = (uint32) (secs * 1000.0);
    }

    /**
     * @brief Set timeout from HRT ticks.
     * @param[in] ticks are the number of cpu ticks.
     */
    void SetTimeOutHighResolutionTimerTicks(int64 ticks) {
        if (ticks < 0) {
            ticks = 0;
        }
        double msDT = 1000.0 * (ticks * HighResolutionTimer::Period());
        msecTimeout = (uint32) msDT;
    }

    /**
     * @brief Get the timeout in HighResolutionTimer Ticks
     * @return the number of ticks related to the timeout.
     */
    int64 HighResolutionTimerTicks() const {
        double dT = msecTimeout;
        dT = dT * 1e-3;
        double freq = HighResolutionTimer::Frequency();
        dT = dT * freq;
        int64 ticks = (int64) dT;
        return ticks;
    }

    /**
     * @brief Subtract n to the timeout.
     * @param[in] n is the value which must be subtracted to the timeout (milliseconds).
     */
    void operator-=(uint32 n) {
        if (msecTimeout > n)
            msecTimeout -= n;
        else
            msecTimeout = 0;
    }

    /**
     * @brief Compare two timeout times.
     * @param[in] tt is the timeout object to be compared with this.
     * @return true of msecTimeout attributes of both timeout object are equal.
     */
    bool operator==(const TimeoutType &tt) const {
        return msecTimeout == tt.msecTimeout;
    }

    /**
     * @brief Check if two timeout are different.
     * @param[in] tt is another timeout.
     * @return true if msecTimeout attributes of both timeout object are different.
     */
    bool operator!=(const TimeoutType &tt) const {
        return msecTimeout != tt.msecTimeout;
    }

    /**
     * @brief Copy operator.
     * @param[in] tt is the timout to copy in this.
     */
    void operator=(const TimeoutType &tt) {
        msecTimeout = tt.msecTimeout;
    }

    /**
     * @brief Check if the timeout is finite.
     * @return true if the timeout is less than the max timeout acceptable.
     */
    bool IsFinite() const {
        return (msecTimeout < (uint32) 0xFFFFFFFE);
    }

};

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait((uint32) 0x00000000);

/** Infinite wait Timeout */
const TimeoutType TTInfiniteWait((uint32) 0xFFFFFFFF);

/** Used in semafore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected((uint32) 0xFFFFFFFD);

/** Used in semafore protected codes to specify to bypass the check! */
const TimeoutType TTDefault((uint32) 0xFFFFFFFE);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMEOUTTYPE_H_ */

