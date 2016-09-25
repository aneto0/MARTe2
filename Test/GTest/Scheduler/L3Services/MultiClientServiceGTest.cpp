/**
 * @file MultiClientServiceGTest.cpp
 * @brief Source file for class MultiClientServiceGTest
 * @date 19/09/2016
 * @author Andre Neto
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
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include <MultiClientServiceTest.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MultiClientServiceGTest,TestConstructor) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(MultiClientServiceGTest,TestInitialise) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(MultiClientServiceGTest,TestInitialise_False_Timeout) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_Timeout());
}

TEST(MultiClientServiceGTest,TestInitialise_False_MaxNumberOfThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MaxNumberOfThreads());
}

TEST(MultiClientServiceGTest,TestInitialise_False_MinNumberOfThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinNumberOfThreads());
}

TEST(MultiClientServiceGTest,TestInitialise_False_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinGreaterThanMax());
}

TEST(MultiClientServiceGTest,TestInitialise_False_MinNumberOfThreads_0) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestInitialise_False_MinNumberOfThreads_0());
}

TEST(MultiClientServiceGTest,TestStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(MultiClientServiceGTest,TestStart_False) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(MultiClientServiceGTest,TestStart_Restart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(MultiClientServiceGTest,TestStop) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(MultiClientServiceGTest,TestStop_Kill) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(MultiClientServiceGTest,TestGetMinimumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetMinimumNumberOfPoolThreads());
}

TEST(MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads());
}

TEST(MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_0) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_0());
}

TEST(MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_AfterStart());
}

TEST(MultiClientServiceGTest,TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMinimumNumberOfPoolThreads_MinGreaterThanMax());
}

TEST(MultiClientServiceGTest,TestGetMaximumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetMaximumNumberOfPoolThreads());
}

TEST(MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads());
}

TEST(MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads_AfterStart) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads_AfterStart());
}

TEST(MultiClientServiceGTest,TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestSetMaximumNumberOfPoolThreads_MinGreaterThanMax());
}

TEST(MultiClientServiceGTest,TestTestGetNumberOfActiveThreads) {
    MultiClientServiceTest target;
    ASSERT_TRUE(target.TestGetNumberOfActiveThreads());
}
