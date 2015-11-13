/**
 * @file Select.cpp
 * @brief Source file for class Select
 * @date 12/11/2015
 * @author Llorenç Capellà
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "sys/select.h"

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

bool Select::AddReadHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    //Check that the descriptor is valid
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &readHandle)) {
            FD_SET(retVal, &readHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddReadHandle(). The descriptor is still in the readHandle.");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddReadHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddWriteHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &writeHandle)) {
            FD_SET(descriptor, &writeHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddWriteHandle(). The descriptor is still in the writeHandle.");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddWriteHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddExceptionHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &exceptionHandle)) {
            FD_SET(descriptor, &exceptionHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::AddExceptionHandle(). The descriptor is still in the exceptionHandle.");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
        retVal = false;
    }
    return retVal;
}

bool Select::RemoveReadHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &readHandle)) {
            FD_CLR(descriptor, &readHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is not in the readHandle.");
            return false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is invalid.");
    }
    return retVal;
}

bool Select::RemoveWriteHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &writeHandle)) {
            FD_CLR(descriptor, &writeHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is not in the writeHandle.");
            return false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is invalid.");
    }
    return retVal;
}

bool Select::RemoveExceptionHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        if (!FD_ISSET(descriptor, &exceptionHandle)) {
            FD_CLR(descriptor, &exceptionHandle);
        }
        else {
            REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is not in the exceptionHandle.");
            return false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is invalid.");
    }
    return retVal;
}

bool Select::ClearAllHandle() {
    bool retVal = true;
    FD_ZERO(&readHandle);
    FD_ZERO(&writeHandle);
    FD_ZERO(&exceptionHandle);
    highestHandle = 0;
    return retVal;
}

int32 Select::WaitUntil(TimeoutType timeout) {
    struct timeval timeoutLinux;
    timeoutLinux.tv_usec = static_cast<int64>(timeout.GetTimeoutMSec()) * 1000;
    return (select(highestHandle + 1, &readHandle, &writeHandle, &exceptionHandle, &timeoutLinux));
}

}/*End namespace MARTe*/

