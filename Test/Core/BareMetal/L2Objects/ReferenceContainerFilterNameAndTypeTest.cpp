/**
 * @file ReferenceContainerFilterNameAndTypeTest.cpp
 * @brief Source file for class ReferenceContainerFilterNameAndTypeTest
 * @date 27/08/2018
 * @author Giuseppe Ferro
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
 * the class ReferenceContainerFilterNameAndTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterNameAndTypeTest.h"
#include "StreamString.h"
#include "ReferenceT.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

ReferenceContainerFilterNameAndTypeTest::ReferenceContainerFilterNameAndTypeTest() {
}

ReferenceContainerFilterNameAndTypeTest::~ReferenceContainerFilterNameAndTypeTest() {
}

class ReferenceContainerFilterNameAndTypeTestObj: public ReferenceContainer {

public:

    ReferenceContainerFilterNameAndTypeTestObj();

    virtual ~ReferenceContainerFilterNameAndTypeTestObj();
};

ReferenceContainerFilterNameAndTypeTestObj::ReferenceContainerFilterNameAndTypeTestObj() {

}

ReferenceContainerFilterNameAndTypeTestObj::~ReferenceContainerFilterNameAndTypeTestObj() {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ReferenceContainerFilterNameAndTypeTest::TestDefaultConstructor() {
    ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test;
    return test.GetRemainedAddrIndex() == 0u;
}

bool ReferenceContainerFilterNameAndTypeTest::TestSetConstructor() {
    uint32 modes[] = { ReferenceContainerFilterMode::SHALLOW, ReferenceContainerFilterMode::RECURSIVE, ReferenceContainerFilterMode::REVERSE,
            ReferenceContainerFilterMode::PATH, ReferenceContainerFilterMode::REMOVE };

    bool ret = true;
    for (uint32 i = 0u; (i < 5u) && (ret); i++) {
        int32 occurrence = -1;
        for (uint32 j = 0u; (j < 4u) && (ret); j++) {
            StreamString address = "Node";
            for (uint32 k = 0u; (k < 3u) && (ret); k++) {
                ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test(occurrence, modes[i], address.Buffer());
                if (modes[i] != 0) {
                    ret = ((test.GetMode() & modes[i]) != 0);
                }
                if (ret) {
                    if ((modes[i] == ReferenceContainerFilterMode::PATH) || (k > 0u)) {
                        ret = ((test.GetMode() & ReferenceContainerFilterMode::RECURSIVE) != 0);
                        if (ret) {
                            ret = ((test.GetMode() & ReferenceContainerFilterMode::PATH) != 0);
                        }
                        if (ret) {
                            if (occurrence < 0) {
                                ret = (test.GetOriginalSetOccurrence() == 1);
                            }
                            else {
                                ret = (test.GetOriginalSetOccurrence() == occurrence);
                            }
                        }
                    }
                    else {
                        ret = (test.GetOriginalSetOccurrence() == occurrence);
                    }
                }
                if (ret) {
                    ret = (test.GetRemainedAddrIndex() == 0u);
                }
                address += ".Node";
            }
            occurrence++;
        }

    }

    return ret;
}

bool ReferenceContainerFilterNameAndTypeTest::TestCopyConstructor() {
    ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test(-1, ReferenceContainerFilterMode::SHALLOW, "Node1.Node2");
    ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test2(test);

    bool ret = test2.IsStorePath();
    if (ret) {
        ret = test2.IsRecursive();
    }
    if (ret) {
        ret = test2.GetOriginalSetOccurrence() == 1;
    }

    return ret;
}

bool ReferenceContainerFilterNameAndTypeTest::TestTest() {
    /*build a tree
         A
        / \
       B   C*
      / \   \
     D*  F*  G*
     |
     E
     */

    ReferenceT<ReferenceContainer> a(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    a->SetName("A");
    ReferenceT<ReferenceContainer> b(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    b->SetName("B");
    a->Insert(b);
    ReferenceT<ReferenceContainerFilterNameAndTypeTestObj> c(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    c->SetName("C");
    a->Insert(c);
    ReferenceT<ReferenceContainerFilterNameAndTypeTestObj> d(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    d->SetName("D");
    b->Insert(d);
    ReferenceT<ReferenceContainer> e(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    e->SetName("E");
    d->Insert(e);
    ReferenceT<ReferenceContainerFilterNameAndTypeTestObj> f(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    f->SetName("F");
    b->Insert(f);
    ReferenceT<ReferenceContainerFilterNameAndTypeTestObj> g(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    g->SetName("G");
    c->Insert(g);

    //first test... find C
    ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test(1, ReferenceContainerFilterMode::PATH, "C");
    ReferenceContainer result;
    a->Find(result, test);
    bool ret = result.Get(0) == c;
    if (ret) {
        ret = test.GetRemainedAddrIndex() == 1;
    }

    if (ret) {
        //find G, C should return
        ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test1(1, ReferenceContainerFilterMode::PATH, "C.G");
        ReferenceContainer result1;
        a->Find(result1, test1);
        ret = result1.Get(0) == c;
        if (ret) {
            ret = test1.GetRemainedAddrIndex() == 2;
        }
    }

    if (ret) {
        //find D
        ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test2(1, ReferenceContainerFilterMode::PATH, "B.D");
        ReferenceContainer result2;
        a->Find(result2, test2);
        ret = result2.Get(1) == d;
        if (ret) {
            ret = test2.GetRemainedAddrIndex() == 3;
        }
    }

    if (ret) {
        //find D
        ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test3(1, ReferenceContainerFilterMode::PATH, "B.D.E");
        ReferenceContainer result3;
        a->Find(result3, test3);
        ret = result3.Get(1) == d;
        if (ret) {
            ret = test3.GetRemainedAddrIndex() == 4;
        }
    }


    if (ret) {
        //find D
        ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test4(1, ReferenceContainerFilterMode::PATH, "B.F");
        ReferenceContainer result4;
        a->Find(result4, test4);
        ret = result4.Get(1) == f;
        if (ret) {
            ret = test4.GetRemainedAddrIndex() == 3;
        }
    }


    if (ret) {
        //find D
        ReferenceContainerFilterNameAndType<ReferenceContainerFilterNameAndTypeTestObj> test5(1, ReferenceContainerFilterMode::PATH, "B");
        ReferenceContainer result5;
        a->Find(result5, test5);
        ret = result5.Size() == 0;

    }

    return ret;
}

bool ReferenceContainerFilterNameAndTypeTest::TestGetRemainedAddrIndex() {
    return TestTest();
}

