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

#include "../L2Objects/ProgressiveTypeCreator.h"
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
#include "AnyObjectT.h"
#include "SaturatedInteger.h"


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

    for (int i=0;i<12;i++) test1Class.int16Arr[i] = 16*i;

    test1Class.int32PVar = &test1Class.test2.int32Var;
    test1Class.CCStringVar = data[0];
    test1Class.CStringVar  = dataBuffer;
    test1Class.DCStringVar = data[2];

    static int64 arr1[mypPAA3*mypPAA1] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};
    for (int i=0;i<mypPAA4;i++) test1Class.int64PArr[i] = NULL;
    test1Class.int64PArr[0]= &arr1;
    test1Class.int64PArr[4]= &arr1;

    static int16 (*int16AAPAAp)[mypPAA1*mypPAA2][mypPAA1*mypPAA2] ;

    {
    	uint32 i,j;
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
    static Vector<CCString> vv4[][4] = {{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{a1,a2,a3,a4},{0,0,0,0}};
    test1Class.CStringVAZTAVar = ZeroTerminatedArray<Vector<CCString>[4]>(vv4);

    static float (*arrayP10[10][10])[10];
    for (int i=0;i<10;i++) for (int j=0;j<10;j++) arrayP10[i][j] = NULL;
    static float arrayF[10];
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
        if (!string.isSameAs(typeCheck.GetList())){
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
	printf("%s",string);
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
        if (!string1.isSameAs(string2.GetList())){
            err.comparisonFailure = true;
            DynamicCString errM;
            errM.Append(string1.GetList());
            errM.Append('{');
            errM.Append(vd1.GetModifiers());
            errM.Append("} != ");
            errM.Append(string2.GetList());
            errM.Append('{');
            errM.Append(vd2.GetModifiers());
            errM.Append('}');
			REPORT_ERROR(err,errM.GetList());


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
    	if (!ok){
    		REPORT_ERROR(ok,"GetSize error");
    	}
    }

    if (ok){
        if ((dataSize != dataSizeCheck) || (storageSize != storageSizeCheck)){
        	ok.comparisonFailure = true;
        	DynamicCString errM;
        	errM.Append('(');
        	errM.Append(dataSize);
        	errM.Append(',');
        	errM.Append(storageSize);
        	errM.Append(") != (");
        	errM.Append(dataSizeCheck);
        	errM.Append(',');
        	errM.Append(storageSizeCheck);
        	errM.Append(')');
    		REPORT_ERROR(ok,errM.GetList());

//            printf("(%lli,%lli) != (%lli,%lli) ",dataSize,storageSize,dataSizeCheck,storageSizeCheck);
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

    if (ok){
		ok.comparisonFailure = !destinationString.isSameAs(contentCheck.GetList());
    	if (!ok){
        	printf("Values (%s != %s)",destinationString.GetList(),contentCheck.GetList());
    	}
	}

    return ok;
}

ErrorManagement::ErrorType CheckAnyContent(AnyType at,AnyType contentCheck){
    ErrorManagement::ErrorType ok;

   	ok=at.CompareWith(contentCheck);
   	if (!ok){
		REPORT_ERROR(ok,"CheckAnyContent error");
   	}

    return ok;
}


void Check1(AnyType at,CCString expression, CCString typeCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true ){
    ErrorManagement::ErrorType ok;
	ok = at.MultipleDereference(expression);
	CONDITIONAL_REPORT_ERROR(ok,"MultipleDereference error");

	// creates a clone
	Reference atc;
	if (ok && clone){
		atc = Reference(at);

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
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckType error");

    	ok.SetError(ok2);

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckSize error");

    	ok.SetError(ok2);
    }


    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
    	printf("*NO*  -->See error Log ");
    } else {
        printf ("OK %s,sz=%lli,stor=%lli",typeCheck.GetList(),dataSizeCheck,storageSizeCheck);
    }
	printf("\n");
}



void Check2(AnyType at,CCString expression,CCString typeCheck,CCString contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone = true ){
    ErrorManagement::ErrorType ok;

	ok = at.MultipleDereference(expression);
	CONDITIONAL_REPORT_ERROR(ok,"MultipleDereference error");

	Reference atc;
	if (ok && clone){
		atc = Reference(at);

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
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckType error");

    	ok.SetError(ok2);

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckSize error");

    	ok.SetError(ok2);

    	ok2 = CheckContent(at,contentCheck);
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckContent error");

    	ok.SetError(ok2);
	}

    if (!ok){
		REPORT_ERROR(ok,expression.GetList());
    	printf("*NO*  -->See error Log ");
    } else {
        printf("OK (%s)%s,sz=%lli,stor=%lli",typeCheck.GetList(),contentCheck.GetList(),dataSizeCheck,storageSizeCheck);
    }
	printf("\n");
}

/**
 * Takes anytype at, then evaluates the expression on it.
 * It is then checked against typeCheck,contentCheck, and dataSizeChecks
 */
void Check3(AnyType at,CCString expression,CCString typeCheck,AnyType contentCheck,uint64 dataSizeCheck,uint64 storageSizeCheck,bool clone=true,bool expectFail=false ){
	ErrorManagement::ErrorType ok;

	ok = at.MultipleDereference(expression);
	CONDITIONAL_REPORT_ERROR(ok,"MultipleDereference error");

	Reference atc;
	if (ok && clone){
		atc = Reference(at);

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
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckType error");

    	ok.SetError(ok2);

    	ok2 = CheckSize(at,dataSizeCheck,storageSizeCheck);
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckSize error");

    	ok.SetError(ok2);

    	ok2 = CheckAnyContent(at,contentCheck);
    	CONDITIONAL_REPORT_ERROR(ok2,"CheckAnyContent error");

    	ok.SetError(ok2);
    }


    if (!ok){
    	DynamicCString errM;
    	errM.Append(expression.GetList());
    	errM.Append(" --> ");
		contentCheck.ToString(errM);
		REPORT_ERROR(ok,errM.GetList());

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
}


template<typename T1>
 static inline  void NumSet(T1 &dest, uint64 &source) {
		SafeNumber2Number(source,dest);
		SafeNumber2Number(dest,source);
 }

//static inline void NumSet<DynamicCString>(DynamicCString &dest,uint32 &source) {
static inline void NumSet(DynamicCString &dest,uint64 &source) {
		dest.Truncate(0);
		dest.Append(source);
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

template <typename T, int size1, int size2,int minSize2=size2,typename T2=T>
void Check4(ProgressiveTypeCreator &pfstc,TypeDescriptor td){
	// remapped types for type comparison - in case T is for storing only
	typedef  T (TA)[size1][size2];
	typedef  T (*(TB)[size1])[size2];
	typedef  Vector<T>(TC)[size1];
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
    message.Truncate(0);

	ErrorManagement::ErrorType ret;
	ret = pfstc.Start(td);
	if (!ret){
		REPORT_ERROR(ret,"pfstc.Start failed");
	}

	//int32 counter = 0;	//TODO
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
//kk = counter++;//TODO
				sprintf(buffer,"%lli",kk);
				ret = pfstc.AddElement(buffer);
				if (!ret){
					DynamicCString errMsg;
					errMsg.Append("pfstc.AddElement(");
					errMsg.Append(buffer);
					errMsg.Append(")");
					REPORT_ERROR(ret,errMsg);
				}
			}
			targetC[j].InitVector(reinterpret_cast<T *>(&targetA[j]),actualSize2);
//printf("S@%p -> (%p %i)\n",&targetC[j],&targetA[j],actualSize2);

			if (ret){
				ret = pfstc.EndVector();
				if (!ret){
					REPORT_ERROR(ret,"pfstc.EndVector failed");
				}
			}
		}
	}

	Reference aoi;
	if (ret){
		ret = pfstc.End();
		if (!ret){
			REPORT_ERROR(ret,"pfstc.End failed");
		}
	}

	if (ret){
		ret = pfstc.GetReference(aoi);
		ret.fatalError = !aoi.IsValid();
		if (!ret){
			REPORT_ERROR(ret,"pfstc.GetReference failed");
		} else {
			message.Append(aoi->GetClassRegistryItem()->GetClassName());
//			message.Append(aoi->GetClassRegistryItem()->GetTypeidName());
			message.Append("{");
		}
	}
	if (ret){
		AnyType x;
		aoi.ToAnyType(x);
		ret = CompareType(x,at);
		if (ret){
			x.ToString(message);
//			DynamicCString s;
//			x.ToString(s);
//			message.Append(s.GetList());
		}
		message.Append("}");
	}
	//ok

	if (ret){
		AnyType x;
		aoi.ToAnyType(x);
	    ret = x.CompareWith(at);
		if (!ret){
			REPORT_ERROR(ret,"Compare content fault");
		}
	}
	if (!ret){
		DynamicCString string;
		string.Append("Failed ");
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
//    int32 ival = i1;
    ss.Printf("%!(%?) --> %!(%?) !! %!(%?) \n",in,in,out,out,tst,tst);
    printf("%s",ss.Buffer());
//    printf ("%f --> %i(int%i)\n",f,ival,i1.GetNumberOfBits());

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

	if (x5.notANormalNumber()){
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

	if (expected.notANormalNumber()){
		ss.Printf("%s\n",CCString("saturated"));
	} else {
		ss.Printf("%i\n",expected.GetData());
	}
    printf("%s",ss.Buffer());

}

void Test(){

    AnyType at(test1Class);

    Check2(at,".int8Var","int8","18",sizeof(int8),0);
    Check2(at,".char8Var","char8","c",sizeof(char8),0);
    Check2(at,".int16Var","int16","116",sizeof(int16),0);
    Check2(at,".int32Var","int32","132",sizeof(int32),0);
    Check2(at,".uint32Var","uint32","132",sizeof(uint32),0);
    Check2(at,".int64Var","int64","164",sizeof(int64),0);
    Check2(at,".floatVar","float","10.100001",sizeof(float),0);
    Check2(at,".doubleVar","double","11100000000.000000",sizeof(double),0);
    Check2(at,".test2.char8Var","char8","c",sizeof(char8),0);
    Check2(at,".test2.int8Var","int8","8",sizeof(int8),0);
    Check2(at,".test2.int16Var","int16","16",sizeof(int16),0);
    Check2(at,".test2.int32Var","int32","32",sizeof(int32),0);
    Check2(at,".test2.uint32Var","uint32","32",sizeof(uint32),0);
    Check2(at,".test2.int64Var","int64","64",sizeof(int64),0);
    Check2(at,".test2.floatVar","float","0.100000",sizeof(float),0);
    Check2(at,".test2.doubleVar","double","1100000000.000000",sizeof(double),0);
    Check1(at,".test2","class MARTe::Test2Class",sizeof(Test2Class),0);
    Check1(at,".test3","union MARTe::Test3Class",sizeof(uint32),0);
    Check2(at,".test3.bitset1","BitRange<uint32,1,0>","1",sizeof(uint32),0);
    Check2(at,".test3.bitset2","BitRange<uint32,2,1>","2",sizeof(uint32),0);
    Check2(at,".test3.bitset3","BitRange<uint32,3,3>","3",sizeof(uint32),0);
    Check2(at,".test3.bitset4","BitRange<uint32,4,6>","4",sizeof(uint32),0);
    Check2(at,".test3.bitset5","BitRange<uint32,5,10>","5",sizeof(uint32),0);
    Check2(at,".test3.bitset6","BitRange<uint32,6,15>","6",sizeof(uint32),0);
    Check2(at,".test3.bitset7","BitRange<uint32,7,21>","7",sizeof(uint32),0);
    Check2(at,".test3.bitset8","BitRange<uint32,4,28>","8",sizeof(uint32),0);
    Check1(at,".int16Arr","int16[12]",sizeof(test1Class.int16Arr),0);
    Check2(at,".int16Arr[11]","int16","176",sizeof(test1Class.int16Arr[11]),0);
    Check1(at,".int64Arr","int64[12][25]",sizeof(test1Class.int64Arr),0);
    Check1(at,".int64PArr","int64( *[11])[21]",424,sizeof(test1Class.int64PArr),false/* cannot clone pointers*/);
    Check1(at,".int64PArr[4]","int64( *)[21]",176,8,false); // cannot clone pointers
    Check2(at,".int64PArr[4][5]","int64","6",8,0);
    Check1(at,".int32PVar","int32 *",8,0,false);  // cannot clone pointers
    Check1(at,".int32PVar*","int32",4,0);
    Check2(at,".CCStringVar","CCString","pippo",14,8);
    Check2(at,".CCStringVar[0]","const char8","p",1,0);
    Check2(at,".CStringVar","CString","mizzega",16,8);
    Check2(at,".CStringVar[2]","char8","z",1,0);
    Check2(at,".DCStringVar","DynamicCString","paperino",17,8);
    Check2(at,".DCStringVar[3]","char8","e",1,0);
    Check1(at,".VCharVar","Vector<char8>",16,16,false/* clone creates only non modifiable structures*/);
    Check1(at,".VCharVar","const Vector<char8>",16,16/* clone creates only non modifiable structures*/);
    Check1(at,".MFloatVar","Matrix<float>",24,24);
    Check1(at,".CStringZTAVar","const Vector<CCString>",83,56/* clone converts ZTA to vector */);
    Check1(at,".CStringZTAVar","ZeroTerminatedArray<CCString>",83,56,false/* clone converts ZTA to vector */);
    Check2(at,".CStringZTAVar[0]","CCString","uno",12,8);
    Check2(at,".CStringZTAVar[2]","CCString","tre",12,8);
    Check2(at,".CStringZTAVar[2][2]","const char8","e",1,0);
    Check1(at,".CStringVAZTAVar","ZeroTerminatedArray<Vector<CCString>[4]>",968,712,false/* cloning changes type*/);
    Check1(at,".CStringVAZTAVar","const Vector<const Vector<CCString>[4]>",912,656/* cloning changes to const Vector*/);
    Check1(at,".CStringVAZTAVar[1]","Vector<CCString>[4]",208,144,false);
    Check1(at,".CStringVAZTAVar[1]","const Vector<CCString>[4]",208,144/* cloning changes to const Vector*/);
    Check1(at,".CStringVAZTAVar[1][2]","Vector<CCString>",29,24,false);
    Check1(at,".CStringVAZTAVar[1][2]","const Vector<CCString>",29,24);
    Check2(at,".CStringVAZTAVar[1][2][0]","CCString","agii",13,8);
    Check2(at,".CStringVAZTAVar[1][2][0][3]","const char8","i",1,0);
    CCString testPatt[3] = {CCString("grande"),CCString("spazio"),CCString("aperto")};
    Check3(at,".CStringVAZTAVar[1][1]","Vector<CCString>",testPatt,61,40,false/* cloning creates const structs*/);
    Check3(at,".CStringVAZTAVar[1][1]","const Vector<CCString>",testPatt,61,40,true/* cloning creates const structs*/);
    Check3(at,".int16AAPAA","int16( *[3][5])[7][11]",int16AAPAA,2430,120,false/* cloning creates const structs*/);
    Check3(at,".int16AAPAA","int16( * const[3][5])[7][11]",int16AAPAA,2430,120,true/* cloning creates const structs*/);
    Check1(at,".MFloat10","Matrix<float( *)[10]>",864,824);
    Check1(at,".pStreamI","StreamI *",8,0,false/* pointers not supporte by clone */); // treat as a pointer
    Check2(at,".pStreamI*","StreamI","riello",22,8,false);
    test1Class.pStreamI->Seek(3);
    Check2(at,".pStreamI*","StreamI","chiarriello",22,8,false);
//    Check2(at,".pStreamI","StreamI *","chiarriello",30,16);  // this we do not support anymore
    Check2(at,".SString","StreamString","succhiarriello",78,64,false);
    Check3(at,".SString","StreamString",(char8 *)"succhiarriello",78,64,false,true); // expect to fail as char8* is not considered a string
    Check3(at,".SString","StreamString",CCString("succhiarriello"),78,64,false);
    Check1(at,".myStream","StreamI",72,72,false);

	ProgressiveTypeCreator pfstc(1024);

	Check4<int8,1,1>(pfstc,SignedInteger8Bit);
	Check4<int8,1,2>(pfstc,SignedInteger8Bit);
	Check4<int16,1,2>(pfstc,SignedInteger16Bit);
	Check4<int32,1,2>(pfstc,SignedInteger32Bit);
	Check4<int64,1,2>(pfstc,SignedInteger64Bit);
	Check4<int32,1,8>(pfstc,SignedInteger32Bit);
	Check4<int16,1,32>(pfstc,SignedInteger16Bit);
	Check4<int64 ,20,3000,5>(pfstc,SignedInteger64Bit);
	Check4<int64,2,2>(pfstc,SignedInteger64Bit);
	Check4<int8,8,8>(pfstc,SignedInteger8Bit);
	Check4<int64,1,10>(pfstc,SignedInteger64Bit);
	Check4<int32,12,10>(pfstc,SignedInteger32Bit);
	Check4<uint32,2,2>(pfstc,UnsignedInteger32Bit);
	Check4<int16,34,12>(pfstc,SignedInteger16Bit);
	Check4<int8 ,56,87>(pfstc,SignedInteger8Bit);
	Check4<uint8,22,3>(pfstc,UnsignedInteger8Bit);
	Check4<int8 ,256,87>(pfstc,SignedInteger8Bit);
	Check4<uint16 ,72,4>(pfstc,UnsignedInteger16Bit);
	Check4<uint32 ,27,17>(pfstc,UnsignedInteger32Bit);
	Check4<uint32 ,4,45,5>(pfstc,UnsignedInteger32Bit);
	Check4<uint64 ,141,532,5>(pfstc,UnsignedInteger64Bit);
	Check4<DynamicCString ,1,1,1,CCString>(pfstc,ConstCharString(sizeof(CCString)));
	Check4<DynamicCString ,1,6,6,CCString>(pfstc,ConstCharString(sizeof(CCString)));
	Check4<DynamicCString ,2,2,2,CCString>(pfstc,ConstCharString(sizeof(CCString)));
	Check4<DynamicCString ,5,5,5,CCString>(pfstc,ConstCharString(sizeof(CCString)));
	Check4<DynamicCString ,10,35,5,CCString>(pfstc,ConstCharString(sizeof(CCString)));
	Check4<DynamicCString ,515,235,5,CCString>(pfstc,ConstCharString(sizeof(CCString)));

	TestSafeN2N<float,int20>(1e6);
    TestSafeN2N<float,int21>(1e6);
    TestSafeN2N<float,uint19>(1e6);
    TestSafeN2N<float,uint20>(1e6);
    TestSafeN2N<float,int20>(-1e6);
    TestSafeN2N<float,int21>(-1e6);
    TestSafeN2N<uint30,int27>(1000000000);
    TestSafeN2N<uint35,int32>(4000000000);
    TestSafeN2N<uint35,uint32>(4000000000);
    TestSafeN2N<int35 ,uint32>(4000000000);
    TestSafeN2N<int35 ,float>(4000000000);
    TestSafeN2N<int35 ,double>(4000000000);
    TestSafeN2N<double,uint17>(4000000000);

    TestSatInteger(1e4,1e4,5000,4000,SaturatedInteger<uint32>(uint32(100001000)));
    TestSatInteger(1e4,1e4,5000,4000,SaturatedInteger<uint16>(uint32(100001000)));



    printf ("%i %i %le \n",TypeCharacteristics<double>::UsableBitSize(),DBL_MAX_EXP,TypeCharacteristics<double>::MaxValue());
    printf ("%i %le \n",TypeCharacteristics<float>::UsableBitSize(),TypeCharacteristics<float>::MaxValue());
    printf ("%i %i \n",TypeCharacteristics<uint17>::UsableBitSize(),TypeCharacteristics<uint17>::MaxValue());

    int32 temp;
	AnyType xx;
	Reference zz;

	PrintType(temp);printf("\n");
	PrintType(zz);printf("\n");
	PrintType(xx);printf("\n");

	xx = temp;
	PrintType(xx);printf("\n");

	ReferenceT<AnyObjectT<9>> ao8(buildNow);
	ao8->Setup(SignedInteger32Bit,"",&temp,sizeof(temp));

	xx = ao8;
	PrintType(xx);printf("\n");
    zz = ao8;
    xx = zz;
	PrintType(xx);printf("\n");
}
}


int main(int argc, char **argv){
    MARTe::PrepareTestObject();
    MARTe::Test();
    return 0;
}
