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

#define Test1ClassTemplate(subTemplate,subTemplateArr, className)\
	subTemplate(int8,int8Var,[],className)\
	subTemplate(int16,int16Var,[],className)\
	subTemplate(int32,int32Var,[],className)\
	subTemplate(int64,int64Var,[],className)\
	subTemplate(uint32,uint32Var,[],className)\
	subTemplate(char8,char8Var,[],className)\
	subTemplate(float,floatVar,[],className)\
	subTemplate(double,doubleVar,[],className)\
	subTemplateArr(int16,int16Arr,[12],className)\
	subTemplateArr(int64,int64Arr,[12][25],className)\
	subTemplate(int32 *,int32PVar,[],className)\
    subTemplate(CCString,CCStringVar,[],className)\
	subTemplate(CString,CStringVar,[],className)\
	subTemplate(DynamicCString,DCStringVar,[],className)\
    subTemplate(ZeroTerminatedArray<CCString>,CStringZTAVar,[],className)\
    subTemplate(ZeroTerminatedArray<Vector<CCString>[4]>,CStringVAZTAVar,[],className)\
	subTemplate(Matrix<float (*)[10]>,MFloat10,[],className)\

class Test1Class{
public:

	Test1ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test1Class)
} test1Class;

Test1ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test1Class)

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
	test1Class.int8Var = 8;
	test1Class.int16Var = 16;
	test1Class.int32Var = 32;
	test1Class.int64Var = 64;
	test1Class.uint32Var = 32;
	test1Class.char8Var = 'c';
	test1Class.floatVar = 0.1;
	test1Class.doubleVar = 1.1e9;
	test1Class.int32PVar = &test1Class.int32Var;
	test1Class.CCStringVar = data[0];
	test1Class.CStringVar  = dataBuffer;
	test1Class.DCStringVar = data[0];
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

void Check(AnyType at,CCString expression,CCString returnType ){
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
	    err.fatalError = !string.isSameAs(returnType.GetList());
	}

	if (err){
	    printf("OK  -->%s (%lli %lli)\n",string.GetList(),dataSize,storageSize);
	} else {
	    printf("NO ");
	    PrintError(err);
	    printf("(%s)%s= %s\n",string2.GetList(),string.GetList(),returnType.GetList());
	}
}

void Test(){

	AnyType at(test1Class);

    Check(at,".int8Var","int8");
    Check(at,".int16Var","int16");
    Check(at,".int32Var","int32");
    Check(at,".uint32Var","uint32");
    Check(at,".int64Var","int64");
    Check(at,".floatVar","float");
    Check(at,".doubleVar","double");
    Check(at,".int16Arr","int16[12]");
    Check(at,".int64Arr","int64[12][25]");
    Check(at,".int32PVar","int32 *");
    Check(at,".int32PVar*","int32");
    Check(at,".CCStringVar","CCString");
    Check(at,".CCStringVar[0]","const char8");
    Check(at,".CStringVar","CString");
    Check(at,".CStringVar[2]","char8");
    Check(at,".DCStringVar","DynamicCString");
    Check(at,".DCStringVar[3]","char8");
    Check(at,".CStringZTAVar","ZeroTerminatedArray<CCString>");
    Check(at,".CStringZTAVar[2]","CCString");
    Check(at,".CStringZTAVar[2][2]","const char8");
    Check(at,".CStringVAZTAVar","ZeroTerminatedArray<Vector<CCString>[4]>");
    Check(at,".CStringVAZTAVar[1]","Vector<CCString>[4]");
    Check(at,".CStringVAZTAVar[1][2]","Vector<CCString>");
    Check(at,".CStringVAZTAVar[1][2][0]","CCString");
    Check(at,".CStringVAZTAVar[1][2][0][4]","const char8");
    Check(at,".MFloat10","Matrix<float( *)[10]>");
}
}


int main(int argc, char **argv){
	MARTe::PrepareTestObject();
	MARTe::Test();
	return 0;
}
