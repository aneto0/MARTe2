/**
 * @file RealTimeDataSourceInputReaderGTest.cpp
 * @brief Source file for class RealTimeDataSourceInputReaderGTest
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
 * the class RealTimeDataSourceInputReaderGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataSourceInputReaderTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataSourceInputReaderGTest,TestConstructor) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestConstructor());
}

TEST(RealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Static) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_DefaultValues_Static());
}

TEST(RealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Allocation) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_DefaultValues_Allocation());
}

TEST(RealTimeDataSourceInputReaderGTest,TestRead_Static) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_Static());
}

TEST(RealTimeDataSourceInputReaderGTest,TestRead_Allocation) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_Allocation());
}

TEST(RealTimeDataSourceInputReaderGTest,TestRead_MoreThanOneVariable) {
    RealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_MoreThanOneVariable());
}
