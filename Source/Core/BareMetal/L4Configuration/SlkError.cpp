/**
 * @file SlkError.cpp
 * @brief Source file for class SlkError
 * @date 04/12/2015
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
 * the class SlkError (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SlkError.h"
#include "SlkConstants.h"
#include "BufferedStreamI.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void PrintOnStream(const char8 * const message,
                          BufferedStreamI* err) {
    if (err != NULL) {
        if (!err->Printf("%s", message)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on error stream");
        }
        REPORT_ERROR(ErrorManagement::FatalError, message);
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

SlkError::SlkError(BufferedStreamI* err) {
    errorStream = err;
    isError = false;
    isEOF = false;
}

uint16 SlkError::no_entry(uint16 symbol,
                          uint16 token,
                          int32 level) {
    // if it is not EOF!
    if (token != 0u) {
        PrintOnStream("\nInvalid Token!", errorStream);
        isError = true;
    }
    else {
        isEOF = true;
    }
    return END_OF_SLK_INPUT_;
}

uint16 SlkError::mismatch(uint16 symbol,
                          uint16 token) {
    PrintOnStream("\nInvalid Expression!", errorStream);
    isError = true;
    return END_OF_SLK_INPUT_;
}

void SlkError::input_left() {
    isError = true;
    PrintOnStream("\nEOF found with tokens on internal parser stack!", errorStream);
}

bool SlkError::IsError() const {
    return isError;
}

bool SlkError::IsEOF() const {
    return isEOF;
}

void SlkError::SetError() {
    isError = true;
}
