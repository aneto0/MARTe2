/**
 * @file TypeCharacteristicsTest.cpp
 * @brief Source file for class TypeCharacteristicsTest
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
 * the class TypeCharacteristicsTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristicsTest.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


TypeCharacteristicsTest::TypeCharacteristicsTest() {
    retVal = false;
}

bool TypeCharacteristicsTest::TestIsFloatInt8(){
    retVal = !TypeCharacteristics<int8>::IsFloat();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsFloatFloat32(){
    retVal = TypeCharacteristics<float32>::IsFloat();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsFloatFloat64(){
    retVal = TypeCharacteristics<float64>::IsFloat();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsSignedInt8(){
    retVal = TypeCharacteristics<int8>::IsSigned();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsSignedUInt32(){
    retVal = !TypeCharacteristics<uint32>::IsSigned();
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueInt8(){
    retVal = (TypeCharacteristics<int8>::MaxValue() == 127);
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueInt8BZ2(){
    retVal = (TypeCharacteristics<FractionalInteger<int8, 2> >::MaxValue<2>() == 1);
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueUInt32(){
    uint32 maxUInt = 4294967295; //(2^32)-1
    retVal = (TypeCharacteristics<uint32>::MaxValue() == maxUInt);
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueUInt32BZ2(){
    uint32 maxUInt = 3; //(2^2)-1
    retVal = (TypeCharacteristics<FractionalInteger<uint32, 2> >::MaxValue<2>() == maxUInt);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueInt8(){
    retVal = (TypeCharacteristics<int8>::MinValue() == -128);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueInt8BZ2(){
    retVal = (TypeCharacteristics<FractionalInteger<int8, 2> >::MinValue<2>() == -2);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueUInt32(){
    retVal = (TypeCharacteristics<uint32>::MinValue() == 0);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueUInt32BZ2(){
    retVal = (TypeCharacteristics<FractionalInteger<uint32, 2> >::MinValue() == 0);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeInt8(){
    retVal = (TypeCharacteristics<int8>::UsableBitSize() == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeInt8BZ(){
    retVal = (TypeCharacteristics<FractionalInteger<int8,2> >::UsableBitSize<2>() == 1);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeUInt32(){
    retVal = (TypeCharacteristics<uint32>::UsableBitSize() == 32);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeUInt32BZ2(){
    retVal = (TypeCharacteristics<FractionalInteger<uint32,2> >::UsableBitSize<2>() == 2);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableNegativeBitSizeInt8(){
    retVal = (TypeCharacteristics<int8>::UsableNegativeBitSize() == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableNegativeBitSizeInt32(){
    retVal = (TypeCharacteristics<int32>::UsableNegativeBitSize() == 31);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableNegativeBitSizeUInt32(){
    retVal = (TypeCharacteristics<uint32>::UsableNegativeBitSize() == 0);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableNegativeBitSizeFloat32(){
    retVal = (TypeCharacteristics<float32>::UsableNegativeBitSize() == FLT_MAX_EXP);
    return retVal;
}

bool TypeCharacteristicsTest::TestSaturableIntegerNoChanges(){
    retVal = (SaturateInteger<uint32,uint32,32>(10) == 10);
    return retVal;
}

bool TypeCharacteristicsTest::TestSaturableIntegerLargerThan(){
    retVal = (SaturateInteger<uint32,uint32, 3>(10) == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestSaturableIntegerLargerThan2(){
    retVal = (SaturateInteger<uint16,uint32, 3>(10) == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestSaturableIntegerLessThan(){
    retVal = (SaturateInteger<int32,int32, 3>(-10) == -4);
    return retVal;
}

bool TypeCharacteristicsTest::TestSaturableIntegerLessThan2(){
    retVal = (SaturateInteger<int16,int32, 3>(-10) == -4);
    return retVal;
}

bool TypeCharacteristicsTest::TestEpsilon(){
    bool retVal = false;
    
    float32 epsilon32 = TypeCharacteristics<float32>::Epsilon();
    float64 epsilon64 = TypeCharacteristics<float64>::Epsilon();
    
    float32 testValue32 = 1.0f;
    float64 testValue64 = 1.0;
    
    retVal = (epsilon32 < 0.000000120f && testValue32 - epsilon32 != 1.0f && testValue32 + epsilon32 != 1.0f);
    if (retVal) {
        retVal = (epsilon64 < 0.000000000000000223 && testValue64 - epsilon64 != 1.0 && testValue64 + epsilon64 != 1.0);
    }
    
    return retVal;
}

