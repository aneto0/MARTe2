/**
 * @file TypeDescriptorTest.cpp
 * @brief Source file for class TypeDescriptorTest
 * @date 03/09/2015
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
 * the class TypeDescriptorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptorTest.h"
#include "DynamicCString.h"
#include "CString.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool TypeDescriptorTest::TestConstructorFullType(bool dataIsConstantVal,uint32 objectSizeVal,TD_FullType fulltypeVal) {

	uint8 dataIsConstantInt = 0;
	if (dataIsConstantVal) {dataIsConstantInt = 1;}

	TYPE_DESCRIPTOR_TYPE number = (TDRANGE(fullType,fulltypeVal) | TDRANGE(objectSize,objectSizeVal) | TDRANGE(dataIsConstant,dataIsConstantInt));

    TypeDescriptor testTD = TypeDescriptor(number);

    if (testTD.isStructuredData != false) {
        return false;
    }

    if (testTD.dataIsConstant != dataIsConstantVal) {
        return false;
    }

    if (testTD.objectSize != objectSizeVal) {
        return false;
    }

    if (testTD.fullType != fulltypeVal) {
        return false;
    }

    return true;
}

bool TypeDescriptorTest::TestConstructorBasicType(
						bool dataIsConstantVal,
        				TD_FullType basicTypeVal,
						TDBasicTypeSize basicTypeSizeVal,
						bool hasBitSizeVal,
						uint8 bitOffsetVal,
						uint8 numberOfBitsVal) {

	uint8 hasBitSizeInt = 0;
	if (hasBitSizeVal) {hasBitSizeInt = 1;}
	uint8 dataIsConstantInt = 0;
	if (dataIsConstantVal) {dataIsConstantInt = 1;}

    TypeDescriptor testTD = TypeDescriptor(TDRANGE(fullType,basicTypeVal) | TDRANGE(hasBitSize,hasBitSizeInt) | TDRANGE(basicTypeSize,basicTypeSizeVal) | TDRANGE(numberOfBits,numberOfBitsVal) | TDRANGE(bitOffset,bitOffsetVal) | TDRANGE(dataIsConstant,dataIsConstantInt));

    if (testTD.isStructuredData != false) {
        return false;
    }

    if (testTD.dataIsConstant != dataIsConstantVal) {
        return false;
    }

    if (testTD.basicTypeSize != basicTypeSizeVal) {
        return false;
    }

    if (testTD.numberOfBits != numberOfBitsVal) {
        return false;
    }

    if (testTD.bitOffset != bitOffsetVal) {
        return false;
    }

    if (testTD.fullType  != basicTypeVal) {
        return false;
    }

    return true;

}

bool TypeDescriptorTest::TestConstructorObject(bool dataIsConstant, uint32 structuredDataIdCodeVal) {

    TypeDescriptor testTD(dataIsConstant, structuredDataIdCodeVal);

    if (testTD.isStructuredData != true) {
        return false;
    }

    if (testTD.dataIsConstant != dataIsConstant) {
        return false;
    }

    if (testTD.structuredDataIdCode != structuredDataIdCodeVal) {
        return false;
    }

    return true;
}

bool TypeDescriptorTest::TestIsEqualOperator(const TypeDescriptor &t1,const TypeDescriptor &t2, bool isSame, bool isSameType, bool isSameTypeAndSize){

	bool success = true;

	success = success && (t1.SameAs(t2)^ isSame);
	success = success && (t1.SameTypeAs(t2)^ isSameType);
	success = success && (t1.SameTypeAndSizeAs(t2)^ isSameTypeAndSize);

    return success;
}

bool TypeDescriptorTest::TestFieldSaturation() {

    TypeDescriptor testTD1 = UnsignedBitSet_number(uint32,1,1);

    testTD1.numberOfBits = 256;
    testTD1.bitOffset = 256;
    testTD1.basicTypeSize = 16;
    testTD1.fullType = 64;

    if (testTD1.numberOfBits != 255) {
        return false;
    }

    if (testTD1.bitOffset != 255) {
        return false;
    }

    if (testTD1.basicTypeSize != 15) {
        return false;
    }

    if (testTD1.fullType != 63) {
        return false;
    }
}

static const TypeDescriptor typeDes[] = { DynamicCharString, ConstCharString(0), CharString, SignedInteger8Bit, SignedInteger16Bit, SignedInteger32Bit, SignedInteger64Bit, UnsignedInteger8Bit,
        UnsignedInteger16Bit, UnsignedInteger32Bit, UnsignedInteger64Bit, Float32Bit, Float64Bit, Character8Bit, VoidType, InvalidType(0) };
static const CCString typeNames[] = { "DynamicCString", "CCString", "CString", "int8", "int16", "int32", "int64", "uint8", "uint16", "uint32", "uint64", "float32", "float64", "char8", "void",emptyString};

bool TypeDescriptorTest::TestGetTypeDescriptorFromTypeName() {
    bool ret = true;
    uint32 i = 0u;
    while (typeNames[i] != emptyString) {

    	TypeDescriptor td = TypeDescriptor(typeNames[i]);
        if ( !td.SameAs(typeDes[i])) {
    		COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"Expecting ",typeNames[i]," to become :",typeDes[i].all, " instead :",td.all);
            ret = false;
        }
        i++;
    }
    return ret;
}

bool TypeDescriptorTest::TestGetTypeNameFromTypeDescriptor() {
    uint32 i = 0u;
    bool ret = true;
    while (typeNames[i] != emptyString) {
    	DynamicCString name;
    	CStringTool nameT = name();
    	typeDes[i].ToString(nameT);
    	if (!name.IsSameAs(typeNames[i])){
    		COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"Expecting :",typeNames[i]," obtained :",name);
            ret = false;
        }
        i++;
    }
    return ret;
}
