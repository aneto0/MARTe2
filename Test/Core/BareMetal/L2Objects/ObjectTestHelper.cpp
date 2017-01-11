/**
 * @file ObjectTestHelper.cpp
 * @brief Source file for class ObjectTestHelper
 * @date 07/08/2015
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
 * the class ObjectTestHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItemT.h"
#include "ObjectTestHelper.h"
#include "IntrospectionT.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CLASS_REGISTER(IntegerObject, "1.0")


CLASS_REGISTER(SpecialIntegerObject, "1.0")


CLASS_REGISTER(FloatObject, "1.0")


CLASS_REGISTER(CollectInts, "1.0")


CLASS_REGISTER(NonIntrospectableIntegerObject, "1.0")


CLASS_REGISTER(IntrospectableIntegerObject, "1.0")


CLASS_REGISTER(IntrospectableObjectWith2Members, "1.0")


CLASS_REGISTER(IntrospectableObjectWith3Members, "1.0")


DECLARE_CLASS_MEMBER(IntrospectableIntegerObject, member, int32, "", "");
static const IntrospectionEntry* objFields[] = { &IntrospectableIntegerObject_member_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableIntegerObject, objFields);
//CLASS_INTROSPECTION_REGISTER(IntrospectableIntegerObject, "1.0", IntrospectableIntegerObject_introspection)


DECLARE_CLASS_MEMBER(IntrospectableObjectWith2Members, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith2Members, member2, uint64, "", "");
static const IntrospectionEntry* IntrospectableObjectWith2Members_objFields[] = { &IntrospectableObjectWith2Members_member1_introspectionEntry, &IntrospectableObjectWith2Members_member2_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWith2Members, IntrospectableObjectWith2Members_objFields);
//CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWith2Members, "1.0", IntrospectableObjectWith2Members_introspection)


DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member2, uint64, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member3, IntrospectableIntegerObject, "", "");
static const IntrospectionEntry* IntrospectableObjectWith3Members_objFields[] = { &IntrospectableObjectWith3Members_member1_introspectionEntry, &IntrospectableObjectWith3Members_member2_introspectionEntry, &IntrospectableObjectWith3Members_member3_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWith3Members, IntrospectableObjectWith3Members_objFields);
//CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWith3Members, "1.0", IntrospectableObjectWith3Members_introspection)
