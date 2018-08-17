/**
 * @file FastResourceContainerTest.cpp
 * @brief Source file for class FastResourceContainerTest
 * @date 09/03/2017
 * @author Andre' Neto
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
 * the class FastResourceContainerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FastResourceContainer.h"
#include "FastResourceContainerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool FastResourceContainerTest::TestConstructor() {
    using namespace MARTe;
    uint32 numberOfElements = 100;
    FastResourceContainer test1(numberOfElements, false);
    FastResourceContainer test2(numberOfElements, true);
    bool ok = (test1.GetSize() == numberOfElements);
    ok &= (test2.GetSize() == 0u);
    return ok;
}

bool FastResourceContainerTest::TestGetSize() {
    using namespace MARTe;
    uint32 numberOfElements = 2;
    FastResourceContainer test1(numberOfElements, false);
    bool ok = (test1.GetSize() == numberOfElements);
    test1.Take();
    ok &= (test1.GetSize() == (numberOfElements - 1));
    test1.Take();
    ok &= (test1.GetSize() == (numberOfElements - 2));
    test1.Return(1);
    ok &= (test1.GetSize() == (numberOfElements - 1));
    test1.Return(0);
    ok &= (test1.GetSize() == numberOfElements);
    return ok;
}

bool FastResourceContainerTest::TestTake() {
    using namespace MARTe;
    uint32 numberOfElements = 2;
    FastResourceContainer test1(numberOfElements, false);
    bool ok = (test1.GetSize() == numberOfElements);
    uint32 e1 = test1.Take();
    ok &= (test1.GetSize() == (numberOfElements - 1));
    uint32 e2 = test1.Take();
    ok &= (test1.GetSize() == (numberOfElements - 2));
    uint32 e3 = test1.Take();
    ok &= (e3 == 0xFFFFFFFFu);
    //Return an invalid element
    test1.Return(e3);
    ok &= (test1.GetSize() == (numberOfElements - 2));
    test1.Return(e2);
    ok &= (test1.GetSize() == (numberOfElements - 1));
    e2 = test1.Take();
    ok &= (e2 != 0xFFFFFFFFu);
    ok &= (test1.GetSize() == (numberOfElements - 2));
    test1.Return(e1);
    ok &= (test1.GetSize() == (numberOfElements - 1));
    e1 = test1.Take();
    ok &= (e1 != 0xFFFFFFFFu);
    test1.Return(e1);
    test1.Return(e2);
    ok &= (test1.GetSize() == numberOfElements);
    return ok;
}

bool FastResourceContainerTest::TestReturn() {
    return TestTake();
}

