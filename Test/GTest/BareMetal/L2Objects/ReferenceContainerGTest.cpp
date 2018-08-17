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
#include <ReferenceContainerFilterObjectName.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerTest.h"
#include "ReferenceContainerFilterReferences.h"
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

TEST(ReferenceContainerGTest,TestCopyConstructor) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestCopyConstructor());
}

TEST(ReferenceContainerGTest,TestOperatorEqual) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestOperatorEqual());
}

//TEST(ReferenceContainerGTest,TestGetClassPropertiesCopy) {
//    ReferenceContainerTest referenceContainerTest;
//    ASSERT_TRUE(referenceContainerTest.TestGetClassPropertiesCopy());
//}

TEST(ReferenceContainerGTest,TestGetClassProperties) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetClassProperties());
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
/*TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}*/

TEST(ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceObject) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "B");
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
    ReferenceContainerFilterObjectName filter(1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}


/*****************************
 * TestFind  *
 *****************************/

TEST(ReferenceContainerGTest,TestFind) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFind());
}
/*****************************
 * TestFindFirstOccurrence   *
 *****************************/
TEST(ReferenceContainerGTest,TestFindFirstOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindFirstOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "E");
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

TEST(ReferenceContainerGTest,TestFindSecondOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, 0u, "H");
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

TEST(ReferenceContainerGTest,TestFindThirdOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(3, 0u, "H");
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

TEST(ReferenceContainerGTest,TestFindAllOfASingleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "G");
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

TEST(ReferenceContainerGTest,TestFindAllOfMultipleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "H");
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

TEST(ReferenceContainerGTest,TestFindNonExistentObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "NoExist");
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

TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectNameFilterNegative) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectNameFilterPositive) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(4, 0u, "H");
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

TEST(ReferenceContainerGTest,TestFindEmptyTreeObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindEmptyTree(filter));
}


/***********************************************
 * TestFindObjectPath                          *
 ***********************************************/
TEST(ReferenceContainerGTest,TestFindPathLegalObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilter());
}

TEST(ReferenceContainerGTest,TestFindPathLegalObjectNameFilterStartDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilterStartDot());
}

TEST(ReferenceContainerGTest,TestFindPathLegalObjectNameFilterEndDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilterEndDot());
}

TEST(ReferenceContainerGTest,TestFindPathIllegalObjectNameFilterTwoDots) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathIllegalObjectNameFilterTwoDots());
}

TEST(ReferenceContainerGTest,TestFindWrongPathObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindWrongPathObjectNameFilter());
}

TEST(ReferenceContainerGTest,TestFindRelativePathObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindRelativePathObjectNameFilter());
}

/***********************
 * TestFindPerformance *
 ***********************/


#define Linux 1
#define Windows 2
#define FreeRTOS 3
#if (ENVIRONMENT == Windows) || (ENVIRONMENT == Linux)

TEST(ReferenceContainerGTest,TestFindPerformanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceT<ReferenceContainer> largeTree = referenceContainerTest.GenerateTestTreeLarge(5000);
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafU3);
    float totalTime = referenceContainerTest.TestFindPerformance(largeTree, filter);
    //printf("Total time for TestFindPerformanceReferenceFilter = %e\n", totalTime);
    ASSERT_TRUE(totalTime < 1.0);
}

TEST(ReferenceContainerGTest,TestFindPerformanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceT<ReferenceContainer> largeTree = referenceContainerTest.GenerateTestTreeLarge(5000);
    ReferenceContainerFilterObjectName filter(3, ReferenceContainerFilterMode::RECURSIVE, "U3");
    float totalTime = referenceContainerTest.TestFindPerformance(largeTree, filter);
    //printf("Total time for TestFindPerformanceReferenceFilter = %e\n", totalTime);
    ASSERT_TRUE(totalTime < 1.0);
}

#endif
/*********************************
 * TestFindRemoveFirstOccurrence *
 *********************************/
TEST(ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveFirstOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

/****************************************
 * TestFindRemoveFirstOccurrenceReverse *
 ****************************************/
TEST(ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReverseReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrenceReverse(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReverseObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

/*********************************
 * TestFindRemoveSecondOccurrence *
 *********************************/
TEST(ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrence(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveSecondOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, ReferenceContainerFilterMode::RECURSIVE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrence(filter));
}

/****************************************
 * TestFindRemoveSecondOccurrenceReverse *
 ****************************************/
TEST(ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReverseReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrenceReverse(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReverseObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrenceReverse(filter));
}

/****************************************
 * TestFindRemoveSecondOccurrenceReverse *
 ****************************************/
TEST(ReferenceContainerGTest,TestFindRemoveFromSubcontainerReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFromSubcontainer(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveFromSubcontainerObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFromSubcontainer(filter));
}

/****************************************
 * TestFindRemoveAllOfMultipleInstance  *
 ****************************************/
TEST(ReferenceContainerGTest,TestFindRemoveAllOfMultipleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindRemoveAllOfMultipleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveAllOfMultipleInstance(filter));
}

TEST(ReferenceContainerGTest,TestFindWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindWithPath());
}


/***************
 * TestInsert  *
 ***************/

TEST(ReferenceContainerGTest,TestInsertWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertWithPath());
}

TEST(ReferenceContainerGTest,TestInsertAtEnd) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtEnd());
}

TEST(ReferenceContainerGTest,TestInsertAtBeginning) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtBeginning());
}

TEST(ReferenceContainerGTest,TestInsertAtMiddle) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtMiddle());
}

TEST(ReferenceContainerGTest,TestInsertInvalid) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertInvalid());
}

/*************
 * TestSize  *
 *************/
TEST(ReferenceContainerGTest,TestSize) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSize());
}

TEST(ReferenceContainerGTest,TestSizeZero) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSizeZero());
}

/*************
 * TestGet   *
 *************/
TEST(ReferenceContainerGTest,TestGet) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGet());
}

TEST(ReferenceContainerGTest,TestGetInvalid) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetInvalid());
}

/*******************
 * TestIsContainer *
 *******************/
TEST(ReferenceContainerGTest,TestIsContainer) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsContainer());
}

/*******************
 * TestDelete *
 *******************/
TEST(ReferenceContainerGTest,TestDelete) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestDelete());
}

TEST(ReferenceContainerGTest,TestDeleteWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestDeleteWithPath());
}

/*******************
 * TestInitialise *
 *******************/

TEST(ReferenceContainerGTest,TestInitialise) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInitialise());
}

/*******************
 * TestPurge *
 *******************/

TEST(ReferenceContainerGTest,TestPurge) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestPurge());
}

TEST(ReferenceContainerGTest,TestPurge_Shared) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestPurge_Shared());
}

TEST(ReferenceContainerGTest,TestExportData) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestExportData());
}

TEST(ReferenceContainerGTest,TestIsReferenceContainer) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsReferenceContainer());
}
