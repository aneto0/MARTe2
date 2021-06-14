/**
 * @file FormatDescriptorGTest.cpp
 * @brief Source file for class FormatDescriptorGTest
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
 * the class FormatDescriptorGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "FormatDescriptorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestInitialiseFromString) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestInitialiseFromString());
}

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestInitialiseFromString2) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestInitialiseFromString2());
}

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestDefaultConstructor) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestDefaultConstructor());
}

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestIntConstructor) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestIntConstructor());
}

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestConstructor) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestConstructor());
}

TEST(BareMetal_L1Portability_FormatDescriptorGTest,TestAssignOperator) {
    FormatDescriptorTest MyFormatDEscriptorTest;
    ASSERT_TRUE(MyFormatDEscriptorTest.TestAssignOperator());
}


	
