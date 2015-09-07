/**
 * @file ReferenceContainerFilterReferencesTest.cpp
 * @brief Source file for class ReferenceContainerFilterReferencesTest
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
 * the class ReferenceContainerFilterReferencesTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterReferencesTest.h"
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ReferenceContainerFilterReferencesTest::TestDefaultConstructor() {

    //checks if the default values are set in the father class.
    ReferenceContainerFilterReferences myRefFilter;

    //checks if the mode is zero 8all flags false and if the number of occurrences is zero.
    return (myRefFilter.GetMode() == 0) && (myRefFilter.IsFinished());

}

bool ReferenceContainerFilterReferencesTest::TestFullConstructor(int32 occurrenceNumber,
                                                                 uint32 mode) {

    Reference ref = Reference("Object");
    ReferenceContainerFilterReferences myRefFilter(occurrenceNumber, mode, ref);

    //consider the exclusive conditions
    if (((mode & ReferenceContainerFilterMode::PATH) == ReferenceContainerFilterMode::PATH) && (myRefFilter.IsSearchAll())) {
        mode &= ~(ReferenceContainerFilterMode::PATH);
    }

    if (myRefFilter.IsStorePath()) {
        mode |= ReferenceContainerFilterMode::RECURSIVE;
    }

    if (myRefFilter.GetMode() != mode) {
        return false;
    }

    if (myRefFilter.GetOriginalSetOccurrence() != occurrenceNumber) {
        return false;
    }

    ReferenceContainer myRefContainer;

    //the test does not consider the reference container but only the reference.
    return myRefFilter.Test(myRefContainer, ref);
}

bool ReferenceContainerFilterReferencesTest::TestTest(uint32 nRefs,
                                                      int32 occurrence) {

    Reference **arrayRefs = (Reference**) HeapManager::Malloc(sizeof(Reference*) * nRefs);

    if (arrayRefs == NULL) {
        return false;
    }

    Reference original = Reference("Object");
    for (uint32 i = 0; i < nRefs; i++) {
        arrayRefs[i] = new Reference(original);
    }

    ReferenceContainerFilterReferences myRefFilter(occurrence, 0, original);

    uint32 i = 0;
    uint32 cnt = 0;
    bool ret = true;

    while ((!myRefFilter.IsFinished())) {
        ReferenceContainer myTree;

        //avoid the array overflow
        if (i >= nRefs) {
            i = nRefs - 1;
        }
        if (!myRefFilter.Test(myTree, *arrayRefs[i])) {
            ret = false;
            break;
        }
        i++;
        cnt++;
    }

    for (uint32 i = 0; i < nRefs; i++) {
        HeapManager::Free((void*&) arrayRefs[i]);
    }

    HeapManager::Free((void*&) arrayRefs);

    return (ret) && (cnt == (uint32) occurrence);
}

bool ReferenceContainerFilterReferencesTest::TestTestFailure() {

    Reference tesRef1 = Reference("Object");
    Reference tesRef2 = Reference("Object");

    ReferenceContainer myTree;
    ReferenceContainerFilterReferences myRefFilter(1, 0, tesRef1);

    return (!myRefFilter.Test(myTree, tesRef2)) && (!myRefFilter.IsFinished());

}

bool ReferenceContainerFilterReferencesTest::TestIncrementFound() {

    Reference testRef = Reference("Object");
    ReferenceContainer myTree;

    ReferenceContainerFilterReferences myRefFilter(1, 0, testRef);

    if (!myRefFilter.Test(myTree, testRef)) {
        return false;
    }

    if (!myRefFilter.IsFinished()) {
        return false;
    }

    //just to check that with a negative occurrence
    //the filter never finishes
    uint32 cycles = 500;

    myRefFilter.SetOriginalSetOccurrence(-1);
    myRefFilter.Reset();

    for (uint32 i = 0; i < cycles; i++) {
        if (!myRefFilter.Test(myTree, testRef)) {
            return false;
        }
        if (myRefFilter.IsFinished()) {
            return false;
        }
    }

    return true;
}

bool ReferenceContainerFilterReferencesTest::TestIsRemove() {
    Reference dummyRef;
    ReferenceContainerFilterReferences myRefFilter(-1, 0, dummyRef);

    if (myRefFilter.IsRemove()) {
        return false;
    }

    for (uint32 i = 0; i < 8u; i++) {
        uint32 mode = i | (ReferenceContainerFilterMode::REMOVE);

        myRefFilter.SetMode(mode);

        if (!myRefFilter.IsRemove()) {
            return false;
        }

        if (!myRefFilter.IsSearchAll()) {
            return false;
        }
    }
    return true;
}

bool ReferenceContainerFilterReferencesTest::TestIsSearchAll() {

    Reference dummyRef;

    ReferenceContainerFilterReferences myRefFilter(-1, 0, dummyRef);

    for (uint32 i = 8; i < 8u; i++) {
        uint32 mode = i;

        myRefFilter.SetMode(mode);

        if (mode != myRefFilter.GetMode()) {
            return false;
        }

        if (!myRefFilter.IsSearchAll()) {
            return false;
        }
    }

    //for 0x1=PATH the SearchAll condition is exclusive so it is not enabled
    myRefFilter.SetMode(ReferenceContainerFilterMode::PATH);

    return (myRefFilter.IsSearchAll()) && (!myRefFilter.IsStorePath()) && (myRefFilter.GetMode() == 0u);
}

bool ReferenceContainerFilterReferencesTest::TestIsStorePath() {

    Reference dummyRef;
    ReferenceContainerFilterReferences myRefFilter(1, 0, dummyRef);

    if (myRefFilter.IsStorePath()) {
        return false;
    }

    for (uint32 i = 0u; i < 8u; i++) {
        uint32 mode = i | (ReferenceContainerFilterMode::PATH);

        myRefFilter.SetMode(mode);

        if (!myRefFilter.IsStorePath()) {
            return false;
        }

    }

    myRefFilter.SetOriginalSetOccurrence(-1);
    myRefFilter.Reset();

    myRefFilter.SetMode(ReferenceContainerFilterMode::PATH);

    return (myRefFilter.IsSearchAll()) && (myRefFilter.GetMode() == 0u);

}

bool ReferenceContainerFilterReferencesTest::TestIsReverse() {

    Reference dummyRef;
    ReferenceContainerFilterReferences myRefFilter(1, 0, dummyRef);

    if (myRefFilter.IsReverse()) {
        return false;
    }

    for (uint32 i = 0u; i < 8u; i++) {
        uint32 mode = i | (ReferenceContainerFilterMode::REVERSE);

        myRefFilter.SetMode(mode);

        if (!myRefFilter.IsReverse()) {
            return false;
        }

    }

    // the search all condition should not be in contrast.
    myRefFilter.SetOriginalSetOccurrence(-1);
    myRefFilter.Reset();

    myRefFilter.SetMode(ReferenceContainerFilterMode::REMOVE);

    return (myRefFilter.GetMode() == ReferenceContainerFilterMode::REMOVE) && (myRefFilter.IsRemove());

}

bool ReferenceContainerFilterReferencesTest::TestIsRecursive() {

    Reference dummyRef;
    ReferenceContainerFilterReferences myRefFilter(1, 0, dummyRef);

    if (myRefFilter.IsRecursive()) {
        return false;
    }

    for (uint32 i = 0u; i < 8u; i++) {
        uint32 mode = i | (ReferenceContainerFilterMode::RECURSIVE);

        myRefFilter.SetMode(mode);

        if (!myRefFilter.IsRecursive()) {
            return false;
        }

    }

    //path mode sets recursive mode automatically
    myRefFilter.SetMode(ReferenceContainerFilterMode::PATH);

    if (!myRefFilter.IsRecursive()) {
        return false;
    }

    // the search all condition should not be in contrast.
    myRefFilter.SetOriginalSetOccurrence(-1);
    myRefFilter.Reset();

    myRefFilter.SetMode(ReferenceContainerFilterMode::REMOVE);

    return (myRefFilter.GetMode() == ReferenceContainerFilterMode::RECURSIVE) && (myRefFilter.IsRecursive());

}

bool ReferenceContainerFilterReferencesTest::TestSetGetMode(int32 occurrence) {

    Reference dummyRef;
    ReferenceContainerFilterReferences myRefFilter(occurrence, 0, dummyRef);

    if (myRefFilter.GetMode() != 0) {
        return false;
    }

    for (uint32 i = 0u; i < 8u; i++) {

        uint32 mode = i;

        myRefFilter.SetMode(mode);
        if (occurrence == -1) {
            mode &= ~(ReferenceContainerFilterMode::PATH);
        }

        if ((mode & ReferenceContainerFilterMode::PATH) == ReferenceContainerFilterMode::PATH) {
            mode |= ReferenceContainerFilterMode::RECURSIVE;
        }

        if (myRefFilter.GetMode() != mode) {
            return false;
        }
    }
    return true;

}

bool ReferenceContainerFilterReferencesTest::TestReset(uint32 occurrence) {

    Reference testRef = Reference("Object");
    ReferenceContainerFilterReferences myRefFilter(occurrence, 0, testRef);
    ReferenceContainer myTree;

    uint32 cnt = 0;
    while (!myRefFilter.IsFinished()) {

        if (!myRefFilter.Test(myTree, testRef)) {
            return false;
        }

        cnt++;
    }

    if (cnt != occurrence) {
        return false;
    }

    cnt = 0;
    myRefFilter.Reset();
    while (!myRefFilter.IsFinished()) {

        if (!myRefFilter.Test(myTree, testRef)) {
            return false;
        }

        cnt++;
    }

    return cnt == occurrence;

}

bool ReferenceContainerFilterReferencesTest::TestSetGetOriginalSetOccurrence(int32 initialOccurrence,
                                                                             int32 finalOccurrence) {
    Reference testRef = Reference("Object");
    ReferenceContainerFilterReferences myRefFilter(initialOccurrence, ReferenceContainerFilterMode::PATH, testRef);
    ReferenceContainer myTree;

    uint32 cnt = 0;

    //saturate them to avoid infinite loops
    if (initialOccurrence < -1) {
        initialOccurrence = -1;
    }

    if (finalOccurrence < -1) {
        finalOccurrence = -1;
    }

    if (initialOccurrence == -1) {
        if (myRefFilter.IsStorePath()) {
            return false;
        }
        cnt = initialOccurrence;
    }
    else {
        if (!myRefFilter.IsStorePath()) {
            return false;
        }

        while (!myRefFilter.IsFinished()) {

            if (!myRefFilter.Test(myTree, testRef)) {
                return false;
            }

            cnt++;
        }
    }

    if (cnt != (uint32) initialOccurrence) {
        return false;
    }

    if (myRefFilter.GetOriginalSetOccurrence() != (int32) initialOccurrence) {
        return false;
    }
    myRefFilter.SetOriginalSetOccurrence(finalOccurrence);
    if (myRefFilter.GetOriginalSetOccurrence() != (int32) finalOccurrence) {
        return false;
    }

    cnt = 0;
    if (finalOccurrence == -1) {

        // the mode must be refreshed.
        if (myRefFilter.IsStorePath()) {
            return false;
        }
        cnt = finalOccurrence;
    }
    else {

        //if also the initial occurrence was >= 0 the mode should be the same
        if ((initialOccurrence >= 0) && (!myRefFilter.IsStorePath())) {
            return false;
        }

        while (!myRefFilter.IsFinished()) {

            if (!myRefFilter.Test(myTree, testRef)) {
                return false;
            }

            cnt++;
        }
    }

    return (cnt == (uint32) finalOccurrence) && ((myRefFilter.GetOriginalSetOccurrence() == finalOccurrence));
}

