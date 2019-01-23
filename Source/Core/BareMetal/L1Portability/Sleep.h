/**
 * @file Sleep.h
 * @brief Header file for module Sleep
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

 * @details This header file contains the declaration of the module Sleep
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEP_H_
#define SLEEP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"
#include "MicroSeconds.h"
#include "MilliSeconds.h"
#include "Ticks.h"
#include "TimeCalibration.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief MARTe Sleep functions.
 */
class DLL_API Sleep {
public:
    /**
     * @brief Retrieve the time as seconds from the start of time.
     * @return the seconds elapsed from 00:00 of Jan 1, 1970.
     */
    static int32 GetDateSeconds(void);

    /**
     * @brief Sleeps for the time specified. Resolution is microseconds. Max Sleep is 4000 seconds
     * @param[in] time is the time to sleep.
     */
    static inline void Short(const MicroSeconds &time);

    /**
     * @brief Sleeps for the time specified. Resolution is microseconds. Max Sleep is 4000 seconds
	 * @tparam bT the type of time
	 * @tparam uT the type of units
     * @param[in] time is the time to sleep specified in the units units.
     * @param[in] units is the unit for time
     */
    template<typename bT, class uT>
    static inline void Short(bT time,const uT &units);

    /**
     * @brief Sleeps for the time specified. Resolution is milliseconds. Max Sleep is 4000000 seconds
     * @param[in] time is the time to sleep.
     */
    static inline void Long(const MilliSeconds &time);

    /**
     * @brief Sleeps for the time specified. Resolution is milliseconds. Max Sleep is 4000000 seconds
	 * @tparam bT the type of time
	 * @tparam uT the type of units
     * @param[in] time is the time to sleep specified in the units units.
     * @param[in] units is the unit for time
     */
    template<typename bT, class uT>
    static inline void Long(bT time,const uT &units);

    /**
     * @brief Sleeps no more than the requested time.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (no more).
     */
    static inline void NoMore(const MicroSeconds &time);

    /**
     * @brief Sleeps no more than the requested time
	 * @tparam bT the type of time
	 * @tparam uT the type of units
     * @param[in] time is the time to sleep specified in the units units.
     * @param[in] units is the unit for time
     */
    template<typename bT, class uT>
    static inline void NoMore(bT time,const uT &units);

    /**
     * @brief Sleep without yield cpu.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the seconds to sleep.
     */
    static inline void Busy(const MicroSeconds &time);

    /**
     * @brief Sleep without yield cpu.
	 * @tparam bT the type of time
	 * @tparam uT the type of units
     * @param[in] time is the time to sleep specified in the units units.
     * @param[in] units is the unit for time
     */
    template<typename bT, class uT>
    static inline void Busy(bT time,const uT &units);

    /**
     * @brief Sleep yielding cpu for nonBusySleepSec.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] totalSleepSec is the total time in seconds to sleep.
     * @param[in] nonBusySleepSec is the time to sleep without use cpu.
     */
    static inline void SemiBusy(const MicroSeconds &totalTime,const MicroSeconds &nonBusyTime);

private:

    /**
     * @brief Function to be used by all the other methods
     * @param[in] totalUsecTime is the total time in micro-seconds to sleep.
     * @param[in] atomUsSleep is the size of an individual OSUSleep.
     * @param[in] minBusyTicks: if the tick left threshold to switch to BusySleep
     */
    static inline void DoSleep(const MicroSeconds &totalTime,uint32 atomUsSleep,uint64 minBusyTicks);

    /**
     * @brief Wraps the operating system sleep call
     * @param[in] usecTime is the time to sleep in micro-seconds
     */
    static void OsUsleep(uint32 usecTime);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void Sleep::NoMore(const MicroSeconds &time) {
	DoSleep(time, TimeCalibration::osMinSleepUsec,TimeCalibration::osSleepTicks);
}

template<typename bT, class uT>
void Sleep::NoMore(bT time,const uT &units){
	NoMore(MicroSeconds(time,units));
}

void Sleep::Long(const MilliSeconds &time){
	MicroSeconds usTime(time);
    OsUsleep(usTime.GetTimeRaw());
}

template<typename bT, class uT>
void Sleep::Long(bT time,const uT &units){
	Long(MicroSeconds(time,units));
}

void Sleep::Short(const MicroSeconds &time) {
    OsUsleep(time.GetTimeRaw());
}

template<typename bT, class uT>
void Sleep::Short(bT time,const uT &units){
	Short(MicroSeconds(time,units));
}

void Sleep::Busy(const MicroSeconds &time) {
	Ticks tTime(time);
	DoSleep(time, 0u,tTime.GetTimeRaw());
}

template<typename bT, class uT>
void Sleep::Busy(bT time,const uT &units){
	Busy(MicroSeconds(time,units));
}

void Sleep::SemiBusy(const MicroSeconds &totalTime,const MicroSeconds &nonBusyTime) {
	Ticks nbTime(nonBusyTime);
	DoSleep(totalTime, TimeCalibration::osMinSleepUsec,nbTime.GetTimeRaw());
}

void Sleep::DoSleep(const MicroSeconds &totalTime,uint32 atomUsSleep,uint64 minBusyTicks) {
    uint64 startCounter = HighResolutionTimer::Counter();
	Ticks tTime(totalTime);
    uint64 stopCounter = startCounter + tTime.GetTimeRaw();

    while ((stopCounter - HighResolutionTimer::Counter() ) < minBusyTicks) {
        OsUsleep(atomUsSleep);
    }

    while (HighResolutionTimer::Counter()  < stopCounter) {
    }
}

}
#endif /* SLEEP_H_ */
