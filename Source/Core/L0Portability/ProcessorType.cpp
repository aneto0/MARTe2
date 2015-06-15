/**
 * @file ProcessorType.cpp
 * @brief Header file for class ProcessorType
 * @date 12/06/2015
 * @author Giuseppe Ferrò
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
 *
 * @details This header file contains the declaration of the class ProcessorType
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorType.h"

#if ProcessorTypeDefaultCPUs
uint32 ProcessorType::defaultCPUs = ProcessorTypeDefaultCPUs;
#else
uint32 ProcessorType::defaultCPUs = 0;
#endif

uint32 ProcessorTypeGetDefaultCPUs() {
    return ProcessorType::defaultCPUs;
}

void ProcessorTypeSetDefaultCPUs(uint32 defaultMask) {
    ProcessorType::defaultCPUs = defaultMask;
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

