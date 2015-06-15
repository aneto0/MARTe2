/**
 * @file Processor.cpp
 * @brief Header file for class Processor
 * @date 15/06/2015
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
 * @details This header file contains the declaration of the class Processor
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

#include "Processor.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,ProcessorOS.h)
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,ProcessorA.h)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/** @see ProcessorA::Family() */
uint32 ProcessorFamily() {
    return ProcessorA::Family();
}

/** @see Processor::VendorId(). */
const char *ProcessorVendorId() {
    return ProcessorA::VendorId();
}

/** @see Processor::Model(). */
uint32 ProcessorModel() {
    return ProcessorA::Model();
}

/** @see Processor::ProcessorsAvaiable(). */
uint32 ProcessorsAvailable() {
    return ProcessorOS::Available();
}
