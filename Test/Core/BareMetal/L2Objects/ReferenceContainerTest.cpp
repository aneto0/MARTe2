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
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectTestHelper.h"
#include "ReferenceContainerFilter.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerTest.h"
#include "StringHelper.h"
#include "Threads.h"

ReferenceContainerTest::ReferenceContainerTest() {
    h = NULL;
    tree = GenerateTestTree();
    spinLock = 0;
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

bool ReferenceContainerTest::TestCopyConstructor() {
    ReferenceT<ReferenceContainer> otherContainer("ReferenceContainer", h);
    ReferenceContainer rc;
    bool ok = (rc.Size() == 0u);
    if (ok) {
        ok &= (rc.GetTimeout() == TTInfiniteWait);
    }
    if (ok) {
        ok &= rc.Insert(otherContainer);
    }
    if (ok) {
        rc.SetTimeout(100);
        ReferenceContainer rc2 = rc;
        ok = (rc2.Size() == 1u);
        if (ok) {
            ok &= (rc2.GetTimeout() == rc.GetTimeout());
        }
    }
    return ok;
}

bool ReferenceContainerTest::TestOperatorEqual() {
    ReferenceT<ReferenceContainer> otherContainer("ReferenceContainer", h);
    ReferenceContainer rc;
    bool ok = (rc.Size() == 0u);
    if (ok) {
        ok &= (rc.GetTimeout() == TTInfiniteWait);
    }
    if (ok) {
        ok &= rc.Insert(otherContainer);
    }
    if (ok) {
        rc.SetTimeout(100);
        ReferenceContainer rc2;
        rc2 = rc;
        ok = (rc2.Size() == 1u);
        if (ok) {
            ok &= (rc2.GetTimeout() == rc.GetTimeout());
        }
    }
    return ok;
}
//bool ReferenceContainerTest::TestGetClassPropertiesCopy() {
//    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
//    ClassProperties cp;
//    container->GetClassPropertiesCopy(cp);
//    return (StringHelper::Compare(cp.GetName(), "ReferenceContainer") == 0);
//}

bool ReferenceContainerTest::TestGetClassProperties() {
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    const ClassProperties* cp = container->GetClassProperties();
    return (StringHelper::Compare(cp->GetName(), "ReferenceContainer") == 0);
}

bool ReferenceContainerTest::TestGetTimeout(TimeoutType timeout) {
    ReferenceT<ReferenceContainer> container("ReferenceContainer");
    bool ok = container.IsValid();
    if (ok) {
        container->SetTimeout(timeout);
        ok = (container->GetTimeout() == timeout);
    }
    return ok;
}

bool ReferenceContainerTest::TestSetTimeout(TimeoutType timeout) {
    ReferenceT<ReferenceContainer> container("ReferenceContainer");
    bool ok = container.IsValid();
    if (ok) {
        container->SetTimeout(timeout);
        ok = (container->GetTimeout() == timeout);
    }
    return ok;
}

bool ReferenceContainerTest::TestFind() {
    Reference result = tree->Find("H");
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::SHALLOW, "H");
    ReferenceContainer resTest;
    tree->Find(resTest, filter);

    return resTest.Get(0) == result;
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
            if (filter.IsStorePath()) {
                ok &= TestFindFilter(tree, filter, "D.F.G");
            }
            //It should never get here given that find all => !StorePath
            else {
                ok &= TestFindFilter(tree, filter, "G");
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
        if (filter.IsRecursive()) {
            if (filter.IsStorePath()) {
                if (filter.IsReverse()) {
                    ok &= TestFindFilter(tree, filter, "H");
                }
                else {
                    ok &= TestFindFilter(tree, filter, "C.E.H");
                }
            }
            //It should never get here given that find all => !StorePath
            else {
                ok &= TestFindFilter(tree, filter, "H.H.H");
            }

        }
        else {
            ok &= TestFindFilter(tree, filter, "H");
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
        ReferenceContainerFilterObjectName filter(1, i, "D.C.E.H");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathLegalObjectNameFilterStartDot() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjectName filter(1, i, ".D.C.E.H");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathLegalObjectNameFilterEndDot() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjectName filter(1, i, "D.C.E.H.");
        ok &= TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindPathIllegalObjectNameFilterTwoDots() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjectName filter(1, i, "D.C.E..H");
        ok &= TestFindFilter(tree, filter, "");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindWrongPathObjectNameFilter() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjectName filter(1, i, "D.C.F.H");
        ok &= !TestFindFilter(tree, filter, "D.C.E.H");
    }

    return ok;
}

bool ReferenceContainerTest::TestFindRelativePathObjectNameFilter() {
    bool ok = true;
    uint32 i = 0;
    for (i = 0; i < 8; i++) {
        ReferenceContainerFilterObjectName filter(1, i, "F.G");
        ok &= !TestFindFilter(tree, filter, "F.G");
    }

    return ok;
}

float ReferenceContainerTest::TestFindPerformance(ReferenceT<ReferenceContainer> largeTree, ReferenceContainerFilter &filter) {
    uint64 start = HighResolutionTimer::Counter();
    TestFindFilter(largeTree, filter, "U3");
    return (HighResolutionTimer::Counter() - start) * HighResolutionTimer::Period();
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

void PrintTree(ReferenceT<ReferenceContainer> tree) {
    uint32 i = 0;
    for (i = 0; i < tree->Size(); i++) {
        ReferenceT<ReferenceContainer> node = tree->Get(i);
        if (node.IsValid()) {
            //PrintTree(node);
        }
    }
}

bool ReferenceContainerTest::TestFindRemoveFromSubcontainer(ReferenceContainerFilter &filter) {
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REMOVE);
    //Remove node "E"
    bool ok = TestFindFilter(tree, filter, "E");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE);
    //Should no longer find any reference of E
    filter.SetOriginalSetOccurrence(-1);
    //Cannot call TestFindFilter as GenerateExpectedResultFromStringUsingExistingReferences needs E to be in the tree.
    ReferenceContainer result;
    tree->Find(result, filter);
    ok &= (result.Size() == 0);
    return ok;
}

bool ReferenceContainerTest::TestFindRemoveAllOfMultipleInstance(ReferenceContainerFilter &filter) {
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE | ReferenceContainerFilterMode::REMOVE);
    //Remove node "H" (notice that when removing from the E container two occurrences will be removed)
    bool ok = TestFindFilter(tree, filter, "H.H");
    filter.Reset();
    filter.SetMode(ReferenceContainerFilterMode::RECURSIVE);
    //Should no longer find any reference of H
    filter.SetOriginalSetOccurrence(-1);
    //Cannot call TestFindFilter as GenerateExpectedResultFromStringUsingExistingReferences needs H to be in the tree.
    ReferenceContainer result;
    tree->Find(result, filter);
    ok &= (result.Size() == 0);
    return ok;
}

bool ReferenceContainerTest::TestFindFilter(ReferenceT<ReferenceContainer> tree, ReferenceContainerFilter &filter, const char8 * const expectedResult) {
    ReferenceContainer expectedResultContainer;
    ReferenceContainer result;
    bool ok = false;
    bool filterByObjectName = (dynamic_cast<ReferenceContainerFilterObjectName *>(&filter) != NULL);

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

bool ReferenceContainerTest::TestFindWithPath() {
    return TestInsertWithPath();
}

bool ReferenceContainerTest::TestInsertWithPath() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    Reference ref("Object");
    containerRoot->Insert("A.B.C.MyObject", ref);

    if (containerRoot->Find("A.B.C.MyObject") != ref) {
        return false;
    }

    Reference ref2("Object");
    containerRoot->Insert("A.B.C.MyObject2", ref2);
    containerRoot->Insert("", ref);

    if (containerRoot->Find("A.B.C.MyObject2") != ref2) {
        return false;
    }
    containerRoot->Insert("", ref);
    return (containerRoot->Find("MyObject") == ref);
}

bool ReferenceContainerTest::TestInsertAtEnd() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    containerRoot->Insert(leafB);
    containerRoot->Insert(containerC);
    containerRoot->Insert(containerD);
    containerRoot->Insert(leafH);
    bool ok = (containerRoot->Get(0) == leafB);
    ok &= (containerRoot->Get(1) == containerC);
    ok &= (containerRoot->Get(2) == containerD);
    ok &= (containerRoot->Get(3) == leafH);

    ok &= (containerRoot->Size() == 4);

    return ok;
}

bool ReferenceContainerTest::TestInsertAtBeginning() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);

    containerRoot->Insert(leafB, 0);
    containerRoot->Insert(containerC, 0);
    containerRoot->Insert(containerD, 0);
    containerRoot->Insert(leafH, 0);
    bool ok = (containerRoot->Get(3) == leafB);
    ok &= (containerRoot->Get(2) == containerC);
    ok &= (containerRoot->Get(1) == containerD);
    ok &= (containerRoot->Get(0) == leafH);

    ok &= (containerRoot->Size() == 4);

    return ok;
}

bool ReferenceContainerTest::TestInsertAtMiddle() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);

    containerRoot->Insert(leafB, 0);
    containerRoot->Insert(containerD, 0);
    containerRoot->Insert(leafH, 0);

    containerRoot->Insert(containerC, 2);

    bool ok = (containerRoot->Get(3) == leafB);
    ok &= (containerRoot->Get(2) == containerC);
    ok &= (containerRoot->Get(1) == containerD);
    ok &= (containerRoot->Get(0) == leafH);

    ok &= (containerRoot->Size() == 4);
    return ok;
}

bool ReferenceContainerTest::TestInsertInvalid() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    Reference invalidRef;
    bool ok = !containerRoot->Insert(invalidRef);
    ok &= (containerRoot->Size() == 0);
    return ok;
}

bool ReferenceContainerTest::TestSize() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);

    containerRoot->Insert(leafB, 0);
    containerRoot->Insert(containerD, 0);
    containerRoot->Insert(leafH, 0);

    containerRoot->Insert(containerC, 2);

    return (containerRoot->Size() == 4);
}

bool ReferenceContainerTest::TestSizeZero() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);

    return (containerRoot->Size() == 0);
}

bool ReferenceContainerTest::TestGet() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    containerRoot->Insert(leafB);
    containerRoot->Insert(containerC);
    containerRoot->Insert(containerD);
    containerRoot->Insert(leafH);
    bool ok = (containerRoot->Get(0) == leafB);
    ok &= (containerRoot->Get(1) == containerC);
    ok &= (containerRoot->Get(2) == containerD);
    ok &= (containerRoot->Get(3) == leafH);

    return ok;
}

bool ReferenceContainerTest::TestGetInvalid() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    containerRoot->Insert(leafB);
    containerRoot->Insert(containerC);
    containerRoot->Insert(containerD);
    containerRoot->Insert(leafH);

    Reference invalid1 = containerRoot->Get(4);
    Reference invalid2 = containerRoot->Get(-1);

    return (!invalid1.IsValid() && !invalid2.IsValid());
}

bool ReferenceContainerTest::TestIsContainer() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    return (containerRoot->IsContainer(containerC) && !containerRoot->IsContainer(leafB));
}

bool ReferenceContainerTest::TestDelete() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    containerRoot->Insert(leafB);
    containerRoot->Insert(containerC);
    containerRoot->Insert(containerD);
    containerRoot->Insert(leafH);
    bool ok = (containerRoot->Delete(containerC));
    ok &= (containerRoot->Size() == 3);
    return ok;
}

bool ReferenceContainerTest::TestInitialise() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("+intObj1");
    cdb.Write("Class", "IntegerObject");
    int32 value = 1;
    cdb.Write("var", value);
    cdb.CreateAbsolute("$container");
    cdb.Write("Class", "ReferenceContainer");
    cdb.CreateRelative("+intObj2");
    cdb.Write("Class", "IntegerObject");
    value = 3;
    cdb.Write("var", value);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("+specIntObj3");
    cdb.Write("Class", "SpecialIntegerObject");
    cdb.MoveToRoot();
    ReferenceContainer container;
    if (!container.Initialise(cdb)) {
        return false;
    }

    ReferenceContainerFilterObjectName filter1(1, ReferenceContainerFilterMode::RECURSIVE, "intObj1");
    ReferenceContainer resultSingle1;
    container.Find(resultSingle1, filter1);
    ReferenceT<IntegerObject> test1 = (resultSingle1.Get(0));
    if (test1->GetVariable() != 1) {
        return false;
    }

    ReferenceContainerFilterObjectName filter2(1, ReferenceContainerFilterMode::RECURSIVE, "container.intObj2");
    ReferenceContainer resultSingle2;
    container.Find(resultSingle2, filter2);
    ReferenceT<IntegerObject> test2 = (resultSingle2.Get(1));
    if (test2->GetVariable() != 3) {
        return false;
    }

    ReferenceContainerFilterObjectName filter3(1, ReferenceContainerFilterMode::RECURSIVE, "container.specIntObj3");
    ReferenceContainer resultSingle3;
    container.Find(resultSingle3, filter3);
    ReferenceT<SpecialIntegerObject> test3 = (resultSingle3.Get(1));
    if (test3->GetVariable() != 2) {
        return false;
    }
    return true;
}

bool ReferenceContainerTest::TestPurge() {
    ReferenceContainer container;
    ConfigurationDatabase simpleCDB;
    simpleCDB.CreateAbsolute("+A");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("+A.+B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("+A.+B.+C");
    simpleCDB.Write("Class", "ReferenceContainer");
    uint32 leaf = 1;
    simpleCDB.Write("leaf", leaf);
    simpleCDB.CreateAbsolute("+B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.MoveToRoot();

    container.Initialise(simpleCDB);

    ReferenceT<ReferenceContainer> recursiveLeaf = container.Find("A");

    ReferenceT<ReferenceContainer> toLeaf = container.Find("A.B.C");
    //  -->A---
    //  |  |  |
    //  |  v  |
    //  |  B  |
    //  |  |  |
    //  |  v  |
    //  ---C<--
    recursiveLeaf->Insert(toLeaf);
    toLeaf->Insert(recursiveLeaf);

    if (toLeaf.NumberOfReferences() != 3) {
        return false;
    }

    container.Purge();

    if (toLeaf.NumberOfReferences() != 1) {
        return false;
    }
    return (container.Size() == 0);
}

static void PurgeRoutine(ReferenceContainerTest &param) {
    while (param.spinLock != 1) {
        Sleep::MSec(10);
    }
    param.containerU1->Purge();
    Threads::EndThread();
}

bool ReferenceContainerTest::TestPurge_Shared() {
    ReferenceT<ReferenceContainer> container = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
    containerU1 = container;

    ReferenceT<ReferenceContainer> nodeU1;
    ReferenceT<ReferenceContainer> nodeU2;
    for (uint32 i = 0; i < 100; i++) {
        nodeU1 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
        nodeU2 = ReferenceT<ReferenceContainer>("ReferenceContainer", h);
        nodeU1->SetName("");
        nodeU2->SetName("");
        container->Insert(nodeU1);
        container->Insert(nodeU2);
        container = nodeU1;
    }
    //recursive assignment
    container->Insert(containerU1);

    if (containerU1->NumberOfReferences() != 2) {
        return false;
    }

    //See how many threads are already running, as in some implementations
    //(e.g. FreeRTOS) there may be at least one task already there before.
    uint32 numOfThreadsBefore = Threads::NumberOfThreads();
    
    for (uint32 i = 0u; i < 3u; i++) {
        Threads::BeginThread((ThreadFunctionType) PurgeRoutine, this);
    }

    Atomic::Increment(&spinLock);

    uint32 numOfThreads = Threads::NumberOfThreads();

    //Wait until the same number of threads as before starting everything up
    //is still running.
    while (numOfThreads > numOfThreadsBefore) {
        Sleep::MSec(100);
        numOfThreads = Threads::NumberOfThreads();
    }

    if (containerU1.NumberOfReferences() != 1) {
        return false;
    }
    return (containerU1->Size() == 0);
}

bool ReferenceContainerTest::TestDeleteWithPath() {
    ReferenceT<ReferenceContainer> containerRoot("ReferenceContainer", h);
    ReferenceT<Object> ref("Object", h);
    containerRoot->Insert("A.B.C.D", ref);
    bool ok = (containerRoot->Delete("A.B.C.D"));

    ReferenceT<ReferenceContainer> C = containerRoot->Find("A.B.C");
    ok &= C->Size() == 0u;
    ok &= (containerRoot->Delete("A.B"));
    ReferenceT<ReferenceContainer> A = containerRoot->Get(0);
    ok &= A->Size() == 0u;
    return ok;
}

bool ReferenceContainerTest::TestExportData() {
    ReferenceContainer container;
    ConfigurationDatabase simpleCDB;
    simpleCDB.CreateAbsolute("+A");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("+A.+B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("+A.+B.+C");
    simpleCDB.Write("Class", "ReferenceContainer");
    uint32 leaf = 1;
    simpleCDB.Write("leaf", leaf);
    simpleCDB.CreateAbsolute("+B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("+B.+C");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.MoveToRoot();

    container.Initialise(simpleCDB);
    container.SetName("root");
    ConfigurationDatabase out;
    if (!container.ExportData(out)) {
        // return false;
    }
    StreamString output;
    output.Printf("%!", out);

    const char8 *test = ""
            "Name = \"root\"\r\n"
            "Class = \"ReferenceContainer\"\r\n"
            "0 = {\r\n"
            "    Name = \"A\"\r\n"
            "    Class = \"ReferenceContainer\"\r\n"
            "    IsContainer = 1\r\n"
            "}\r\n"
            "1 = {\r\n"
            "    Name = \"B\"\r\n"
            "    Class = \"ReferenceContainer\"\r\n"
            "    IsContainer = 1\r\n"
            "}\r\n";
    return output == test;
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

bool ReferenceContainerTest::VerifyExpectedResult(ReferenceContainer &source, ReferenceContainer &test) {
    bool ok = (source.Size() == test.Size());
    if (ok) {
        uint32 i;
        for (i = 0; i < source.Size(); i++) {
            ok &= (source.Get(i) == test.Get(i));
        }
    }

    return ok;
}

bool ReferenceContainerTest::VerifyExpectedResultByObjectName(ReferenceContainer &source, ReferenceContainer &test) {
    bool ok = (source.Size() == test.Size());
    if (ok) {
        uint32 i;
        for (i = 0; i < source.Size(); i++) {
            ok &= (StringHelper::Compare(source.Get(i)->GetName(), test.Get(i)->GetName()) == 0);
        }
    }

    return ok;
}

bool ReferenceContainerTest::GenerateExpectedResultFromStringUsingExistingReferences(ReferenceT<ReferenceContainer> source, ReferenceContainer &result, const char8 * const str) {
    bool ok = true;
    uint32 len = StringHelper::Length(str) + 1;
    char8 *name = new char[len];
    MemoryOperationsHelper::Set(name, '\0', len);

    uint32 i;
    uint32 j = 0;
    for (i = 0; i < (len - 1); i++) {
        if (str[i] == '.') {
            ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, name);
            ReferenceContainer resultSingle;
            //Cannot pass result directly into the Find because the filter uses the result as an history buffer!
            source->Find(resultSingle, filter);
            if (resultSingle.Size() > 0) {
                result.Insert(resultSingle.Get(0));
            }
            j = 0;
            MemoryOperationsHelper::Set(name, '\0', len);
        }
        else {
            name[j++] = str[i];
        }
    }
    //Last node must be a leaf
    //If nothing was found => first node is also a leaf
    ReferenceContainerFilterObjectName filter(1, ReferenceContainerFilterMode::RECURSIVE, name);
    ReferenceContainer resultSingle;
    //Cannot pass result directly into the Find because the filter uses the result as an history buffer!
    source->Find(resultSingle, filter);
    if (resultSingle.Size() > 0) {
        result.Insert(resultSingle.Get(0));
    }

    delete[] name;
    return ok;
}

bool ReferenceContainerTest::GenerateExpectedResultFromString(ReferenceContainer &result, const char8 * const str) {
    bool ok = true;
    uint32 len = StringHelper::Length(str) + 1;
    char8 *name = new char[len];
    MemoryOperationsHelper::Set(name, '\0', len);

    uint32 i;
    uint32 j = 0;
    for (i = 0; i < (len - 1); i++) {
        if (str[i] == '.') {
            ReferenceT<ReferenceContainer> node("ReferenceContainer");
            node->SetName(name);
            result.Insert(node);
            j = 0;
            MemoryOperationsHelper::Set(name, '\0', len);
        }
        else {
            name[j++] = str[i];
        }
    }
    //Last node must be a leaf
    //If nothing was found => first node is also a leaf
    if (len > 1) {
        ReferenceT<Object> leaf("Object");
        leaf->SetName(name);
        result.Insert(leaf);
    }
    delete[] name;
    return ok;
}

bool ReferenceContainerTest::TestIsReferenceContainer() {
    using namespace MARTe;
    ReferenceContainer rc;
    return rc.IsReferenceContainer();
}

bool ReferenceContainerTest::TestIsBuildToken() {
    ReferenceContainer container;
    ReferenceContainer::AddBuildToken('_');
    ReferenceContainer::AddBuildToken('*');
    ReferenceContainer::AddBuildToken('*');
    ReferenceContainer::AddBuildToken('*');
    ReferenceContainer::AddBuildToken('*');
    ReferenceContainer::AddBuildToken('*');
    ReferenceContainer::AddBuildToken('%');
    ReferenceContainer::AddBuildToken('_');
    ReferenceContainer::AddBuildToken('_');
    ReferenceContainer::AddBuildToken('_');
    ReferenceContainer::AddBuildToken('_');

    ConfigurationDatabase simpleCDB;
    simpleCDB.CreateAbsolute("+A");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("_B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("*C");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("%D");
    simpleCDB.Write("Class", "ReferenceContainer");
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    ord->Purge();
    simpleCDB.MoveToRoot();
    bool ok = ord->Initialise(simpleCDB);
    if (ok) {
        ok = (ord->Size() == 4u);
    }
    if (ok) {
        ok = ReferenceContainer::AddBuildToken('#');
    }
    if (ok) {
        //No more space...
        ok = !ReferenceContainer::AddBuildToken('@');
    }
    if (ok) {
        ReferenceContainer::RemoveBuildToken('*');
        ok = ReferenceContainer::AddBuildToken('@');
    }
    ReferenceContainer::RemoveBuildToken('@');
    ReferenceContainer::RemoveBuildToken('_');
    ReferenceContainer::RemoveBuildToken('#');
    ReferenceContainer::RemoveBuildToken('%');

    ord->Purge();
    return ok;
}

bool ReferenceContainerTest::TestAddBuildToken() {
    return TestIsBuildToken();
}

bool ReferenceContainerTest::TestRemoveBuildToken() {
    return TestIsBuildToken();
}

bool ReferenceContainerTest::TestIsDomainToken() {
    ReferenceContainer container;
    ReferenceContainer::AddDomainToken('_');
    ReferenceContainer::AddDomainToken('_');
    ReferenceContainer::AddDomainToken('_');
    ReferenceContainer::AddDomainToken('*');
    ReferenceContainer::AddDomainToken('%');
    ReferenceContainer::AddDomainToken('*');
    ReferenceContainer::AddDomainToken('*');

    ConfigurationDatabase simpleCDB;
    simpleCDB.CreateAbsolute("+A");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("$B");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("_C");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("%D");
    simpleCDB.Write("Class", "ReferenceContainer");
    simpleCDB.CreateAbsolute("*E");
    simpleCDB.Write("Class", "ReferenceContainer");
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    ord->Purge();
    simpleCDB.MoveToRoot();
    bool ok = ord->Initialise(simpleCDB);
    if (ok) {
        ok = (ord->Size() == 5u);
    }
    if (ok) {
        ok = !(ord->Get(0u)->IsDomain());
    }
    if (ok) {
        ok = (ord->Get(1u)->IsDomain());
    }
    if (ok) {
        ok = (ord->Get(2u)->IsDomain());
    }
    if (ok) {
        ok = (ord->Get(3u)->IsDomain());
    }
    if (ok) {
        ok = (ord->Get(4u)->IsDomain());
    }
    if (ok) {
        ok = ReferenceContainer::AddDomainToken('#');
    }
    if (ok) {
        //No more space...
        ok = !ReferenceContainer::AddDomainToken('@');
    }
    if (ok) {
        ReferenceContainer::RemoveDomainToken('*');
        ok = ReferenceContainer::AddDomainToken('@');
    }
    if (ok) {
        ok = ReferenceContainer::AddDomainToken('@');
    }
    if (ok) {
        ok = ReferenceContainer::AddDomainToken('@');
    }
    if (ok) {
        ok = ReferenceContainer::AddDomainToken('@');
    }
    ReferenceContainer::RemoveDomainToken('@');
    ReferenceContainer::RemoveDomainToken('_');
    ReferenceContainer::RemoveDomainToken('#');
    ReferenceContainer::RemoveDomainToken('%');

    ord->Purge();
    return ok;
}

bool ReferenceContainerTest::TestAddDomainToken() {
    return TestIsDomainToken();
}

bool ReferenceContainerTest::TestRemoveDomainToken() {
    return TestIsDomainToken();
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

