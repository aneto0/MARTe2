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
#include "ClassMethodCallerT.h"
#include "ClassWithCallableMethods.h"
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

bool ClassMethodCallerTest::TestCall() {
    using namespace MARTe;
    bool result = true;
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::FaultyMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target.Call(&context, params);
        result &= status.functionError;
        result &= (context.GetLastMethodExecuted() == "FaultyMethod(MARTe::ReferenceContainer&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods> target((bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        status = target.Call(&context);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, int&> target((bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target.Call(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target((bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target.Call(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, int&> target(&ClassWithCallableMethods::MethodWithInputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 10;
        status = target.Call(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "MethodWithInputInteger(int&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, int&> target(&ClassWithCallableMethods::MethodWithOutputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target.Call(&context, params);
        result &= status;
        result &= (params == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, int&> target(&ClassWithCallableMethods::MethodWithInputOutputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 30;
        status = target.Call(&context, params);
        result &= status;
        result &= (params == (30 + 5));
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodWithInputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.Call(&context, params);
            result &= status;
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainer(MARTe::ReferenceContainer&)");
        }
        else {
            result = false;
        }
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj;
        status = target.Call(&context, params);
        result &= status;
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainer(MARTe::ReferenceContainer&)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodWithInputOutputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.Call(&context, params);
            result &= status;
            obj = params.Find("TestObject");
            result &= !obj.IsValid();
            obj = params.Find("TestObject2");
            result &= obj.IsValid();
            result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer&)");
        }
        else {
            result = false;
        }
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, int> target(&ClassWithCallableMethods::MethodWithInputIntegerByCopy);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 80;
        status = target.Call(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
        if (success) {
            status = target.Call(&context, params);
            result &= status;
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer)");
        }
        else {
            result = false;
        }
    }
    return result;
}
