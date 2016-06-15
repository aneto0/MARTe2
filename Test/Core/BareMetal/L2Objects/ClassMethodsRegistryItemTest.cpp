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
//    bool MethodK(ReferenceContainer& ref) {
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
//    bool MethodX(ReferenceContainer& ref) {
//        return true;
//    };
//    bool MethodX() {
//        return true;
//    }
//    /**
//     * produces output into a ReferenceContainer
//     */
//    bool MethodY(ReferenceContainer& ref) {
//        return true;
//    };
//    /**
//     * consumes/produces from/into a ReferenceContainer
//     */
//    bool MethodZ(ReferenceContainer& ref) {
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
//CLASS_METHOD_REGISTER(ClassWithCallableMethods, &ClassWithCallableMethods::MethodK, &ClassWithCallableMethods::MethodX, &ClassWithCallableMethods::MethodY, &ClassWithCallableMethods::MethodZ)

class ClassWithCallableMethods1: public ClassWithCallableMethods {};
class ClassWithCallableMethods2: public ClassWithCallableMethods {};

CLASS_METHOD_REGISTER(ClassWithCallableMethods1, &ClassWithCallableMethods1::MethodK, &ClassWithCallableMethods1::MethodX, &ClassWithCallableMethods1::MethodY, &ClassWithCallableMethods1::MethodZ)
CLASS_METHOD_REGISTER(ClassWithCallableMethods2, &ClassWithCallableMethods2::MethodK, &ClassWithCallableMethods2::MethodX, &ClassWithCallableMethods2::MethodY, &ClassWithCallableMethods2::MethodZ)

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
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods1::MethodM, &ClassWithCallableMethods1::MethodK, &ClassWithCallableMethods1::MethodX, &ClassWithCallableMethods1::MethodY, &ClassWithCallableMethods1::MethodZ };
    const char* names = "ClassWithCallableMethods1::MethodM, ClassWithCallableMethods1::MethodK, ClassWithCallableMethods1::MethodX, ClassWithCallableMethods1::MethodY, ClassWithCallableMethods1::MethodZ";
    ClassMethodsRegistryItem target(cri, cmim, names);
    result = (target.Size() > 0);
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction() {
    using namespace MARTe;
    bool result = true;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods2>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods2::MethodM, &ClassWithCallableMethods2::MethodK, &ClassWithCallableMethods2::MethodX, &ClassWithCallableMethods2::MethodY, &ClassWithCallableMethods2::MethodZ };
    const char* names = "ClassWithCallableMethods2::MethodM, ClassWithCallableMethods2::MethodK, ClassWithCallableMethods2::MethodX, ClassWithCallableMethods2::MethodY, ClassWithCallableMethods2::MethodZ";
    ClassMethodsRegistryItem target(cri, cmim, names);
    {
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target.CallFunction(&context, "NonRegisteredMethod", params);
        result &= status.unsupportedFeature;
    }
    {
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target.CallFunction(&context, "MethodK", params);
        result &= status.functionError;
    }
    {
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("A.B.C.TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods2 context;
            status = target.CallFunction<ReferenceContainer&>(&context, "MethodX", params);
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
        status = target.CallFunction<ReferenceContainer&>(&context, "MethodY", params);
        result &= status;
        obj = params.Find("X.Y.Z.TestObject");
        result &= obj.IsValid();
    }
    {
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("A.B.C.TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods2 context;
            status = target.CallFunction<ReferenceContainer&>(&context, "MethodZ", params);
            result &= status;
            obj = params.Find("A.B.C.TestObject");
            result &= !obj.IsValid();
            obj = params.Find("X.Y.Z.TestObject");
            result &= obj.IsValid();
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
        ClassWithCallableMethods context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target->CallFunction<ReferenceContainer&>(&context, "NonRegisteredMethod", params);
        result &= status.unsupportedFeature;
    }
    {
        ClassWithCallableMethods context;
        ReferenceContainer params;
        ErrorManagement::ErrorType status;
        status = target->CallFunction<ReferenceContainer&>(&context, "MethodK", params);
        result &= status.functionError;
    }
    {
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("A.B.C.TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            status = target->CallFunction<ReferenceContainer&>(&context, "MethodX", params);
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
        status = target->CallFunction<ReferenceContainer&>(&context, "MethodY", params);
        result &= status;
        obj = params.Find("X.Y.Z.TestObject");
        result &= obj.IsValid();
    }
    {
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("A.B.C.TestObject", obj);
        if (success) {
            ErrorManagement::ErrorType status;
            ClassWithCallableMethods context;
            status = target->CallFunction<ReferenceContainer&>(&context, "MethodZ", params);
            result &= status;
            obj = params.Find("A.B.C.TestObject");
            result &= !obj.IsValid();
            obj = params.Find("X.Y.Z.TestObject");
            result &= obj.IsValid();
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
            ReferenceContainer params;
            ErrorManagement::ErrorType status;
            status = target->CallRegisteredMethod("NonRegisteredMethod", params);
            result &= status.unsupportedFeature;
        }
        {
            ReferenceContainer params;
            ErrorManagement::ErrorType status;
            status = target->CallRegisteredMethod("MethodK", params);
            result &= status.functionError;
        }
        {
            ReferenceContainer params;
            Reference obj("Object");
            bool success;
            success = params.Insert("A.B.C.TestObject", obj);
            if (success) {
                ErrorManagement::ErrorType status;
                status = target->CallRegisteredMethod<ReferenceContainer&>("MethodX", params);
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
            status = target->CallRegisteredMethod<ReferenceContainer&>("MethodY", params);
            result &= status;
            obj = params.Find("X.Y.Z.TestObject");
            result &= obj.IsValid();
        }
        {
            ReferenceContainer params;
            Reference obj("Object");
            bool success;
            success = params.Insert("A.B.C.TestObject", obj);
            if (success) {
                ErrorManagement::ErrorType status;
                status = target->CallRegisteredMethod<ReferenceContainer&>("MethodZ", params);
                result &= status;
                obj = params.Find("A.B.C.TestObject");
                result &= !obj.IsValid();
                obj = params.Find("X.Y.Z.TestObject");
                result &= obj.IsValid();
            }
            else {
                result = false;
            }
        }
    }
    return result;
}
