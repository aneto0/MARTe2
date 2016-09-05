/**
 * @file ClassMethodCallerGTest.cpp
 * @brief Source file for class ClassMethodCallerGTest
 * @date 27/06/2016
 * @author Ivan Herrero
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
 * the class ClassMethodCallerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCallerTTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ClassMethodCallerTGTest,TestDefaultConstructor) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(ClassMethodCallerTGTest,TestCall_VoidParameters) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_VoidParameters());
}

TEST(ClassMethodCallerTGTest,TestCall_OneBasicParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneBasicParameterReadOnly());
}

TEST(ClassMethodCallerTGTest,TestCall_OneBasicParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneBasicParameterReadWrite());
}

TEST(ClassMethodCallerTGTest,TestCall_OneStructuredDataIParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStructuredDataIParameterReadOnly());
}

TEST(ClassMethodCallerTGTest,TestCall_OneStructuredDataIParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneStructuredDataIParameterReadWrite());
}

TEST(ClassMethodCallerTGTest,TestCall_OneReferenceContainerParameterReadOnly) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneReferenceContainerParameterReadOnly());
}

TEST(ClassMethodCallerTGTest,TestCall_OneReferenceContainerParameterReadWrite) {
    ClassMethodCallerTTest target;
    ASSERT_TRUE(target.TestCall_OneReferenceContainerParameterReadWrite());
}

