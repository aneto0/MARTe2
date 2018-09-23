/**
 * @file UsecTime.h
 * @brief Header file for class AnyType
 * @date 13 Sep 2018
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef USECTIME_H_
#define USECTIME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"
#include "OSInitializer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief UsecTime type definition.
 *
 * @details This number is used to specify sleep times in uSeconds.
 */
class UsecTime{

    /**
     * @brief Constructor from integer.
     * @param[in] usecs is the time in microseconds.
     */
    inline UsecTime(const uint32 &usecs = 0xFFFFFFFFu);

    /**
     * @brief Sets the timeout from float32.
     * @param[in] secs is the time in seconds (limited to 4000 seconds. After that Infinite).
     */
    /*lint -e(1960) , functions should have external effects*/
    inline void FromSeconds(float32 secs);

    /**
     * @brief Sets the timeout from HighResolutionTimer ticks.
     * @param[in] ticks are the number of cpu ticks.
     */
    /*lint -e(1960) , functions should have external effects*/
    inline void FromTicks(uint64 ticks);

    /**
     * @brief Gets the timeout in HighResolutionTimer Ticks
     * @return the number of ticks related to the timeout.
     */
    inline uint64 GetTicks() const;

    /**
     * @brief Returns the value in microseconds.
     * @return the value in microseconds.
     */
    inline uint32 GetUsecs() const;

    /**
     * @brief Returns the value in seconds.
     * @return the value in seconds.
     */
    inline uint32 GetSeconds() const;

    /**
     * @brief Compares two times.
     * @param[in] ut is the time object to be compared with this.
     * @return true if both times object are equal.
     */
    inline bool operator==(const UsecTime &ut) const;

    /**
     * @brief Copy operator.
     * @param[in] ut is the time to copy in this.
     * @return this object.
     */
    inline UsecTime &operator=(const UsecTime &ut);

    /**
     * @brief Check if the time is finite.
     * @return true if the time is less than the max timeout acceptable.
     */
    inline bool IsFinite() const;

private:

    /**
     * Time in microseconds
     */
	uint32 microSeconds;
};


/** Do not wait (or wait indefinitely if blocking is set */
const UsecTime UTNoWait(0x00000000u);

/** Infinite wait timeout */
const UsecTime UTInfiniteWait(0xFFFFFFFFu);

/** Minimum time for a sleep to yield CPU*/
#define  UTMinSleep  UsecTime( OSInit::osMinSleepUsec)

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

UsecTime::UsecTime(const uint32 &usecs) {
    microSeconds = usecs;
}

void UsecTime::FromSeconds(float32 secs) {
	if (secs > 4000.0){
		*this = UTInfiniteWait;
	} else
		if (secs <= 0.0){
			*this = UTNoWait;
		} else {
		    microSeconds = static_cast<uint32>(secs*1.0E6);
		}
}

void UsecTime::FromTicks(uint64 ticks) {
    float64 temp = static_cast<float64>(ticks);
    temp = temp * HighResolutionTimer::Period();
    temp = temp * 1.0e6;
    microSeconds = static_cast<uint32>(temp);
}

uint64 UsecTime::GetTicks() const {
    float64 temp = static_cast<float64>(microSeconds);
    temp = temp * 1.0e-6;
    float64 frequency = static_cast<float64>(HighResolutionTimer::Frequency());
    temp = temp * frequency;
    uint64 ticks = static_cast<uint64>(temp);
    return ticks;
}

bool UsecTime::operator==(const UsecTime &ut) const {
    return microSeconds == ut.microSeconds;
}

UsecTime &UsecTime::operator=(const UsecTime &ut) {
   	microSeconds = ut.microSeconds;
    return *this;
}

bool UsecTime::IsFinite() const {
    return (microSeconds < 0xFFFFFFFEu);
}

inline uint32 UsecTime::GetUsecs() const {
    return microSeconds;
}

}

#endif /* USECTIME_H_ */
