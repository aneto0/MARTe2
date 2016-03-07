/**
 * @file GAMGroupGTest.cpp
 * @brief Source file for class GAMGroupGTest
 * @date 07/mar/2016
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
 * the class GAMGroupGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GAMGroupTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(GAMGroupGTest,TestConstructor) {
    GAMGroupTest gamGroupTest;
    ASSERT_TRUE(gamGroupTest.TestConstructor());
}

TEST(GAMGroupGTest,TestAddState) {
    GAMGroupTest gamGroupTest;
    ASSERT_TRUE(gamGroupTest.TestAddState());
}

TEST(GAMGroupGTest,TestGetSupportedStates) {
    GAMGroupTest gamGroupTest;
    ASSERT_TRUE(gamGroupTest.TestGetSupportedStates());
}

TEST(GAMGroupGTest,TestGetNumberOfSupportedStates) {
    GAMGroupTest gamGroupTest;
    ASSERT_TRUE(gamGroupTest.TestGetNumberOfSupportedStates());
}
TEST(GAMGroupGTest,TestInitialise) {
    GAMGroupTest gamGroupTest;
    ASSERT_TRUE(gamGroupTest.TestInitialise());
}
