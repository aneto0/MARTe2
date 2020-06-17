/**
 * @file BasicTypeTest.cpp
 * @brief Source file for class BasicTypeTest
 * @date 08/09/2016
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
 * the class BasicTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ErrorTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool ErrorTypeTest::TestConstructor_Default() {
    ErrorManagement::ErrorType err;
    return err;
}

bool ErrorTypeTest::TestConstructor_False() {
    ErrorManagement::ErrorType err(false);
    return !err;
}

bool ErrorTypeTest::TestConstructor_BitSet() {
    ErrorManagement::ErrorType err(ErrorManagement::Debug | ErrorManagement::ErrorSharing);
    bool ret = !err;
    ret &= (err == ErrorManagement::ErrorType(ErrorManagement::Debug | ErrorManagement::ErrorSharing));
    return ret;
}

bool ErrorTypeTest::TestErrorsCleared() {
    ErrorManagement::ErrorType err(ErrorManagement::Debug | ErrorManagement::ErrorSharing);
    bool ret = !err;
    err = false;
    ret &= err;
    return ret;
}

bool ErrorTypeTest::TestOperatorBool() {
    ErrorManagement::ErrorType err;
    bool ret = err;
    err = true;
    if (ret) {
        ret = !err;
    }
    err = false;
    if (ret) {
        ret = err;
    }
    return ret;
}

bool ErrorTypeTest::TestOperatorErrorIntegerFormat() {
    ErrorManagement::ErrorType err = ErrorManagement::Debug;
    uint32 debug = err;
    return (debug == ErrorManagement::Debug);
}

bool ErrorTypeTest::TestOperatorEquals() {
    ErrorManagement::ErrorType err = ErrorManagement::Debug;
    return (err == ErrorManagement::ErrorType(ErrorManagement::Debug));
}

bool ErrorTypeTest::TestOperatorNotEqual() {
    ErrorManagement::ErrorType err = ErrorManagement::Information;
    return (err != ErrorManagement::ErrorType(ErrorManagement::Debug));
}

bool ErrorTypeTest::TestOperatorAssignment() {
    ErrorManagement::ErrorType err = (ErrorManagement::Information | ErrorManagement::Exception);
    return (err == ErrorManagement::ErrorType(ErrorManagement::Information | ErrorManagement::Exception));
}

bool ErrorTypeTest::TestSetError() {
    ErrorManagement::ErrorType err;
    err = ErrorManagement::Information | ErrorManagement::Exception;
    return (err == ErrorManagement::ErrorType(ErrorManagement::Information | ErrorManagement::Exception));
}

bool ErrorTypeTest::TestClearError() {
    ErrorManagement::ErrorType err;
    err = ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation;
    bool ret = (err == ErrorManagement::ErrorType(ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation));
    err.ClearErrorBits(ErrorManagement::Exception_bit | ErrorManagement::IllegalOperation_bit);
    ret &= (err != ErrorManagement::ErrorType(ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation));
    ret &= (err == ErrorManagement::ErrorType(ErrorManagement::Information));
    return ret;
}

bool ErrorTypeTest::TestContains() {
    ErrorManagement::ErrorType err;
    err = ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation;
    bool ret = (err == ErrorManagement::ErrorType(ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation));
    ret &= (err.Contains(ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation));
    ret &= !(err.Contains(ErrorManagement::Information | ErrorManagement::Exception | ErrorManagement::IllegalOperation | ErrorManagement::FatalError));
    ret &= (err.Contains(ErrorManagement::Information));
    err = ErrorManagement::Information;
    ret &= (err == ErrorManagement::ErrorType(ErrorManagement::Information));
    return ret;
}
