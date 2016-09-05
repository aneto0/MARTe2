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

#include "CLASSMETHODREGISTER.h"
#include "ClassRegistryItemT.h"
#include "ClassWithCallableMethods.h"
#include "ConfigurationDatabase.h"
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

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::FaultyMethod(MARTe::ReferenceContainer& data) {
    lastMethodExecuted = "FaultyMethod(MARTe::ReferenceContainer&)";
    return false;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithConstInputReferenceContainer(const MARTe::ReferenceContainer& data) {
    bool result = true;
    MARTe::ReferenceContainer &forcedData = const_cast<MARTe::ReferenceContainer &>(data);
    MARTe::Reference obj = forcedData.Find("TestObject");
    result &= obj.IsValid();
    lastMethodExecuted = "MethodWithConstInputReferenceContainer(ReferenceContainer&)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOutputReferenceContainer(MARTe::ReferenceContainer& data) {
    bool result = true;
    bool status;
    MARTe::Reference obj("Object");
    status = data.Insert("TestObject2", obj);
    result &= status;
    lastMethodExecuted = "MethodWithOutputReferenceContainer(ReferenceContainer&)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputOutputReferenceContainer(MARTe::ReferenceContainer& data) {
    bool result = true;
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
    lastMethodExecuted = "MethodWithInputOutputReferenceContainer(ReferenceContainer&)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithConstInputInteger(const int& data) {
    bool result = (data == 40);
    lastMethodExecuted = "MethodWithConstInputInteger(int&)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOutputInteger(int& data) {
    data = 20;
    lastMethodExecuted = "MethodWithOutputInteger(int&)";
    return true;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputOutputInteger(int& data) {
    bool result = (data == 10);
    if (result) {
        data = 20;
    }
    lastMethodExecuted = "MethodWithInputOutputInteger(int&)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputIntegerByCopy(int data) {
    bool result = (data == 80);
    lastMethodExecuted = "MethodWithInputIntegerByCopy(int)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithConstInputStructuredDataI(const MARTe::StructuredDataI &data) {
    int value;
    MARTe::StructuredDataI &forcedData = const_cast<MARTe::StructuredDataI &>(data);
    bool result = forcedData.Read("value", value);
    result &= (value == 20);
    lastMethodExecuted = "MethodWithConstInputStructuredDataI(StructuredDataI)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOutputStructuredDataI(MARTe::StructuredDataI &data) {
    int value;
    bool result = data.Read("value", value);
    result &= (value == 30);
    value = 40;
    result &= data.Write("value", value);

    lastMethodExecuted = "MethodWithOutputStructuredDataI(StructuredDataI)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputReferenceContainerByCopy(MARTe::ReferenceContainer data) {
    bool result = true;
    MARTe::Reference obj = data.Find("TestObject");
    result &= obj.IsValid();
    lastMethodExecuted = "MethodWithInputReferenceContainerByCopy(ReferenceContainer)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputReferenceContainerAndStructuredDataIAt0(MARTe::ReferenceContainer &data) {
    bool result = true;
    MARTe::ReferenceT<MARTe::StructuredDataI> obj = data.Get(0);
    result &= obj.IsValid();
    MARTe::uint32 value;
    obj->Read("value", value);
    result &= (value == 10);
    lastMethodExecuted = "MethodWithInputReferenceContainerAndStructuredDataIAt0(ReferenceContainer)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOutputReferenceContainerAndStructuredDataIAt0(MARTe::ReferenceContainer &data) {
    bool result = true;
    MARTe::ReferenceT<MARTe::ConfigurationDatabase> obj(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
    MARTe::uint32 value = 60;
    obj->Write("value", value);
    data.Insert(obj);
    lastMethodExecuted = "MethodWithOutputReferenceContainerAndStructuredDataIAt0(ReferenceContainer&)";
    return result;
}

MARTe::StreamString ClassWithCallableMethods::GetLastMethodExecuted() {
    return lastMethodExecuted;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithVoidParameters(void) {
    lastMethodExecuted = "MethodWithVoidParameters(void)";
    return true;
}

CLASS_REGISTER(ClassWithCallableMethods, "1.0")

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithVoidParameters)
/*CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputInteger)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputOutputInteger)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, FaultyMethod)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputReferenceContainer)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputReferenceContainer)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputOutputReferenceContainer)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputIntegerByCopy)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputReferenceContainerByCopy)
 CLASS_METHOD_REGISTER(ClassWithCallableMethods, OverloadedMethod)*/
