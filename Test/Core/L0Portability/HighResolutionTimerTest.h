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
 * $Id:$
 *
 */
/**
 * @file HighResolutionTimerTest.h
 * @brief Tests the HighResolutionTimer and associated functions.
 *
 * The test consists in concurrent threads incrementing a shared variable.
 * These will only be allowed to change this variable value when the semaphore is posted.
 */

#ifndef MUTEX_TEST_H
#define MUTEX_TEST_H

#include "Sleep.h"

/** @brief Class for testing of HighResolutionTimer functions. */
class HighResolutionTimerTest {

private:

public:

   /**
     * @brief Checks The functions Period() and Frequency() comparing the returned frequency with the mutual of the period 
     * @reurn true if period and frequency are mutual, false otherwise. **/
    bool TestFrequency();

    /**
     * @brief Sleep for sleepTime time and check if the time measured with Counter() is more or less the same. 
     * @param sleepTime is the time to sleep.
     * @return true if the measured time is the same of the sleep time less than a tolerance, false otherwise. **/
    bool TestCounter(double sleepTime);

};

#endif
