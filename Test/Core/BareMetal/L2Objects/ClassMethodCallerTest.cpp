/**
 * @file ClassMethodCallerTest.cpp
 * @brief Source file for class ClassMethodCallerTest
 * @date 27/06/2016
 * @author Ivan Herrero
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
 * the class ClassMethodCallerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCallerTest.h"
#include "ClassMethodCaller.h"
#include "ErrorType.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ClassMethodCallerTest::ClassMethodCallerTest() {
}

ClassMethodCallerTest::~ClassMethodCallerTest() {
}

bool ClassMethodCallerTest::TestDefaultConstructor() {
    return true;
}

bool ClassMethodCallerTest::TestCall() {
    using namespace MARTe;
    bool result = true;
    {
        ClassMethodCaller target;
        ErrorManagement::ErrorType status;
        Object context;
        status = target.Call(&context);
        result &= (status == ErrorManagement::UnsupportedFeature);
    }
    {
        ClassMethodCaller target;
        ErrorManagement::ErrorType status;
        Object context;
        int32 param = 0;
        status = target.Call(&context, param);
        result &= (status == ErrorManagement::UnsupportedFeature);
    }
    {
        ClassMethodCaller target;
        ErrorManagement::ErrorType status;
        Object context;
        ReferenceContainer param;
        status = target.Call(&context, param);
        result &= (status == ErrorManagement::UnsupportedFeature);
    }
    {
        ClassMethodCaller target;
        ErrorManagement::ErrorType status;
        Object context;
        int32 param = 0;
        status = target.Call(&context, param, true);
        result &= (status == ErrorManagement::UnsupportedFeature);
        status = target.Call(&context, param, false);
        result &= (status == ErrorManagement::UnsupportedFeature);
    }
    {
        ClassMethodCaller target;
        ErrorManagement::ErrorType status;
        Object context;
        ReferenceContainer param;
        status = target.Call(&context, param, true);
        result &= (status == ErrorManagement::UnsupportedFeature);
        status = target.Call(&context, param, false);
        result &= (status == ErrorManagement::UnsupportedFeature);
    }
    return result;
}
