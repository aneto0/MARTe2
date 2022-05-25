/**
 * @file ExecutableI.cpp
 * @brief Source file for class ExecutableI
 * @date 19/07/2016
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
 * the class ExecutableI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ExecutableI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ExecutableI::ExecutableI() {
    timingSignalAddress = NULL_PTR(uint32 * const);
    enabled = true;
}

/*lint -e{1540} the timingSignalAddress is to freed by the class that uses the ExecutableI, typically a GAMSchedulerI.*/
ExecutableI::~ExecutableI() {
}

void ExecutableI::SetTimingSignalAddress(uint32 * const timingSignalAddressIn) {
    timingSignalAddress = timingSignalAddressIn;
}

void ExecutableI::SetEnabled(const bool isEnabled) {
    enabled = isEnabled;
}

bool ExecutableI::IsEnabled() const {
    return enabled;
}


}
