/**
 * @file MessageFilterPool.cpp
 * @brief Source file for class MessageFilterPool
 * @date 22/08/2016
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

 * @details This source file contains the definition of all the methods for
 * the class MessageFilterPool (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageFilterPool.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
MessageFilterPool::MessageFilterPool() {

}

ErrorManagement::ErrorType MessageFilterPool::InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,
                                                                   int32 position) {

    ErrorManagement::ErrorType err;
    err.timeout = !Insert(messageFilter, position);
    return err;
}

ErrorManagement::ErrorType MessageFilterPool::RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter) {
    ErrorManagement::ErrorType err;
    err.timeout = !Delete(messageFilter);

    return err;
}

ErrorManagement::ErrorType MessageFilterPool::RemoveMessageFilter(CCString name) {
    ErrorManagement::ErrorType err;
    err.timeout = !Delete(name);

    return err;
}

ErrorManagement::ErrorType MessageFilterPool::FindMessageFilter(CCString name,
                                                                ReferenceT<MessageFilter> messageFilter) {
    messageFilter = Find(name);
    ErrorManagement::ErrorType err;
    err.unsupportedFeature = !messageFilter.IsValid();

    return err;

}

ErrorManagement::ErrorType MessageFilterPool::ReceiveMessage(ReferenceT<Message> &message) {
    bool matched = false;
    ErrorManagement::ErrorType err;
    ReferenceT<MessageFilter> messageFilter;

    uint32 i;
    for (i = 0; (i < Size() && !matched); i++) {
        messageFilter = Get(i);

        if (messageFilter.IsValid()) {
            err = messageFilter->ConsumeMessage(message);
            matched = err.ErrorsCleared();
        }
    }

    if (matched) {
        if (!messageFilter->IsPermanentFilter()) {
            err.timeout = !Delete(messageFilter);
        }
    }
    else {
        err.unsupportedFeature = true;
    }

    return err;
}

}
