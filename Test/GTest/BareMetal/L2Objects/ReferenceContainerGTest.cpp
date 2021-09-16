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
#include <ReferenceContainerFilterObjectName.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerTest.h"
#include "ReferenceContainerFilterReferences.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestConstructor) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestConstructor());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestCopyConstructor) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestCopyConstructor());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestOperatorEqual) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestOperatorEqual());
}

//TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGetClassPropertiesCopy) {
//    ReferenceContainerTest referenceContainerTest;
//    ASSERT_TRUE(referenceContainerTest.TestGetClassPropertiesCopy());
//}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGetClassProperties) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetClassProperties());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGetTimeoutInfinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(TTInfiniteWait));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGetTimeoutFinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(1));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestSetTimeoutInfinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetTimeout(TTInfiniteWait));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestSetTimeoutFinite) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSetTimeout(1));
}

/***********************************
 * TestFindFirstOccurrenceAlways   *
 ***********************************/
/*TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}*/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindReferenceFirstOccurrenceAlwaysReferenceObject) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceAlways(filter));
}

/*****************************************
 * TestFindFirstOccurrenceBranchAlways   *
 *****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindFirstOccurrenceBranchAlwaysReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindFirstOccurrenceBranchAlwaysReferenceObject) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrenceBranchAlways(filter));
}


/*****************************
 * TestFind  *
 *****************************/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFind) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFind());
}
/*****************************
 * TestFindFirstOccurrence   *
 *****************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindFirstOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindFirstOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindFirstOccurrence(filter));
}

/*****************************
 * TestFindSecondOccurrence  *
 *****************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindSecondOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindSecondOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindSecondOccurrence(filter));
}

/****************************
 * TestFindThirdOccurrence  *
 ****************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindThirdOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(3, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindThirdOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(3, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindThirdOccurrence(filter));
}

/********************************
 * TestFindAllOfASingleInstance *
 ********************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindAllOfASingleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafG);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindAllOfASingleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "G");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfASingleInstance(filter));
}

/*********************************
 * TestFindAllOfMultipleInstance *
 *********************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindAllOfMultipleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindAllOfMultipleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindAllOfMultipleInstance(filter));
}

/*****************************************
 * TestFindReferenceNonExistent *
 *****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindNonExistentReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, 0u, referenceContainerTest.leafNoExist);
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindNonExistentObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, 0u, "NoExist");
    ASSERT_TRUE(referenceContainerTest.TestFindNonExistent(filter));
}

/*********************************************
 * TestFindReferenceOutOfBoundsOccurrence    *
 *********************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceReferenceFilterNegative) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-2, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceReferenceFilterPositive) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(4, 0u, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectNameFilterNegative) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-2, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindOutOfBoundsOccurrenceObjectNameFilterPositive) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(4, 0u, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindOutOfBoundsOccurrence(filter));
}

/***********************
 * TestFindEmptyTree   *
 ***********************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindEmptyTreeReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, 0u, referenceContainerTest.leafB);
    ASSERT_TRUE(referenceContainerTest.TestFindEmptyTree(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindEmptyTreeObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, 0u, "B");
    ASSERT_TRUE(referenceContainerTest.TestFindEmptyTree(filter));
}


/***********************************************
 * TestFindObjectPath                          *
 ***********************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPathLegalObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilter());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPathLegalObjectNameFilterStartDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilterStartDot());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPathLegalObjectNameFilterEndDot) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathLegalObjectNameFilterEndDot());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPathIllegalObjectNameFilterTwoDots) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindPathIllegalObjectNameFilterTwoDots());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindWrongPathObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindWrongPathObjectNameFilter());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRelativePathObjectNameFilter) {
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

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPerformanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceT<ReferenceContainer> largeTree = referenceContainerTest.GenerateTestTreeLarge(5000);
    ReferenceContainerFilterReferences filter(3, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafU3);
    float totalTime = referenceContainerTest.TestFindPerformance(largeTree, filter);
    //printf("Total time for TestFindPerformanceReferenceFilter = %e\n", totalTime);
    ASSERT_TRUE(totalTime < 1.0);
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindPerformanceObjectNameFilter) {
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
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFirstOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

/****************************************
 * TestFindRemoveFirstOccurrenceReverse *
 ****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReverseReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrenceReverse(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFirstOccurrenceReverseObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFirstOccurrence(filter));
}

/*********************************
 * TestFindRemoveSecondOccurrence *
 *********************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrence(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveSecondOccurrenceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, ReferenceContainerFilterMode::RECURSIVE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrence(filter));
}

/****************************************
 * TestFindRemoveSecondOccurrenceReverse *
 ****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReverseReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, referenceContainerTest.containerC);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrenceReverse(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveSecondOccurrenceReverseObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(2, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REVERSE, "C");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveSecondOccurrenceReverse(filter));
}

/****************************************
 * TestFindRemoveSecondOccurrenceReverse *
 ****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFromSubcontainerReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.containerE);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFromSubcontainer(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveFromSubcontainerObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, "E");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveFromSubcontainer(filter));
}

/****************************************
 * TestFindRemoveAllOfMultipleInstance  *
 ****************************************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveAllOfMultipleInstanceReferenceFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterReferences filter(-1, ReferenceContainerFilterMode::RECURSIVE, referenceContainerTest.leafH);
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveAllOfMultipleInstance(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindRemoveAllOfMultipleInstanceObjectNameFilter) {
    ReferenceContainerTest referenceContainerTest;
    ReferenceContainerFilterObjectName filter(-1, ReferenceContainerFilterMode::RECURSIVE, "H");
    ASSERT_TRUE(referenceContainerTest.TestFindRemoveAllOfMultipleInstance(filter));
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestFindWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestFindWithPath());
}


/***************
 * TestInsert  *
 ***************/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInsertWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertWithPath());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInsertAtEnd) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtEnd());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInsertAtBeginning) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtBeginning());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInsertAtMiddle) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertAtMiddle());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInsertInvalid) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInsertInvalid());
}

/*************
 * TestSize  *
 *************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestSize) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSize());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestSizeZero) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestSizeZero());
}

/*************
 * TestGet   *
 *************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGet) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGet());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestGetInvalid) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestGetInvalid());
}

/*******************
 * TestIsContainer *
 *******************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestIsContainer) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsContainer());
}

/*******************
 * TestDelete *
 *******************/
TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestDelete) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestDelete());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestDeleteWithPath) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestDeleteWithPath());
}

/*******************
 * TestInitialise *
 *******************/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestInitialise) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestInitialise());
}

/*******************
 * TestPurge *
 *******************/

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestPurge) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestPurge());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestPurge_Shared) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestPurge_Shared());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestExportData) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestExportData());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestIsReferenceContainer) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsReferenceContainer());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestAddBuildToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestAddBuildToken());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestIsBuildToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsBuildToken());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestRemoveBuildToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestRemoveBuildToken());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestAddDomainToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestAddDomainToken());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestIsDomainToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestIsDomainToken());
}

TEST(BareMetal_L2Objects_ReferenceContainerGTest,TestRemoveDomainToken) {
    ReferenceContainerTest referenceContainerTest;
    ASSERT_TRUE(referenceContainerTest.TestRemoveDomainToken());
}
