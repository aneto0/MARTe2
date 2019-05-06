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
    highestHandle = -1;
}

Select::~Select() {
}

bool Select::AddReadHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

bool Select::AddWriteHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

bool Select::AddExceptionHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

bool Select::RemoveReadHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

bool Select::RemoveWriteHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

bool Select::RemoveExceptionHandle(const HandleI &handle) {
    bool retVal = true;
    return retVal;
}

void Select::ClearAllHandles() {
}

bool Select::IsSet(const HandleI &handle) const {
    bool retVal = false;
    return retVal;
}

int32 Select::WaitUntil(const TimeoutType &timeout) {

    int32 retSel = -1;
    return retSel;
}
}
