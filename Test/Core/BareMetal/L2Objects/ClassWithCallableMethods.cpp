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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

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
    return true;
}

bool ClassWithCallableMethods::MethodK(MARTe::ReferenceContainer& data) {
    return false;
}

bool ClassWithCallableMethods::MethodX(MARTe::ReferenceContainer& data) {
    bool result= true;
    MARTe::Reference obj = data.Find("TestObject");
    result &= obj.IsValid();
    return result;
}

bool ClassWithCallableMethods::MethodX() {
    return true;
}

bool ClassWithCallableMethods::MethodY(MARTe::ReferenceContainer& data) {
    bool result= true;
    bool status;
    MARTe::Reference obj("Object");
    status = data.Insert("TestObject2", obj);
    result &= status;
    return result;
}

bool ClassWithCallableMethods::MethodZ(MARTe::ReferenceContainer& data) {
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
    return result;
}

bool ClassWithCallableMethods::MethodWithInputInteger(int& data) {
    return (data == 10);
}

bool ClassWithCallableMethods::MethodWithOutputInteger(int& data) {
    data = 20;
    return true;
}

bool ClassWithCallableMethods::MethodWithInputOutputInteger(int& data) {
    data = data + 5;
    return true;
}

CLASS_REGISTER(ClassWithCallableMethods,"1.0")
