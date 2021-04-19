/**
 * @file MemoryMapAsyncTriggerOutputBroker.h
 * @brief Header file for class MemoryMapAsyncTriggerOutputBroker
 * @date 24/01/2017
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

 * @details This header file contains the declaration of the class MemoryMapAsyncTriggerOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPASYNCTRIGGEROUTPUTBROKER_H_
#define MEMORYMAPASYNCTRIGGEROUTPUTBROKER_H_

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
struct MemoryMapAsyncTriggerOutputBrokerBufferEntry {
    /**
     * Buffer index
     */
    MARTe::uint32 index;

    /**
     * Triggered to be stored?
     */
    bool triggered;

    /**
     * Signal addresses
     */
    void **mem;
};
/**
 * @brief A MemoryMapBroker which stores the signals in a DataSourceI memory only after a Trigger signal being asserted.
 * @details This BrokerI implementation stores the GAM data in an internal memory buffer.
 * Every time a triggering signal is set to 1, this data is asynchronously flushed into the DataSourceI memory (retrieved with GetSignalMemoryBuffer)
 * and the Synchronise method is called on the DataSourceI. The flushing of this data is performed in the context of a different thread (SingleThreadService).
 *
 * If the number of pre-trigger buffers is greater than zero, every time a trigger is detected, the pre-trigger number of pages before that were acquired before the
 * trigger will also be flushed into the DataSourceI. If the number of post-trigger buffers is greater than zero, the post-trigger number of pages
 * after the trigger will also be flushed into the DataSourceI.
 *
 * The Triggering signal shall be the first signal of the DataSourceI (i.e. the signal with the name GetSignalName(0, name)) and shall have
 * type uint8. All the signals shall have one and only one sample. The DataSourceI shall return GetNumberOfMemoryBuffers() == 1.
 *
 * Only one GAM is allowed to interact with this MemoryMapAsyncTriggerOutputBroker (an IOGAM can be used to collate all the signals).
 * 
 * The DataSource shall call the UnlinkDataSource in the DataSourceI::Purge.
 */
class MemoryMapAsyncTriggerOutputBroker: public MemoryMapBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
MemoryMapAsyncTriggerOutputBroker    ();

    /**
     * @brief Destructor. Frees all the allocated memory and stops the service responsible
     * by flushing the data into the DataSourceI.
     */
    virtual ~MemoryMapAsyncTriggerOutputBroker();

    /**
     * @brief Prevents this MemoryMapAsyncTriggerOutputBroker from being used as a MemoryMapBroker.
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
     * @param preTriggerBuffersIn number of pre-trigger pages to flush into the DataSourceI.
     * @param postTriggerBuffersIn number of post-trigger pages to flush into the DataSourceI.
     * @param cpuMaskIn the CPU mask where the SingleThreadService will execute.
     * @param stackSizeIn the stack size assigned to the SingleThreadService.
     *
     * @return true if MemoryMapOutputBroker::Init() returns true and if all the pre-conditions are met.
     * @pre
     *   numberOfBuffers > 0 &&
     *   preTriggerBuffers < numberOfBuffers &&
     *   postTriggerBuffers < numberOfBuffers &&
     *   (preTriggerBuffers + postTriggerBuffers) < numberOfBuffers &&
     *   dataSourceIn.GetNumberOfFunctions() == 1 &&
     *   dataSourceIn.GetNumberOfMemoryBuffers() &&
     *   dataSourceIn.GetSignalType(0u) == UnsignedInteger8Bit //The triggering signal shall be the first DataSourceI signal and shall be of type uint8
     */
    virtual bool InitWithTriggerParameters(const SignalDirection direction, DataSourceI &dataSourceIn, const char8 * const functionName,
            void * const gamMemoryAddress, const uint32 numberOfBuffersIn, const uint32 preTriggerBuffersIn,
            const uint32 postTriggerBuffersIn, const ProcessorType& cpuMaskIn, const uint32 stackSizeIn);

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
     * @brief Gets the number of pre-trigger buffers (i.e. pages) that are to be stored after each trigger.
     * @return the number of pre-trigger buffers (i.e. pages) that are to be stored after each trigger.
     */
    uint32 GetPreTriggerBuffers() const;

    /**
     * @brief Gets the number of post-trigger buffers (i.e. pages) that are to be stored after each trigger.
     * @return the number of post-trigger buffers (i.e. pages) that are to be stored after each trigger.
     */
    uint32 GetPostTriggerBuffers() const;

    /**
     * @brief Resets the pre-trigger buffers so that a new acquisition does not store data from a previous acquisition.
     */
    void ResetPreTriggerBuffers();

    /**
     * @brief Flushes all the triggers asynchronously.
     * @details All the buffers which are marked with a trigger are copied to the DataSourceI and the Synchronise method called.
     * Note that this happens asynchronously with respect to the buffer consumer thread, so the higher level application using this
     *  should make sure that no data is written to this thread while this method is being called.
     * @return true if all the Synchronise calls return true.
     */
    bool FlushAllTriggers();

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
    MemoryMapAsyncTriggerOutputBrokerBufferEntry *bufferMemoryMap;

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
     * Number of pre-trigger buffers.
     */
    uint32 preTriggerBuffers;

    /**
     * Number of post-trigger buffers.
     */
    uint32 postTriggerBuffers;

    /**
     * This counter decrements every-time a post buffer is written. It will reset to postTriggerBuffers when a trigger occurs so that the post-trigger buffers
     * always count from the last post trigger
     */
    uint32 postTriggerBuffersCounter;

    /**
     * If two consecutive triggers are set, only set the pre-trigger if it was not already triggered.
     */
    bool wasTriggered;

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
     * True if the BufferLoop is executing (needed for the FlushAllTrigger)
     */
    bool bufferLoopExecuting;

    /**
     * The binder for the SingleThreadService.
     */
    EmbeddedServiceMethodBinderT<MemoryMapAsyncTriggerOutputBroker> binder;

    /**
     * Do not store pre-trigger of empty buffers that were never acquired
     */
    int32 numberOfPreBuffersWritten;

    /**
     * The index of the trigger on the GAM signal (it is for sure zero in the DataSource, but it is not necessarily zero in the GAM memory).
     */
    uint32 triggerIndexInGAMMemory;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPASYNCTRIGGEROUTPUTBROKER_H_ */

