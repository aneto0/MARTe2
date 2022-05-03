/**
 * @file Select.cpp
 * @brief Source file for class Select
 * @date 22/04/2019
 * @author Andre Neto
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
#ifdef LWIP_ENABLED
#include "sys/select.h"
#endif
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
#ifdef LWIP_ENABLED
    FD_ZERO(&readHandle);
    FD_ZERO(&writeHandle);
    FD_ZERO(&exceptionHandle);
#endif
    highestHandle = -1;
}

Select::~Select() {
}

bool Select::AddReadHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetReadHandle();
    //Check that the descriptor is valid
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (!FD_ISSET(descriptor, &readHandle)) {
            FD_SET(descriptor, &readHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::AddReadHandle(). The descriptor is still in the readHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Select::AddReadHandle(). Invalid descriptor.");
        retVal = false;
    }
#endif
    return retVal;
}

bool Select::AddWriteHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetWriteHandle();
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (!FD_ISSET(descriptor, &writeHandle)) {
            FD_SET(descriptor, &writeHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::AddWriteHandle(). The descriptor is still in the writeHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Select::AddWriteHandle(). Invalid descriptor.");
        retVal = false;
    }
#endif
    return retVal;
}

bool Select::AddExceptionHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetReadHandle();
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (!FD_ISSET(descriptor, &exceptionHandle)) {
            FD_SET(descriptor, &exceptionHandle);
            if (descriptor > highestHandle) {
                highestHandle = descriptor;
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::AddExceptionHandle(). The descriptor is still in the exceptionHandle.");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
        retVal = false;
    }
    //In the case of the BasicConsle also the write is added to exception
    if (descriptor != handle.GetWriteHandle()) {
        descriptor = handle.GetWriteHandle();
        if (descriptor >= 0) {
            /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
            /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
            /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
            /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
            if (!FD_ISSET(descriptor, &exceptionHandle)) {
                FD_SET(descriptor, &exceptionHandle);
                if (descriptor > highestHandle) {
                    highestHandle = descriptor;
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::AddExceptionHandle(). The descriptor is still in the exceptionHandle.");
                retVal = false;
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Select::AddExceptionHandle(). Invalid descriptor.");
            retVal = false;
        }
    }
#endif
    return retVal;
}

bool Select::RemoveReadHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetReadHandle();
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (FD_ISSET(descriptor, &readHandle)) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &readHandle);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is not in the readHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveReadHandle(). The descriptor is invalid.");
    }
#endif
    return retVal;
}

bool Select::RemoveWriteHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetWriteHandle();
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (FD_ISSET(descriptor, &writeHandle)) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &writeHandle);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is not in the writeHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveWriteHandle(). The descriptor is invalid.");
    }
#endif
    return retVal;
}

bool Select::RemoveExceptionHandle(const HandleI &handle) {
    bool retVal = true;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetReadHandle();
    if (descriptor >= 0) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        if (FD_ISSET(descriptor, &exceptionHandle)) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &exceptionHandle);
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is not in the exceptionHandle.");
            retVal = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is invalid.");
    }
    //In the case of the BasicConsle also the write is removed from exception
    if (descriptor != handle.GetWriteHandle()) {
        descriptor = handle.GetWriteHandle();
        if (descriptor >= 0) {
            /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
            /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
            /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
            /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
            if (FD_ISSET(descriptor, &exceptionHandle)) {
                /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
                FD_CLR(descriptor, &exceptionHandle);
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is not in the exceptionHandle.");
                retVal = false;
            }
        }
        else {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::RemoveExceptionHandle(). The descriptor is invalid.");
        }
    }
#endif
    return retVal;
}

void Select::ClearAllHandles() {
#ifdef LWIP_ENABLED
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&readHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&writeHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&exceptionHandle);
    highestHandle = 0;
#endif
}

bool Select::IsSet(const HandleI &handle) const {
    bool retVal = false;
#ifdef LWIP_ENABLED
    int32 descriptor = handle.GetReadHandle();
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
    /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
    /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
    retVal = FD_ISSET(descriptor, &readHandle);
    if (!retVal) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        retVal = FD_ISSET(descriptor, &writeHandle);
    }
    if (!retVal) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        retVal = FD_ISSET(descriptor, &exceptionHandle);
    }
    if (!retVal) {
        //It checks if it has a double handle (i.e. a different handle for reading and writing).
        if (handle.GetWriteHandle() != descriptor) {
            descriptor = handle.GetWriteHandle();
            /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
            /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
            /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
            /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
            retVal = FD_ISSET(descriptor, &readHandle);
            if (!retVal) {
                /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
                /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
                /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
                /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
                retVal = FD_ISSET(descriptor, &writeHandle);
            }
            if (!retVal) {
                /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
                /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
                /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
                /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
                retVal = FD_ISSET(descriptor, &exceptionHandle);
            }
        }
    }
#endif

    return retVal;
}

int32 Select::WaitUntil(const TimeoutType &timeout) {

    int32 retSel = -1;
#ifdef LWIP_ENABLED
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
    /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
    /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
    while ((highestHandle >= 0) && (!FD_ISSET(highestHandle, &readHandle)) && (!FD_ISSET(highestHandle, &writeHandle))
            && (!FD_ISSET(highestHandle, &exceptionHandle))) {
        highestHandle--;
    }
    if (highestHandle < 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Select::WaitAll(). The highestHandle is negative.");
    }
    else {
        if (timeout.IsFinite()) {
            struct timeval timeoutLinux;
            /*lint -e{9114} .Justification: Removes the warning "implicit conversion of integer cvalue expression [MISRA C++ Rule 5-0-3]". */
            timeoutLinux.tv_usec = static_cast<int64>(timeout.GetTimeoutMSec()) * 1000;
            /*lint -e{9114} .Justification: Removes the warning "implicit conversion of integer cvalue expression [MISRA C++ Rule 5-0-3]". */
            timeoutLinux.tv_sec = static_cast<int64>(timeout.GetTimeoutMSec()) / 1000;
            retSel = select(highestHandle + 1, &readHandle, &writeHandle, &exceptionHandle, &timeoutLinux);
        }
        else {
            retSel = select(highestHandle + 1, &readHandle, &writeHandle, &exceptionHandle, static_cast<timeval *>(NULL));
        }
    }
#endif
    return retSel;
}
}
