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
    FD_ZERO(&readHandle);
    FD_ZERO(&writeHandle);
    FD_ZERO(&exceptionHandle);
    highestHandle = -1;
}

Select::~Select() {
}

bool Select::AddReadHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetReadHandle());

    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &readHandle)) {
            FD_SET(descriptor, &readHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddReadHandle(). The descriptor is still in the readHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddReadHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddWriteHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetWriteHandle());
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &writeHandle)) {
            FD_SET(descriptor, &writeHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddWriteHandle(). The descriptor is still in the writeHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddWriteHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddExceptionHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetReadHandle());
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &exceptionHandle)) {
            FD_SET(descriptor, &exceptionHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddExceptionHandle(). The descriptor is still in the exceptionHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
        retVal = false;
    }
    //In the case of the BasicConsle also the write is added to exception
    if (descriptor != reinterpret_cast<int32>(handle.GetWriteHandle())) {
        descriptor = reinterpret_cast<int32>(handle.GetWriteHandle());
        if (descriptor >= 0) {
            if (!FD_ISSET(descriptor, &exceptionHandle)) {
                FD_SET(descriptor, &exceptionHandle);
                if (descriptor > highestHandle) {
                    highestHandle = descriptor;
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "Select::AddExceptionHandle(). The descriptor is still in the exceptionHandle.");
                retVal = false;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
            retVal = false;
        }
    }
    return retVal;
}

bool Select::RemoveReadHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetReadHandle());
    if (descriptor >= 0) {
        if (FD_ISSET(descriptor, &readHandle)) {
            FD_CLR(descriptor, &readHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is not in the readHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is invalid.");
    }
    return retVal;
}

bool Select::RemoveWriteHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetWriteHandle());
    if (descriptor >= 0) {
        if (FD_ISSET(descriptor, &writeHandle)) {
            FD_CLR(descriptor, &writeHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is not in the writeHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is invalid.");
    }
    return retVal;
}

bool Select::RemoveExceptionHandle(const HandleI &handle) {
    bool retVal = true;
    int32 descriptor = reinterpret_cast<int32>(handle.GetReadHandle());
    if (descriptor >= 0) {
        if (FD_ISSET(descriptor, &exceptionHandle)) {
            FD_CLR(descriptor, &exceptionHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is not in the exceptionHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is invalid.");
    }
    //In the case of the BasicConsle also the write is removed from exception
    if (descriptor != reinterpret_cast<int32>(handle.GetWriteHandle())) {
        descriptor = reinterpret_cast<int32>(handle.GetWriteHandle());
        if (descriptor >= 0) {
            if (FD_ISSET(descriptor, &exceptionHandle)) {
                /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
                FD_CLR(descriptor, &exceptionHandle);
            }
            else {
                REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is not in the exceptionHandle.");
                retVal = false;
            }
        }
        else {
            retVal = false;
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is invalid.");
        }
    }
    return retVal;
}

void Select::ClearAllHandles() {
    FD_ZERO(&readHandle);
    FD_ZERO(&writeHandle);
    FD_ZERO(&exceptionHandle);
    highestHandle = 0;
    return;
}

bool Select::IsSet(const HandleI &handle) const {
    int32 descriptor = reinterpret_cast<int32>(handle.GetReadHandle());
    bool retVal = FD_ISSET(descriptor, &readHandle);
    if (!retVal) {
        retVal = FD_ISSET(descriptor, &writeHandle);
    }
    if (!retVal) {
        retVal = FD_ISSET(descriptor, &exceptionHandle);
    }
    if (!retVal) {
        //It checks if it has a double handle (i.e. a different handle for reading and writing).
        if (descriptor != reinterpret_cast<int32>(handle.GetWriteHandle())) {
            descriptor = reinterpret_cast<int32>(handle.GetWriteHandle());
            retVal = FD_ISSET(descriptor, &readHandle);
            if (!retVal) {
                retVal = FD_ISSET(descriptor, &writeHandle);
            }
            if (!retVal) {
                retVal = FD_ISSET(descriptor, &exceptionHandle);
            }
        }
    }
    return retVal;
}

int32 Select::WaitUntil(const TimeoutType &msecTimeout) {
    struct timeval timeoutWindows;
    int32 retSel = -1;
    while ((highestHandle >= 0) && (!FD_ISSET(highestHandle, &readHandle)) && (!FD_ISSET(highestHandle, &writeHandle))
            && (!FD_ISSET(highestHandle, &exceptionHandle))) {
        highestHandle--;
    }
    if (highestHandle < 0) {
        REPORT_ERROR(ErrorManagement::Information, "Select::WaitUntil(). The highestHandle is negative.");
        retSel = -1;
    }
    else {
        timeoutWindows.tv_usec = static_cast<int64>(msecTimeout.GetTimeoutMSec()) * 1000;
        timeoutWindows.tv_sec = static_cast<int64>(msecTimeout.GetTimeoutMSec()) / 1000;
        retSel = select(highestHandle + 1, &readHandle, &writeHandle, &exceptionHandle, &timeoutWindows);
    }
    return retSel;
}

}


	
