/**
 * @file ExecutionInfo.h
 * @brief Header file for class ExecutionInfo
 * @date 20/09/2016
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

 * @details This header file contains the declaration of the class ExecutionInfo
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXECUTIONINFO_H_
#define EXECUTIONINFO_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BitRange.h"
#include "BitBoolean.h"
#include "ErrorType.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/*lint -sem(MARTe::ExecutionInfo::Reset,initializer)*/
namespace MARTe {
/**
 * @brief Communicates to the user code the stage of the thread life, which evolves accordingly to rules
 * that are specific to the EmbeddedServiceI derived class.
 * @details In addition to stage, it provides a stage which is fully custom to the class that inherits
 * from EmbeddedServiceI and that receives an ExecutionInfo as part of its callback method.
 */
class ExecutionInfo {

public:
    //Note that the list of stages is not an enum as it is used in a BitRange below as a uint8
    /**
     * Start of a thread execution sequence
     */
    static const uint8 StartupStage = 0u;

    /**
     * Main part of a sequence - looping unless an error or completed is returned
     */
    static const uint8 MainStage = 1u;

    /**
     * Normal termination stage - following the end of the mainStage
     */
    static const uint8 TerminationStage = 2u;

    /**
     * Bad termination stage - after an error returned by the user code or following the Stop()
     */
    static const uint8 BadTerminationStage = 3u;

    /**
     * After a kill - called by the killing task
     */
    static const uint8 AsyncTerminationStage = 4u;

    /**
     *  Sub-states of mainStage.
     */
    /**
     * Set when stage is startupStage or terminationStage
     */
    static const uint8 NullStageSpecific = 0u;

    /**
     * For client&server model - wait for service request
     */
    static const uint8 WaitRequestStageSpecific = 1u;

    /**
     * For client&server model - servicing the client
     */
    static const uint8 ServiceRequestStageSpecific = 2u;

    /**
     * @brief Constructor.
     * @post
     *   Reset()
     */
    ExecutionInfo();

    /**
     * @brief Sets the thread unique number in a thread pool of an EmbeddedService.
     * @param[in] number the thread number.
     * @pre
     *   GetStage() == StartupStage
     */
    void SetThreadNumber(const ThreadIdentifier &number);

    /**
     * @brief Sets the current stage (as encoded above).
     * @param[in] number the current stage.
     * @post
     *   GetStage() == number
     */
    void SetStage(const uint8 number);

    /**
     * @brief Sets a stage which is specific to a class derived from EmbeddedServiceI.
     * @param[in] number the current stage.
     * @post
     *   GetStageSpecific() == number
     */
    void SetStageSpecific(const uint8 number);

    /**
     * @brief Gets the thread unique number in a thread pool of an EmbeddedServiceI.
     * @return the thread unique number in a thread pool.
     */
    ThreadIdentifier GetThreadNumber() const;

    /**
     * @brief Gets the current stage (as encoded above).
     * @return the current stage (as encoded above).
     */
    uint8 GetStage() const;

    /**
     * @brief Gets a stage which is specific to a class derived from EmbeddedServiceI.
     * @return a stage which is specific to a class derived from EmbeddedServiceI.
     */
    uint8 GetStageSpecific() const;

    /**
     * @brief Resets the thread number and the stages to their initial values.
     * @post
     *   GetThreadNumber() == 0 &&
     *   GetStage() == StartupStage &&
     *   GetStageSpecific() == NullStageSpecific
     */
    void Reset();

private:

    /**
     * ThreadNumber is an unique id that identifies a thread within an EmbeddedService.
     */
    ThreadIdentifier threadNumber;

    /**
     * The operating stage of the thread.
     */
    uint8 stage;

    /**
     * The operating stage of the thread (specific to the class inhering from EmbeddedServiceI).
     */
    uint8 stageSpecific;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXECUTIONINFO_H_ */

