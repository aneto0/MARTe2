/**
 * @file EmbeddedServiceI.h
 * @brief Header file for class EmbeddedServiceI
 * @date 01/09/2016
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class EmbeddedServiceI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EMBEDDEDSERVICEI_H_
#define EMBEDDEDSERVICEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceMethodBinderI.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "EmbeddedThreadI.h"
#include "ErrorType.h"
#include "ReferenceContainer.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Family of objects that enable the interfacing of a class method to a thread or to a pool of threads.
 * @details The method callback interface is specified in EmbeddedServiceMethodBinderT<class>::MethodPointer.
 */
class DLL_API EmbeddedServiceI: public ReferenceContainer {
public:
    /**
     * @brief Constructor. NOOP.
     * @post
     *   GetPriorityLevel() == 0u &&
     *   GetPriorityClass() == NormalPriorityClass &&
     *   GetTimeout() == TTInfiniteWait &&
     *   GetCPUMask() == UndefinedCPUs &&
     *   GetStackSize() == THREADS_DEFAULT_STACKSIZE
     */
    EmbeddedServiceI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~EmbeddedServiceI();

    /**
     * @brief Starts the embedded service (which will call the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully started.
     */
    virtual ErrorManagement::ErrorType Start() = 0;

    /**
     * @brief Stops the embedded service (which is calling the registered callback method in the context of a thread).
     * @return ErrorManagement::NoError if the service can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop() = 0;

    /**
     * @brief Reads the Timeout from the data input.
     * @param[in] data shall contain a parameter with name "Timeout" holding the timeout in milliseconds.
     * If "Timeout=0" => Timeout = TTInfiniteWait.
     * data may contain a parameter with name "PriorityLevel" holding the thread priority level (default is 0).
     * data may contain a parameter with name "PriorityClass" holding the thread priority class.
     *   Possible values are: IdlePriorityClass; NormalPriorityClass and RealTimePriorityClass (default is NormalPriorityClass)
     * data may contain a parameter with name "CPUMask" holding the thread CPU affinity encoded as uint32 mask (default value is UndefinedCPUs).
     * @return true if all the parameters are valid.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Gets the thread priority class.
     * @return the thread priority class.
     */
    Threads::PriorityClassType GetPriorityClass() const;

    /**
     * @brief Sets the thread priority class.
     * @param[in] priorityClassIn the thread priority class.
     */
    virtual void SetPriorityClass(Threads::PriorityClassType priorityClassIn);

    /**
     * @brief Gets the thread priority level.
     * @return the thread priority level.
     */
    uint8 GetPriorityLevel() const;

    /**
     * @brief Sets the thread priority level.
     * @param[in] priorityLevelIn the thread priority level.
     */
    virtual void SetPriorityLevel(uint8 priorityLevelIn);

    /**
     * @brief Gets the thread stack size.
     * @return the thread stack size.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Sets the thread stack size.
     * @param[in] stackSizeIn the thread stack size.
     */
    virtual void SetStackSize(uint32 stackSizeIn);

    /**
     * @brief Gets the thread CPU mask (i.e. thread affinity).
     * @return the thread CPU mask.
     */
    ProcessorType GetCPUMask() const;

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity).
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     */
    virtual void SetCPUMask(const ProcessorType& cpuMaskIn);

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeoutIn the maximum time in milliseconds to execute a state change.
     */
    virtual void SetTimeout(const TimeoutType &msecTimeoutIn);

    /**
     * @brief Gets the maximum time to execute a state change.
     * @return the maximum time to execute a state change.
     */
    TimeoutType GetTimeout() const;

protected:

    /**
     * The maximum time to execute a state change.
     */
    TimeoutType msecTimeout;

    /**
     * The thread priority class.
     */
    Threads::PriorityClassType priorityClass;

    /**
     * The thread priority level.
     */
    uint8 priorityLevel;

    /**
     * The thread stack size
     */
    uint32 stackSize;

    /**
     * The thread CPU mask
     */
    ProcessorType cpuMask;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* EMBEDDEDSERVICEI_H_ */

