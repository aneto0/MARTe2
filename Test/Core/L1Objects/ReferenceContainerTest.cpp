/**
 * @file ReferenceContainerTest.cpp
 * @brief Source file for class ReferenceContainerTest
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

 * @details This source file contains the definition of all the methods for
 * the class ReferenceContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerTest.h"

#include <ReferenceContainerFilterObjectName.h>
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilter.h"
#include "StringHelper.h"
#include "Memory.h"

ReferenceContainerTest::ReferenceContainerTest() {
    tree = GenerateTestTree();
}

bool ReferenceContainerTest::TestConstructor() {
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    bool ok = container.IsValid();
    if (ok) {
        ok &= (container->Size() == 0);
    }
    if (ok) {
        ok &= (container->GetTimeout() == TTInfiniteWait);
    }
    return ok;
}

bool ReferenceContainerTest::TestGetTimeout(TimeoutType timeout) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    bool ok = container.IsValid();
    if (ok) {
        container->SetTimeout(timeout);
        ok = (container->GetTimeout() == timeout);
    }
    return ok;
}

bool ReferenceContainerTest::TestSetTimeout(TimeoutType timeout) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    bool ok = container.IsValid();
    if (ok) {
        container->SetTimeout(timeout);
        ok = (container->GetTimeout() == timeout);
    }
    return ok;
}

bool ReferenceContainerTest::TestFindFirstOccurrenceAlways(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        ok &= TestFindFilter(tree, filter, "B");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindFirstOccurrenceBranchAlways(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (!filter.IsStorePath()) {
            ok &= TestFindFilter(tree, filter, "H");
        }
        else if (!filter.IsRecursive()) {
            ok &= TestFindFilter(tree, filter, "H");
        }
        else {
            if (!filter.IsReverse()) {
                ok &= TestFindFilter(tree, filter, "C.E.H");
            }
            else {
                ok &= TestFindFilter(tree, filter, "H");
            }
        }

    }

    return ok;
}

bool ReferenceContainerTest::TestFindFirstOccurrence(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (filter.IsRecursive()) {
            if (!filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "E");
            }
            else {
                if (!filter.IsReverse()) {
                    ok &= TestFindFilter(tree, filter, "C.E");
                }
                else {
                    ok &= TestFindFilter(tree, filter, "D.C.E");
                }
            }
        }
        else {
            ok &= TestFindFilter(tree, filter, "");
        }
    }
    return ok;
}

bool ReferenceContainerTest::TestFindSecondOccurrence(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (filter.IsRecursive()) {
            if (!filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "H");
            }
            else {
                //Reverse or not reverse the second leafH is always in the middle
                ok &= TestFindFilter(tree, filter, "D.C.E.H");
            }
        }
        else {
            ok &= TestFindFilter(tree, filter, "");
        }
    }
    return ok;
}

bool ReferenceContainerTest::TestFindThirdOccurrence(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (filter.IsRecursive()) {
            if (!filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "H");
            }
            else {
                //From LR it is the last
                if (!filter.IsReverse()) {
                    ok &= TestFindFilter(tree, filter, "H");
                }
                else {
                    ok &= TestFindFilter(tree, filter, "C.E.H");
                }
            }
        }
        else {
            ok &= TestFindFilter(tree, filter, "");
        }
    }

    return ok;
}

bool ReferenceContainerTest::TestFindAllOfASingleInstance(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (filter.IsRecursive()) {
            if (!filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "G");
            }
            //It should never get here given that find all => !StorePath
            else {
                ok &= TestFindFilter(tree, filter, "");
            }
        }
        else {
            ok &= TestFindFilter(tree, filter, "");
        }
    }

    return ok;
}

bool ReferenceContainerTest::TestFindAllOfMultipleInstance(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        if (!filter.IsRecursive()) {
            ok &= TestFindFilter(tree, filter, "H");
        }
        else {
            if (!filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "H.H.H");
            }
            //It should never get here given that find all => !StorePath
            else {
                ok &= TestFindFilter(tree, filter, "");
            }
        }
    }

    return ok;
}

bool ReferenceContainerTest::TestFindOutOfBoundsOccurrence(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        ok &= TestFindFilter(tree, filter, "");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindNonExistent(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    //Look for a node that does not exist.
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        ok &= TestFindFilter(tree, filter, "");
    }
    return ok;
}

bool ReferenceContainerTest::TestFindEmptyTree(ReferenceContainerFilter &filter) {
    bool ok = true;
    uint32 i = 0;
    ReferenceT<ReferenceContainer> emptyTree("ReferenceContainer", h);
    //Look for a node that does not exist.
    for (i = 0; i < 8; i++) {
        filter.SetMode(i);
        filter.Reset();
        ok &= TestFindFilter(emptyTree, filter, "");
    }
    return ok;
}

bool ReferenceContainerTest::TestFindPathLegalObjectNameFilter() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjects filter(1, i, "D.C.E.H");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathLegalObjectNameFilterStartDot() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjects filter(1, i, ".D.C.E.H");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathLegalObjectNameFilterEndDot() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjects filter(1, i, "D.C.E.H.");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathIllegalObjectNameFilterTwoDots() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjects filter(1, i, "D.C.E..H");
        ok &= TestFindFilter(tree, filter, "");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPerformance(ReferenceT<ReferenceContainer> largeTree,
                                                 ReferenceContainerFilter &filter) {
    return TestFindFilter(largeTree, filter, "U3");
}

bool ReferenceContainerTest::TestFindRemoveFirstOccurrence(ReferenceContainerFilter &filter) {
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REMOVE);
    //Remove node "C"
    bool ok = TestFindFilter(tree, filter, "C");
    filter.Reset();
    filter.SetMode(0u);
    //Should no longer find C but still find D.C
    ok &= !TestFindFilter(tree, filter, "C");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
    ok &= TestFindFilter(tree, filter, "D.C");

    return ok;
}

bool ReferenceContainerTest::TestFindRemoveFirstOccurrenceReverse(ReferenceContainerFilter &filter) {
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REMOVE);
    //Remove node "D.C"
    bool ok = TestFindFilter(tree, filter, "D.C");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE);
    //Should no longer find D.C but still find C
    ok &= !TestFindFilter(tree, filter, "D.C");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
    ok &= TestFindFilter(tree, filter, "C");

    return ok;
}

bool ReferenceContainerTest::TestFindRemoveSecondOccurrence(ReferenceContainerFilter &filter) {
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REMOVE);
    //Remove node "C"
    bool ok = TestFindFilter(tree, filter, "C");
    filter.SetOriginalSetOccurrence(1);
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE);
    //Should no longer find D.C but still find C
    ok &= TestFindFilter(tree, filter, "C");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
    ok &= !TestFindFilter(tree, filter, "D.C");

    return ok;
}

bool ReferenceContainerTest::TestFindRemoveSecondOccurrenceReverse(ReferenceContainerFilter &filter) {
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REMOVE);
    //Remove node "C"
    bool ok = TestFindFilter(tree, filter, "C");
    filter.SetOriginalSetOccurrence(1);
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
    //Should no longer find C but still find D.C
    ok &= TestFindFilter(tree, filter, "D.C");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
    ok &= !TestFindFilter(tree, filter, "C");

    return ok;
}

bool ReferenceContainerTest::TestFindFilter(ReferenceT<ReferenceContainer> tree,
                                            ReferenceContainerFilter &filter,
                                            const char8 * const expectedResult) {
    ReferenceContainer expectedResultContainer;
    ReferenceContainer result;
    bool ok = false;
    bool filterByObjectName = (dynamic_cast<ReferenceContainerFilterObjects *>(&filter) != NULL);

    if (filterByObjectName) {
        ok = GenerateExpectedResultFromString(expectedResultContainer, expectedResult);
    }
    else {
        ok = GenerateExpectedResultFromStringUsingExistingReferences(tree, expectedResultContainer, expectedResult);
    }

    if (ok) {
        tree->Find(result, filter);
        if (filterByObjectName) {
            ok = VerifyExpectedResultByObjectName(result, expectedResultContainer);
        }
        else {
            ok = VerifyExpectedResult(result, expectedResultContainer);
        }
    }
    return ok;
}

ReferenceT<ReferenceContainer> ReferenceContainerTest::GenerateTestTree() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    leafH = ReferenceT<Object>("Object", h);
    leafB = ReferenceT<Object>("Object", h);
    containerC = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    containerD = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    containerE = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    containerF = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    leafG = ReferenceT<Object>("Object", h);
    leafH = ReferenceT<Object>("Object", h);
    leafNoExist = ReferenceT<Object>("Object", h);

    leafB->SetName("B");
    containerC->SetName("C");
    containerD->SetName("D");
    containerE->SetName("E");
    containerF->SetName("F");
    leafG->SetName("G");
    leafH->SetName("H");
    leafNoExist->SetName("NoExist");

    containerE->Insert(leafH);
    containerC->Insert(containerE);
    containerD->Insert(containerF);
    containerD->Insert(containerC);
    containerF->Insert(leafG);
    containerRoot->Insert(leafB);
    containerRoot->Insert(containerC);
    containerRoot->Insert(containerD);
    containerRoot->Insert(leafH);
    return containerRoot;
}

ReferenceT<ReferenceContainer> ReferenceContainerTest::GenerateTestTreeLarge(uint32 depth) {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    leafU3 = ReferenceT<Object>("Object", h);
    containerU1 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    containerU2 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);

    leafU3->SetName("U3");
    containerU1->SetName("U1");
    containerU2->SetName("U2");

    uint32 i = 0u;
    ReferenceT<ReferenceContainer> nodeU1;
    ReferenceT<ReferenceContainer> nodeU2;

    for (i = 0; i < depth; i++) {
        nodeU1 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
        nodeU2 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
        nodeU1->SetName("");
        nodeU2->SetName("");
        containerU1->Insert(nodeU1);
        containerU2->Insert(nodeU2);
    }
    nodeU1->Insert(leafU3);
    nodeU2->Insert(leafU3);

    containerRoot->Insert(leafU3);
    containerRoot->Insert(containerU2);
    containerRoot->Insert(containerU1);
    return containerRoot;
}

bool ReferenceContainerTest::VerifyExpectedResult(ReferenceContainer &source,
                                                  ReferenceContainer &test) {
    bool ok = (source.Size() == test.Size());
    if (ok) {
        uint32 i;
        for (i = 0; i < source.Size(); i++) {
            ok &= (source.Get(i) == test.Get(i));
        }
    }

    return ok;
}

bool ReferenceContainerTest::VerifyExpectedResultByObjectName(ReferenceContainer &source,
                                                              ReferenceContainer &test) {
    bool ok = (source.Size() == test.Size());
    if (ok) {
        uint32 i;
        for (i = 0; i < source.Size(); i++) {
            ok &= (StringHelper::Compare(source.Get(i)->GetName(), test.Get(i)->GetName()) == 0);
        }
    }

    return ok;
}

bool ReferenceContainerTest::GenerateExpectedResultFromStringUsingExistingReferences(ReferenceT<ReferenceContainer> source,
                                                                                     ReferenceContainer &result,
                                                                                     const char8 * const str) {
    bool ok = true;
    Heap h;
    uint32 len = StringHelper::Length(str) + 1;
    char8 *name = new char[len];
    Memory::Set(name, '\0', len);

    uint32 i;
    uint32 j = 0;
    for (i = 0; i < (len - 1); i++) {
        if (str[i] == '.') {
            ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, name);
            ReferenceContainer resultSingle;
            //Cannot pass result directly into the Find because the filter uses the result as an history buffer!
            source->Find(resultSingle, filter);
            if (resultSingle.Size() > 0) {
                result.Insert(resultSingle.Get(0));
            }
            j = 0;
            Memory::Set(name, '\0', len);
        }
        else {
            name[j++] = str[i];
        }
    }
    //Last node must be a leaf
    //If nothing was found => first node is also a leaf
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, name);
    ReferenceContainer resultSingle;
    //Cannot pass result directly into the Find because the filter uses the result as an history buffer!
    source->Find(resultSingle, filter);
    if (resultSingle.Size() > 0) {
        result.Insert(resultSingle.Get(0));
    }

    delete[] name;
    return ok;
}

bool ReferenceContainerTest::GenerateExpectedResultFromString(ReferenceContainer &result,
                                                              const char8 * const str) {
    bool ok = true;
    Heap h;
    uint32 len = StringHelper::Length(str) + 1;
    char8 *name = new char[len];
    Memory::Set(name, '\0', len);

    uint32 i;
    uint32 j = 0;
    for (i = 0; i < (len - 1); i++) {
        if (str[i] == '.') {
            ReferenceT<ReferenceContainer> node("ReferenceContainer", h);
            node->SetName(name);
            result.Insert(node);
            j = 0;
            Memory::Set(name, '\0', len);
        }
        else {
            name[j++] = str[i];
        }
    }
    //Last node must be a leaf
    //If nothing was found => first node is also a leaf
    if (len > 1) {
        ReferenceT<Object> leaf("Object", h);
        leaf->SetName(name);
        result.Insert(leaf);
    }
    delete[] name;
    return ok;
}

void PrintTree(ReferenceT<ReferenceContainer> tree) {
    uint32 i = 0;
    for (i = 0; i < tree->Size(); i++) {
        printf("%s|", tree->Get(i)->GetName());
        ReferenceT<ReferenceContainer> node = tree->Get(i);
        if (node.IsValid()) {
            PrintTree(node);
        }
    }
}

#if 0
bool ReferenceContainerTest::TestFindRemoveFirstOccurrence(ReferenceT<ReferenceContainer> removeTree, ReferenceContainerFilter &filter) {

    bool ok = true;
    if (ok) {
        ReferenceT<ReferenceContainer> removeTree = GenerateTestTree();
        filter = ReferenceContainerFilterReferences(1, ReferenceContainerFilterMode::RECURSIVE, containerC);
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REMOVE);
        ok &= TestFindFilter(removeTree, filter, "C");
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(0u);
        //Should no longer find C.E.H. but still find D.C.E.H
        ok &= !TestFindFilter(removeTree, filter, "C");
        filter.Reset();
        filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
        ok &= TestFindFilter(removeTree, filter, "D.C");

        removeTree = GenerateTestTree();
        filter = ReferenceContainerFilterReferences(1, ReferenceContainerFilterMode::RECURSIVE, containerC);
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(
                ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE
                | ReferenceContainerFilterMode::REMOVE);
        ok &= TestFindFilter(removeTree, filter, "D.C");
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(ReferenceContainerFilterMode::REVERSE);
        ok &= !TestFindFilter(removeTree, filter, "D.C");
        filter.Reset();
        ok &= TestFindFilter(removeTree, filter, "C");
    }
    return ok;
}

bool ReferenceContainerTest::TestFindRemoveFirstOccurrence() {
    ReferenceContainerFilterObjects filter(1, ReferenceContainerFilterMode::RECURSIVE, "E");
    bool ok = TestFindFirstOccurrence(filter);
    if (ok) {
        ReferenceT<ReferenceContainer> removeTree = GenerateTestTreeUniqueReference();
        filter = ReferenceContainerFilterObjects(1, ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REMOVE, "E");
        PrintTree(removeTree);
        printf("\n");
        //Find and remove
        ok &= TestFindFilter(removeTree, filter, "C.E");
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH);
        //Should no longer find C.E but still find D.C.E
        ok &= !TestFindFilter(removeTree, filter, "C.E");
        filter.Reset();
        ok &= TestFindFilter(removeTree, filter, "D.C.E");
        removeTree = GenerateTestTreeUniqueReference();
        filter.Reset();
        PrintTree(removeTree);
        printf("\n");
        filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE | ReferenceContainerFilterMode::REMOVE);
        //Find and remove
        ok &= TestFindFilter(removeTree, filter, "D.C.E");
        PrintTree(removeTree);
        printf("\n");
        filter.Reset();
        filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::PATH | ReferenceContainerFilterMode::REVERSE);
        //Should no longer find D.C.E but still find C.E
        ok &= TestFindFilter(removeTree, filter, "C.E");
        filter.Reset();
        ok &= !TestFindFilter(removeTree, filter, "D.C.E");
    }
    return ok;
}
#endif

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

