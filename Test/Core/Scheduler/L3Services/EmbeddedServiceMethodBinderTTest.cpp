/**
 * @file EmbeddedServiceMethodBinderTTest.cpp
 * @brief Source file for class EmbeddedServiceMethodBinderTTest
 * @date 20/09/2016
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
 * the class EmbeddedThreadTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "EmbeddedServiceI.h"
#include "EmbeddedServiceMethodBinderTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class EmbeddedServiceMethodBinderTTestCallbackClass {
public:
    EmbeddedServiceMethodBinderTTestCallbackClass() {
        functionCalled = false;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunction(MARTe::ExecutionInfo &information) {
        functionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType CallbackFunctionWithError(MARTe::ExecutionInfo &information) {
        functionCalled = true;
        return MARTe::ErrorManagement::FatalError;
    }

    bool functionCalled;
};


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EmbeddedServiceMethodBinderTTest::EmbeddedServiceMethodBinderTTest() {
}

EmbeddedServiceMethodBinderTTest::~EmbeddedServiceMethodBinderTTest() {
}

bool EmbeddedServiceMethodBinderTTest::TestDefaultConstructor() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderTTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedServiceMethodBinderTTestCallbackClass> binderT(callbackClass, &EmbeddedServiceMethodBinderTTestCallbackClass::CallbackFunction);
    ExecutionInfo info;
    return binderT.Execute(info);
}

bool EmbeddedServiceMethodBinderTTest::TestExecute() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderTTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedServiceMethodBinderTTestCallbackClass> binderT(callbackClass, &EmbeddedServiceMethodBinderTTestCallbackClass::CallbackFunction);
    ExecutionInfo info;
    ErrorManagement::ErrorType err = binderT.Execute(info);
    bool ok = (err == ErrorManagement::NoError);
    ok &= callbackClass.functionCalled;
    return ok;
}

bool EmbeddedServiceMethodBinderTTest::TestExecute_WithError() {
    using namespace MARTe;
    EmbeddedServiceMethodBinderTTestCallbackClass callbackClass;
    EmbeddedServiceMethodBinderT<EmbeddedServiceMethodBinderTTestCallbackClass> binderT(callbackClass, &EmbeddedServiceMethodBinderTTestCallbackClass::CallbackFunctionWithError);
    ExecutionInfo info;
    ErrorManagement::ErrorType err = binderT.Execute(info);
    bool ok = (err == ErrorManagement::FatalError);
    return ok;
}

