/**
 * @file AnyTypeTest.cpp
 * @brief Source file for class AnyTypeTest
 * @date 31/08/2015
 * @author Llorenç Capellà
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
 * the class AnyTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyTypeTest.h"
#include "TestObjectHelper1.h"
#include "CLASSMEMBERREGISTER.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct TestATStructure {
    uint32 member1;
};

CLASS_MEMBER_REGISTER(TestATStructure, member1)

//DECLARE_CLASS_MEMBER(TestATStructure, member1, uint32, "", "");
//static const IntrospectionEntry* fields[] = { &TestATStructure_member1_introspectionEntry, 0 };
//DECLARE_STRUCT_INTROSPECTION(TestATStructure, fields);



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;


bool AnyTypeTest::TestAnyType_Void() {
    AnyType anytype;
    const VariableDescriptor &vd= anytype.GetFullVariableDescriptor();
    const TypeDescriptor     &td= vd.GetSummaryTypeDescriptor();

    bool retVal = (anytype.GetVariablePointer() == NULL);
    retVal &= (td.SameAs(VoidType));
    return retVal;
}

bool AnyTypeTest::TestAnyType_AnyTypeVoid() {
    AnyType anytype;
    AnyType anytype1(anytype);

    const VariableDescriptor &vd= anytype1.GetFullVariableDescriptor();
    const TypeDescriptor     &td= vd.GetSummaryTypeDescriptor();

    bool retVal = (anytype.GetVariablePointer() == NULL);
    retVal &= (td.SameAs(VoidType));
    return retVal;
}


bool AnyTypeTest::TestSetDataPointer() {
    AnyType at;
    int8 x;
    at.Setup(SignedInteger8Bit,"",&x);
    return at.GetVariablePointer() == &x;
}

bool AnyTypeTest::TestGetDataPointer() {
    return TestSetDataPointer();
}

bool AnyTypeTest::TestGetTypeDescriptor() {
    AnyType at(UnsignedInteger64Bit,NULL);
    return at.GetFullVariableDescriptor().GetFinalTypeDescriptor().SameAs(UnsignedInteger64Bit);
}


