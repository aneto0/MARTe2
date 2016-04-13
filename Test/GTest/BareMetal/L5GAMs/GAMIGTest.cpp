/**
 * @file GAMIGTest.cpp
 * @brief Source file for class GAMIGTest
 * @date 06/04/2016
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
 * the class GAMIGTest (public, protected, and private). Be aware that some 
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
#include "GAMITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(GAMIGTest,TestConstructor) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConstructor());
}

TEST(GAMIGTest,TestInitialise) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestInitialise());
}

TEST(GAMIGTest,TestSetApplication) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestSetApplication());
}

TEST(GAMIGTest,TestSetGAMGroup) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestSetGAMGroup());
}

TEST(GAMIGTest,TestAddState) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestAddState());
}

TEST(GAMIGTest,TestAddStateFalse_MoreThreadsPerGAM) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestAddStateFalse_MoreThreadsPerGAM());
}

TEST(GAMIGTest,TestGetNumberOfSupportedStates) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetNumberOfSupportedStates());
}

TEST(GAMIGTest,TestGetNumberOfSupportedStates_GAMGroup) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetNumberOfSupportedStates_GAMGroup());
}

TEST(GAMIGTest,TestGetSupportedStates) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetSupportedStates());
}

TEST(GAMIGTest,TestGetSupportedThreads_GAMGroup) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetSupportedThreads_GAMGroup());
}

TEST(GAMIGTest,TestGetSupportedThreads) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetSupportedThreads());
}

TEST(GAMIGTest,TestGetSupportedStates_GAMGroup) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestGetSupportedStates_GAMGroup());
}

TEST(GAMIGTest,TestConfigureDataSource) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConfigureDataSource());
}

TEST(GAMIGTest,TestConfigureDataSourceFalse_Final) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConfigureDataSourceFalse_Final());
}

TEST(GAMIGTest,TestConfigureDataSourceFalse_NoData) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConfigureDataSourceFalse_NoData());
}

TEST(GAMIGTest,TestConfigureDataSourceFalse_NoApplicationSet) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConfigureDataSourceFalse_NoApplicationSet());
}

TEST(GAMIGTest,TestConfigureDataSource_NoStates) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestConfigureDataSource_NoStates());
}

TEST(GAMIGTest,TestIsSync) {
    GAMITest gamITest;
    ASSERT_TRUE(gamITest.TestIsSync());
}
