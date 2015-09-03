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

using namespace TypeDefinition;

TypeCharacteristicsTest::TypeCharacteristicsTest() {
    retVal = false;
}

TypeCharacteristicsTest::~TypeCharacteristicsTest() {
    // Auto-generated destructor stub for TypeCharacteristicsTest
    // TODO Verify if manual additions are needed
}

bool TypeCharacteristicsTest::TestIsSignedInt8(){
    TypeCharacteristics <int8> tc;
    retVal = tc.IsSigned();
    return retVal;
}

bool TypeCharacteristicsTest::TestIsSignedUInt32(){
    TypeCharacteristics <uint32> tc;
    retVal = !tc.IsSigned();
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueInt8(){
    TypeCharacteristics <int8> tc;
    retVal = (tc.MaxValue() == 127);
    return retVal;
}

bool TypeCharacteristicsTest::TestMaxValueUInt32(){
    TypeCharacteristics <uint32> tc;
    retVal = (tc.MaxValue() == (2^32));
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueInt8(){
    TypeCharacteristics <int8> tc;
    retVal = (tc.MinValue() == -128);
    return retVal;
}

bool TypeCharacteristicsTest::TestMinValueUInt32(){
    TypeCharacteristics <uint32> tc;
    retVal = (tc.MinValue() == 0);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeInt8(){
    TypeCharacteristics <int8> tc;
    retVal = (tc.UsableBitSize() == 7);
    return retVal;
}

bool TypeCharacteristicsTest::TestUsableBitSizeUInt32(){
    TypeCharacteristics <uint32> tc;
    retVal = (tc.UsableBitSize() == 32);
    return retVal;
}


