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
 * @brief Linux timer implementation based on signals.
 */


#include "cmsis_os.h"


static osTimerId timerID = NULL;
static Timer* timerThisPtr;


/** @brief Define the routine to be executed at the end of each timer expire.
  * @param sig is the signal caughted (SIGALRM must be handled). */
void TimerOSTimerServiceRoutine(const void *arg) {
    timerThisPtr->TimerServiceRoutine();
}

/** @see Timer::Init */
void TimerOSInit(Timer &t) {
    timerThisPtr = &t;
}

/** @see Timer::ConfigTimer.
  * 
  * Uses sigaction function to caught SIGALRM signal and launch the desired service routine. */
bool TimerOSConfigTimer(Timer &t, int32 usec, int32 cpuMask) {
	const osTimerDef_t timerDefinition = {TimerOSTimerServiceRoutine};

	//argument function is not implemented in the higher level.
	timerID = osTimerCreate(&timerDefinition, osTimerOnce, NULL);
	
	return timerID != NULL;
}

/** @see Timer::StartTimer
  *
  * A sigalarm signal is generated when the timer is expired. */
bool TimerOSStartTimer(Timer &t) {

	if(timerID == NULL) return False;
	
	return osTimerStart(timerID, t.timerUsecPeriod/1000) == 0;
}

/** @see Timer::StopTimer */
bool TimerOSStopTimer(Timer &t) {

	if(timerID == NULL) return False;
	
	return osTimerStop(timerID) == 0;

	
}


/** @see Timer::DeleteTimer */
bool TimerOSDeleteTimer(Timer &t) {

	if(timerID == NULL) return False;
	
	osStatus ret = osTimerDelete(timerID);
	timerID=NULL;
	return ret;
	
}
