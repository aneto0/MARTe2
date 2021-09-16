/**
 * @file ClassPropertiesGTest.cpp
 * @brief Source file for class ClassPropertiesGTest
 * @date 11/08/2015
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
 * the class ClassPropertiesGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassPropertiesTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestDefaultConstructor) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestDefaultConstructor());
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestFullConstructor) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestFullConstructor("Hello", "Hello", "World"));
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestReset) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestReset());
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestGetName) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestGetName("Hello"));
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestGetVersion) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestGetVersion("World"));
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestGetUniqueIdentifier) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestGetUniqueIdentifier(0xff2));
}

TEST(BareMetal_L2Objects_ClassPropertiesGTest,TestSetUniqueIdentifier) {
    ClassPropertiesTest myClassPropertiesTest;
    ASSERT_TRUE(myClassPropertiesTest.TestSetUniqueIdentifier(0xff3));
}

