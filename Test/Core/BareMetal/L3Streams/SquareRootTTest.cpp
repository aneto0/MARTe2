/**
 * @file SquareRootTTest.cpp
 * @brief Source file for class SquareRootTTest
 * @date 20/04/2017
 * @author Andre' Torres
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
 * the class SquareRootTTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SquareRootTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool SquareRootTTest::TestUint8(const uint8 testVal,const  uint8 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint8>(testVal));
}


bool SquareRootTTest::TestUint16(const uint16 testVal,const  uint16 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint16>(testVal));
}


bool SquareRootTTest::TestUint32(const uint32 testVal,const  uint32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint32>(testVal));
}


bool SquareRootTTest::TestUint64(const uint64 testVal,const  uint64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<uint64>(testVal));
}

bool SquareRootTTest::TestInt8(const int8 testVal,const  int8 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int8>(testVal));
}

bool SquareRootTTest::TestInt16(const int16 testVal,const  int16 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int16>(testVal));
}

bool SquareRootTTest::TestInt32(const int32 testVal,const  int32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int32>(testVal));
}

bool SquareRootTTest::TestInt64(const int64 testVal,const  int64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<int64>(testVal));
}

bool SquareRootTTest::TestFloat32(const float32 testVal,const  float32 expectedRes){
    return (expectedRes==FastMath::SquareRoot<float32>(testVal));
}

bool SquareRootTTest::TestFloat64(const float64 testVal,const  float64 expectedRes){
    return (expectedRes==FastMath::SquareRoot<float64>(testVal));
}


} /*namespace MARTe*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



	
