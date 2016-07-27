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
#include "ConfigurationDatabase.h"
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
 * @brief Interface for the components that interact with hardware.
 * @details The main role of components that implement this interface is to
 * offer a communication channel between signals of GAM components and signals
 * that typically exist in hardware components.
 * The minimum configuration structure is (more information about the syntax in RealTimeApplicationConfigurationBuilder):
 * +ThisDataSourceIObjectName = {"
 *    Class = ClassThatImplementsDataSourceI"
 *    Signals = {
 *        +*NAME = {
 *            +Type = BasicType|StructuredType
 *            +NumberOfDimensions = 0|1|2
 *            +NumberOfElements = NUMBER>0
 *       }
 *    }
 * }
 */
class DLL_API DataSourceI: public ReferenceContainer {

public:
    /**
     * @brief Initialises the ReferenceContainer.
     * @post
     *   GetNumberOfSignals() == 0
     */
    DataSourceI();

    /**
     * @brief NOOP
     */
    virtual ~DataSourceI();

    /**
     * @brief Initialises the signals database.
     * @details Initialises the ReferenceContainer with input \a data StructuredDataI.
     * Moves to the node "Signals" in the input \a data StructuredDataI and
     * locally stores the information of all the available signals.
     * @return true if the ReferenceContainer is successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Writes all the available signals information into \a data.
     * @details During Initialise the Signals configuration node is locally stored.
     * Calling this function will store this information into \a data.
     * @return true if the signals can be successfully written into \a data.
     */
    bool AddSignals(StructuredDataI & data);

    /**
     * @brief Assigns a valid and StructuredDataI to this DataSourceI.
     * @details This method is called by the RealTimeApplicationConfigurationBuilder once all
     *  the information has been validated and negotiated. The structure of this information is (the syntax is described in RealTimeApplicationConfigurationBuilder):
     *  QualifiedName = "x.y.DATASOURCENAME"
     *    Signals = {
     *      *NUMBER={
     *        QualifiedName = "QualifiedName of the signal"
     *        Type = BasicType
     *        NumberOfDimensions = 0|1|2
     *        NumberOfElements = NUMBER>0
     *        +Frequency = -1|NUMBER>0
     *        +States = {
     *          *StateN = {
     *            GAMConsumers = { "0" ... "N" }
     *            SignalConsumers = { "0" ... "N" }
     *            GAMProducers = { "0" ... "N" }
     *            SignalProducers = { "0" ... "N" }
     *          }
     *        }
     *        Functions+ = {
     *          *NUMBER = {
     *            QualifiedName = "QualifiedName of the Function"
     *            ByteSize = NUMBER>0
     *            GAMMemoryOffset = NUMBER>0
     *            Signals = {
     *              *NUMBER = {
     *                QualifiedName = "QualifiedName of the Signal"
     *                +ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ... }
     *                Frequency = -1|NUMBER>0
     *                Samples = -1|NUMBER>0
     *                Broker = "Name of the Broker returned by the DataSource"
     *              }
     *            }
     *          }
     *        }
     *      }
     *    }
     */
    bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Gets the number of signals handled by this DataSource.
     * @details Notice that after SetConfiguredDatabase is called the
     * number of signals might be different from the number of signals
     * set in the Initialise phase.
     * @return the number of signals handled by this DataSource.
     * @pre
     *   SetConfiguredDatabase
     */
    uint32 GetNumberOfSignals();

    /**
     * @brief Gets the index of the signal with name \a signalIdx.
     * @param[out] signalIdx the index of the signal.
     * @param[out] signalName the name of the signal.
     * @return true if the signalName can be found.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalIndex(uint32 &signalIdx,
                        const char8* const signalName);

    /**
     * @brief Gets the name of the signal at position \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @param[out] signalName where to write the signal name.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalName(uint32 signalIdx,
                       StreamString &signalName);

    /**
     * @brief Gets the type of the signal at position \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @return the signal type or InvalidType if the signal is not found.
     * @pre
     *   SetConfiguredDatabase
     */
    TypeDescriptor GetSignalType(uint32 signalIdx);

    /**
     * @brief Gets the number of dimensions of the signal at position \a signalIdx.
     * @details The default number of dimensions of a signal is 0.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfDimensions the number of dimensions.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfDimensions(uint32 signalIdx,
                                     uint32 &numberOfDimensions);

    /**
     * @brief Gets the number of elements of the signal at position \a signalIdx.
     * @details The default number of elements of a signal is 1.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfDimensions the number of elements.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfElements(uint32 signalIdx,
                                   uint32 &numberOfElements);

    /**
     * @brief Gets the number of bytes required to represent the signal at position \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @param[out] byteSize the size in bytes.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalByteSize(uint32 signalIdx,
                           uint32 &byteSize);

    /**
     * @brief Gets the number of states on which the signal at position \a signalIdx is used.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfStates the number of states where this is used.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfStates(uint32 signalIdx,
                                 uint32 &numberOfStates);

    /**
     * @brief For the signal at position \a signalIdx, gets the name of the state at position \a stateIdx (see GetSignalNumberOfStates)
     * @param[in] signalIdx the index of the signal.
     * @param[in] stateIdx the index of the signal.
     * @param[out] stateName the name of state the at position \a stateIdx for the signal at position \a signalIdx.
     * @return true if the signalIdx and the stateIdx exist.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalStateName(uint32 signalIdx,
                            uint32 stateIdx,
                            StreamString &stateName);

    /**
     * @brief Gets the number of functions consuming the signal at position \a signalIdx in the state with name \a stateName.
     * @param[in] signalIdx the index of the signal.
     * @param[in] stateName the name of the state (see GetSignalStateName).
     * @param[out] numberOfConsumers the number of functions consuming this signal at the state with name \a stateName.
     * @return true if the signalIdx and the stateName exist for this signalIdx.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfConsumers(uint32 signalIdx,
                                    const char8 *stateName,
                                    uint32 &numberOfConsumers);

    /**
     * @brief Gets the number of functions producing the signal at position \a signalIdx in the state with name \a stateName.
     * @param[in] signalIdx the index of the signal.
     * @param[in] stateName the name of the state (see GetSignalStateName).
     * @param[out] numberOfProducers the number of functions producing this signal at the state with name \a stateName.
     * @return true if the signalIdx and the stateName exist for this signalIdx.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfProducers(uint32 signalIdx,
                                    const char8 *stateName,
                                    uint32 &numberOfProducers);
    /**
     * @brief Gets the name of function with index \a consumerIdx and which is consuming the signal at position \a signalIdx in the state with name \a stateName.
     * @param[in] signalIdx the index of the signal.
     * @param[in] stateName the name of the state (see GetSignalStateName).
     * @param[in] consumerIdx the index of the consumer (see GetSignalNumberOfConsumers).
     * @param[out] consumerName the name of function with index \a consumerIdx consuming this signal at the state with name \a stateName.
     * @return true if the signalIdx, the consumerIdx and the stateName exist for this signalIdx.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalConsumerName(uint32 signalIdx,
                               const char8 *stateName,
                               uint32 consumerIdx,
                               StreamString &consumerName);
    /**
     * @brief Gets the name of function with index \a producerIdx and which is producing the signal at position \a signalIdx in the state with name \a stateName.
     * @param[in] signalIdx the index of the signal.
     * @param[in] stateName the name of the state (see GetSignalStateName).
     * @param[in] producerIdx the index of the producer (see GetSignalNumberOfProducers).
     * @param[out] producerName the name of function with index \a consumerIdx producing this signal at the state with name \a stateName.
     * @return true if the signalIdx, the producerIdx and the stateName exist for this signalIdx.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalProducerName(uint32 signalIdx,
                               const char8 *stateName,
                               uint32 producerIdx,
                               StreamString &producerName);

    /**
     * @brief Gets the default of the signal with index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @param[out] defaultValue the default value of the signal.
     * @return true if the signalIdx exists and if a default value was set for this signal.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalDefaultValue(uint32 signalIdx,
                               const AnyType &defaultValue);

    /**
     * @brief Gets the number of functions that interact with this DataSourceI.
     * @return the number of functions that interact with this DataSourceI
     * @pre
     *   SetConfiguredDatabase
     */
    uint32 GetNumberOfFunctions();

    /**
     * @brief Gets the name of the function at index \a functionIdx.
     * @param[in] functionIdx the index of the function.
     * @param[out] functionName the name of the function at index \a functionIdx.
     * @return true if functionIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionName(uint32 functionIdx,
                         StreamString &functionName);

    /**
     * @brief Gets the index of the function with name \a functionName.
     * @param[out] functionIdx the index of the function with name \a functionName.
     * @param[in] functionName the name of the function.
     * @return true if there is a function with name == \a functionName.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionIndex(uint32 &functionIdx,
                          const char8* const functionName);

    /**
     * @brief Gets the number of signals, from the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[out] numberOfSignals the number of signals, from the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @return true if the functionIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionNumberOfSignals(SignalDirection direction,
                                    uint32 functionIdx,
                                    uint32 &numberOfSignals);

    /**
     * @brief Gets the number of bytes, required for the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[out] byteSize the number of bytes, required for the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @return true if the functionIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalsByteSize(SignalDirection direction,
                                    uint32 functionIdx,
                                    uint32 &byteSize);

    /**
     * @brief Gets the name of the signal from the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[out] functioSignalName the name of the signal.
     * @return true if the functionIdx and the functionSignalIdx exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalName(SignalDirection direction,
                               uint32 functionIdx,
                               uint32 functionSignalIdx,
                               StreamString &functioSignalName);

    /**
     * @brief Gets the alias of the signal from the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[out] functionSignalAlias the signal alias or an empty string if this was not specified.
     * @return true if the functionIdx and the functionSignalIdx exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalAlias(SignalDirection direction,
                                uint32 functionIdx,
                                uint32 functionSignalIdx,
                                StreamString &functionSignalAlias);

    /**
     * @brief Gets the index of the signal with the name \a functionSignalName, from the function with index \a functionIdx, which interacts with this DataSourceI (in the specified \a direction).
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[out] functionSignalIdx the index of the signal in this function.
     * @param[in] functionSignalName the name of the signal.
     * @return true if the functionSignalName is a member of this function (in the specified direction).
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalIndex(SignalDirection direction,
                                uint32 functionIdx,
                                uint32 &functionSignalIdx,
                                const char8* const functionSignalName);

    /**
     * @brief Gets the number of different byte offsets (one for each different Range) that were set for the signal with index \a functionSignalIdx.
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[out] numberOfByteOffsets the number of byte offsets.
     * @return true if the functionIdx and the functionSignalIdx exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalNumberOfByteOffsets(SignalDirection direction,
                                              uint32 functionIdx,
                                              uint32 functionSignalIdx,
                                              uint32 &numberOfByteOffsets);

    /**
     * @brief Gets the number of different byte offset index and size (one for each different Range) that were set for the signal with index \a functionSignalIdx.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[in] byteOffsetIndex the index of the offset in this signal.
     * @param[out] byteOffsetStart the offset starting index.
     * @param[out] byteOffsetSize the offset size.
     * @return true if the functionIdx, the functionSignalIdx and the byteOffsetIndex exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalByteOffsetInfo(SignalDirection direction,
                                         uint32 functionIdx,
                                         uint32 functionSignalIdx,
                                         uint32 byteOffsetIndex,
                                         uint32 &byteOffsetStart,
                                         uint32 &byteOffsetSize);

    /**
     * @brief Gets the number of samples that were set for the signal with index \a functionSignalIdx.
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[out] samples the number of samples.
     * @return true if the functionIdx and the functionSignalIdx exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalSamples(SignalDirection direction,
                                  uint32 functionIdx,
                                  uint32 functionSignalIdx,
                                  uint32 &samples);

    /**
     * @brief Gets the read frequency that was set for the signal with index \a functionSignalIdx.
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[out] frequency the frequency at which the signal is to be read.
     * @return true if the functionIdx and the functionSignalIdx exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetFunctionSignalReadFrequency(SignalDirection direction,
                                        uint32 functionIdx,
                                        uint32 functionSignalIdx,
                                        float32 &frequency);

    /**
     * @brief Checks if the broker with name \a brokerClassName is suitable for this signal
     * @param[in] direction the signal direction.
     * @param[in] functionIdx the index of the function.
     * @param[in] functionSignalIdx the index of the signal in this function.
     * @param[in] brokerClassName the name of the broker to be queried.
     * @return true if the broker with name \a brokerClassName is capable of interfacing to this signal.
     * @pre
     *   SetConfiguredDatabase
     */
    bool IsSupportedBroker(SignalDirection direction,
                           uint32 functionIdx,
                           uint32 functionSignalIdx,
                           const char8* brokerClassName);

    /**
     * @brief For every signal in the provided direction assign a Broker that knows how to copy from the DataSourceI memory to the GAM memory.
     * @param[in] direction are the signal directions.
     * @return true if a BrokerI can be assigned to every signal in the given direction.
     */
    bool AddBrokers(SignalDirection direction);

    /**
     * @brief Allocate the memory for this DataSourceI.
     * @return true if the memory can be successfully allocated.
     */
    virtual bool AllocateMemory() = 0;

    /**
     * @brief Gets the number of memory buffers supported by this DataSourceI.
     * @details This information can be used by a BrokerI to implement, for example, a circular buffer interface.
     * @return the number of memory buffers supported by this DataSourceI.
     */
    virtual uint32 GetNumberOfMemoryBuffers() = 0;

    /**
     * @brief Gets the memory address for the signal at index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @param[in] bufferIdx the index of the buffer (see GetNumberOfMemoryBuffers()).
     * @param[out] signalAddress a pointer to the variable that holds the memory address of this signal for this \a bufferIdx.
     * @return true if the signalIdx and the bufferIdx exist and the memory address can be retrieved for this signal.
     * @pre
     *   signalIdx < GetNumberOfSignals() &&
     *   bufferIdx < GetNumberOfMemoryBuffers()
     */
    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
                                       uint32 bufferIdx,
                                       void **&signalAddress) = 0;

    /**
     * @brief Gets the name of the broker for the signal information available in the input \a data.
     * @param[in] data the information about the signal. The structure is:
     *   QualifiedName = "Name of the signal"
     *   NumberOfDimensions = N
     *   NumberOfElements = N
     *   Samples = N
     *   Frequency = N
     * @param[in] direction the signal direction.
     * @return the name of the BrokerI class that will handle the copy of this signal from the DataSourceI memory to the GAM memory.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       SignalDirection direction)=0;

    /**
     * @brief This function is called previous to a state change and allows the DataSourceI to react in advance.
     * @param[in] status information about the state change.
     * @return true if the state change can be performed.
     */
    virtual bool PrepareNextState(const RealTimeStateInfo &status) = 0;

    /**
     * @brief Adds to the \a inputBrokers all the BrokerI instances that will interact with the GAM with name \a functionName.
     * @param[out] inputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if a list of BrokerI instances can be successfully added to the inputBrokers list.
     */
    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* functionName,
                                 void * gamMemPtr)=0;

    /**
     * @brief Adds to the \a outputBrokers all the BrokerI instances that will interact with the GAM with name \a functionName.
     * @param[out] outputBrokers where the BrokerI instances have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be written to.
     * @return true if a list of BrokerI instances can be successfully added to the outputBrokers list.
     */
    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* functionName,
                                  void * gamMemPtr)=0;

protected:

    /**
     * Configuration database with the Signals {} information.
     */
    ConfigurationDatabase signalsDatabase;

    /**
     * Configured database with the information defined in the Data node of the RealTimeApplicationConfigurationBuilder.
     */
    ConfigurationDatabase configuredDatabase;

    /**
     * Number of signals assigned to this function
     */
    uint32 numberOfSignals;

    /**
     * @brief Moves the configuredDatabase to the signal with index \a signalIdx.
     * @param[in] signalIdx the index of the signal where to move to.
     * @return true if the signalIdx exists.
     */
    bool MoveToSignalIndex(uint32 signalIdx);

    /**
     * @brief Moves the configuredDatabase to the function with index \a functionIdx.
     * @param[in] functionIdx the index of the function where to move to.
     * @return true if the functionIdx exists.
     */
    bool MoveToFunctionIndex(uint32 functionIdx);

    /**
     * @brief Moves the configuredDatabase to the signal to the index \a functionIdx in the function with the index \a functionIdx.
     * @param[in] signalIdx the index of the signal where to move to.
     * @param[in] functionIdx the index of the function where to move to.
     * @return true if the functionIdx exists.
     */
    bool MoveToFunctionSignalIndex(SignalDirection direction,
                                   uint32 functionIdx,
                                   uint32 functionSignalIdx);
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCE_H_ */

