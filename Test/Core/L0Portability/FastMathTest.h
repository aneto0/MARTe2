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
 **/
/**
 * @file FastMathTest.h
 * @brief Tests the FastMath.h functions. */
#ifndef FASTMATH_TEST_H
#define FASTMATH_TEST_H

#include "FastMath.h"

/** @brief Class fot testing of FastMath functions. */
class FastMathTest {
private:

    static const float SIN_1;
    static const float SIN_PI_2;
    static const float SIN_PI_4;

    static const float COS_1;
    static const float COS_PI_2;
    static const float COS_PI_4;

    static const float EPSILON;

public:
    FastMathTest() {
    }

    /**
     * @brief Tests the fast float to int32 conversion.
     * @param testFloat is the number to convert.
     * @param is the expected integer value after conversion.
     * @return true if the conversion result is equal to the expected value.
     */
    bool TestFloatToInt32(float testFloat, int32 expectedValue) {
        int32 testInt32 = 0;

        testInt32 = FastMath::FloatToInt(testFloat);

        return (testInt32 == expectedValue);
    }

    /**
     * @brief Tests the fast Sin function
     * @param testFloat is the number input.
     * @param is the expected result value after the operation.
     * @return true if the result is equal to the expected value.
     */
    bool TestSin(float angle, float expectedValue) {
        float result = 0.0;
        bool testResult = false;

        result = FastMath::Sin(angle);

        if ((result >= expectedValue - EPSILON)
                && (result <= expectedValue + EPSILON))
            testResult = true;

        return testResult;
    }

    /**
     * @brief Tests the fast Cos function
     * @param testFloat is the number input.
     * @param is the expected result value after the operation.
     * @return true if the result is equal to the expected value.
     */   
    bool TestCos(float angle, float expectedValue) {
        float result = 0.0;
        bool testResult = false;

        result = FastMath::Cos(angle);

        if ((result >= expectedValue - EPSILON)
                && (result <= expectedValue + EPSILON))
            testResult = true;

        return testResult;
    }

    /**
     * @brief Tests for each angle that sin^2(a)+cos^2(a)=1. 
     * @return true if the circle tests works.
     */
    bool CircleTest() {
        float angle = 0;
        float sin = 0;
        float cos = 0;
        while (angle < 2 * (FastMath::PI)) {
            cos = FastMath::Cos(angle);
            sin = FastMath::Sin(angle);
            cos = cos * cos + sin * sin;
            if (!((cos - 1 <= EPSILON) && (cos - 1 >= -EPSILON))) {
                return false;
            }
            angle += 0.001;
        }
        return true;
    }

    /**
     * @brief Executes all the tests.
     * @return true if all tests return true.
     */
    bool All() {
        bool ok = TestFloatToInt32(42.24, 42);
        ok = ok && TestFloatToInt32(-42.24, -42);
        ok = ok && TestSin(1, SIN_1);
        ok = ok && TestSin(FastMath::PI_2, SIN_PI_2);
        ok = ok && TestSin(FastMath::PI_4, SIN_PI_4);
        ok = ok && TestCos(1, COS_1);
        ok = ok && TestCos(3 * (FastMath::PI), -1.0);
        ok = ok && TestCos(FastMath::PI_4, COS_PI_4);
        return ok;
    }
};

#endif

