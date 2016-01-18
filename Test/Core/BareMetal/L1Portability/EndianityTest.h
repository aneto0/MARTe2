/**
 * @file EndianityTest.h
 * @brief Header file for class EndianityTest
 * @date 22/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class EndianityTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ENDIANITYTEST_H_
#define ENDIANITYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Endianity.h"
#include "MemoryOperationsHelper.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests all the Endianity class functions.
 *
 * @details This class is designed to test all the Endianity functions,
 * for all the supported MARTe types. It forces the conversion
 * from (to) the target architecture endianity (i.e. the endianity of
 * machine where the code is being executed), to (from) the
 * complementary endianity.
 */
template<class T>
class EndianityTest {

public:

    /**
     * @brief Constructor.
     * @param[in] testValue the value to be tested by the all
     * the test functions.
     */

    EndianityTest(T testValue);
    /**
     * @brief Converts the testValue to big endian.
     * @details If the target architecture is little endian the test value and result must be different.
     * @return true if the conditions above are verified.
     */
    bool TestToBigEndian();

    /**
     * @brief Converts the testValue to little endian.
     * @details If the target architecture is big endian the test value and the result must be different.
     * @return true if the conditions above are verified.
     */
    bool TestToLittleEndian();

    /**
     * @brief Converts the testValue from big endian.
     * @details If the target architecture is little endian the test value and the result value be different.
     * @return true if the conditions above are verified.
     */
    bool TestFromBigEndian();

    /**
     * @brief Converts the testValue to little endian.
     * @details If the target architecture is big endian the test value and the result must be different.
     * @return true if the conditions above are verified.
     */
    bool TestFromLittleEndian();

    /**
     * @brief Converts the testArray to big endian from the target
     * architecture endianity.
     * @details If the target architecture is little endian each element of the test array and the big endian array must be different.
     * @return true if the conditions above are verified.
     */
    bool TestMemCopyToBigEndian();

    /**
     * @brief Converts the testArray to little endian from the target
     * architecture endianity.
     * @details If the target architecture is big endian each element of the test array and the big endian array must be different.
     * @return true if the conditions above are verified.
     */
    bool TestMemCopyToLittleEndian();

    /**
     * @brief Converts the testArray from big endian to the
     * target architecture endianity.
     * @details If the target architecture is little endian each element of the test array and the big endian array must be different.
     * @return true if the conditions above are verified.
     */
    bool TestMemCopyFromBigEndian();

    /**
     * @brief Converts the testArray from little endian to the
     * target architecture endianity.
     * @details If the target architecture is big endian each element of the test array and the little endian array must be different.
     * @return true if the conditions above are verified.
     */
    bool TestMemCopyFromLittleEndian();

    /**
     * @brief Verifies that going to a different endianity and returning to the
     * target architecture endianity does not change the value.
     *
     * @details Converts the testValue to little endian and to big endian and then back
     * from little and from big endian. The values should return to the original test
     * value.
     *
     * @return true if the test value is not changed after returning to the original
     * endianity.
     */
    bool TestToFromEndian();

    /**
     * @brief The same logic as ToFromEndian but for arrays.
     *
     * @details The same EndianityTest::ToFromEndian but using the
     * Endianity::MemCopyToFromEndian and Endianity::MemCopyFromLittleEndian functions.
     */
    bool TestMemCopyToFromEndian();

private:

    /**
     * The test class is designed to support any basic type
     */
    T testValue;

    /**
     * A static array to test operations that include arrays
     */
    T testArray[3];
};

/*---------------------------------------------------------------------------*/
/*                        Template method definitions                        */
/*---------------------------------------------------------------------------*/

template<class T>
EndianityTest<T>::EndianityTest(T testValue) {
    this->testValue = testValue;
    this->testArray[0] = testValue;
    this->testArray[1] = testValue + 1;
    this->testArray[2] = testValue - 1;
}

template<class T>
bool EndianityTest<T>::TestToBigEndian() {
    T valueBigEndian = testValue;

    //converts to big endian the value.
    Endianity::ToBigEndian(valueBigEndian);

    //the system uses big endian if the be conversion returns the same number.
    bool isBigEndian = MemoryOperationsHelper::Compare(&testValue, &valueBigEndian, sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return !isBigEndian;
    }
    else {
        return isBigEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestToLittleEndian() {
    T valueLittleEndian = testValue;

    //converts to little endian the value.
    Endianity::ToLittleEndian(valueLittleEndian);

    //the system uses little endian if the le conversion returns the same number.
    bool isLittleEndian = MemoryOperationsHelper::Compare(&testValue, &valueLittleEndian, sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return isLittleEndian;
    }
    else {
        return !isLittleEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestFromBigEndian() {
    T valueBigEndian = testValue;

    //converts from big endian the value.
    Endianity::FromBigEndian(valueBigEndian);

    //the system uses big endian if the be conversion returns the same number.
    bool isBigEndian = MemoryOperationsHelper::Compare(&testValue, &valueBigEndian, sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return !isBigEndian;
    }
    else {
        return isBigEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestFromLittleEndian() {
    T valueLittleEndian = testValue;

    //converts from little endian the value.
    Endianity::FromLittleEndian(valueLittleEndian);

    //the system uses little endian if the le conversion returns the same number.
    bool isLittleEndian = MemoryOperationsHelper::Compare(&testValue, &valueLittleEndian, sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return isLittleEndian;
    }
    else {
        return !isLittleEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestMemCopyToBigEndian() {
    T arrayBigEndian[3];

    //converts to big endian the array
    Endianity::MemCopyToBigEndian((T *) arrayBigEndian, (T *) testArray, 3);

    //if the system uses big endian each number in the array remains the same.
    bool isBigEndian =  MemoryOperationsHelper::Compare(&testArray[0], &arrayBigEndian[0], 3 * sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return !isBigEndian;
    }
    else {
        return isBigEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestMemCopyToLittleEndian() {
    T arrayLittleEndian[3];

    //converts to little endian the array
    Endianity::MemCopyToLittleEndian((T *) arrayLittleEndian, (T *) testArray, 3);

    //if the system uses little endian each number in the array remains the same.
    bool isLittleEndian = MemoryOperationsHelper::Compare(&testArray[0], &arrayLittleEndian[0], 3 * sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return isLittleEndian;
    }
    else {
        return !isLittleEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestMemCopyFromBigEndian() {
    T arrayBigEndian[3];

    //converts from big endian the array
    Endianity::MemCopyFromBigEndian((T *) arrayBigEndian, (T *) testArray, 3);

    //if the system uses big endian each number in the array remains the same.
    bool isBigEndian = MemoryOperationsHelper::Compare(&testArray[0], &arrayBigEndian[0], 3 * sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return !isBigEndian;
    }
    else {
        return isBigEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestMemCopyFromLittleEndian() {
    T arrayLittleEndian[3];

    //converts from little endian the array
    Endianity::MemCopyFromLittleEndian((T *) arrayLittleEndian, (T *) testArray, 3);

    //if the system uses little endian each number in the array remains the same.
    bool isLittleEndian = MemoryOperationsHelper::Compare(&testArray[0], &arrayLittleEndian[0], 3 * sizeof(T)) == 0;

    //checks if the type function is consistent with the conversions.
    if (Endianity::Type() == Endianity::ENDIANITY_LITTLE_ENDIAN) {
        return isLittleEndian;
    }
    else {
        return !isLittleEndian;
    }
}

template<class T>
bool EndianityTest<T>::TestToFromEndian() {
    T valueLittleEndian = testValue;
    T valueBigEndian = testValue;

    //converts to little endian and then from little endian
    Endianity::ToLittleEndian(valueLittleEndian);
    Endianity::FromLittleEndian(valueLittleEndian);

    //converts to big endian and then from big endian
    Endianity::ToBigEndian(valueBigEndian);
    Endianity::FromBigEndian(valueBigEndian);

    //the value must remain the same after the opposite conversions
    return (MemoryOperationsHelper::Compare(&testValue, &valueLittleEndian, sizeof(T)) == 0)
            && (MemoryOperationsHelper::Compare(&testValue, &valueBigEndian, sizeof(T)) == 0);
}

template<class T>
bool EndianityTest<T>::TestMemCopyToFromEndian() {
    T arrayLittleEndian[3];
    T arrayBigEndian[3];
    T arrayToFrom[3];

    //copy the array to little endian and then from little endian
    Endianity::MemCopyToLittleEndian((T *) arrayLittleEndian, (T *) testArray, 3);
    Endianity::MemCopyFromLittleEndian((T *) arrayToFrom, (T *) arrayLittleEndian, 3);

    //Each element should remain the same
    bool okLittleEndian = MemoryOperationsHelper::Compare(&testArray[0], &arrayToFrom[0], 3 * sizeof(T)) == 0;

    //copy the array to big endian and then from big endian
    Endianity::MemCopyToBigEndian((T *) arrayBigEndian, (T *) testArray, 3);
    Endianity::MemCopyFromBigEndian((T *) arrayToFrom, (T *) arrayBigEndian, 3);

    //Each element should remain the same
    bool okBigEndian = MemoryOperationsHelper::Compare(&testArray[0], &arrayToFrom[0], 3 * sizeof(T)) == 0;

    return (okLittleEndian && okBigEndian);
}

#endif /* ENDIANITYTEST_H_ */
