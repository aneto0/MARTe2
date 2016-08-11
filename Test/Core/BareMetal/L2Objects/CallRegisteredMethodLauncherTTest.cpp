/**
 * @file CallRegisteredMethodLauncherTTest.cpp
 * @brief Source file for class CallRegisteredMethodLauncherTTest
 * @date 27/07/2016
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
 * the class CallRegisteredMethodLauncherTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CallRegisteredMethodLauncherTTest.h"
#include "CallRegisteredMethodLauncherT.h"
#include "ClassWithCallableMethods.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

/**
 * @brief This class is a fresh version of ClassWithCallableMethods, providing
 * the same methods but without automatic registering.
 */
class ClassWithCallableMethods2: public ClassWithCallableMethods {};

/**
 * @brief This class is a fresh version of ClassWithCallableMethods, providing
 * the same methods but without automatic registering.
 */
class ClassWithCallableMethods3: public ClassWithCallableMethods {};

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CallRegisteredMethodLauncherTTest::CallRegisteredMethodLauncherTTest() {
}

CallRegisteredMethodLauncherTTest::~CallRegisteredMethodLauncherTTest() {
}

bool CallRegisteredMethodLauncherTTest::TestDefaultConstructor() {
    bool result;
    /*
     * The state of the object after construction is not queryable, so it is
     * not possible to make a unit test for it. Therefore, the unit test
     * defaults to true.
     */
    result = true;
    return result;
}

bool CallRegisteredMethodLauncherTTest::TestTest() {

    /*
     * Warning: When registering methods with a cast like:
     *   (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod
     * If the method does not exist in the derived class, the cast must be
     * referred to the base class, while the method must be to the derived one.
     */

    using namespace MARTe;
    bool result = true;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods2>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod };
    const char* names = "&ClassWithCallableMethods2::MethodWithInputInteger, &ClassWithCallableMethods2::MethodWithOutputInteger, &ClassWithCallableMethods2::MethodWithInputOutputInteger, &ClassWithCallableMethods2::FaultyMethod, &ClassWithCallableMethods2::MethodWithInputReferenceContainer, &ClassWithCallableMethods2::MethodWithOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods2::MethodWithInputIntegerByCopy, &ClassWithCallableMethods2::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods2::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods2::OverloadedMethod";

    ClassMethodsRegistryItem cmri(cri, cmim, names);
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "NonRegisteredMethod", params);
        status = target.Test(&cmri);
        result &= (!status && target.GetResults().unsupportedFeature);
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "FaultyMethod", params);
        status = target.Test(&cmri);
        result &= (status && target.GetResults().functionError);
        result &= (context.GetLastMethodExecuted() == "FaultyMethod(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        int params = 0;
        CallRegisteredMethodLauncherT<int&> target(&context, "OverloadedMethod", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "OverloadedMethod", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        int params = 10;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithInputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        int params = 0;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithOutputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (params == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        int params = 30;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithInputOutputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (params == (30 + 5));
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithInputReferenceContainer", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainer(MARTe::ReferenceContainer&)");
        }
        else {
            result = false;
        }
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithOutputReferenceContainer", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainer(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithInputOutputReferenceContainer", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
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
        bool status;
        ClassWithCallableMethods2 context;
        int params = 80;
        CallRegisteredMethodLauncherT<int> target(&context, "MethodWithInputIntegerByCopy", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        bool status;
        ClassWithCallableMethods2 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer> target(&context, "MethodWithInputReferenceContainerByCopy", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer)");
        }
        else {
            result = false;
        }
    }

    return result;
}

bool CallRegisteredMethodLauncherTTest::TestGetResults() {

    /*
     * The method GetResults() work in tandem with Test(), in fact the
     * execution of GetResults() is linked to the last execution of Test(),
     * so the test for GetResults is the same that the Test's one, but using
     * a new ClassWithCallableMethods named ClassWithCallableMethods3.
     *
     * Warning: Trying to recycle ClassWithCallableMethods2, i.e. calling
     * TestTest() again, will cause a segmentation fault.
     *
     * Warning: When registering methods with a cast like:
     *   (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods3::OverloadedMethod
     * If the method does not exist in the derived class, the cast must be
     * referred to the base class, while the method must be to the derived one.
     */

    using namespace MARTe;
    bool result = true;
    ClassRegistryItem* const cri = ClassRegistryItemT<ClassWithCallableMethods3>::Instance();
    ClassMethodInterfaceMapper cmim[] = { &ClassWithCallableMethods3::MethodWithInputInteger, &ClassWithCallableMethods3::MethodWithOutputInteger, &ClassWithCallableMethods3::MethodWithInputOutputInteger, &ClassWithCallableMethods3::FaultyMethod, &ClassWithCallableMethods3::MethodWithInputReferenceContainer, &ClassWithCallableMethods3::MethodWithOutputReferenceContainer, &ClassWithCallableMethods3::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods3::MethodWithInputIntegerByCopy, &ClassWithCallableMethods3::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods3::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods3::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods3::OverloadedMethod };
    const char* names = "&ClassWithCallableMethods3::MethodWithInputInteger, &ClassWithCallableMethods3::MethodWithOutputInteger, &ClassWithCallableMethods3::MethodWithInputOutputInteger, &ClassWithCallableMethods3::FaultyMethod, &ClassWithCallableMethods3::MethodWithInputReferenceContainer, &ClassWithCallableMethods3::MethodWithOutputReferenceContainer, &ClassWithCallableMethods3::MethodWithInputOutputReferenceContainer, &ClassWithCallableMethods3::MethodWithInputIntegerByCopy, &ClassWithCallableMethods3::MethodWithInputReferenceContainerByCopy, (bool (ClassWithCallableMethods::*)())&ClassWithCallableMethods3::OverloadedMethod, (bool (ClassWithCallableMethods::*)(int&))&ClassWithCallableMethods3::OverloadedMethod, (bool (ClassWithCallableMethods::*)(MARTe::ReferenceContainer&))&ClassWithCallableMethods3::OverloadedMethod";

    ClassMethodsRegistryItem cmri(cri, cmim, names);
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "NonRegisteredMethod", params);
        status = target.Test(&cmri);
        result &= (!status && target.GetResults().unsupportedFeature);
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "FaultyMethod", params);
        status = target.Test(&cmri);
        result &= (status && target.GetResults().functionError);
        result &= (context.GetLastMethodExecuted() == "FaultyMethod(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        int params = 0;
        CallRegisteredMethodLauncherT<int&> target(&context, "OverloadedMethod", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "OverloadedMethod", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "OverloadedMethod(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        int params = 10;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithInputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        int params = 0;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithOutputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (params == 20);
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        int params = 30;
        CallRegisteredMethodLauncherT<int&> target(&context, "MethodWithInputOutputInteger", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (params == (30 + 5));
        result &= (context.GetLastMethodExecuted() == "MethodWithInputOutputInteger(int&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithInputReferenceContainer", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainer(MARTe::ReferenceContainer&)");
        }
        else {
            result = false;
        }
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        Reference obj;
        CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithOutputReferenceContainer", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        obj = params.Find("TestObject2");
        result &= obj.IsValid();
        result &= (context.GetLastMethodExecuted() == "MethodWithOutputReferenceContainer(MARTe::ReferenceContainer&)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObject", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer&> target(&context, "MethodWithInputOutputReferenceContainer", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
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
        bool status;
        ClassWithCallableMethods3 context;
        int params = 80;
        CallRegisteredMethodLauncherT<int> target(&context, "MethodWithInputIntegerByCopy", params);
        status = target.Test(&cmri);
        result &= (status && !target.GetResults().unsupportedFeature);
        result &= (context.GetLastMethodExecuted() == "MethodWithInputIntegerByCopy(int)");
    }
    {
        bool status;
        ClassWithCallableMethods3 context;
        ReferenceContainer params;
        Reference obj("Object");
        bool success;
        success = params.Insert("TestObjectIntoReferenceContainerByCopy", obj);
        if (success) {
            CallRegisteredMethodLauncherT<ReferenceContainer> target(&context, "MethodWithInputReferenceContainerByCopy", params);
            status = target.Test(&cmri);
            result &= (status && !target.GetResults().unsupportedFeature);
            result &= (context.GetLastMethodExecuted() == "MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer)");
        }
        else {
            result = false;
        }
    }

    return result;
}
