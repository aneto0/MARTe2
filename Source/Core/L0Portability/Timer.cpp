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

#include "Timer.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,TimerOS.h)

int32 Timer::NOT_CONFIGURED = 0;
int32 Timer::CONFIGURED = 1;
int32 Timer::RUNNING = 2;

void TimerCServiceRoutine(Timer &t) {
    t.timerCounter++;
    /** Call user defined signal handler routine */
    t.UserTimerServiceRoutine();
}

void TimerInit(Timer &t) {
    TimerOSInit(t);
    t.timerStatus = Timer::NOT_CONFIGURED;
}

bool TimerConfigTimer(Timer &t, int32 usec, int32 cpuMask) {
    t.timerUsecPeriod = usec;
    if (!TimerOSConfigTimer(t, usec, cpuMask)) {
        return False;
    }

    /** Set timer status */
    t.timerStatus = Timer::CONFIGURED;

    return True;
}

bool TimerStartTimer(Timer &t) {
    // Check if timer has already been configured or is already running
    if (t.TimerStatus() != Timer::CONFIGURED) {
        //CStaticAssertErrorCondition(InitialisationError, "Timer::StartTimer(): Timer not yet configured");
        return False;
    }
    if (t.TimerStatus() == Timer::RUNNING) {
        //CStaticAssertErrorCondition(InitialisationError, "Timer::StartTimer(): Timer not yet configured");
        return False;
    }

    if (!TimerOSStartTimer(t)) {
        return False;
    }

    /** Set timer status */
    t.timerStatus = Timer::RUNNING;

    return True;
}

bool TimerConfigAndStartTimer(Timer &t, int32 usec, int32 cpuMask) {
    if (!t.ConfigTimer(usec, cpuMask)) {
        //CStaticAssertErrorCondition(InitialisationError, "Timer::ConfigAndStartTimer(int32 usec) -> ConfigTimer() failed");
        return False;
    }
    if (!t.StartTimer()) {
        //CStaticAssertErrorCondition(InitialisationError, "Timer::ConfigAndStartTimer(int32 usec) -> StartTimer() failed");
        return False;
    }
    return True;
}

bool TimerStopTimer(Timer &t) {
    if (!TimerOSStopTimer(t)) {
        return False;
    }

    /** Set timer status */
    t.timerStatus = Timer::NOT_CONFIGURED;

    return True;
}

bool TimerResetTimer(Timer &t) {
    if (t.TimerStatus() != Timer::RUNNING) {
        //CStaticAssertErrorCondition(FatalError, "Timer::ResetTimer() -> timer not yet running");
        return False;
    }

    t.InitStats();

    return True;
}


bool TimerDeleteTimer(Timer &t) {
   //timer destructor
   return TimerOSDeleteTimer(t);
}

