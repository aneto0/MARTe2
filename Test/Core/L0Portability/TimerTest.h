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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 * @file TimerTest.h
 * @brief Tests the Timer class and associated functions
 */
#ifndef TIMER_TEST_H
#define TIMER_TEST_H

#include "Timer.h"
#include "EventSem.h"

/** @brief A class for testing of Timer functions. */
class TimerTest: public Timer {
    /**
     * Generic event sem used for synchronisations in the test
     */
    EventSem eventSem;

public:
    TimerTest() {

    }

    /**
     * @brief Tests the ConfigAndStartTimer function.
     *
     * It starts the timer with a 1 msec period
     * and waits on an event semaphore with a timeout of 1 sec. It is expected
     * that the callback function of the timer (UserTimerServiceRoutine) will
     * unlock the semaphore before 1 sec.
     *
     * @return True if the callback unlocks the semaphore which demonstrates that the
     * Timer has been configured and started.
     */
    bool ConfigAndStartTimerTest() {
        if (!eventSem.Create()) {
            return False;
        }
        if (!eventSem.Reset()) {
            return False;
        }
        if (!ConfigAndStartTimer(1000, 0xff)) {
            return False;
        }
        if (TimerStatus() != Timer::RUNNING) {
            return False;
        }

        TimeoutType testTimeout(1000);
        if (!eventSem.Wait(testTimeout)) {
            return False;
        }
        if (!StopTimer()) {
            return False;
        }

        return True;
    }

    /**
     * @brief Tests the ResetTimer function.
     *
     * It configure and start timer, then the reset should return true.
     * If the timer is stopped, the reset should return false.
     * 
     * @return true if the reset return the correct value. */
    bool ResetTimerTest() {

        if (!ConfigAndStartTimer(10000, 0xff)) {
            return False;
        }

        //The timer is running so the reset should return true
        if (!ResetTimer()) {
            return False;
        }

        if (!StopTimer()) {
            return False;
        }

        //The timer is not running so the reset should return false
        if (ResetTimer()) {

            return False;
        }

        return True;

    }

    /**
     * @brief Configures but does not start the timer.
     *
     * It waits for 1 second on a semaphore
     * that should not be unlocked by the callback routine, as otherwise the Timer
     * would have been started without being requested
     *
     * @return True if the semaphore is not unlocked and consequently the Timer is not
     * started.
     */
    bool ConfigAndNotStartTimerTest() {
        if (!eventSem.Create()) {
            return False;
        }
        if (!eventSem.Reset()) {
            return False;
        }
        if (!ConfigTimer(1000, 0xff)) {
            return False;
        }
        if (TimerStatus() != Timer::CONFIGURED) {
            return False;
        }

        TimeoutType testTimeout(100);
        if (!eventSem.Wait(testTimeout)) {
            return True;
        }

        return False;
    }

    /**
     * @brief Callback function of the timer, called everytime the timer is fired
     */
    virtual void UserTimerServiceRoutine() {
        eventSem.Post();
    }
};

#endif
