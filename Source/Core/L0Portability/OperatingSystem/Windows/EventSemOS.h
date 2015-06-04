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
 * $Id:  $
 *
 **/

/**
 * @file 
 * Windows implementation of the EventSem
 */
#ifndef EVENT_SEM_OS_H
#define EVENT_SEM_OS_H

#include "../../TimeoutType.h"

/** 
 * @see EventSem::Create
 */
static bool EventSemCreate(HANDLE &semH) {
    semH = CreateEvent(NULL, TRUE, FALSE, NULL);
    return (semH != NULL);
}

/**
 * @see EventSem::Close
 */
static bool EventSemClose(HANDLE &semH) {
    if (semH == (HANDLE) NULL)
        return True;
    if (CloseHandle(semH) == FALSE) {
        return False;
    }
    semH = NULL;
    return True;
}

/** 
 * @see EventSem::Wait
 */
static inline bool EventSemWait(HANDLE &semH, TimeoutType msecTimeout =
                                        TTInfiniteWait) {
    int ret;
    ret = WaitForSingleObject((HEV) semH, msecTimeout.msecTimeout);
    if (ret == (int) WAIT_FAILED) {
        return False;
    }
    if (ret == (int) WAIT_TIMEOUT)
        return False;
    return True;
}

/**
 * @see EventSem::Post
 */
static inline bool EventSemPost(HANDLE &semH) {
    if (SetEvent((HEV) semH) == FALSE) {
        return False;
    }
    return True;
}

/** 
 * @see EventSem::Reset
 */
static inline bool EventSemReset(HANDLE &semH) {
    if (ResetEvent((HEV) semH) == FALSE) {
        return False;
    }
    return True;
}

/** 
 * @see EventSem::ResetWait
 */
static inline bool EventSemResetWait(HANDLE &semH, TimeoutType msecTimeout =
                                             TTInfiniteWait) {
    EventSemReset(semH);
    return EventSemWait(semH, msecTimeout);
}

#endif

