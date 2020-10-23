/**
 * @file MultiThreadServiceTest.h
 * @brief Header file for class MultiThreadServiceTest
 * @date 19/09/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class MultiThreadServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTITHREADSERVICETEST_H_
#define MULTITHREADSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MultiThreadService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the MultiThreadService public methods.
 */
class MultiThreadServiceTest {
public:

    /**
     * @brief Default constructor
     */
    MultiThreadServiceTest();

    /**
     * @brief Destructor
     */
    virtual ~MultiThreadServiceTest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor_Template();

    /**
     * @brief Tests the Initialise function.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise function without specifying the Timeout parameter.
     */
    bool TestInitialise_False_Timeout();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass that is not a matrix.
     */
    bool TestInitialise_False_PrioritiesClass_NotMatrix();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass with a matrix that has more than two columns.
     */
    bool TestInitialise_False_PrioritiesClass_Columns();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass with an out of range index.
     */
    bool TestInitialise_False_PrioritiesClass_OutOfRangeIndex();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass that is not a matrix.
     */
    bool TestInitialise_False_PrioritiesLevel_NotMatrix();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesLevel with a matrix that has more than two columns.
     */
    bool TestInitialise_False_PrioritiesLevel_Columns();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesLevel with an out of range index.
     */
    bool TestInitialise_False_PrioritiesLevel_OutOfRangeIndex();

    /**
     * @brief Tests the Initialise function specifying a StackSize that is not a matrix.
     */
    bool TestInitialise_False_StackSizes_NotMatrix();

    /**
     * @brief Tests the Initialise function specifying a StackSize with a matrix that has more than two columns.
     */
    bool TestInitialise_False_StackSizes_Columns();

    /**
     * @brief Tests the Initialise function specifying a StackSize with an out of range index.
     */
    bool TestInitialise_False_StackSizes_OutOfRangeIndex();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass with an index that is not a number.
     */
    bool TestInitialise_False_PrioritiesClass_TypeConvert();

    /**
     * @brief Tests the Initialise function specifying a PrioritiesClass with an invalid priority class.
     */
    bool TestInitialise_False_PrioritiesClass_Invalid();

    /**
     * @brief Tests the Initialise function specifying a CPUMasks that is not a matrix.
     */
    bool TestInitialise_False_CPUMasks_NotMatrix();

    /**
     * @brief Tests the Initialise function specifying a CPUMasks with a matrix that has more than two columns.
     */
    bool TestInitialise_False_CPUMasks_Columns();

    /**
     * @brief Tests the Initialise function specifying a CPUMasks with an out of range index.
     */
    bool TestInitialise_False_CPUMasks_OutOfRangeIndex();

    /**
     * @brief Tests the Start function.
     */
    bool TestStart();

    /**
     * @brief Tests the Start function calling it twice, which should trigger an illegal operation.
     */
    bool TestStart_False();

    /**
     * @brief Tests the Start function after a stop has been called.
     */
    bool TestStart_Restart();

    /**
     * @brief Tests the CreateThread function.
     */
    bool TestCreateThreads();

    /**
     * @brief Tests the CreateThread function calling it twice, which should trigger an illegal operation.
     */
    bool TestCreateThreads_False();

    /**
     * @brief Tests the CreateThread function after a stop has been called.
     */
    bool TestCreateThreads_Restart();

    /**
     * @brief Tests the Stop function.
     */
    bool TestStop();

    /**
     * @brief Tests the Stop function forcing a kill.
     */
    bool TestStop_Kill();

    /**
     * @brief Tests the GetStatus function.
     */
    bool TestGetStatus();

    /**
     * @brief Tests the SetTimeout function
     */
    bool TestSetTimeout();

    /**
     * @brief Tests the GetTimeout function
     */
    bool TestGetTimeout();

    /**
     * @brief Tests the SetPriorityClass method.
     */
    bool TestSetPriorityClass();

    /**
     * @brief Tests the SetPriorityLevel method.
     */
    bool TestSetPriorityLevel();

    /**
     * @brief Tests the SetStackSize method.
     */
    bool TestSetStackSize();

    /**
     * @brief Tests the TestSetCPUMask method.
     */
    bool TestSetCPUMask();

    /**
     * @brief Tests the SetNumberOfPoolThreads function
     */
    bool TestSetNumberOfPoolThreads();

    /**
     * @brief Tests the GetNumberOfPoolThreads function
     */
    bool TestGetNumberOfPoolThreads();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetPriorityClass_Start();

    /**
     * @brief Verifies that priority level is not changed after the service has started.
     */
    bool TestSetPriorityLevel_Start();

    /**
     * @brief Verifies that stack size is not changed after the service has started.
     */
    bool TestSetStackSize_Start();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetCPUMask_Start();

    /**
     * @brief Tests the SetPriorityClassThreadPool method.
     */
    bool TestSetPriorityClassThreadPool();

    /**
     * @brief Tests the SetPriorityLevelThreadPool method.
     */
    bool TestSetPriorityLevelThreadPool();

    /**
     * @brief Tests the SetStackSizeThreadPool method.
     */
    bool TestSetStackSizeThreadPool();

    /**
     * @brief Tests the SetThreadNameThreadPool method.
     */
    bool TestSetThreadNameThreadPool();

    /**
     * @brief Tests the SetCPUMaskThreadPool method.
     */
    bool TestSetCPUMaskThreadPool();

    /**
     * @brief Tests the GetPriorityClassThreadPool method.
     */
    bool TestGetPriorityClassThreadPool();

    /**
     * @brief Tests the GetPriorityLevelThreadPool method.
     */
    bool TestGetPriorityLevelThreadPool();

    /**
     * @brief Tests the GetStackSizeThreadPool method.
     */
    bool TestGetStackSizeThreadPool();

    /**
     * @brief Tests the GetThreadNameThreadPool method.
     */
    bool TestGetThreadNameThreadPool();

    /**
     * @brief Tests the GetCPUMaskThreadPool method.
     */
    bool TestGetCPUMaskThreadPool();

    /**
     * @brief Tests the GetPriorityClassThreadPool method with an index which is greater than the number of threads in the pool.
     */
    bool TestGetPriorityClassThreadPool_OutOfIndex();

    /**
     * @brief Tests the GetPriorityLevelThreadPool method with an index which is greater than the number of threads in the pool.
     */
    bool TestGetPriorityLevelThreadPool_OutOfIndex();

    /**
     * @brief Tests the GetStackSizeThreadPool method with an index which is greater than the number of threads in the pool.
     */
    bool TestGetStackSizeThreadPool_OutOfIndex();

    /**
     * @brief Tests the GetCPUMaskThreadPool method with an index which is greater than the number of threads in the pool.
     */
    bool TestGetCPUMaskThreadPool_OutOfIndex();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetPriorityClassThreadPool_Start();

    /**
     * @brief Verifies that priority level is not changed after the service has started.
     */
    bool TestSetPriorityLevelThreadPool_Start();

    /**
     * @brief Verifies that priority class is not changed after the service has started.
     */
    bool TestSetCPUMaskThreadPool_Start();

    /**
     * @brief Verifies that stack size is not changed after the service has started.
     */
    bool TestSetStackSizeThreadPool_Start();

    /**
     * @brief Verifies that the thread name is not changed after the service has started.
     */
    bool TestSetThreadNameThreadPool_Start();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTITHREADSERVICETEST_H_ */

