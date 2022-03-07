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
#include <Private/AnyObjectS.h>
#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BitRange.h"
#include "AnyType.h"
#include "VariableDescriptor.h"
#include "DynamicCString.h"
#include "StreamString.h"
#include "StaticCString.h"
#include "ErrorType.h"
#include "MemoryCheck.h"
#include "TypeDescriptor.h"
#include "CLASSMEMBERREGISTER.h"
#include "CLASSREGISTER.h"
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
        subTemplate(float,floatVar,[],className)\
        subTemplate(double,doubleVar,[],className)

/// declare Test2Class
class Test2Class{
public:

    Test2ClassTemplate(varDeclSubTemplate,arrayDeclSubTemplate,Test2Class)
} ;


/// register Test2Class
Test2ClassTemplate(memberDeclSubTemplate,memberDeclSubTemplate,Test2Class)

////
class MyStream: public StreamString{

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
        subTemplate(Matrix<float>,MFloatVar,[],className)\
        subTemplate(ZeroTerminatedArray<CCString>,CStringZTAVar,[],className)\
        subTemplate(ZeroTerminatedArray<Vector<CCString>[4]>,CStringVAZTAVar,[],className)\
        subTemplate(Matrix<float (*)[10]>,MFloat10,[],className)\
        subTemplate(StreamI *,pStreamI,[],className)\
        subTemplate(StreamString,SString,[],className)\
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
    test1Class.test3.bitset1 = 1;
    test1Class.test3.bitset2 = 2;
    test1Class.test3.bitset3 = 3;
    test1Class.test3.bitset4 = 4;
    test1Class.test3.bitset5 = 5;
    test1Class.test3.bitset6 = 6;
    test1Class.test3.bitset7 = 7;
    test1Class.test3.bitset8 = 8;

    test1Class.VCharVar.SetSize(3);
    test1Class.VCharVar[0]= 'a';
    test1Class.VCharVar[1]= 'b';
    test1Class.VCharVar[2]= 'c';

    test1Class.MFloatVar.SetSize(3,3);
    test1Class.MFloatVar[0][0]= 1.0;
    test1Class.MFloatVar[1][0]= 2.0;
    test1Class.MFloatVar[2][0]= 3.0;
    test1Class.MFloatVar[0][1]= 4.0;
    test1Class.MFloatVar[1][1]= 5.0;
    test1Class.MFloatVar[2][1]= 6.0;
    test1Class.MFloatVar[0][2]= 7.0;
    test1Class.MFloatVar[1][2]= 8.0;
    test1Class.MFloatVar[2][2]= 9.0;

    for (int i=0;i<12;i++) {
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
    		pdummy[i] = rand();
    	}
    	myPArrType0 *p =  &test1Class.int16AAPAA[0][0];
    	for (i=0;i<mypPAA1*mypPAA2;i++) {
    		p[i] = (myPArrType0 )&int16AAPAA[i/mypPAA2][i%mypPAA2][0][0];
    	}
    }

//printf("&arr1 = %p\n",&arr1);
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
    static Vector<CCString> vvx[4];
    static Vector<CCString> vv4[][4] = {{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{}};

    test1Class.CStringVAZTAVar = ZeroTerminatedArray<Vector<CCString>[4]>(vv4);

#if 0
    {  // TODO remove
        void **pp = (void **)test1Class.CStringVAZTAVar.GetList();
        for (int i=0;i<64;){
        	for (int j=0;j<8;j++)
        		printf("%p ",pp[i++]);  //TODO remove
        	printf("\n ");  //TODO remove
        }
    }
#endif

    static float (*arrayP10[10][10])[10];
    for (int i=0;i<10;i++) for (int j=0;j<10;j++) {
    	arrayP10[i][j] = NULL;
    }

    static float arrayF[10]= {1,2,3,4,5,6,7,8,9,10};
    arrayP10[1][1] = &arrayF;
    test1Class.MFloat10 = arrayP10;

    test1Class.SString = "succhiarriello";
    test1Class.pStreamI = &test1Class.SString;
    test1Class.SString.Seek(8);


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

ErrorManagement::ErrorType CheckType(AnyType at,CCString typeCheck){
    ErrorManagement::ErrorType err;

    DynamicCString string;
    DynamicCString string2;
    if (err){
        const VariableDescriptor &vd =  at.GetFullVariableDescriptor();
        err = vd.ToString(string);
        vd.ToString(string2,true);
        if (!err){
            printf("ToString error");
        }
    }

    if (err){
        if (!string.IsSameAs(typeCheck.GetList())){
            err.comparisonFailure = true;
            printf("(%s)%s != %s ",string2.GetList(),string.GetList(),typeCheck.GetList());
        }
    }
    return err;
}

static void PrintType(AnyType at){
    ErrorManagement::ErrorType err;
    DynamicCString string;

    err = at.ToString(string);
	printf("%s",string.GetList());
}


ErrorManagement::ErrorType CompareType(AnyType at1,AnyType at2){
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

ErrorManagement::ErrorType CheckSize(AnyType at,uint64 dataSizeCheck,uint64 storageSizeCheck ){
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

ErrorManagement::ErrorType CheckContent(AnyType at,CCString contentCheck){
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

ErrorManagement::ErrorType DereferenceAndCheckTypeAndSize(AnyType at,CCString expression, CCString typeCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true ){

	ErrorManagement::ErrorType ok;
	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");
//printf ("<<%s>>\n",at.GetFullVariableDescriptor().GetModifiers());

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
        for (uint32 ix= expression.GetSize();ix<28;ix++) putchar(' ');
        if (clone){
            printf ("%s-C>",expression.GetList());
        } else {
            printf ("%s ->",expression.GetList());
        }

        ErrorManagement::ErrorType ok2;
        ok2 = CheckType(at,typeCheck);
    	REPORT_ERROR(ok2,"CheckType error");

    	ok = ok | ok2;

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	REPORT_ERROR(ok2,"CheckSize error");

    	ok = ok | ok2;
    }


    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
    	printf("*NO*  -->See error Log ");
    } else {
        printf ("OK %s,sz=%lli,stor=%lli",typeCheck.GetList(),dataSizeCheck,storageSizeCheck);
    }
	printf("\n");

	return ok;
}


#define MARK

//printf("(%i)\n",__LINE__);

ErrorManagement::ErrorType DereferenceAndCheckTypeSizeAndContent(AnyType at,CCString expression,CCString typeCheck,CCString contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true ){
    ErrorManagement::ErrorType ok;

MARK
	for (uint32 ix= expression.GetSize();ix<28;ix++) putchar(' ');
	printf("%s", expression.GetList());
	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");

//printf("Is constant : %i\n",at.GetFullVariableDescriptor().GetSummaryTypeDescriptor().DataIsConstant());

MARK
	Reference atc;
MARK
	if (ok && clone){
		atc.FromAnyType(at);
//		atc = Reference(at);
MARK
		if (atc.IsValid()){
			atc.ToAnyType(at);
		} else {
			ok.fatalError = true;
			REPORT_ERROR(ok,"Cloning of variable failed");
		}
//printf("refNo = %i atc=%p\n",atc->NumberOfReferences(),atc.operator ->());
	}
MARK
	if (ok){
        if (clone){
            printf ("-C>");
        } else {
            printf ("->");
        }

	    ErrorManagement::ErrorType ok2;
MARK
	    ok2 = CheckType(at,typeCheck);
    	REPORT_ERROR(ok2,"CheckType error");
    	ok = ok | ok2;
MARK
    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	REPORT_ERROR(ok2,"CheckSize error");
    	ok = ok | ok2;
MARK
    	ok2 = CheckContent(at,contentCheck);
    	REPORT_ERROR(ok2,"CheckContent error");
    	ok = ok | ok2;
	}
MARK
    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
    	printf("*NO*  -->See error Log ");
    } else {
        printf("OK (%s)%s,sz=%lli,stor=%lli",typeCheck.GetList(),contentCheck.GetList(),dataSizeCheck,storageSizeCheck);
    }
	printf("\n");
MARK
	return ok;
}

/**
 * Takes anytype at, then evaluates the expression on it.
 * It is then checked against typeCheck,contentCheck, and dataSizeChecks
 */
ErrorManagement::ErrorType Check3(AnyType at,CCString expression,CCString typeCheck,AnyType contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone=true,bool expectFail=false ){
	ErrorManagement::ErrorType ok;

	ok = at.MultipleDereference(expression);
	REPORT_ERROR(ok,"MultipleDereference error");
//printf ("<<%s>>\n",at.GetFullVariableDescriptor().GetModifiers());

	Reference atc;
	if (ok && clone){
		atc.FromAnyType(at);
//		atc = Reference(at);

		if (atc.IsValid()){
			atc.ToAnyType(at);
//printf ("<<%s>>\n",at.GetFullVariableDescriptor().GetModifiers());

		} else {
			ok.fatalError = true;
			REPORT_ERROR(ok,"Cloning of variable failed");
		}
	}

	if (ok){
		for (uint32 ix= expression.GetSize();ix<28;ix++) putchar(' ');
        if (clone){
            printf ("%s-C>",expression.GetList());
        } else {
            printf ("%s ->",expression.GetList());
        }

	    ErrorManagement::ErrorType ok2;

	    ok2 = CheckType(at,typeCheck);
    	REPORT_ERROR(ok2,"CheckType error");

    	ok = ok | ok2;

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	REPORT_ERROR(ok2,"CheckSize error");

    	ok = ok | ok2;

    	ok2=at.CompareWith(contentCheck);
    	REPORT_ERROR(ok2,"CompareWith error");

    	ok = ok | ok2;
    }


    if (!ok){
    	DynamicCString resultS;
		at.ToString(resultS);
    	DynamicCString referenceS;
    	contentCheck.ToString(referenceS);
    	COMPOSITE_REPORT_ERROR(ok,referenceS.GetList()," != ","clone:",resultS.GetList());

		if (expectFail){
	    	printf("OK! Failed as expected - see log");
		} else {
	    	printf("*NO*  -->See error Log ");
		}
    } else {
		if (expectFail){
	    	printf("*NO*!  -->Should have failed ");
		} else {
			printf("OK (%s){content},sz=%lli,stor=%lli",typeCheck.GetList(),dataSizeCheck,storageSizeCheck);
		}
    }
	printf("\n");

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

//template <typename T, int size1, int size2,int minSize2=size2,typename T2=T>
template <typename T, int size1, int size2,int minSize2,typename T2>
ErrorManagement::ErrorType Check4(ProgressiveTypeCreator &pfstc,TypeDescriptor td){
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

	DynamicCString message;
	at.ToString(message);

    for (uint32 ix= message.GetSize();ix<28;ix++) putchar(' ');
    printf ("%s ->",message.GetList());
    message().Truncate(0);

	ErrorManagement::ErrorType ret;
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
				uint64 kk = rand();
				T k;
				NumSet(k,kk);
				(*targetB[j])[i]=k;
				targetA[j][i] = k;
				char8 buffer[32];
				sprintf(buffer,"%lli",kk);
				ret = pfstc.AddElement(buffer);
				COMPOSITE_REPORT_ERROR(ret,"pfstc.AddElement(",buffer,")");
			}
			targetC[j].InitVector(reinterpret_cast<T *>(&targetA[j]),actualSize2);

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

	if (ret){
		ret = pfstc.GetReference(aoi);
		ret.fatalError = !aoi.IsValid();
		REPORT_ERROR(ret,"pfstc.GetReference failed");

		if (ret){
			message().Append(aoi->GetClassRegistryItem()->GetClassName()).Append("{");
		}
	}

	if (ret){
		AnyType x;
		aoi.ToAnyType(x);
		ret = CompareType(x,at);
		if (ret){
			x.ToString(message);
		}
		message().Append("}");
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
		printf ("*NO* Random Creation/CopyTo Compare Failed (see log) \n");
	} else {
		printf ("OK %s :Random Creation/CopyTo Compare \n",message.GetList());
	}


	{
		for (int i=0;i<size1;i++){
			delete[] (targetB[i]);
		}
		delete[] targetAP;
	}

	pfstc.Clean();

	return ret;
}


template <typename T1,typename T2>
void TestSafeN2N(T1 value)
{
    T1 in = value;
    T2 out;
    StreamString ss;
    bool ret = SafeNumber2Number(in,out);
    double tst = out;
    if (!ret) printf("sat   "); else printf("no sat ");
    ss.Printf("%!(%?) --> %!(%?) !! %!(%?) \n",in,in,out,out,tst,tst);
    printf("%s",ss.Buffer().GetList());
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
    printf("%s",ss.Buffer().GetList());

}


template <typename type1,typename type2>
ErrorManagement::ErrorType CopyCheck(bool expectSuccess, CCString type1S,CCString type2S){
	ErrorManagement::ErrorType ret;
	type1 data;
	type2 dataCopy;

	{
		uint8 *p = reinterpret_cast<uint8 *> (&data);
		for (uint32 i = 0; i< sizeof(data);i++){
			p[i] = i*i+1;
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

    StreamString ss;

	if (!ret){
		if (expectSuccess){
		    ss.Printf(" NO = FAILED ! see log");
		} else {
		    ss.Printf(" OK = FAILED as expected");
		}
	} else {
		if (expectSuccess){
			ss.Printf(" OK");
		} else {
			ss.Printf(" NO = Sucess but should have FAILED - see LOG");
		}
	}

	ss.Printf(" (%s)x copy to (%s)y and compare y to x:",dataCS,dataCopyCS);

    printf("%s\n",ss.Buffer().GetList());
    return ret;
}

#define MEM_SAVE()
#define MEM_CHECK()

//#define MEM_SAVE()  { long long saveCount = allocCount; printf("<<%lli>",saveCount);

//#define MEM_CHECK()  ErrorManagement::ErrorType ok2; 	ok2.notCompleted = (saveCount != allocCount); REPORT_ERROR(ok2,"!!!!!!MEM_BALANCE Failed\n"); printf("<%lli>>",allocCount);}

#define DEREF_CHECK(at,mod,type,size,store)\
MEM_SAVE()\
ok = DereferenceAndCheckTypeAndSize(at,mod,type,size,store,false);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check1("  mod  ")Failed\n");

#define DEREF_CLONE_CHECK(at,mod,type,size,store)\
MEM_SAVE()\
ok = DereferenceAndCheckTypeAndSize(at,mod,type,size,store,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check1("  mod  ")Failed\n");

#define DEREF_CHECKCONTENT(at,mod,type,val,size,store)\
MEM_SAVE()\
ok = DereferenceAndCheckTypeSizeAndContent(at,mod,type,val,size,store,false);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check2(" mod ")Failed\n");

#define DEREF_CLONE_CHECKCONTENT(at,mod,type,val,size,store)\
MEM_SAVE()\
ok = DereferenceAndCheckTypeSizeAndContent(at,mod,type,val,size,store,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check2(" mod ")Failed\n");

#define CHECK3(at,mod,type,val,size,store,clone)\
MEM_SAVE()\
ok = Check3(at,mod,type,val,size,store,clone);   \
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check3(" mod ")Failed\n");

#define CHECK3N(at,mod,type,val,size,store,clone)\
MEM_SAVE()\
ok = Check3(at,mod,type,val,size,store,clone,true);   \
MEM_CHECK()\
REPORT_ERROR(ok,"......Check3N(" mod ")Failed as expected\n");

#define CHECK4(type1, size1, size2,minSize2,type2,typeId)\
MEM_SAVE()\
ok = Check4<type1,size1,size2,minSize2,type2> (pfstc,typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check4<>(" #type1 ")Failed\n");

#define CHECK4S(type1, size1, size2,typeId)\
MEM_SAVE()\
ok = Check4<type1,size1,size2,size2,type1> (pfstc,typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check4<>(" #type1 ")Failed\n");

#define CHECK4R(type1, size1, size2,minSize2,typeId)\
MEM_SAVE()\
ok = Check4<type1,size1,size2,minSize2,type1> (pfstc,typeId);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****Check4<>(" #type1 ")Failed\n");

#define COPY_CHECK_OK(type1, type2)\
MEM_SAVE()\
ok = CopyCheck<type1,type2> (true,#type1 , #type2);\
MEM_CHECK()\
REPORT_ERROR(ok,"*****CopyCheck<>(" #type1 "," #type2 ")Failed\n");

#define COPY_CHECK_NOK(type1, type2)\
MEM_SAVE()\
ok = CopyCheck<type1,type2> (false,#type1 , #type2);\
MEM_CHECK()\
REPORT_ERROR(ok,"......CopyCheck<>(" #type1 "," #type2 ")Failed as expected\n");


void xx(){
    ErrorManagement::ErrorType ok;
	ProgressiveTypeCreator pfstc(1024);
	CHECK4S(int8,1,1,1);
}

void Test(){

    ErrorManagement::ErrorType ok;
    AnyType at(test1Class);


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
    DEREF_CLONE_CHECKCONTENT(at,".int8Var","int8","18",sizeof(int8),0); // MC
    DEREF_CLONE_CHECKCONTENT(at,".char8Var","char8","c",sizeof(char8),0);
    DEREF_CLONE_CHECKCONTENT(at,".int16Var","int16","116",sizeof(int16),0);
    DEREF_CLONE_CHECKCONTENT(at,".int32Var","int32","132",sizeof(int32),0);
    DEREF_CLONE_CHECKCONTENT(at,".uint32Var","uint32","132",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".int64Var","int64","164",sizeof(int64),0);  // MC
    DEREF_CLONE_CHECKCONTENT(at,".floatVar","float32","10.100001",sizeof(float32),0);
    DEREF_CLONE_CHECKCONTENT(at,".doubleVar","float64","11100000000.000000",sizeof(double),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.char8Var","char8","c",sizeof(char8),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.int8Var","int8","8",sizeof(int8),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.int16Var","int16","16",sizeof(int16),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.int32Var","int32","32",sizeof(int32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.uint32Var","uint32","32",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.int64Var","int64","64",sizeof(int64),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.floatVar","float32","0.100000",sizeof(float32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test2.doubleVar","float64","1100000000.000000",sizeof(double),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset1","BitRange<uint32,1,0>","1",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset2","BitRange<uint32,2,1>","2",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset3","BitRange<uint32,3,3>","3",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset4","BitRange<uint32,4,6>","4",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset5","BitRange<uint32,5,10>","5",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset6","BitRange<uint32,6,15>","6",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset7","BitRange<uint32,7,21>","7",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".test3.bitset8","BitRange<uint32,4,28>","8",sizeof(uint32),0);
    DEREF_CLONE_CHECKCONTENT(at,".int16Arr[11]","int16","176",sizeof(test1Class.int16Arr[11]),0);
    DEREF_CLONE_CHECKCONTENT(at,".int64PArr[4][5]","int64","6",8,0);
    DEREF_CLONE_CHECKCONTENT(at,".CCStringVar","CCString","pippo",14,8);
    DEREF_CHECKCONTENT(at,".CCStringVar[0]","const char8","p",1,0);
    DEREF_CLONE_CHECKCONTENT(at,".CCStringVar[0]","const char8","p",1,0);
    DEREF_CLONE_CHECKCONTENT(at,".CStringVar","CString","mizzega",16,8);
    DEREF_CLONE_CHECKCONTENT(at,".CStringVar[2]","char8","z",1,0);
    DEREF_CLONE_CHECKCONTENT(at,".DCStringVar","DynamicCString","paperino",17,8);
    DEREF_CLONE_CHECKCONTENT(at,".DCStringVar[3]","char8","e",1,0);
    DEREF_CLONE_CHECKCONTENT(at,".CStringZTAVar[0]","CCString","uno",12,8);
    DEREF_CLONE_CHECKCONTENT(at,".CStringZTAVar[2]","CCString","tre",12,8);
    DEREF_CLONE_CHECKCONTENT(at,".CStringZTAVar[2][2]","const char8","e",1,0);
    DEREF_CLONE_CHECKCONTENT(at,".CStringVAZTAVar[1][2][0]","CCString","agii",13,8);
    DEREF_CLONE_CHECKCONTENT(at,".CStringVAZTAVar[1][2][0][3]","const char8","i",1,0);

    /*
     * starts with test1Class introspectable anyType
     * then dereference on the base of the first string
     * then checks the resulting type against the second string
     * then converts value to string and checks against third string
     * then checks data size against fourth string
     * then checks overhead storage size against 5th string
     */
    DEREF_CHECKCONTENT(at,".pStreamI*","StreamI","riello",22,8);
    test1Class.pStreamI->Seek(3);
    DEREF_CHECKCONTENT(at,".pStreamI*","StreamI","chiarriello",22,8);
    DEREF_CHECKCONTENT(at,".SString","StreamString","succhiarriello",78,64);

    /// these tests operate on more complex variables
    /*
     * starts with test1Class introspectable anyType
     * then dereference on the base of the first string
     * then checks the resulting type against the second string
     * then checks data size against fourth string
     * then checks overhead storage size against 5th string
     */
    DEREF_CHECK(at,".int64PArr","int64( *[11])[21]",424,sizeof(test1Class.int64PArr)/* cannot clone pointers*/);
    DEREF_CHECK(at,".int64PArr[4]","int64( *)[21]",176,8); // cannot clone pointers
    DEREF_CHECK(at,".int32PVar","int32 *",8,0);  // cannot clone pointers
    DEREF_CHECK(at,".VCharVar","Vector<char8>",19,16/* clone creates only const structures*/);
    DEREF_CHECK(at,".MFloatVar","Matrix<float32>",60,24/* clone creates only const structures*/);
    DEREF_CHECK(at,".CStringZTAVar","ZeroTerminatedArray<CCString>",83,56/* clone converts ZTA to vector */);
    DEREF_CHECK(at,".CStringVAZTAVar","ZeroTerminatedArray<Vector<CCString>[4]>",904,648/* cloning changes type*/);
    DEREF_CHECK(at,".CStringVAZTAVar[1]","Vector<CCString>[4]",208,144);
    DEREF_CHECK(at,".CStringVAZTAVar[1][2]","Vector<CCString>",29,24);
    DEREF_CHECK(at,".MFloat10","Matrix<float32( *)[10]>",864,824);
    DEREF_CHECK(at,".pStreamI","StreamI *",8,0/* pointers not supporte by clone */); // treat as a pointer
    DEREF_CHECK(at,".myStream","StreamI",72,72);

    /*
     * starts with test1Class introspectable anyType
     * then dereference on the base of the first string
     * then clones the variable
     * then checks the resulting type against the second string
     * then checks data size against fourth string
     * then checks overhead storage size against 5th string
     */
    DEREF_CLONE_CHECK(at,".test2","MARTe::Test2Class",sizeof(Test2Class),0);   //
    DEREF_CLONE_CHECK(at,".test3","MARTe::Test3Class",sizeof(uint32),0);
    DEREF_CLONE_CHECK(at,".int16Arr","int16[12]",sizeof(test1Class.int16Arr),0);     //
    DEREF_CLONE_CHECK(at,".int64Arr","int64[12][25]",sizeof(test1Class.int64Arr),0);
    DEREF_CLONE_CHECK(at,".int32PVar*","int32",4,0);
    DEREF_CLONE_CHECK(at,".VCharVar","const Vector<char8>",19,16/* clone creates only const structures*/);
    DEREF_CLONE_CHECK(at,".MFloatVar","const Matrix<float32>",60,24);
    DEREF_CLONE_CHECK(at,".CStringZTAVar","const Vector<CCString>",83,56/* clone converts ZTA to vector */);
    DEREF_CLONE_CHECK(at,".CStringVAZTAVar","const Vector<const Vector<CCString>[4]>",848,592/* cloning changes to const Vector*/);
    DEREF_CLONE_CHECK(at,".CStringVAZTAVar[1]","const Vector<CCString>[4]",208,144/* cloning changes to const Vector*/);
    DEREF_CLONE_CHECK(at,".CStringVAZTAVar[1][2]","const Vector<CCString>",29,24);
    DEREF_CLONE_CHECK(at,".MFloat10","const Matrix<float32( * const)[10]>",864,824);


//return;
    CCString testPatt[3] = {CCString("grande"),CCString("spazio"),CCString("aperto")};
    CHECK3(at,".CStringVAZTAVar[1][1]","Vector<CCString>",testPatt,61,40,false/* cloning creates const structs*/);
    CHECK3(at,".CStringVAZTAVar[1][1]","const Vector<CCString>",testPatt,61,40,true/* cloning creates const structs*/);
    CHECK3(at,".int16AAPAA","int16( *[3][5])[7][11]",int16AAPAA,2430,120,false/* cloning creates const structs*/);
    CHECK3(at,".int16AAPAA","int16( * const[3][5])[7][11]",int16AAPAA,2430,120,true/* cloning creates const structs*/);
    CHECK3N(at,".SString","StreamString",(char8 *)"succhiarriello",78,64,false); // expect to fail as char8* is not considered a string
    CHECK3(at,".SString","StreamString",CCString("succhiarriello"),78,64,false);

    ProgressiveTypeCreator pfstc(1024);

	CHECK4S(int8,1,1,SignedInteger8Bit);
	CHECK4S(int8,1,2,SignedInteger8Bit);
	CHECK4S(int16,1,2,SignedInteger16Bit);
	CHECK4S(int32,1,2,SignedInteger32Bit);
	CHECK4S(int64,1,2,SignedInteger64Bit);
	CHECK4S(int32,1,8,SignedInteger32Bit);
	CHECK4S(int16,1,32,SignedInteger16Bit);
	CHECK4R(int64 ,20,3000,5,SignedInteger64Bit);
	CHECK4S(int64,2,2,SignedInteger64Bit);
	CHECK4S(int8,8,8,SignedInteger8Bit);
	CHECK4S(int64,1,10,SignedInteger64Bit);
	CHECK4S(int32,12,10,SignedInteger32Bit);
	CHECK4S(uint32,2,2,UnsignedInteger32Bit);
	CHECK4S(int16,34,12,SignedInteger16Bit);
	CHECK4S(int8 ,56,87,SignedInteger8Bit);
	CHECK4S(uint8,22,3,UnsignedInteger8Bit);
	CHECK4S(int8 ,256,87,SignedInteger8Bit);
	CHECK4S(uint16 ,72,4,UnsignedInteger16Bit);
	CHECK4S(uint32 ,27,17,UnsignedInteger32Bit);
	CHECK4R(uint32 ,4,45,5,UnsignedInteger32Bit);
	CHECK4R(uint64 ,141,532,5,UnsignedInteger64Bit);
	CHECK4(DynamicCString ,1,1,1,CCString,ConstCharString);
//	return;
	CHECK4(DynamicCString ,1,6,6,CCString,ConstCharString);
	CHECK4(DynamicCString ,2,2,2,CCString,ConstCharString);
	CHECK4(DynamicCString ,5,5,5,CCString,ConstCharString);
	CHECK4(DynamicCString ,10,35,5,CCString,ConstCharString);
	CHECK4(DynamicCString ,515,235,5,CCString,ConstCharString);
//	return;

    typedef uint32 uint32_4[4];
    typedef uint32 uint32_1[1];
    typedef uint32 uint32_4_8[4][8];
    typedef uint32 uint32_4_8_6[4][8][6];
    typedef uint32 uint32_4_8_6_5[4][8][6][5];
    typedef uint32 uint32_4_8_6_5_3[4][8][6][5][3];

#if 1
    COPY_CHECK_OK(uint32, uint32);
    COPY_CHECK_OK(uint32, Vector<uint32>);
    COPY_CHECK_OK(uint32, Vector<uint64>);
    COPY_CHECK_NOK(uint32, uint8);
    COPY_CHECK_OK(uint32_1, uint32);

    COPY_CHECK_NOK(uint32_4, uint8);
    COPY_CHECK_NOK(uint32_4, uint32);
    COPY_CHECK_OK(uint32_4, Vector<uint32>);
    COPY_CHECK_NOK(uint32_4, Vector<uint8>);
    COPY_CHECK_NOK(uint32_4, Matrix<uint32>);

    COPY_CHECK_NOK(uint32_4_8, uint32);
    COPY_CHECK_NOK(uint32_4_8, Vector<uint32>);
    COPY_CHECK_NOK(uint32_4_8, Matrix<uint8>);
    COPY_CHECK_OK(uint32_4_8, uint32_4_8);
    COPY_CHECK_OK(uint32_4_8, Matrix<uint32>);
    COPY_CHECK_OK(uint32_4_8, Matrix<uint64>);
    COPY_CHECK_NOK(uint32_4_8, Vector<Matrix<uint64> >);

    COPY_CHECK_NOK(uint32_4_8_6, uint32);
    COPY_CHECK_NOK(uint32_4_8_6, uint32_4_8);
    COPY_CHECK_NOK(uint32_4_8_6, Vector<uint32>);
    COPY_CHECK_NOK(uint32_4_8_6, Matrix<uint32>);
    COPY_CHECK_OK(uint32_4_8_6, uint32_4_8_6);

    COPY_CHECK_OK(uint32_4_8_6, Vector<Matrix<uint32> >);
    COPY_CHECK_OK(uint32_4_8_6, Matrix<Vector<uint32> >);

    COPY_CHECK_NOK(uint32_4_8_6_5, Vector<Matrix<uint32> >);
    COPY_CHECK_NOK(uint32_4_8_6_5, Matrix<Vector<uint32> >);
    COPY_CHECK_OK(uint32_4_8_6_5, Matrix<Matrix<uint32> >);
    COPY_CHECK_OK(uint32_4_8_6_5, Vector<Vector<Matrix<uint32> > >);

    COPY_CHECK_NOK(uint32_4_8_6_5_3, Vector<Vector<Matrix<uint32> > >);
    COPY_CHECK_OK(uint32_4_8_6_5_3, Vector<Matrix<Matrix<uint32> > >);
#endif
	TestSafeN2N<float32,int20>(1e6);
    TestSafeN2N<float32,int21>(1e6);
    TestSafeN2N<float32,uint19>(1e6);
    TestSafeN2N<float32,uint20>(1e6);
    TestSafeN2N<float32,int20>(-1e6);
    TestSafeN2N<float32,int21>(-1e6);
    TestSafeN2N<uint30,int27>(1000000000);
    TestSafeN2N<uint35,int32>(4000000000);
    TestSafeN2N<uint35,uint32>(4000000000);
    TestSafeN2N<int35 ,uint32>(4000000000);
    TestSafeN2N<int35 ,float32>(4000000000);
    TestSafeN2N<int35 ,double>(4000000000);
    TestSafeN2N<double,uint17>(4000000000);

    TestSatInteger(1e4,1e4,5000,4000,SaturatedInteger<uint32>(uint32(100001000)));
    TestSatInteger(1e4,1e4,5000,4000,SaturatedInteger<uint16>(uint32(100001000)));

    printf ("%i %i %le \n",TypeCharacteristics<double>::UsableBitSize(),DBL_MAX_EXP,TypeCharacteristics<double>::MaxValue());
    printf ("%i %le \n",TypeCharacteristics<float32>::UsableBitSize(),TypeCharacteristics<float32>::MaxValue());
    printf ("%i %i \n",TypeCharacteristics<uint17>::UsableBitSize(),TypeCharacteristics<uint17>::MaxValue());


    int32 temp;
	AnyType xx;
	Reference zz;

	PrintType(temp);printf("\n");
	PrintType(zz.operator MARTe::AnyType());printf("\n");
	PrintType(xx);printf("\n");

	xx = temp;
	PrintType(xx);printf("\n");

	ReferenceT<AnyObjectS<9> > ao8(HeapManager::standardHeapId);
	ao8->Setup(sizeof(temp),&temp,VariableDescriptor(&temp));

	xx = ao8.operator MARTe::AnyType();
	PrintType(xx);printf("\n");
        zz = ao8;
        xx = zz.operator MARTe::AnyType();
	PrintType(xx);printf("\n");

	printf("test of GetVariableInformation\n");
	{

		int pippo[5]= {1,2,3,4,5};
		AnyType pippoAT(pippo);

		TypeDescriptor td;
		uint32 maxDim = 1;
		uint32 dimension[3] = {0,0,0};
		pippoAT.GetVariableInformation(td,maxDim,&dimension[0]);
		printf("int[5] ==> nDim=%i size = %i\n",maxDim,dimension[0]);

		Vector<int32> pippa(32);
		AnyType pippaAT(pippa);
		maxDim = 1;
		dimension[0] = 0;
		pippaAT.GetVariableInformation(td,maxDim,&dimension[0]);
		printf("Vector<int>(32) ==> nDim=%i size = %i\n",maxDim,dimension[0]);

		DynamicCString pippu;
		AnyType pippuAT(pippu);
		maxDim = 1;
		dimension[0] = 0;
		pippuAT.GetVariableInformation(td,maxDim,&dimension[0]);
		printf("DynamicCString ==> nDim=%i size = %i\n",maxDim,dimension[0]);

		float32 (*pippx[5])[6];
		AnyType pippxAT(pippx);
		maxDim = 3;
		dimension[0] = 0;
		pippxAT.GetVariableInformation(td,maxDim,dimension);
		printf("float32 (*[5])[6] ==> nDim=%i size = %i,%i,%i\n",maxDim,dimension[0],dimension[1],dimension[2]);

		ZeroTerminatedArray<int [5]> pippy[7];
		AnyType pippyAT(pippy);
		maxDim = 3;
		dimension[0] = 0;
		pippyAT.GetVariableInformation(td,maxDim,dimension);
		printf("ZeroTerminatedArray<int [5]> [7] ==> nDim=%i size = %i,%i,%i\n",maxDim,dimension[0],dimension[1],dimension[2]);

	}

    {
        static CCString pippo[5] = {
                CCString("uno"),
                CCString("duo"),
                CCString("tre"),
                CCString("quattro"),
                CCString("cinque")
        };

        Vector<DynamicCString> pluto;
        AnyType pippoAT(pippo);

        ErrorManagement::ErrorType ret = pippoAT.CopyTo(pluto);
        REPORT_ERROR(ret,"CopyTo failed");
        if (ret){
            for (uint32 i = 0;i< pluto.GetNumberOfElements();i++){
                printf("%s\n",pluto[i].GetList());
        }
    }
  }
}
}





int main(int argc, char **argv){
	MARTe::StartupManager::Initialise();
#if 1
	MARTe::PrepareTestObject();

	{
	    MARTe::StreamString dump,structure;
	    dump.Printf("%J!",MARTe::test1Class);
	    printf("DUMP:\n%s\n\n",dump.Buffer().GetList());
        structure.Printf("%?",MARTe::test1Class);
        printf("STRUCTURE:\n%s\n\n",structure.Buffer().GetList());
	}

    MARTe::Test();

//    MARTe::Vector<float32> *p = new MARTe::Vector<float32>[4];
    void *q = malloc(sizeof(MARTe::Vector<float>)*4);
	((MARTe::Vector<float> *)q)[0].InitVector(NULL,0);
	((MARTe::Vector<float> *)q)[1].InitVector(NULL,0);
	((MARTe::Vector<float> *)q)[2].InitVector(NULL,0);
	((MARTe::Vector<float> *)q)[3].InitVector(NULL,0);

#endif
    


    MARTe::StartupManager::Terminate();
    return 0;
}
