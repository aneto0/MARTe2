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

#include <cstddef>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectTestHelper.h"
#include "Introspection.h"

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


DECLARE_CLASS_MEMBER(IntrospectableIntegerObject, member, int32, "", "");
static const IntrospectionEntry* objFields[] = { &IntrospectableIntegerObject_member_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableIntegerObject, objFields);
CLASS_INTROSPECTION_REGISTER(IntrospectableIntegerObject, "1.0", IntrospectableIntegerObject_introspection)


DECLARE_CLASS_MEMBER(IntrospectableObjectWith2Members, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith2Members, member2, uint64, "", "");
static const IntrospectionEntry* IntrospectableObjectWith2Members_objFields[] = { &IntrospectableObjectWith2Members_member1_introspectionEntry, &IntrospectableObjectWith2Members_member2_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWith2Members, IntrospectableObjectWith2Members_objFields);
CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWith2Members, "1.0", IntrospectableObjectWith2Members_introspection)


DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member2, uint64, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWith3Members, member3, IntrospectableIntegerObject, "", "");
static const IntrospectionEntry* IntrospectableObjectWith3Members_objFields[] = { &IntrospectableObjectWith3Members_member1_introspectionEntry, &IntrospectableObjectWith3Members_member2_introspectionEntry, &IntrospectableObjectWith3Members_member3_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWith3Members, IntrospectableObjectWith3Members_objFields);
CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWith3Members, "1.0", IntrospectableObjectWith3Members_introspection)

DECLARE_CLASS_MEMBER(IntrospectableObjectWithVector, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithVector, member2, uint64, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithVector, member3, int32, "[5]", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithVector, member4, float32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithVector, member5, float64, "", "");
static const IntrospectionEntry* IntrospectableObjectWithVector_objFields[] = { &IntrospectableObjectWithVector_member1_introspectionEntry, &IntrospectableObjectWithVector_member2_introspectionEntry, &IntrospectableObjectWithVector_member3_introspectionEntry, &IntrospectableObjectWithVector_member4_introspectionEntry, &IntrospectableObjectWithVector_member5_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWithVector, IntrospectableObjectWithVector_objFields);
CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWithVector, "1.0", IntrospectableObjectWithVector_introspection)

DECLARE_CLASS_MEMBER(IntrospectableObjectWithMatrix, member1, int32, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithMatrix, member2, uint64, "", "");
DECLARE_CLASS_MEMBER(IntrospectableObjectWithMatrix, member3, int32, "[5][3]", "");
static const IntrospectionEntry* IntrospectableObjectWithMatrix_objFields[] = { &IntrospectableObjectWithMatrix_member1_introspectionEntry, &IntrospectableObjectWithMatrix_member2_introspectionEntry, &IntrospectableObjectWithMatrix_member3_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(IntrospectableObjectWithMatrix, IntrospectableObjectWithMatrix_objFields);
CLASS_INTROSPECTION_REGISTER(IntrospectableObjectWithMatrix, "1.0", IntrospectableObjectWithMatrix_introspection)

IntrospectableIntegerObject& TestObjectsDataSet::GetTest3Object() {
    static bool init = false;
    static IntrospectableIntegerObject obj;
    if (!init) {
        obj.SetName("Test3");
        obj.member = 30;
        init = true;
    }
    return obj;
}

IntrospectableObjectWith2Members& TestObjectsDataSet::GetTest4Object() {
    static bool init = false;
    static IntrospectableObjectWith2Members obj;
    if (!init) {
        obj.SetName("Test4");
        obj.member1 = 10;
        obj.member2 = 20;
        init = true;
    }
    return obj;
}

IntrospectableObjectWith3Members& TestObjectsDataSet::GetTest5Object() {
    static bool init = false;
    static IntrospectableObjectWith3Members obj;
    if (!init) {
        obj.SetName("Test5");
        obj.member1 = 10;
        obj.member2 = 20;
        obj.member3.member = 30;
        init = true;
    }
    return obj;
}

IntrospectableObjectWithVector& TestObjectsDataSet::GetTest6Object() {
    static bool init = false;
    static IntrospectableObjectWithVector obj;
    if (!init) {
        obj.SetName("Test6");
        obj.member1 = 10;
        obj.member2 = 20;
        obj.member3[0] = 0;
        obj.member3[1] = -10;
        obj.member3[2] = 20;
        obj.member3[3] = -30;
        obj.member3[4] = 40;
        obj.member4 = -4000.5f;
        obj.member5 = 5000.5;
        init = true;
    }
    return obj;
}

IntrospectableObjectWithMatrix& TestObjectsDataSet::GetTest7Object() {
    static bool init = false;
    static IntrospectableObjectWithMatrix obj;
    if (!init) {
        obj.SetName("Test7");
        obj.member1 = 10;
        obj.member2 = 20;
        obj.member3[0][0] = 1;
        obj.member3[0][1] = 2;
        obj.member3[0][2] = 3;
        obj.member3[1][0] = 4;
        obj.member3[1][1] = 5;
        obj.member3[1][2] = 6;
        obj.member3[2][0] = 7;
        obj.member3[2][1] = 8;
        obj.member3[2][2] = 9;
        obj.member3[3][0] = 10;
        obj.member3[3][1] = 11;
        obj.member3[3][2] = 12;
        obj.member3[4][0] = 13;
        obj.member3[4][1] = 14;
        obj.member3[4][2] = 15;
        init = true;
    }
    return obj;
}

const StreamString& JsonStreamsDataSet::GetTest3DataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"Test3\":{\"Class\":\"IntrospectableIntegerObject\",\"member\":30}}";
        static const AnyType PARAMS[] = { voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest3MetadataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"IntrospectableIntegerObject\":{\"member\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u}}}";
        static const AnyType PARAMS[] = { static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableIntegerObject, member)), voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest4DataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"Test4\":{\"Class\":\"IntrospectableObjectWith2Members\",\"member1\":10,\"member2\":20}}";
        static const AnyType PARAMS[] = { voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest4MetadataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"IntrospectableObjectWith2Members\":{\"member1\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member2\":{\"type\":\"uint64\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u}}}";
        static const AnyType PARAMS[] = { static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableObjectWith2Members, member1)), static_cast<uint8>(sizeof(uint64)), static_cast<uint64>(offsetof(IntrospectableObjectWith2Members, member2)), voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest5DataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"Test5\":{\"Class\":\"IntrospectableObjectWith3Members\",\"member1\":10,\"member2\":20,\"member3\":{\"Class\":\"IntrospectableIntegerObject\",\"member\":30}}}";
        static const AnyType PARAMS[] = { voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest5MetadataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"IntrospectableObjectWith3Members\":{\"member1\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member2\":{\"type\":\"uint64\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member3\":{\"type\":\"IntrospectableIntegerObject\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u,\"IntrospectableIntegerObject\":{\"member\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u}}}}}";
        static const AnyType PARAMS[] = { static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableObjectWith3Members, member1)), static_cast<uint8>(sizeof(uint64)), static_cast<uint64>(offsetof(IntrospectableObjectWith3Members, member2)), static_cast<uint8>(sizeof(IntrospectableIntegerObject)), static_cast<uint64>(offsetof(IntrospectableObjectWith3Members, member3)), static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableIntegerObject, member)), voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest6DataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"Test6\":{\"Class\":\"IntrospectableObjectWithVector\",\"member1\":10,\"member2\":20,\"member3\":[0,-10,20,-30,40],\"member4\":-4.000500E+3,\"member5\":5.00050000000000E+3}}";
        static const AnyType PARAMS[] = { voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest6MetadataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"IntrospectableObjectWithVector\":{\"member1\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member2\":{\"type\":\"uint64\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member3\":{\"type\":\"int32\",\"modifiers\":\"[5]\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member4\":{\"type\":\"float32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member5\":{\"type\":\"float64\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u}}}";
        static const AnyType PARAMS[] = { static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableObjectWithVector, member1)), static_cast<uint8>(sizeof(uint64)), static_cast<uint64>(offsetof(IntrospectableObjectWithVector, member2)), static_cast<uint64>(sizeof(int32)*5), static_cast<uint64>(offsetof(IntrospectableObjectWithVector, member3)), static_cast<uint8>(sizeof(float32)), static_cast<uint64>(offsetof(IntrospectableObjectWithVector, member4)), static_cast<uint8>(sizeof(float64)), static_cast<uint64>(offsetof(IntrospectableObjectWithVector, member5)), voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest7DataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"Test7\":{\"Class\":\"IntrospectableObjectWithMatrix\",\"member1\":10,\"member2\":20,\"member3\":[[1,2,3],[4,5,6],[7,8,9],[10,11,12],[13,14,15]]}}";
        static const AnyType PARAMS[] = { voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}

const StreamString& JsonStreamsDataSet::GetTest7MetadataAsJson() {
    static bool init = false;
    static StreamString str;
    if (!init) {
        static const char8 TEMPLATE[] =
                "{\"IntrospectableObjectWithMatrix\":{\"member1\":{\"type\":\"int32\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member2\":{\"type\":\"uint64\",\"modifiers\":\"\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u},\"member3\":{\"type\":\"int32\",\"modifiers\":\"[5][3]\",\"attributes\":\"\",\"size\":%u,\"pointer\":%u}}}";
        static const AnyType PARAMS[] = { static_cast<uint8>(sizeof(int32)), static_cast<uint64>(offsetof(IntrospectableObjectWithMatrix, member1)), static_cast<uint8>(sizeof(uint64)), static_cast<uint64>(offsetof(IntrospectableObjectWithMatrix, member2)), static_cast<uint64>(sizeof(int32)*5*3), static_cast<uint64>(offsetof(IntrospectableObjectWithMatrix, member3)), voidAnyType };
        str.PrintFormatted(TEMPLATE, &PARAMS[0]);
        str.Seek(0);
        init = true;
    }
    return str;
}
