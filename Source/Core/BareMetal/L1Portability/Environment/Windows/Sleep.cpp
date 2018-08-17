/**
 * @file Sleep.cpp
 * @brief Source file for module Sleep
 * @date 20/06/2015
 * @author Giuseppe Ferr�
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

 * @details This source file contains the definition of all the methods for
 * the module Sleep (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */


#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <time.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Sleep {

void PreciseUsec(uint32 usec, uint32 margin) {
    uint64 start = HighResolutionTimer::Counter();
    // if frequency is >4G and usec is max there is the risk of overflow
    // this reduces resolution but allows up to 16G of clock!!
    uint64 sleepTicks = (usec * (HighResolutionTimer::Frequency() >> 2)) / 250000LLu;
    uint64 end  = start + sleepTicks;
    uint64 minYieldTicks = HighResolutionTimer::GetOsSleepGranularityTicks();
    uint32 minYieldUsec  = HighResolutionTimer::GetOsSleepGranularityUsec();
    uint32 minYieldMsec  = minYieldUsec/1000;

    bool done = false;
    while (!done) {
    	// this works even on a numeric overflow.
    	uint64 missingTicks = (end - HighResolutionTimer::Counter());

    	uint32 counts = missingTicks / minYieldTicks;

    	if ( counts > (margin+1)){
    		::Sleep( minYieldMsec * (counts-margin));
    	} else {
    		done = true;
    	}
    }

    // this will overflow - but it is ok
    uint64 toSleepLeft = (HighResolutionTimer::Counter() - end);
    while (toSleepLeft < sleepTicks) {
        toSleepLeft = (HighResolutionTimer::Counter() - end);
    }
}


void PreciseSeconds(const float32 seconds, uint32 margin){
    if (seconds > 0){
        if (seconds > 4000.0){
        	unsigned long msecs = 0;
            if (seconds > 4.0E6){
            	msecs = 4000000000u;
            } else {
            	msecs = (unsigned long)(seconds * 1000u);
            }
        	::Sleep(msecs);
        } else {
        	uint32 usec = seconds * 1000000u;
    		PreciseUsec(usec,margin);
    	}
    }
}


int32 GetDateSeconds() {
    return (int32) time((time_t *) NULL);
}

}

}
