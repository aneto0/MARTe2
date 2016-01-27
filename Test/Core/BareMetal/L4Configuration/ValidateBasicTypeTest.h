/**
 * @file ValidateBasicTypeTest.h
 * @brief Header file for class ValidateBasicTypeTest
 * @date 26/01/2016
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

 * @details This header file contains the declaration of the class ValidateBasicTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VALIDATEBASICTYPETEST_H_
#define VALIDATEBASICTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ValidateBasicType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
using namespace MARTe;

template<typename T>
struct ValidateBasicTypeTestTable {
    const char8 * attributes;
    T value;
    bool expected;
};

class ValidateBasicTypeTest {
public:

    template<typename T>
    bool TestValidateBasicType(const ValidateBasicTypeTestTable<T>* table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool ValidateBasicTypeTest::TestValidateBasicType(const ValidateBasicTypeTestTable<T>* table){

    uint32 i=0;
    while(table[i].attributes!=NULL){
        AnyType at(table[i].value);
        if(ValidateBasicType(at, table[i].attributes)!=table[i].expected){
            printf("\n%d\n",i);
            return false;
        }
        i++;
    }

    return true;
}

#endif /* VALIDATEBASICTYPETEST_H_ */

