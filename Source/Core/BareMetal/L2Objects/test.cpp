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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

void printType(AnyType x){
    VariableDescriptor vd = x.GetFullVariableDescriptor();

    char8 token = 0;
    bool constant = false;
    uint32 size;
    uint32 depth = 0;
    vd.GetModifier(token,constant,size,depth);
    printf ("Mod={");
    char buffer[200];
    buffer[0] = 0;
    char *pbuf = &buffer[0];
    while (token != 0) {
        depth++;
        if (constant) pbuf += sprintf (pbuf,"C ");
        switch (token){
        case 'A':{
        	pbuf += sprintf (pbuf,"[%i]",size);
        }break;
        case 'P':{
        	pbuf += sprintf (pbuf,"* ");
        }break;
        default :{
        	pbuf += sprintf (pbuf,"%c ",token);
        }break;
        }
        vd.GetModifier(token,constant,size,depth);
    } ;
    printf ("%-12s}",buffer);

    TypeDescriptor td = vd.GetFullTypeDescriptor();
    if (td.isConstant){
        printf ("const ");
    }
    if (td.isStructuredData){
        printf ("S(%i) ",(int)td.structuredDataIdCode);
    } else {
        if (td.IsBitType()){
            printf (" %s%i:%i ",BasicTypeName(td.type),(int)td.numberOfBits,(int)td.bitOffset);
        } else {
            switch(td.arrayType){
            case ZeroTermArray:{
                printf (" %s%i Z",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case DynamicZeroTermArray:{
                printf (" %s%i DZ",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case StaticZeroTermArray:{
                printf (" %s%i SZ",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            case Array1D:{
                printf (" %s%i [%i]",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize),(int)td.arraySize);
            }break;
            case Array2D:{
                printf (" %s%i [%i][%i]",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize),(int)td.numberOfRows,(int)td.numberOfColumns);
            }break;
            case ArrayLarge:{
                printf (" %s%i [%i][...]",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize),(int)td.arraySize);
            }break;
            default:
            case ArrayUnknown:{
                printf (" %s%i ?",BasicTypeName(td.type),BitsFromBasicObjectSize((int)td.objectSize));
            }break;
            }
        }

    }
    printf("\n");

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

	/*
	 * used as a pointer to avoid allocating unnecessary stack memory
	 */
    T *x;
    const int pappa = 0;
    /* assigned to a real pointer to avoid problems */
    x = reinterpret_cast<T*>(reinterpret_cast<int>(&pappa));

    AnyType at(*x);

    printf("%-26s ",orig.GetList());

    printType(at);


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
    TEST(uint32 const **);
    TEST(uint32  * const *);
    TEST(uint32  * * );
    TEST(uint32***);
    TEST(uint32[2]);
    TEST(uint32[21]);
    TEST(uint32[128]);
    TEST(uint32[12345]);
    TEST(uint32[32769]);
    TEST(uint32[132000]);
    TEST(uint32[1320000]);
    TEST(uint32[32][20][17]);
    TEST(uint32 (*  )[32]);
    TEST(uint32 *[32]);

    TEST(CString);
    TEST(CCString);
    TEST(DynamicCString);
    TEST(StaticCString<32>);

    TEST(ZeroTerminatedArray<float>);

    TEST(pippone);
    TEST(const pippone2 *);

}

}



int main(int argc, char **argv){

MARTe::testAT();


return 0;
}

