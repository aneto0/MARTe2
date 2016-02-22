/**
 * @file TestObjectHelper3.cpp
 * @brief Source file for class TestObjectHelper3
 * @date 22/feb/2016
 * @author pc
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
 * the class TestObjectHelper3 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestObjectHelper3.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
DECLARE_CLASS_MEMBER(NestedObjectHelper3, x, int32, "", "nested");

static const IntrospectionEntry* nestedObj3[] = { &NestedObjectHelper3_x_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(NestedObjectHelper3, nestedObj3);
CLASS_INTROSPECTION_REGISTER(NestedObjectHelper3, "1.0", NestedObjectHelper3_introspection)

DECLARE_CLASS_MEMBER(TestObjectHelper3, a, uint32, "", "hello");

DECLARE_CLASS_MEMBER(TestObjectHelper3, b, string, "", "world");

DECLARE_CLASS_MEMBER(TestObjectHelper3, c, float64, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestObjectHelper3, d, NestedObjectHelper3, "**", "hola");

DECLARE_CLASS_MEMBER(TestObjectHelper3, e, NestedObjectHelper3, "", "mundo");

static const IntrospectionEntry* fieldsObj3[] = { &TestObjectHelper3_a_introspectionEntry, &TestObjectHelper3_b_introspectionEntry,
        &TestObjectHelper3_c_introspectionEntry, &TestObjectHelper3_d_introspectionEntry, &TestObjectHelper3_e_introspectionEntry, 0 };

DECLARE_CLASS_INTROSPECTION(TestObjectHelper3, fieldsObj3);
CLASS_INTROSPECTION_REGISTER(TestObjectHelper3, "1.0", TestObjectHelper3_introspection)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TestObjectHelper3::TestObjectHelper3() {
    a = 1;
    b = "2";
    c[0][0] = 3.0;
    c[0][1] = 3.1;
    c[1][0] = 3.2;
    c[1][1] = 3.3;

    d = new NestedObjectHelper3*;
    *d = new NestedObjectHelper3;
}
TestObjectHelper3::~TestObjectHelper3() {
    delete *d;
    delete d;
}

NestedObjectHelper3::NestedObjectHelper3() {
    x = 4;
}

