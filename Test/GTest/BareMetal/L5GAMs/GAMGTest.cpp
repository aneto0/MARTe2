/**
 * @file GAMGTest.cpp
 * @brief Source file for class GAMGTest
 * @date 18/02/2016
 * @author Giuseppe Ferrò
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
 * the class GAMGTest (public, protected, and private). Be aware that some 
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
#include "GAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(GAMGTest,TestConstructor) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConstructor());
}

TEST(GAMGTest,TestInitialise) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestInitialise());
}

TEST(GAMGTest,TestSetApplication) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestSetApplication());
}

TEST(GAMGTest,TestSetGAMGroup) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestSetGAMGroup());
}

TEST(GAMGTest,TestAddState) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestAddState());
}

TEST(GAMGTest,TestGetNumberOfSupportedStates) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetNumberOfSupportedStates());
}

TEST(GAMGTest,TestGetSupportedStates) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetSupportedStates());
}

TEST(GAMGTest,TestGetNumberOfSupportedStates_GAMGroup) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetNumberOfSupportedStates_GAMGroup());
}

TEST(GAMGTest,TestGetSupportedStates_GAMGroup) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestGetSupportedStates_GAMGroup());
}

TEST(GAMGTest,TestConfigureDataSource) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSource());
}

TEST(GAMGTest,TestConfigureDataSource_NotFinal) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSource_NotFinal());
}

TEST(GAMGTest,TestConfigureDataSourceFalse_Final) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSourceFalse_Final());
}

TEST(GAMGTest,TestConfigureDataSourceFalse_NoData) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSourceFalse_NoData());
}

TEST(GAMGTest,TestConfigureDataSourceFalse_NoApplicationSet) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSourceFalse_NoApplicationSet());
}

TEST(GAMGTest,TestConfigureDataSource_NoStates) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSource_NoStates());
}

TEST(GAMGTest,TestConfigureDataSourceLinks) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestConfigureDataSourceLinks());
}

TEST(GAMGTest,TestExecute) {
    GAMTest gamTest;
    ASSERT_TRUE(gamTest.TestExecute());
}

