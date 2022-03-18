/**
 * @file MemoryMapMultiBufferBroker.h
 * @brief Header file for class MemoryMapMultiBufferBroker
 * @date 03/05/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class MemoryMapMultiBufferBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPMULTIBUFFERBROKER_H_
#define MEMORYMAPMULTIBUFFERBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "FastPollingMutexSem.h"
#include "MemoryMapBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Multi-buffer memory mapped BrokerI implementation.
 * @details This class knows how to copy from/to a DataSourceI multi-buffer memory address to/from a GAM signal memory address.
 *
 * This broker will query, for each signal, the current offset (bytes) of the signal memory in the DataSourceI (as returned by the GetSignalMemoryBuffer method).
 * Note that this offset is allowed to be different for each signal. This broker is expected to be used in DataSourceI implementations that stored data
 *  in buffers of a given length (e.g. dual circular), so that the DataSourceI may be writing in a given memory offset while this broker is reading from another offset.
 *
 * If the number of samples provided by the DataSourceI is > than the number of memory buffers a circular buffer implementation will be assumed.
 *
 * The reason why the offset needs to be computed for every signal is that there might be DataSourceI implementations where a given signal is ready
 *  before others and thus can be copied in advanced.
 */
class MemoryMapMultiBufferBroker: public MemoryMapBroker {
public:

    /**
     * @brief Constructor. NOOP.
     */
    MemoryMapMultiBufferBroker();

    /**
     * @brief Destructor. Frees any memory that might have been allocated in the Init method.
     */
    virtual ~MemoryMapMultiBufferBroker();

    /**
     * @brief Initialises the MemoryMapMultiBufferBroker (see MemoryMapBroker::Init for details about the Brokers) but creating a copyTable for every buffer of the DataSource::GetNumberOfMemoryBuffers.
     * @details Allocates the copy tables for all the signals to be copied taking into account the possible signal samples and ranges.
     * @post
     *   GetNumberOfCopies() > 0
     */
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *const gamMemoryAddress);

    /**
     * @brief Calls Init(), i.e. optim is ignored.
     */
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *const gamMemoryAddress,
                      const bool optim);

protected:

    /**
     * @brief Copies from the DataSourceI to the GAM memory.
     * @return true if DataSourceI::TerminateInputCopy is successful for all signal copies.
     */
    bool CopyInputs();

    /**
     * @brief Copies from the GAM memory to the DataSourceI memory.
     * @return true if DataSourceI::TerminateOutputCopy is successful for all signal copies.
     */
    bool CopyOutputs();

    /**
     * The offset indexes of all the signals to be copied (note that these might be repeated, if the same signal is to be copied with different ranges).
     */
    uint32 *signalIdxArr;

    /**
     * The number of samples to be copied for each signal copy.
     */
    uint32 *samples;

    /**
     * The offset in bytes to be copied for each copy. Needed to trap out-of-bounds exceptions in circular buffer implementations.
     */
    int32 *maxOffset;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPMULTIBUFFERBROKER_H_ */

