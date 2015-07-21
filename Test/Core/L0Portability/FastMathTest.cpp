/**
 * @file FastMathTest.cpp
 * @brief Source file for class FastMathTest
 * @date 26/06/2015
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
 * the class FastMathTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "FastMathTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
// Math constants
const float FastMathTest::SIN_1 = 0.841470984807;
const float FastMathTest::SIN_PI_2 = 1.0;
const float FastMathTest::SIN_PI_4 = 0.707106781186;

const float FastMathTest::COS_1 = 0.540302305868;
const float FastMathTest::COS_PI_2 = 0.0;
const float FastMathTest::COS_PI_4 = 0.707106781186;

const float FastMathTest::EPSILON = 0.000001;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FastMathTest::FastMathTest() {
}

bool FastMathTest::TestFloatToInt32(float testFloat,
                                    int32 expectedValue) {
    int32 testInt32 = 0;
    testInt32 = FastMath::FloatToInt(testFloat);
    return (testInt32 == expectedValue);
}

bool FastMathTest::TestSin(float angle,
                           float expectedValue) {
    float result = 0.0;
    bool testResult = false;
    result = FastMath::Sin(angle);
    if ((result >= expectedValue - EPSILON) && (result <= expectedValue + EPSILON))
        testResult = true;
    return testResult;
}

bool FastMathTest::TestCos(float angle,
                           float expectedValue) {
    float result = 0.0;
    bool testResult = false;
    result = FastMath::Cos(angle);
    if ((result >= expectedValue - EPSILON) && (result <= expectedValue + EPSILON))
        testResult = true;
    return testResult;
}

bool FastMathTest::CircleTest() {
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

bool FastMathTest::All() {
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
