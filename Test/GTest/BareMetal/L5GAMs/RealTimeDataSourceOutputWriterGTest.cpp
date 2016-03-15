/**
 * @file RealTimeDataSourceOutputWriterGTest.cpp
 * @brief Source file for class RealTimeDataSourceOutputWriterGTest
 * @date 14/03/2016
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
 * the class RealTimeDataSourceOutputWriterGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataSourceOutputWriterTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceOutputWriterGTest,TestConstructor) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestConstructor());
}

TEST(RealTimeDataSourceOutputWriterGTest,TestWrite_Allocation) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_Allocation());
}

TEST(RealTimeDataSourceOutputWriterGTest,TestWrite_Static) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_Static());
}

TEST(RealTimeDataSourceOutputWriterGTest,TestWrite_MoreThanOneVariable) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MoreThanOneVariable());
}

TEST(RealTimeDataSourceOutputWriterGTest,TestWrite_MultiDim_Vector) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MultiDim_Vector());
}

TEST(RealTimeDataSourceOutputWriterGTest,TestWrite_MultiDim_Matrix) {
    RealTimeDataSourceOutputWriterTest rtdsorTest;
    ASSERT_TRUE(rtdsorTest.TestWrite_MultiDim_Matrix());
}
