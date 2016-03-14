/**
 * @file RealTimeDataSourceGTest.cpp
 * @brief Source file for class RealTimeDataSourceGTest
 * @date 09/03/2016
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
 * the class RealTimeDataSourceGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataSourceTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceGTest,TestConstructor) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestConstructor());
}

TEST(RealTimeDataSourceGTest,TestInitialise) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestInitialise());
}

TEST(RealTimeDataSourceGTest,TestAddDataDefinition_Final) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinition_Final());
}

TEST(RealTimeDataSourceGTest,TestAddDataDefinitionFalse_Final) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinitionFalse_Final());
}

TEST(RealTimeDataSourceGTest,TestAddDataDefinition_NotFinal) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinition_NotFinal());
}

TEST(RealTimeDataSourceGTest,TestAddDataDefinition_AlreadyExistentLeaf) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAddDataDefinition_AlreadyExistentLeaf());
}

TEST(RealTimeDataSourceGTest,TestVerify) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestVerify());
}

TEST(RealTimeDataSourceGTest,TestVerify_TwoProducers) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestVerify_TwoProducers());
}

TEST(RealTimeDataSourceGTest,TestAllocate) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAllocate());
}

TEST(RealTimeDataSourceGTest,TestAllocate_Structure) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestAllocate_Structure());
}

TEST(RealTimeDataSourceGTest,TestPrepareNextState) {
    RealTimeDataSourceTest rtdsdcTest;
    ASSERT_TRUE(rtdsdcTest.TestPrepareNextState());
}
