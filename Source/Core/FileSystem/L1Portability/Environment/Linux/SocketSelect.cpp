/**
 * @file SocketSelect.cpp
 * @brief Source file for class SocketSelect
 * @date 27/ott/2015
 * @author pc
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
 * the class SocketSelect (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SocketSelect.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** */
SocketSelect::SocketSelect() {
    Reset();
}

/** */
void SocketSelect::Reset() {
    FD_ZERO(&readFDS);
    FD_ZERO(&writeFDS);
    FD_ZERO(&exceptFDS);
    FD_ZERO(&readFDS_done);
    FD_ZERO(&writeFDS_done);
    FD_ZERO(&exceptFDS_done);
}

/** */
void SocketSelect::AddWaitOnWriteReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_SET(s->GetConnectionSocket(), &writeFDS);
}
/** */
void SocketSelect::DeleteWaitOnWriteReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_CLR(s->GetConnectionSocket(), &writeFDS);
}
/** */
void SocketSelect::AddWaitOnReadReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_SET(s->GetConnectionSocket(), &readFDS);
}
/** */
void SocketSelect::DeleteWaitOnReadReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_CLR(s->GetConnectionSocket(), &readFDS);
}
/** */
void SocketSelect::AddWaitOnExceptReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_SET(s->GetConnectionSocket(), &exceptFDS);
}
/** */
void SocketSelect::DeleteWaitOnExceptReady(BasicSocket *s) {
    if (s == NULL)
        return;
    FD_CLR(s->GetConnectionSocket(), &exceptFDS);
}
/** Wait for all the event*/
bool SocketSelect::Wait(TimeoutType msecTimeout) {
    readFDS_done = readFDS;
    writeFDS_done = writeFDS;
    exceptFDS_done = exceptFDS;

    timeval timeWait;
    if (msecTimeout == TTInfiniteWait) {
        readySockets = select(SELECT_WIDTH, &readFDS_done, &writeFDS_done, &exceptFDS_done, NULL);
    }
    else {
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
        readySockets = select(SELECT_WIDTH, &readFDS_done, &writeFDS_done, &exceptFDS_done, &timeWait);
    }
    return (readySockets > 0);
}

/** wait for data on the input */
bool SocketSelect::WaitRead(TimeoutType msecTimeout) {
    readFDS_done = readFDS;

    timeval timeWait;
    if (msecTimeout == TTInfiniteWait) {
        readySockets = select(SELECT_WIDTH, &readFDS_done, NULL, NULL, NULL);
    }
    else {
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
        readySockets = select(SELECT_WIDTH, &readFDS_done, NULL, NULL, &timeWait);
    }
    return (readySockets > 0);
}

/** wait on free space on the output buffer*/
bool SocketSelect::WaitWrite(TimeoutType msecTimeout) {
    writeFDS_done = writeFDS;

    timeval timeWait;
    if (msecTimeout == TTInfiniteWait) {
        readySockets = select(SELECT_WIDTH, NULL, &writeFDS_done, NULL, NULL);
    }
    else {
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
        readySockets = select(SELECT_WIDTH, NULL, &writeFDS_done, NULL, &timeWait);
    }
    return (readySockets > 0);
}

/** wait for an exception */
bool SocketSelect::WaitExcept(TimeoutType msecTimeout) {
    exceptFDS_done = exceptFDS;

    timeval timeWait;
    if (msecTimeout == TTInfiniteWait) {
        readySockets = select(SELECT_WIDTH, NULL, NULL, &exceptFDS_done, NULL);
    }
    else {
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = 1000 * (msecTimeout.GetTimeoutMSec() - (timeWait.tv_sec * 1000));
        readySockets = select(SELECT_WIDTH, NULL, NULL, &exceptFDS_done, &timeWait);
    }
    return (readySockets > 0);
}

/** */
int32 SocketSelect::ReadySockets() {
    return readySockets;
}

/** */
bool SocketSelect::CheckRead(BasicSocket *s) {
    return (FD_ISSET(s->GetConnectionSocket(), &readFDS_done) != 0);
}

/** */
bool SocketSelect::CheckWrite(BasicSocket *s) {
    return (FD_ISSET(s->GetConnectionSocket(), &writeFDS_done) != 0);
}

/** */
bool SocketSelect::CheckExcept(BasicSocket *s) {
    return (FD_ISSET(s->GetConnectionSocket(), &exceptFDS_done) != 0);
}

/** */
fd_set &SocketSelect::ReadFDS() {
    return readFDS_done;
}

/** */
fd_set &SocketSelect::WriteFDS() {
    return readFDS_done;
}

/** */
fd_set &SocketSelect::ExceptFDS() {
    return readFDS_done;
}

}
