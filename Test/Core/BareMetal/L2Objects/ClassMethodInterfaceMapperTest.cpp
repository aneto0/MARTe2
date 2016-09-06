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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapperTest.h"
#include "ClassMethodInterfaceMapper.h"
#include "ClassWithCallableMethods.h"
#include "ConfigurationDatabase.h"
#include "ErrorType.h"
#include "Reference.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ClassMethodInterfaceMapperTest::ClassMethodInterfaceMapperTest() {
}

ClassMethodInterfaceMapperTest::~ClassMethodInterfaceMapperTest() {
}

bool ClassMethodInterfaceMapperTest::TestDefaultConstructor() {
    /* Warning: It is not possible to query the target about the address
     * actually set as the callable method, so it is not possible to check
     * if it has been set to NULL.
     */
    using namespace MARTe;
    ClassMethodInterfaceMapper target();
    return true;
}

bool ClassMethodInterfaceMapperTest::TestConstructorForMethodWithNoArguments() {
    /* Warning: It is not possible to query the target about the address
     * actually set as the callable method, so it is not possible to check
     * if it has been set to method.
     */
    using namespace MARTe;
    /*bool (ClassWithCallableMethods::*method)() = &ClassWithCallableMethods::OverloadedMethod;
     ClassMethodInterfaceMapper target(method);*/
    return true;
}

bool ClassMethodInterfaceMapperTest::TestConstructorForMethodWith1ArgumentByCopy() {
    /* Warning: It is not possible to query the target about the address
     * actually set as the callable method, so it is not possible to check
     * if it has been set to method.
     */
    using namespace MARTe;
    /*bool (ClassWithCallableMethods::*method)(int) = &ClassWithCallableMethods::MethodWithInputIntegerByCopy;
     ClassMethodInterfaceMapper target(method);*/
    return true;
}

bool ClassMethodInterfaceMapperTest::TestConstructorForMethodWith1ArgumentByRef() {
    /* Warning: It is not possible to query the target about the address
     * actually set as the callable method, so it is not possible to check
     * if it has been set to method.
     */
    using namespace MARTe;
    /*bool (ClassWithCallableMethods::*method)(int&) = &ClassWithCallableMethods::MethodWithOutputInteger;
     ClassMethodInterfaceMapper target(method);*/
    return true;
}

bool ClassMethodInterfaceMapperTest::TestConstructor_3_parameters() {
    using namespace MARTe;
    bool result = true;

    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_C_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_R_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_C_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_C_W_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_C_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_R_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_R_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_R_W_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_C_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_R_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithThreeParameters_W_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        StreamString param3 = "KO";
        parameters.Write("param3", param3);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithThreeParameters_W_W_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == "KOOK");
    }

    return result;
}

bool ClassMethodInterfaceMapperTest::TestConstructor_4_parameters() {
    using namespace MARTe;
    bool result = true;

    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_C_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_R_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_C");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_R");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_C_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_C_W_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_C_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_R_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_C");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_R");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_R_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_R_W_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_C_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_R_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_C_W_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_C_W_W_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_C_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_R_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_C");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_R");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_C_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_C_W_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_C_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_C");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_R");
        }
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_R_W");
        }
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_C");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_R");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_R_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_R_W_W");
        }
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_C_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_R_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_C");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_R");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_R_W_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_R_W_W_W");
        }
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_C_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_R_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_C_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_C_W_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_C_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_R_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_R_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_R_W_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_C_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_C_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_R_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_R_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_C);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_C");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_R);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_R");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
    }
    {
        ClassWithCallableMethods context;
        ClassMethodInterfaceMapper mapper(&ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W);
        ConfigurationDatabase parameters;
        uint32 param1 = 3;
        parameters.Write("param1", param1);
        float32 param2 = 2.0;
        parameters.Write("param2", param2);
        float64 param3 = -9.0;
        parameters.Write("param3", param3);
        StreamString param4 = "KO";
        parameters.Write("param4", param4);
        result &= (mapper.GetMethod() != NULL);
        if (result) {
             result &= (mapper.GetMethod()->Call(&context, parameters) == ErrorManagement::noError);
             result &= (context.GetLastMethodExecuted() == "MethodWithFourParameters_W_W_W_W");
        }
        parameters.Read("param1", param1);
        result &= (param1 == 5);
        parameters.Read("param2", param2);
        result &= (param2 == 6.0);
        parameters.Read("param3", param3);
        result &= (param3 == -7.0);
        parameters.Read("param4", param4);
        result &= (param4 == "KOOK");
    }


    return result;
}
