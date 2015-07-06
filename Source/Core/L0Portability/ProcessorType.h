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
#define PROCESSORTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"


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
    /**
     * Declares that the number of CPUs is undefined
     */
    static const ProcessorType UndefinedCPUs;

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
    void SetMask(const uint32 mask);

    /**
     * @brief Add a cpu to the mask.
     * @param[in] cpuNumber is the number of the cpu to activate.
     */
    void AddCPU(const uint32 cpuNumber);

    /**
     * @brief Assign operator with integer.
     * @param[in] cpuMask is the desired cpu mask.
     */
    void operator=(const uint32 cpuMask);

    /**
     * @brief Assign operator with another ProcessorType.
     * @param[in] pt is the ProcessorType to copy in this.
     */
    ProcessorType& operator=(const ProcessorType &pt);

    /**
     * @brief or operator with integer.
     * @param[in] cpuMask is the cpu mask which will be in or with this.
     */
    void operator|=(const uint32 cpuMask);

    /**
     * @brief or operator with another ProcessorType object.
     * @param[in] pt is the ProcessorType which will be in or with this.
     */
    void operator|=(const ProcessorType &pt);

    /**
     * @brief Compares this object with another ProcessorType.
     * @param[in] pt is the Processor type which will be compared with this.
     * @return true if masks are equal.
     */
    /*lint -e(1739) , operation int32 == ProcessorType will not be supported*/
    bool operator==(const ProcessorType &pt) const;

    /**
     * @brief Compares this cpu mask with an input mask.
     * @param[in] mask is the mask which will be compared with this cpu mask.
     * @return true if masks are equal.
     */
    bool operator==(const uint32 mask) const;

    /**
     * @brief Compares this object with another ProcessorType.
     * @param[in] pt is the ProcessorType which will be compared with this.
     * @return true if masks are different.
     */
    /*lint -e(1739) , operation int32 != ProcessorType will not be supported*/
    bool operator!=(const ProcessorType &pt) const;

    /**
     * @brief Compares this cpu mask with an input mask.
     * @param[in] mask is the mask which will be compared with this cpu mask.
     * @return true if masks are different.
     */
    bool operator!=(const uint32 mask) const;

    /**
     * @brief Returns the currently set cpu mask.
     * @return the cpu mask as an uint32.
     */
    uint32 GetProcessorMask() const;

    /**
     * @brief Get the default cpu mask.
     * @return the default cpu mask.
     */
    static uint32 GetDefaultCPUs();

    /**
     * @brief Sets the default cpu mask.
     * @param[in] mask defines the default cpu.
     */
    static void SetDefaultCPUs(const uint32 &mask);

private:
    /**
     * The processor mask
     */
    uint32 processorMask;

    /**
     * The default CPU mask. Initialised to zero.
     */
    static uint32 defaultCPUs;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORTYPE_H_ */

