/**
 * @file ReferenceContainerTest.h
 * @brief Header file for class ReferenceContainerTest
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERTEST_H_
#define REFERENCECONTAINERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"

#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests for the ReferenceContainer class.
 * @details These tests are based around a tree which is automatically generated by the constructor
 * and whose structure is described in GenerateTree.
 */
class ReferenceContainerTest {
public:
    /**
     * @brief Constructor. Generates the testing tree (see GenerateTree).
     */
    ReferenceContainerTest();

    /**
     * @brief Tests the ReferenceContainer constructor.
     * @return true if a container with zero size and infinite timeout is created by the default constructor of ReferenceContainer.
     */
    bool TestConstructor();

//    /**
//     * @brief Tests the TestGetClassPropertiesCopy function.
//     * @return true if ClassProperties::GetName == "ReferenceContainer".
//     */
//    bool TestGetClassPropertiesCopy();

    /**
     * @brief Tests the TestGetClassProperties function.
     * @return true if ClassProperties::GetName == "ReferenceContainer".
     */

    /**
     * @brief Tests the ReferenceContainer copy constructor.
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the ReferenceContainer operator =.
     */
    bool TestOperatorEqual();

    bool TestGetClassProperties();

    /**
     * @brief Tests the ReferenceContainer::GetTimeout function.
     * @return true if the timeout is changed to a new value and correctly read-back.
     */
    bool TestGetTimeout(TimeoutType timeout);

    /**
     * @brief Tests the ReferenceContainer::SetTimeout function.
     * @return true if the timeout is changed to a new value and correctly read-back.
     */
    bool TestSetTimeout(TimeoutType timeout);

    /**
     * @brief Tests the ReferenceContainer::Find function.
     * @return true if the leaf "E" is found.
     */
    bool TestFind();

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that always finds an occurrence.
     * @details For all the ReferenceContainerFilterMode combinations this test will always find the leaf "B".
     * @param[in] filter The filter must be setup to look for the first occurrence of the leaf "B".
     * @return true if the leaf "B" can be found for any combination of ReferenceContainerFilterMode (excluding ReferenceContainerFilterMode::REMOVE).
     */
    bool TestFindFirstOccurrenceAlways(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that always finds an occurrence (also looking inside a branch).
     * @details For all the ReferenceContainerFilterMode combinations this test will always find the leaf "H". In reverse mode
     * the leaf will be the first and will have no parents, while in forward (and recursive) mode the leaf will be found
     * inside the branch C.E.H..
     * @param[in] filter The filter must be setup to look for the first occurrence of the leaf "H".
     * @return true if the leaf "H" can be found for any combination of ReferenceContainerFilterMode (excluding ReferenceContainerFilterMode::REMOVE).
     */
    bool TestFindFirstOccurrenceBranchAlways(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the container "E".
     * @details For all the ReferenceContainerFilterMode combinations this test will only find the container "E" when
     * RECURSIVE is set to true. If PATH is set, the test verifies that the correct branching history is stored.
     * @param[in] filter The filter must be setup to look for the first occurrence of the container "E".
     * @return true if the container "E" can only be found for RECURSIVE mode and the branch "C.E" is returned when moving forward
     * and the branch "D.C.E. is returned when moving in REVERSE.
     */
    bool TestFindFirstOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for second occurrence of the leaf "H".
     * @details For all the ReferenceContainerFilterMode combinations this test will only find the leaf "H" when
     * ReferenceContainerFilterMode::RECURSIVE is set to true. If PATH is set, the test verifies that the correct branching history is stored.
     * @param[in] filter The filter must be setup to look for the second occurrence of the leaf "H".
     * @return true if the second instance of the leaf "H" can be only found for ReferenceContainerFilterMode::RECURSIVE mode and
     * the branch "D.C.E.H" is returned when moving the ReferenceContainerFilterMode::PATH is stored.
     */
    bool TestFindSecondOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for third occurrence of the leaf "H".
     * @details For all the ReferenceContainerFilterMode combinations this test will only find the leaf "H" when
     * ReferenceContainerFilterMode::RECURSIVE is set to true. If ReferenceContainerFilterMode::PATH is set, the test verifies that the correct branching history is stored.
     * @param[in] filter The filter must be setup to look for the third occurrence of the leaf "H".
     * @return true if the third instance of the leaf "H" can be only found for ReferenceContainerFilterMode::RECURSIVE mode and
     * the branch "C.E.H" is returned only when moving forward and the ReferenceContainerFilterMode::PATH is stored.
     */
    bool TestFindThirdOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for all instances of the leaf "G" (only a single instance exists).
     * @details For all the ReferenceContainerFilterMode combinations this test will only find the leaf "G" when
     * ReferenceContainerFilterMode::RECURSIVE is set to true. If ReferenceContainerFilterMode::PATH is set, the test verifies that no branches are stored (in multiple instance search no history can be retrieved).
     * @param[in] filter The filter must be setup to look for the all occurrences (i.e. with -1) of the leaf "G".
     * @return true if one and only one instance of "G" can be found and that when ReferenceContainerFilterMode::PATH is set no path is returned.
     */
    bool TestFindAllOfASingleInstance(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for all instances of the leaf "H" (multiple instances exist).
     * @details For all the ReferenceContainerFilterMode combinations this test will always find the leaf "H" .
     * If ReferenceContainerFilterMode::PATH is set, the test verifies that no branches are stored (in multiple instance search no history can be retrieved).
     * @param[in] filter The filter must be setup to look for the all occurrences (i.e. with -1) of the leaf "H".
     * @return true if one instance of "H" can be found when RECURSIVE is not set and three instances of "H" can be found when ReferenceContainerFilterMode::RECURSIVE is set..
     */
    bool TestFindAllOfMultipleInstance(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for an occurrence number that does not exist in the tree.
     * @details For all the ReferenceContainerFilterMode combinations this test will always never find the searched node/leaf .
     * @param[in] filter The filter must be setup to look for the an occurrence number of any of the nodes that does not exist.
     * @return true if the occurrence number does not exist.
     */
    bool TestFindOutOfBoundsOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for a reference that does not exist in the tree.
     * @details For all the ReferenceContainerFilterMode combinations this test will always never find the searched node/leaf.
     * @param[in] filter The filter must be setup to look for a reference that does not exist in the tree.
     * @return true if the reference does not exist in the tree.
     */
    bool TestFindNonExistent(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for a reference that on an empty tree.
     * @param[in] filter The filter must be setup to look for any reference.
     * @return true if no reference is found.
     */
    bool TestFindEmptyTree(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that searches for an absolute path "D.C.E.H".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path can be successfully retrieved from the tree.
     */
    bool TestFindPathLegalObjectNameFilter();

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that searches for an absolute path with a small typo ".D.C.E.H".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path can be successfully retrieved from the tree.
     */
    bool TestFindPathLegalObjectNameFilterStartDot();

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that searches for an absolute path with a small typo "D.C.E.H.".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path can be successfully retrieved from the tree.
     */
    bool TestFindPathLegalObjectNameFilterEndDot();

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that searches for an absolute path with a typo "D.C.E..H.".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path cannot be successfully retrieved from the tree.
     */
    bool TestFindPathIllegalObjectNameFilterTwoDots();

    /**
     * Tests the ReferenceContainer::Find function with a use-case that searches for an absolute path with a typo "D.C.F.H".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path cannot be found given that the full absolute path must be matched.
     */
    bool TestFindWrongPathObjectNameFilter();

    /**
     * Tests the ReferenceContainer::Find function with a use-case that searches for a relative path "F.G".
     * @details Note that when looking for an absolute path RECURSIVE and PATH are automatically enforced.
     * @return true if the path cannot be found given that only absolute paths are supported.
     */
    bool TestFindRelativePathObjectNameFilter();

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the container "C" and deletes it.
     * @details When moving forward this will delete the second main branch of the tree. The third branch, where "C" also
     * exists, should not be deleted.
     *
     * Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     *
     * @param[in] filter The filter must be setup to look for the first occurrence of the container "C".
     * @return true if moving forward the branch "C" is deleted and the branch "D.C" is not removed.
     */
    bool TestFindRemoveFirstOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the container "C" in reverse and removes it.
     * @details When moving in reverse this will remove the branch under the "D" container, but should not delete the main "C" branch.
     *
     * Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     *
     * @param[in] filter The filter must be setup to look for the first occurrence of the container "C" in REVERSE.
     * @return true if moving in reverse the branch "C" under "D" is removed and the main branch "C" is not removed.
     */
    bool TestFindRemoveFirstOccurrenceReverse(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the second occurrence of the container "C" and deletes it.
     * @details When moving forward this will remove the branch under the "D" container, but should not delete the main "C" branch.
     *
     * Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     *
     * @param[in] filter The filter must be setup to look for the second occurrence of the container "C".
     * @return true if moving forward the branch "C" under "D" is removed and the main branch "C" is not removed.
     */
    bool TestFindRemoveSecondOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the second occurrence of the container "C" in reverse and removes it.
     * @details When moving in reverse this will delete the second main branch of the tree. The third branch, where "C" also
     * exists, should not be deleted.
     *
     * Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     *
     * @param[in] filter The filter must be setup to look for the second occurrence of the container "C" in REVERSE.
     * @return true if moving in reverse the branch "C" is deleted and the branch "D.C" is not removed.
     */
    bool TestFindRemoveSecondOccurrenceReverse(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for the first occurrence of the container "E" removes it.
     * @details Given that the container "E" is contained in container "C", removing it from any of the instances of "C" will result
     * in removing "E" from the tree, given that the multiple occurrences of "C" reference the same underlying container.
     *
     * Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     *
     * @param[in] filter The filter must be setup to look for the first occurrence of the container "E".
     * @return true if removing the first occurrence of the container "E" removes all occurrences of the container "E" in the tree.
     */
    bool TestFindRemoveFromSubcontainer(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find function with a use-case that looks for all instances of the leaf "H" (multiple instances exist) and removes all.
     * @details Note that the tree will be modified and calling subsequent tests without generating a new instance of ReferenceContainterTests
     * will lead to undefined behaviour.
     * @param[in] filter must be setup to look for the all occurrences (i.e. with -1) of the leaf "H".
     * @return true if after searching and removing all instances of "H" no references remain in the tree.
     */
    bool TestFindRemoveAllOfMultipleInstance(ReferenceContainerFilter &filter);

    /**
     * @brief Tests the ReferenceContainer::Find on a large tree.
     * @param[in] largeTree a reference to a tree created with GenerateTestTreeLarge
     * @param[in] filter must be setup to look for the third occurrence of U3.
     * @return the time in seconds that it took to execute the function.
     */
    float TestFindPerformance(ReferenceT<ReferenceContainer> largeTree, ReferenceContainerFilter &filter);

    bool TestFindWithPath();

    /**
     * @brief Tests the ReferenceContainer::Insert with the full path as parameter.
     * @return true if the function inserts the reference at the specified path
     */
    bool TestInsertWithPath();


    bool TestInsertWithLongNameBug();

    /**
     * @brief Tests the ReferenceContainer::Insert function.
     * @details Creates an empty container and Inserts the following nodes to the end of the container: leafB, containerC, containerD, leafH
     * @return true if the elements are inserted in the order: leafB, containerC, containerD, leafH and the size of the container is 4.
     */
    bool TestInsertAtEnd();

    /**
     * @brief Tests the ReferenceContainer::Insert function.
     * @details Creates an empty container and Inserts the following nodes always to the beginning leafB, containerC, containerD, leafH
     * @return true if the elements are inserted in the order: leafH, containerD, containerC, leafB and the size of the container is 4.
     */
    bool TestInsertAtBeginning();

    /**
     * @brief Tests the ReferenceContainer::Insert function.
     * @details Creates an empty container and Inserts the following nodes always to the beginning of the container: leafB, containerD
     * and leafH. Subsequently inserts containerC at position 2.
     * @return true if the elements are inserted in the order: leafH, containerD, containerC, leafB and the size of the container is 4.
     */
    bool TestInsertAtMiddle();

    /**
     * @brief Tests the ReferenceContainer::Insert function with an invalid reference.
     * @return true if calling ReferenceContainer::Insert with an invalid reference returns false and the size of the container is not incremented.
     */
    bool TestInsertInvalid();

    /**
     * @brief Tests the ReferenceContainer::Size function.
     * @details Creates an empty container and Inserts the following nodes to the container: leafB, containerD
     * and leafH. Subsequently inserts containerC at position 2.
     * @return true if the size of the container is 4.
     */
    bool TestSize();

    /**
     * @brief Tests the ReferenceContainer::Size function.
     * @details Creates an empty container and checks that the size is zero.
     * @return true if the size of the container is 0.
     */
    bool TestSizeZero();

    /**
     * @brief Tests the ReferenceContainer::Get function.
     * @details Creates an empty container and Inserts the following nodes to the end of the container: leafB, containerC, containerD, leafH
     * @return true if ReferenceContainer::Get(i), for i=0..3, returns the element described above.
     */
    bool TestGet();

    /**
     * @brief Tests the ReferenceContainer::Get function with an invalid index.
     * @details Creates an empty container and Inserts the following nodes to the end of the container: leafB, containerC, containerD, leafH
     * @return true if ReferenceContainer::Get(4) and ReferenceContainer::Get(-1) return an invalid reference.
     */
    bool TestGetInvalid();

    /**
     * @brief Tests the ReferenceContainer::IsContainer function.
     * @return true if ReferenceContainer::IsContainer(containerC) returns true and ReferenceContainer::IsContainer(leafB) returns false.
     */
    bool TestIsContainer();

    /**
     * @brief Tests the ReferenceContainer::Delete function.
     * @details Creates an empty container and Inserts the following nodes to the end of the container: leafB, containerC, containerD, leafH.
     * Subsequently ReferenceContainer::Delete(containerC) is called.
     * @return true if ReferenceContainer::Delete(containerC) returns true and ReferenceContainer::Size is equal to 3.
     */
    bool TestDelete();

    bool TestDeleteWithPath();

    /**
     * @brief Tests if for each of the marked nodes of \a data a new Object
     * will be created and its Reference added to the container.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Purge method
     */
    bool TestPurge();

    /**
     * @brief Tests the Purge method
     */
    bool TestPurge_Shared();

    /**
     * @brief Tests the ExportData function
     */
    bool TestExportData();

    /**
     * @brief Tests the IsBuildToken method.
     */
    bool TestIsBuildToken();

    /**
     * @brief Tests the AddBuildToken method.
     */
    bool TestAddBuildToken();

    /**
     * @brief Tests the RemoveBuildToken method.
     */
    bool TestRemoveBuildToken();

    /**
     * @brief Tests the IsDomainToken method.
     */
    bool TestIsDomainToken();

    /**
     * @brief Tests the AddDomainToken method.
     */
    bool TestAddDomainToken();

    /**
     * @brief Tests the RemoveDomainToken method.
     */
    bool TestRemoveDomainToken();
    /**
     * List of nodes for the tree described in the GenerateTestTree function
     */
    ReferenceT<ReferenceContainer> tree;
    ReferenceT<Object> leafB;
    ReferenceT<ReferenceContainer> containerC;
    ReferenceT<ReferenceContainer> containerD;
    ReferenceT<ReferenceContainer> containerE;
    ReferenceT<ReferenceContainer> containerF;
    ReferenceT<Object> leafG;
    ReferenceT<Object> leafH;
    ReferenceT<Object> leafNoExist;

    ReferenceT<Object> leafU3;
    ReferenceT<ReferenceContainer> containerU1;
    ReferenceT<ReferenceContainer> containerU2;

    /**
     * @brief Generates a testing tree which is used by the other test methods.
     * @details The tree has the following structure
     *           Root
     *          /|\ \
     *         B C D H
     *           | |\
     *           E F C
     *           | |  \
     *           H G   E
     *                  \
     *                   H
     * @return the container with the generated tree, i.e. the Root
     */
    ReferenceT<ReferenceContainer> GenerateTestTree();

    /**
     * @brief Generates a testing tree which is used by the other test methods.
     * @details The tree has the following structure
     *           Root
     *          /| \
     *        U3 U2 U1
     *           |   \
     *           U2  U1
     *           |    \
     *           .     .
     *           .      .
     *           .       .
     *           |        \
     *           U3        U3
     * @param depth the number of U2 and U1 nodes
     * @return the container with the generated tree, i.e. the Root
     */
    ReferenceT<ReferenceContainer> GenerateTestTreeLarge(uint32 depth);

    /**
     * @brief Tests the IsReferenceContainer method.
     */
    bool TestIsReferenceContainer();

    volatile int32 spinLock;

private:
    /**
     * Heap used for the tests
     */
    HeapI* h;

    /**
     * @brief Verifies if the \a source and \a test containers contain the same information.
     * @param[in] source first container to be compared.
     * @param[in] test second container to be compared.
     * @return true if the \a source and \a test containers hold the same object and if the containers are equally ordered.
     */
    bool VerifyExpectedResult(ReferenceContainer &source, ReferenceContainer &test);

    /**
     * @brief Verifies if the \a source and \a test containers contain the same information.
     * @param[in] source first container to be compared.
     * @param[in] test second container to be compared.
     * @return true if the references hold by \a source and \a test hold objects that have the same name  and if the containers are equally ordered.
     */
    bool VerifyExpectedResultByObjectName(ReferenceContainer &source, ReferenceContainer &test);

    /**
     * @brief Generic utility function to test the ReferenceContainer::Find.
     * @param[in] tree The ReferenceT<ReferenceContainer> to be searched.
     * @param[in] filter The filter to be applied.
     * @param[in] expectedResult The expected resulted expressed as a string (dots separate nodes, e.g. D.C.E.H).
     * @return true if the expectedResult is equal to the result retrieved by tree->Find
     */
    bool TestFindFilter(ReferenceT<ReferenceContainer> tree, ReferenceContainerFilter &filter, const char8 * const expectedResult);

    /**
     * @brief Generates a container of references from a string representation.
     * @param[in] result destination container.
     * @param[in] str source string presentation.
     * @return true if the string can be translated into a container of references.
     */
    bool GenerateExpectedResultFromString(ReferenceContainer &result, const char8 * const str);

    /**
     * @brief Generates a container of references, linking to existing references, from a string representation.
     * @param[in] source tree with existing references.
     * @param[in] result destination container.
     * @param[in] str source string presentation.
     * @return true if the string can be translated into a container of references.
     */
    bool GenerateExpectedResultFromStringUsingExistingReferences(ReferenceT<ReferenceContainer> source, ReferenceContainer &result, const char8 * const str);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERTEST_H_ */

