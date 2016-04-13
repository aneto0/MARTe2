/**
 * @file GAMSchedulerIGTest.cpp
 * @brief Source file for class GAMSchedulerIGTest
 * @date 04/apr/2016
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
 * the class GAMSchedulerIGTest (public, protected, and private). Be aware that some 
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
#include "GAMSchedulerITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(GAMSchedulerIGTest,TestConstructor) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestConstructor());
}

TEST(GAMSchedulerIGTest,TestInsertRecord) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestInsertRecord());
}

TEST(GAMSchedulerIGTest,TestPrepareNextState) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestPrepareNextState());
}

TEST(GAMSchedulerIGTest,TestPrepareNextStateFalse_InvalidNextState) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestPrepareNextStateFalse_InvalidNextState());
}

TEST(GAMSchedulerIGTest,TestPrepareNextStateFalse_NoAppSet) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestPrepareNextStateFalse_NoAppSet());
}

TEST(GAMSchedulerIGTest,TestSetApplication) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestSetApplication());
}

TEST(GAMSchedulerIGTest,TestChangeState) {
    GAMSchedulerITest schedTest;
    ASSERT_TRUE(schedTest.TestChangeState());
}
