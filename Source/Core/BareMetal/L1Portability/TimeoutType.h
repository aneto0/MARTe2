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
 * @details These methods define the timeout object which are simply two integer which represent time in milliseconds (uint32) and microseconds (uint64). Both time integers are equivalent (both represents the same
 * timeout value but with different physical units) but microseconds representation has more precision.
 *
 * Since the maximum value that can be represented with the two integers are different, the timeout infinite is based on the milliseconds representation because is the smallest range. Therefore, IsFinite() uses
 * msecTimeout.
 *
 * Furthermore here are defined flags for different types of timeout.
 * This class is used to implement semaphores with timed locks and in applications which needs
 * Timeouts.
 */
class TimeoutType {

public:

//    /**
//     * @brief Default Constructor
//     */
//    inline TimeoutType();

    /**
     * @deprecated - Use TimeoutType(const uint64 &usecs)
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     * @details Added to keep compatibility with the current objects (most of them initialise TimeoutType(int32 timeoutValue) which now conflicts with TimeoutType(const uint64 &usecs))
     */
    inline TimeoutType(const int32 &msecs);

    /**
     * @brief Constructor from integer.
     * @param[in] msecs is the time in milliseconds.
     */
    inline TimeoutType(const uint32 &msecs = 0xFFFFFFFFu);

    /**
     * @brief Constructor from integer.
     * @param[in] usecs is the time in microseconds.
     */
    inline TimeoutType(const uint64 &usecs);

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
    inline void SetTimeoutHighResolutionTimerTicks(const uint64 ticks);

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
    inline TimeoutType& operator-=(const uint32 &mSecs);

    /**
     * @brief Subtracts a number of microseconds from the timeout.
     * @param[in] mSecs is the value which will be subtracted to the timeout (microseconds).
     * @return this object.
     */
    inline TimeoutType& operator-=(const uint64 &uSecs);

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
    inline TimeoutType& operator=(const TimeoutType &tt);

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
    /**
     * @brief Returns the timeout value in microseconds.
     * @return the timeout value in microseconds.
     */
    inline uint64 GetTimeoutUSec() const;

private:

    /**
     * Timeout in milliseconds
     */
    uint32 msecTimeout;
    /**
     * Timeout in microseconds. usecTimeout = 1000*msecTimeout
     */
    uint64 usecTimeout;
//lint -e{1739} Binary operator 'MARTe::TimeoutType::operator==(const MARTe::TimeoutType &) const' should be non-member function (also !=). Justification operation is no symmetric,
//but incorrect usage will not compile. i.e TimeoutType tot(100u); bool ok = (tot == 100u); will return true. bool ok = (100u == tot); will not compile.
};

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait(static_cast<uint32>(0x00000000));

/** Infinite wait timeout */
const TimeoutType TTInfiniteWait(static_cast<uint32>(0xFFFFFFFFu));

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected(static_cast<uint32>(0xFFFFFFFDu));

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTDefault(static_cast<uint32>(0xFFFFFFFEu));

/** Do not wait (or wait indefinitely if blocking is set */
const TimeoutType TTNoWait64(static_cast<uint64>(0x0000000000000000ULL));

/** Infinite wait timeout */
const TimeoutType TTInfiniteWait64(static_cast<uint64>(0xFFFFFFFFFFFFFFFFULL));

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTUnProtected64(static_cast<uint64>(0xFFFFFFFFFFFFFFFDULL));

/** Used in semaphore protected codes to specify to bypass the check! */
const TimeoutType TTDefault64(static_cast<uint64>(0xFFFFFFFFFFFFFFFEULL));

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

//TimeoutType::TimeoutType() {
//    msecTimeout = 0xFFFFFFFFu;
//    usecTimeout = static_cast<uint64>(msecTimeout)*1000ULL;
//}
TimeoutType::TimeoutType(const int32 &msecs) {
    if (msecs < 0) {
        msecTimeout = 0u;
        usecTimeout = 0u;
    }
    else {
        msecTimeout = static_cast<uint32>(msecs);
        usecTimeout = static_cast<uint64>(msecTimeout) * 1000u;
    }
}
TimeoutType::TimeoutType(const uint32 &msecs) {
    msecTimeout = msecs;
    usecTimeout = static_cast<uint64>(msecTimeout) * 1000u;
}

TimeoutType::TimeoutType(const uint64 &usecs) {
    usecTimeout = usecs;
    if ((usecTimeout / 1000ULL) > 0xFFFFFFFFULL) {
        msecTimeout = 0xFFFFFFFFu;
    }
    else {
        msecTimeout = static_cast<uint32>(usecTimeout / 1000u);
    }
}

void TimeoutType::SetTimeoutSec(float64 secs) {
    secs *= 1000.0;
    msecTimeout = static_cast<uint32>(secs);
    secs *= 1000.0;
    usecTimeout = static_cast<uint64>(secs);
}

void TimeoutType::SetTimeoutHighResolutionTimerTicks(const uint64 ticks) {
    float64 TimeoutSecFromTicks = (static_cast<float64>(ticks) * HighResolutionTimer::Period());
    float64 TimeoutMSecFromTicks = 1000.0 * TimeoutSecFromTicks;
    msecTimeout = static_cast<uint32>(TimeoutMSecFromTicks);
    float64 auxResult = 1000000.0 * TimeoutSecFromTicks;
    usecTimeout = static_cast<uint64>(auxResult);
}

uint64 TimeoutType::HighResolutionTimerTicks() const {
    float64 deltaT = static_cast<float64>(usecTimeout);
    deltaT = deltaT * 1e-6;
    float64 frequency = static_cast<float64>(HighResolutionTimer::Frequency());
    deltaT = deltaT * frequency;
    uint64 ticks = static_cast<uint64>(deltaT);
    return ticks;
}

TimeoutType& TimeoutType::operator-=(const uint32 &mSecs) {
    uint64 auxuSecs = static_cast<uint64>(mSecs) * 1000ULL;
    if (usecTimeout > auxuSecs) {
        usecTimeout -= auxuSecs;
        msecTimeout = static_cast<uint32>(usecTimeout / 1000u);
    }
    else {
        msecTimeout = 0u;
        usecTimeout = 0u;
    }
    return *this;
}

TimeoutType& TimeoutType::operator-=(const uint64 &uSecs) {
    if (usecTimeout > uSecs) {
        usecTimeout -= uSecs;
        if ((usecTimeout / 1000ULL) > 0xFFFFFFFFULL) {
            msecTimeout = 0xFFFFFFFFu;
        }
        else {
            msecTimeout = static_cast<uint32>(usecTimeout / 1000u);
        }
    }
    else {
        msecTimeout = 0u;
        usecTimeout = 0u;
    }
    return *this;
}

bool TimeoutType::operator==(const TimeoutType &tt) const {
    //return usecTimeout == tt.usecTimeout;
    return usecTimeout == tt.usecTimeout;
}

bool TimeoutType::operator!=(const TimeoutType &tt) const {
//    return usecTimeout != tt.usecTimeout;
    return usecTimeout != tt.usecTimeout;
}

TimeoutType& TimeoutType::operator=(const TimeoutType &tt) {
    if (this != &tt) {
        msecTimeout = tt.msecTimeout;
        usecTimeout = tt.usecTimeout;
    }
    return *this;
}

bool TimeoutType::IsFinite() const {
    return (msecTimeout < 0xFFFFFFFEu);
}

inline uint32 TimeoutType::GetTimeoutMSec() const {
    return msecTimeout;
}

inline uint64 TimeoutType::GetTimeoutUSec() const {
    return usecTimeout;
}

}
#endif /* TIMEOUTTYPE_H_ */

