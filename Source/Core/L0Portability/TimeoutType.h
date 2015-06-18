/**
 * @file TimeoutType.h
 * @brief Header file for class TimeoutType
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class TimeoutType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
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
    /** how many milliseconds to wait */
    uint32 msecTimeout;

    /**
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     */
    inline TimeoutType(uint32 msecs = (uint32) 0xFFFFFFFF);


    /**
     * @brief Set timeout from float.
     * @param[in] secs is the time in seconds.
     */
    inline void SetTimeOutSec(double secs);

    /**
     * @brief Set timeout from HRT ticks.
     * @param[in] ticks are the number of cpu ticks.
     */
    inline void SetTimeOutHighResolutionTimerTicks(int64 ticks);

    /**
     * @brief Get the timeout in HighResolutionTimer Ticks
     * @return the number of ticks related to the timeout.
     */
    inline int64 HighResolutionTimerTicks() const;

    /**
     * @brief Subtract n to the timeout.
     * @param[in] n is the value which must be subtracted to the timeout (milliseconds).
     */
    inline void operator-=(uint32 n);

    /**
     * @brief Compare two timeout times.
     * @param[in] tt is the timeout object to be compared with this.
     * @return true of msecTimeout attributes of both timeout object are equal.
     */
    inline bool operator==(const TimeoutType &tt) const;

    /**
     * @brief Check if two timeout are different.
     * @param[in] tt is another timeout.
     * @return true if msecTimeout attributes of both timeout object are different.
     */
    inline bool operator!=(const TimeoutType &tt) const;

    /**
     * @brief Copy operator.
     * @param[in] tt is the timout to copy in this.
     */
    inline void operator=(const TimeoutType &tt);

    /**
     * @brief Check if the timeout is finite.
     * @return true if the timeout is less than the max timeout acceptable.
     */
    inline bool IsFinite() const;

};

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait((uint32) 0x00000000);

/** Infinite wait Timeout */
const TimeoutType TTInfiniteWait((uint32) 0xFFFFFFFF);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected((uint32) 0xFFFFFFFD);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTDefault((uint32) 0xFFFFFFFE);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
TimeoutType::TimeoutType(uint32 msecs) {
    msecTimeout = msecs;
}

void TimeoutType::SetTimeOutSec(double secs) {
    msecTimeout = (uint32) (secs * 1000.0);
}

void TimeoutType::SetTimeOutHighResolutionTimerTicks(int64 ticks) {
    if (ticks < 0) {
        ticks = 0;
    }
    double msDT = 1000.0 * (ticks * HighResolutionTimer::Period());
    msecTimeout = (uint32) msDT;
}

int64 TimeoutType::HighResolutionTimerTicks() const {
    double dT = msecTimeout;
    dT = dT * 1e-3;
    double freq = HighResolutionTimer::Frequency();
    dT = dT * freq;
    int64 ticks = (int64) dT;
    return ticks;
}

void TimeoutType::operator-=(uint32 n) {
    if (msecTimeout > n)
        msecTimeout -= n;
    else
        msecTimeout = 0;
}

bool TimeoutType::operator==(const TimeoutType &tt) const {
    return msecTimeout == tt.msecTimeout;
}

bool TimeoutType::operator!=(const TimeoutType &tt) const {
    return msecTimeout != tt.msecTimeout;
}

void TimeoutType::operator=(const TimeoutType &tt) {
    msecTimeout = tt.msecTimeout;
}

bool TimeoutType::IsFinite() const {
    return (msecTimeout < (uint32) 0xFFFFFFFE);
}

#endif /* TIMEOUTTYPE_H_ */

