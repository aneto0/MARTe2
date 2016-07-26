/**
 * @file ClassWithCallableMethods.cpp
 * @brief Source file for class ClassWithCallableMethods
 * @date 24/05/2016
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
 * the class ClassWithCallableMethods (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemT.h"
#include "ClassWithCallableMethods.h"
#include "ClassMethodsRegistryItem.h"
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ClassWithCallableMethods::ClassWithCallableMethods() {
}

ClassWithCallableMethods::~ClassWithCallableMethods() {
}

bool ClassWithCallableMethods::NonRegisteredMethod(MARTe::ReferenceContainer& data) {
    lastMethodExecuted = "NonRegisteredMethod(MARTe::ReferenceContainer&)";
    return true;
}

bool ClassWithCallableMethods::FaultyMethod(MARTe::ReferenceContainer& data) {
    lastMethodExecuted = "FaultyMethod(MARTe::ReferenceContainer&)";
    return false;
}

bool ClassWithCallableMethods::OverloadedMethod() {
    lastMethodExecuted = "OverloadedMethod()";
    return true;
}

bool ClassWithCallableMethods::OverloadedMethod(int& data) {
    lastMethodExecuted = "OverloadedMethod(int&)";
    return true;
}

bool ClassWithCallableMethods::OverloadedMethod(MARTe::ReferenceContainer& data) {
    lastMethodExecuted = "OverloadedMethod(MARTe::ReferenceContainer&)";
    return true;
}

bool ClassWithCallableMethods::MethodWithInputReferenceContainer(MARTe::ReferenceContainer& data) {
    bool result= true;
    MARTe::Reference obj = data.Find("TestObject");
    result &= obj.IsValid();
    lastMethodExecuted = "MethodWithInputReferenceContainer(MARTe::ReferenceContainer&)";
    return result;
}

bool ClassWithCallableMethods::MethodWithOutputReferenceContainer(MARTe::ReferenceContainer& data) {
    bool result= true;
    bool status;
    MARTe::Reference obj("Object");
    status = data.Insert("TestObject2", obj);
    result &= status;
    lastMethodExecuted = "MethodWithOutputReferenceContainer(MARTe::ReferenceContainer&)";
    return result;
}

bool ClassWithCallableMethods::MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer& data) {
    bool result= true;
    {
        bool status;
        MARTe::Reference obj = data.Find("TestObject");
        result &= obj.IsValid();
        status = data.Delete(obj);
        result &= status;
    }
    {
        bool status;
        MARTe::Reference obj("Object");
        status = data.Insert("TestObject2", obj);
        result &= status;
    }
    lastMethodExecuted = "MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer&)";
    return result;
}

bool ClassWithCallableMethods::MethodWithInputInteger(int& data) {
    bool result= (data == 10);
    lastMethodExecuted = "MethodWithInputInteger(int&)";
    return result;
}

bool ClassWithCallableMethods::MethodWithOutputInteger(int& data) {
    data = 20;
    lastMethodExecuted = "MethodWithOutputInteger(int&)";
    return true;
}

bool ClassWithCallableMethods::MethodWithInputOutputInteger(int& data) {
    data = data + 5;
    lastMethodExecuted = "MethodWithInputOutputInteger(int&)";
    return true;
}

bool ClassWithCallableMethods::MethodWithInputIntegerByCopy(int data) {
    bool result= (data == 80);
    lastMethodExecuted = "MethodWithInputIntegerByCopy(int)";
    return result;
}

bool ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer data) {
    bool result= true;
    MARTe::Reference obj = data.Find("TestObjectIntoReferenceContainerByCopy");
    result &= obj.IsValid();
    lastMethodExecuted = "MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer)";
    return result;
}

MARTe::StreamString ClassWithCallableMethods::GetLastMethodExecuted() {
    return lastMethodExecuted;
}

CLASS_REGISTER(ClassWithCallableMethods,"1.0")
