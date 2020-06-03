/**
 * @file MultiClientService.h
 * @brief Header file for class MultiClientService
 * @date 05/09/2016
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

 * @details This header file contains the declaration of the class MultiClientService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTICLIENTSERVICE_H_
#define MULTICLIENTSERVICE_H_

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

namespace MARTe {
/**
 * @brief Multiple client connections oriented EmbeddedServiceI implementation.
 * @details This class allows associating a class method in the form (MARTe::ErrorManagement::ErrorType (*)(MARTe::EmbeddedServiceI::ExecutionInfo &)) to a pool of threads.
 * This method will be continuously called (see Start) with the stage encoded in the Information parameter.
 * Notice that the user-callback should not block and should return ErrorManagement::Timeout while waiting for a connection to be established.
 */
class MultiClientService: public MultiThreadService {

public:

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this pool of thread.
     * @post
     *   GetMinimumNumberOfPoolThreads() == 1 &&
     *   GetMaximumNumberOfPoolThreads() == 3
     */
    MultiClientService(EmbeddedServiceMethodBinderI &binder);

    /**
     * @brief Constructor. Forces the setting of the method binder.
     * @param[in] binder the method which will be called in the context of this pool of threads.
     *   GetMinimumNumberOfPoolThreads() == 1 &&
     *   GetMaximumNumberOfPoolThreads() == 3
     */
    template<typename className>
    MultiClientService(EmbeddedServiceMethodBinderT<className> &binder);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MultiClientService();

    /**
     * @brief Reads the minimum and maximum number of pool threads from the data input. Calls EmbeddedServiceI::Initialise.
     * @param[in] data in addition to the parameters defined in EmbeddedServiceI::Initialise it shall contain a parameter
     * named "MinNumberOfThreads" holding the minimum number of pool threads,
     * another parameter named "MaxNumberOfThreads" containing the maximum number of pool threads
     * and another parameter named "Timeout" with the timeout to apply to each of the SingleThreadService instances.
     * If "Timeout=0" => Timeout = TTInfiniteWait.
     * The MinNumberOfThreads shall be > 1.
     * @return true if all the parameters are available and valid (i.e. MinNumberOfThreads > 1).
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief If the current number of allocated threads is < GetMaximumNumberOfPoolThreads() allocates a new thread.
     * @return ErrorManagement::IllegalOperation if the number of allocated threads is >= GetMaximumNumberOfPoolThreads()
     * @post
     *   GetNumberOfActiveThreads()++
     */
    virtual ErrorManagement::ErrorType AddThread();

    /**
     * @brief Remove a the EmbeddedThreadI with GetThreadId=threadId from the list of active threads.
     * @param[in] threadId the identifier of the thread to be removed.
     * @return ErrorManagement::NoError if the thread can be successfully removed from the list of active threads.
     * @post
     *   GetNumberOfActiveThreads()--
     */
    virtual ErrorManagement::ErrorType RemoveThread(const ThreadIdentifier threadId);

    /**
     * @brief Gets the maximum number of threads available in the pool.
     * @return the maximum number of threads available in the pool.
     */
    uint16 GetMaximumNumberOfPoolThreads() const;

    /**
     * @brief Gets the minimum number of threads available in the pool.
     * @return the minimum number of threads available in the pool.
     */
    uint16 GetMinimumNumberOfPoolThreads() const;

    /**
     * @brief Sets the maximum number of threads available in the pool.
     * @param[in] maxNumberOfThreadsIn the maximum number of threads available in the pool.
     * @pre
     *   Stop() &&
     *   maxNumberOfThreadsIn > GetMinimumNumberOfPoolThreads()
     */
    void SetMaximumNumberOfPoolThreads(const uint16 maxNumberOfThreadsIn);

    /**
     * @brief Sets the minimum number of threads available in the pool.
     * @param[in] minNumberOfThreadsIn the minimum number of threads available in the pool.
     * @pre
     *   Stop() &&
     *   minNumberOfThreadsIn < GetMaximumNumberOfPoolThreads() &&
     *   minNumberOfThreadsIn > 0
     *
     */
    void SetMinimumNumberOfPoolThreads(const uint16 minNumberOfThreadsIn);

    /**
     * @brief Checks if sufficient threads have already been allocated and thus the thread calling this function can be destroyed.
     * @return true if sufficient threads have already been allocated, i.e. if the current number of threads running > GetMinimumNumberOfPoolThreads().
     */
    inline bool MoreThanEnoughThreads();

    /**
     * @brief Starts N (GetMinimumNumberOfPoolThreads()) MultiClientEmbeddedThread instances.
     * @return ErrorManagement::NoError if all the instances can be successfully started.
     * ErrorManagement::IllegalOperation if start is called twice without calling stop beforehand.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Gets the number of active threads.
     * @return the number of active threads.
     */
    uint16 GetNumberOfActiveThreads();

    /**
     * @brief Sets the thread priority class.
     * @param[in] priorityClassIn the thread priority class.
     * @pre
     *   threadPool.Size() == 0
     */
    virtual void SetPriorityClass(Threads::PriorityClassType priorityClassIn);

    /**
     * @brief Sets the thread priority level.
     * @param[in] priorityLevelIn the thread priority level.
     * @pre
     *   threadPool.Size() == 0
     */
    virtual void SetPriorityLevel(uint8 priorityLevelIn);

    /**
     * @brief Sets the thread CPU mask (i.e. thread affinity).
     * @param[in] cpuMaskIn the thread CPU mask (i.e. thread affinity).
     * @pre
     *   threadPool.Size() == 0
     */
    virtual void SetCPUMask(const ProcessorType& cpuMaskIn);

protected:
    /**
     * Maximum number of pool threads.
     */
    uint16 maxNumberOfThreads;

    /**
     * Minimum number of pool threads.
     */
    uint16 minNumberOfThreads;

    /*lint -e{1712} This class does not have a default constructor because
     * the callback method must be defined at construction time and will remain constant
     * during the object's lifetime*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool MultiClientService::MoreThanEnoughThreads() {
    return (threadPool.Size() > minNumberOfThreads);
}

template<typename className>
MultiClientService::MultiClientService(EmbeddedServiceMethodBinderT<className> &binder) :
        MultiThreadService(binder) {
    minNumberOfThreads = 1u;
    maxNumberOfThreads = 3u;
}

}
#endif /* MULTICLIENTSERVICE_H_ */

