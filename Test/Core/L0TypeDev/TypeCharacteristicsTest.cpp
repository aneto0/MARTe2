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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition::TypeCharacteristics;


TypeCharacteristicsTest::TypeCharacteristicsTest() {
    retVal = false;
}

bool TypeCharacteristicsTest::TestIsSignedInt8(){

    retVal = IsSigned<int8>();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsSignedUInt32(){
    retVal = !IsSigned<uint32>();
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueInt8(){
    retVal = (MaxValue<int8>() == 127);
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueUInt32(){
    uint32 maxUInt = 4294967295; //(2^32)-1
    retVal = (MaxValue<uint32>() == maxUInt);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueInt8(){
    retVal = (MinValue<int8>() == -128);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueUInt32(){
    retVal = (MinValue<uint32>() == 0);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeInt8(){
    retVal = (UsableBitSize<int8>() == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeUInt32(){
    retVal = (UsableBitSize<uint32>() == 32);
    return retVal;
}

