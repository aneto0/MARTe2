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
#define TIMEOUTTYPE_H_

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

namespace MARTe {

/**
 * @brief Timeout type definition.
 *
 * @details These methods define the timeout object which is simply an integer which represent a time in milliseconds.
 * Furthermore here are defined flags for different types of timeout.
 * This class is used to implement semaphores with timed locks and in applications which needs
 * Timeouts.
 */
class TimeoutType {

public:

    /**
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     */
    inline TimeoutType(const uint32 &msecs = 0xFFFFFFFFu);

    /**
     * @brief Sets the timeout from float32.
     * @param[in] secs is the time in seconds.
     */
    /*lint -e(1960) , functions should have external effects*/
    inline void SetTimeoutSec(float64 secs);

    /**
     * @brief Sets the timeout from HighResolutionTimer ticks.
     * @param[in] ticks are the number of cpu ticks.
     */
    /*lint -e(1960) , functions should have external effects*/
    inline void SetTimeoutHighResolutionTimerTicks(uint64 ticks);

    /**
     * @brief Gets the timeout in HighResolutionTimer Ticks
     * @return the number of ticks related to the timeout.
     */
    inline uint64 HighResolutionTimerTicks() const;

    /**
     * @brief Subtracts a number of milliseconds from the timeout.
     * @param[in] mSecs is the value which will be subtracted to the timeout (milliseconds).
     * @return this object.
     */
    inline TimeoutType &operator-=(const uint32 &mSecs);

    /**
     * @brief Compares two timeout times.
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
     * @param[in] tt is the timeout to copy in this.
     * @return this object.
     */
    inline TimeoutType &operator=(const TimeoutType &tt);

    /**
     * @brief Check if the timeout is finite.
     * @return true if the timeout is less than the max timeout acceptable.
     */
    inline bool IsFinite() const;

    /**
     * @brief Returns the timeout value in milliseconds.
     * @return the timeout value in milliseconds.
     */
    inline uint32 GetTimeoutMSec() const;

private:

    /**
     * Timeout in milliseconds
     */
    uint32 msecTimeout;

};

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait(0x00000000u);

/** Infinite wait timeout */
const TimeoutType TTInfiniteWait(0xFFFFFFFFu);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected(0xFFFFFFFDu);

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTDefault(0xFFFFFFFEu);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TimeoutType::TimeoutType(const uint32 &msecs) {
    msecTimeout = msecs;
}

void TimeoutType::SetTimeoutSec(float64 secs) {
    secs *= 1000.0;
    msecTimeout = static_cast<uint32>(secs);
}

void TimeoutType::SetTimeoutHighResolutionTimerTicks(uint64 ticks) {
<<<<<<< HEAD
=======
    if (ticks < 0) {
        ticks = 0;
    }
>>>>>>> refs/remotes/origin/#306_Backport_GAMs
    float64 TimeoutSecFromTicks = (static_cast<float64>(ticks) * HighResolutionTimer::Period());
    float64 TimeoutMSecFromTicks = 1000.0 * TimeoutSecFromTicks;
    msecTimeout = static_cast<uint32>(TimeoutMSecFromTicks);
}

uint64 TimeoutType::HighResolutionTimerTicks() const {
    float64 deltaT = static_cast<float64>(msecTimeout);
    deltaT = deltaT * 1e-3;
    float64 frequency = static_cast<float64>(HighResolutionTimer::Frequency());
    deltaT = deltaT * frequency;
    uint64 ticks = static_cast<uint64>(deltaT);
    return ticks;
}

TimeoutType &TimeoutType::operator-=(const uint32 &mSecs) {
    if (msecTimeout > mSecs) {
        msecTimeout -= mSecs;
    }
    else {
        msecTimeout = 0u;
    }
    return *this;
}

bool TimeoutType::operator==(const TimeoutType &tt) const {
    return msecTimeout == tt.msecTimeout;
}

bool TimeoutType::operator!=(const TimeoutType &tt) const {
    return msecTimeout != tt.msecTimeout;
}

TimeoutType &TimeoutType::operator=(const TimeoutType &tt) {
    if (this != &tt) {
        msecTimeout = tt.msecTimeout;
    }
    return *this;
}

bool TimeoutType::IsFinite() const {
    return (msecTimeout < 0xFFFFFFFEu);
}

inline uint32 TimeoutType::GetTimeoutMSec() const {
    return msecTimeout;
}

}
#endif /* TIMEOUTTYPE_H_ */

