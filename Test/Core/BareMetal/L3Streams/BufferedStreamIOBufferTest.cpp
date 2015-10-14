/**
 * @file BufferedStreamIOBufferTest.cpp
 * @brief Source file for class BufferedStreamIOBufferTest
 * @date 14/10/2015
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
 * the class BufferedStreamIOBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BufferedStreamIOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool BufferedStreamIOBufferTest::TestDefaultConstructor() {

    BufferedStreamIOBuffer buffered;
    if (buffered.AmountLeft() != 0) {
        return false;
    }

    if (buffered.UsedAmountLeft() != 0) {
        return false;
    }

    if (buffered.MaxUsableAmount() != 0) {
        return false;
    }

    if (buffered.Buffer() != NULL) {
        return false;
    }

    if (buffered.GetTimeout() != TTDefault) {
        return false;
    }

    if (buffered.GetStream() != NULL) {
        return false;
    }
    return buffered.UndoLevel() == 0;

}
