/**
 * @file ProcessorTypeTest.h
 * @brief Header file for class ProcessorTypeTest
 * @date 25/06/2015
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

 * @details This header file contains the declaration of the class ProcessorTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORTYPETEST_H_
#define 		PROCESSORTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ProcessorType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/** @brief A class to test the ProcessorType functions. */
class ProcessorTypeTest {

public:

    /**
     * @brief Constructor.
     */
    ProcessorTypeTest() :
            ptFirst(0xFE),
            ptSecond(0xFD),
            ptAny(0xFF),
            ptNone(0x0) {

    }

    /**
     * @brief Tests the overloaded assignment operator in ProcessorType
     * @return True if the test is successful, false otherwise.
     */
    bool TestAssignmentOperator();

    /**
     * @brief Tests the overloaded OR operator in ProcessorType
     * @return True if the test is successful, false otherwise.
     */
    bool TestOROperator();

    /**
     * @brief Tests the overloaded equality operator in ProcessorType
     * @return True if the test is successful, false otherwise.
     */
    bool TestEqualityOperator();

    /**
     * @brief Tests the overloaded inequality operator in ProcessorType
     * @return True if the test is successful, false otherwise.
     */
    bool TestInequalityOperator();

    /**
     * @brief Tests Get/SetDefaultCPUs in ProcessorType
     * @return True if the test is successful, false otherwise.
     */
    bool TestGetSetDefaultCPUs();

    /**
     * @brief Tests ProcessorType's constructors
     * @return True if the test is successful, false otherwise.
     */
    bool TestConstructors();

private:

    /**
     * First processor type variable
     */
    ProcessorType ptFirst;

    /**
     * Second processor type variable
     */
    ProcessorType ptSecond;

    /**
     * Initialized with a mask of ones
     */
    ProcessorType ptAny;

    /**
     * Initialized with a mask of zeros
     */
    ProcessorType ptNone;

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORTYPETEST_H_ */

