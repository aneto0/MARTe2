/**
 * @file MemoryMapTriggerOutputBroker.h
 * @brief Header file for class MemoryMapTriggerOutputBroker
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

 * @details This header file contains the declaration of the class MemoryMapTriggerOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPTRIGGEROUTPUTBROKER_H_
#define MEMORYMAPTRIGGEROUTPUTBROKER_H_

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
 * TODO
 */
struct MemoryMapTriggerOutputBrokerBufferEntry {
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
 * @brief TODO
 */
class MemoryMapTriggerOutputBroker: public MemoryMapBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
MemoryMapTriggerOutputBroker    ();

    /**
     * TODO
     */
    virtual ~MemoryMapTriggerOutputBroker();

    /**
     * TODO
     * @param direction
     * @param dataSourceIn
     * @param functionName
     * @param gamMemoryAddress
     * @return
     */
    virtual bool Init(const SignalDirection direction,
            DataSourceI &dataSourceIn,
            const char8 * const functionName,
            void * const gamMemoryAddress);

    /**
     * @brief Sequentially copies all the signals from the GAM memory to the buffer memory.
     * @return true if all copies are successfully performed.
     */
    virtual bool Execute();

    /**
     * TODO
     * @param[in] cpuMaskIn
     */
    void SetCPUMask(const ProcessorType& cpuMaskIn);

    /**
     * TODO
     * @param[in] preTriggerBuffersIn
     */
    void SetPreTriggerBuffers(const uint32 preTriggerBuffersIn);

    /**
     * TODO
     * @param[in] postTriggerBuffersIn
     */
    void SetPostTriggerBuffers(const uint32 postTriggerBuffersIn);

private:
    /**
     * TODO
     */
    ErrorManagement::ErrorType BufferLoop(const ExecutionInfo & info);

    /**
     * TODO
     */
    SingleThreadService *service;

    /**
     * TODO
     */
    ProcessorType cpuMask;

    /**
     * TODO
     */
    MemoryMapTriggerOutputBrokerBufferEntry *buffer;

    /**
     * The DataSource associated to this broker
     */
    DataSourceI *dataSource;

    /**
     * TODO
     */
    uint32 numberOfBuffers;
    /**
     * TODO
     */
    uint32 writeIdx;
    /**
     * TODO
     */
    uint32 readSynchIdx;
    /**
     * TODO
     */
    uint32 preTriggerBuffers;
    /**
     * TODO
     */
    uint32 postTriggerBuffers;
    /**
     * TODO
     */
    uint32 postTriggerBuffersCounter;
    /**
     * TODO
     */
    bool wasTriggered;
    /**
     * TODO
     */
    EventSem sem;

    /**
     * TODO
     */
    EmbeddedServiceMethodBinderI *binder;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPTRIGGEROUTPUTBROKER_H_ */

