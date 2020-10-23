/**
 * @file MultiThreadService.h
 * @brief Header file for class MultiThreadServerClass
 * @date 30/08/2016
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

 * @details This header file contains the declaration of the class MultiThreadServerClass
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTITHREADSERVICE_H_
#define MULTITHREADSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EmbeddedServiceI.h"
#include "ReferenceContainer.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Associates a pool of EmbeddedThread instances (fixed size) to a class method.
 * @details This class allows associating a class method in the form (MARTe::ErrorManagement::ErrorType (*)(MARTe::EmbeddedServiceI::ExecutionInfo &)) to a thread context.
 * This method will be continuously called (see Start) with the stage encoded in the Information parameter.
 * This class allows to request for a "kind" Stop of the embedded thread and, if this fails, for a direct killing of the thread.
 */
class MultiThreadService: public EmbeddedServiceI {

public:
    /**
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this MultiThreadService.
     * @post
     *   GetNumberOfPoolThreads() == 1
     */
    MultiThreadService(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Destructor.
     * @post
     *   Stop()
     */
    virtual ~MultiThreadService();

    /**
     * @brief Reads the number of pool threads from the data input. Calls EmbeddedServiceI::Initialise().
     * @details in addition to the parameters defined in EmbeddedServiceI::Initialise it shall contain a parameter
     * named "NumberOfPoolThreads" holding the number of pool threads
     * and another parameter named "Timeout" with the timeout to apply to each of the SingleThreadService instances.
     * If "Timeout=0" => Timeout = TTInfiniteWait.
     * A matrix named PrioritiesClass may be defined. The first column of each row shall contain the thread index to
     * which to apply the PriorityClass specified in the second column.
     * A matrix named PrioritiesLevel may be defined. The first column of each row shall contain the thread index to
     * which to apply the PriorityLevel specified in the second column.
     * A matrix named StackSizes may be defined. The first column of each row shall contain the thread index to
     * which to apply the StackSize specified in the second column.
     * A matrix named CPUMasks may be defined. The first column of each row shall contain the thread index to
     * which to apply the CPUMask specified in the second column.
     * If the initialise is successful it calls CreateThreads() and applies the individual thread parameters (priority class and level plus affinity).
     * @param[in] data is the container with the parameters.
     * @return true if all the parameters are available and if CreateThreads() returns NoError.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Starts N (GetNumberOfPoolThreads()) SingleThreadService instances.
     * @details If the SingleThreadService were not created yet, CreateThreads is called.
     * @return ErrorManagement::NoError if all the instances can be successfully started.
     * ErrorManagement::IllegalOperation if start is called twice without calling stop beforehand.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops N (GetNumberOfPoolThreads()) SingleThreadService instances.
     * @details A Kill will be issued if the SingleThreadService do not Stop at the first time, i.e. if the first
     *  call to Stop fails (or times-out) a second Stop() will kill the threads.
     * @return ErrorManagement::NoError if all the instances can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop();

    /**
     * @brief Creates N (GetNumberOfPoolThreads()) SingleThreadService instances but does not Start them.
     * @details The SingleThreadServices are configured with the default parameters given by GetPriorityLevel, GetPriorityClass and GetCPUMask.
     * @return ErrorManagement::NoError if all the instances can be successfully created and configured with the default parameters.
     * ErrorManagement::IllegalOperation if it is called twice without calling stop beforehand.
     * @pre
     *   Stop()
     */
    ErrorManagement::ErrorType CreateThreads();

    /**
     * @brief Gets the number of pool threads.
     * @return the number of pool threads.
     */
    uint32 GetNumberOfPoolThreads() const;

    /**
     * @brief Sets the number of pool threads.
     * @param[in] numberOfPoolThreadsIn the number of pool threads.
     * @pre
     *   Stop()
     */
    void SetNumberOfPoolThreads(const uint32 numberOfPoolThreadsIn);

    /**
     * @brief Gets the status of the SingleThreadService with index \a threadIdx.
     * @param[in] threadIdx the index of the thread to query.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     */
    EmbeddedThreadI::States GetStatus(const uint32 threadIdx);

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeoutIn the maximum time in milliseconds to execute a state change.
     */
    virtual void SetTimeout(const TimeoutType & msecTimeoutIn);

    /**
     * @brief Sets the thread priority class level for all the threads.
     * @param[in] priorityClassIn the thread priority class.
     * @pre
     *   GetStatus(*) == OffState
     */
    virtual void SetPriorityClass(Threads::PriorityClassType priorityClassIn);

    /**
     * @brief Sets the thread priority level for all the threads.
     * @param[in] priorityLevelIn the thread priority level.
     * @pre
     *   GetStatus(*) == OffState
     */
    virtual void SetPriorityLevel(uint8 priorityLevelIn);

    /**
     * @brief Sets the thread stack size for all the threads.
     * @param[in] stackSizeIn the thread stack size.
     * @pre
     *   GetStatus(*) == OffState
     */
    virtual void SetStackSize(uint32 stackSizeIn);

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity) level for all the threads.
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     * @pre
     *   GetStatus(*) == OffState
     */
    virtual void SetCPUMask(const ProcessorType& cpuMaskIn);

    /**
     * @brief Gets the thread priority class for the thread with index \a threadIdx.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     * @return the thread priority class or UnknownPriorityClass if the pre conditions are not met.
     * TODO tests
     */
    Threads::PriorityClassType GetPriorityClassThreadPool(uint32 threadIdx);

    /**
     * @brief Gets the thread priority level for the thread with index \a threadIdx.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     * @return the thread priority class or 0 if the pre conditions are not met.
     */
    uint8 GetPriorityLevelThreadPool(uint32 threadIdx);

    /**
     * @brief Gets the thread stack size for the thread with index \a threadIdx.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     * @return the thread stack size or 0 if the pre conditions are not met.
     */
    uint32 GetStackSizeThreadPool(uint32 threadIdx);

    /**
     * @brief Gets the thread name for the thread with index \a threadIdx.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     * @return the thread name or NULL if the pre conditions are not met.
     * @post
     *   The returned pointer may be deleted at any time.
     */
    const char8 * const GetThreadNameThreadPool(uint32 threadIdx);

    /**
     * @brief Gets the thread priority CPU mask for the thread with index \a threadIdx.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   threadIdx < GetNumberOfPoolThreads()
     * @return the thread CPUMask class or UndefinedCPUs if the pre conditions are not met.
     */
    ProcessorType GetCPUMaskThreadPool(uint32 threadIdx);

    /**
     * @brief Sets the thread priority class level for the thread with index \a threadIdx.
     * @param[in] priorityClassIn the thread priority class.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   GetStatus(threadIdx) == OffState
     *   threadIdx < GetNumberOfPoolThreads()
     */
    void SetPriorityClassThreadPool(Threads::PriorityClassType priorityClassIn, uint32 threadIdx);

    /**
     * @brief Sets the thread priority level for the thread with index \a threadIdx.
     * @param[in] priorityLevelIn the thread priority level.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   GetStatus(threadIdx) == OffState
     *   threadIdx < GetNumberOfPoolThreads()
     */
    void SetPriorityLevelThreadPool(uint8 priorityLevelIn, uint32 threadIdx);

    /**
     * @brief Sets the thread stack size for the thread with index \a threadIdx.
     * @param[in] stackSizeIn the thread priority level.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   GetStatus(threadIdx) == OffState
     *   threadIdx < GetNumberOfPoolThreads()
     */
    void SetStackSizeThreadPool(uint32 stackSizeIn, uint32 threadIdx);

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity) level for the thread with index \a threadIdx.
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   GetStatus(threadIdx) == OffState
     *   threadIdx < GetNumberOfPoolThreads()
     */
    void SetCPUMaskThreadPool(const ProcessorType& cpuMaskIn, uint32 threadIdx);

    /**
     * @brief Sets the thread name for the thread with index \a threadIdx.
     * @param[in] threadName the thread name to set.
     * @param[in] threadIdx the index of the thread.
     * @pre
     *   GetStatus(threadIdx) == OffState
     *   threadIdx < GetNumberOfPoolThreads()
     */
    void SetThreadNameThreadPool(const char8 * const threadName, uint32 threadIdx);


protected:
    /**
     * Holds a pool of ReferenceT<SingleThreadService> references.
     */
    ReferenceContainer threadPool;

    /**
     * The fixed number of pool threads.
     */
    uint32 numberOfPoolThreads;

    /**
     * The callback method
     */
    /*lint -e{1725} method is a reference that is initialised during construction and is the basic mechanism to register
     * the callback function to be called by the EmbeddedThreadI. */
    EmbeddedServiceMethodBinderI &method;
    /*lint -e{1712} This class does not have a default constructor because
     * the callback method must be defined at construction and will remain constant
     * during the object's lifetime*/
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* MULTITHREADSERVICE_H_ */

