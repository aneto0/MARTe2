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

/***********************************
 * TestFindFirstOccurrenceAlways   *
 ***********************************/
TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceObject) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

/*****************************************
 * TestFindFirstOccurrenceBranchAlways   *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindFirstOccurrenceBranchAlwaysReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceBranchAlwaysReferenceObject) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

/*****************************
 * TestFindFirstOccurrence   *
 *****************************/
TEST(ReferenceContainerGTest,TestFindFirstOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

/*****************************
 * TestFindSecondOccurrence  *
 *****************************/
TEST(ReferenceContainerGTest,TestFindSecondOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

/****************************
 * TestFindThirdOccurrence  *
 ****************************/
TEST(ReferenceContainerGTest,TestFindThirdOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(3, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

/********************************
 * TestFindAllOfASingleInstance *
 ********************************/
TEST(ReferenceContainerGTest,TestFindAllOfASingleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindAllOfASingleInstanceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

/*********************************
 * TestFindAllOfMultipleInstance *
 *********************************/
TEST(ReferenceContainerGTest,TestFindAllOfMultipleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindAllOfMultipleInstanceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

/*****************************************
 * TestFindReferenceNonExistent *
 *****************************************/
TEST(ReferenceContainerGTest,TestFindNonExistentReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(ReferenceContainerGTest,TestFindNonExistentObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-1, 0u, "NoExist");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

/*********************************************
 * TestFindReferenceOutOfBoundsOccurrence    *
 *********************************************/
TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceReferenceFilterNegative) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceReferenceFilterPositive) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectFilterNegative) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(-2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectFilterPositive) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(4, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/***********************
 * TestFindEmptyTree   *
 ***********************/
TEST(ReferenceContainerGTest,TestFindEmptyTreeReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindEmptyTree(filter));
}

TEST(ReferenceContainerGTest,TestFindEmptyTreeObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjects filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindEmptyTree(filter));
}


/***********************************************
 * TestFindObjectPath                          *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindPathLegalObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectFilter());
}

TEST(ReferenceContainerGTest,TestFindPathLegalObjectFilterStartDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectFilterStartDot());
}

TEST(ReferenceContainerGTest,TestFindPathLegalObjectFilterEndDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectFilterEndDot());
}

TEST(ReferenceContainerGTest,TestFindPathIllegalObjectFilterTwoDots) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathIllegalObjectFilterTwoDots());
}

/***********************
 * TestFindPerformance *
 ***********************/
/*TEST(ReferenceContainerGTest,TestFindPerformanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceT<ReferenceContainer> largeTree = referenceContainerTest.GenerateTestTreeLarge(5000);
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafU3);
    ASSERT_TRUE(referenceContainerTest.TestFindPerformance(largeTree, filter));
}*/

TEST(ReferenceContainerGTest,TestFindPerformanceObjectFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceT<ReferenceContainer> largeTree = referenceContainerTest.GenerateTestTreeLarge(5000);
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "U3");
    ASSERT_TRUE(referenceContainerTest.TestFindPerformance(largeTree, filter));
}
