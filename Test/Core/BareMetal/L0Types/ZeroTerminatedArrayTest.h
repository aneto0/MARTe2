/**
 * @file ZeroTerminatedArrayTest.h
 * @brief Header file for class ZeroTerminatedArrayTest
 * @date 21/01/2016
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

 * @details This header file contains the declaration of the class ZeroTerminatedArrayTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ZEROTERMINATEDARRAYTEST_H_
#define ZEROTERMINATEDARRAYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the ZeroTerminatedArray functions
 */
template<typename T>
class ZeroTerminatedArrayTest {

public:

    /**
     * @brief Test the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Checks if the [] operator returns the element in the correct position.
     */
    bool TestSubscriptOperator();

    /**
     * @brief Checks if the function returns the array size.
     */
    bool TestGetSize();

    /**
     * @brief Checks if the function returns the array pointer.
     */
    bool TestGetList();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool ZeroTerminatedArrayTest<T>::TestConstructor() {
	bool result = true;
	{
		T array[32]={0};
		ZeroTerminatedArray<T> test(array);
		result = (result && (test.GetList() == array));
	}
	{
		ZeroTerminatedArray<T> test;
		result = (result && (test.GetList() == NULL_PTR(T *)));
	}
	return result;
}

template<typename T>
bool ZeroTerminatedArrayTest<T>::TestSubscriptOperator() {
    T array[32]={0};
    T value = 0;
    for (uint32 i = 0; i < 31; i++) {
        array[i] = value;
        value++;
    }
    array[(uint32)31] = static_cast<T>(0);

    ZeroTerminatedArray<T> test(array);

    for (uint32 i = 0; i < 32; i++) {
        if (test[i] != array[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool ZeroTerminatedArrayTest<T>::TestGetSize() {
    T array[32]={0};
    ZeroTerminatedArray<T> test(array);

    for (uint32 i = 0; i < 31; i++) {
        test[i] = static_cast<T>(1);
    }
    test[(uint32)31] = static_cast<T>(0);
    return test.GetSize() == 31;
}

template<typename T>
bool ZeroTerminatedArrayTest<T>::TestGetList() {
    return TestConstructor();
}

#endif /* TEST_CORE_BAREMETAL_L0TYPES_ZEROTERMINATEDARRAYTEST_H_ */

