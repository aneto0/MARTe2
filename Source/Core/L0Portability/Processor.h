/**
 * @file Processor.h
 * @brief Header file for class Processor
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class Processor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSOR_H_
#define 		PROCESSOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

extern "C" {

/** @see Processor::Family(). */
uint32 ProcessorFamily();

/** @see Processor::VendorId(). */
const char *ProcessorVendorId();

/** @see Processor::ProcessorsAvaiable(). */
uint32 ProcessorsAvailable();

/** @see Processor::Model(). */
uint32 ProcessorModel();
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Functions to get the processor's informations.
 *
 * @details These functions allow to get cpu informations like family,
 * id, model and number of available processors.
 *
 * @details Most of the implementation is delegated to ProcessorA.h and ProcessorOS.h.
 */
class Processor {
public:

    /**
     * @brief Returns the processor type.
     * @return a string which contains the processor type.
     */
    static inline const char *VendorId();

    /**
     * @brief Returns the processor family.
     * @return processor family.
     */
    static inline uint32 Family();

    /**
     * @brief Returns the processor model.
     * @return processor model.
     */
    static inline uint32 Model();

    /**
     * @brief Returns the number of available cpus.
     * @returns the number of available cpus.
     */
    static inline uint32 Available();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

const char *Processor::VendorId() {
    return ProcessorVendorId();
}

uint32 Processor::Family() {
    return ProcessorFamily();
}

uint32 Processor::Model() {
    return ProcessorModel();
}

uint32 Processor::Available() {
    return ProcessorsAvailable();
}

#endif /* PROCESSOR_H_ */

