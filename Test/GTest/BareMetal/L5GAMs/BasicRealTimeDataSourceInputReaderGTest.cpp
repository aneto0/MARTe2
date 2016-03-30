/**
 * @file BasicRealTimeDataSourceInputReaderGTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceInputReaderGTest
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
 * the class BasicRealTimeDataSourceInputReaderGTest (public, protected, and private). Be aware that some 
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

#include "BasicRealTimeDataSourceInputReaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*TEST(BasicRealTimeDataSourceInputReaderGTest,TestConstructor) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestConstructor());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Static) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_DefaultValues_Static());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Allocation) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_DefaultValues_Allocation());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_Static) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_Static());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_Allocation) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_Allocation());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MoreThanOneVariable) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_MoreThanOneVariable());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MultiDim_Vector) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_MultiDim_Vector());
}
*/
TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MultiDim_Matrix) {
    BasicRealTimeDataSourceInputReaderTest myBasicInputReaderTest;
    ASSERT_TRUE(myBasicInputReaderTest.TestRead_MultiDim_Matrix());
}

	
