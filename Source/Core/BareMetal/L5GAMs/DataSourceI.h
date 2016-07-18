/**
 * @file DataSourceI.h
 * @brief Header file for class DataSourceI
 * @date 18/04/2016
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

 * @details This header file contains the declaration of the class DataSourceI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSignalI.h"
#include "MemoryArea.h"
#include "RealTimeStateInfo.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "StaticList.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class BrokerI;

enum SignalDirection {
    InputSignals, OutputSignals, None
};

/**
 * @brief Memory container for the exchange of signal data.
 * @details This class provides the memory area where all signals interchanged between GAMs exist.
 * It can also be sub-classed to provide a uniform mechanism of sharing the same memory area between
 *  DataSourceSignalI that are inserted as a child of this container.
 *
 * @details The syntax in the configuration stream shall be:
 *
 * +DataSource_name = {\n
 *     Class = DataSourceI\n
 *     HeapName = "the heap name" (default NULL, i.e StandardHeap will be used)
 *     ...\n
 * }\n
 *
 * and it can only exist as a child of the [RealTimeApplication].+Data declaration.
 */
class DLL_API DataSourceI: public ReferenceContainer {

public:
    /**
     * @brief Initialises the ReferenceContainer.
     */
    DataSourceI();

    virtual ~DataSourceI();

    /**
     * @brief Configures the DataSourceI against the input configuration \a data.
     * @details The following fields can be specified in \a data:
     *
     *   HeapName = "the name of the heap memory"
     *
     * By default (if not specified) the StandardHeap will be used to manage the heap memory.
     * @return true if the ReferenceContainer::Initialise succeeds
     * @post
     *    if HeapName exits:
     *      memory.SetHeapName(heapName.Buffer())
     */
    virtual bool Initialise(StructuredDataI & data);

    bool AddSignals(StructuredDataI & data);

    bool SetConfiguredDatabase(StructuredDataI & data);

    uint32 GetNumberOfSignals();

    bool GetSignalName(uint32 signalIdx,
                       StreamString &signalName);

    bool GetSignalIndex(uint32 &signalIdx,
                        const char8* const signalName);

    TypeDescriptor GetSignalType(uint32 signalIdx);

    bool GetSignalNumberOfDimensions(uint32 signalIdx,
                                     uint32 &numberOfDimensions);

    bool GetSignalNumberElements(uint32 signalIdx,
                                 uint32 &numberOfElements);

    bool GetSignalByteSize(uint32 signalIdx,
                           uint32 &byteSize);

    bool GetSignalNumberOfStates(uint32 signalIdx,
                                 uint32 &numberOfStates);

    bool GetSignalStateName(uint32 signalIdx,
                            uint32 stateIdx,
                            StreamString &stateName);

    bool GetSignalNumberOfConsumers(uint32 signalIdx,
                                    const char8 *stateName,
                                    uint32 &numberOfConsumers);

    bool GetSignalNumberOfProducers(uint32 signalIdx,
                                    const char8 *stateName,
                                    uint32 &numberOfProducers);

    bool GetSignalConsumerName(uint32 signalIdx,
                               const char8 *stateName,
                               uint32 consumerIdx,
                               StreamString &consumerName);

    bool GetSignalProducerName(uint32 signalIdx,
                               const char8 *stateName,
                               uint32 producerIdx,
                               StreamString &producerName);

    bool GetSignalDefaultValue(uint32 signalIdx,
                               StreamString &defaultValue);

    uint32 GetNumberOfFunctions();

    bool GetFunctionName(uint32 functionIdx,
                         StreamString &functionName);

    bool GetFunctionIndex(uint32 &functionIdx,
                          const char8* const functionName);

    bool GetFunctionNumberOfSignals(SignalDirection direction,
                                    uint32 functionIdx,
                                    uint32 &numberOfSignals);

    bool GetFunctionSignalsByteSize(SignalDirection direction,
                                    uint32 functionIdx,
                                    uint32 &byteSize);

    bool GetFunctionSignalsAddress(SignalDirection direction,
                                   uint32 functionIdx,
                                   void *&address);

    bool GetFunctionSignalName(SignalDirection direction,
                               uint32 functionIdx,
                               uint32 functionSignalIdx,
                               StreamString &functioSignalName);

    bool GetFunctionSignalIndex(SignalDirection direction,
                                uint32 functionIdx,
                                uint32 &functionSignalIdx,
                                const char8* const functionSignalName);

    bool GetFunctionSignalNumberOfByteOffsets(SignalDirection direction,
                                              uint32 functionIdx,
                                              uint32 functionSignalIdx,
                                              uint32 &numberOfByteOffsets);

    bool GetFunctionSignalByteOffsetInfo(SignalDirection direction,
                                         uint32 functionIdx,
                                         uint32 functionSignalIdx,
                                         uint32 byteOffsetIndex,
                                         uint32 &byteOffsetStart,
                                         uint32 &byteOffsetSize);

    bool GetFunctionSignalTimeCyclesInfo(SignalDirection direction,
                                         uint32 functionIdx,
                                         uint32 functionSignalIdx,
                                         uint32 &timeCycles,
                                         uint32 &timeSamples);

    virtual bool AllocateMemory() = 0;

    virtual uint32 GetCurrentBufferIndex() = 0;

    virtual uint32 GetNumberOfMemoryBuffers() = 0;

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
                                       uint32 bufferIdx,
                                       void *&signalAddress) = 0;

    virtual ReferenceT<BrokerI> GetInputReader(const char8 * const functionName) = 0;

    virtual ReferenceT<BrokerI> GetOutputWriter(const char8 * const functionName) = 0;

    virtual bool PrepareNextState(const RealTimeStateInfo &status) = 0;

    virtual bool ChangeState() = 0;

#if 0
    /**
     * @brief Recursively calls DataSourceSignalI::Allocate
     * @details During the initialisation of a RealTimeApplication this method is called and allows
     * to share the memory of this DataSourceI with all of its children (provided that they inherit from DataSourceSignalI).
     * @return true if DataSourceSignalI::Allocate returns true for all the DataSourceSignalI in this container.
     * @pre
     *    All the children of this container inherit either from DataSourceSignalI or from ReferenceContainer.
     */
    virtual bool Allocate();

    /**
     * @brief Prepares the environment for the next state.
     * @param[in] status contains informations about the current and the next state and about the current
     * active buffer.
     * @return true if the state change request is admissible by this DataSourceSignalI.
     */
    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    /**
     * @brief Allocates the memory for this signal.
     * @param[in] dsMemory is the memory where this signal has to be allocated.
     * @return true if the memory for this signal can be successfully allocated in \a dsMemory.
     */
    //virtual bool Allocate(MemoryArea &dsMemory)=0;
    /**
     * @brief The synchronising routine.
     * @details This function will block the execution thread until an event is generated.
     * In particular, if a GAMSignalI linked to this signal specifies a number of cycles
     * different from 0, the read (write) operation will be synchronised and this function
     * will be called from the broker (DataSourceBrokerI) before the read (write) operation.
     * @return false in case of errors or if the timeout expire.
     */
    virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait)=0;

    /**
     * @brief Retrieves the DataSourceBrokerI reader for the signal passed in input.
     * @details Generally a DataSourceSignalI will support a certain number of interfaces to interact with GAMs.
     * This function retrieves the first input reader compatible with this DataSourceSignalI and with the
     * GAMSignalI in input, or an invalid reference if the two are incompatible. The GAM signal \a defIn in input
     * will be added to eventually generated input reader.
     * @param[in] signalIn is a GAMSignalI to be linked to this signal.
     * @param[in] varPtr is the pointer of the GAMSignalI memory (if NULL the broker will allocate the memory).
     * @return a reference to a reader compatible with \a signalIn, an invalid reference in case of incompatibility.
     */
    virtual ReferenceT<BrokerI> GetInputReader(ReferenceT<GAMSignalI> signalIn,
            void * varPtr = NULL_PTR(void*))=0;

    /**
     * @brief Retrieves the DataSourceBrokerI writer for the signal passed in input.
     * @details Generally a data source can supports a certain number of interfaces to interact with GAMs.
     * This function retrieves the first output writer compatible with this DataSourceSignalI and with the
     * GAMSignalI in output, or an invalid reference if the two are incompatible. The GAM signal \a defIn in input
     * will be added to eventually generated output writer.
     * @param[in] signalOut is a GAMSignalI to be linked to this signal.
     * @param[in] varPtr is the pointer of the GAMSignalI memory (if NULL the broker will allocate the memory).
     * @return a reference to a writer compatible with \a signalOut, an invalid reference in case of incompatibility.
     */
    virtual ReferenceT<BrokerI> GetOutputWriter(ReferenceT<GAMSignalI> signalOut,
            void * varPtr = NULL_PTR(void*))=0;

    /**
     * @brief Configures the signal from a GAMSignalI definition.
     * @details If the signal is not fully configured (i.e. if not all the configurable fields were already specified),
     * this function completes the definition reading the fields of a GAMSignalI linked to this signal passed in input.
     * @param[in] gamSignalIn is a GAMSignalI to be linked to this signal.
     * @return false if the current configuration is not compatible with the \a gamSignalIn one.
     */
    virtual bool Configure(ReferenceT<GAMSignalI> gamSignalIn)=0;

    /**
     * @brief Checks if the broker in input is compatible with this signal.
     * @param[in] testBroker is the broker to be checked.
     * @return true if \a testBroker is supported by this signal, false otherwise.
     */
    virtual bool IsSupportedBroker(BrokerI &testBroker)=0;

private:

    /**
     * The heap name
     */
    StreamString heapName;

    /**
     * The memory of the data source.
     */
    MemoryArea memory;
#endif
    ConfigurationDatabase signalsDatabase;

    ConfigurationDatabase configuredDatabase;

    uint32 numberOfSignals;

    bool MoveToSignalIndex(uint32 signalIdx);

    bool MoveToFunctionIndex(uint32 functionIdx);

    bool MoveToFunctionSignalIndex(SignalDirection direction,
                                   uint32 functionIdx,
                                   uint32 functionSignalIdx);
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCE_H_ */

