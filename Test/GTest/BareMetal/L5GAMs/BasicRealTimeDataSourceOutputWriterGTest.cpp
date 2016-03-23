/**
 * @file BasicRealTimeDataSourceOutputWriterGTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceOutputWriterGTest
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
 * the class BasicRealTimeDataSourceOutputWriterGTest (public, protected, and private). Be aware that some 
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

#include "BasicRealTimeDataSourceOutputWriterTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestConstructor) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestConstructor());
}

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestWrite_Allocation) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_Allocation());
}

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestWrite_Static) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_Static());
}

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestWrite_MoreThanOneVariable) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MoreThanOneVariable());
}

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestWrite_MultiDim_Vector) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MultiDim_Vector());
}

TEST(BasicRealTimeDataSourceOutputWriterGTest,TestWrite_MultiDim_Matrix) {
    BasicRealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MultiDim_Matrix());
}
