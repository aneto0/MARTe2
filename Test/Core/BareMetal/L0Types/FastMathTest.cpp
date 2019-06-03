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

#include "FastMathTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

const float FastMathTest::SIN_1 = (float)0.841470984807;
const float FastMathTest::SIN_PI_2 = 1.0;
const float FastMathTest::SIN_PI_4 = (float)0.707106781186;

const float FastMathTest::COS_1 = (float)0.540302305868;
const float FastMathTest::COS_PI_2 = 0.0;
const float FastMathTest::COS_PI_4 = (float)0.707106781186;

const float FastMathTest::EPSILON = (float)0.000001;

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
        angle += (float)0.001;
    }
    return true;
}

bool FastMathTest::All() {
    bool ok = TestFloatToInt32((float)42.24, 42);
    ok = ok && TestFloatToInt32((float)-42.24, -42);
    ok = ok && TestSin(1, SIN_1);
    ok = ok && TestSin((float)FastMath::PI_2, (float)SIN_PI_2);
    ok = ok && TestSin((float)FastMath::PI_4, (float)SIN_PI_4);
    ok = ok && TestCos(1, (float)COS_1);
    ok = ok && TestCos(3 * ((float)FastMath::PI), -1.0);
    ok = ok && TestCos((float)FastMath::PI_4, (float)COS_PI_4);
    return ok;
}

#if 0
bool FastMathTest::TestSquareRootUint8(const uint8 testVal,const  uint8 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint8>(testVal));
}


bool FastMathTest::TestSquareRootUint16(const uint16 testVal,const  uint16 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint16>(testVal));
}


bool FastMathTest::TestSquareRootUint32(const uint32 testVal,const  uint32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint32>(testVal));
}


bool FastMathTest::TestSquareRootUint64(const uint64 testVal,const  uint64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint64>(testVal));
}

bool FastMathTest::TestSquareRootInt8(const int8 testVal,const  int8 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int8>(testVal));
}

bool FastMathTest::TestSquareRootInt16(const int16 testVal,const  int16 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int16>(testVal));
}

bool FastMathTest::TestSquareRootInt32(const int32 testVal,const  int32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int32>(testVal));
}

bool FastMathTest::TestSquareRootInt64(const int64 testVal,const  int64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int64>(testVal));
}

bool FastMathTest::TestSquareRootFloat32(const float32 testVal,const  float32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<float32>(testVal));
}

bool FastMathTest::TestSquareRootFloat64(const float64 testVal,const  float64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<float64>(testVal));
}
#endif
