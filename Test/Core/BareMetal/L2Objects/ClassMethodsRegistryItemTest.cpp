/**
 * @file ClassMethodsRegistryItemTest.cpp
 * @brief Source file for class ClassMethodsRegistryItemTest
 * @date 23/05/2016
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
 * the class ClassMethodsRegistryItemTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodsRegistryItemTest.h"
#include "ClassWithCallableMethods.h"
#include "Object.h"
#include "ReferenceContainer.h"
#include "ClassRegistryItem.h"
#include "ErrorType.h"
#include "ClassMethodsRegistryItem.h"
#include "ClassMethodInterfaceMapper.h"
#include "ObjectRegistryDatabase.h"
#include "ConfigurationDatabase.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

//using namespace MARTe;

//class ClassWithCallableMethods: public Object {
//public:
//    CLASS_REGISTER_DECLARATION()
//    ClassWithCallableMethods() {};
//    virtual ~ClassWithCallableMethods() {};
//    /**
//     * Method which simulates an error execution (i.e. returns false)
//     */
//    bool FaultyMethod(ReferenceContainer& ref) {
//        return false;
//    }
//    /**
//     *
//     */
//    bool NonRegisteredMethod(ReferenceContainer& ref) {
//        return true;
//    }
//    /**
//     * consumes input from a ReferenceContainer
//     */
//    bool MethodWithInputReferenceContainer(ReferenceContainer& ref) {
//        return true;
//    };
//    bool MethodWithInputReferenceContainer() {
//        return true;
//    }
//    /**
//     * produces output into a ReferenceContainer
//     */
//    bool MethodWithOutputReferenceContainer(ReferenceContainer& ref) {
//        return true;
//    };
//    /**
//     * consumes/produces from/into a ReferenceContainer
//     */
//    bool MethodWithInputOutputReferenceContainer(ReferenceContainer& ref) {
//        return true;
//    };
//
//    /**
//     * consumes an integer
//     */
//    bool MethodM(int ref) {
//        return true;
//    }
//};
//
//CLASS_REGISTER(ClassWithCallableMethods,"1.0")
//
//CLASS_METHOD_REGISTER(ClassWithCallableMethods, &ClassWithCallableMethods::FaultyMethod, &ClassWithCallableMethods::MethodWithInputReferenceContainer, &ClassWithCallableMethods::MethodWithOutputReferenceContainer, &ClassWithCallableMethods::MethodWithInputOutputReferenceContainer)

class ClassWithCallableMethods1: public ClassWithCallableMethods {};
class ClassWithCallableMethods2: public ClassWithCallableMethods {};

//CLASS_METHOD_REGISTER(ClassWithCallableMethods1, &ClassWithCallableMethods1::FaultyMethod, (bool (ClassWithCallableMethods1::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods1::MethodWithInputReferenceContainer, &ClassWithCallableMethods1::MethodWithOutputReferenceContainer, &ClassWithCallableMethods1::MethodWithInputOutputReferenceContainer)
//CLASS_METHOD_REGISTER(ClassWithCallableMethods2, &ClassWithCallableMethods2::FaultyMethod, (bool (ClassWithCallableMethods2::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer)

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ClassMethodsRegistryItemTest::ClassMethodsRegistryItemTest() {
//Auto-generated constructor stub for ClassMethodsRegistryItemTest

//TODO Verify if manual additions are needed here
}

ClassMethodsRegistryItemTest::~ClassMethodsRegistryItemTest() {
//Auto-generated destructor stub for ClassMethodsRegistryItemTest

//TODO Verify if manual additions are needed here
}

bool ClassMethodsRegistryItemTest::TestConstructor() {
    using namespace MARTe;
    bool result = false;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods1>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod };
    const char* names = "&ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod";
    ClassMethodsRegistryItem target(cri, cmim, names);
    result = (target.Size() > 0);
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction() {
    using namespace MARTe;
    bool result = true;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods2>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod };
    const char* names = "&ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod";

    /*
     * Warning about the registered methods with a cast like:
     *
     *   (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod
     *
     * If the method does not exist in the derived class, the cast must be refered to the base class, while the method to the derived one.
     */

    ClassMethodsRegistryItem target(cri, cmim, names);
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        status = target.CallFunction(&context, "NonRegisteredMethod", params);
        result &= status.unsupportedFeature;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        status = target.CallFunction(&context, "FaultyMethod", params);
        result &= status.functionError;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        status = target.CallFunction(&context, "OverloadedMethod");
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target.CallFunction<int&>(&context, "OverloadedMethod", params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target.CallFunction<ReferenceContainer&>(&context, "OverloadedMethod", params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        int params = 10;
        status = target.CallFunction<int&>(&context, "MethodWithInputInteger", params);
        result &= status;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        int params = 0;
        status = target.CallFunction<int&>(&context, "MethodWithOutputInteger", params);
        result &= status;
        result &= (params == 20);
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        int params = 30;
        status = target.CallFunction<int&>(&context, "MethodWithInputOutputInteger", params);
        result &= status;
        result &= (params == (30 + 5));
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.CallFunction<ReferenceContainer&>(&context, "MethodWithInputReferenceContainer", params);
            result &= status;
        }
        else {
            result = false;
        }
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj;
        status = target.CallFunction<ReferenceContainer&>(&context, "MethodWithOutputReferenceContainer", params);
        result &= status;
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target.CallFunction<ReferenceContainer&>(&context, "MethodWithInputOutputReferenceContainer", params);
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
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        int params = 80;
        status = target.CallFunction<int>(&context, "MethodWithInputIntegerByCopy", params);
        result &= status;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
        if (success) {
            status = target.CallFunction<ReferenceContainer>(&context, "MethodWithInputReferenceContainerByCopy", params);
            result &= status;
        }
        else {
            result = false;
        }
    }
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction_WithMacroSupport() {
    using namespace MARTe;
    bool result = true;
    ClassMethodsRegistryItem* const target = &ClassWithCallableMethods__ClassMethodsRegistryItem;
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target->CallFunction<ReferenceContainer&>(&context, "NonRegisteredMethod", params);
        result &= status.unsupportedFeature;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target->CallFunction<ReferenceContainer&>(&context, "FaultyMethod", params);
        result &= status.functionError;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        status = target->CallFunction(&context, "OverloadedMethod");
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target->CallFunction<int&>(&context, "OverloadedMethod", params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        status = target->CallFunction<ReferenceContainer&>(&context, "OverloadedMethod", params);
        result &= status;
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 10;
        status = target->CallFunction<int&>(&context, "MethodWithInputInteger", params);
        result &= status;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 0;
        status = target->CallFunction<int&>(&context, "MethodWithOutputInteger", params);
        result &= status;
        result &= (params == 20);
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 30;
        status = target->CallFunction<int&>(&context, "MethodWithInputOutputInteger", params);
        result &= status;
        result &= (params == (30 + 5));
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target->CallFunction<ReferenceContainer&>(&context, "MethodWithInputReferenceContainer", params);
            result &= status;
        }
        else {
            result = false;
        }
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj;
        status = target->CallFunction<ReferenceContainer&>(&context, "MethodWithOutputReferenceContainer", params);
        result &= status;
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            status = target->CallFunction<ReferenceContainer&>(&context, "MethodWithInputOutputReferenceContainer", params);
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
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        int params = 80;
        status = target->CallFunction<int>(&context, "MethodWithInputIntegerByCopy", params);
        result &= status;
    }
    {
        ErrorManagement::ErrorType status;
        ClassWithCallableMethods context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
        if (success) {
            status = target->CallFunction<ReferenceContainer>(&context, "MethodWithInputReferenceContainerByCopy", params);
            result &= status;
        }
        else {
            result = false;
        }
    }
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction2() {
    using namespace MARTe;
    bool result = true;
    Reference target;
    ObjectRegistryDatabase* const ordb = ObjectRegistryDatabase::Instance();
    StreamString definition("+context={Class=ClassWithCallableMethods}");
    ConfigurationDatabase cdb;
    StandardParser parser(definition, cdb);
    definition.Seek(0);
    parser.Parse();
    ordb->Initialise(cdb);
    target = ordb->Find("+context");
    if (target.IsValid()) {
        {
            ErrorManagement::ErrorType status;
            ReferenceContainer params;
            status = target->CallRegisteredMethod("NonRegisteredMethod", params);
            result &= status.unsupportedFeature;
        }
        {
            ErrorManagement::ErrorType status;
            ReferenceContainer params;
            status = target->CallRegisteredMethod("FaultyMethod", params);
            result &= status.functionError;
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            status = target->CallRegisteredMethod("OverloadedMethod");
            result &= status;
            result &= (context.GetLastMethodExecuted() == "OverloadedMethod()");
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            int params = 0;
            status = target->CallRegisteredMethod<int&>("OverloadedMethod", params);
            result &= status;
            result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            ReferenceContainer params;
            status = target->CallRegisteredMethod<ReferenceContainer&>("OverloadedMethod", params);
            result &= status;
            result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            int params;
            params = 10;
            status = target->CallRegisteredMethod<int&>("MethodWithInputInteger", params);
            result &= status;
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            int params;
            status = target->CallRegisteredMethod<int&>( "MethodWithOutputInteger", params);
            result &= status;
            result &= (params == 20);
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            int params = 30;
            status = target->CallRegisteredMethod<int&>("MethodWithInputOutputInteger", params);
            result &= status;
            result &= (params == (30 + 5));
        }
        {
            ErrorManagement::ErrorType status;
            ReferenceContainer params;
            Reference obj("Object");
            bool success;
            success = params.Insert("TestObject", obj);
            if (success) {
                status = target->CallRegisteredMethod<ReferenceContainer&>("MethodWithInputReferenceContainer", params);
                result &= status;
            }
            else {
                result = false;
            }
        }
        {
            ErrorManagement::ErrorType status;
            ReferenceContainer params;
            Reference obj;
            status = target->CallRegisteredMethod<ReferenceContainer&>("MethodWithOutputReferenceContainer", params);
            result &= status;
            obj = params.Find("TestObject2");
            result &= obj.IsValid();
        }
        {
            ErrorManagement::ErrorType status;
            ReferenceContainer params;
            Reference obj("Object");
            bool success;
            success = params.Insert("TestObject", obj);
            if (success) {
                status = target->CallRegisteredMethod<ReferenceContainer&>("MethodWithInputOutputReferenceContainer", params);
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
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            int params = 80;
            status = target->CallRegisteredMethod<int>("MethodWithInputIntegerByCopy", params);
            result &= status;
        }
        {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            ReferenceContainer params;
            Reference obj("Object");
            bool success;
            success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
            if (success) {
                status = target->CallRegisteredMethod<ReferenceContainer>("MethodWithInputReferenceContainerByCopy", params);
                result &= status;
            }
            else {
                result = false;
            }
        }
    }
    return result;
}
