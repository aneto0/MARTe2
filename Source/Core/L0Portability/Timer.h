/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Multi-platform generic timer implementation
 */
#ifndef TIMER_H
#define TIMER_H

#include "Threads.h"
#include "Sleep.h"




class Timer;

extern "C" {

/** @see Timer::ConfigTimer(). */
bool TimerConfigTimer(Timer &t, int32 usec, int32 cpuMask);

/** @see Timer::ConfigAndStartTimer(). */
bool TimerConfigAndStartTimer(Timer &t, int32 usec, int32 cpuMask);

/** @see Timer::GetTimerUsecPeriod(). */
int64 TimerGetTimerUsecPeriod(Timer &t);

/** @see Timer::ResetTimer(). */
bool TimerResetTimer(Timer &t);

/** @see Timer::Init(). */
void TimerInit(Timer &t);

/** @see Timer::StartTimer(). */
bool TimerStartTimer(Timer &t);

/** @see Timer::StopTimer(). */
bool TimerStopTimer(Timer &t);

/** @see Timer::DeleteTimer; */
bool TimerDeleteTimer(Timer &t);

/** @see Timer::TimerServiceRoutine(). */
void TimerCServiceRoutine(Timer &t);
}

/**
 * @brief Functions for timer management.
 * 
 * These methods allows to call a subroutine function when a timer is elapsed. Here are implemented functions to
 * configure the desired period of the timer, to start and terminate the timer.
 *
 * Most of the implementation is delegated to TimerOS.h which provides system level functions for timer management.
 *  
 */

class Timer {
private:
    /** Variable containing timer status */
    int32 timerStatus;

    /** Friend functions*/
    friend bool TimerConfigTimer(Timer &t, int32 usec, int32 cpuMask);
    friend bool TimerConfigAndStartTimer(Timer &t, int32 usec, int32 cpuMask);
    friend int64 TimerGetTimerUsecPeriod(Timer &t);
    friend bool TimerResetTimer(Timer &t);
    friend void TimerInit(Timer &t);
    friend bool TimerStartTimer(Timer &t);
    friend bool TimerStopTimer(Timer &t);
    friend bool TimerDeleteTimer(Timer &t);
    friend void TimerCServiceRoutine(Timer &t);

public:
    /** Period set for the timer */
    int64 timerUsecPeriod;

    /** Statistics */
    int64 timerCounter;
    int64 currentHRTTick;
    int64 oldHRTTick;

    static int32 NOT_CONFIGURED;
    static int32 CONFIGURED;
    static int32 RUNNING;

public:

    /** @brief Constructor.
      * Creates, configures and starts the timer with usec as period (in microseconds). */
    Timer(int32 usec) {
        Init();
        if (!ConfigAndStartTimer(usec)) {
            //CStaticAssertErrorCondition(InitialisationError, "Timer::Timer(int32 usec) -> timer setup did not complete successfully");
        }
    }

    /** @brief Constructor.
      * Creates the timer. */
    Timer() {
        Init();
    }

    /** @brief Initialise timer parameters. */
    void Init() {
        TimerInit(*this);
    }

    /** @brief Initialise statistics parameters. */
    void InitStats() {
        timerCounter = 0;
        currentHRTTick = 0;
        oldHRTTick = 0;
    }

    /** @brief Destructor. */
    virtual ~Timer() {
        if (timerStatus == RUNNING) {
            if (!StopTimer()) {	
                //CStaticAssertErrorCondition(InitialisationError, "Timer::~Timer() -> StopTimer() failed");
            }
        }
	TimerDeleteTimer(*this);
    }

    /** @brief Configure the timer
      * This method can be called anytime as long as the object exists.
      * @param usec is the desired period of the timer (in microseconds).
      * @param cpuMask is the mask of the cpu.
      * @return true if the system level function return without errors.  */
    bool ConfigTimer(int32 usec, int32 cpuMask = 0xff) {
        return TimerConfigTimer(*this, usec, cpuMask);
    }

    /** @brief Configure and start the timer.
      * This method can be called anytime as long as the object exists and even if a timer is already running.
      * @param usec is the desired period of the timer (in microseconds).
      * @param cpuMask is the mask of cpu.
      * @return true if configuration return true and if the state of timer was not already in RUNNING.*/
    bool ConfigAndStartTimer(int32 usec, int32 cpuMask = 0xff) {
        return TimerConfigAndStartTimer(*this, usec, cpuMask);
    }

    /** @brief Starts a timer that has already been configured.
      * @return false if the timer is not in CONFIGURED status. */
    bool StartTimer() {
        return TimerStartTimer(*this);
    }

    /** @brief Get current timer period in microseconds.
      * @return the timer period in microseconds. */
    int64 GetTimerUsecPeriod() {
        return TimerGetTimerUsecPeriod(*this);
    }

    /** @brief Get value for internal timer counter (how many times a timer started).
      * @return the timer counter. */
    int64 GetTimerCounter() {
        return timerCounter;
    }

    /** @brief Stop timer.
      * @return true if the system level function returns without errors. */
    bool StopTimer() {
        return TimerStopTimer(*this);
    }

    /** @brief Reset timer internal counter and statistics. 
      * It works only if the timer is not in RUNNING mode.
      * @return false if the timer is in RUNNING mode, true otherwise. */
    bool ResetTimer() {
        return TimerResetTimer(*this);
    }

    /** @brief Get the timer status.
     *  @return the timer status. */
    int32 TimerStatus() {
        return timerStatus;
    }

    /** @brief Collects statistical info and calls the user's timer service routine. */
    void TimerServiceRoutine() {
        TimerCServiceRoutine(*this);
    }

    /** @brief User's timer service routine.
      * This function is called once every period usec. */
    virtual void UserTimerServiceRoutine() {
    }
};

#endif

