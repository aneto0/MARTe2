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
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjects.h"
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

TEST(ReferenceContainerGTest,TestGetTimeoutInfinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(TTInfiniteWait));
}

TEST(ReferenceContainerGTest,TestGetTimeoutFinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(1));
}

TEST(ReferenceContainerGTest,TestSetTimeoutInfinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(TTInfiniteWait));
}

TEST(ReferenceContainerGTest,TestSetTimeoutFinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSetTimeout(1));
}

/********************************************
 * TestFindReferenceFirstOccurrenceAlways   *
 ********************************************/
TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

/********************************************
 * TestFindReferenceFirstOccurrenceBranch   *
 ********************************************/
TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceBranchAlwaysPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

/**************************************
 * TestFindReferenceFirstOccurrence   *
 **************************************/
TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE,
                                              referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

/**************************************
 * TestFindReferenceSecondOccurrence  *
 **************************************/
TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceSecondOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

/**************************************
 * TestFindReferenceThirdOccurrence  *
 **************************************/
TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceThirdOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            3, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

/*****************************************
 * TestFindReferenceAllOfASingleInstance *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstanceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstanceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstanceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstanceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstancePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstancePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstancePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfASingleInstancePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            -1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

/*****************************************
 * TestFindReferenceAllOfMultipleInstance *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstanceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstanceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstanceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstanceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstancePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstancePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstancePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceAllOfMultipleInstancePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            -1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

/*****************************************
 * TestFindReferenceNonExistent *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindReferenceNonExistentNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafNoExist);
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE,
                                              referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceNonExistentPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            -1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

/*********************************************
 * TestFindReferenceOutOfBoundsOccurrenceTop *
 *********************************************/
TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceTopPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            4, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/*****************************************
 * TestFindReferenceOutOfBoundsOccurrenceNegative *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativeNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativeNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativeNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativeNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                              referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::PATH, referenceContainerTest.leafH);
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceOutOfBoundsOccurrenceNegativePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(
            -2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
            referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/**************************************
 * TestFindObjectFirstOccurrenceAlways   *
 **************************************/
TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceAlwaysPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

/**************************************
 * TestFindObjectFirstOccurrenceBranch   *
 **************************************/
TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceBranchAlwaysPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

/**************************************
 * TestFindObjectFirstOccurrence      *
 **************************************/
TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectFirstOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

/**************************************
 * TestFindObjectSecondOccurrence  *
 **************************************/
TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectSecondOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

/**************************************
 * TestFindObjectThirdOccurrence  *
 **************************************/
TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrenceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrenceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrenceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrenceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrencePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrencePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrencePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectThirdOccurrencePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

/*****************************************
 * TestFindObjectAllOfASingleInstance *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstanceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstanceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::REVERSE, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstanceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstanceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstancePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH, "G");
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstancePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstancePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfASingleInstancePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

/*****************************************
 * TestFindObjectAllOfMultipleInstance *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstanceNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstanceNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstanceNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstanceNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstancePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstancePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstancePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectAllOfMultipleInstancePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

/*****************************************
 * TestFindReferenceNonExistent          *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindObjectNonExistentNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::REVERSE, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectNonExistentPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "NonExistent");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

/*****************************************
 * TestFindObjectOutOfBoundsOccurrenceTop      *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceTopPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/***********************************************
 * TestFindObjectOutOfBoundsOccurrenceNegative *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativeNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativeNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativeNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativeNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativePathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::PATH, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativePathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativePathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindObjectOutOfBoundsOccurrenceNegativePathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2,
                                           ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE,
                                           "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/***********************************************
 * TestFindObjectPathDCEH                      *
 ***********************************************/
static void CreateDCEHPathExpectedResult(ReferenceContainerTest &referenceContainerTest,
                                         ReferenceContainer &expectedResult) {
    expectedResult.Insert(referenceContainerTest.leafH);
    expectedResult.Insert(referenceContainerTest.containerE);
    expectedResult.Insert(referenceContainerTest.containerC);
    expectedResult.Insert(referenceContainerTest.containerD);
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathDCEHPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

/***********************************************
 * TestFindObjectPathStartDotDCEH              *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathStartDotDCEHPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, ".D.C.E.H");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

/***********************************************
 * TestFindObjectPathEndDotDCEH              *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathEndDotDCEHPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "D.C.E.H.");
    ReferenceContainer expectedResult;
    CreateDCEHPathExpectedResult(referenceContainerTest, expectedResult);
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

/***********************************************
 * TestFindObjectPathTwoDotsDCEH               *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHNoPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHNoPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHNoPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHNoPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHPathNoRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHPathNoRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHPathRecursiveNoReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}

TEST(ReferenceContainerGTest,TestFindObjectPathTwoDotsDCEHPathRecursiveReverse) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "D.C.E..H");
    ReferenceContainer expectedResult;
    ASSERT_TRUE(referenceContainerTest.TestFindPath(filter, expectedResult));
}
