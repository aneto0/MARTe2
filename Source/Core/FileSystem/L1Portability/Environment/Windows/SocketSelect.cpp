/**
 * @file SocketSelect.cpp
 * @brief Source file for class SocketSelect
 * @date Nov 24, 2015
 * @author Frank Perez Paz
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
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>
#include <winsock.h>
#include <windows.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SocketSelect.h"
#include "BasicSocket.h"
#include "ErrorManagement.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const int32 SELECT_WIDTH = 256;

SocketSelect::SocketSelect() {
    Reset();
}

void SocketSelect::Reset() {
    FD_ZERO(&(selectHandle.readFDS));
    FD_ZERO(&(selectHandle.writeFDS));
    FD_ZERO(&(selectHandle.exceptFDS));
    FD_ZERO(&(selectHandle.readFDS_done));
    FD_ZERO(&(selectHandle.writeFDS_done));
    FD_ZERO(&(selectHandle.exceptFDS_done));
    readySockets = 0;
}

void SocketSelect::AddWaitOnWriteReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.writeFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }
}

void SocketSelect::DeleteWaitOnWriteReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.writeFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

}

void SocketSelect::AddWaitOnReadReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.readFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

}

void SocketSelect::DeleteWaitOnReadReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.readFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

}

void SocketSelect::AddWaitOnExceptReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.exceptFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

}

void SocketSelect::DeleteWaitOnExceptReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.exceptFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

}

bool SocketSelect::Wait(const TimeoutType &timeout) {
    selectHandle.readFDS_done = selectHandle.readFDS;
    selectHandle.writeFDS_done = selectHandle.writeFDS;
    selectHandle.exceptFDS_done = selectHandle.exceptFDS;

    timeval timeWait;
    if (timeout.IsFinite()) {
        timeWait.tv_sec = timeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, &selectHandle.writeFDS_done, &selectHandle.exceptFDS_done, &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, &selectHandle.writeFDS_done, &selectHandle.exceptFDS_done,
                              static_cast<struct timeval*>(NULL));
    }
    return (readySockets > 0);
}

bool SocketSelect::WaitRead(const TimeoutType &timeout) {
    selectHandle.readFDS_done = selectHandle.readFDS;

    timeval timeWait;
    if (timeout.IsFinite()) {
        timeWait.tv_sec = timeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, static_cast<fd_set*>(NULL), static_cast<fd_set*>(NULL), &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, static_cast<fd_set*>(NULL),static_cast<fd_set*>(NULL), static_cast<timeval *>(NULL));

    }
    return (readySockets > 0);
}

bool SocketSelect::WaitWrite(const TimeoutType &timeout) {
    selectHandle.writeFDS_done = selectHandle.writeFDS;

    timeval timeWait;
    if (timeout.IsFinite()) {
        timeWait.tv_sec = timeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), &selectHandle.writeFDS_done, static_cast<fd_set*>(NULL), &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), &selectHandle.writeFDS_done, static_cast<fd_set*>(NULL), static_cast<timeval *>(NULL));
    }
    //printf("readySockets = %ld errorSelect = %ld \n",readySockets, GetLastError());

    return (readySockets > 0);
}

bool SocketSelect::WaitExcept(const TimeoutType &timeout) {
    selectHandle.exceptFDS_done = selectHandle.exceptFDS;

    timeval timeWait;
    if (timeout.IsFinite()) {
        timeWait.tv_sec = timeout.GetTimeoutMSec() / 1000;
        timeWait.tv_usec = (timeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), static_cast<fd_set*>(NULL), &selectHandle.exceptFDS_done, &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), static_cast<fd_set*>(NULL), &selectHandle.exceptFDS_done, static_cast<timeval *>(NULL));
    }
    return (readySockets > 0);
}

int32 SocketSelect::ReadySockets() const {
    return readySockets;
}

bool SocketSelect::CheckRead(const BasicSocket * const s) {

    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.readFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

    return ret;
}

bool SocketSelect::CheckWrite(const BasicSocket * const s) {

    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.writeFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

    return ret;
}

bool SocketSelect::CheckExcept(const BasicSocket * const s) {
    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.exceptFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"SocketSelect: The input is NULL");
    }

    return ret;
}

}

