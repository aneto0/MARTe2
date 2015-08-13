/**
 * @file ContainerReferenceGTest.cpp
 * @brief Source file for class ContainerReferenceGTest
 * @date 13 Aug 2015
 * @author andre
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
 * the class ContainerReferenceGTest (public, protected, and private). Be aware that some 
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

#include "ReferenceContainerTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ReferenceContainerGTest,TestConstructor) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestConstructor());
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(0u));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::PATH));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceAlwaysPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(0u));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(0u));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(0u));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(ReferenceContainerSearchMode::PATH));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::REVERSE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(ReferenceContainerSearchMode::PATH | ReferenceContainerSearchMode::RECURSIVE | ReferenceContainerSearchMode::REVERSE));
}
