/**
 * @file MemoryDataSourceI.h
 * @brief Header file for class MemoryDataSourceI
 * @date 07/05/2018
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

 * @details This header file contains the declaration of the class MemoryDataSourceI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5GAMS_MEMORYDATASOURCEI_H_
#define L5GAMS_MEMORYDATASOURCEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A DataSourceI which defines a memory area with sufficient space to store all the signals declared by the DataSource instance.
 *
 * @details The memory is allocated in a contiguous memory region: S_1|S_2|...|S_N, where S_N has sufficient space to hold the signal N x number of buffers.
 *
 * A possible configuration structure is:
 * <pre>
 * +ThisDataSourceIObjectName = {
 *    Class = ClassThatImplementsDataSourceI
 *    NumberOfBuffers = 3 //Optional. Default = 1. Each buffer contains a copy of each signal.
 *    HeapName = "Default" //Optional. Default = GlobalObjectsDatabase::Instance()->GetStandardHeap();
 *    Signals = {
 *        +*NAME = {
 *            +Type = BasicType|StructuredType
 *            +NumberOfDimensions = 0|1|2
 *            +NumberOfElements = NUMBER>0
 *       }
 *    }
 * }
 * </pre>
 */
class DLL_API MemoryDataSourceI: public DataSourceI {
public:
    /**
     * @brief Constructor. NOOP.
     */
    MemoryDataSourceI();

    /**
     * @brief Destructor. Frees the memory structure.
     */
    virtual ~MemoryDataSourceI();

    /**
     * @brief Allocates the memory to store all the signals declared in the DataSourceI.
     * @return true if the memory can be successfully allocated.
     */
    virtual bool AllocateMemory();

    /**
     * @brief Gets the number of memory buffers defined in the Initialise method.
     * @return the number of memory buffers defined in the Initialise method.
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief Gets the memory address for the signal at index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @param[in] bufferIdx the index of the buffer (see GetNumberOfMemoryBuffers()).
     * @param[out] signalAddress a pointer to the memory address of this signal for this \a bufferIdx.
     * @return true if the signalIdx and the bufferIdx exist and the memory address can be retrieved for this signal.
     * @pre
     *   Initialise()
     *   SetConfiguredDatabase
     *   signalIdx < GetNumberOfSignals() &&
     *   bufferIdx < 1
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

    /**
     * @brief See DataSourceI::Initialise. Reads the optional NumberOfBuffers and HeapName parameters.
     * @param[in] data The configuration information which may include a Signals node.
     * @return true if the DataSourceI is successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

protected:
    /**
     * The memory address.
     */
    uint8 *memory;

    /**
     * The memory size for each state buffer
     */
    uint32 stateMemorySize;

    /**
     * The total memory size
     */
    uint32 totalMemorySize;

    /**
     * The offset of the signals in memory.
     */
    uint32 *signalOffsets;

    /**
     * The memory heap that is used to allocate the signal memory.
     */
    HeapI *memoryHeap;

    /**
     * The number of buffers
     */
    uint32 numberOfBuffers;

    /**
     * The size in bytes of each signal.
     */
    uint32 *signalSize;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYDATASOURCEI_H_ */

