/**
 * @file ProcessorType.h
 * @brief Header file for class ProcessorType
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
#include "BitSet.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Defines the processors where a particular task should run.
 *
 * @details CPUs are set by a mask of bits (64 bits max => 64 processors max).
 * System dependent calls to get the used cpus could be found in ProcessorType.h
 */
class DLL_API ProcessorType {
public:

    /**
     * @brief Constructor from integer.
     *
     * @details The default is to run the tasks all CPUs but the first.
     * @param[in] cpuMask the CPU mask.
     */
    ProcessorType(const BitSet cpuMask = 0xFEu);

    /**
     * @brief Constructor from another ProcessorType object
     * @param[in] pt the object to be copied into this.
     */
    ProcessorType(const ProcessorType &pt);

    /**
     * @brief Constructor from integer.
     *
     * @details The default is to run the tasks all CPUs but the first.
     * @param[in] cpuMask the CPU mask.
     * @deprecated 
     */
    ProcessorType(const uint32 &cpuMask);

    /**
     * @brief Set the processor mask.
     * @param[in] mask the CPU mask.
     */
    void SetMask(const BitSet mask);

    /**
     * @brief Add a CPU to the mask.
     * @param[in] cpuNumber the number of the CPU to activate.
     */
    void AddCPU(const uint32 cpuNumber);

    /**
     * @brief Check if a CPU is enabled or not.
     * @param[in] cpuNumber the number of the CPU to check.
     * @return the status of the selected CPU.
     */
    const bool CPUEnabled(const uint32 cpuNumber) const;

    /**
     * @brief Get the total maximum number of CPUs available in this configuration (the size of the BitSet)/
     * @return the number of bits of the current bitmask.
     */
    const uint32 GetCPUsNumber() const;


    /**
     * @brief Get the CPU mask as a `uint32` in order to be compatible with the current configuration files.
     * @return the CPUMask BitSet as a trunkated `uint32`
     * @deprecated
     */
    const uint32 GetLegacyUint32Mask() const;

    /**
     * @brief Assign operator with integer.
     * @param[in] cpuMask is the desired CPU mask.
     */
    void operator=(const BitSet cpuMask);

    /**
     * @brief Assign operator with uint32.
     * @param[in] cpuMask is the desired CPU mask.
     * @deprecated
     */
    void operator=(const uint32 cpuMask);

    /**
     *64brief Assign operator with another ProcessorType.
     * @param[in] pt is the ProcessorType to copy in this.
     */
    ProcessorType& operator=(const ProcessorType &pt);
    
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
     * @brief Compares this object with another ProcessorType.
     * @param[in] pt is the ProcessorType which will be compared with this.
     * @return true if the masks are different.
     */
    /*lint -e(1739) , operation int32 != ProcessorType will not be supported*/
    bool operator!=(const ProcessorType &pt) const;

    /**
     * @brief Returns the currently set CPU mask.
     * @return the CPU mask as a BitSize.
     */
    BitSet GetProcessorMask() const;

    /**
     * @brief Rerurns the default CPU mask.
     * @return the default CPU mask.
     */
    static ProcessorType GetDefaultCPUs();

    /**
     * @brief Sets the default CPU mask.
     * @param[in] mask defines the default CPU.
     */
    static void SetDefaultCPUs(const BitSet &mask);

private:
    /**
     * The processor mask
     */
    BitSet processorMask;

    /**
     * The default CPU mask. Initialised to zero.
     */
    static BitSet defaultCPUs;
};

/**
 * Declares that the number of CPUs is undefined
 */
/*lint -e{9141} constant that can be reused by other classes*/
static const ProcessorType UndefinedCPUs(0ull);
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* PROCESSORTYPE_H_ */

