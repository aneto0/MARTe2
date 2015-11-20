/**
 * @file SelectTest.cpp
 * @brief Source file for class SelectTest
 * @date 19/11/2015
 * @author Llorenç Capellà
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
 * the class SelectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SelectTest.h"
#include "Select.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

SelectTest::SelectTest() {
    retVal = true;
}

bool SelectTest::TestDefaultConstructor() {
    BasicConsole bc1;
    return (!sel.IsSet(bc1));
}

bool SelectTest::TestAddReadHandle() {
    retVal &= sel.AddReadHandle(bc);
    return retVal;
}

bool SelectTest::TestAddReadHandle_SameHandle() {
    retVal &= sel.AddReadHandle(bc);
    retVal &= !sel.AddReadHandle(bc);
    return retVal;
}

bool SelectTest::TestAddReadHandle_InvalidHandle() {
    retVal &= !sel.AddReadHandle(bf);
    return retVal;
}

bool SelectTest::TestAddWriteHandle() {
    retVal &= sel.AddWriteHandle(bc);
    return retVal;
}

bool SelectTest::TestAddWritedHandle_SameHandle() {
    retVal &= sel.AddWriteHandle(bc);
    retVal &= !sel.AddWriteHandle(bc);
    return retVal;
}

bool SelectTest::TestAddWritedHandle_InvalidHandle() {
    retVal &= !sel.AddWriteHandle(bf);
    return retVal;
}

bool SelectTest::TestAddExceptionHandle() {
    retVal &= sel.AddExceptionHandle(bc);
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_SameHandle() {
    retVal &= sel.AddExceptionHandle(bc);
    retVal &= !sel.AddExceptionHandle(bc);
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_Invaliddle() {
    retVal &= !sel.AddExceptionHandle(bf);
    return retVal;
}

bool SelectTest::TestRemoveReadHandle() {
    sel.AddReadHandle(bc);
    retVal &= sel.RemoveReadHandle(bc);
    retVal &= !sel.IsSet(bc);
    return retVal;
}

bool SelectTest::TestRemoveReadHandle_InvalidHandle() {
    retVal &= !sel.RemoveReadHandle(bf);
    return retVal;
}
