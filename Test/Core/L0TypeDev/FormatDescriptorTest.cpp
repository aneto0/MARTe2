/**
 * @file FormatDescriptorTest.cpp
 * @brief Source file for class FormatDescriptorTest
 * @date 01/09/2015
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
 * the class FormatDescriptorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FormatDescriptorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition;

FormatDescriptorTest::FormatDescriptorTest() {
    retVal = false;
    s = NULL;
}

FormatDescriptorTest::~FormatDescriptorTest() {
    // Auto-generated destructor stub for FormatDescriptorTest
    // TODO Verify if manual additions are needed
}

bool FormatDescriptorTest::TestInitialiseFromString(){
    s = "#7.9f";

    retVal = fd.InitialiseFromString(s);
    retVal &= (fd.desiredAction == PrintFloat);
    retVal &= (fd.size == 7);
    retVal &= (fd.precision == 9);
    retVal &= (fd.padded == false);
    retVal &= (fd.leftAligned == false);
    retVal &= (fd.floatNotation == FixedPointNotation);
    retVal &= (fd.binaryNotation == DecimalNotation);
    retVal &= (fd.binaryPadded == false);
    retVal &= (fd.fullNotation == true);
    retVal &= (fd.spareBits == 0u);

    return retVal;
}

bool FormatDescriptorTest::TestInitialiseFromString2(){
    s = "#7.af";

    retVal = !fd.InitialiseFromString(s);

    return retVal;
}

bool FormatDescriptorTest::TestDefaultConstructor(){
    FormatDescriptor fdDefaultConst;

    retVal = (fdDefaultConst.desiredAction == PrintAnything);
    retVal &= (fdDefaultConst.size == 0u);
    retVal &= (fdDefaultConst.precision == defaultPrecision);
    retVal &= (fdDefaultConst.padded == false);
    retVal &= (fdDefaultConst.leftAligned == false);
    retVal &= (fdDefaultConst.floatNotation == FixedPointNotation);
    retVal &= (fdDefaultConst.binaryNotation == DecimalNotation);
    retVal &= (fdDefaultConst.binaryPadded == false);
    retVal &= (fdDefaultConst.fullNotation == false);
    retVal &= (fdDefaultConst.spareBits == 0u);

    return retVal;
}

bool FormatDescriptorTest::TestIntConstructor(){
    const uint32 aux = 7;
    FormatDescriptor fdIntConst(aux);

    retVal = (fdIntConst.format_as_uint32 == aux);

    return retVal;
}

bool FormatDescriptorTest::TestConstructor(){
    FormatDescriptor fdDefaultConst(PrintAnything, 0u, defaultPrecision, false, false, FixedPointNotation, DecimalNotation, false, false);

    retVal = (fdDefaultConst.desiredAction == PrintAnything);
    retVal &= (fdDefaultConst.size == 0u);
    retVal &= (fdDefaultConst.precision == defaultPrecision);
    retVal &= (fdDefaultConst.padded == false);
    retVal &= (fdDefaultConst.leftAligned == false);
    retVal &= (fdDefaultConst.floatNotation == FixedPointNotation);
    retVal &= (fdDefaultConst.binaryNotation == DecimalNotation);
    retVal &= (fdDefaultConst.binaryPadded == false);
    retVal &= (fdDefaultConst.fullNotation == false);
    retVal &= (fdDefaultConst.spareBits == 0u);

    return retVal;
}

bool FormatDescriptorTest::TestAssignOperator(){
    FormatDescriptor fd;
    FormatDescriptor fd1(PrintAnything, 0u, defaultPrecision, true, true, FixedPointNotation, DecimalNotation, true, true);

    fd = fd1;
    retVal = (fd.padded == true);
    retVal &= (fd.leftAligned == true);
    retVal &= (fd.binaryPadded == true);
    retVal &= (fd.fullNotation == true);

    return retVal;
}

bool FormatDescriptorTest::TestBitWiseOrOperator(){
    FormatDescriptor fd;
    FormatDescriptor fd1(PrintAnything, 0u, defaultPrecision, true, true, FixedPointNotation, DecimalNotation, true, true);

    fd |= fd1;
    retVal = (fd.padded == true);
    retVal &= (fd.leftAligned == true);
    retVal &= (fd.binaryPadded == true);
    retVal &= (fd.fullNotation == true);

    return retVal;
}



