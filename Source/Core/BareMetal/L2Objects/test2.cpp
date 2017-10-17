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


void printType(const AnyType &at){
    const VariableDescriptor &vd = at.GetFullVariableDescriptor();

    MARTe::DynamicCString line;
    bool ret = vd.ToString(line);
    printf("%s",line.GetList());
    line.Truncate(0);
    ret = vd.ToString(line,true);
    printf("  {%s}",line.GetList());

    uint64 dataSize;
    uint64 storageSize;
    vd.GetSize(reinterpret_cast<const uint8 *>(at.GetVariablePointer()),dataSize, &storageSize);

    int sz = dataSize;
    int stsz = storageSize;
    printf(" sz = %i stsz = %i\n",sz,stsz);
}


template <class T>
ErrorManagement::ErrorType testDerefT(CCString orig,CCString deref=""){

	ErrorManagement::ErrorType ok;

	printf("------------------\n");

	/*
	 * used as a pointer to avoid allocating unnecessary stack memory
	 */
    T *x ;
    // build a memory block made with pointers pointing to the next pointer...
    const uint32 NS = 128;
    void *memory[NS];
    uint32 i;
    for (i=0;i<(NS-1);i++){
    	memory[i]=&memory[i+1];
    }
    memory[NS-1]=NULL;

    /* assigned to a real pointer to avoid problems */
    x = reinterpret_cast<T*>(reinterpret_cast<uintp>(&memory));

    AnyType at(*x);
//printf("!1");

    if (deref.GetSize() > 0){
        printf("(%-24s)%s",orig.GetList(),deref.GetList());

        ok = at.MultipleDereference(deref);

    } else {
        printf("%-26s ",orig.GetList());
    }

//printf("!2");
    if (ok){
        bool sameAddress = (at.GetVariablePointer() == &memory);
        if (sameAddress) {
        	printf("[Address same] ");
        }
        else {
        	int64 delta =  reinterpret_cast<const char8 *>(at.GetVariablePointer()) - reinterpret_cast<const char8  *>(&memory[0]);
        	printf("[Address delta(B)= %Li] ",delta);
        }
    	printf("\n");
//printf("!3");
        printType(at);
    } else {
        ErrorManagement::ErrorTypeLookup *etl = &ErrorManagement::errorTypeLookup[0];
        while (!etl->name.IsNullPtr()){
        	if ((etl->errorBitSet &  ok.format_as_integer)!= 0){
        		printf("%s\n",etl->name.GetList());
        	}
        	etl++;
        }
    }

    return ok;
}

#define varDeclSubTemplate(type,typeName,className)\
	type  typeName;

#define memberDeclSubTemplate(type,typeName,className)\
	CLASS_MEMBER_REGISTER(className,typeName)

#define Test1ClassTemplate(subTemplate,className)\
	subTemplate(int8,int8Var,className)\
	subTemplate(int16,int16Var,className)\
	subTemplate(int32,int32Var,className)\
	subTemplate(int64,int64Var,className)\
	subTemplate(uint32,uint32Var,className)\
	subTemplate(char8,char8Var,className)\
	subTemplate(float,floatVar,className)\
	subTemplate(double,doubleVar,className)\
	subTemplate(int32 *,int32PVar,className)\
    subTemplate(CCString,CCStringVar,className)\
	subTemplate(CString,CStringVar,className)\
	subTemplate(DynamicCString,DCStringVar,className)

class Test1Class{
public:

	Test1ClassTemplate(varDeclSubTemplate,Test1Class)
} test1Class;

Test1ClassTemplate(memberDeclSubTemplate,Test1Class)


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
	test1Class.CCStringVar = "pippo0";
	test1Class.CStringVar  = "pippo1";
	test1Class.DCStringVar = "pippo2";
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

	printf ("% 20s ->",expression.GetList());

	err = at.MultipleDereference(expression);
    DynamicCString string;
    DynamicCString string2;
	if (err){
	    const VariableDescriptor &vd =  at.GetFullVariableDescriptor();
	    err = vd.ToString(string);
	    vd.ToString(string2,true);
	}

	if (err){
	    err.fatalError = !string.isSameAs(returnType.GetList());
	}

	if (err){
	    printf("OK  %s= %s\n",string.GetList(),returnType.GetList());
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
    Check(at,".int32PVar","int32 *");
    Check(at,".int32PVar*","int32");
    Check(at,".CCStringVar","CCString");
    Check(at,".CStringVar","CString");
    Check(at,".DCStringVar","DynamicCString");

}
}


int main(int argc, char **argv){
	MARTe::PrepareTestObject();
	MARTe::Test();
	return 0;
}
