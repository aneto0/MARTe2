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
 *            +Frequency = -1|NUMBER>0, the frequency at which the signal is expected to be produced. -1 => the latest value available
 *                   (i.e. the frequency is not important), any other positive number is the desired frequency. The default is -1
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
     * @param[out] numberOfConsumers the number of functions consuming this signal at the the state with name \a stateName.
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
     * @param[out] numberOfProducers the number of functions producing this signal at the the state with name \a stateName.
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
     * @param[out] consumerName the name of function with index \a consumerIdx consuming this signal at the the state with name \a stateName.
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
     * @param[out] producerName the name of function with index \a consumerIdx producing this signal at the the state with name \a stateName.
     * @return true if the signalIdx, the producerIdx and the stateName exist for this signalIdx.
     * @pre
     *   SetConfiguredDatabase
     */
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

