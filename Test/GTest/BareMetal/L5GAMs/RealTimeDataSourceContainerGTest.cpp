/**
 * @file RealTimeDataSourceContainerGTest.cpp
 * @brief Source file for class RealTimeDataSourceContainerGTest
 * @date 21/03/2016
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
 * the class RealTimeDataSourceContainerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSourceContainerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceContainerGTest,TestConstructor) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestConstructor());
}

TEST(RealTimeDataSourceContainerGTest,TestInitialise) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestInitialise());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinition_Final) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinition_Final());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinitionFalse_Final) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinitionFalse_Final());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinition_AlreadyExistentLeaf) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinition_AlreadyExistentLeaf());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinitionFalse_NoDefinitionInGAM) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinitionFalse_NoDefinitionInGAM());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinitionFalse_NoDefinitionContainerInGAM) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinitionFalse_NoDefinitionContainerInGAM());
}

TEST(RealTimeDataSourceContainerGTest,TestAddDataDefinitionFalse_InvalidGAM) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinitionFalse_InvalidGAM());
}

TEST(RealTimeDataSourceContainerGTest,TestVerify) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestVerify());
}

TEST(RealTimeDataSourceContainerGTest,TestVerify_TwoProducers) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestVerify_TwoProducers());
}

TEST(RealTimeDataSourceContainerGTest,TestAllocate) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAllocate());
}

TEST(RealTimeDataSourceContainerGTest,TestAllocate_Structure) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAllocate_Structure());
}

TEST(RealTimeDataSourceContainerGTest,TestPrepareNextState) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestPrepareNextState());
}

TEST(RealTimeDataSourceContainerGTest,TestPrepareNextState_MultiDim_Vector) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestPrepareNextState_MultiDim_Vector());
}


TEST(RealTimeDataSourceContainerGTest,TestPrepareNextState_MultiDim_Vector_Matrix) {
    RealTimeDataSourceContainerTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestPrepareNextState_MultiDim_Matrix());
}


	
