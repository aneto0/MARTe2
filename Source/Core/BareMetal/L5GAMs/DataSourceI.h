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
class RealTimeApplication;
class GAM;
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
                               AnyType &defaultValue);

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

    /* bool GetFunctionSignalsAddress(SignalDirection direction,
     uint32 functionIdx,
     void *&address);
     */
    bool GetFunctionSignalName(SignalDirection direction,
                               uint32 functionIdx,
                               uint32 functionSignalIdx,
                               StreamString &functioSignalName);

    bool GetFunctionSignalAlias(SignalDirection direction,
                                uint32 functionIdx,
                                uint32 functionSignalIdx,
                                StreamString &functionSignalAlias);

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

    bool GetFunctionSignalSamples(SignalDirection direction,
                                  uint32 functionIdx,
                                  uint32 functionSignalIdx,
                                  uint32 &samples);

    bool GetFunctionSignalReadFrequency(SignalDirection direction,
                                        uint32 functionIdx,
                                        uint32 functionSignalIdx,
                                        float32 &frequency);

    bool IsSupportedBroker(SignalDirection direction,
                           uint32 functionIdx,
                           uint32 functionSignalIdx,
                           const char8* brokerClassName);

    virtual bool AllocateMemory() = 0;

    virtual uint32 GetNumberOfMemoryBuffers() = 0;

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
                                       uint32 bufferIdx,
                                       void **&signalAddress) = 0;
    virtual const char8 *Negotiate(StructuredDataI &data,
                                   SignalDirection direction)=0;

    virtual bool PrepareNextState(const RealTimeStateInfo &status) = 0;

    virtual bool AddBrokers(RealTimeApplication &application,
                            SignalDirection direction);

protected:

    virtual bool AddInputBrokerToGAM(ReferenceT<GAM> gam,
                                     const char8* functionName,
                                     void * gamMemPtr)=0;

    virtual bool AddOutputBrokerToGAM(ReferenceT<GAM> gam,
                                      const char8* functionName,
                                      void * gamMemPtr)=0;

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

