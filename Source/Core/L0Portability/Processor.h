/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Access to processor's information.
 */
#ifndef PROCESSOR_H
#define PROCESSOR_H

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

/**
 * @brief Functions to get the main processor informations.
 * 
 * These functions allows to get cpu informatios like family,
 * id, model and number of avaiable processors.
 *
 * Most of the implementation is delegated to ProcessorA.h and ProcessorOS.h.
 */


class Processor {
public:

    /** @brief Returns the processor type.
      * @return a string which contains the processor type. */
    static inline const char *VendorId() {
        return ProcessorVendorId();
    }

    /** @brief Returns the processor family. 
      * @return processor family. */
    static inline uint32 Family() {
        return ProcessorFamily();
    }

    /** @brief Returns the processor model.
      * @return processor model. */
    static inline uint32 Model() {
        return ProcessorModel();
    }

    /** @brief Returns the number of avaible cpus. 
      * @returns the number of avaible cpus. */
    static inline uint32 Available() {
        return ProcessorsAvailable();
    }
};

#endif

