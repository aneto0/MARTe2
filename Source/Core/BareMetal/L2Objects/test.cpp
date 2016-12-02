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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

void printType(AnyType x){
//    printf("[\n");
    VariableDescriptor vd = x.GetVariableDescriptor();
#if 0
    char8 *rawmod = vd.GetRawModifiers();

    if (rawmod != NULL)
    while (*rawmod != 0){
        printf("%02x ",*rawmod);
        rawmod++;
    }
    printf("(END)\n");
    return;
#endif

    char8 token = 0;
    bool constant = false;
    uint32 size;
    uint32 depth = 0;
    vd.GetModifier(token,constant,size,depth);
    while (token != 0) {
//        printf ("(%i)",depth);
        depth++;
        if (constant) printf ("C ");
        switch (token){
        case 'A':{
            printf ("[%i]",size);
        }break;
        default :{
            printf ("%c ",token);
        }break;
        }
        vd.GetModifier(token,constant,size,depth);
    } ;

    TypeDescriptor td = vd.GetTypeDescriptor();
    if (td.isConstant){
        printf ("C-");
    }
    if (td.isStructuredData){
        printf ("S(%i) \n",(int)td.structuredDataIdCode);
    } else {
        if (td.IsBitType()){
            printf ("btype = %i-%i-%i \n",(int)td.type,(int)td.numberOfBits,(int)td.bitOffset);
        } else {
            switch(td.arrayType){
            case 0:{
                printf ("Btype = %i-%i Z\n",(int)td.type,(int)td.numberOfBytes);
            }break;
            case 1:{
                printf ("Btype = %i-%i [%i]\n",(int)td.type,(int)td.numberOfBytes,(int)td.arraySize);
            }break;
            case 2:{
                printf ("Btype = %i-%i [%i][%i]\n",(int)td.type,(int)td.numberOfBytes,(int)td.numberOfRows,(int)td.numberOfColumns);
            }break;
            case 3:{
                printf ("Btype = %i-%i [%i][...]\n",(int)td.type,(int)td.numberOfBytes,(int)td.arraySize);
            }break;
            }
        }

    }

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
    T x;

    AnyType at(x);

    printf("%s ==> ",orig.GetList());

    printType(at);


}
#define TEST(x)  testT<x>(#x)



void testAT(){
    TEST(uint64);
    TEST(uint48);
    TEST(uint32);
    TEST(const uint24);
    TEST(uint16);
    TEST(uint8);
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

    TEST(ZeroTerminatedArray<float>);

    TEST(pippone);
    TEST(const pippone2 *);

}

}



int main(int argc, char **argv){

MARTe::testAT();


return 0;
}

