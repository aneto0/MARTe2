/**
 * @file Select.cpp
 * @brief Source file for class Select
 * @date Dec 2, 2015
 * @author CCS Portable Station
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
 * the class Select (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <winsock2.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Select.h"
#include "TimeoutType.h"
#include "ErrorManagement.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Select::Select() {
    //Note that the readHandle will contain the read and the write handles (see WaitUntil which accepts only one array)
    WSADATA wsaData;
    int32 err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::Select(), Fail WSAStartup.");
    }
    readHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    writeHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    writeHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    writeHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    exceptionHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    exceptionHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    exceptionHandle.selectedHandle = reinterpret_cast<Handle>(-1);

    for (uint32 i = 0; i < MAXIMUM_WAIT_OBJECTS; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;

        writeHandle.registeredHandles[i] = NULL;
        writeHandle.selectHandles[i] = NULL;

        exceptionHandle.registeredHandles[i] = NULL;
        exceptionHandle.selectHandles[i] = NULL;
    }
    highestHandle = 0;
}

Select::~Select() {
    delete[] readHandle.registeredHandles;
    delete[] readHandle.selectHandles;

    delete[] writeHandle.registeredHandles;
    delete[] writeHandle.selectHandles;

    delete[] exceptionHandle.registeredHandles;
    delete[] exceptionHandle.selectHandles;
}

bool Select::AddReadHandle(const HandleI &handle) {
    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    Handle h = handle.GetReadHandle();
    int32 indexMax = 0L;
    if (reinterpret_cast<int32>(h) >= 0) {
        if (retVal) {
            while (readHandle.selectHandles[indexMax] != NULL) {
                indexMax++;
            }
            for (int32 i = 0; i < indexMax; i++) {
                if (readHandle.registeredHandles[i] == h) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(), Same Handle.");
                    retVal = false;
                }
            }
        }
        if (retVal) {
            WSAEVENT wsaevent;
            wsaevent = WSACreateEvent();
            if (wsaevent != WSA_INVALID_EVENT) {
                int32 hSocket = (int32) h;
                int32 error = WSAEventSelect(hSocket, wsaevent, FD_READ);
                if (error != SOCKET_ERROR) {
                    readHandle.registeredHandles[indexMax] = wsaevent;
                }
                else {
                    readHandle.registeredHandles[indexMax] = h;
                }
                readHandle.selectHandles[indexMax] = h;
            }

            highestHandle++;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddWriteHandle(const HandleI &handle) {
    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    Handle h = handle.GetWriteHandle();
    int32 indexMax = 0L;
    if (reinterpret_cast<int32>(h) >= 0) {
        if (retVal) {
            while (writeHandle.selectHandles[indexMax] != NULL) {
                indexMax++;
            }
            for (int32 i = 0; i < indexMax; i++) {
                if (writeHandle.registeredHandles[i] == h) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(), Same Handle.");
                    retVal = false;
                }
            }
        }
        if (retVal) {
            WSAEVENT wsaevent;
            wsaevent = WSACreateEvent();
            if (wsaevent != WSA_INVALID_EVENT) {
                int32 hSocket = (int32) h;
                int32 error = WSAEventSelect(hSocket, wsaevent, FD_READ);
                if (error != SOCKET_ERROR) {
                    writeHandle.registeredHandles[indexMax] = wsaevent;
                }
                else {
                    writeHandle.registeredHandles[indexMax] = h;
                }
                writeHandle.selectHandles[indexMax] = h;
            }

            highestHandle++;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddExceptionHandle(const HandleI &handle) {
    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    Handle h = handle.GetReadHandle();
    int32 indexMax = 0L;
    if (reinterpret_cast<int32>(h) >= 0) {
        if (retVal) {
            while (exceptionHandle.selectHandles[indexMax] != NULL) {
                indexMax++;
            }
            for (int32 i = 0; i < indexMax; i++) {
                if (exceptionHandle.registeredHandles[i] == h) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(), Same Handle.");
                    retVal = false;
                }
            }
        }
        if (retVal) {
            WSAEVENT wsaevent;
            wsaevent = WSACreateEvent();
            if (wsaevent != WSA_INVALID_EVENT) {
                int32 hSocket = (int32) h;
                int32 error = WSAEventSelect(hSocket, wsaevent, FD_READ);
                if (error != SOCKET_ERROR) {
                    exceptionHandle.registeredHandles[indexMax] = wsaevent;
                }
                else {
                    exceptionHandle.registeredHandles[indexMax] = h;
                }
                exceptionHandle.selectHandles[indexMax] = h;
            }

            highestHandle++;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::RemoveReadHandle(const HandleI &handle) {
    bool retVal = false;
    Handle h = handle.GetReadHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (readHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                readHandle.registeredHandles[i] = readHandle.registeredHandles[i + 1];
                readHandle.selectHandles[i] = readHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
}

bool Select::RemoveWriteHandle(const HandleI &handle) {
    bool retVal = false;
    Handle h = handle.GetWriteHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (writeHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                writeHandle.registeredHandles[i] = writeHandle.registeredHandles[i + 1];
                writeHandle.selectHandles[i] = writeHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
}

bool Select::RemoveExceptionHandle(const HandleI &handle) {
    bool retVal = false;
    Handle h = handle.GetReadHandle();
    if (reinterpret_cast<int32>(h) >= 0) {
        for (int32 i = 0; i < (MAXIMUM_WAIT_OBJECTS - 1); i++) {
            if (exceptionHandle.selectHandles[i] == h) {
                retVal = true;
            }
            if (retVal) {
                exceptionHandle.registeredHandles[i] = exceptionHandle.registeredHandles[i + 1];
                exceptionHandle.selectHandles[i] = exceptionHandle.selectHandles[i + 1];
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    if (retVal) {
        highestHandle--;
    }
    return retVal;
}

void Select::ClearAllHandles() {
    for (int32 i = 0; i < highestHandle; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;

        writeHandle.registeredHandles[i] = NULL;
        writeHandle.selectHandles[i] = NULL;

        exceptionHandle.registeredHandles[i] = NULL;
        exceptionHandle.selectHandles[i] = NULL;
    }
    highestHandle = 0;
    return;
}

bool Select::IsSet(const HandleI &handle) const {
    bool retVal = false;
    Handle hr = handle.GetReadHandle();
    Handle hw = handle.GetWriteHandle();
    if (reinterpret_cast<int32>(hr) >= 0) {
        if (readHandle.selectedHandle == hr) {
            retVal = true;
        }
        else if (readHandle.selectedHandle == hw) {
            retVal = true;
        }

        if (writeHandle.selectedHandle == hr) {
            retVal = true;
        }
        else if (writeHandle.selectedHandle == hw) {
            retVal = true;
        }

        if (exceptionHandle.selectedHandle == hr) {
            retVal = true;
        }
        else if (exceptionHandle.selectedHandle == hw) {
            retVal = true;
        }
    }
    return retVal;
}

int32 Select::WaitUntil(const TimeoutType &msecTimeout) {
    Handle * allHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    uint32 i = 0;
    uint32 aux = 0;
    while (readHandle.registeredHandles[i] != NULL) {
        allHandles[aux] = readHandle.registeredHandles[i];
        i++;
        aux++;
    }
    i = 0;
    while (writeHandle.registeredHandles[i] != NULL) {
        allHandles[aux] = writeHandle.registeredHandles[i];
        i++;
        aux++;
    }
    i = 0;
    while (exceptionHandle.registeredHandles[i] != NULL) {
        allHandles[aux] = exceptionHandle.registeredHandles[i];
        i++;
        aux++;
    }

    int32 ret = WaitForMultipleObjectsEx(static_cast<DWORD>(highestHandle), &allHandles[0], false, msecTimeout.GetTimeoutMSec(), true);
    if (ret == 0x102l) {
        ret = 0;
    }
    else if (ret != -1) {
        readHandle.selectedHandle = readHandle.selectHandles[ret];
        //In Windows only one handle can be identified as the source of the event
        ret = 1;
    }
    return ret;
}

}

