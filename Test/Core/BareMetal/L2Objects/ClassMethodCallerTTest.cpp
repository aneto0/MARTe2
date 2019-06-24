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
#include "ErrorType.h"
#include "ReferenceContainer.h"
#include "VoidStream.h"
#include "SimpleStructuredData.h"
#include "SimpleStream.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



namespace {

/**
 * @brief Helper class to test a DynamicCString inside a reference container
 */
class ClassMethodCallerTTestStreamString: public MARTe::Object, public MARTe::SimpleStream {
public:
    CLASS_REGISTER_DECLARATION();

    ClassMethodCallerTTestStreamString    () : Object(),  SimpleStream() {

    }

    MARTe::CCString ToString(){
		return MARTe::CCString(this->GetList());
	}

};
CLASS_REGISTER(ClassMethodCallerTTestStreamString, "1.0")

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
        SimpleStructuredData parameters;
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
        SimpleStream parameters;
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
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 80;
        info->Write("param1", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        info->Read("param1", param1);
        result &= (param1 == 80);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 80;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputIntegerByCopy, 0x0000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 40;
        info->Write("param1", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        info->Read("param1", param1);
        result &= (param1 == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 40;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputInteger, 0x0000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
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
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        uint32 param1;
        info->Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputOutputInteger, 0x1000);
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        ReferenceContainer parameters;
        parameters.Insert(info);
        uint32 param1 = 10;
        info->Write("param1", param1);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        info->Read("param1", param1);
        result &= (param1 == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 10;
        info->Write("param2", param1);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(int32 &), int32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputInteger, 0x1000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
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
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 value = 20;
        info->Write("value", value);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        info->Read("value", value);
        result &= (value == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StructuredDataI &), StructuredDataI, void,
                void, void> target(&ClassWithCallableMethods::MethodWithConstInputStructuredDataI, 0x0000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
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
        SimpleStructuredData parameters;
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
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 value = 30;
        info->Write("value", value);
        ReferenceContainer parameters;
        parameters.Insert(info);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        info->Read("value", value);
        result &= (value == 40);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputStructuredDataI(StructuredDataI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StructuredDataI &), StructuredDataI, void, void,
                void> target(&ClassWithCallableMethods::MethodWithOutputStructuredDataI, 0x1000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
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
        ReferenceT<Object> obj(HeapManager::standardHeapId);
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerByCopy(ReferenceContainer)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerAndStructuredDataIAt0, 0x0000);
        ReferenceT<SimpleStructuredData> info(HeapManager::standardHeapId);
        uint32 param1 = 10;
        info->Write("value", param1);
        result &= (target.Call(&context, *(info.operator->())) == ErrorManagement::NoError);
        info->Read("value", param1);
        result &= (param1 == 10);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerAndStructuredDataIAt0(ReferenceContainer)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const ReferenceContainer &), ReferenceContainer,
                void, void, void> target(&ClassWithCallableMethods::MethodWithConstInputReferenceContainer, 0x0000);

        ReferenceContainer parameters;
        ReferenceT<Object> obj(HeapManager::standardHeapId);
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputReferenceContainer(ReferenceContainer&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy, 0x0000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
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
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        ReferenceT<Object> obj = parameters.Get(0);
        result &= obj.IsValid();
        if (result) {
            result &= (obj->GetName() == "TestObject2") ;
        }
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainer(ReferenceContainer&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainerAndStructuredDataIAt0, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        uint32 value;
        ReferenceT<SimpleStructuredData> info = parameters.Get(0);
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
        ReferenceT<Object> obj(HeapManager::standardHeapId);
        obj->SetName("TestObject");
        parameters.Insert(obj);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputReferenceContainer(ReferenceContainer&)");
        ReferenceT<Object> obj2 = parameters.Find("TestObject2");
        result &= obj2.IsValid();
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(ReferenceContainer &), ReferenceContainer, void,
                void, void> target(&ClassWithCallableMethods::MethodWithOutputReferenceContainer, 0x1000);
        SimpleStream parameters;
        ClassMethodCaller targetForce = target;
        result &= (targetForce.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneStreamIParameterReadOnly() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputStreamI, 0x0000);
        SimpleStream parameters("MethodWithConstInputStreamI");
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputStreamI(StreamI)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithConstInputStreamI, 0x0000);
        ReferenceContainer parameters;
        ReferenceT<ClassMethodCallerTTestStreamString> ss(HeapManager::standardHeapId);
        ss->Init("MethodWithConstInputStreamI");
//        uint32 size = StringHelper::Length("MethodWithConstInputStreamI");
//        ss->("MethodWithConstInputStreamI", size);
        parameters.Insert(ss);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (context.GetLastMethodExecuted() == "MethodWithConstInputStreamI(StreamI)");
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneStreamIParameterReadWrite() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputStreamI, 0x1000);
        SimpleStream parameters;

        result &= (!target.Call(&context, parameters).IsErrorCode());
        result &= (parameters == "MethodWithOutputStreamI");
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputStreamI(StreamI&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOutputStreamI, 0x1000);
        ReferenceContainer parameters;
        ReferenceT<ClassMethodCallerTTestStreamString> ss(HeapManager::standardHeapId);
        parameters.Insert(ss);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
//        DynamicCString ssToCompare = ss->Buffer();
//        result &= (ssToCompare == "MethodWithOutputStreamI");
        result &= (ss->ToString() == "MethodWithOutputStreamI");
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputStreamI(StreamI&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputOutputStreamI, 0x1000);
        SimpleStream parameters = "MethodWithInputOutputStreamI";
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        result &= (parameters == "ReturnFromMethodWithInputOutputStreamII");
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputStreamI(StreamI&)");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(StreamI &), StreamI, void, void, void> target(
                &ClassWithCallableMethods::MethodWithInputOutputStreamI, 0x1000);
        ReferenceContainer parameters;
        ReferenceT<ClassMethodCallerTTestStreamString> ss(HeapManager::standardHeapId);
//        uint32 size = StringHelper::Length("MethodWithInputOutputStreamI");
//        ss->Write("MethodWithInputOutputStreamI", size);
        ss->Init("MethodWithInputOutputStreamI");
        parameters.Insert(ss);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
//        DynamicCString ssToCompare = ss->Buffer();
        result &= (ss->ToString() == "ReturnFromMethodWithInputOutputStreamII");
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputStreamI(StreamI&)");
    }
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
																										  MARTe::DynamicCString),
					uint32, float32, float64,MARTe::DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_C, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_R, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_C_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_W, 0x11);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_C, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_R, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_R_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_W, 0x11);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_W, 0x101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_W, 0x101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_C, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_R, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_C_W_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_W, 0x111);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_C, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_R, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_C_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_W, 0x11);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_W, 0x1);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_C, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_R, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_R_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_W, 0x11);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_W, 0x101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_W, 0x101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_C, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_R, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_R_W_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_W, 0x111);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_W, 0x1001);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_W, 0x1001);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_C, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_R, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_C_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_W, 0x1011);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_W, 0x1001);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_W, 0x1001);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_C, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_R, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_R_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_W, 0x1011);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_C, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_R, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_W, 0x1101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_C, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_R, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_W, 0x1101);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            float64, DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_C, 0x1110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_R, 0x1110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_W_W_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::float64 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, float64,
            DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    float64 param3 = -9.0;
    parameters.Write("param3", param3);
    DynamicCString param4 = "KO";
    parameters.Write("param4", param4);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == -7.0);
    parameters.Read("param4", param4);
    result &= (param4 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ErrorParameter1() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1E", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        DynamicCString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ErrorParameter2() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2E", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        DynamicCString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ErrorParameter3() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3E", param3);
        DynamicCString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ErrorParameter4() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        DynamicCString param4 = "KO";
        parameters.Write("param4E", param4);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ErrorParameterReturn() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParametersReturnError, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        DynamicCString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_FourParameters_ReferenceContainer() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        float32 param2 = 2.0;
        parametersDB->Write("param2", param2);
        float64 param3 = -9.0;
        parametersDB->Write("param3", param3);
        DynamicCString param4 = "KO";
        parametersDB->Write("param4", param4);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        parametersDB->Read("param1", param1);
        result &= (param1 == 5);
        parametersDB->Read("param2", param2);
        result &= (param2 == 6.0);
        parametersDB->Read("param3", param3);
        result &= (param3 == -7.0);
        parametersDB->Read("param4", param4);
        result &= (param4 == "KOOK");
        result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_W");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParametersReturnError, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        float32 param2 = 2.0;
        parametersDB->Write("param2", param2);
        float64 param3 = -9.0;
        parametersDB->Write("param3", param3);
        DynamicCString param4 = "KO";
        parametersDB->Write("param4", param4);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::float64 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32, float64,
                DynamicCString> target(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W, 0x1111);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_W, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_W, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_C_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_W, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_W, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_W, 0x10);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_C, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_R, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_R_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_W, 0x110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_W, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_W, 0x1010);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, float32,
            DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_C, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_R, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_W_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::float32 &,
                                                                                                          MARTe::DynamicCString &), uint32, float32, DynamicCString,
            void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1110);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    float32 param2 = 2.0;
    parameters.Write("param2", param2);
    DynamicCString param3 = "KO";
    parameters.Write("param3", param3);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == 6.0);
    parameters.Read("param3", param3);
    result &= (param3 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_ErrorParameter1() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1E", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        DynamicCString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_ErrorParameter2() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2E", param2);
        DynamicCString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_ErrorParameter3() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        DynamicCString param3 = "KO";
        parameters.Write("param3E", param3);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_ErrorParameterReturn() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParametersReturnError, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        DynamicCString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_ThreeParameters_ReferenceContainer() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        float32 param2 = 2.0;
        parametersDB->Write("param2", param2);
        DynamicCString param3 = "KO";
        parametersDB->Write("param3", param3);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        parametersDB->Read("param1", param1);
        result &= (param1 == 5);
        parametersDB->Read("param2", param2);
        result &= (param2 == 6.0);
        parametersDB->Read("param3", param3);
        result &= (param3 == "KOOK");
        result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_W");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParametersReturnError, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        float32 param2 = 2.0;
        parametersDB->Write("param2", param2);
        DynamicCString param3 = "KO";
        parametersDB->Write("param3", param3);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::float32 &,
                                                                                                              MARTe::DynamicCString &), uint32, float32,
                DynamicCString, void> target(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W, 0x1111);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_C_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, DynamicCString,
            void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_C_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_C_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_C_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::DynamicCString), uint32, DynamicCString, void, void> target(
            &ClassWithCallableMethods::MethodWithTwoParameters_C_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_C_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_C_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &,
                                                                                                          MARTe::DynamicCString &), uint32, DynamicCString, void,
            void> target(&ClassWithCallableMethods::MethodWithTwoParameters_C_W, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_C_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_R_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          const MARTe::DynamicCString &), uint32, DynamicCString,
            void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_R_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_R_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_R_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::DynamicCString), uint32, DynamicCString, void, void> target(
            &ClassWithCallableMethods::MethodWithTwoParameters_R_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_R_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_R_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32,
                                                                                                          MARTe::DynamicCString &), uint32, DynamicCString, void,
            void> target(&ClassWithCallableMethods::MethodWithTwoParameters_R_W, 0x100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param2", param2);
    result &= (param2 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_R_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_W_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          const MARTe::DynamicCString &), uint32, DynamicCString,
            void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_C, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_W_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_W_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::DynamicCString), uint32, DynamicCString, void, void> target(
            &ClassWithCallableMethods::MethodWithTwoParameters_W_R, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_W_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_W_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                          MARTe::DynamicCString &), uint32, DynamicCString, void,
            void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_W, 0x1100);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    DynamicCString param2 = "KO";
    parameters.Write("param2", param2);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    parameters.Read("param2", param2);
    result &= (param2 == "KOOK");
    result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_W_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_ErrorParameter1() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1E", param1);
        DynamicCString param2 = "KO";
        parameters.Write("param2", param2);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_ErrorParameter2() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_W, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        DynamicCString param2 = "KO";
        parameters.Write("param2E", param2);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_ErrorParameterReturn() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParametersReturnError, 0x1111);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        DynamicCString param2 = "KO";
        parameters.Write("param2", param2);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_TwoParameters_ReferenceContainer() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_W, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        DynamicCString param2 = "KO";
        parametersDB->Write("param2", param2);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        parametersDB->Read("param1", param1);
        result &= (param1 == 5);
        parametersDB->Read("param2", param2);
        result &= (param2 == "KOOK");
        result &= (context.GetLastMethodExecuted() == "MethodWithTwoParameters_W_W");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParametersReturnError, 0x1111);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);
        DynamicCString param2 = "KO";
        parametersDB->Write("param2", param2);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &,
                                                                                                              MARTe::DynamicCString &), uint32, DynamicCString,
                void, void> target(&ClassWithCallableMethods::MethodWithTwoParameters_W_W, 0x1111);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_C() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &), uint32, void, void, void> target(
            &ClassWithCallableMethods::MethodWithOneParameter_C, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithOneParameter_C");
    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_R() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32), uint32, void, void, void> target(
            &ClassWithCallableMethods::MethodWithOneParameter_R, 0x0);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    result &= (context.GetLastMethodExecuted() == "MethodWithOneParameter_R");
    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_W() {
    using namespace MARTe;
    bool result = true;
    ClassWithCallableMethods context;
    ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &), uint32, void, void, void> target(
            &ClassWithCallableMethods::MethodWithOneParameter_W, 0x1000);
    SimpleStructuredData parameters;
    uint32 param1 = 3;
    parameters.Write("param1", param1);
    result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
    parameters.Read("param1", param1);
    result &= (param1 == 5);
    result &= (context.GetLastMethodExecuted() == "MethodWithOneParameter_W");
    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_ErrorParameter1() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(const MARTe::uint32 &), uint32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOneParameter_C, 0x0);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1E", param1);
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_ErrorParameterReturn() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &), uint32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOneParameterReturnError, 0x0);
        SimpleStructuredData parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }

    return result;
}

bool ClassMethodCallerTTest::TestCall_OneParameter_ReferenceContainer() {
    using namespace MARTe;
    bool result = true;
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &), uint32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOneParameter_W, 0x1000);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) == ErrorManagement::NoError);
        parametersDB->Read("param1", param1);
        result &= (param1 == 5);
        result &= (context.GetLastMethodExecuted() == "MethodWithOneParameter_W");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &), uint32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOneParameterReturnError, 0x0);
        ReferenceT<SimpleStructuredData> parametersDB("SimpleStructuredData");
        uint32 param1 = 3;
        parametersDB->Write("param1", param1);

        ReferenceContainer parameters;
        parameters.Insert(parametersDB);
        result &= (target.Call(&context, parameters) != ErrorManagement::NoError);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodCallerT<ClassWithCallableMethods, ErrorManagement::ErrorType (ClassWithCallableMethods::*)(MARTe::uint32 &), uint32, void, void, void> target(
                &ClassWithCallableMethods::MethodWithOneParameter_W, 0x1000);
        ReferenceContainer parameters;
        result &= (target.Call(&context, parameters) == ErrorManagement::ParametersError);
    }
    return result;
}
