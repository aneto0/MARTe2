/**
 * @file QueuedMessageI.h
 * @brief Header file for class QueuedMessageI
 * @date 22/04/2016
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

 * @details This header file contains the declaration of the class QueuedMessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEDMESSAGEI_H_
#define QUEUEDMESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"
#include "ReferenceContainer.h"
#include "QueueingMessageFilter.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief MessageI queued implementation.
 * @details Messages consumed by this MessageI are processed in the context of a thread.
 * A QueueingMessageFilter is installed (MessageI::InstallMessageFilter) and the thread blocks until
 *  a new message is consumed by this queue (QueueingMessageFilter::GetMessage). This message is then
 *  propagated to all the filter that were added to this QueuedMessageI (see InstallMessageFilterInQueue).
 */
class QueuedMessageI: public MessageI {
public:

    /**
     * @brief Constructor.
     * @details Installs a QueueingMessageFilter in the parent MessageI.
     */
    QueuedMessageI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~QueuedMessageI();

    /**
     * @brief Starts the EmbeddedThread that consumes the messages sent to this QueuedMessageI.
     * @return ErrorManagement::NoError if EmbeddedThread can be successfully started.
     */
    ErrorManagement::ErrorType Start();

    /**
     * @brief Stops the EmbeddedThread that is consuming the messages sent to this QueuedMessageI.
     * @return ErrorManagement::NoError if EmbeddedThread can be successfully stopped.
     */
    ErrorManagement::ErrorType Stop();

    /**
     * @brief Installs a message filter that is capable of handling messages addressed to this QueuedMessageI.
     * @param[in] messageFilter a reference to the filter to be installed.
     * @param[in] position the position of the filter. Filters with lower position will handle messages before.
     * @return ErrorManagement::NoError if the filter can be successfully installed.
     * @pre
     *   messageFilter.IsValid()
     */
    /*lint -e(1735) the derived classes shall use this default parameter or no default parameter at all*/
    virtual ErrorManagement::ErrorType InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,
                                                            const int32 position = 0);

    /**
     * @brief Removes a previously installed message filter (see InstallMessageFilterInQueue).
     * @param[in] messageFilter a reference to the filter to be removed.
     * @return ErrorManagement::NoError if the filter can be successfully installed.
     * @pre
     *   messageFilter.IsValid()
     */
    virtual ErrorManagement::ErrorType RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter);

protected:
    /**
     * @brief Proxy method to update the name of the SingleThreadService.
     * @param[in] name the name of the queue to set.
     */
    void SetQueueName(const char8 * const name);

    /**
     * @brief Purge all the installed filters.
     */
    void PurgeFilters();

private:

    /**
     * @brief Callback thread function that consumes the messages from the QueueingMessageFilter.
     * @param[in] info the ExecutionInfo for the processing thread.
     * @return ErrorManagement::NoError if the message can be get from the QueueingMessageFilter. Note that a timeout
     *  is not considered to be an error (thread is expected to Poll).
     */
    ErrorManagement::ErrorType QueueProcessing(ExecutionInfo &info);

    /**
     * Message pool that contains the filters that handle the requests in QueueProcessing.
     */
    MessageFilterPool queuedMessageFilters;

    /**
     * The QueueingMessageFilter that is installed in the parent MessageI.
     */
    ReferenceT<QueueingMessageFilter> queue;

    /**
     * EmbeddedThread that executes the QueueProcessing.
     */
    SingleThreadService queueProcessingThread;

    /**
     * Binds the QueueProcessing against the queueProcessingThread.
     */
    EmbeddedServiceMethodBinderT<QueuedMessageI> binder;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* QUEUEDMESSAGEI_H_ */

