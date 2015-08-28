/**
 * @file BitFieldTest.cpp
 * @brief Source file for class BitFieldTest
 * @date 27/ago/2015
 * @author pc
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
 * the class BitFieldTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BitFieldTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*
template<typename T, uint8 size, uint8 offset>
bool BitFieldTest<T, size, offset>::TestSum() {

    T maxValue = ~((T) 0);
    T value1 = 0;

    uint64 testValue1=0;
    uint64 testValue2=0;
    uint64 mask=(1<<(sizeof(T)*8))-1;

    for (T value1 = 0; value1 < maxValue; value1++) {
        for(T value2 =0 ; value2< maxValue; value2++){

            T sum=value1+value2;
            uint64 testSum=(testValue1+testValue2) & mask;

            if(sum != testSum){
                return false;
            }
            testValue2++;
        }
        testValue1++;
    }

    return true;
}



template<typename T, uint8 size, uint8 offset>
bool BitFieldTest<T, size, offset>::TestSubtraction() {

    T maxValue = ~((T) 0);
    T value1 = 0;

    uint64 testValue1=0;
    uint64 testValue2=0;
    uint64 mask=(1<<(sizeof(T)*8))-1;

    for (T value1 = 0; value1 < maxValue; value1++) {
        for(T value2 =0 ; value2< maxValue; value2++){

            T sum=value1+value2;
            uint64 testDiff=(testValue1-testValue2) & mask;

            if(sum != testDiff){
                return false;
            }
            testValue2++;
        }
        testValue1++;
    }

    return true;
}


*/








