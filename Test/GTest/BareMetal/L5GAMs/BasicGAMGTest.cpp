/**
 * @file BasicGAMGTest.cpp
 * @brief Source file for class BasicGAMGTest
 * @date 22/mar/2016
 * @author pc
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
 * the class BasicGAMGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicGAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicGAMGTest,TestConstructor) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConstructor());
}

TEST(BasicGAMGTest,TestConfigureDataSourceLinks) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSourceLinks());
}

TEST(BasicGAMGTest,TestIsSync) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestIsSync());
}

TEST(BasicGAMGTest,TestGetInputReader) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetInputReader());
}

TEST(BasicGAMGTest,TestGetOutputWriter) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetOutputWriter());
}

TEST(BasicGAMGTest,TestExecute) {
    BasicGAMTest gamTest;
    ASSERT_TRUE(gamTest.TestExecute());
}
