/**
 * @file test.cpp
 * @brief Source file for class test
 * @date Nov 29, 2016
 * @author fsartori
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
 * the class test (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"
#include "VariableDescriptor.h"
#include "DynamicCString.h"
#include "StaticCString.h"
#include "ErrorType.h"
#include "MemoryCheck.h"
#include "TypeDescriptor.h"
#include "CLASSMEMBERREGISTER.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

#define varDeclSubTemplate(type,typeName,arrayDims, className)\
	type  typeName;

#define arrayDeclSubTemplate(type,typeName,arrayDims, className)\
	type  typeName arrayDims;


#define memberDeclSubTemplate(type,typeName,arrayDims,className)\
	CLASS_MEMBER_REGISTER(className,typeName)

typedef int64 (*myPArrType[12])[25] ;

#define Test2ClassTemplate(subTemplate,subTemplateArr, className)\
	subTemplate(int8,int8Var,[],className)\
	subTemplate(int16,int16Var,[],className)\
	subTemplate(int32,int32Var,[],className)\
	subTemplate(int64,int64Var,[],className)\
	subTemplate(uint32,uint32Var,[],className)\
	subTemplate(char8,char8Var,[],className)\
	subTemplate(float,floatVar,[],className)\
	subTemplate(double,doubleVar,[],className)

/// declare Test2Class
class Test2Class{
public:

	Test2ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test1Class)
} ;

/// register Test2Class
Test2ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test2Class)


#define Test1ClassTemplate(subTemplate,subTemplateArr, className)\
	subTemplate(Test2Class,test2,[],className)\
	subTemplateArr(int16,int16Arr,[12],className)\
	subTemplateArr(int64,int64Arr,[12][25],className)\
	subTemplate(myPArrType,int64PArr,[],className)\
	subTemplate(int32 *,int32PVar,[],className)\
    subTemplate(CCString,CCStringVar,[],className)\
	subTemplate(CString,CStringVar,[],className)\
	subTemplate(DynamicCString,DCStringVar,[],className)\
	subTemplate(Vector<char8>,VCharVar,[],className)\
	subTemplate(Matrix<float>,MFloatVar,[],className)\
    subTemplate(ZeroTerminatedArray<CCString>,CStringZTAVar,[],className)\
    subTemplate(ZeroTerminatedArray<Vector<CCString>[4]>,CStringVAZTAVar,[],className)\
	subTemplate(Matrix<float (*)[10]>,MFloat10,[],className)\

class Test1Class: public Test2Class{
public:

	Test1ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test1Class)
} test1Class;

Test1ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test1Class)
CLASS_INHERIT_REGISTER(Test1Class,Test2Class)

const char8* data[] = {
		"pippo",
		"pluto",
		"paperino",
		"minni",
		"paperon",
		"nonnapapera"
};
char8 dataBuffer[]="mizzega";

void PrepareTestObject(){
	test1Class.int8Var = 18;
	test1Class.int16Var = 116;
	test1Class.int32Var = 132;
	test1Class.int64Var = 164;
	test1Class.uint32Var = 132;
	test1Class.char8Var = 'c';
	test1Class.floatVar = 10.1;
	test1Class.doubleVar = 11.1e9;
	test1Class.test2.int8Var = 8;
	test1Class.test2.int16Var = 16;
	test1Class.test2.int32Var = 32;
	test1Class.test2.int64Var = 64;
	test1Class.test2.uint32Var = 32;
	test1Class.test2.char8Var = 'c';
	test1Class.test2.floatVar = 0.1;
	test1Class.test2.doubleVar = 1.1e9;
	test1Class.int32PVar = &test1Class.test2.int32Var;
	test1Class.CCStringVar = data[0];
	test1Class.CStringVar  = dataBuffer;
	test1Class.DCStringVar = data[0];

	static int64 arr1[25];
	test1Class.int64PArr[0]= &arr1;
	test1Class.int64PArr[4]= &arr1;
//	printf("{%s,%s,%s}\n",test1Class.CCStringVar.GetList(),test1Class.CStringVar.GetList(),test1Class.DCStringVar.GetList());
/*
	printf("{%p,%p,%p}\n",test1Class.CCStringVar.GetList(),test1Class.CStringVar.GetList(),test1Class.DCStringVar.GetList());
	printf("{%p,%p,%p}\n",&test1Class.CCStringVar,&test1Class.CStringVar,&test1Class.DCStringVar);
	printf("{%i,%i,%i}\n",MemoryCheck::Check(test1Class.CCStringVar.GetList()),
						  MemoryCheck::Check(test1Class.CStringVar.GetList()),
						  MemoryCheck::Check(test1Class.DCStringVar.GetList()));
	printf("{%i,%i,%i}\n",MemoryCheck::Check(&test1Class.CCStringVar),
						  MemoryCheck::Check(&test1Class.CStringVar),
						  MemoryCheck::Check(&test1Class.DCStringVar));
	printf("%s\n",test1Class.CCStringVar.GetList());
*/
	static CCString pippo[] = {
			CCString("uno"),
			CCString("duo"),
			CCString("tre"),
			CCString("quattro"),
			CCString("cinque"),
			emptyString
	};
	test1Class.CStringZTAVar = ZeroTerminatedArray<CCString>(pippo);

	static CCString a1[]= { CCString("ampio"),CCString("lume") };
	static CCString a2[]= { CCString("grande"),CCString("spazio"),CCString("aperto")  };
	static CCString a3[]= { CCString("agii")};
	static CCString a4[]= { CCString("roma"),CCString("firenze"),CCString("napoli"),CCString("milano")};
	static Vector<CCString> v1(a1);
	static Vector<CCString> v2(a2);
	static Vector<CCString> v3(a3);
	static Vector<CCString> v4(a4);
	static Vector<CCString> vv0[4] = {a1,a2,a3,a4};
	static Vector<CCString> vv1[4] = {a1,a2,a3,a4};
	static Vector<CCString> vv2[4] = {a1,a2,a3,a4};
	static Vector<CCString> vv3[4] = {a1,a2,a3,a4};
	static Vector<CCString> vv4[][4] = {{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{0,0,0,0}};
    test1Class.CStringVAZTAVar = ZeroTerminatedArray<Vector<CCString>[4]>(vv4);

    static float (*arrayP10[10][10])[10];
    for (int i=0;i<10;i++) for (int j=0;j<10;j++) arrayP10[i][j] = NULL;
    static float arrayF[10];
    arrayP10[1][1] = &arrayF;
    test1Class.MFloat10 = arrayP10;
}

ErrorManagement::ErrorType PrintError(ErrorManagement::ErrorType e){
	if (!e){
	    ErrorManagement::ErrorTypeLookup *etl = &ErrorManagement::errorTypeLookup[0];
	    while (!etl->name.IsNullPtr()){
	    	if ((etl->errorBitSet &  e.format_as_integer)!= 0){
	    		printf("(err=%s)",etl->name.GetList());
	    	}
	    	etl++;
	    }
	}

    return e;
}

void Check(AnyType at,CCString expression,CCString returnType,uint64 expectedDataSize,uint64 expectedStorageSize ){
	ErrorManagement::ErrorType err;

	printf ("% 28s ->",expression.GetList());

	err = at.MultipleDereference(expression);
    DynamicCString string;
    DynamicCString string2;
    uint64 dataSize;
    uint64 storageSize;
	if (err){
	    const VariableDescriptor &vd =  at.GetFullVariableDescriptor();
	    err = vd.ToString(string);
	    vd.ToString(string2,true);
		if (err){
		    err = vd.GetSize(reinterpret_cast<const uint8 *>(at.GetVariablePointer()),dataSize, &storageSize);
//			if (!err.ErrorsCleared()){
//				printf ("[%p]",at.GetVariablePointer());
//			}
		}
	}

	if (err){
		if (!string.isSameAs(returnType.GetList())){
			err.comparisonFailure = true;
			printf("NO (%s)%s != %s ",string2.GetList(),string.GetList(),returnType.GetList());
		}
		if ((dataSize != expectedDataSize) || (storageSize != expectedStorageSize)){
			err.comparisonFailure = true;
			printf("NO (%lli,%lli) != (%lli,%lli) ",dataSize,storageSize,expectedDataSize,expectedStorageSize);
		}
	}
	if (err){
	    err.comparisonFailure = ((dataSize != expectedDataSize) || (storageSize != expectedStorageSize));
	}

	if (err){
	    printf("OK  -->%s (%lli %lli)\n",string.GetList(),dataSize,storageSize);
	} else {
	    printf("NO ");
	    PrintError(err);
	    printf("\n");
	}
}

void Test(){

	AnyType at(test1Class);

    Check(at,".int8Var","int8",sizeof(int8),0);
    Check(at,".int16Var","int16",sizeof(int16),0);
    Check(at,".int32Var","int32",sizeof(int32),0);
    Check(at,".uint32Var","uint32",sizeof(uint32),0);
    Check(at,".int64Var","int64",sizeof(int64),0);
    Check(at,".floatVar","float",sizeof(float),0);
    Check(at,".doubleVar","double",sizeof(double),0);
    Check(at,".test2.int8Var","int8",sizeof(int8),0);
    Check(at,".test2.int16Var","int16",sizeof(int16),0);
    Check(at,".test2.int32Var","int32",sizeof(int32),0);
    Check(at,".test2.uint32Var","uint32",sizeof(uint32),0);
    Check(at,".test2.int64Var","int64",sizeof(int64),0);
    Check(at,".test2.floatVar","float",sizeof(float),0);
    Check(at,".test2.doubleVar","double",sizeof(double),0);
    Check(at,".int16Arr","int16[12]",sizeof(test1Class.int16Arr),0);
    Check(at,".int64Arr","int64[12][25]",sizeof(test1Class.int64Arr),0);
    Check(at,".int64PArr","int64( *[12])[25]",496,sizeof(test1Class.int64PArr));
    Check(at,".int32PVar","int32 *",12,8);
    Check(at,".int32PVar*","int32",4,0);
    Check(at,".CCStringVar","CCString",14,8);
    Check(at,".CCStringVar[0]","const char8",1,0);
    Check(at,".CStringVar","CString",16,8);
    Check(at,".CStringVar[2]","char8",1,0);
    Check(at,".DCStringVar","DynamicCString",14,8);
    Check(at,".DCStringVar[3]","char8",1,0);
    Check(at,".VCharVar","Vector<char8>",16,16);
    Check(at,".MFloatVar","Matrix<float>",24,24);
    Check(at,".CStringZTAVar","ZeroTerminatedArray<CCString>",83,56);
    Check(at,".CStringZTAVar[2]","CCString",12,8);
    Check(at,".CStringZTAVar[2][2]","const char8",1,0);
    Check(at,".CStringVAZTAVar","ZeroTerminatedArray<Vector<CCString>[4]>",968,712);
    Check(at,".CStringVAZTAVar[1]","Vector<CCString>[4]",208,144);
    Check(at,".CStringVAZTAVar[1][2]","Vector<CCString>",43,32);
    Check(at,".CStringVAZTAVar[1][2][0]","CCString",14,8);
    Check(at,".CStringVAZTAVar[1][2][0][4]","const char8",1,0);
    Check(at,".MFloat10","Matrix<float( *)[10]>",864,824);
}
}


int main(int argc, char **argv){
	MARTe::PrepareTestObject();
	MARTe::Test();
	return 0;
}
