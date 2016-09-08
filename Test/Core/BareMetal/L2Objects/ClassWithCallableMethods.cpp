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
#include "StreamString.h"

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

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithConstInputStreamI(const MARTe::StreamI &data) {
    const MARTe::StreamString &ss = reinterpret_cast<const MARTe::StreamString &>(data);
    bool result = (ss == "MethodWithConstInputStreamI");
    lastMethodExecuted = "MethodWithConstInputStreamI(StreamI)";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOutputStreamI(MARTe::StreamI &data) {
    MARTe::StreamString &ss = reinterpret_cast<MARTe::StreamString &>(data);
    ss = "MethodWithOutputStreamI";
    lastMethodExecuted = "MethodWithOutputStreamI(StreamI&)";
    return true;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithInputOutputStreamI(MARTe::StreamI &data) {
    MARTe::StreamString &ss = reinterpret_cast<MARTe::StreamString &>(data);
    bool result = (ss == "MethodWithInputOutputStreamI");
    lastMethodExecuted = "MethodWithInputOutputStreamI(StreamI&)";
    ss = "ReturnFromMethodWithInputOutputStreamII";
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

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_C_C(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_C_R(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_C_W(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_C_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_R_C(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_R_R(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_R_W(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_C_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_W_C(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_W_R(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_C_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_C_W_W(const MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_C_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_C_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_C_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_C_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_R_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_R_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_R_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_R_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_R_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_W_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_W_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_R_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_R_W_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_R_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_C_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_C_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_C_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_W_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_R_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_R_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_R_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_W_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_W_C(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_W_R(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_C_W_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_C_W_W_W(const MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_C_W_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_C_C(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_C_R(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_C_W(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_C_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_R_C(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_R_R(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_R_W(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_C_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_W_C(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_W_R(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_C_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_C_W_W(MARTe::uint32 param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_C_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_C_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_C_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_C_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_R_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_R_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_R_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_R_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_R_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_W_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_W_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_R_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_R_W_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_R_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_C_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_C_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_C_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_W_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_R_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_R_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_R_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_W_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_W_C(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_W_R(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_R_W_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_R_W_W_W(MARTe::uint32 param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_R_W_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_C_C(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_C_R(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_C_W(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_C_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_R_C(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_R_R(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_R_W(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_C_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_W_C(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_W_R(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_C_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_C_W_W(MARTe::uint32 &param1,
                                                                                             const MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_C_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_C_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_C_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_C_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_R_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_R_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_R_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_R_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_R_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_W_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_W_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_R_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_R_W_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_R_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_C_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_C_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_C_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             const MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_W_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_R_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_R_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_R_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == -9.0);
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_W_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_W_C(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             const MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_W_R(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    param3 = -7.0;
    result &= (param4 == "KO");
    lastMethodExecuted = "MethodWithFourParameters_W_W_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParameters_W_W_W_W(MARTe::uint32 &param1,
                                                                                             MARTe::float32 &param2,
                                                                                             MARTe::float64 &param3,
                                                                                             MARTe::StreamString &param4) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    param3 = -7.0;
    param4 = "OK";
    lastMethodExecuted = "MethodWithFourParameters_W_W_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithFourParametersReturnError(MARTe::uint32 &param1,
                                                                                                MARTe::float32 &param2,
                                                                                                MARTe::float64 &param3,
                                                                                                MARTe::StreamString &param4) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_C_C(const MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_C_R(const MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_C_W(const MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_C_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_R_C(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_R_R(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_R_W(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_C_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_W_C(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_W_R(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_C_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_C_W_W(const MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_C_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_C_C(MARTe::uint32 param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_C_R(MARTe::uint32 param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_C_W(MARTe::uint32 param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_R_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_R_C(MARTe::uint32 param1,
                                                                                            MARTe::float32 param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_R_R(MARTe::uint32 param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_R_W(MARTe::uint32 param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_R_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_W_C(MARTe::uint32 param1,
                                                                                            MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_W_R(MARTe::uint32 param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_R_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_R_W_W(MARTe::uint32 param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    result &= (param1 == 3);
    param2 = 6.0;
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_R_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_C_C(MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_C_R(MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_C_W(MARTe::uint32 & param1,
                                                                                            const MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_W_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_R_C(MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_R_R(MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_R_W(MARTe::uint32 & param1,
                                                                                            MARTe::float32 param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    result &= (param2 == 2.0);
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_W_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_W_C(MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            const MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_W_R(MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString param3) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    result &= (param3 == "KO");
    lastMethodExecuted = "MethodWithThreeParameters_W_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParameters_W_W_W(MARTe::uint32 & param1,
                                                                                            MARTe::float32 & param2,
                                                                                            MARTe::StreamString & param3) {
    bool result = true;
    param1 = 5;
    param2 = 6.0;
    param3 = "OK";
    lastMethodExecuted = "MethodWithThreeParameters_W_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithThreeParametersReturnError(MARTe::uint32 &param1,
                                                                                                 MARTe::float32 &param2,
                                                                                                 MARTe::StreamString &param3) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_C_C(const MARTe::uint32 & param1,
                                                                                        const MARTe::StreamString & param2) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_C_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_C_R(const MARTe::uint32 & param1,
                                                                                        MARTe::StreamString param2) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_C_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_C_W(const MARTe::uint32 & param1,
                                                                                        MARTe::StreamString & param2) {
    bool result = true;
    result &= (param1 == 3);
    param2 = "OK";
    lastMethodExecuted = "MethodWithTwoParameters_C_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_R_C(MARTe::uint32 param1,
                                                                                        const MARTe::StreamString & param2) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_R_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_R_R(MARTe::uint32 param1,
                                                                                        MARTe::StreamString param2) {
    bool result = true;
    result &= (param1 == 3);
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_R_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_R_W(MARTe::uint32 param1,
                                                                                        MARTe::StreamString & param2) {
    bool result = true;
    result &= (param1 == 3);
    param2 = "OK";
    lastMethodExecuted = "MethodWithTwoParameters_R_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_W_C(MARTe::uint32 & param1,
                                                                                        const MARTe::StreamString & param2) {
    bool result = true;
    param1 = 5;
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_W_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_W_R(MARTe::uint32 & param1,
                                                                                        MARTe::StreamString param2) {
    bool result = true;
    param1 = 5;
    result &= (param2 == "KO");
    lastMethodExecuted = "MethodWithTwoParameters_W_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_W_W(MARTe::uint32 & param1,
                                                                                        MARTe::StreamString & param2) {
    bool result = true;
    param1 = 5;
    param2 = "OK";
    lastMethodExecuted = "MethodWithTwoParameters_W_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParametersReturnError(MARTe::uint32 &param1,
                                                                                               MARTe::StreamString &param2) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_Pointer_1(MARTe::uint32 * param1,
                                                                                              MARTe::StreamString & param2) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_Pointer_2(MARTe::uint32 & param1,
                                                                                              MARTe::StreamString * param2) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithTwoParameters_Pointer_1_2(MARTe::uint32 * param1,
                                                                                                MARTe::StreamString * param2) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOneParameter_C(const MARTe::uint32 & param1) {
    bool result = true;
    result &= (param1 == 3);
    lastMethodExecuted = "MethodWithOneParameter_C";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOneParameter_R(MARTe::uint32 param1) {
    bool result = true;
    result &= (param1 == 3);
    lastMethodExecuted = "MethodWithOneParameter_R";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOneParameter_W(MARTe::uint32 & param1) {
    bool result = true;
    param1 = 5;
    lastMethodExecuted = "MethodWithOneParameter_W";
    return result;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOneParameter_Pointer(MARTe::uint32 * param1) {
    return MARTe::ErrorManagement::FatalError;
}

MARTe::ErrorManagement::ErrorType ClassWithCallableMethods::MethodWithOneParameterReturnError(MARTe::uint32 &param1) {
    return MARTe::ErrorManagement::FatalError;
}

CLASS_REGISTER(ClassWithCallableMethods, "1.0")

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithVoidParameters)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithConstInputInteger)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputInteger)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputOutputInteger)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputIntegerByCopy)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithConstInputStructuredDataI)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputStructuredDataI)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputReferenceContainerByCopy)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputReferenceContainerAndStructuredDataIAt0)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputReferenceContainerAndStructuredDataIAt0)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithConstInputReferenceContainer)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputReferenceContainer)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputOutputReferenceContainer)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithConstInputStreamI)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOutputStreamI)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithInputOutputStreamI)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_C_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_R_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_C_W_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_C_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_R_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_R_W_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_C_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_R_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParameters_W_W_W_W)

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_C_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_R_W_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParameters_W_W_W)

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_C_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_C_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_C_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_R_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_R_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_R_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_W_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_W_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParameters_W_W)

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOneParameter_C)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOneParameter_R)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOneParameter_W)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOneParameter_Pointer)

CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithFourParametersReturnError)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithThreeParametersReturnError)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithTwoParametersReturnError)
CLASS_METHOD_REGISTER(ClassWithCallableMethods, MethodWithOneParameterReturnError)
