/**
 * @file BasicTypeTest.h
 * @brief Header file for class BasicTypeTest
 * @date 02/09/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class BasicTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0TYPEDEV_BASICTYPETEST_H_
#define TEST_CORE_L0TYPEDEV_BASICTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BasicType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class BasicTypeTest {
public:
    /**
     * BasicTypeTest constructor
     */
    BasicTypeTest();

    /**
     * Returned value of the test functions.
     */
    bool retVal;

    /**
     * @brief Test the BasicType constructor using and int8.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorInt8();

    /**
     * @brief Test the BasicType constructor using and uint8.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorUInt8();

    /**
     * @brief Test the BasicType constructor using and int16.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorInt16();

    /**
     * @brief Test the BasicType constructor using and Uint16.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorUInt16();

    /**
     * @brief Test the BasicType constructor using and int32.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorInt32();

    /**
     * @brief Test the BasicType constructor using and uint32.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorUInt32();

    /**
     * @brief Test the BasicType constructor using and int64.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorInt64();

    /**
     * @brief Test the BasicType constructor using and uint64.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorUInt64();

    /**
     * @brief Test the BasicType constructor using and float32.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorFloat32();

    /**
     * @brief Test the BasicType constructor using and float64.
     * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
     * order to fully test the constructor. Then is check that the value is set as expected (in the range
     * [0, 15]).
     * @return True if in the three cases the value is as expected.
     */
    bool TestBasicTypeConstructorFloat64();

    /**
      * @brief Test the BasicType constructor using and char8.
      * @detail This test use three different values to call the constructor: x1 < 0, 0 < x2 <, x3 > 15 in
      * order to fully test the constructor. Then is check that the value is set as expected (in the range
      * [0, 15]).
      * @return True if in the three cases the value is as expected.
      */
    bool TestBasicTypeConstructorChar8();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0TYPEDEV_BASICTYPETEST_H_ */

