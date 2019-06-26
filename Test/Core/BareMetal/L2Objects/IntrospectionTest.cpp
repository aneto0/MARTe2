/**
 * @file IntrospectionTest.cpp
 * @brief Source file for class IntrospectionTest
 * @date 18/01/2016
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
 * the class IntrospectionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManager.h"
#include "IntrospectionTest.h"
#include "Object.h"
#include "ClassRegistryDatabase.h"
#include "CLASSMEMBERREGISTER.h"
#include "CompositeErrorManagement.h"
#include "AnyType.h"


using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct TestIntrospectionNestedStructure {
    uint32 nestedMember1;
};

struct TestIntrospectionStructure {
    uint32 member1;
    float32 *member2;
    float64 member3[32];
    CCString member4;
    TestIntrospectionNestedStructure member5;

    // dummy constructor to please compiler
    TestIntrospectionStructure():member4(""){
    	member1 =0;
    	member2 =NULL;
    }
    // dummy copy to please compiler
    void operator=(const TestIntrospectionStructure & s){
    	member1 = s.member1;
    }
};

/**
 * @brief Helper class to support the testing of references.
 */
class DummyObject: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    DummyObject() {
        member = 1;
    }

    virtual ~DummyObject() {
    }

    int32 member;

};

CLASS_REGISTER(DummyObject, "1.0");
CLASS_MEMBER_REGISTER(DummyObject, member);

CLASS_MEMBER_REGISTER(TestIntrospectionNestedStructure, nestedMember1);

CLASS_MEMBER_REGISTER(TestIntrospectionStructure, member1);
CLASS_MEMBER_REGISTER(TestIntrospectionStructure, member2);
CLASS_MEMBER_REGISTER(TestIntrospectionStructure, member3);
CLASS_MEMBER_REGISTER(TestIntrospectionStructure, member4);
CLASS_MEMBER_REGISTER(TestIntrospectionStructure, member5);


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


template <class T>
static ErrorManagement::ErrorType CheckIntroType(CCString memberName,const AnyType &type,uint32 offset){
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<T>();

	const ClassMember *cm1 = cri->FindMember(memberName);
	ret.fatalError = (cm1 == NULL);
	COMPOSITE_REPORT_ERROR(ret,"cannot find member ",memberName);
	if (ret){
		const VariableDescriptor vdi = cm1->GetDescriptor();
		const VariableDescriptor vdc = type.GetFullVariableDescriptor();

		ret.fatalError = !(vdi.GetSummaryTypeDescriptor().SameAs(vdc.GetSummaryTypeDescriptor())  && CCString(vdi.GetModifiers()) == vdc.GetModifiers());
		if (!ret){
			DynamicCString vdiTypeName;
			vdi.ToString(vdiTypeName);
			DynamicCString vdcTypeName;
			vdc.ToString(vdcTypeName);
			COMPOSITE_REPORT_ERROR(ret,memberName," is not ",vdcTypeName," but ",vdiTypeName );
		}

		if (ret){
			ret.fatalError = (cm1->GetOffset() != offset);
			COMPOSITE_REPORT_ERROR(ret,memberName," is not at offset ",offset," but ",cm1->GetOffset() );
		}
	}


	return ret;
};

bool IntrospectionTest::TestConstructor() {
	ErrorManagement::ErrorType ret;

	TestIntrospectionStructure tis;

	ret = CheckIntroType<TestIntrospectionStructure>("member1",tis.member1,indexof(TestIntrospectionStructure,member1));
	ret = CheckIntroType<TestIntrospectionStructure>("member2",tis.member2,indexof(TestIntrospectionStructure,member2));

#if 0

	////// check member 1
	ClassRegistryItem *cri = ClassRegistryItem::Instance<TestIntrospectionStructure>();

	ClassMember *cm1 = cri->FindMember("member1");
	ret.fatalError = (cm1 == NULL);
	REPORT_ERROR(ret,"cannot find member member1");
	if (ret){
		VariableDescriptor vd1 = cm1->GetDescriptor();
		TypeDescriptor td1 = vd1.GetSummaryTypeDescriptor();

		ret.fatalError = (td1 != UnsignedInteger32Bit);
		REPORT_ERROR(ret,"member1 is not uint32");
	}

    const IntrospectionEntry member1Copy = TestIntrospectionStructure_introspection[0];
    if (StringHelper::Compare(member1Copy.GetMemberName(), "member1") != 0) {
        return false;
    }

    if (member1Copy.IsConstant()) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().type != UnsignedInteger) {
        return false;
    }

    if (member1Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member1Copy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (member1Copy.GetMemberByteOffset() != 0) {
        return false;
    }

    if (member1Copy.GetMemberSize() != 4) {
        return false;
    }

    //////// check member2

    const IntrospectionEntry member2Copy = TestIntrospectionStructure_introspection[1];
    if (StringHelper::Compare(member2Copy.GetMemberName(), "member2") != 0) {
        return false;
    }

    if (member2Copy.IsConstant()) {
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().type != Float) {
        return false;
    }

    if (member2Copy.GetMemberTypeDescriptor().numberOfBits != 32) {
        return false;
    }

    if (StringHelper::Compare(member2Copy.GetMemberModifiers(), "*") != 0) {
        return false;
    }

    if (member2Copy.GetMemberSize() != sizeof(float32*)) {
        return false;
    }

    // the members are aligned !
    if (member2Copy.GetMemberByteOffset() != sizeof(float32*)) {
        return false;
    }
    return true;
#endif
    return ret;
}

bool IntrospectionTest::TestPositionOperator() {
	ErrorManagement::ErrorType ret;

	TestIntrospectionStructure tis;

	ret = CheckIntroType<TestIntrospectionStructure>("member3",tis.member3,indexof(TestIntrospectionStructure,member3));
#if 0






    //////// check member3
    const IntrospectionEntry member3Copy = TestIntrospectionStructure_introspection[2];
    if (StringHelper::Compare(member3Copy.GetMemberName(), "member3") != 0) {
        return false;
    }

    if (member3Copy.IsConstant()) {
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().type != Float) {
        return false;
    }

    if (member3Copy.GetMemberTypeDescriptor().numberOfBits != 64) {
        return false;
    }

    if (StringHelper::Compare(member3Copy.GetMemberModifiers(), "[32]") != 0) {
        return false;
    }

    if (member3Copy.GetMemberSize() != sizeof(float64) * 32) {
        return false;
    }

    // the members are aligned !
    if (member3Copy.GetMemberByteOffset() != 2 * sizeof(float32*)) {
        return false;
    }

    return true;
#endif
    return ret;

}

bool IntrospectionTest::TestMacroToAddBasicInClassRegistryDatabase() {
	ErrorManagement::ErrorType ret;

	TestIntrospectionStructure tis;

	ret = CheckIntroType<TestIntrospectionStructure>("member4",tis.member4,indexof(TestIntrospectionStructure,member4));
	return ret;
#if 0

    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[3];
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member4") != 0) {
        return false;
    }

    if (!member4Copy.IsConstant()) {
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().type != BT_CCString) {
        return false;
    }

    if (member4Copy.GetMemberTypeDescriptor().numberOfBits != sizeof(const char8*) * 8) {
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "C") != 0) {
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(const char8*)) {
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64)) {
        return false;
    }
    return true;
#endif
}

bool IntrospectionTest::TestMacroToAddStructuredInClassRegistryDatabase() {
	ErrorManagement::ErrorType ret;

	TestIntrospectionStructure tis;

	ret = CheckIntroType<TestIntrospectionStructure>("member5",tis.member5,indexof(TestIntrospectionStructure,member5));
	return ret;
#if 0
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("TestIntrospectionStructure");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry member4Copy = (*introInfo->GetIntrospection())[4];
    if (StringHelper::Compare(member4Copy.GetMemberName(), "member5") != 0) {
        return false;
    }

    if (member4Copy.IsConstant()) {
        return false;
    }

    if (!member4Copy.GetMemberTypeDescriptor().isStructuredData) {
        return false;
    }

    if (StringHelper::Compare(member4Copy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (member4Copy.GetMemberSize() != sizeof(TestIntrospectionNestedStructure)) {
        return false;
    }

    // the members are aligned !
    if (member4Copy.GetMemberByteOffset() != 2 * sizeof(float32*) + 32 * sizeof(float64) + sizeof(const char8*)) {
        return false;
    }
    return true;
#endif
}

bool IntrospectionTest::TestGetNumberOfMembers() {

	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<TestIntrospectionStructure>();

	return (cri->NumberOfMembers()==5);
}

bool IntrospectionTest::TestGetClassSize() {
	ErrorManagement::ErrorType ret;
	ClassRegistryItem *cri = ClassRegistryItem::Instance<TestIntrospectionStructure>();

	return (cri->GetSizeOfClass() == sizeof(TestIntrospectionStructure));
}

bool IntrospectionTest::TestMacroToAddObjectClassRegistryDatabase() {
	ErrorManagement::ErrorType ret;

	DummyObject dobj;

	ret = CheckIntroType<DummyObject>("member",dobj.member,indexof(DummyObject,member));

	//the ability to dereference will be tested in the IntrospectionExtendedTest.h

	return ret;

#if 0
    ClassRegistryDatabase *instance = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *introInfo = instance->Find("DummyObject");
    if (introInfo == NULL) {
        return false;
    }

    IntrospectionEntry memberCopy = (*introInfo->GetIntrospection())[0];
    if (StringHelper::Compare(memberCopy.GetMemberName(), "member") != 0) {
        return false;
    }

    if (memberCopy.IsConstant()) {
        return false;
    }

    if (memberCopy.GetMemberTypeDescriptor().isStructuredData) {
        return false;
    }

    if (StringHelper::Compare(memberCopy.GetMemberModifiers(), "") != 0) {
        return false;
    }

    if (memberCopy.GetMemberSize() != sizeof(int32)) {
        return false;
    }

    DummyObject obj;
    char8* objBegin = (char8*) &obj;

    char8* memberPtrChar = objBegin + memberCopy.GetMemberByteOffset();

    int32* memberPtr = (int32*) memberPtrChar;

    *memberPtr = 2;

    return obj.member == 2;
#endif
}
