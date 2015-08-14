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
#include "ReferenceContainerFilter.h"

ReferenceContainerTest::ReferenceContainerTest() :
        leafB("Object", h),
        containerC("ReferenceContainer", h),
        containerD("ReferenceContainer", h),
        containerE("ReferenceContainer", h),
        containerF("ReferenceContainer", h),
        leafG("Object", h),
        leafH("Object", h),
        leafNoExist("Object", h) {

    leafB->SetName("B");
    containerC->SetName("C");
    containerD->SetName("D");
    containerE->SetName("E");
    containerF->SetName("F");
    leafG->SetName("G");
    leafH->SetName("H");
    leafNoExist->SetName("NoExist");
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
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    ReferenceContainer result;
    container->Find(result, filter);

    bool ok = true;
    //leafB should be found no matter what
    if (result.Size() > 0) {
        ok = (result.Get(0) == leafB);
    }
    else {
        ok = false;
    }
    return ok;
}

bool ReferenceContainerTest::TestFindFirstOccurrenceBranchAlways(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    ReferenceContainer result;
    container->Find(result, filter);
    bool ok = true;
    //leafH should be found no matter what
    if (result.Size() > 0) {
        if (!filter.IsStorePath()) {
            ok &= (result.Get(0) == leafH);
        }
        else if (!filter.IsRecursive()) {
            ok &= (result.Get(0) == leafH);
        }
        else {
            if (!filter.IsReverse()) {
                if (result.Size() == 3) {
                    ok &= (result.Get(0) == containerC);
                    ok &= (result.Get(1) == containerE);
                    ok &= (result.Get(2) == leafH);
                }
                else {
                    ok = false;
                }
            }
            else {
                if (result.Size() == 1) {
                    ok &= (result.Get(0) == leafH);
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

bool ReferenceContainerTest::TestFindFirstOccurrence(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    bool ok = true;
    //Look for containerE
    ReferenceContainer result;
    container->Find(result, filter);
    //containerE should only be found if recursive is set to true
    if (filter.IsRecursive()) {
        if (!filter.IsStorePath()) {
            ok &= (result.Get(0) == containerE);
        }
        else {
            if (!filter.IsReverse()) {
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

bool ReferenceContainerTest::TestFindSecondOccurrence(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    //Look for the second instance of leafH
    ReferenceContainer result;
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

bool ReferenceContainerTest::TestFindThirdOccurrence(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);
    bool ok = true;

    //Look for the third instance of leafH
    ReferenceContainer result;
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

bool ReferenceContainerTest::TestFindAllOfASingleInstance(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);
    bool ok = true;

    //Look for the only instance of leafG
    ReferenceContainer result;
    container->Find(result, filter);
    //It should only be found if recursive is set to true.
    if (filter.IsRecursive()) {
        if (!filter.IsStorePath()) {
            ok &= (result.Get(0) == leafG);
        }
        else {
            if (result.Size() == 3) {
                ok &= (result.Get(0) == containerC);
                ok &= (result.Get(1) == containerF);
                ok &= (result.Get(2) == leafG);
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

bool ReferenceContainerTest::TestFindAllOfMultipleInstance(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);
    bool ok = true;

    //Look for all the instances of leafH
    ReferenceContainer result;
    container->Find(result, filter);
    //It not recursive only one should be found
    if (!filter.IsRecursive()) {
        ok &= (result.Get(0) == leafH);
    }
    else {
        //Store path should not be enabled for this type of search
        if (!filter.IsStorePath()) {
            if (result.Size() == 3) {
                ok &= (result.Get(0) == leafH);
                ok &= (result.Get(1) == leafH);
                ok &= (result.Get(2) == leafH);
            }
            else {
                ok = false;
            }
        }
        else {
            ok = false;
        }
    }
    return ok;
}

bool ReferenceContainerTest::TestFindOutOfBoundsOccurrence(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    //The occurrence of leafH provided by the filter (> 3 || <-1) will not exist
    ReferenceContainer result;
    container->Find(result, filter);
    return (result.Size() == 0);
}

bool ReferenceContainerTest::TestFindNonExistent(ReferenceContainerFilter &filter) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);

    //Look for a node that does not exist.
    ReferenceContainer result;
    container->Find(result, filter);
    return (result.Size() == 0);
}

bool ReferenceContainerTest::TestFindPath(ReferenceContainerFilterObjects &filter,
                                          ReferenceContainer &expectedResult) {
    Heap h;
    ReferenceT<ReferenceContainer> container("ReferenceContainer", h);
    GenerateTestTree(container);
    bool ok = true;

    //Look for all the instances of leafH
    ReferenceContainer result;
    container->Find(result, filter);
    ok = VerifyExpectedResult(result, expectedResult);

    return ok;

}

void ReferenceContainerTest::GenerateTestTree(ReferenceT<ReferenceContainer> container) {
    containerE->Insert(leafH);
    containerC->Insert(containerE);

    containerD->Insert(containerF);
    containerD->Insert(containerC);

    containerF->Insert(leafG);

    container->Insert(leafB);
    container->Insert(containerC);
    container->Insert(containerD);
    container->Insert(leafH);

}

bool ReferenceContainerTest::VerifyExpectedResult(ReferenceContainer &source,
                                                  ReferenceContainer &test) {
    bool ok = (source.Size() == test.Size());
    if (!ok) {
        uint32 i;
        for (i = 0; i < source.Size(); i++) {
            ok &= (source.Get(i) == test.Get(i));
        }
    }

    return ok;
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

