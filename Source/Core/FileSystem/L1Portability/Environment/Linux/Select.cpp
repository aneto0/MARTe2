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
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{1401} .Justification: Removes the warning "not initialized by constructor [MISRA C++ Rule 8-5-1]". */
Select::Select() {
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.readHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.writeHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.exceptionHandle);
    selectProperties.highestHandle = -1;
}

Select::~Select() {
}


bool Select::Add(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent){
	return Add(socket.GetSocket(),readEvent,writeEvent,exceptEvent);
}

bool Select::Add(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent){
    bool retVal = true;
	int32 descriptor = handle;

    if (descriptor == 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "Select::AddselectProperties.readHandle(). Invalid descriptor.");
        retVal = false;
    } else {
        if (readEvent) {
            FD_SET(descriptor, &selectProperties.readHandle);
            if (descriptor > selectProperties.highestHandle) {
            	selectProperties.highestHandle = descriptor;
            }
        }
        if (writeEvent) {
            FD_SET(descriptor, &selectProperties.writeHandle);
            if (descriptor > selectProperties.highestHandle) {
            	selectProperties.highestHandle = descriptor;
            }
        }
        if (exceptEvent) {
            FD_SET(descriptor, &selectProperties.exceptionHandle);
            if (descriptor > selectProperties.highestHandle) {
            	selectProperties.highestHandle = descriptor;
            }
        }
    }
    return retVal;
}

bool Select::Remove(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent){
	return Remove(socket.GetSocket(),readEvent,writeEvent,exceptEvent);
}

bool Select::Remove(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent) {
    bool retVal = true;
    int32 descriptor = handle;
    if (descriptor == 0) {
        retVal = false;
        REPORT_ERROR(ErrorManagement::Information, "Select::RemoveselectProperties.readHandle(). The descriptor is invalid.");
    } else {
        if (readEvent) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &selectProperties.readHandle);
        }
        if (writeEvent) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &selectProperties.writeHandle);
        }
        if (exceptEvent) {
            /*lint -e{502} .Justification: Removes the warning "Expected unsigned type". */
            FD_CLR(descriptor, &selectProperties.exceptionHandle);
        }
    }
    return retVal;
}


void Select::ClearAllHandles() {
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.readHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.writeHandle);
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1960} .Justification: Removes the warning "Violates MISRA C++ 2008 Required Rule 17-0-2, Re-use of C++ identifier pattern: __d0". */
    /*lint -e{9146} .Justification: Removes the warning "multiple declarators in a declaration [MISRA C++ Rule 8-0-1]". */
    /*lint -e{529} .Justification: Removes the warning "Symbol '__d0' (line 49) not subsequently referenced [MISRA C++ Rule 0-1-3], [MISRA C++ Rule 0-1-4]". */
    /*lint -e{717} .Justification: Removes the warning "do ... while(0)". */
    /*lint -e{909} .Justification: Removes the warning "Implicit conversion from int to bool [MISRA C++ Rule 5-0-13], [MISRA C++ Rule 5-0-14], [MISRA C++ Rule 5-3-1]". */
    FD_ZERO(&selectProperties.exceptionHandle);
    selectProperties.highestHandle = 0;
    return;
}

bool Select::IsSet(const BasicSocket &socket,bool readEvent,bool writeEvent,bool exceptEvent) const{
	return IsSet(socket.GetSocket(),readEvent,writeEvent,exceptEvent);
}

bool Select::IsSet(const Handle &handle,bool readEvent,bool writeEvent,bool exceptEvent) const {
    int32 descriptor = handle;

    bool retVal = false;

    if (readEvent){
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
    	retVal = FD_ISSET(descriptor, &selectProperties.readHandle);
    }
    if (!retVal && writeEvent) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        retVal = FD_ISSET(descriptor, &selectProperties.writeHandle);
    }
    if (!retVal && exceptEvent) {
        /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
        /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
        /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
        /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
        retVal = FD_ISSET(descriptor, &selectProperties.exceptionHandle);
    }
    return retVal;
}

int32 Select::WaitUntil(const MilliSeconds &timeout) {

    int32 retSel = -1;
    /*lint -e{970} .Justification: Removes the warning "Use of modifier or type 'int' outside of a typedef [MISRA C++ Rule 3-9-2]". */
    /*lint -e{1924} .Justification: Removes the warning "C-style cast [MISRA C++ Rule 5-2-4]". */
    /*lint -e{9130} .Justification: Removes the warning "bitwise operator '<<' applied to signed underlying type [MISRA C++ Rule 5-0-21]". */
    /*lint -e{703} .Justification: Removes the warning "Shift left of signed quantity (long)". */
    while ((selectProperties.highestHandle >= 0) && (!FD_ISSET(selectProperties.highestHandle, &selectProperties.readHandle)) && (!FD_ISSET(selectProperties.highestHandle, &selectProperties.writeHandle))
            && (!FD_ISSET(selectProperties.highestHandle, &selectProperties.exceptionHandle))) {
        selectProperties.highestHandle--;
    }
    if (selectProperties.highestHandle < 0) {
        REPORT_ERROR(ErrorManagement::Information, "Select::WaitAll(). The selectProperties.highestHandle is negative.");
    }
    else {
        if (timeout.IsValid()) {
        	uint32 ms = timeout.GetTimeRaw();
        	uint32 s  = ms / 1000u;
        	ms = ms - (s * 1000u);
        	uint32 us = ms *1000u;

            struct timeval timeoutLinux;
            timeoutLinux.tv_usec = static_cast<__time_t>(s);
            timeoutLinux.tv_sec = static_cast<__time_t>(us);

            retSel = select(selectProperties.highestHandle + 1, &selectProperties.readHandle, &selectProperties.writeHandle, &selectProperties.exceptionHandle, &timeoutLinux);
        }
        else {
            retSel = select(selectProperties.highestHandle + 1, &selectProperties.readHandle, &selectProperties.writeHandle, &selectProperties.exceptionHandle, static_cast<timeval *>(NULL));
        }
    }
    return retSel;
}



}
