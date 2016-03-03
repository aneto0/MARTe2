/**
 * @file RealTimeGenericDataDefGTest.cpp
 * @brief Source file for class RealTimeGenericDataDefGTest
 * @date 03/mar/2016
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
 * the class RealTimeGenericDataDefGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeGenericDataDefTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeGenericDataDefGTest,TestConstructor) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestConstructor());
}

TEST(RealTimeGenericDataDefGTest,TestInitialiseTrue) {
    RealTimeGenericDataDefTest rtgdTest;
    ConfigurationDatabase data;
    data.Write("IsFinal", "true");
    data.Write("Default", "1");

    ASSERT_TRUE(rtgdTest.TestInitialise(data, true));
}

TEST(RealTimeGenericDataDefGTest,TestInitialiseFalseNoIsFinalFlag) {
    RealTimeGenericDataDefTest rtgdTest;
    ConfigurationDatabase data;
    data.Write("Default", "1");

    ASSERT_TRUE(rtgdTest.TestInitialise(data, false));
}

TEST(RealTimeGenericDataDefGTest,TestInitialiseTrueNoDefault) {
    RealTimeGenericDataDefTest rtgdTest;
    ConfigurationDatabase data;
    data.Write("IsFinal", "true");

    ASSERT_TRUE(rtgdTest.TestInitialise(data, true));
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal());
}
