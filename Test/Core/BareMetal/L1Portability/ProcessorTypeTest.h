/**
 * @file ProcessorTypeTest.h
 * @brief Header file for class ProcessorTypeTest
 * @date 25/06/2015
 * @author Giuseppe Ferro
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
#define PROCESSORTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ProcessorType.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the ProcessorType functions.
 */
class ProcessorTypeTest {

public:

    /**
     * @brief Constructor.
     */
    ProcessorTypeTest();

    /**
     * @brief Tests the overloaded assignment operator in ProcessorType
     * @return true if a local test ProcessorType variable can be successfully assigned to two different ProcessorType values.
     */
    bool TestAssignmentOperator();

    /**
     * @brief Tests the overloaded OR operator in ProcessorType
     * @return true if the OR operation between several combinations of a ProcessorType local variable and some ProcessorType dummy test variables are valid.
     */
    bool TestOROperator();

    /**
     * @brief Tests the overloaded equality operator in ProcessorType
     * @return true if the equality operation between a ProcessorType local variable and a ProcessorType dummy test variables is valid.
     */
    bool TestEqualityOperator();

    /**
     * @brief Tests the overloaded inequality operator in ProcessorType
     * @return true if the inequality operation between a ProcessorType local variable and a ProcessorType dummy test variables is valid.
     */
    bool TestInequalityOperator();

    /**
     * @brief Tests ProcessorType::GetDefaultCPUs and ProcessorType::SetDefaultCPUs.
     * @return true if the ProcessorType default CPUs can be successfully set and retrieved.
     */
    bool TestSetGetDefaultCPUs();

    /**
     * @brief Tests ProcessorType's default constructor
     * @return true if all the ProcessorType constructors can be successfully assigned to the expected initial CPU mask.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the Processor type constructor with a mask as input.
     * @return true if the mask is equal to the input passed by argument.
     */
    bool TestConstructorFromMask();

    /**
     * @brief Tests the Processor type constructor with another ProcessorType as input.
     * @return true if the mask is equal to the mask of the ProcessorType object passed by argument.
     */
    bool TestConstructorFromProcessorType();

    /**
     * @brief Tests the ProcessorType::SetMask function.
     * @param[in] mask is the desired cpu mask.
     * @return true if the mask is assigned correctly.
     */
    bool TestSetMask(uint64 mask);

    /**
     * @brief Tests the ProcessorType::AddCPU function.
     * @param[in] cpuNumber1 is the first cpu to be added.
     * @param[in] cpuNumber2 is the second cpu to be added.
     * @return true if in the cpu mask is set 1 to the bits related to the desired cpu numbers.
     */
    bool TestAddCPU(uint32 cpuNumber1,
                    uint32 cpuNumber2);

    /**
     * @brief Tests the ProcessorType::GetProcessorMask function
     */
    bool TestGetProcessorMask();

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

