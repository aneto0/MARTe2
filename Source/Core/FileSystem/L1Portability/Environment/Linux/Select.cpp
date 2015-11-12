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

Select::Select() :
        MAX_numberHandle(FD_SETSIZE) {
    FD_ZERO(&readHandle);
    numberOfSetReadHandle = 0;
    FD_ZERO(&writeHandle);
    numberOfSetWriteHandle = 0;
    FD_ZERO(&exceptionsHandle);
    numberOfSetExceptionsHandle = 0;
}

bool Select::AddReadHandle(const StreamI &stream) {
    bool retVal = true;
    //verify that another handle can be added
    if (numberOfSetReadHandle < MAX_numberHandle) {
        int32 descriptor = stream.GetHandle();
        //Check that the descriptor is valid
        if (descriptor >= 0) {
            FD_SET(retVal, &readHandle);
            numberOfSetReadHandle++;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Select::AddReadHandle(). Invalid descriptor.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddReadHandle(). readHandle cannot hold more handles.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddWriteHandle(const StreamI &stream) {
    bool retVal = true;
    if (numberOfSetWriteHandle < MAX_numberHandle) {
        int32 descriptor = stream.GetHandle();
        if (descriptor >= 0) {
            FD_SET(descriptor, &writeHandle);
            numberOfSetWriteHandle++;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Select::AddWriteHandle(). Invalid descriptor.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddWriteHandle(). writeHandle cannot hold more handles.");
        retVal = false;
    }
    return retVal;
}

bool Select::AddExceptionHandle(const StreamI &stream) {
    bool retVal = true;
    if (numberOfSetExceptionsHandle < MAX_numberHandle) {
        int32 descriptor = stream.GetHandle();
        if (descriptor >= 0) {
            FD_SET(descriptor, &writeHandle);
            numberOfSetExceptionsHandle++;
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddExceptionHandle(). exceptionHandle cannot hold more handles.");
        retVal = false;
    }
    return retVal;
}

bool Select::RemoveReadHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        FD_CLR(descriptor, &readHandle);
        numberOfSetReadHandle--;
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool Select::RemoveWriteHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        FD_CLR(descriptor, &writeHandle);
        numberOfSetWriteHandle--;
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool Select::RemoveExceptionHandle(const StreamI &stream) {
    bool retVal = true;
    int32 descriptor = stream.GetHandle();
    if (descriptor >= 0) {
        FD_CLR(descriptor, &exceptionsHandle);
        numberOfSetExceptionsHandle--;
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool Select::ClearAllHandle() {
    bool retVal = true;
    FD_ZERO(&readHandle);
    numberOfSetReadHandle = 0;
    FD_ZERO(&writeHandle);
    numberOfSetWriteHandle = 0;
    FD_ZERO(&exceptionsHandle);
    numberOfSetExceptionsHandle = 0;
    return retVal;
}

int32 Select::WaitUntil(TimeoutType timeout) {
    int32 maximum = numberOfSetReadHandle;
    //Calculate the maximum
    if (numberOfSetWriteHandle > maximum) {
        maximum = numberOfSetWriteHandle;
    }
    if (numberOfSetExceptionsHandle > maximum) {
        maximum = numberOfSetExceptionsHandle;
    }
    struct timeval timeoutLinux;
    timeoutLinux.tv_usec = static_cast<int64>(timeout.GetTimeoutMSec()) * 1000;
    return (select(maximum + 1, &readHandle, &writeHandle, &exceptionsHandle, &timeoutLinux));
}

}

