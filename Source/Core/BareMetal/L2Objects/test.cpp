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
        printf ("%sS(%i) ",consts,(int)td.structuredDataIdCode);
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

struct pippone{

    int pillo;
    int pollo;
};

struct pippone2{

    int pillo[32];
    int pollo;
};

template <class T>
void testT(CCString orig){

	printf("------------------\n");

	/*
	 * used as a pointer to avoid allocating unnecessary stack memory
	 */
    T *x ;
    void *pappa[4] = /*{NULL,NULL,NULL,NULL};*/{&pappa[1],&pappa[2],&pappa[3],NULL};
    /* assigned to a real pointer to avoid problems */
    x = reinterpret_cast<T*>(reinterpret_cast<uintp>(&pappa));

    AnyType at(*x);

    bool sameAddress = (at.GetVariablePointer() == &pappa);
    if (sameAddress) {
    	printf("Address Ok :");
    }
    else {
    	printf("(%08p-%08p) ",at.GetVariablePointer(),&pappa);
    }


    printf("%-26s ",orig.GetList());

    printType(at);

    ErrorManagement::ErrorType ret;
    while (ret = at.Dereference()){
        printType(at);
    }

    ErrorManagement::ErrorTypeLookup *etl = &ErrorManagement::errorTypeLookup[0];
    while (!etl->name.IsNullPtr()){
    	if ((etl->errorBitSet &  ret.format_as_integer)!= 0){
    		printf("%s\n",etl->name);
    	}
    	etl++;
    }


}


#define TEST(x)  testT<x>(#x)



void testAT(){
    TEST(double);
    TEST(float);
    TEST(uint64);
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
    TEST(uint32 const *);
    TEST(uint32 *const);
    TEST(uint32  * * );
    TEST(uint32 const **);
    TEST(uint32  * const *);
    TEST(uint32  * * const );
    TEST(uint32***);
    TEST(uint32** const *);
    TEST(uint32 const * const * const * const * const );
    TEST(uint32******);
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
    TEST(Vector<int32 *>);
    TEST(Vector<int32> * const );


    TEST(pippone);
    TEST(const pippone2 *);

}

}


int pluto;

int main(int argc, char **argv){

	int pippo;
	float *x = (float *)0x123;


	printf ("%016p  %i\n", &pluto, MARTe::MemoryCheck::Check(&pluto));
	printf ("%016p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo));
	printf ("%016p  %i\n", &pippo, MARTe::MemoryCheck::Check(&pippo,MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%016p  %i\n", &main, MARTe::MemoryCheck::Check(&main));
	printf ("%016p  %i\n", &main, MARTe::MemoryCheck::Check(&main,MARTe::MemoryCheck::ExecuteAccessMode));
	printf ("%016p  %i\n", x, MARTe::MemoryCheck::Check(x));


MARTe::testAT();


return 0;
}

