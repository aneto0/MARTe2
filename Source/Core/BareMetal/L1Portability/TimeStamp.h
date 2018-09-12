/**
 * @file TimeStamp.h
 * @brief Header file for class TimeStamp
 * @date 11/11/2015
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

 * @details This header file contains the declaration of the class TimeStamp
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class which describes a time stamp information.
 */
class DLL_API TimeStamp {

public:

    /**
     * @brief Default constructor.
     * @post
     *   microseconds=0;
     *   seconds=0;
     *   minutes=0;
     *   hours=0;
     *   days=0;
     *   month=0;
     *   year=1900;
     */
    TimeStamp();

    /**
     * TODO Initializes  with current time
     */
    bool GetTime();

    /**
     * @brief Converts the seconds from epoch in input to the time stamp.
     * @param[in] secondsFromEpoch The seconds elapsed from epoch (January 1, 1970).
     * @return false in case of errors, true otherwise.
     */
    bool ConvertFromEpoch(const oslong secondsFromEpoch);

    /**
     * @brief Sets the microseconds in time stamp.
     * @param[in] microsecondsIn is the input for microseconds member.
     * @pre
     *   microsecondsIn in [0, 999999];
     * @post
     *   microseconds=microsecondsIn % 1000000;
     */
    inline void SetMicroseconds(const uint32 microsecondsIn);

    /**
     * @brief Returns the microseconds (0-999999).
     */
    inline uint32 GetMicroseconds() const;

    /**
     * @brief Sets the seconds in time stamp.
     * @param[in] secondsIn is the input for seconds member.
     * @pre
     *   secondsIn in [0, 59];
     * @post
     *   seconds=secondsIn % 60;
     */
    inline void SetSeconds(const uint32 secondsIn);

    /**
     * @brief Returns the seconds (0-59).
     */
    inline uint32 GetSeconds() const;

    /**
     * @brief Sets the minutes in time stamp.
     * @param[in] minutesIn is the input for minutes member.
     * @pre
     *   minutesIn in [0, 59];
     * @post
     *   minutes=minutesIn % 60;
     */
    inline void SetMinutes(const uint32 minutesIn);

    /**
     * @brief Returns the minutes (0-59).
     */
    inline uint32 GetMinutes() const;

    /**
     * @brief Sets the hours in time stamp.
     * @param[in] hoursIn is the input for hours member.
     * @pre
     *   hoursIn in [0, 23];
     * @post
     *   hours=hoursIn % 24;
     */
    inline void SetHour(const uint32 hoursIn);

    /**
     * @brief Returns the hour (0-23).
     */
    inline uint32 GetHour() const;

    /**
     * @brief Sets the day in time stamp.
     * @param[in] dayIn is the input for the day member.
     * @pre
     *   dayIn in [0, 30];
     * @post
     *   day=dayIn % 31;
     */
    inline void SetDay(const uint32 dayIn);

    /**
     * @brief Returns the day (0-30)
     */
    inline uint32 GetDay() const;

    /**
     * @brief Sets the month in time stamp.
     * @param[in] monthIn is the input for microseconds member.
     * @pre
     *   monthIn in [0, 11];
     * @post
     *   month=monthIn % 12;
     */
    inline void SetMonth(const uint32 monthIn);

    /**
     * @brief Returns the month number (0-11).
     */
    inline uint32 GetMonth() const;

    /**
     * @brief Sets the year in time stamp.
     * @param[in] yearIn is the input for year member.
     * @pre
     *   yearIn >= 1900;
     * @post
     *   yearIn >= 1900;
     */
    inline void SetYear(const uint32 yearIn);

    /**
     * @brief Returns the year (>=1900).
     */
    inline uint32 GetYear() const;

private:
    /** nanoseconds 0-999999 */
    uint32 microseconds;

    /** seconds 0-59 */
    uint32 seconds;

    /** minutes 0-59 */
    uint32 minutes;

    /** hours 0-23 */
    uint32 hours;

    /** days 0-30 */
    uint32 days;

    /** month 0-11 */
    uint32 month;

    /** year since 1900 */
    uint32 year;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void TimeStamp::SetMicroseconds(const uint32 microsecondsIn) {
    microseconds = (microsecondsIn % 1000000u);
}

uint32 TimeStamp::GetMicroseconds() const {
    return microseconds;
}

void TimeStamp::SetSeconds(const uint32 secondsIn) {
    seconds = (secondsIn % 60u);
}

uint32 TimeStamp::GetSeconds() const {
    return seconds;
}

void TimeStamp::SetMinutes(const uint32 minutesIn) {
    minutes = (minutesIn % 60u);
}

uint32 TimeStamp::GetMinutes() const {
    return minutes;
}

void TimeStamp::SetHour(const uint32 hoursIn) {
    hours = (hoursIn % 24u);
}

uint32 TimeStamp::GetHour() const {
    return hours;
}

void TimeStamp::SetDay(const uint32 dayIn) {
    days = (dayIn % 31u);
}

uint32 TimeStamp::GetDay() const {
    return days;
}

void TimeStamp::SetMonth(const uint32 monthIn) {
    month = (monthIn % 12u);
}

uint32 TimeStamp::GetMonth() const {
    return month;
}

void TimeStamp::SetYear(const uint32 yearIn) {
    year = yearIn;
    if (year < 1900u) {
        year = 1900u;
    }
}

uint32 TimeStamp::GetYear() const {
    return year;
}

}

#endif /* TIMESTAMP_H_ */

