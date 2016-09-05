/**
 * @file ClassMethodCallerTTest.cpp
 * @brief Source file for class ClassMethodCallerTTest
 * @date 14/07/2016
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
 * the class ClassMethodCallerTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCallerTTest.h"
#include "ClassMethodCallerT.h"
#include "ClassWithCallableMethods.h"
#include "ConfigurationDatabase.h"
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

ClassMethodCallerTTest::ClassMethodCallerTTest() {
}

ClassMethodCallerTTest::~ClassMethodCallerTTest() {
}

bool ClassMethodCallerTTest::TestDefaultConstructor() {
    return true;
}

bool ClassMethodCallerTTest::TestCall_VoidParameters() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(void), void, void, void, void> target(
                &ClassWithCallableMethods::MethodWithVoidParameters, 0x0000);
        target.Call(&context);
        result = (context.GetLastMethodExecuted() == "MethodWithVoidParameters(void)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(void), void, void, void, void> target(
                &ClassWithCallableMethods::MethodWithVoidParameters, 0x0000);
        ConfigurationDatabase parameters;
        target.Call(&context, parameters);
        result = (context.GetLastMethodExecuted() == "MethodWithVoidParameters(void)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(void), void, void, void, void> target(
                &ClassWithCallableMethods::MethodWithVoidParameters, 0x0000);
        ReferenceContainer parameters;
        target.Call(&context, parameters);
        result = (context.GetLastMethodExecuted() == "MethodWithVoidParameters(void)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(void), void, void, void, void> target(
                &ClassWithCallableMethods::MethodWithVoidParameters, 0x0000);
        StreamString parameters;
        result = target.Call(&context, parameters);
        result &= (context.GetLastMethodExecuted() == "MethodWithVoidParameters(void)");
    }
    return result;
}

bool ClassMethodCallerTTest::TestCall_OneBasicParameterReadOnly() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ConfigurationDatabase parameters;
        uint32 param1 = 80;
        parameters.Write("param1", param1);
        result = target.Call(&context, parameters);
        parameters.Read("param1", param1);
        result &= (param1 == 80);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 80;
        info->Write("param1", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        info->Read("param1", param1);
        result &= (param1 == 80);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 80;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ConfigurationDatabase parameters;
        uint32 param1 = 40;
        parameters.Write("param1", param1);
        result = target.Call(&context, parameters);
        parameters.Read("param1", param1);
        result &= (param1 == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 40;
        info->Write("param1", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        info->Read("param1", param1);
        result &= (param1 == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 40;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneBasicParameterReadWrite() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ConfigurationDatabase parameters;
        result = target.Call(&context, parameters);
        uint32 param1;
        parameters.Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        uint32 param1;
        info->Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputOutputInteger, 0x1000);
        ConfigurationDatabase parameters;
        uint32 param1 = 10;
        parameters.Write("param1", param1);
        result = target.Call(&context, parameters);
        parameters.Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputOutputInteger, 0x1000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ReferenceContainer parameters;
        parameters.Insert(info);
        uint32 param1 = 10;
        info->Write("param1", param1);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        info->Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 10;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneStructuredDataIParameterReadOnly() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        ConfigurationDatabase parameters;
        uint32 value = 20;
        parameters.Write("value", value);
        result = target.Call(&context, parameters);
        parameters.Read("value", value);
        result &= (value == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 value = 20;
        info->Write("value", value);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        info->Read("value", value);
        result &= (value == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneStructuredDataIParameterReadWrite() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        ConfigurationDatabase parameters;
        uint32 value = 30;
        parameters.Write("value", value);
        result = target.Call(&context, parameters);
        parameters.Read("value", value);
        result &= (value == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 value = 30;
        info->Write("value", value);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        info->Read("value", value);
        result &= (value == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::parametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneReferenceContainerParameterReadOnly() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy, 0x0000);

        ReferenceContainer parameters;
        ReferenceT<Object> obj(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerByCopy(ReferenceContainer)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerAndStructuredDataIAt0, 0x0000);
        ReferenceT<ConfigurationDatabase> info(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        uint32 param1 = 10;
        info->Write("value", param1);
        result &= (target.Call(&context, *(info.operator->())) == ErrorManagement::noError);
        info->Read("value", param1);
        result &= (param1 == 10);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerAndStructuredDataIAt0(ReferenceContainer)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const ReferenceContainer &), ReferenceContainer,
                void, void, void> target(&ClassWithCallableMethods::MethodWithConstInputReferenceContainer, 0x0000);

        ReferenceContainer parameters;
        ReferenceT<Object> obj(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputReferenceContainer(ReferenceContainer&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy, 0x0000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneReferenceContainerParameterReadWrite() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainer, 0x1000);

        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        ReferenceT<Object> obj = parameters.Get(0);
        result &= obj.IsValid();
        if (result) {
            result &= (StringHelper::Compare(obj->GetName(), "TestObject2") == 0);
        }
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainer(ReferenceContainer&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainerAndStructuredDataIAt0, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        uint32 value;
        ReferenceT<ConfigurationDatabase> info = parameters.Get(0);
        result = info.IsValid();
        if (result) {
            info->Read("value", value);
        }
        result &= (value == 60);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainerAndStructuredDataIAt0(ReferenceContainer&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputOutputReferenceContainer, 0x1000);

        ReferenceContainer parameters;
        ReferenceT<Object> obj(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::noError);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputReferenceContainer(ReferenceContainer&)");
        ReferenceT<Object> obj2 = parameters.Find("TestObject2");
        result &= obj2.IsValid();
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainer, 0x1000);
        StreamString parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::parametersError);
    }

    return result;
}
