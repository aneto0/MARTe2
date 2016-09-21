/**
 * @file EmbeddedThreadITest.cpp
 * @brief Source file for class EmbeddedThreadITest
 * @date 19/09/2016
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
 * the class EmbeddedThreadITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedThreadI.h"
#include "EmbeddedThreadITest.h"
#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/\
class EmbeddedThreadITestCallbackClass {
public:
    EmbeddedThreadITestCallbackClass() {
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        return MARTe::ErrorManagement::NoError;
    }
};


class EmbeddedThreadITestStub : public MARTe::EmbeddedThreadI {
public:
    EmbeddedThreadITestStub(MARTe::EmbeddedServiceMethodBinderI &binder) : MARTe::EmbeddedThreadI(binder) {
    }

    void ThreadLoop() {
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EmbeddedThreadITest::EmbeddedThreadITest() {
}

EmbeddedThreadITest::~EmbeddedThreadITest() {
}

bool EmbeddedThreadITest::TestDefaultConstructor() {
    using namespace MARTe;
    EmbeddedThreadITestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedThreadITestCallbackClass> binder(callbackClass, &EmbeddedThreadITestCallbackClass::CallbackFunction);
    EmbeddedThreadITestStub embeddedThreadI(binder);
    bool ok = (embeddedThreadI.GetThreadId() == InvalidThreadIdentifier);
    ok &= (embeddedThreadI.GetThreadNumber() == 0);
    ok &= (embeddedThreadI.GetCommands() == EmbeddedThreadI::StopCommand);
    return ok;
}
