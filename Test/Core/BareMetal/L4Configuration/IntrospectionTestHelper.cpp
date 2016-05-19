/**
 * @file IntrospectionTestHelper.cpp
 * @brief Source file for class IntrospectionTestHelper
 * @date 26/01/2016
 * @author Giuseppe Ferrò
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
 * the class IntrospectionTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IntrospectionTestHelper.h"
#include "Object.h"
#include "Introspection.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

// the from introspection
DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureFrom, nestedMember1_from, uint32, "*", "");

DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureFrom, nestedMember2_from, uint32, "", "");

static const IntrospectionEntry* nestedFields[] = { &TestIntrospectionNestedStructureFrom_nestedMember1_from_introspectionEntry,
        &TestIntrospectionNestedStructureFrom_nestedMember2_from_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionNestedStructureFrom , nestedFields);
//INTROSPECTION_REGISTER(TestIntrospectionNestedStructureFrom, "1.0", TestIntrospectionNestedStructureFrom_introspection)

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member1_from, uint32, "", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member2_from, float32, "*", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member3_from, float64, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member4_from, string, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectFrom, member5_from, TestIntrospectionNestedStructureFrom, "", "");

static const IntrospectionEntry* fieldsFrom[] = { &TestIntrospectionObjectFrom_member1_from_introspectionEntry,
        &TestIntrospectionObjectFrom_member2_from_introspectionEntry, &TestIntrospectionObjectFrom_member3_from_introspectionEntry,
        &TestIntrospectionObjectFrom_member4_from_introspectionEntry, &TestIntrospectionObjectFrom_member5_from_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionObjectFrom , fieldsFrom);
//INTROSPECTION_REGISTER(TestIntrospectionObjectFrom, "1.0", TestIntrospectionObjectFrom_introspection)

//the to introspection
DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureTo, nestedMember1_to, uint32, "*", "");

DECLARE_CLASS_MEMBER(TestIntrospectionNestedStructureTo, nestedMember2_to, string, "", "");

static const IntrospectionEntry* nestedFieldsTo[] = { &TestIntrospectionNestedStructureTo_nestedMember1_to_introspectionEntry,
        &TestIntrospectionNestedStructureTo_nestedMember2_to_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionNestedStructureTo , nestedFieldsTo);
//INTROSPECTION_REGISTER(TestIntrospectionNestedStructureTo, "1.0", TestIntrospectionNestedStructureTo_introspection)

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member1_to, char8, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member2_to, uint64, "", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member3_to, float32, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member4_to, uint32, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectTo, member5_to, TestIntrospectionNestedStructureTo, "", "");

static const IntrospectionEntry* fieldsTo[] = { &TestIntrospectionObjectTo_member1_to_introspectionEntry,
        &TestIntrospectionObjectTo_member2_to_introspectionEntry, &TestIntrospectionObjectTo_member3_to_introspectionEntry,
        &TestIntrospectionObjectTo_member4_to_introspectionEntry, &TestIntrospectionObjectTo_member5_to_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionObjectTo , fieldsTo);
//INTROSPECTION_REGISTER(TestIntrospectionObjectTo, "1.0", TestIntrospectionObjectTo_introspection)

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, member1_to, char8, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, member2_to, uint64, "", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, member3_to, float32, "[32]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, member4_to, uint32, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, x, int32, "", "");

DECLARE_CLASS_MEMBER(TestIntrospectionObjectNoCompatibility, member5_to, TestIntrospectionNestedStructureTo, "", "");

static const IntrospectionEntry* fieldsNoCompatibility[] = { &TestIntrospectionObjectNoCompatibility_member1_to_introspectionEntry,
        &TestIntrospectionObjectNoCompatibility_member2_to_introspectionEntry, &TestIntrospectionObjectNoCompatibility_member3_to_introspectionEntry,
        &TestIntrospectionObjectNoCompatibility_member4_to_introspectionEntry, &TestIntrospectionObjectNoCompatibility_x_introspectionEntry,
        &TestIntrospectionObjectNoCompatibility_member5_to_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIntrospectionObjectNoCompatibility , fieldsNoCompatibility);
//INTROSPECTION_REGISTER(TestIntrospectionObjectNoCompatibility, "1.0", TestIntrospectionObjectNoCompatibility_introspection)

//static const ClassProperties properties("TestNoIntrospectionObject", "TestNoIntrospectionObject", "1.0");
//static const ClassRegistryItem item(properties, NULL);
CLASS_REGISTER(TestNoIntrospectionObject, "1.0")
