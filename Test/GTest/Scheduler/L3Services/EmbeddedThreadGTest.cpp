/**
 * @file EmbeddedThreadGTest.cpp
 * @brief Source file for class EmbeddedThreadGTest
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


#include "gtest/gtest.h"
#include "EmbeddedThreadTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(EmbeddedThreadGTest,TestConstructor) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(EmbeddedThreadGTest,TestDefaultConstructor_Template) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestDefaultConstructor_Template());
}

TEST(EmbeddedThreadGTest,TestInitialise) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(EmbeddedThreadGTest,TestInitialise_False) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestInitialise_False());
}

TEST(EmbeddedThreadGTest,TestStart) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestStart());
}

TEST(EmbeddedThreadGTest,TestStart_False) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestStart_False());
}

TEST(EmbeddedThreadGTest,TestStart_Restart) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestStart_Restart());
}

TEST(EmbeddedThreadGTest,TestStop) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestStop());
}

TEST(EmbeddedThreadGTest,TestStop_Kill) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestStop_Kill());
}

TEST(EmbeddedThreadGTest,TestThreadLoop) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestThreadLoop());
}

TEST(EmbeddedThreadGTest,TestGetStatus) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestGetStatus());
}

TEST(EmbeddedThreadGTest,TestSetTimeout) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestSetTimeout());
}

TEST(EmbeddedThreadGTest,TestGetTimeout) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestGetTimeout());
}

TEST(EmbeddedThreadGTest,TestGetThreadId) {
    EmbeddedThreadTest target;
    ASSERT_TRUE(target.TestGetThreadId());
}
