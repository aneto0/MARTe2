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

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FormatDescriptorTest::FormatDescriptorTest() {
    retVal = false;
    string = NULL;
}

bool FormatDescriptorTest::TestInitialiseFromString(){
    string = "#7.9f";

    retVal = formatDescriptor.InitialiseFromString(string);
    retVal &= (formatDescriptor.desiredAction == PrintFloat);
    retVal &= (formatDescriptor.size == 7);
    retVal &= (formatDescriptor.precision == 9);
    retVal &= (formatDescriptor.padded == false);
    retVal &= (formatDescriptor.leftAligned == false);
    retVal &= (formatDescriptor.floatNotation == FixedPointNotation);
    retVal &= (formatDescriptor.binaryNotation == DecimalNotation);
    retVal &= (formatDescriptor.binaryPadded == false);
    retVal &= (formatDescriptor.fullNotation == true);
    retVal &= (formatDescriptor.desiredGrammar == 0u);

    return retVal;
}

bool FormatDescriptorTest::TestInitialiseFromString2(){
    string = "#7.af";

    retVal = !formatDescriptor.InitialiseFromString(string);

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
    retVal &= (fdDefaultConst.desiredGrammar == 0u);

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
    retVal &= (fdDefaultConst.desiredGrammar == 0u);

    return retVal;
}

bool FormatDescriptorTest::TestAssignOperator(){
    FormatDescriptor formatDescriptor1(PrintAnything, 0u, defaultPrecision, true, true, FixedPointNotation, DecimalNotation, true, true);

    formatDescriptor = formatDescriptor1;
    retVal = (formatDescriptor.padded == true);
    retVal &= (formatDescriptor.leftAligned == true);
    retVal &= (formatDescriptor.binaryPadded == true);
    retVal &= (formatDescriptor.fullNotation == true);

    return retVal;
}
