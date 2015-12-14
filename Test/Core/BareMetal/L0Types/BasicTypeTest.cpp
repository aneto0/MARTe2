/**
 * @file BasicTypeTest.cpp
 * @brief Source file for class BasicTypeTest
 * @date 02/09/2015
 * @author Llorenç Capellà
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
 * the class BasicTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicTypeTest::BasicTypeTest() {
    retVal = false;
}

bool BasicTypeTest::TestConstructorInt8(){
    int8 x1 = 16;
    int8 x2 = 5;
    int8 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorUInt8(){
    uint8 x1 = 16;
    uint8 x2 = 5;
    uint8 x3 = 0;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorInt16(){
    int16 x1 = 16;
    int16 x2 = 5;
    int16 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorUInt16(){
    uint16 x1 = 16;
    uint16 x2 = 5;
    uint16 x3 = 0;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorInt32(){
    int32 x1 = 16;
    int32 x2 = 5;
    int32 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorUInt32(){
    uint32 x1 = 16;
    uint32 x2 = 5;
    uint32 x3 = 0;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorInt64(){
    int64 x1 = 16;
    int64 x2 = 5;
    int64 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorUInt64(){
    uint64 x1 = 16;
    uint64 x2 = 5;
    uint64 x3 = 0;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorFloat32(){
    float32 x1 = 16;
    float32 x2 = 5;
    float32 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorFloat64(){
    float64 x1 = 16;
    float64 x2 = 5;
    float64 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}

bool BasicTypeTest::TestConstructorChar8(){
    char8 x1 = 16;
    char8 x2 = 5;
    char8 x3 = -5;
    BasicType btLarge(x1);
    BasicType btMedium(x2);
    BasicType btSmall(x3);

    retVal = (btLarge.operator unsigned char() == 15);
    retVal &= (btMedium.operator unsigned char() == x2);
    retVal &= (btSmall.operator unsigned char() == 0);

    return retVal;
}


