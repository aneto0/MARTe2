
/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 * See the Licence for the specific language governing
   permissions and limitations under the Licence.
 *
 * $Id: Timer.cpp 77 2012-11-07 09:44:30Z aneto $
 *
**/

#include "Timer.h"

static MMRESULT FTimerID=NULL;

void CALLBACK TimerOSServiceRoutine(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2){
	Timer *timer = (Timer*)dwUser;
	if( timer == NULL) return;
	timer->TimerServiceRoutine();
}


void TimerOSInit(Timer &t) {
  t.timerUsecPeriod = 0;
}

bool TimerOSConfigTimer(Timer &t, int32 usec, int32 cpuMask) {

	t.timerUsecPeriod = usec;


  return True;
}

bool TimerOSStartTimer(Timer &t) {


  /** Reset statistic variables */
  t.InitStats();


  timeBeginPeriod(t.timerUsecPeriod/1000);
    FTimerID = timeSetEvent(t.timerUsecPeriod/1000, t.timerUsecPeriod/1000, TimerOSServiceRoutine, (unsigned int)&t, TIME_ONESHOT);

  //t.FTimerID = timeSetEvent(t.timerUsecPeriod/1000, t.timerUsecPeriod/1000, TimerOSServiceRoutine, (unsigned int)&t, TIME_PERIODIC);
  if( FTimerID == NULL ){
//    CStaticAssertErrorCondition(InitialisationError,"TimerDrv: StartTimer(): Unable to start timer");
    return False;
  }




  return True;
}

bool TimerOSStopTimer(Timer &t) {

  if( FTimerID != NULL ){
    timeKillEvent(FTimerID);
  }
  timeEndPeriod(t.timerUsecPeriod/1000);


	FTimerID=NULL;

  return True;
}


bool TimerOSDeleteTimer(Timer &t){
    return TimerOSStopTimer(t);
}


