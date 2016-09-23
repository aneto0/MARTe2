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
 * This method will be continuously called (see Start) with the stage encoded in the Information parameter. In particular this class allows to request for a "kind" Stop of the
 * embedded thread and, if this fails, for a direct killing of the thread.
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
     * @brief Constructor.
     * @param[in] binder contains the function to be executed by this MultiThreadService.
     * @post
     *   GetNumberOfPoolThreads() == 1
     */
    template<typename className>
    MultiThreadService(EmbeddedServiceMethodBinderT<className> &binder);

    /**
     * @brief Destructor.
     * @post
     *   Stop()
     */
    virtual ~MultiThreadService();

    /**
     * @brief Reads the number of pool threads from the data input.
     * @param[in] data shall contain a parameter named "NumberOfPoolThreads" holding the number of pool threads
     * and another parameter named "Timeout" with the timeout to apply to each of the SingleThreadService instances.
     * If "Timeout=0" => Timeout = TTInfiniteWait
     * @return true if all the parameters are available.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Starts N (GetNumberOfPoolThreads()) SingleThreadService instances.
     * @return ErrorManagement::NoError if all the instances can be successfully started.
     * ErrorManagement::IllegalOperation if start is called twice without calling stop beforehand.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops N (GetNumberOfPoolThreads()) SingleThreadService instances.
     * @details A Kill will be issued if the SingleThreadService do not Stop at the first time.
     * @return ErrorManagement::NoError if all the instances can be successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop();

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
    EmbeddedThreadI::States GetStatus(uint32 threadIdx);

    /**
     * @brief Sets the maximum time to execute a state change.
     * @param[in] msecTimeout the maximum time in milliseconds to execute a state change.
     */
    void SetTimeout(TimeoutType msecTimeoutIn);

    /**
     * @brief Gets the maximum time to execute a state change.
     * @return the maximum time to execute a state change.
     */
    TimeoutType GetTimeout() const;

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
     * The timeout to apply to each of the SingleThreadService instances.
     */
    uint32 msecTimeout;

    /**
     * The callback method
     */
    EmbeddedServiceMethodBinderI &method;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * TODO
 */
template<typename className>
MultiThreadService::MultiThreadService(EmbeddedServiceMethodBinderT<className> &binder) :
        EmbeddedServiceI (),
        method(binder) {
    numberOfPoolThreads = 1;
    msecTimeout = TTInfiniteWait.GetTimeoutMSec();
}

}

#endif /* MULTITHREADSERVICE_H_ */

