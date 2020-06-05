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
#include <Private/AnyObjectT.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"

#include "BitRange.h"
#include "AnyType.h"
#include "VariableDescriptor.h"
#include "DynamicCString.h"
#include "VoidStream.h"
#include "SimpleStream.h"
#include "StaticCString.h"
#include "ErrorType.h"
#include "MemoryCheck.h"
#include "TypeDescriptor.h"
#include "CLASSMEMBERREGISTER.h"
#include "SaturatedInteger.h"
#include "ProgressiveTypeCreator.h"
#include "ErrorTypeLookup.h"


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

#define mypPAA1 3
#define mypPAA2 5
#define mypPAA3 7
#define mypPAA4 11
typedef int64 (*myPArrType[mypPAA4])[mypPAA3*mypPAA1] ;
typedef int16 (*myPArrType2[mypPAA1][mypPAA2])[mypPAA3][mypPAA4] ;
typedef int16 (*myPArrType0)[mypPAA3][mypPAA4] ;

typedef BitRange<uint32,1, 0> uint32_1_0 ;
typedef BitRange<uint32,2, 1> uint32_2_1 ;
typedef BitRange<uint32,3, 3> uint32_3_3 ;
typedef BitRange<uint32,4, 6> uint32_4_6 ;
typedef BitRange<uint32,5,10> uint32_5_10;
typedef BitRange<uint32,6,15> uint32_6_15;
typedef BitRange<uint32,7,21> uint32_7_21;
typedef BitRange<uint32,4,28> uint32_4_28;

typedef uint32 uint32_4[4];
typedef uint32 uint32_1[1];
typedef uint32 uint32_4_8[4][8];
typedef uint32 uint32_4_8_6[4][8][6];
typedef uint32 uint32_4_8_6_5[4][8][6][5];
typedef uint32 uint32_4_8_6_5_3[4][8][6][5][3];


#define Test3ClassTemplate(subTemplate,subTemplateArr, className)\
        subTemplate(int8,int8Var,[],className)\
        subTemplate(uint32_1_0 ,bitset1,[],className)\
        subTemplate(uint32_2_1 ,bitset2,[],className)\
        subTemplate(uint32_3_3 ,bitset3,[],className)\
        subTemplate(uint32_4_6 ,bitset4,[],className)\
        subTemplate(uint32_5_10,bitset5,[],className)\
        subTemplate(uint32_6_15,bitset6,[],className)\
        subTemplate(uint32_7_21,bitset7,[],className)\
        subTemplate(uint32_4_28,bitset8,[],className)\

/// declare Test2Class
union Test3Class{
    Test3ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test3Class)
} ;

/// register Test2Class
Test3ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test3Class)

#define Test2ClassTemplate(subTemplate,subTemplateArr, className)\
        subTemplate(int8,int8Var,[],className)\
        subTemplate(int16,int16Var,[],className)\
        subTemplate(int32,int32Var,[],className)\
        subTemplate(int64,int64Var,[],className)\
        subTemplate(uint32,uint32Var,[],className)\
        subTemplate(char8,char8Var,[],className)\
        subTemplate(float32,floatVar,[],className)\
        subTemplate(float64,float64Var,[],className)

/// declare Test2Class
class Test2Class{
public:

    Test2ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test2Class)
} ;

/// register Test2Class
Test2ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test2Class)

////
class MyStream: public VoidStream{
public:
private:
	int test;
	int test2;
};


#define Test1ClassTemplate(subTemplate,subTemplateArr, className)\
        subTemplate(Test3Class,test3,[],className)\
        subTemplate(Test2Class,test2,[],className)\
        subTemplateArr(int16,int16Arr,[12],className)\
        subTemplateArr(int64,int64Arr,[12][25],className)\
        subTemplate(myPArrType,int64PArr,[],className)\
        subTemplate(myPArrType2,int16AAPAA,[],className)\
        subTemplate(int32 *,int32PVar,[],className)\
        subTemplate(CCString,CCStringVar,[],className)\
        subTemplate(CString,CStringVar,[],className)\
        subTemplate(DynamicCString,DCStringVar,[],className)\
        subTemplate(Vector<char8>,VCharVar,[],className)\
        subTemplate(Matrix<float32>,MFloatVar,[],className)\
        subTemplate(ZeroTerminatedArray<CCString>,CStringZTAVar,[],className)\
        subTemplate(ZeroTerminatedArray<Vector<CCString>[4]>,CStringVAZTAVar,[],className)\
        subTemplate(Matrix<float32 (*)[10]>,MFloat10,[],className)\
        subTemplate(StreamI *,pStreamI,[],className)\
        subTemplate(SimpleStream,SString,[],className)\
        subTemplate(MyStream,myStream,[],className)\

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

static int16 int16AAPAA[mypPAA1][mypPAA2][mypPAA3][mypPAA4];
/*
static CCString pippo[] = {
        CCString("uno"),
        CCString("duo"),
        CCString("tre"),
        CCString("quattro"),
        CCString("cinque"),
        emptyString
};

static CCString a1[]= { CCString("ampio"),CCString("lume") };
static CCString a2[]= { CCString("grande"),CCString("spazio"),CCString("aperto")  };
static CCString a3[]= { CCString("agii")};
static CCString a4[]= { CCString("roma"),CCString("firenze"),CCString("napoli"),CCString("milano")};
static Vector<CCString> v1(a1);
static Vector<CCString> v2(a2);
static Vector<CCString> v3(a3);
static Vector<CCString> v4(a4);
static Vector<CCString> vv4[][4] = {{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{0,0,0,0}};
*/




/**
 * Forces construction of at when necessary and after all other initialisations
 */
class TestEnvironment{
public:
	/**
	 * singleton access
	 */
	static TestEnvironment &Instance(){
		static TestEnvironment te;
		return te;
	}

	/**
	 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size
	 * If clone is true, at(structure) is first cloned and then dereferenced
	 */
	ErrorManagement::ErrorType DereferenceAndCheckTypeAndSize(CCString expression, CCString typeCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true );
	/**
	 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size and content
	 * If clone is true, at(structure) is first cloned and then dereferenced
	 * NB: Content is checked after conversion to string
	*/
	ErrorManagement::ErrorType DereferenceAndCheckTypeSizeAndContent(CCString expression,CCString typeCheck,CCString contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true );
	/**
	 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size and content
	 * If clone is true, at(structure) is first cloned and then dereferenced
	 * NB: Content is checked to an Anytype content
	 */
	ErrorManagement::ErrorType DereferenceAndCheckTypeSizeAndContentAT(CCString expression,CCString typeCheck,AnyType contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone=true,bool expectFail=false );

	/**
	 * Creates an object of type T and dimensions [size1] [minSize2:size2] using 3 forms:
	 * T[size1][size2]
	 * T(*[size1])[size2]
	 * Vector<T>[size1]
	 * depending on variability (minSize2 != size2) and size (check pfstc.DefaultPageSize()) chooses one format
	 * and then generates a set of random values and, at the same time, fills the chosen object and creates
	 * an object using the ProgressiveTypeCreator.
	 * Finally it compares the type and the content of the chosen object and the dynamically created object.
	 *
	 */
	template <typename T, int size1, int size2,int minSize2,typename T2>
	ErrorManagement::ErrorType AutoGenObjectTAndCheckContent(TypeDescriptor td);

	/**
	 * Tests CopyTo/CompareWith function of AnyType: copies/compares multidimensional data and converts types
	 * data.CopyTo(dataCopy)
	 * Quasi-randomly loads data
	 * Copies to dataCopy.
	 * Checks types of data and dataCopy
	 * Checks content by using CompareWith function in reverse dataCopy.CompareWith(data)
	 */
	template <typename type1,typename type2>
	ErrorManagement::ErrorType CopyToAndCompareWith(bool expectSuccess, CCString type1S,CCString type2S);


    CCString testPatt[3];

	CCString pippo[6];

	ProgressiveTypeCreator pfstc;

	CCString a1[2];

	AnyType at;
private:
	/**
	 * Initialises all variables
	 */
	void PrepareTestObject();

	/**
	 * hidden constructor
	 */
	TestEnvironment():pfstc(1024),at(test1Class){
		MARTe::StartupManager::Initialise();
		PrepareTestObject();
	}
};


void TestEnvironment::PrepareTestObject(){

	pippo[0] = CCString("uno");
	pippo[1] = CCString("duo");
	pippo[2] = CCString("tre");
	pippo[3] = CCString("quattro");
	pippo[4] = CCString("cinque");
	pippo[5] = emptyString;
	a1[0] = CCString("ampio");
	a1[1] = CCString("lume");

	static CCString a1[]= { CCString("ampio"),CCString("lume") };
	static CCString a2[]= { CCString("grande"),CCString("spazio"),CCString("aperto")  };
	static CCString a3[]= { CCString("agii")};
	static CCString a4[]= { CCString("roma"),CCString("firenze"),CCString("napoli"),CCString("milano")};
	static Vector<CCString> v1(a1);
	static Vector<CCString> v2(a2);
	static Vector<CCString> v3(a3);
	static Vector<CCString> v4(a4);
	static Vector<CCString> vv4[][4] = {{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{0,0,0,0}};


    test1Class.int8Var = 18;
    test1Class.int16Var = 116;
    test1Class.int32Var = 132;
    test1Class.int64Var = 164;
    test1Class.uint32Var = 132;
    test1Class.char8Var = 'c';
    test1Class.floatVar = 10.1F;
    test1Class.float64Var = 11.1e9;
    test1Class.test2.int8Var = 8;
    test1Class.test2.int16Var = 16;
    test1Class.test2.int32Var = 32;
    test1Class.test2.int64Var = 64;
    test1Class.test2.uint32Var = 32;
    test1Class.test2.char8Var = 'c';
    test1Class.test2.floatVar = 0.1F;
    test1Class.test2.float64Var = 1.1e9;
    test1Class.test3.bitset1 = 1;
    test1Class.test3.bitset2 = 2;
    test1Class.test3.bitset3 = 3;
    test1Class.test3.bitset4 = 4;
    test1Class.test3.bitset5 = 5;
    test1Class.test3.bitset6 = 6;
    test1Class.test3.bitset7 = 7;
    test1Class.test3.bitset8 = 8;

    for (int16 i=0;i<12;i++) {
    	test1Class.int16Arr[i] = 16*i;
    }

    test1Class.int32PVar = &test1Class.test2.int32Var;
    test1Class.CCStringVar = data[0];
    test1Class.CStringVar  = dataBuffer;
    test1Class.DCStringVar = data[2];

    static int64 arr1[mypPAA3*mypPAA1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    for (int i=0;i<mypPAA4;i++) test1Class.int64PArr[i] = NULL;
    test1Class.int64PArr[0]= &arr1;
    test1Class.int64PArr[4]= &arr1;

//    static int16 (*int16AAPAAp)[mypPAA1*mypPAA2][mypPAA1*mypPAA2] ;

    {
    	uint32 i;
    	int16 *pdummy = &int16AAPAA[0][0][0][0];
    	for (i=0;i<mypPAA1*mypPAA2*mypPAA1*mypPAA2;i++) {
    		pdummy[i] = static_cast<int16>(rand());
    	}
    	myPArrType0 *p =  &test1Class.int16AAPAA[0][0];
    	for (i=0;i<mypPAA1*mypPAA2;i++) {
    		p[i] = (myPArrType0 )&int16AAPAA[i/mypPAA2][i%mypPAA2][0][0];
    	}
    }

    test1Class.CStringZTAVar = ZeroTerminatedArray<CCString>(pippo);
    test1Class.CStringVAZTAVar = ZeroTerminatedArray<Vector<CCString>[4]>(vv4);

    static float32 (*arrayP10[10][10])[10];
    for (int i=0;i<10;i++) for (int j=0;j<10;j++) {
    	arrayP10[i][j] = NULL;
    }

    static float32 arrayF[10];
    arrayP10[1][1] = &arrayF;
    test1Class.MFloat10 = arrayP10;

    test1Class.SString = CCString("succhiarriello");
    test1Class.pStreamI = &test1Class.SString;
    test1Class.SString.Seek(8);
    //TODO implement Seek

    testPatt[0] = CCString("grande");
    testPatt[1] = CCString("spazio");
    testPatt[2] = CCString("aperto");

}


/*
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
}*/

ErrorManagement::ErrorType CheckType(AnyType at,CCString typeCheck,CStringTool message){
    ErrorManagement::ErrorType err;

    DynamicCString string;
    DynamicCString string2;
    if (err){
        const VariableDescriptor &vd =  at.GetFullVariableDescriptor();
        err = vd.ToString(string);
        vd.ToString(string2,true);
        if (!err){
            message.Append("ToString error");
        }
    }

    if (err){
        if (!string.IsSameAs(typeCheck.GetList())){
            err.comparisonFailure = true;
            message.Append('(').Append(string2).Append(')').Append(string).Append("!=").Append(typeCheck);
        }
    }
    return err;
}

#if 0
static void PrintType(AnyType at,DynamicCString &message ){
    ErrorManagement::ErrorType err;
    DynamicCString string;

    err = at.ToString(string);
	printf("%s",string);
}
#endif

static ErrorManagement::ErrorType CompareType(AnyType at1,AnyType at2,CStringTool &message ){
    ErrorManagement::ErrorType err;

    DynamicCString string1;
    DynamicCString string2;
    const VariableDescriptor &vd1 =  at1.GetFullVariableDescriptor();
    const VariableDescriptor &vd2 =  at2.GetFullVariableDescriptor();

    err = vd1.ToString(string1);
    if (!err){
		REPORT_ERROR(err,"at1 Variable Descriptor to string conversion error");
    }
    if (err){
        err = vd2.ToString(string2);
        if (!err){
			REPORT_ERROR(err,"at2 Variable Descriptor to string conversion error");
        }
    }

    if (err){
        if (!string1.IsSameAs(string2.GetList())){
            err.comparisonFailure = true;
			COMPOSITE_REPORT_ERROR(err,string1.GetList(),'{',vd1.GetModifiers(),"} != ",string2.GetList(),'{',vd2.GetModifiers(),'}');
        }
    }

    return err;
}

static ErrorManagement::ErrorType CheckSize(AnyType at,uint64 dataSizeCheck,uint64 storageSizeCheck, CStringTool &message ){
    ErrorManagement::ErrorType ok;

    uint64 dataSize;
    uint64 storageSize;

    if (ok){
        const VariableDescriptor &vd =  at.GetFullVariableDescriptor();
        ok = vd.GetSize(reinterpret_cast<const uint8 *>(at.GetVariablePointer()),dataSize, &storageSize);
    	REPORT_ERROR(ok,"GetSize error");
    }

    if (ok){
        if ((dataSize != dataSizeCheck) || (storageSize != storageSizeCheck)){
        	ok.comparisonFailure = true;
    		COMPOSITE_REPORT_ERROR(ok,'(',dataSize,',',storageSize,") != (",dataSizeCheck,',',storageSizeCheck,')');
        }
    }

    return ok;
}

static ErrorManagement::ErrorType CheckContent(AnyType at,CCString contentCheck){
    ErrorManagement::ErrorType ok;

	DynamicCString destinationString;
   	ok=at.CopyTo(destinationString);
   	if (!ok){
		REPORT_ERROR(ok,"CopyTo error");
   	}

	ok.comparisonFailure = !destinationString.IsSameAs(contentCheck.GetList());
	COMPOSITE_REPORT_ERROR(ok,"Values (",destinationString.GetList(),"!=",contentCheck.GetList());
    return ok;
}



/**
 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size
 * If clone is true, at(structure) is first cloned and then dereferenced
 */
ErrorManagement::ErrorType TestEnvironment::DereferenceAndCheckTypeAndSize(CCString expression, CCString typeCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone){

	ErrorManagement::ErrorType ok;
	DynamicCString message;
	CStringTool messageT = message();

	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");

	// creates a clone
	Reference atc;
	if (ok && clone){
		atc.FromAnyType(at);

		if (atc.IsValid()){
			atc.ToAnyType(at);
		} else {
			ok.fatalError = true;
			REPORT_ERROR(ok,"Cloning of variable failed");
		}
	}

    if (ok){
        for (uint32 ix= expression.GetSize();ix<28;ix++) {
        	messageT.Append(' ');
        }
        if (clone){
        	messageT.Append(expression.GetList()).Append("-C>");
        } else {
        	messageT.Append(expression.GetList()).Append(" ->");
        }

        ErrorManagement::ErrorType ok2;
        ok2 = CheckType(at,typeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckType error");

    	ok.SetError(ok2);

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckSize error");

    	ok.SetError(ok2);
    }


    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
    	messageT.Append("*NO*  -->See error Log ");
    } else {
    	messageT.Append("OK ").Append(typeCheck).Append(",sz=").Append(dataSizeCheck).Append(",stor=").Append(storageSizeCheck);
    }
    messageT.Append("\n");

    // writes on the error stream the test result
    REPORT(ErrorManagement::Information,message.GetList());

	return ok;
}


/**
 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size and content
 * If clone is true, at(structure) is first cloned and then dereferenced
 * NB: Content is checked after conversion to string
 */
ErrorManagement::ErrorType TestEnvironment::DereferenceAndCheckTypeSizeAndContent(CCString expression,CCString typeCheck,CCString contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone){
    ErrorManagement::ErrorType ok;
	DynamicCString message;
	CStringTool messageT = message();

	for (uint32 ix= expression.GetSize();ix<28;ix++) {
		messageT.Append(' ');
	}
	messageT.Append(expression.GetList());

	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");
	Reference atc;
	if (ok && clone){
		atc.FromAnyType(at);
		if (atc.IsValid()){
			atc.ToAnyType(at);
		} else {
			ok.fatalError = true;
			REPORT_ERROR(ok,"Cloning of variable failed");
		}
	}
	if (ok){
        if (clone){
        	messageT.Append("-C>");
        } else {
        	messageT.Append(" ->");
        }

	    ErrorManagement::ErrorType ok2;
	    ok2 = CheckType(at,typeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckType error");
    	ok.SetError(ok2);
    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckSize error");
    	ok.SetError(ok2);
    	ok2 = CheckContent(at,contentCheck);
    	REPORT_ERROR(ok2,"CheckContent error");
    	ok.SetError(ok2);
	}
    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
		messageT.Append("*NO*  -->See error Log ");
    } else {
    	messageT.Append("OK (").Append(typeCheck).Append(')').Append(contentCheck).Append(",sz=").Append(dataSizeCheck).Append(",stor=").Append(storageSizeCheck);
    }
    messageT.Append("\n");

    // writes on the error stream the test result
    REPORT(ErrorManagement::Information,message.GetList());

	return ok;
}

/**
 * Starts from at(structure), then applies dereferencing to it and then compares the type and the size and content
 * If clone is true, at(structure) is first cloned and then dereferenced
 * NB: Content is checked to an Anytype content
 */
ErrorManagement::ErrorType TestEnvironment::DereferenceAndCheckTypeSizeAndContentAT(CCString expression,CCString typeCheck,AnyType contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone,bool expectFail){
	ErrorManagement::ErrorType ok;
	DynamicCString message;
	CStringTool messageT = message();

	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");

	Reference atc;
	if (ok && clone){
		atc.FromAnyType(at);

		if (atc.IsValid()){
			atc.ToAnyType(at);
		} else {
			ok.fatalError = true;
			REPORT_ERROR(ok,"Cloning of variable failed");
		}
	}

	if (ok){
		for (uint32 ix= expression.GetSize();ix<28;ix++) {
			messageT.Append(' ');
		}
        if (clone){
        	messageT.Append(expression).Append("-C>");
        } else {
        	messageT.Append(expression).Append(" ->");
        }

	    ErrorManagement::ErrorType ok2;

	    ok2 = CheckType(at,typeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckType error");

    	ok.SetError(ok2);

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck,messageT);
    	REPORT_ERROR(ok2,"CheckSize error");

    	ok.SetError(ok2);

    	ok2=at.CompareWith(contentCheck);
    	REPORT_ERROR(ok2,"CompareWith error");

    	ok.SetError(ok2);
    }


    if (!ok){
    	DynamicCString resultS;
		at.ToString(resultS);
    	DynamicCString referenceS;
    	contentCheck.ToString(referenceS);
    	COMPOSITE_REPORT_ERROR(ok,referenceS.GetList()," != ","clone:",resultS.GetList());

		if (expectFail){
			messageT.Append("OK! Failed as expected - see log");
		} else {
			messageT.Append("*NO*  -->See error Log ");
		}
    } else {
		if (expectFail){
			messageT.Append("*NO*!  -->Should have failed ");
		} else {
	    	messageT.Append("OK (").Append(typeCheck).Append("){content},sz=").Append(dataSizeCheck).Append(",stor=").Append(storageSizeCheck);
		}
    }
    messageT.Append("\n");

    // writes on the error stream the test result
    REPORT(ErrorManagement::Information,message.GetList());

	return ok;
}


template<typename T1>
 static inline  void NumSet(T1 &dest, uint64 &source) {
		SafeNumber2Number(source,dest);
		SafeNumber2Number(dest,source);
 }

//static inline void NumSet<DynamicCString>(DynamicCString &dest,uint32 &source) {
static inline void NumSet(DynamicCString &dest,uint64 &source) {
		dest().Truncate(0);
		dest().Append(source);
 }

/**
 * Creates an object of type T and dimensions [size1] [minSize2:size2] using 3 forms:
 * T[size1][size2]
 * T(*[size1])[size2]
 * Vector<T>[size1]
 * depending on variability (minSize2 != size2) and size (check pfstc.DefaultPageSize()) chooses one format
 * and then generates a set of random values and, at the same time, fills the chosen object and creates
 * an object using the ProgressiveTypeCreator.
 * Finally it compares the type and the content of the chosen object and the dynamically created object.
 *
 */
template <typename T, int size1, int size2,int minSize2,typename T2>
ErrorManagement::ErrorType TestEnvironment::AutoGenObjectTAndCheckContent(TypeDescriptor td){
	ErrorManagement::ErrorType ret;
	DynamicCString message;
	CStringTool messageT = message();

	// remapped types for type comparison - in case T is for storing only
	typedef  T (TA)[size1][size2];
//	typedef  T (*(TB)[size1])[size2];
//	typedef  Vector<T>(TC)[size1];
	typedef  T2 (TAu)[size1][size2];
	typedef  T2 (*(TBu)[size1])[size2];
	typedef  Vector<T2>(TCu)[size1];

	// Data structures to store comparison values
	TA *targetAP = (TA*)new T[size1*size2];
	TA& targetA(*targetAP);
	//	T targetA[size1][size2];
	T (*targetB[size1])[size2];
	Vector<T>targetC[size1];
	{
		for (int i=0;i<size1;i++){
			targetB[i] = reinterpret_cast<T (*)[size2]>(new T[size2]);
		}
	}
	TAu *targetAu = reinterpret_cast<TAu *>(&targetA);
	TBu *targetBu = reinterpret_cast<TBu *>(&targetB);
	TCu *targetCu = reinterpret_cast<TCu *>(&targetC);

	AnyType at;
	if (minSize2 != size2){
		at = AnyType(*targetCu);
	} else {
		if (sizeof (targetA) > pfstc.DefaultPageSize()){
			at = AnyType(*targetBu);
		} else {
			at = AnyType(*targetAu);
		}
	}

	DynamicCString atName;
	at.ToString(atName);

    for (uint32 ix= atName.GetSize();ix<28;ix++) {
    	messageT.Append(' ');
    }
    messageT.Append(atName).Append(" ->");

	ret = pfstc.Start(td);
	REPORT_ERROR(ret,"pfstc.Start failed");

	if (ret){
		for (int j=0;(j<size1) && ret ;j++){

			int actualSize2 = size2;
			if (minSize2 < size2){
				actualSize2 = rand();
				actualSize2 = actualSize2 % (size2-minSize2+1);
				actualSize2 = actualSize2 + minSize2;
			}

			for (int i=0;(i<actualSize2) && ret ;i++){
				uint64 kk = static_cast<uint64>(rand()*rand());
				T k;
				NumSet(k,kk);
				(*targetB[j])[i]=k;
				targetA[j][i] = k;
				char8 buffer[32];
				sprintf(buffer,"%lli",kk);
				ret = pfstc.AddElement(buffer);
				COMPOSITE_REPORT_ERROR(ret,"pfstc.AddElement(",buffer,")");
			}
			targetC[j].InitVector(reinterpret_cast<T *>(&targetA[j]),static_cast<uint32>(actualSize2));

			if (ret){
				ret = pfstc.EndVector();
				REPORT_ERROR(ret,"pfstc.EndVector failed");
			}
		}
	}

	Reference aoi;
	if (ret){
		ret = pfstc.End();
		REPORT_ERROR(ret,"pfstc.End failed");
	}

	DynamicCString message2;
	CStringTool message2T=message2();
	if (ret){
		ret = pfstc.GetReference(aoi);
		ret.fatalError = !aoi.IsValid();
		REPORT_ERROR(ret,"pfstc.GetReference failed");

		if (ret){
			message2T.Append(aoi->GetClassRegistryItem()->GetClassName()).Append("{");
		}
	}

	if (ret){
		AnyType x;
		aoi.ToAnyType(x);
		ret = CompareType(x,at,messageT);
		if (ret){
			x.ToString(message2T);
		}
		message2T.Append("}");
	}
	//ok

	if (ret){
		AnyType x;
		aoi.ToAnyType(x);
	    ret = x.CompareWith(at);
		REPORT_ERROR(ret,"Compare content fault");
	}

	if (!ret){
		DynamicCString string;
		string().Append("Failed ");
		at.ToString(string);
		REPORT_ERROR(ret, string.GetList());
		messageT.Append("*NO* Random Creation/CopyTo Compare Failed (see log) \n");
	} else {
		messageT.Append("OK ").Append(message2).Append(" :Random Creation/CopyTo Compare \n");
	}


	{
		for (int i=0;i<size1;i++){
			delete[] (targetB[i]);
		}
		delete[] targetAP;
	}

	pfstc.Clean();

    // writes on the error stream the test result
    REPORT(ErrorManagement::Information,message.GetList());

	return ret;
}

#if 0
template <typename T1,typename T2>
void TestSafeN2N(T1 value)
{
    T1 in = value;
    T2 out;
    DynamicCString ss;
//    StreamString ss;
    bool ret = SafeNumber2Number(in,out);
    float64 tst = out;
    if (!ret) printf("sat   "); else printf("no sat ");
    ss().Append()
    ss.Printf("%!(%?) --> %!(%?) !! %!(%?) \n",in,in,out,out,tst,tst);
    printf("%s",ss.Buffer());
}

template <typename T1>
void TestSatInteger(int64 mul1,int64 mul2, int64 sum1, int64 sub1,SaturatedInteger<T1> expected)
{
	SaturatedInteger<T1> x1(mul1);
	SaturatedInteger<T1> x2(mul2);
	SaturatedInteger<T1> x3(sum1);
	SaturatedInteger<T1> x4(sub1);
	SaturatedInteger<T1> x5 = (x1*x2)+x3 - x4;
    StreamString ss;

    T1 x;
	ss.Printf("(%?)((%f*%f)+%f-%f) == ",x,mul1,mul2, sum1, sub1);

	if (!x5.IsValid()){
//	if (x5.notANormalNumber()){
		ss.Printf("%s",CCString("saturated"));
	} else {
		ss.Printf("%i",x5.GetData());
	}

	if (x5 == expected){
	    printf("OK: ");
		ss.Printf("%s",CCString(" == "));
	} else {
	    printf("NO! ");
		ss.Printf("%s",CCString(" != "));
	}

	if (!expected.IsValid()){
//	if (expected.notANormalNumber()){
		ss.Printf("%s\n",CCString("saturated"));
	} else {
		ss.Printf("%i\n",expected.GetData());
	}
    printf("%s",ss.Buffer());

}
#endif



/**
 * Tests CopyTo/CompareWith function of AnyType: copies/compares multidimensional data and converts types
 * data.CopyTo(dataCopy)
 * Quasi-randomly loads data
 * Copies to dataCopy.
 * Checks types of data and dataCopy
 * Checks content by using CompareWith function in reverse dataCopy.CompareWith(data)
 */
template <typename type1,typename type2>
ErrorManagement::ErrorType TestEnvironment::CopyToAndCompareWith(bool expectSuccess, CCString type1S,CCString type2S){
	ErrorManagement::ErrorType ret;
	DynamicCString message;
	CStringTool messageT = message();

	type1 data;
	type2 dataCopy;

	{
		uint8 *p = reinterpret_cast<uint8 *> (&data);
		for (uint32 i = 0; i< sizeof(data);i++){
			p[i] = static_cast<uint8>(i*i+1);
		}
	}

	AnyType dataAT(data);
	AnyType dataCopyAT(dataCopy);
	ret = dataAT.CopyTo(dataCopy);
	REPORT_ERROR(ret,"CopyTo failed");

	DynamicCString dataCS,dataCopyCS;
    dataAT.ToString(dataCS);
    dataCopyAT.ToString(dataCopyCS);

    if (ret){
    	ret = dataCopyAT.CompareWith(dataAT);
    	REPORT_ERROR(ret,"Compare failed");
    }

	if (!ret.ErrorsCleared()){
		if (expectSuccess){
			messageT.Append(" NO = FAILED ! see log");
		} else {
			messageT.Append(" OK = FAILED as expected");
		}
	} else {
		if (expectSuccess){
			messageT.Append(" OK");
		} else {
			messageT.Append(" NO = Sucess but should have FAILED - see LOG");
		}
	}

	messageT.Append(" (").Append(dataCS).Append("x copy to (").Append(dataCopyCS).Append(")y and compare y to x:");
    messageT.Append("\n");

    // writes on the error stream the test result
    REPORT(ErrorManagement::Information,message.GetList());

    return ret;
}

#define MEM_SAVE()
#define MEM_CHECK()
//#define MEM_SAVE()  { long long saveCount = allocCount; printf("<<%lli>",saveCount);
//#define MEM_CHECK()  ErrorManagement::ErrorType ok2; 	ok2.notCompleted = (saveCount != allocCount); REPORT_ERROR(ok2,"!!!!!!MEM_BALANCE Failed\n"); printf("<%lli>>",allocCount);}


#define DEREF_CHECK(te,mod,type,size,store,name)\
TEST(IntrospectionGTest, DEREF_CHECK ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeAndSize(mod,type,size,store,false);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****DEREF_CHECK("  mod  ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define DEREF_CLONE_CHECK(te,mod,type,size,store,name)\
TEST(IntrospectionGTest, DEREF_CLONE_CHECK ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeAndSize(mod,type,size,store,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****DEREF_CLONE_CHECK("  mod  ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define DEREF_CHECKCONTENT(te,mod,type,val,size,store,name)\
TEST(IntrospectionGTest, DEREF_CHECKCONTENT ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeSizeAndContent(mod,type,val,size,store,false);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****DEREF_CHECKCONTENT(" mod ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define DEREF_CLONE_CHECKCONTENT(te,mod,type,val,size,store,name)\
TEST(IntrospectionGTest, DEREF_CLONE_CHECKCONTENT ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeSizeAndContent(mod,type,val,size,store,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****DEREF_CLONE_CHECKCONTENT(" mod ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define DEREF_CLONE_CHECKCONTENT_ANYTYPE(te,mod,type,val,size,store,clone,name)\
TEST(IntrospectionGTest, DEREF_CLONE_CHECKCONTENT_ANYTYPE ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeSizeAndContentAT(mod,type,val,size,store,clone);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****DEREF_CLONE_CHECKCONTENT_ANYTYPE(" mod ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define DEREF_CLONE_CHECKCONTENT_ANYTYPE_FAIL(te,mod,type,val,size,store,clone,name)\
TEST(IntrospectionGTest, DEREF_CLONE_CHECKCONTENT_ANYTYPE_FAIL ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.DereferenceAndCheckTypeSizeAndContentAT(mod,type,val,size,store,clone,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"......DEREF_CLONE_CHECKCONTENT_ANYTYPE_FAIL(" mod ")Failed as expected\n");\
ASSERT_TRUE(ok);\
}

#define RANDOM_2D_CONVERT_CHECK(te,type1, size1, size2,minSize2,type2,typeId)\
TEST(IntrospectionGTest, RANDOM_2D_CONVERT_CHECK ## type1 ## _ ## size1 ## _ ## size2 ## type2) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.AutoGenObjectTAndCheckContent<type1,size1,size2,minSize2,type2> (typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****RANDOM_2D_CONVERT_CHECK<>(" #type1 ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define RANDOM_2D_COPY_CHECK(te,type1, size1, size2,typeId)\
TEST(IntrospectionGTest, RANDOM_2D_COPY_CHECK ## type1 ## _ ## size1 ## _ ## size2) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.AutoGenObjectTAndCheckContent<type1,size1,size2,size2,type1> (typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****RANDOM_2D_COPY_CHECK<>(" #type1 ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define RANDOM_2D_COPY_CHECKR(te,type1, size1, size2,minSize2,typeId)\
TEST(IntrospectionGTest, RANDOM_2D_COPY_CHECKR ## type1 ## _ ## size1 ## _ ## size2 ## _ ## minSize2) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.AutoGenObjectTAndCheckContent<type1,size1,size2,minSize2,type1> (typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****RANDOM_2D_COPY_CHECKR<>(" #type1 ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define COPY_CHECK_OK(te,type1, type2,name)\
TEST(IntrospectionGTest, COPY_CHECK_OK ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.CopyToAndCompareWith<type1,type2> (true,#type1 , #type2);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****COPY_CHECK_OK<>(" #type1 "," #type2 ")Failed\n");\
ASSERT_TRUE(ok);\
}

#define COPY_CHECK_NOK(te,type1, type2,name)\
TEST(IntrospectionGTest, COPY_CHECK_NOK ## name) {\
ErrorManagement::ErrorType ok;\
MEM_SAVE()\
ok = te.CopyToAndCompareWith<type1,type2> (false,#type1 , #type2);\
MEM_CHECK()\
REPORT_ERROR(ok,"......COPY_CHECK_NOK<>(" #type1 "," #type2 ")Failed as expected\n");\
ASSERT_TRUE(ok);\
}

/*
void xx(){
    ErrorManagement::ErrorType ok;
	ProgressiveTypeCreator pfstc(1024);
	RANDOM_2D_COPY_CHECK(int8,1,1,1);
}
*/

/*
 * starts with test1Class introspectable anyType
 * then dereference on the base of the first string
 * then clones the variable
 * then checks the resulting type against the second string
 * then converts value to string and checks against third string
 * then checks data size against fourth string
 * then checks overhead storage size against 5th string
 */
/// note all these tests operate on simple 1D variables after redirection.
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int8Var","int8","18",sizeof(int8),0,int8Var); // MC
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".char8Var","char8","c",sizeof(char8),0,char8Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int16Var","int16","116",sizeof(int16),0,int16Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int32Var","int32","132",sizeof(int32),0,int32Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".uint32Var","uint32","132",sizeof(uint32),0,uint32Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int64Var","int64","164",sizeof(int64),0,int64Var);  // MC
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".floatVar","float32","10.100001",sizeof(float32),0,floatVar);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".float64Var","float64","11100000000.000000",sizeof(float64),0,float64Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.char8Var","char8","c",sizeof(char8),0,test2_char8Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.int8Var","int8","8",sizeof(int8),0,test2_int8Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.int16Var","int16","16",sizeof(int16),0,test2_int16Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.int32Var","int32","32",sizeof(int32),0,test2_int32Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.uint32Var","uint32","32",sizeof(uint32),0,test2_uint32Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.int64Var","int64","64",sizeof(int64),0,test2_int64Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.floatVar","float32","0.100000",sizeof(float32),0,test2_floatVar);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test2.float64Var","float64","1100000000.000000",sizeof(float64),0,test2_float64Var);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset1","BitRange<uint32,1,0>","1",sizeof(uint32),0,bitset1);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset2","BitRange<uint32,2,1>","2",sizeof(uint32),0,bitset2);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset3","BitRange<uint32,3,3>","3",sizeof(uint32),0,bitset3);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset4","BitRange<uint32,4,6>","4",sizeof(uint32),0,bitset4);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset5","BitRange<uint32,5,10>","5",sizeof(uint32),0,bitset5);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset6","BitRange<uint32,6,15>","6",sizeof(uint32),0,bitset6);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset7","BitRange<uint32,7,21>","7",sizeof(uint32),0,bitset7);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".test3.bitset8","BitRange<uint32,4,28>","8",sizeof(uint32),0,bitset8);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int16Arr[11]","int16","176",sizeof(test1Class.int16Arr[11]),0,int16Arr);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".int64PArr[4][5]","int64","6",8,0,int64PArr);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CCStringVar","CCString","pippo",14,8,CCStringVar);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CCStringVar[0]","const char8","p",1,0,CCStringVar_0);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringVar","CString","mizzega",16,8,CStringVar);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringVar[2]","char8","z",1,0,CStringVar_2);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".DCStringVar","DynamicCString","paperino",17,8,DCStringVar);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".DCStringVar[3]","char8","e",1,0,DCStringVar_3);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringZTAVar[0]","CCString","uno",12,8,CStringZTAVar_0);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringZTAVar[2]","CCString","tre",12,8,CStringZTAVar_2);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringZTAVar[2][2]","const char8","e",1,0,CStringZTAVar_2_2);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringVAZTAVar[1][2][0]","CCString","agii",13,8,CStringVAZTAVar_1_2_0);
DEREF_CLONE_CHECKCONTENT(TestEnvironment::Instance(),".CStringVAZTAVar[1][2][0][3]","const char8","i",1,0,CStringVAZTAVar_1_2_0_3);

/*
 * starts with test1Class introspectable anyType
 * then dereference on the base of the first string
 * then checks the resulting type against the second string
 * then converts value to string and checks against third string
 * then checks data size against fourth string
 * then checks overhead storage size against 5th string
 */
//DEREF_CHECKCONTENT(TestEnvironment::Instance(),".pStreamI*","StreamI","riello",22,8);
//test1Class.pStreamI->Seek(3);
//DEREF_CHECKCONTENT(TestEnvironment::Instance(),".pStreamI*","StreamI","chiarriello",22,8);
DEREF_CHECKCONTENT(TestEnvironment::Instance(),".SString","StreamString","succhiarriello",78,64,SString);


/// these tests operate on more complex variables
/*
 * starts with test1Class introspectable anyType
 * then dereference on the base of the first string
 * then checks the resulting type against the second string
 * then checks data size against fourth string
 * then checks overhead storage size against 5th string
 */
DEREF_CHECK(TestEnvironment::Instance(),".int64PArr","int64( *[11])[21]",424,sizeof(test1Class.int64PArr),int64PArr);/* cannot clone pointers*/
DEREF_CHECK(TestEnvironment::Instance(),".int64PArr[4]","int64( *)[21]",176,8,int64PArr_4); // cannot clone pointers
DEREF_CHECK(TestEnvironment::Instance(),".int32PVar","int32 *",8,0,int32PVar);  // cannot clone pointers
DEREF_CHECK(TestEnvironment::Instance(),".VCharVar","Vector<char8>",16,16,VCharVar);/* clone creates only const structures*/
DEREF_CHECK(TestEnvironment::Instance(),".MFloatVar","Matrix<float32>",24,24,MFloatVar);/* clone creates only const structures*/
DEREF_CHECK(TestEnvironment::Instance(),".CStringZTAVar","ZeroTerminatedArray<CCString>",83,56/* clone converts ZTA to vector */,CStringZTAVar);
DEREF_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar","ZeroTerminatedArray<Vector<CCString>[4]>",968,712/* cloning changes type*/,CStringVAZTAVar);
DEREF_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar[1]","Vector<CCString>[4]",208,144,CStringVAZTAVar_1);
DEREF_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar[1][2]","Vector<CCString>",29,24,CStringVAZTAVar_1_2);
DEREF_CHECK(TestEnvironment::Instance(),".MFloat10","Matrix<float32( *)[10]>",864,824,MFloat10);
DEREF_CHECK(TestEnvironment::Instance(),".pStreamI","StreamI *",8,0/* pointers not supporte by clone */,pStreamI); // treat as a pointer
DEREF_CHECK(TestEnvironment::Instance(),".myStream","StreamI",72,72,myStream);

/*
 * starts with test1Class introspectable anyType
 * then dereference on the base of the first string
 * then clones the variable
 * then checks the resulting type against the second string
 * then checks data size against fourth string
 * then checks overhead storage size against 5th string
 */
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".test2","class MARTe::Test2Class",sizeof(Test2Class),0,test2);   //
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".test3","union MARTe::Test3Class",sizeof(uint32),0,test3);
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".int16Arr","int16[12]",sizeof(test1Class.int16Arr),0,int16Arr);     //
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".int64Arr","int64[12][25]",sizeof(test1Class.int64Arr),0,int64Arr);
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".int32PVar*","int32",4,0,int32PVar);
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".VCharVar","const Vector<char8>",16,16,VCharVar);/* clone creates only const structures*/
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".MFloatVar","const Matrix<float32>",24,24,MFloatVar);
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".CStringZTAVar","const Vector<CCString>",83,56,CStringZTAVar); /* clone converts ZTA to vector */
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar","const Vector<const Vector<CCString>[4]>",912,656,CStringVAZTAVar);/* cloning changes to const Vector*/
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar[1]","const Vector<CCString>[4]",208,144,CStringVAZTAVar_1);/* cloning changes to const Vector*/
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".CStringVAZTAVar[1][2]","const Vector<CCString>",29,24,CStringVAZTAVar_1_2);
DEREF_CLONE_CHECK(TestEnvironment::Instance(),".MFloat10","const Matrix<float32( * const)[10]>",864,824,MFloat10);


CCString testPatt[3] = {CCString("grande"),CCString("spazio"),CCString("aperto")};
DEREF_CLONE_CHECKCONTENT_ANYTYPE(TestEnvironment::Instance(),".CStringVAZTAVar[1][1]","Vector<CCString>",testPatt,61,40,false,CStringVAZTAVar_VC);/* cloning creates const structs*/
DEREF_CLONE_CHECKCONTENT_ANYTYPE(TestEnvironment::Instance(),".CStringVAZTAVar[1][1]","const Vector<CCString>",testPatt,61,40,true,CStringVAZTAVar_CVC);/* cloning creates const structs*/
DEREF_CLONE_CHECKCONTENT_ANYTYPE(TestEnvironment::Instance(),".int16AAPAA","int16( *[3][5])[7][11]",int16AAPAA,2430,120,false,int16AAPAA_I3_5_7_11);/* cloning creates const structs*/
DEREF_CLONE_CHECKCONTENT_ANYTYPE(TestEnvironment::Instance(),".int16AAPAA","int16( * const[3][5])[7][11]",int16AAPAA,2430,120,true,int16AAPAA_CI3_5_7_11);/* cloning creates const structs*/
DEREF_CLONE_CHECKCONTENT_ANYTYPE_FAIL(TestEnvironment::Instance(),".SString","StreamString",(char8 *)"succhiarriello",78,64,false,SString_c8); // expect to fail as char8* is not considered a string
DEREF_CLONE_CHECKCONTENT_ANYTYPE(TestEnvironment::Instance(),".SString","StreamString",CCString("succhiarriello"),78,64,false,SString_CCS);


RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int8,1,1,SignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int8,1,2,SignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int16,1,2,SignedInteger16Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int32,1,2,SignedInteger32Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int64,1,2,SignedInteger64Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int32,1,8,SignedInteger32Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int16,1,32,SignedInteger16Bit);
RANDOM_2D_COPY_CHECKR(TestEnvironment::Instance(),int64 ,20,3000,5,SignedInteger64Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int64,2,2,SignedInteger64Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int8,8,8,SignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int64,1,10,SignedInteger64Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int32,12,10,SignedInteger32Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),uint32,2,2,UnsignedInteger32Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int16,34,12,SignedInteger16Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int8 ,56,87,SignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),uint8,22,3,UnsignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),int8 ,256,87,SignedInteger8Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),uint16 ,72,4,UnsignedInteger16Bit);
RANDOM_2D_COPY_CHECK(TestEnvironment::Instance(),uint32 ,27,17,UnsignedInteger32Bit);
RANDOM_2D_COPY_CHECKR(TestEnvironment::Instance(),uint32 ,4,45,5,UnsignedInteger32Bit);
RANDOM_2D_COPY_CHECKR(TestEnvironment::Instance(),uint64 ,141,532,5,UnsignedInteger64Bit);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,1,1,1,CCString,ConstCharString);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,1,6,6,CCString,ConstCharString);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,2,2,2,CCString,ConstCharString);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,5,5,5,CCString,ConstCharString);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,10,35,5,CCString,ConstCharString);
RANDOM_2D_CONVERT_CHECK(TestEnvironment::Instance(),DynamicCString ,515,235,5,CCString,ConstCharString);

COPY_CHECK_OK(TestEnvironment::Instance(),uint32, uint32,uint32_uint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32, Vector<uint32>,uint32_Vuint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32, Vector<uint64>,uint32_Vuint64);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32, uint8,uint32_uint8);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_1, uint32,uint32_1_uint32);

COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4, uint8,uint32_4_uint8);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4, uint32,uint32_4_uint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4, Vector<uint32>,uint32_4_Vuint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4, Vector<uint8>,uint32_4_Vuint8);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4, Matrix<uint32>,uint32_4_Muint32);

COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8, uint32,uint32_4_8_uint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8, Vector<uint32>,uint32_4_8_Vuint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8, Matrix<uint8>,uint32_4_8_Muint8);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8, uint32_4_8,uint32_4_8_uint32_4_8);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8, Matrix<uint32>,uint32_4_8_Muint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8, Matrix<uint64>,uint32_4_8_Muint64);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8, Vector<Matrix<uint64> >,uint32_4_8_VMuint64);

COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6, uint32,uint32_4_8_6_uint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6, uint32_4_8,uint32_4_8_6_uint32_4_8);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6, Vector<uint32>,uint32_4_8_6_Vuint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6, Matrix<uint32>,uint32_4_8_6_Muint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6, uint32_4_8_6,uint32_4_8_6_uint32_4_8_6);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6, Vector<Matrix<uint32> >,uint32_4_8_6_VMuint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6, Matrix<Vector<uint32> >,uint32_4_8_6_MVuint32);

COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6_5, Vector<Matrix<uint32> >,uint32_4_8_6_5_VMuint32);
COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6_5, Matrix<Vector<uint32> >,uint32_4_8_6_5_MVuint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6_5, Matrix<Matrix<uint32> >,uint32_4_8_6_5_MMuint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6_5, Vector<Vector<Matrix<uint32> > >,uint32_4_8_6_5_VVMuint32);

COPY_CHECK_NOK(TestEnvironment::Instance(),uint32_4_8_6_5_3, Vector<Vector<Matrix<uint32> > >,uint32_4_8_6_5_3_VVMuint32);
COPY_CHECK_OK(TestEnvironment::Instance(),uint32_4_8_6_5_3, Vector<Matrix<Matrix<uint32> > >,uint32_4_8_6_5_3_VMMuint32);

}//MARTe  namespace
