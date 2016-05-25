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
#include "ReturnType.h"
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

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

ClassMethodsRegistryItemTest::ClassMethodsRegistryItemTest() {
//Auto-generated constructor stub for ClassMethodsRegistryItemTest

//TODO Verify if manual additions are needed here
}

ClassMethodsRegistryItemTest::~ClassMethodsRegistryItemTest() {
//Auto-generated destructor stub for ClassMethodsRegistryItemTest

//TODO Verify if manual additions are needed here
}

bool ClassMethodsRegistryItemTest::TestConstructor() {
    bool result = false;
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction() {
    bool result = true;
    ClassRegistryItem* cri = ClassRegistryItemT<ClassWithCallableMethods>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods::MethodM, &ClassWithCallableMethods::MethodK, &ClassWithCallableMethods::MethodX, &ClassWithCallableMethods::MethodY, &ClassWithCallableMethods::MethodZ };
    const char* names = "ClassWithCallableMethods::MethodM, ClassWithCallableMethods::MethodK, ClassWithCallableMethods::MethodX, ClassWithCallableMethods::MethodY, ClassWithCallableMethods::MethodZ";
    {
        ClassMethodsRegistryItem target(cri, cmim, names);
        ClassWithCallableMethods context;
        ReferenceContainer ref;
        ReturnType status;
//        int ref2;
//        status = target.CallFunction(&context, "MethodM", ref2);
//        result &= status.error.functionReturn;
        status = target.CallFunction(&context, "NonRegisteredMethod", ref);
        result &= !status.error.notUnsupportedFeature;
        status = target.CallFunction(&context, "MethodK", ref);
        result &= !status.error.functionReturn;
        status = target.CallFunction(&context, "MethodX", ref);
        result &= status;
        status = target.CallFunction(&context, "MethodY", ref);
        result &= status;
        status = target.CallFunction(&context, "MethodZ", ref);
        result &= status;
    }
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction_WithMacroSupport() {
    bool result = true;
    {
        ClassMethodsRegistryItem& target = ClassWithCallableMethods__ClassMethodsRegistryItem;
        ClassWithCallableMethods context;
        ReferenceContainer ref;
        ReturnType status;
        status = target.CallFunction(&context, "NonRegisteredMethod", ref);
        result &= !status.error.notUnsupportedFeature;
        status = target.CallFunction(&context, "MethodK", ref);
        result &= !status.error.functionReturn;
        status = target.CallFunction(&context, "MethodX", ref);
        result &= status;
        status = target.CallFunction(&context, "MethodY", ref);
        result &= status;
        status = target.CallFunction(&context, "MethodZ", ref);
        result &= status;
    }
    return result;
}

bool ClassMethodsRegistryItemTest::TestCallFunction2() {
    bool result = false;
    ObjectRegistryDatabase* ordb = ObjectRegistryDatabase::Instance();
    Reference target;
    ReferenceContainer params;
    StreamString definition("+context={Class=ClassWithCallableMethods}");
    ConfigurationDatabase cdb;
    definition.Seek(0);
    StandardParser parser(definition, cdb);
    parser.Parse();
    ordb->Initialise(cdb);
    target = ordb->Find("+context");
    if (target.IsValid()) {
        ReturnType status;
        status = target->CallRegisteredMethod("MethodY", params);
        result = status;
    }
    return result;
}
