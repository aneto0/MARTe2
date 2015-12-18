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
#include <windows.h>
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Select.h"
#include "TimeoutType.h"
#include "ErrorManagement.h"

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
        WSACleanup();
        REPORT_ERROR(ErrorManagement::FatalError, "Select::Select(), Fail WSAStartup.");
    }
    readHandle.registeredHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectHandles = new Handle[MAXIMUM_WAIT_OBJECTS];
    readHandle.selectedHandle = reinterpret_cast<Handle>(-1);
    for (uint32 i = 0; i < MAXIMUM_WAIT_OBJECTS; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;
    }
    highestHandle = 0;
}

Select::~Select() {
    delete[] readHandle.registeredHandles;
    delete[] readHandle.selectHandles;
    WSACleanup();
}

bool Select::AddReadHandle(const HandleI &handle) {
    Handle h = handle.GetReadHandle();
    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    if (reinterpret_cast<int32>(h) >= 0) {
        if (retVal) {
            for (int32 i = 0; i < highestHandle; i++) {
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
                    readHandle.registeredHandles[highestHandle] = wsaevent;
                }
                else {
                    readHandle.registeredHandles[highestHandle] = h;
                }
                readHandle.selectHandles[highestHandle] = h;
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
    Handle h = handle.GetWriteHandle();
    bool retVal = ((highestHandle + 1) < MAXIMUM_WAIT_OBJECTS);
    if (reinterpret_cast<int32>(h) >= 0) {
        printf("Is valid handle %d\n",h);
        if (retVal) {
            for (int32 i = 0; i < highestHandle; i++) {
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
                    readHandle.registeredHandles[highestHandle] = wsaevent;
                    printf("saved as event = %d\n",wsaevent);
                }
                else {
                    readHandle.registeredHandles[highestHandle] = h;
                    printf("saved as handle = %d\n",h);
                }
                readHandle.selectHandles[highestHandle] = h;
                highestHandle++;
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddHandle(). Invalid descriptor.");
        retVal = false;
    }
    printf("return %d highestHandle=%d\n",retVal,highestHandle);
    return retVal;
}

bool Select::AddExceptionHandle(const HandleI &handle) {
    return AddReadHandle(handle);
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

bool Select::RemoveExceptionHandle(const HandleI &handle) {
    return RemoveReadHandle(handle);
}

void Select::ClearAllHandles() {
    for (int32 i = 0; i < MAXIMUM_WAIT_OBJECTS; i++) {
        readHandle.registeredHandles[i] = NULL;
        readHandle.selectHandles[i] = NULL;
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
    }
    return retVal;
}

int32 Select::WaitUntil(const TimeoutType &msecTimeout) {
printf("handle in waitUltil = %d\n" ,readHandle.registeredHandles[0]);
    int32 ret = WaitForMultipleObjectsEx(static_cast<DWORD>(highestHandle), &readHandle.registeredHandles[0], false, msecTimeout.GetTimeoutMSec(), true);
    printf("ret=%d ErorN=%d timeout=%d\n",ret,GetLastError(),msecTimeout.GetTimeoutMSec());
    if (ret == 0x102L) {
        ret = 0;
    }
    else if (ret != -1) {
        readHandle.selectedHandle = readHandle.selectHandles[ret];
        //In Windows only one handle can be identified as the source of the event
        ret = 1;
    }
    printf("return of WaitUntil=%ld\n",ret);
    return ret;
}

}

