/**
 * @file MemoryMapAsyncOutputBroker.h
 * @brief Header file for class MemoryMapAsyncOutputBroker
 * @date 13/02/2017
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

 * @details This header file contains the declaration of the class MemoryMapAsyncOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPASYNCOUTPUTBROKER_H_
#define MEMORYMAPASYNCOUTPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventSem.h"
#include "MemoryMapBroker.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Structure which describes a page of memory.
 * @details Each page of memory contains a copy of all the signals, at a given time instant, assigned to this Broker.
 */
struct MemoryMapAsyncOutputBrokerBufferEntry {
    /**
     * Buffer index
     */
    MARTe::uint32 index;

    /**
     * Data ready to be consumed?
     */
    bool toConsume;

    /**
     * Signal addresses
     */
    void **mem;
};
/**
 * @brief A MemoryMapBroker which asynchronously stores the signals in a DataSourceI memory.
 * @details This BrokerI implementation stores the GAM data in an internal memory buffer. The flushing of this data is performed asynchronously
 * in the context of a different thread (SingleThreadService).
 *
 * Only one GAM is allowed to interact with this MemoryMapAsyncOutputBroker (an IOGAM can be used to collate all the signals).
 *
 * The DataSource shall call the UnlinkDataSource in the DataSourceI::Purge.
 */
class MemoryMapAsyncOutputBroker: public MemoryMapBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
MemoryMapAsyncOutputBroker    ();

    /**
     * @brief Destructor. Frees all the allocated memory and stops the service responsible
     * by flushing the data into the DataSourceI.
     */
    virtual ~MemoryMapAsyncOutputBroker();

    /**
     * @brief Prevents this MemoryMapAsyncOutputBroker from being used as a MemoryMapBroker.
     * @return false.
     */
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);

    /**
     * @brief Initialises the broker.
     * @details Initialises the broker (see MemoryMapOutputBroker::Init) and verifies that all the pre conditions are met.
     * Starts the SingleThreadService which will asynchronously flush the data into the DataSourceI.
     * Note that only one GAM is allowed to interact with this DataSourceI.
     * @param direction (see MemoryMapOutputBroker::Init). Only OutputSignals are supported.
     * @param dataSourceIn (see MemoryMapOutputBroker::Init).
     * @param functionName (see MemoryMapOutputBroker::Init).
     * @param gamMemoryAddress (see MemoryMapOutputBroker::Init).
     * @param numberOfBuffersIn the number of pages that will be created to copy the GAM signals. The higher this number, the larger the buffer to
     * allow the asynchronous flushing into the DataSourceI (particularly useful if this operation is slow).
     * @param cpuMaskIn the CPU mask where the SingleThreadService will execute.
     * @param stackSizeIn the stack size assigned to the SingleThreadService.
     * @return true if MemoryMapOutputBroker::Init() returns true and if all the pre-conditions are met.
     * @pre
     *   numberOfBuffers > 0 &&
     *   dataSourceIn.GetNumberOfFunctions() == 1 &&
     *   dataSourceIn.GetNumberOfMemoryBuffers() &&
     */
    virtual bool InitWithBufferParameters(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
            void * const gamMemoryAddress, const uint32 numberOfBuffersIn, const ProcessorType& cpuMaskIn, const uint32 stackSizeIn);

    /**
     * @brief Sequentially copies all the signals from the GAM memory to the next free buffer memory.
     * @details After copying the data, the SingleThreadService is informed that new data is available so that it can be potentially flushed into
     * the DataSourceI.
     * @return true if all copies are successfully performed and if the next free buffer is not marked for triggering (which means that an overrun as occurred).
     */
    virtual bool Execute();

    /**
     * @brief Gets the CPU mask where the SingleThreadService is being executed.
     * @return the CPU mask where the SingleThreadService is being executed.
     */
    ProcessorType GetCPUMask() const;

    /**
     * @brief Gets the stack size of the SingleThreadService.
     * @return the stack size of the SingleThreadService.
     */
    uint32 GetStackSize() const;

    /**
     * @brief Gets the number of buffers (i.e. pages) where the GAM data is stored.
     * @return the number of buffers (i.e. pages) where the GAM data is stored.
     */
    uint32 GetNumberOfBuffers() const;

    /**
     * @brief Sets if buffer overruns shall be ignored (i.e. the consumer thread is not consuming the data fast enough).
     * @param[in] ignoreBufferOverrunIn if true no error will be triggered if there is a buffer overrun.
     */
    void SetIgnoreBufferOverrun(bool ignoreBufferOverrunIn);

    /**
     * @brief Gets if buffer overruns is being ignored (i.e. the consumer thread is not consuming the data fast enough).
     * @return if true no error is to be triggered when there is a buffer overrun.
     */
    bool IsIgnoringBufferOverrun() const;

    /**
     * @brief Flush all the data left in the shared buffer between the threads.
     * @return true if the data is properly flushed.
     */
    bool Flush();

    /**
     * @brief Breaks the link with the owner DataSource (set with InitWithBufferParameters)
     */
    void UnlinkDataSource();

private:

    /**
     * @brief SingleThreadService callback function responsible for flushing the Buffer into the DataSourceI.
     * @param[in] info (see EmbeddedServiceMethodBinderI). Only info.GetStage() == ExecutionInfo::MainStage is supported in this implementation.
     * @return ErrorManagement::NoError if the synchronisation semaphore with the Execute method returns no errors. At the end of the application, i.e.
     *  after the destructor has been called, it returns ErrorManagement::Completed.
     */
    ErrorManagement::ErrorType BufferLoop(ExecutionInfo & info);

    /**
     * The SingleThreadService responsible for flushing the Buffer into the DataSourceI.
     */
    SingleThreadService service;

    /**
     * The SingleThreadService CPU mask.
     */
    ProcessorType cpuMask;

    /**
     * The SingleThreadService stack size.
     */
    uint32 stackSize;

    /**
     * The multi-page memory buffer where the GAM signals are stored.
     */
    MemoryMapAsyncOutputBrokerBufferEntry *bufferMemoryMap;

    /**
     * The DataSource associated to this broker
     */
    ReferenceT<DataSourceI> dataSourceRef;

    /**
     * The number of buffers/pages.
     */
    uint32 numberOfBuffers;

    /**
     * The current index where the Execute method is writing to.
     */
    uint32 writeIdx;

    /**
     * The read index where the BufferLoop is reading from.
     */
    uint32 readSynchIdx;

    /**
     * Semaphore that synchronises the Execute and the BufferLoop methods.
     */
    EventSem sem;

    /**
     * Makes sure that the EventSem is not Reset while being posted and also protects the writeIdx variable.
     */
    FastPollingMutexSem fastSem;

    /**
     * Makes sure that the EventSem is not Reset while being posted and also protects the writeIdx variable.
     */
    bool posted;

    /**
     * Allows a clean exit of the BufferLoop thread
     */
    bool destroying;

    /**
     * The binder for the SingleThreadService.
     */
    EmbeddedServiceMethodBinderT<MemoryMapAsyncOutputBroker> binder;

    /**
     * If true buffer overruns will be ignored.
     */
    bool ignoreBufferOverrun;

    /**
     * True if flushed was called.
     */
    bool flushed;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPASYNCOUTPUTBROKER_H_ */

