/**
 * @file BitBooleanTest.h
 * @brief Header file for class BitBooleanTest
 * @date 28/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class BitBooleanTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BITBOOLEANTEST_H_
#define BITBOOLEANTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BitBoolean.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

template<typename T>
class BitBooleanTest {
public:

    bool TestCopyOperatorUnion();

    bool TestBoolCast(T input);

    bool TestAnyTypeCast(T input);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

union UnionExample {

    uint32 intx1 :31;

    TypeDefinition::BitBoolean<uint64, 31> bitBool;

    uint64 intx3;

};

template<typename T>
bool BitBooleanTest<T>::TestCopyOperatorUnion() {
    UnionExample testShiftBool;
    testShiftBool.intx1 = 0x40000000;
    testShiftBool.bitBool = true;

    if ((testShiftBool.intx1 != 0x40000000) || (!testShiftBool.bitBool)) {
        return false;
    }

    testShiftBool.intx3 = 0xf0000000;
    testShiftBool.bitBool = false;

    return ((testShiftBool.intx1 == 0x70000000) && (!testShiftBool.bitBool) && (sizeof(UnionExample) == 8u));

}

template<typename T>
bool BitBooleanTest<T>::TestBoolCast(T input) {

    const uint8 max = sizeof(input) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    TypeDefinition::BitBoolean<T, zero> myZeroShiftedBool;
    myZeroShiftedBool = true;
    if (!myZeroShiftedBool) {
        return false;
    }

    myZeroShiftedBool = false;

    if (myZeroShiftedBool) {
        return false;
    }

    TypeDefinition::BitBoolean<T, half> myHalfShiftedBool;

    myHalfShiftedBool = true;
    if (!myHalfShiftedBool) {
        printf("\n%d %d\n", 1 << half, myHalfShiftedBool);
        return false;
    }

    myHalfShiftedBool = false;

    if (myHalfShiftedBool) {
        return false;
    }

    TypeDefinition::BitBoolean<T, max> myMaxShiftedBool;

    myMaxShiftedBool = true;
    if (!myMaxShiftedBool) {
        printf("\n%d %d\n", 1 << max, myMaxShiftedBool);
        return false;
    }

    myMaxShiftedBool = false;

    if (myMaxShiftedBool) {
        return false;
    }

    return true;

}

template<typename T>
bool BitBooleanTest<T>::TestAnyTypeCast(T input) {

    const uint8 max = sizeof(input) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    TypeDefinition::BitBoolean<T, zero> myZeroShiftedBool;
    myZeroShiftedBool = true;

    TypeDefinition::AnyType atTest=myZeroShiftedBool;

    if(atTest.GetDataPointer()!=(&myZeroShiftedBool)){
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest=atTest.GetTypeDescriptor();
    if(tdTest.isStructuredData || tdTest.isConstant || tdTest.typeInfo.type!=TypeDefinition::UnsignedInteger || tdTest.typeInfo.size!=1){
        return false;
    }


    if(atTest.GetBitAddress()!=zero){
        return false;
    }

    TypeDefinition::BitBoolean<T, half> myHalfShiftedBool;

    myHalfShiftedBool = true;
    atTest=myHalfShiftedBool;

    if(atTest.GetDataPointer()!=(&myHalfShiftedBool)){
        return false;
    }

    tdTest=atTest.GetTypeDescriptor();
    if(tdTest.isStructuredData || tdTest.isConstant || tdTest.typeInfo.type!=TypeDefinition::UnsignedInteger || tdTest.typeInfo.size!=1){
        return false;
    }


    if(atTest.GetBitAddress()!=half){
        return false;
    }

    TypeDefinition::BitBoolean<T, max> myMaxShiftedBool;

    myMaxShiftedBool = true;
    atTest=myMaxShiftedBool;

    if(atTest.GetDataPointer()!=(&myMaxShiftedBool)){
        return false;
    }

    tdTest=atTest.GetTypeDescriptor();
    if(tdTest.isStructuredData || tdTest.isConstant || tdTest.typeInfo.type!=TypeDefinition::UnsignedInteger || tdTest.typeInfo.size!=1){
        return false;
    }


    if(atTest.GetBitAddress()!=max){
        return false;
    }


    return true;
}

#endif /* BITBOOLEANTEST_H_ */

