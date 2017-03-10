/**
 * @file MemoryMapAsyncTriggerOutputBrokerTest.h
 * @brief Header file for class MemoryMapAsyncTriggerOutputBrokerTest
 * @date 24/01/2017
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

 * @details This header file contains the declaration of the class MemoryMapAsyncTriggerOutputBrokerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPASYNCTRIGGEROUTPUTBROKERTEST_H_
#define MEMORYMAPASYNCTRIGGEROUTPUTBROKERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the MemoryMapAsyncTriggerOutputBroker public methods.
 */
class MemoryMapAsyncTriggerOutputBrokerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Init method.
     */
    bool TestInit();

    /**
     * @brief Tests the InitWithTriggerParameters method.
     */
    bool TestInitWithTriggerParameters();

    /**
     * @brief Tests the InitWithTriggerParameters method with NumberOfBuffers = 0.
     */
    bool TestInitWithTriggerParameters_False_0_NumberOfBuffers();

    /**
     * @brief Tests the InitWithTriggerParameters method with PreTriggerBuffers > NumberOfBuffers = 0.
     */
    bool TestInitWithTriggerParameters_False_PreTriggerGreaterThanNumberOfBuffers();

    /**
     * @brief Tests the InitWithTriggerParameters method with PostTriggerBuffers > NumberOfBuffers = 0.
     */
    bool TestInitWithTriggerParameters_False_PostTriggerGreaterThanNumberOfBuffers();

    /**
     * @brief Tests the InitWithTriggerParameters method with (PrePostTriggerBuffers + PostTriggerBuffers) > NumberOfBuffers = 0.
     */
    bool TestInitWithTriggerParameters_False_PrePlusPostTriggerGreaterThanNumberOfBuffers();

    /**
     * @brief Tests the InitWithTriggerParameters method with more than one GAM writing to the DataSourceI.
     */
    bool TestInitWithTriggerParameters_False_MoreThanOneGAM();

    /**
     * @brief Tests the InitWithTriggerParameters method signals that have Samples > 1.
     */
    bool TestInitWithTriggerParameters_False_SamplesGreaterThanOne();

    /**
     * @brief Tests the InitWithTriggerParameters method with a wrong signal type for the trigger.
     */
    bool TestInitWithTriggerParameters_False_WrongTriggerSignalType();

    /**
     * @brief Tests the Execute method with N PreTriggerBuffers and N PostTriggerBuffers for many cycles.
     */
    bool TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_ManyCycles();

    /**
     * @brief Tests the Execute method with 0 PreTriggerBuffers and 0 PostTriggerBuffers.
     */
    bool TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers();

    /**
     * @brief Tests the Execute method with 0 PreTriggerBuffers and 0 PostTriggerBuffers always triggering.
     */
    bool TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering();

    /**
     * @brief Tests the Execute method with 1 PreTriggerBuffers and 0 PostTriggerBuffers.
     */
    bool TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers();

    /**
     * @brief Tests the Execute method with 1 PreTriggerBuffers and 1 PostTriggerBuffers.
     */
    bool TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers();

    /**
     * @brief Tests the Execute method with 0 PreTriggerBuffers and 1 PostTriggerBuffers.
     */
    bool TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers();

    /**
     * @brief Tests the Execute method with many PreTriggerBuffers and many PostTriggerBuffers.
     */
    bool TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers();

    /**
     * @brief Tests the Execute method with many PreTriggerBuffers and many PostTriggerBuffers with a pre-trigger that starts before zero.
     */
    bool TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_PreTriggerIncludesBeforeZero();

    /**
     * @brief Tests the Execute method with many PreTriggerBuffers and many PostTriggerBuffers always triggering.
     */
    bool TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering();

    /**
     * @brief Tests the Execute method with overwriting PreTriggerBuffers .
     */
    bool TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers();

    /**
     * @brief Tests the Execute method with overwriting PostTriggerBuffers .
     */
    bool TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers();

    /**
     * @brief Tests the Execute method with overwriting PreTriggerBuffers and PostTriggerBuffers .
     */
    bool TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers();

    /**
     * @brief Tests the Execute method with only one buffer.
     */
    bool TestExecute_1_Buffer_AlwaysTriggering();

    /**
     * @brief Tests that a buffer overrun is detected.
     */
    bool TestExecute_Buffer_Overrun();

    /**
     * @brief Tests the GetCPUMask method.
     */
    bool TestGetCPUMask();

    /**
     * @brief Tests the GetNumberOfBuffers method.
     */
    bool TestGetNumberOfBuffers();

    /**
     * @brief Tests the GetPreTriggerBuffers method.
     */
    bool TestGetPreTriggerBuffers();

    /**
     * @brief Tests the GetStackSize method.
     */
    bool TestGetStackSize();

    /**
     * @brief Tests the GetPostTriggerBuffers method.
     */
    bool TestGetPostTriggerBuffers();

    /**
     * @brief Tests the MemoryMapAsyncTriggerOutputBroker method.
     */
    bool TestResetPreTriggerBuffers();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPASYNCTRIGGEROUTPUTBROKERTEST_H_ */

