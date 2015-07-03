/**
 * @file ProcessorType.h
 * @brief Header file for class ProcessorType
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class ProcessorType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORTYPE_H_
#define 		PROCESSORTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"

extern "C" {
/**
 * @brief Get the default cpu.
 * @return a mask which defines the default cpu used.
 */
uint32 ProcessorTypeGetDefaultCPUs(void);

/**
 * @brief Set the default cpu to use.
 * @param[in] defaultMask defines the default cpu to use.
 */
void ProcessorTypeSetDefaultCPUs(uint32 defaultMask);
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Defines the processors where a particular task should run.
 *
 * @details Cpus are setted by a mask of bits (32 bits max => 32 processors max).
 * System dependent calls to get the used cpus could be found in ProcessorTypeOS.h
 */
class ProcessorType {
public:
    /** The default CPU mask. Initialised to zero.*/
    static uint32 defaultCPUs;

    friend uint32 ProcessorTypeGetDefaultCPUs();
    friend void ProcessorTypeSetDefaultCPUs(uint32 defaultMask);

    /**
     * @brief Constructor from integer.
     *
     * @details The default is to run the tasks all CPUs but the first.
     * @param[in] cpuMask is the cpu mask.
     */
    ProcessorType(const uint32 cpuMask = 0xFEu);

    /**
     * @brief Constructor from another ProcessorType object
     * @param[in] pt is the object to be copied in this.
     */
    ProcessorType(const ProcessorType &pt);

    /**
     * @brief Set the processor mask.
     * @param[in] mask is the cpu mask.
     */
    inline void SetMask(const uint32 mask);

    /**
     * @brief Add a cpu to the mask.
     * @param[in] cpuNumber is the number of the cpu to activate.
     */
    inline void AddCPU(const uint32 cpuNumber);

    /**
     * @brief Assign operator with integer.
     * @param[in] cpuMask is the desired cpu mask.
     */
    inline void operator=(const uint32 cpuMask);

    /**
     * @brief Assign operator with another ProcessorType.
     * @param[in] pt is the ProcessorType to copy in this.
     */
    inline ProcessorType& operator=(const ProcessorType &pt);

    /**
     * @brief or operator with integer.
     * @param[in] cpuMask is the cpu mask which will be in or with this.
     */
    inline void operator|=(const uint32 cpuMask);

    /**
     * @brief or operator with another ProcessorType object.
     * @param[in] pt is the ProcessorType which will be in or with this.
     */
    inline void operator|=(const ProcessorType &pt);

    /**
     * @brief Compares this object with another ProcessorType.
     * @param[in] pt is the Processor type which will be compared with this.
     * @return true if masks are equal.
     */
    /*lint -e(1739) , operation int32 == ProcessorType will not be supported*/
    inline bool operator==(const ProcessorType &pt) const;

    /**
     * @brief Compares this cpu mask with an input mask.
     * @param[in] mask is the mask which will be compared with this cpu mask.
     * @return true if masks are equal.
     */
    inline bool operator==(const uint32 mask) const;

    /**
     * @brief Compares this object with another ProcessorType.
     * @param[in] pt is the ProcessorType which will be compared with this.
     * @return true if masks are different.
     */
    /*lint -e(1739) , operation int32 != ProcessorType will not be supported*/
    inline bool operator!=(const ProcessorType &pt) const;

    /**
     * @brief Compares this cpu mask with an input mask.
     * @param[in] mask is the mask which will be compared with this cpu mask.
     * @return true if masks are different.
     */
    inline bool operator!=(const uint32 mask) const;

    /**
     * @brief Returns the currently set cpu mask.
     * @return the cpu mask as an uint32.
     */
    inline uint32 GetProcessorMask() const;

    /**
     * @brief Get the default cpu mask.
     * @return the default cpu mask.
     */
    static inline uint32 GetDefaultCPUs();

    /**
     * @brief Set the default cpu mask.
     * @param[in] mask defines the default cpu.
     */
    static inline void SetDefaultCPUs(const uint32 mask);

private:
    /** The processor mask */
    uint32 processorMask;
};

/**
 * Declares that the number of CPUs is undefined or there is no interest
 * in specifying.
 */
const ProcessorType PTUndefinedCPUs(0u);
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void ProcessorType::SetMask(const uint32 mask) {
    processorMask = mask;
}

void ProcessorType::AddCPU(const uint32 cpuNumber) {
    uint32 cpuMask = 1u;
    cpuMask = cpuMask << (cpuNumber - 1u);
    processorMask |= cpuMask;
}

void ProcessorType::operator=(const uint32 cpuMask) {
    processorMask = cpuMask;
}

ProcessorType& ProcessorType::operator=(const ProcessorType &pt) {
    if (this != &pt) {
        processorMask = pt.processorMask;
    }
    return *this;
}

void ProcessorType::operator|=(const uint32 cpuMask) {
    processorMask |= cpuMask;
}

void ProcessorType::operator|=(const ProcessorType &pt) {
    processorMask |= pt.processorMask;
}

bool ProcessorType::operator==(const ProcessorType &pt) const {
    return processorMask == pt.processorMask;
}

bool ProcessorType::operator==(const uint32 mask) const {
    return processorMask == mask;
}

bool ProcessorType::operator!=(const ProcessorType &pt) const {
    return processorMask != pt.processorMask;
}

bool ProcessorType::operator!=(const uint32 mask) const {
    return processorMask != mask;
}

uint32 ProcessorType::GetDefaultCPUs() {
    return ProcessorTypeGetDefaultCPUs();
}

void ProcessorType::SetDefaultCPUs(const uint32 mask) {
    ProcessorTypeSetDefaultCPUs(mask);
}

uint32 ProcessorType::GetProcessorMask() const {
    return processorMask;
}

#endif /* PROCESSORTYPE_H_ */

