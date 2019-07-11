/**
 * @file ReferenceContainerFilterReferencesGTest.cpp
 * @brief Source file for class ReferenceContainerFilterReferencesGTest
 * @date 18/08/2015
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
 * the class ReferenceContainerFilterReferencesGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterReferencesTest.h"
#include "TestSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ReferenceContainerFilterReferencesGTest,TestDefaultConstructor) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestDefaultConstructor());
}

TEST(ReferenceContainerFilterReferencesGTest,TestFullConstructor) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestFullConstructor(5, ReferenceContainerFilterMode::REMOVE));
}

TEST(ReferenceContainerFilterReferencesGTest,TestTest) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestTest(10, 5));
}

TEST(ReferenceContainerFilterReferencesGTest,TestTestMoreOccurrences) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestTest(10, 50));
}

TEST(ReferenceContainerFilterReferencesGTest,TestTestFailure) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestTestFailure());
}

TEST(ReferenceContainerFilterReferencesGTest,TestIncrementFound) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestIncrementFound());
}

TEST(ReferenceContainerFilterReferencesGTest,TestIsRemove) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestIsRemove());
}

TEST(ReferenceContainerFilterReferencesGTest,TestIsSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestIsSearchAll());
}

TEST(ReferenceContainerFilterReferencesGTest,TestIsStorePath) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestIsStorePath());
}

TEST(ReferenceContainerFilterReferencesGTest,TestIsReverse) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestIsReverse());
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetModeNoSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetMode(1));
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetModeSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetMode(-1));
}

TEST(ReferenceContainerFilterReferencesGTest,TestReset) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestReset(20));
}


TEST(ReferenceContainerFilterReferencesGTest,TestSetGetOriginalSetOccurrenceNoChange) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetOriginalSetOccurrence(10, 20));
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetOriginalSetOccurrence) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetOriginalSetOccurrence(10, 20));
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetOriginalSetOccurrenceToSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetOriginalSetOccurrence(10, -1));
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetOriginalSetOccurrenceFromSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetOriginalSetOccurrence(-1, 10));
}

TEST(ReferenceContainerFilterReferencesGTest,TestSetGetOriginalSetOccurrenceFromToSearchAll) {
    ReferenceContainerFilterReferencesTest referenceContainerFilterReferencesTest;
    ASSERT_TRUE(referenceContainerFilterReferencesTest.TestSetGetOriginalSetOccurrence(-1, -1));
}

