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
#include "ReferenceContainerFilterReferences.h"
#include "ReferenceContainerFilterObjects.h"

ReferenceContainerTest::ReferenceContainerTest() :
        leafB("Object", h),
        containerC("ReferenceContainer", h),
        containerD("ReferenceContainer", h),
        containerE("ReferenceContainer", h),
        containerF("ReferenceContainer", h),
        leafH("Object", h) {

    ReferenceContainerFilterObjects test(1, 0, "F");
    ReferenceContainerFilterObjects test1(1, 0, "F.A");
    ReferenceContainerFilterObjects test2(1, 0, "F.A.B");
    ReferenceContainerFilterObjects test3(1, 0, "F.A.B.");
    ReferenceContainerFilterObjects test4(1, 0, ".F.A.B.");
    ReferenceContainerFilterObjects test5(1, 0, ".");
}

bool ReferenceContainerTest::TestConstructor() {
    //TODO
    return true;
}

bool ReferenceContainerTest::TestFindFirstOccurrenceAlways(uint32 modeFlags) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    ReferenceContainer result;
    ReferenceContainerFilterReferences filter(1, modeFlags, leafB);
    container->Find(result, filter);

    bool ok = true;
    //leafB should be found no matter what
    if (result.Size() > 0) {
        ok = (result.Get(0) == leafB);
    }
    else {
        ok = false;
    }

    ReferenceContainerFilterReferences filter1(1, modeFlags, leafH);
    ReferenceContainer result1;
    container->Find(result1, filter1);
    //leafH should be found no matter what
    if (result1.Size() > 0) {
        if (!filter1.IsStorePath()) {
            ok &= (result1.Get(0) == leafH);
        }
        else if(!filter1.IsRecursive()){
            ok &= (result1.Get(0) == leafH);
        }
        else {
            if (!filter1.IsReverse()) {
                if (result1.Size() == 3) {
                    ok &= (result1.Get(0) == containerC);
                    ok &= (result1.Get(1) == containerE);
                    ok &= (result1.Get(2) == leafH);
                }
                else {
                    ok = false;
                }
            }
            else {
                if (result1.Size() == 1) {
                    ok &= (result1.Get(0) == leafH);
                }
                else {
                    ok = false;
                }
            }
        }
    }
    else {
        ok = false;
    }

    return ok;
}

bool ReferenceContainerTest::TestFindFirstOccurrence(uint32 modeFlags) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    bool ok = true;
    //Look for containerE
    ReferenceContainer result;
    ReferenceContainerFilterReferences filter2(1, modeFlags, containerE);
    container->Find(result, filter2);
    //containerE should only be found if recursive is set to true
    if (filter2.IsRecursive()) {
        if (!filter2.IsStorePath()) {
            ok &= (result.Get(0) == containerE);
        }
        else {
            if (!filter2.IsReverse()) {
                if (result.Size() == 2) {
                    ok &= (result.Get(0) == containerC);
                    ok &= (result.Get(1) == containerE);
                    ok &= (containerE->Get(0) == leafH);
                }
                else {
                    ok = false;
                }
            }
            else {
                if (result.Size() == 3) {
                    ok &= (result.Get(0) == containerD);
                    ok &= (result.Get(1) == containerC);
                    ok &= (result.Get(2) == containerE);
                    ok &= (containerE->Get(0) == leafH);
                }
                else {
                    ok = false;
                }
            }
        }
    }
    else if (result.Size() > 0) {
        ok = false;
    }

    return ok;
}

bool ReferenceContainerTest::TestFindSecondOccurrence(uint32 modeFlags) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    //Look for the second instance of leafH
    ReferenceContainer result;
    ReferenceContainerFilterReferences filter(2, modeFlags, leafH);
    container->Find(result, filter);
    bool ok = true;
    //The second instance of leafH should only be found if recursive is set to true.
    if (filter.IsRecursive()) {
        if (!filter.IsStorePath()) {
            ok &= (result.Get(0) == leafH);
        }
        else {
            //Reverse or not reverse the second leafH is always in the middle
            if (result.Size() == 4) {
                ok &= (result.Get(0) == containerD);
                ok &= (result.Get(1) == containerC);
                ok &= (result.Get(2) == containerE);
                ok &= (result.Get(3) == leafH);
            }
            else {
                ok = false;
            }

        }
    }
    else if (result.Size() > 0) {
        ok = false;
    }

    return ok;
}

bool ReferenceContainerTest::TestFindThirdOccurrence(uint32 modeFlags) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);
    bool ok = true;

    //Look for the third instance of leafH
    ReferenceContainer result;
    ReferenceContainerFilterReferences filter(3, modeFlags, leafH);
    container->Find(result, filter);
    //The third instance of leafH should only be found if recursive is set to true.
    if (filter.IsRecursive()) {
        if (!filter.IsStorePath()) {
            ok &= (result.Get(0) == leafH);
        }
        else {
            //From LR it is the last
            if (!filter.IsReverse()) {
                if (result.Size() == 1) {
                    ok &= (result.Get(0) == leafH);
                }
                else {
                    ok = false;
                }
            }
            else {
                if (result.Size() == 3) {
                    ok &= (result.Get(0) == containerC);
                    ok &= (result.Get(1) == containerE);
                    ok &= (result.Get(2) == leafH);
                }
                else {
                    ok = false;
                }
            }
        }
    }
    else if (result.Size() > 0) {
        ok = false;
    }

    return ok;
}

bool ReferenceContainerTest::TestFindAll(uint32 mode) {
    //TODO
    return true;
}

bool ReferenceContainerTest::TestFindOutOfBoundsOccurrence(uint32 mode) {
    //TODO
    return true;
}

bool ReferenceContainerTest::TestFindNonExistant(uint32 mode) {
    //TODO
    return true;
}

void ReferenceContainerTest::GenerateTestTree(ReferenceT<ReferenceContainer> container) {
    containerE->Insert(leafH);
    containerC->Insert(containerE);
    containerD->Insert(containerC);

    containerF->Insert(leafG);
    containerD->Insert(containerF);

    container->Insert(leafB);
    container->Insert(containerC);
    container->Insert(containerD);
    container->Insert(leafH);

}
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

