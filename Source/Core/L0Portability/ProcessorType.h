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
 * @brief Defines the processors where a particular task should run
 *
 * e.g., if a task is to run on cpu 2 and 3 the mask is: 0x06 (00000110)
 */
#ifndef PROCESSOR_TYPE_H
#define PROCESSOR_TYPE_H

#include "GeneralDefinitions.h"

extern "C" {
uint32 ProcessorTypeGetDefaultCPUs();
void ProcessorTypeSetDefaultCPUs(uint32 defaultMask);
}

/** @brief Defines the processors where a particular task should run.
  *
  * cpus are setted by a mask of bits (32 bits max = 32 processors max). */

class ProcessorType {
public:
    /** The processor mask */
    uint32 processorMask;

    /** The default CPU mask. Initialised to zero.*/
    static uint32 defaultCPUs;

public:
    friend uint32 ProcessorTypeGetDefaultCPUs();
    friend void ProcessorTypeSetDefaultCPUs(uint32 mask);

#if !defined (_CINT)
    /** @brief Constructor from integer.
     *
     *  The default is to run the tasks all CPUs but the first.
     *  @param cpuMask is the cpu mask. */
    ProcessorType(const uint32 cpuMask = 0xFE) {
        processorMask = cpuMask;
    }

    /** @brief Constructor from another ProcessorType object */
    ProcessorType(const ProcessorType &pt) {
        processorMask = pt.processorMask;
    }

#endif

    /** @brief Set the processor mask.
      * @param mask is the cpu mask. */
    void SetMask(const uint32 mask) {
        processorMask = mask;
    }

    /** @brief Add a cpu to the mask.
      * @param cpuNumber is the number of the cpu to activate. */
    void AddCPU(const uint32 cpuNumber) {
        processorMask |= (1 << (cpuNumber - 1));
    }

    /** @brief Assign operator with integer.
      * @param cpuMask is the desired cpu mask. */
    void operator=(const uint32 cpuMask) {
        processorMask = cpuMask;
    }

    /** @brief Assign operator with another ProcessorType. 
      * @param pt is the ProcessorType to copy in this. */
    void operator=(const ProcessorType &pt) {
        processorMask = pt.processorMask;
    }

    /** @brief or operator with integer.
      * @param cpuMask is the cpu mask which will be in or with this. */
    void operator|=(const uint32 cpuMask) {
        processorMask |= cpuMask;
    }

    /** @brief or operator with another ProcessorType object.
      * @param pt is the ProcessorType which will be in or with this. */
    void operator|=(const ProcessorType &pt) {
        processorMask |= pt.processorMask;
    }

    /** @brief Equal operator with another ProcessorType.
      * @param pt is the Processor type which will be compared with this. 
      * @return true if masks are equal. */
    bool operator==(const ProcessorType &pt) {
        return processorMask == pt.processorMask;
    }

    /** @brief Equal operator with integer.
      * @param mask is the mask which will be compared with this cpu mask. 
      * @return true if masks are equal. */
    bool operator==(const uint32 mask) {
        return processorMask == mask;
    }

    /** @brief Different operator with another ProcessorType.
      * @param pt is the ProcessorType which will be compared with this. 
      * @return true if masks are different. */
    bool operator!=(const ProcessorType &pt) {
        return processorMask != pt.processorMask;
    }

    /** @brief Different operator with integer.
      * @param mask is the mask which will be compared with this cpu mask.
      * @return true if masks are different. */
    bool operator!=(const uint32 mask) {
        return processorMask != mask;
    }

    /** @brief Get the default cpu mask.
      * @return the default cpu mask. */
    static uint32 GetDefaultCPUs() {
        return ProcessorTypeGetDefaultCPUs();
    }

    /** @brief Set the default cpu mask. */
    static void SetDefaultCPUs(const uint32 mask) {
        ProcessorTypeSetDefaultCPUs(mask);
    }
};

/** Declares that the number of CPUs is undefined or there is no interest
 * in specifying*/
const ProcessorType PTUndefinedCPUs(0);

#endif

