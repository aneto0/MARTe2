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

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapperTest.h"
#include "ClassMethodInterfaceMapper.h"
#include "ClassWithCallableMethods.h"
#include "ReferenceContainer.h"
#include "ReturnType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ClassMethodCallerTest::ClassMethodCallerTest() {
}

ClassMethodCallerTest::~ClassMethodCallerTest() {
}

bool ClassMethodCallerTest::TestCall() {
    using namespace MARTe;
    bool result = false;
    ClassMethodCallerCT<ClassWithCallableMethods, ReferenceContainer&> target(&ClassWithCallableMethods::MethodX);
    ClassWithCallableMethods context;
    ReferenceContainer ref;
    ReturnType status;
    status = target.Call(&context, ref);
    result = status;
    return result;
}

ClassMethodInterfaceMapperTest::ClassMethodInterfaceMapperTest() {
}

ClassMethodInterfaceMapperTest::~ClassMethodInterfaceMapperTest() {
}

bool ClassMethodInterfaceMapperTest::TestCall() {
    using namespace MARTe;
    bool result = false;
    ClassMethodInterfaceMapper target(&ClassWithCallableMethods::MethodX);
    ClassWithCallableMethods context;
    ReferenceContainer ref;
    ReturnType status;
    status = target.Call(&context, ref);
    result = status;
    return result;
}
