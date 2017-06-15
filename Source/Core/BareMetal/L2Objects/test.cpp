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
        if (cri != NULL) printf ("%s ",cri->GetClassName().GetList());
        else printf ("unknown_struct_code(%i) ",(int)td.structuredDataIdCode);

    } else {
    	if (td.IsComplexType()){
            printf ("%s%s ",consts,BasicTypeName(td.type,td.complexType).GetList());
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
    printf ("@%p{%-12s}",address,mods.GetList());
    td = vd.GetFullTypeDescriptor();
    printTypeDescriptor(td);
    printf(" size = %Li\n",vd.GetSize());

}

/*
static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}
*/

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
//	printf("kkkkk %0x\n",at.GetFullVariableDescriptor().GetFullTypeDescriptor().all);

    if (deref.GetSize() > 0){
        printf("(%-24s)%s",orig.GetList(),deref.GetList());

        ok = at.MultipleDereference(deref);

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
        		printf("%s\n",etl->name.GetList());
        	}
        	etl++;
        }
    }

    return ok;
}





struct testStruct3{
	float 		fieldA;
	CCString	fieldB;
};

struct testStruct6{
	float 		fieldC;
	CCString	fieldD;
};

struct testStruct5: testStruct6{
	int 		*fieldE;
	int         fieldF;
};

struct testStruct4: testStruct3,testStruct5{
	float 		fieldG[2][3];
	DynamicCString	fieldH;
};


struct testStruct2{
    char8         fieldA[3];
    const uint8   fieldB;
     int16       *fieldC;
    uint32        fieldD;
     int64        fieldE;
    Vector<float> fieldF;
    float*        fieldG;
    CString       fieldH;
};


struct testStruct{
    char8         fieldA;
    const uint8   fieldB;
     int16        fieldC;
    uint32        fieldD;
     int64        fieldE;
    float         fieldF;
    float*        fieldG;
    CString       fieldH;
    testStruct   *fieldI;
    testStruct2   fieldJ;
    Matrix<testStruct3>   fieldK;
    testStruct4   fieldL;
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

    TEST(testStruct);
    TEST2(testStruct,.fieldA);
    TEST2(testStruct,.fieldB);
    TEST2(testStruct,.fieldC);
    TEST2(testStruct,.fieldD);
    TEST2(testStruct,.fieldE);
    TEST2(testStruct,.fieldF);
    TEST2(testStruct,.fieldG);
    TEST2(testStruct,.fieldH);
    TEST2(testStruct,.fieldI);
    TEST2(testStruct,.fieldJ);
    TEST2(testStruct,.fieldK);
    TEST2(testStruct,.fieldL);
    TEST2(testStruct,.fieldH[0]);
    TEST2(testStruct,.fieldH[1]);
    TEST2(testStruct,.fieldH[2]);
    TEST2(testStruct,.fieldH[22]);
    TEST2(testStruct,.fieldI*);
    TEST2(testStruct,.fieldI->fieldA);
    TEST2(testStruct,.fieldI->fieldB);
    TEST2(testStruct,.fieldI->fieldC);
    TEST2(testStruct,.fieldI->fieldD);
    TEST2(testStruct,.fieldI->fieldE);
    TEST2(testStruct,.fieldI->fieldF);
    TEST2(testStruct,.fieldI->fieldG);
    TEST2(testStruct,.fieldI->fieldH);
    TEST2(testStruct,.fieldI->fieldI);
    TEST2(testStruct,.fieldJ.fieldA);
    TEST2(testStruct,.fieldJ.fieldB);
    TEST2(testStruct,.fieldJ.fieldC);
    TEST2(testStruct,.fieldJ.fieldD);
    TEST2(testStruct,.fieldJ.fieldE);
    TEST2(testStruct,.fieldJ.fieldF);
    TEST2(testStruct,.fieldJ.fieldG);
    TEST2(testStruct,.fieldJ.fieldH);
    TEST2(testStruct,.fieldK[0]);
    TEST2(testStruct,.fieldK[0][0]);
    TEST2(testStruct,.fieldK[0][0].fieldA);
    TEST2(testStruct,.fieldK[0][0].fieldB);
    TEST2(testStruct,.fieldI->fieldI->fieldA);
    TEST2(testStruct,.fieldI->fieldI->fieldB);
    TEST2(testStruct,.fieldI->fieldI->fieldC);
    TEST2(testStruct,.fieldI->fieldI->fieldD);
    TEST2(testStruct,.fieldI->fieldI->fieldE);
    TEST2(testStruct,.fieldI->fieldI->fieldF);
    TEST2(testStruct,.fieldI->fieldI->fieldG);
    TEST2(testStruct,.fieldI->fieldI->fieldH);
    TEST2(testStruct,.fieldI->fieldI->fieldI);
    TEST2(testStruct,.fieldL.fieldA);
    TEST2(testStruct,.fieldL.fieldB);
    TEST2(testStruct,.fieldL.fieldC);
    TEST2(testStruct,.fieldL.fieldD);
    TEST2(testStruct,.fieldL.fieldE);
    TEST2(testStruct,.fieldL.fieldF);
    TEST2(testStruct,.fieldL.fieldG);
    TEST2(testStruct,.fieldL.fieldH);

}


/**************************************/


CLASS_MEMBER_REGISTER(testStruct,fieldA)
CLASS_MEMBER_REGISTER(testStruct,fieldB)
CLASS_MEMBER_REGISTER(testStruct,fieldC)
CLASS_MEMBER_REGISTER(testStruct,fieldD)
CLASS_MEMBER_REGISTER(testStruct,fieldE)
CLASS_MEMBER_REGISTER(testStruct,fieldF)
CLASS_MEMBER_REGISTER(testStruct,fieldG)
CLASS_MEMBER_REGISTER(testStruct,fieldH)
CLASS_MEMBER_REGISTER(testStruct,fieldI)
CLASS_MEMBER_REGISTER(testStruct,fieldJ)
CLASS_MEMBER_REGISTER(testStruct,fieldK)
CLASS_MEMBER_REGISTER(testStruct,fieldL)
CLASS_MEMBER_REGISTER(testStruct2,fieldA)
CLASS_MEMBER_REGISTER(testStruct2,fieldB)
CLASS_MEMBER_REGISTER(testStruct2,fieldC)
CLASS_MEMBER_REGISTER(testStruct2,fieldD)
CLASS_MEMBER_REGISTER(testStruct2,fieldE)
CLASS_MEMBER_REGISTER(testStruct2,fieldF)
CLASS_MEMBER_REGISTER(testStruct2,fieldG)
CLASS_MEMBER_REGISTER(testStruct2,fieldH)

CLASS_MEMBER_REGISTER(testStruct3,fieldA)
CLASS_MEMBER_REGISTER(testStruct3,fieldB)
CLASS_MEMBER_REGISTER(testStruct6,fieldC)
CLASS_MEMBER_REGISTER(testStruct6,fieldD)
CLASS_INHERIT_REGISTER(testStruct5,testStruct6)
CLASS_MEMBER_REGISTER(testStruct5,fieldE)
CLASS_MEMBER_REGISTER(testStruct5,fieldF)

CLASS_INHERIT_REGISTER(testStruct4,testStruct3)
CLASS_INHERIT_REGISTER(testStruct4,testStruct5)
CLASS_MEMBER_REGISTER(testStruct4,fieldG)
CLASS_MEMBER_REGISTER(testStruct4,fieldH)

/**************************************/


}

void testOther();
int pluto;

int main(int argc, char **argv){

	testOther();
return 0;
	int pippo;
	float *x = (float *)0x123;

    //printf("%0x\n",MARTe::TypeDescriptor(TDRANGE(type,MARTe::SignedInteger)    | TDRANGE(objectSize,MARTe::Size32bit)   | TDRANGE(arrayProperty, MARTe::SizedCArray_AP) | TDRANGE(arraySize, 1)).all);

	printf ("%16p  %i\n", &pluto, MARTe::MemoryCheck::Check(&pluto));
	printf ("%16p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo));
	printf ("%16p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo,MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%16p  %i\n", &main, MARTe::MemoryCheck::Check(reinterpret_cast<void *>(&main)));
	printf ("%16p  %i\n", &main, MARTe::MemoryCheck::Check(reinterpret_cast<void *>(&main),MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%16p  %i\n", x, MARTe::MemoryCheck::Check(x));


MARTe::testAT();

#include "CompilerTypes.h"


printf("%i %i %i %i %i %i %i %i %i\n ",
		(int)indexof(MARTe::testStruct,fieldA),
		(int)indexof(MARTe::testStruct,fieldB),
		(int)indexof(MARTe::testStruct,fieldC),
		(int)indexof(MARTe::testStruct,fieldD),
		(int)indexof(MARTe::testStruct,fieldE),
		(int)indexof(MARTe::testStruct,fieldF),
		(int)indexof(MARTe::testStruct,fieldG),
		(int)indexof(MARTe::testStruct,fieldH),
		(int)indexof(MARTe::testStruct,fieldI)
		);

printf ("%i %i %i\n",
        (int)ancestorIndexof(MARTe::testStruct4,MARTe::testStruct3),
	(int)ancestorIndexof(MARTe::testStruct4,MARTe::testStruct5),
	(int)ancestorIndexof(MARTe::testStruct4,MARTe::testStruct6));

return 0;
}


//#include "IndexedReferenceContainer.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "CCString.h"
#include "BalancedTreeHolder.h"


struct RefToName{
	MARTe::CCString operator()( MARTe::Reference & ref){
		MARTe::CCString ret = "empty";
		if (ref.IsValid()){
			ret = ref->GetName();
		}
		return ret;
	}
};


void printTree(MARTe::BalancedTreeHolder<MARTe::Reference,RefToName> &tree){
	int size = tree.Size();
printf("Tree Size = %i %i\n",tree.Size(),tree.Depth());
	for (int i = 0;i< size;i++){
		MARTe::BalancedTreeNodeT<MARTe::Reference,RefToName> *p = tree.Seek(i);

		printf("%03i ",i);
		if (p != NULL){
			for (int j=0;j<(tree.Depth()-p->Depth());j++) printf(" -> ");
			printf("%s\n",p->GetData()->GetName());
		} else {
			printf("NULL\n");
		}


	}

}

class DummyObject:public MARTe::Object{
public:
	CLASS_REGISTER_DECLARATION()


};

CLASS_REGISTER(DummyObject, "1.0")


void testOther(){
	MARTe::BalancedTreeHolder<MARTe::Reference,RefToName> bth;

	const MARTe::uint32 refsSz = 1000*1000;
	const MARTe::uint32 maxR   = 256*256;
	int i;

//	MARTe::ReferenceT<DummyObject> refs[refsSz];

//	bool indexes[maxR];
//	for (i = 0;i< maxR; i++) indexes[i] = false;

	for (int i = 0;i< refsSz;i++){
		char buffer[256];
		MARTe::uint32 r = rand() & (maxR-1);
//		if (indexes[r]==true){
//			r =0;
//			while ((r<maxR) && (indexes[r]))r++;
//		}

//		indexes[r] = true;

		sprintf(buffer,"%05x%05x",r,i);
		//printf("%03i>>%s \n",i,buffer);
		MARTe::ReferenceT<DummyObject> refd(new DummyObject());
		refd->SetName(buffer);
		bth.Insert(refd);

//		printTree(bth);
		if (bth.Size() < i){
			break;
		}

//		printTree(bth);
	}

	printTree(bth);

//	MARTe::IndexedReferenceContainerRoot<24> ircr;
}

