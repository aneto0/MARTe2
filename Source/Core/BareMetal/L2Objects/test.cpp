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


void printTypeDescriptor(const TypeDescriptor &td){
    const char8 *consts= "";
    if (td.dataIsConstant){
        //printf ("const ");
    	consts = "const ";
    }
    if (td.isStructuredData){
        printf ("%s",consts);
//        printf ("%sS(%i) ",consts,(int)td.structuredDataIdCode);
   		ClassRegistryItem * cri = ClassRegistryDatabase::Instance()->Find(td);
        if (cri != NULL) printf ("%s ",cri->GetClassName());
        else printf ("unknown_struct_code(%i) ",(int)td.structuredDataIdCode);

    } else {
    	if (td.IsComplexType()){
            printf ("%s%s ",consts,BasicTypeName(td.type,td.complexType));
    	} else
        if (td.IsBitType()){
            printf ("%s%s%i:%i ",consts,BasicTypeName(td.type,0).GetList(),(int)td.numberOfBits,(int)td.bitOffset);
        } else
        if (td.arrayProperty == SizedCArray_AP){
           	if (td.arraySize == 0){
           		printf ("%s%s%i [>2M]",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
           	} else
           	if (td.arraySize == 1){
           		printf ("%s%s%i",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
           	} else
           	{
           		printf ("%s%s%i [%i]",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize),(int)td.arraySize);
           	}
        } else
        if (td.arrayProperty == StaticZeroTermArray_AP){
           	if (td.arraySize == 0){
                printf ("StaticZeroTerminatedArray<%s%s%i,>2M>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
           	} else {
                printf ("StaticZeroTerminatedArray<%s%s%i,%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize),(int)td.arraySize);
           	}

        } else
        if (td.arrayProperty == ConstStaticZeroTermArray_AP){
            printf ("const StaticZeroTerminatedArray<%s%s%i,%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize),(int)td.arraySize);
        } else {
        	uint32 temp = td.combinedArrayType;
//        	printf("%x\n",temp);
        	switch(temp){
            case ZeroTermArray:{
                printf ("ZeroTerminatedArray<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case ConstZeroTermArray:{
                printf ("const ZeroTerminatedArray<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case DynamicZeroTermArray:{
                printf ("DynamicZeroTerminatedArray<%s%i>",BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case ConstDynamicZeroTermArray:{
                printf ("const DynamicZeroTerminatedArray<%s%i>",BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case Array1D:{
             	printf ("Vector<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case ConstArray1D:{
             	printf ("const Vector<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case Array2D:{
                printf ("Matrix<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case ConstArray2D:{
                printf ("const Matrix<%s%s%i>",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case PointerArray:{
                printf ("(%s%s%i)* ",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case ConstPointerArray:{
                printf ("(%s%s%i)* const",consts,BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            default:
            case ArrayUnknown:{
                printf (" %s%i ?",BasicTypeName(td.type,0).GetList(),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            }
        }
    }

}

void printType(const AnyType &at){
    printf("\n");
    const VariableDescriptor &vd = at.GetFullVariableDescriptor();
    TypeDescriptor td;

	uint32 index=0;
    while (vd.GetTopTypeDescriptor(td,index++)){
    	printTypeDescriptor (td);
    }
    printf("\n");

    const void *address = at.GetVariablePointer();
    CCString mods = vd.GetModifierString();
    printf ("@%0p{%-12s}",address,mods.GetList());
    td = vd.GetFullTypeDescriptor();
    printTypeDescriptor(td);
    printf(" size = %Li\n",vd.GetSize());

}


static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
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
    const uint32 NS = 4;
    void *memory[NS];
    int i;
    for (i=0;i<(NS-1);i++){
    	memory[i]=&memory[i+1];
    }
    memory[NS-1]=NULL;

    /* assigned to a real pointer to avoid problems */
    x = reinterpret_cast<T*>(reinterpret_cast<uintp>(&memory));

    AnyType at(*x);
//	printf("kkkkk %0x\n",at.GetFullVariableDescriptor().GetFullTypeDescriptor().all);

    if (deref.GetSize() > 0){
        printf("(%-24s)%s",orig.GetList(),deref.GetList());
#if 0
    	while ((deref.GetSize()>0) && (ok)){
    		DynamicCString token;
        	deref = StringHelper::Tokenize(deref,token, CCString("*."),CCString(". "),true);
        	if (token.GetSize() > 0){
            	if (token == CCString("*")){
            		ok = at.Dereference(0);
            	} else {
            		ok = at.Dereference(token);
            	}
        	}
    	}
#else
    	enum DerefStatus {
    		Normal,
    		Member,
    		Matrix,
    		MatrixDone
    	}  status = Normal;

    	while ((deref.GetSize()>0) && (ok)){
    		DynamicCString token;
    		int32 term  =-1;
    		CCString dels[6] = {CCString("."),CCString("*"),CCString("->"),CCString("["),CCString("]"),CCString()};
    		const ZeroTerminatedArray<CCString> delimiters = dels;
        	deref = StringHelper::Tokenize(deref,token,term, delimiters,CCString(". "));

        	// process token
        	if (token.GetSize() > 0){
            	if (isNumber(token[0])){
            		if (status == Matrix ){
                		ok = at.Dereference(atoi(token.GetList()));
                		if (ok) {
                			status = MatrixDone;
                		}
            		}
            	} else {
            		if (status == Member){
            			ok = at.Dereference(token);
                		if (ok) {
                			status = Normal;
                		}
            		}
            	}
        	}
        	// process term
        	if (ok){
            	switch(term){
            	case 0:{ // .
            		if (status == Normal){
            			status = Member;
            		} else ok = false;
            	} break;
            	case 1:{ // *
            		if (status == Normal){
            			ok = at.Dereference(0);
            		} else ok = false;
            	} break;
            	case 2:{ // ->
            		if (status == Normal){
            			ok = at.Dereference(0);
            			if (ok){
            				status = Member;
            			}
            		} else ok = false;
            	} break;
            	case 3:{ // [
            		if (status == Normal){
            			status = Matrix;
            		} else ok = false;
            	} break;
            	case 4:{ // ]
            		if (status == MatrixDone){
            			status = Normal;
            		} else ok = false;
            	} break;
            	default:{

            	}
            	}
        	}
    	}

#endif

    } else {
        printf("%-26s ",orig.GetList());
    }

    if (ok){
        bool sameAddress = (at.GetVariablePointer() == &memory);
        if (sameAddress) {
        	printf("[Address same] ");
        }
        else {
        	int64 delta =  reinterpret_cast<const char8 *>(at.GetVariablePointer()) - reinterpret_cast<const char8  *>(&memory[0]);
        	printf("[Address delta(B)= %Li] ",delta);
        }
        printType(at);
    } else {
        ErrorManagement::ErrorTypeLookup *etl = &ErrorManagement::errorTypeLookup[0];
        while (!etl->name.IsNullPtr()){
        	if ((etl->errorBitSet &  ok.format_as_integer)!= 0){
        		printf("%s\n",etl->name);
        	}
        	etl++;
        }
    }

    return ok;
}


struct fregna;

struct pippone{
    int32 pilla;
    double polla;
    fregna *pp;
};

struct fregna{
    int32 pillo[4];
    pippone pollo;
    pippone *ff;

};






#define TEST(x)     testDerefT<x>(#x)
#define TEST2(x,y)  testDerefT<x>(#x,#y)

void testAT(){
    TEST(double);
    TEST(float);
    TEST(uint64);
    TEST2(uint64,*);
    TEST(uint32);
    TEST(uint16);
    TEST(uint8);
    TEST(int64);
    TEST(int32);
    TEST(int16);
    TEST(int8);
    TEST(uint48);
    TEST(const uint24);
    TEST(uint4);

    TEST(uint32*);
    TEST2(uint32*,*);
    TEST2(uint32*,**);
    TEST2(uint32*,***);

    TEST(uint32 const *);
    TEST(uint32 *const);
    TEST(uint32  * * );
    TEST(uint32 const **);
    TEST(uint32  * const *);
    TEST(uint32  * * const );
    TEST(uint32***);
    TEST(uint32** const *);
    TEST(uint32 const * const * const * const * const );
    TEST2(uint32 const * const * const * const * const,* );
    TEST2(uint32 const * const * const * const * const,** );
    TEST2(uint32 const * const * const * const * const,*** );
    TEST2(uint32 const * const * const * const * const,**** );
    TEST2(uint32 const * const * const * const * const,***** );
    TEST(uint32******);
    TEST2(uint32******,*);
    TEST2(uint32******,**);
    TEST2(uint32******,***);
    TEST2(uint32******,****);
    TEST2(uint32******,*****);
    TEST2(uint32******,******);
    TEST(uint32[2]);
    TEST(uint32[21]);
    TEST(uint32[128]);
    TEST(uint32[12345]);
    TEST(uint32[32769]);
    TEST(uint32[132000]);
    TEST(uint32[1320000]);
    TEST(uint32[32][20][17]);
    TEST(const uint32[32][20][17][23]);
    TEST(uint32 (*  )[32]);
    TEST(uint32 *[32]);

    TEST(CString);
    TEST(CCString);
    TEST(DynamicCString);
    TEST(StaticCString<32>);
    TEST(ZeroTerminatedArray<float>);
    TEST(ZeroTerminatedArray<const int32>);
    TEST(Matrix<float>);
    TEST(Matrix<const float>);
    TEST(const Matrix<uint3>);
    TEST(Vector<int32>);
    TEST2(Vector<int32>,*);
    TEST2(Vector<int32>,[1]);
    TEST2(Vector<int32>,[2]);
    TEST2(Vector<int32>,**);
    TEST(Vector<int32 *>);
    TEST(Vector<int32> * const );

    TEST(pippone);
    TEST2(pippone,.pilla);
    TEST2(pippone,.polla);
    TEST2(pippone,.pp);
    TEST(fregna);
    TEST2(fregna,.pillo);
    TEST2(fregna,.pollo);
    TEST2(fregna,.ff);

    TEST2(pippone,.pp*);
    TEST2(pippone,.pp*.pollo);
    TEST2(pippone,.pp->pollo);
    TEST2(pippone,.pp->pollo.pp);
    TEST2(pippone,.pp->pollo.pp->pollo);
    TEST2(pippone,.pp->pollo.pp->pollo.pp);
    TEST2(pippone,.pp->pollo.pp->pollo.pp->pollo);
    TEST(const fregna *);

}


/**************************************/

CLASS_MEMBER_REGISTER(pippone,pilla)
CLASS_MEMBER_REGISTER(pippone,polla)
CLASS_MEMBER_REGISTER(pippone,pp)
CLASS_MEMBER_REGISTER(fregna,pillo)
CLASS_MEMBER_REGISTER(fregna,pollo)
CLASS_MEMBER_REGISTER(fregna,ff)


/**************************************/


}

int pluto;

int main(int argc, char **argv){

	int pippo;
	float *x = (float *)0x123;

    //printf("%0x\n",MARTe::TypeDescriptor(TDRANGE(type,MARTe::SignedInteger)    | TDRANGE(objectSize,MARTe::Size32bit)   | TDRANGE(arrayProperty, MARTe::SizedCArray_AP) | TDRANGE(arraySize, 1)).all);

	printf ("%016p  %i\n", &pluto, MARTe::MemoryCheck::Check(&pluto));
	printf ("%016p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo));
	printf ("%016p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo,MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%016p  %i\n", &main, MARTe::MemoryCheck::Check(&main));
	printf ("%016p  %i\n", &main, MARTe::MemoryCheck::Check(&main,MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%016p  %i\n", x, MARTe::MemoryCheck::Check(x));


MARTe::testAT();


return 0;
}

