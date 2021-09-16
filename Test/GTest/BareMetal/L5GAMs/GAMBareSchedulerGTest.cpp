/**
 * @file GAMBareSchedulerGTest.cpp
 * @brief Source file for class GAMBareSchedulerGTest
 * @date 18/06/2021
 * @author Giuseppe Avon
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
 * the class GAMBareSchedulerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GAMBareSchedulerTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L5GAMs_GAMBareScheduler, TestConstructor) {
    GAMBareSchedulerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_GAMBareScheduler, TestInitialise) {
    GAMBareSchedulerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BareMetal_L5GAMs_GAMBareScheduler, TestConfigureScheduler) {
    GAMBareSchedulerTest test;
    ASSERT_TRUE(test.TestConfigureScheduler());
}

TEST(BareMetal_L5GAMs_GAMBareScheduler, TestStartNextStateExecution) {
    GAMBareSchedulerTest test;
    ASSERT_TRUE(test.TestStartCurrentStateExecution());
}

TEST(BareMetal_L5GAMs_GAMBareScheduler, TestStopCurrentStateExecution) {
    GAMBareSchedulerTest test;
    ASSERT_TRUE(test.TestStopCurrentStateExecution());
}

