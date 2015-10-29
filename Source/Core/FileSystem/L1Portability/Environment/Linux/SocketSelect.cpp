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
#include <sys/time.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SocketSelect.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


static const int32 SELECT_WIDTH= 256;

/*lint -e{1566} . Justification: The object is initialized called another function */
SocketSelect::SocketSelect() {
    Reset();
}

/*lint -e{970} -e{1960} -e{9146} -e{529} -e{717} -e{909} . Justification: Operating system API are not linted.*/
/*lint -e{1762}  [MISRA C++ Rule 9-3-3]. Justification: The function member could be non-const in other operating system implementations*/
void SocketSelect::Reset() {
    FD_ZERO(&(selectHandle.readFDS));
    FD_ZERO(&(selectHandle.writeFDS));
    FD_ZERO(&(selectHandle.exceptFDS));
    FD_ZERO(&(selectHandle.readFDS_done));
    FD_ZERO(&(selectHandle.writeFDS_done));
    FD_ZERO(&(selectHandle.exceptFDS_done));
    readySockets = 0;
}

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
void SocketSelect::AddWaitOnWriteReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.writeFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }
}

/*lint -e{970} -e{502} -e{1924} -e{9130} -e{731} -e{703} -e{666}  . Justification: Operating system API are not linted.*/
void SocketSelect::DeleteWaitOnWriteReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.writeFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

}

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
void SocketSelect::AddWaitOnReadReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.readFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

}

/*lint -e{970} -e{502} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
void SocketSelect::DeleteWaitOnReadReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.readFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

}

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
void SocketSelect::AddWaitOnExceptReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_SET(s->connectionSocket, &selectHandle.exceptFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

}

/*lint -e{970} -e{502} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
void SocketSelect::DeleteWaitOnExceptReady(const BasicSocket * const s) {
    if (s != NULL) {
        if(s->IsValid()) {
            FD_CLR(s->connectionSocket, &selectHandle.exceptFDS);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

}

bool SocketSelect::Wait(const TimeoutType &msecTimeout) {
    selectHandle.readFDS_done = selectHandle.readFDS;
    selectHandle.writeFDS_done = selectHandle.writeFDS;
    selectHandle.exceptFDS_done = selectHandle.exceptFDS;

    timeval timeWait;
    if (msecTimeout.IsFinite()) {
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_usec = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, &selectHandle.writeFDS_done, &selectHandle.exceptFDS_done, &timeWait);

    }
    else {
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, &selectHandle.writeFDS_done, &selectHandle.exceptFDS_done,
                              static_cast<struct timeval*>(NULL));
    }
    return (readySockets > 0);
}

bool SocketSelect::WaitRead(const TimeoutType &msecTimeout) {
    selectHandle.readFDS_done = selectHandle.readFDS;

    timeval timeWait;
    if (msecTimeout.IsFinite()) {
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_usec = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, static_cast<fd_set*>(NULL), static_cast<fd_set*>(NULL), &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, &selectHandle.readFDS_done, static_cast<fd_set*>(NULL),static_cast<fd_set*>(NULL), static_cast<timeval *>(NULL));

    }
    return (readySockets > 0);
}

bool SocketSelect::WaitWrite(const TimeoutType &msecTimeout) {
    selectHandle.writeFDS_done = selectHandle.writeFDS;

    timeval timeWait;
    if (msecTimeout.IsFinite()) {
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_usec = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), &selectHandle.writeFDS_done, static_cast<fd_set*>(NULL), &timeWait);
    }
    else {
        readySockets = select(SELECT_WIDTH, static_cast<fd_set*>(NULL), &selectHandle.writeFDS_done, static_cast<fd_set*>(NULL), static_cast<timeval *>(NULL));

    }
    return (readySockets > 0);
}

bool SocketSelect::WaitExcept(const TimeoutType &msecTimeout) {
    selectHandle.exceptFDS_done = selectHandle.exceptFDS;

    timeval timeWait;
    if (msecTimeout.IsFinite()) {
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_sec = msecTimeout.GetTimeoutMSec() / 1000;
        /*lint -e{9117} -e{9114} -e{9125}  [MISRA C++ Rule 5-0-3] [MISRA C++ Rule 5-0-4]. Justification: the time structure requires a signed integer. */
        timeWait.tv_usec = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
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

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Another Operating system could have another implementation of this function. */
bool SocketSelect::CheckRead(const BasicSocket * const s) {

    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.readFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

    return ret;
}

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Another Operating system could have another implementation of this function. */
bool SocketSelect::CheckWrite(const BasicSocket * const s) {

    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.writeFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

    return ret;
}

/*lint -e{970} -e{1924} -e{9130} -e{731} -e{703} -e{666} . Justification: Operating system API are not linted.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Another Operating system could have another implementation of this function. */
bool SocketSelect::CheckExcept(const BasicSocket * const s) {

    bool ret = false;
    if (s != NULL) {
        if(s->IsValid()) {
            ret=FD_ISSET(s->connectionSocket, &selectHandle.exceptFDS_done);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError,"Error: The socket handle is not valid");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError,"Error: The input is NULL");
    }

    return ret;
}


}
