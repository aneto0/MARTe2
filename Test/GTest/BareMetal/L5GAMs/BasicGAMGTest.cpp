/**
 * @file BasicGAMGTest.cpp
 * @brief Source file for class BasicGAMGTest
 * @date 30 Mar 2016 TODO Verify the value and format of the date
 * @author perezfr TODO Verify the name and format of the author
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
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConstructor());
}

TEST(BasicGAMGTest,TestInitialise) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestInitialise());
}

TEST(BasicGAMGTest,TestSetApplication) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestSetApplication());
}

TEST(BasicGAMGTest,TestSetGAMGroup) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestSetGAMGroup());
}

TEST(BasicGAMGTest,TestAddState) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestAddState());
}

TEST(BasicGAMGTest,TestGetNumberOfSupportedStates) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestGetNumberOfSupportedStates());
}

TEST(BasicGAMGTest,TestGetSupportedStates) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestGetSupportedStates());
}

TEST(BasicGAMGTest,TestGetNumberOfSupportedStates_GAMGroup) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestGetNumberOfSupportedStates_GAMGroup());
}

TEST(BasicGAMGTest,TestGetSupportedStates_GAMGroup) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestGetSupportedStates_GAMGroup());
}

TEST(BasicGAMGTest,TestConfigureDataSource) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSource());
}

TEST(BasicGAMGTest,TestConfigureDataSourceFalse_NoData) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSourceFalse_NoData());
}

TEST(BasicGAMGTest,TestConfigureDataSourceFalse_NoApplicationSet) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSourceFalse_NoApplicationSet());
}

TEST(BasicGAMGTest,TestConfigureDataSource_NoStates) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSource_NoStates());
}

TEST(BasicGAMGTest,TestConfigureDataSourceFalse_Final) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSourceFalse_Final());
}

TEST(BasicGAMGTest,TestConfigureDataSourceLinks) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestConfigureDataSourceLinks());
}

TEST(BasicGAMGTest,TestExecute) {
    BasicGAMTest myBasicGAM;
    ASSERT_TRUE(myBasicGAM.TestExecute());
}
	
