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

#ifndef TEST_CORE_L1OBJECTS_REFERENCECONTAINERTEST_H_
#define TEST_CORE_L1OBJECTS_REFERENCECONTAINERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjects.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO
 */
class ReferenceContainerTest {
public:
    ReferenceContainerTest();

    /**
     * @brief Tests the constructor.
     * @return @TODO.
     */
    bool TestConstructor();

    /**
     * @brief TODO
     */
    bool TestGetTimeout(TimeoutType timeout);

    /**
     * @brief TODO
     */
    bool TestSetTimeout(TimeoutType timeout);

    /**
     * @brief TODO
     */
    bool TestFindFirstOccurrenceAlways(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindFirstOccurrenceBranchAlways(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindFirstOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindSecondOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindThirdOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindAllOfASingleInstance(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindAllOfMultipleInstance(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindOutOfBoundsOccurrence(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindNonExistent(ReferenceContainerFilter &filter);

    /**
     * @brief TODO
     */
    bool TestFindPath(ReferenceContainerFilterObjects &filter, ReferenceContainer &expectedResult);

    /**
     * List of nodes for the tree described in the GenerateTestTree function
     */
    ReferenceT<Object> leafB;
    ReferenceT<ReferenceContainer> containerC;
    ReferenceT<ReferenceContainer> containerD;
    ReferenceT<ReferenceContainer> containerE;
    ReferenceT<ReferenceContainer> containerF;
    ReferenceT<Object> leafG;
    ReferenceT<Object> leafH;
    ReferenceT<Object> leafNoExist;

private:
    /**
     * Heap used for the tests
     */
    Heap h;

    /**
     * @brief Generates a testing tree which is used by the other test methods.
     * @details The tree has the following structure
     *           A
     *          /|\ \
     *         B C D H
     *           | |\
     *           E F C
     *           | |  \
     *           H G   E
     *                  \
     *                   H
     * @param container The container where the tree is going to be inserted, i.e. the node A
     */
    void GenerateTestTree(ReferenceT<ReferenceContainer> container);

    /**
     * TODO
     */
    bool VerifyExpectedResult(ReferenceContainer &source, ReferenceContainer &test);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L1OBJECTS_REFERENCECONTAINERTEST_H_ */

