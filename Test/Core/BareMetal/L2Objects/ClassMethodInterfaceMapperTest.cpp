/**
 * @file ClassMethodInterfaceMapperTest.cpp
 * @brief Source file for class ClassMethodInterfaceMapperTest
 * @date 24/06/2016
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
 * the class ClassMethodInterfaceMapperTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapperTest.h"
#include "ClassMethodInterfaceMapper.h"
#include "ClassMethodCallerT.h"
#include "ClassWithCallableMethods.h"
#include "ReferenceContainer.h"
#include "ErrorType.h"
#include "Reference.h"

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
        ClassWithCallableMethods context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target.Call(&context, params);
        result &= status.functionError;
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodWithInputReferenceContainer);
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            status = target.Call(&context, params);
            result &= status;
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
    }
    {
        ClassMethodCallerT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodWithInputOutputReferenceContainer);
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            status = target.Call(&context, params);
            result &= status;
            obj = params.Find("TestObject");
            result &= !obj.IsValid();
            obj = params.Find("TestObject2");
            result &= obj.IsValid();
        }
        else {
            result = false;
        }
    }
    return result;
}

ClassMethodInterfaceMapperTest::ClassMethodInterfaceMapperTest() {
}

ClassMethodInterfaceMapperTest::~ClassMethodInterfaceMapperTest() {
}

bool ClassMethodInterfaceMapperTest::TestCall() {
    using namespace MARTe;
    bool result = true;
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::FaultyMethod);
        ClassWithCallableMethods context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target.Call<ReferenceContainer&>(&context, params);
        result &= status.functionError;
    }
    {
        ClassMethodInterfaceMapper target((bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        status = target.Call(&context);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
    }
    {
        ClassMethodInterfaceMapper target((bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target.Call<int&>(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        ClassMethodInterfaceMapper target((bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods::OverloadedMethod);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target.Call<ReferenceContainer&>(&context, params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithInputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 10;
        status = target.Call<int&>(&context, params);
        result &= status;
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithOutputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target.Call<int&>(&context, params);
        result &= status;
        result &= (params == 20);
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithInputOutputInteger);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 30;
        status = target.Call<int&>(&context, params);
        result &= status;
        result &= (params == (30 + 5));
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithInputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.Call<ReferenceContainer&>(&context, params);
            result &= status;
        }
        else {
            result = false;
        }
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithOutputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj;
        status = target.Call<ReferenceContainer&>(&context, params);
        result &= status;
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
    }
    {
        ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodWithInputOutputReferenceContainer);
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.Call<ReferenceContainer&>(&context, params);
            result &= status;
            obj = params.Find("TestObject");
            result &= !obj.IsValid();
            obj = params.Find("TestObject2");
            result &= obj.IsValid();
        }
        else {
            result = false;
        }
    }
    return result;
}
