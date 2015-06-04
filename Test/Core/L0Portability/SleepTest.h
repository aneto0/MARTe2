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
 * $Id$
 *
 **/
/** @file SleepTest.h
  * @brief Tests the Sleep functions. */
#ifndef SLEEP_TEST_H
#define SLEEP_TEST_H

#include "Sleep.h"
#include <time.h>

/** @brief A class for testing of the sleep functions. */
class SleepTest {

public:
    SleepTest() {
    }

    /**
     * @brief Tests the SleepAtLeast function.
     * @param sec is the minimum time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepAtleast(double sec) {
        bool testResult = false;
        double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
        int initialTime = clock();

        SleepAtLeast(sec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepNoMore function
     * @param sec is the maximum time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepNoMore(double sec) {
        bool testResult = false;
        double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
        int initialTime = clock();

        SleepNoMore(sec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepSec function
     * @param sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepSec(double sec) {
        bool testResult = false;
        double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
        int initialTime = clock();

        SleepSec(sec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepSec function
     * @param sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepSec(float sec) {
        bool testResult = false;
        float maxSleepTime = 2 * 1000 * sec; /* 100% margin */
        int initialTime = clock();

        SleepSec(sec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepMSec function
     * @param sec is the time to sleep in milliseconds.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepMSec(int32 msec) {
        bool testResult = false;
        double maxSleepTime = 2 * msec; /* 100% margin */
        int initialTime = clock();

        SleepMSec(msec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepBusy function.
     * @param sec is the time to sleep.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSleepBusy(double sec) {
        bool testResult = false;
        double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
        int initialTime = clock();

        SleepBusy(sec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Tests the SleepSemiBusy function.
     * @param totalSleepSec is the time to sleep using cpu.
     * @param nonBusySleepSec is the time to sleep without using cpu.
     * @return true if it sleeps less than the double of sec.
     */
    bool TestSemiBusy(double totalSleepSec, double nonBusySleepSec) {
        bool testResult = false;
        double maxSleepTime = 2 * 1000 * totalSleepSec; /* 100% margin */
        int initialTime = clock();

        SleepSemiBusy(totalSleepSec, nonBusySleepSec);

        testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000))
                <= maxSleepTime);

        return testResult;
    }

    /**
     * @brief Executes all the tests
     * @return true if all tests return true.
     */
    bool All() {
        bool ok = TestSleepAtleast(1.2);
        ok = ok && TestSleepNoMore(0.8);
        ok = ok && TestSleepSec(0.3);
        ok = ok && TestSleepSec(0.2);
        ok = ok && TestSleepMSec(120);
        ok = ok && TestSleepBusy(0.4);
        ok = ok && TestSemiBusy(0.5, 0.2);
        GetDateSeconds();
        return ok;
    }
};

#endif

