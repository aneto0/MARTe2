/**
 * @file GAM.h
 * @brief Header file for class GAM
 * @date 07/04/2016
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

 * @details This header file contains the declaration of the class GAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAM_H_
#define GAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "ExecutableI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The MARTe application module.
 *
 * @details Classes inheriting from GAM can be scheduled by a RealTimeThread
 * and exchange signals with other GAM instances.
 *
 * The minimum configuration structure is (more information about the syntax
 * in RealTimeApplicationConfigurationBuilder):
 * +ThisGAMName = {"
 *    Class = ClassThatInheritsFromGAM"
 *    Signals = {
 *        InputSignals|OutputSignals = {
 *            NAME*={
 *                +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
 *                +DataSource = "QualifiedName of the DataSource"
 *                +Type = BasicType|StructuredType
 *                +NumberOfDimensions = 0|1|2
 *                +NumberOfElements = NUMBER>0
 *                +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap)
 *                +Samples = NUMBER > 0, defines the number of (time) samples to be copied on each operation. The default value is one.
 *                +Frequency = NUMBER>0, defines the cycle time frequency. Only and only one signal may define this property.
 *                +Default = "Default value as a string". The value to be used when the signal is not produced in a previous state.
 *                +MemberAliases = {//Only valid for StructuredType signals
 *                   OriginalMemberName1 = NewMemberName1
 *                   ...
 *                }
 *                +Defaults = {//Only valid for StructuredType signals
 *                   MemberName1 = DefaultValue1
 *                   ...
 *                }
 *            }
 *         }
 *     }
 * }
 */
class DLL_API GAM: public ReferenceContainer, public ExecutableI {
public:

    /**
     * Allows the DataSourceI implementations to have access to the
     * GAM memory. This strategy makes the DataSourceI the only class
     * which will have direct access to the GAM memory.
     */
    friend class DataSourceI;

    /**
     * @brief Default constructor
     * @post
     *   GetNumberOfInputSignals() == 0 &&
     *   GetNumberOfOutputSignals() == 0 &&
     *   GetInputSignalsMemory() == NULL &&
     *   GetOutputSignalsMemory() == NULL
     */
    GAM();

    /**
     * @brief Destructor. Free the Input and OutputSignals memory.
     */
    virtual ~GAM();

    /**
     * @brief Initialises the signals database.
     * @details Initialises the ReferenceContainer with input \a data StructuredDataI.
     * Moves to the node "Signals" in the input \a data StructuredDataI and
     * locally stores the information of all the available Input/Output signals.
     * @param[in] data The configuration information which may include a Signals node.
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
     * @brief Assigns a valid and StructuredDataI to this GAM.
     * @details This method is called by the RealTimeApplicationConfigurationBuilder once all
     *  the information has been validated and negotiated. The structure of this information is (the syntax is described in RealTimeApplicationConfigurationBuilder):
     *  QualifiedName = "x.y.GAMNAME"
     *      Signals = {
     *          InputSignals|OutputSignals = {
     *              *NUMBER = {
     *                 QualifiedName = "QualifiedName of the signal"
     *                 DataSource = "QualifiedName of the DataSource"
     *                 +Alias = "Path.In.Data.Source (Otherwise SignalName = NAME)"
     *                 Type = BasicType
     *                 NumberOfDimensions = 0|1|2
     *                 NumberOfElements = NUMBER>0
     *                 +Ranges = {{min_idx:max_idx} {min_idx:max_idx} ...} (min_idx<=max_idx indexes may not overlap) (max_idx<NumberOfElements)
     *                 ByteSize = NUMBER>0
     *                 ByteOffset = { { min_idx_bytes range_bytes } { min_idx_bytes range_bytes } ...}
     *                 +Samples = NUMBER > 0
     *                 +Frequency = NUMBER>0
     *              }
     *          }
     *      }
     *  @param[in] data the configured database of parameters.
     *  @return true if the \a data can be successfully copied.
     */
    bool SetConfiguredDatabase(const ConfigurationDatabase & data);

    /**
     * @brief Returns the number of input signals.
     * @return the number of input signals.
     */
    uint32 GetNumberOfInputSignals() const;

    /**
     * @brief Returns the number of output signals.
     * @return the number of output signals.
     */
    uint32 GetNumberOfOutputSignals() const;

    /**
     * @brief Returns the GAM fully qualified name.
     * @param[out] qualifiedName where to write the GAM qualified name.
     * @return false if the GAM qualified name does not exist.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetQualifiedName(StreamString &qualifiedName);

    /**
     * @brief Gets the name of the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] signalName where to write the signal name.
     * @return true if the signalIdx exists.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalName(const SignalDirection direction, const uint32 signalIdx, StreamString &signalName);

    /**
     * @brief Gets the index of the signal with name \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[out] signalIdx the index of the signal.
     * @param[in] signalName the name of the signal.
     * @return true if the signalName can be found.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalIndex(const SignalDirection direction, uint32 &signalIdx, const char8* const signalName);

    /**
     * @brief Gets the DataSourceI name of the signal with name \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] dataSourceName the name of the DataSourceI.
     * @return true if the signalIdx exists and the DataSource configuration field is defined.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalDataSourceName(const SignalDirection direction, const uint32 signalIdx, StreamString &dataSourceName);

    /**
     * @brief Gets the type of the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @return the signal type or InvalidType if the signal is not found.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    TypeDescriptor GetSignalType(const SignalDirection direction, const uint32 signalIdx);

    /**
     * @brief Gets the number of dimensions of the signal at position \a signalIdx.
     * @details The default number of dimensions of a signal is 0.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfDimensions the number of dimensions.
     * @return true if the signalIdx exists.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalNumberOfDimensions(const SignalDirection direction, const uint32 signalIdx, uint32 &numberOfDimensions);

    /**
     * @brief Gets the number of elements of the signal at position \a signalIdx.
     * @details The default number of elements of a signal is 1.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfElements the number of elements.
     * @return true if the signalIdx exists.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalNumberOfElements(const SignalDirection direction, const uint32 signalIdx, uint32 &numberOfElements);

    /**
     * @brief Gets the default of the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] defaultValue the default value of the signal.
     * @return true if the signalIdx exists and if a default value was set for this signal.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalDefaultValue(const SignalDirection direction, const uint32 signalIdx, const AnyType &defaultValue);

    /**
     * @brief Gets the number of bytes required to represent the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] byteSize the size in bytes.
     * @return true if the signalIdx exists.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalByteSize(const SignalDirection direction, const uint32 signalIdx, uint32 &byteSize);

    /**
     * @brief Gets the number of different byte offsets (one for each different Range) that were set for the signal with index \a signalIdx.
     * @details If no ranges were defined this function will return 0.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[out] numberOfByteOffsets the number of byte offsets.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalNumberOfByteOffsets(const SignalDirection direction, const uint32 signalIdx, uint32 &numberOfByteOffsets);

    /**
     * @brief Gets the byte offset index and size (one for each different Range) that were set for the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[in] byteOffsetIndex the index of the offset in this signal.
     * @param[out] byteOffsetStart the offset starting index.
     * @param[out] byteOffsetSize the offset size.
     * @return true if the signalIdx and the byteOffsetIndex exist in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalByteOffsetInfo(const SignalDirection direction, const uint32 signalIdx, const uint32 byteOffsetIndex, uint32 &byteOffsetStart, uint32 &byteOffsetSize);

    /**
     * @brief Gets the number of ranges that were set for the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[out] numberOfRanges the number of ranges or 0 if no ranges were defined.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalNumberOfRanges(const SignalDirection direction, const uint32 signalIdx, uint32 &numberOfRanges);

    /**
     * @brief Gets the range start and end index (one for each different Range) that were set for the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[in] rangeIndex the index of the offset in this signal.
     * @param[out] rangeStart the offset starting index.
     * @param[out] rangeEnd the offset size.
     * @return true if the signalIdx and the rangeIndex exist in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalRangesInfo(const SignalDirection direction, const uint32 signalIdx, const uint32 rangeIndex, uint32 &rangeStart, uint32 &rangeEnd);

    /**
     * @brief Gets the number of samples that were set for the signal with index \a signalIdx.
     * @details The Samples parameter defines the number of samples that the GAM would like to receive/transmit for any given signal.
     * Note that these are samples which are acquired as a function of time (not multi-dimensional arrays of data which should be set with
     * the NumberOfElements and NumberOfDimensions).
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[out] numberOfSamples the number of samples set for this signal.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalNumberOfSamples(const SignalDirection direction, const uint32 signalIdx, uint32 &numberOfSamples);

    /**
     * @brief Gets the frequency that was set for the signal with index \a functionSignalIdx.
     * @details The Frequency parameter defines the rate at which the signal is expected to be produced.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this GAM.
     * @param[out] frequency the frequency at which the signal is to be read/written.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    bool GetSignalFrequency(const SignalDirection direction, const uint32 signalIdx, float32 &frequency);

    /**
     * @brief Allocates all memory required to hold all the input signals of this GAM.
     * @return true if the memory can be successfully allocated.
     * @pre
     *   The ConfiguredDatabase must be set &&
     *   GetInputSignalsMemory() == NULL
     * @remark From the precondition is inferred that this function cannot be called twice.
     */
    bool AllocateInputSignalsMemory();

    /**
     * @brief Allocates all memory required to hold all the output signals of this GAM.
     * @return true if the memory can be successfully allocated.
     * @pre
     *   The ConfiguredDatabase must be set &&
     *   GetOutputSignalsMemory() == NULL
     * @remark From the precondition is inferred that this function cannot be called twice.
     */
    bool AllocateOutputSignalsMemory();

    /**
     * @brief Adds a list of input BrokerI components to this GAM.
     * @details These BrokerI components will be responsible from copying the data from the
     * DataSourceI memory to the GAM memory.
     * @param[in] brokers the list of input BrokerI components to be added.
     * @return true if all the components in brokers implement the BrokerI interface if these can be successfully to the input broker list.
     */
    bool AddInputBrokers(ReferenceContainer brokers);

    /**
     * @brief Adds a list of output BrokerI components to this GAM.
     * @details These BrokerI components will be responsible from copying the data from the
     * GAM memory to the DataSourceImemory.
     * @param[in] brokers the list of output BrokerI components to be added.
     * @return true if all the components in brokers implement the BrokerI interface if these can be successfully to the output broker list.
     */
    bool AddOutputBrokers(ReferenceContainer brokers);

    /**
     * @brief Sorts internally the brokers following the signal order.
     * @return true if success, false otherwise
     */
    bool SortBrokers();

    /**
     * @brief Gets the list of input BrokerI components that are associated to this GAM.
     * @details These BrokerI components will be responsible from copying the data from the
     * DataSourceI memory to the GAM memory.
     * @param[out] brokers the list where the input BrokerI are to be added.
     * @return true if all brokers can be successfully to the input broker list.
     */
    bool GetInputBrokers(ReferenceContainer &brokers);

    /**
     * @brief Gets the list of output BrokerI components that are associated to this GAM.
     * @details These BrokerI components will be responsible from copying the data from the
     * GAM memory to the DataSourceI memory.
     * @param[out] brokers the list where the input BrokerI are to be added.
     * @return true if all brokers can be successfully to the output broker list.
     */
    bool GetOutputBrokers(ReferenceContainer &brokers);

    /**
     * @brief Sets a GAM shared context.
     * @details If this GAM belongs to a GAMGroup (association performed in the configuration stage of a RealTimeApplication)
     *  then this method will be called by the GAMGroup and will allow this GAM to share a common \a context with all the GAMs
     *  that belong to that GAMGroup. This method only is only meaningful if further specialised.
     * @param[in] context the context to be shared. Its final type is to be defined on each specific GAM application.
     * @return true if the provided context is accepted by the GAM.
     */
    virtual bool SetContext(ConstReference context);

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief Function called when all the information about the GAM is known and set.
     * @details This allows to perform post-Initialise configurations. When this function is called
     * all the information about the GAM is known (e.g. GetNumberOfInputSignals ...).
     * @return true if the GAM is successfully configured.
     */
    virtual bool Setup()=0;


    /**
     * @see ReferenceContainer::ExportData(*)
     * @details Also exports the current value of all the input and output signals.
     */
    virtual bool ExportData(StructuredDataI & data);


protected:

    /**
     * @brief Returns a pointer to the beginning of the input signals memory.
     * @return a pointer to the beginning of the input signals memory.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    void *GetInputSignalsMemory();

    /**
     * @brief Returns a pointer to the beginning of the output signals memory.
     * @return a pointer to the beginning of the output signals memory.
     * @pre
     *   The ConfiguredDatabase must be set
     */
    void *GetOutputSignalsMemory();

    /**
     * @brief Returns a pointer to the beginning of the input signal memory with index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @return a pointer to the beginning of the input signal memory with index \a signalIdx or NULL if signalIdx >= GetNumberOfInputSignals().
     * @pre
     *   The ConfiguredDatabase must be set &&
     *   signalIdx < GetNumberOfInputSignals()
     */
    void *GetInputSignalMemory(const uint32 signalIdx) const;

    /**
     * @brief Returns a pointer to the beginning of the output signal memory with index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @return a pointer to the beginning of the output signal memory with index \a signalIdx or NULL if signalIdx >= GetNumberOfOutputSignals().
     * @pre
     *   The ConfiguredDatabase must be set &&
     *   signalIdx < GetNumberOfOutputSignals()
     */
    void *GetOutputSignalMemory(const uint32 signalIdx) const;

    /**
     * Holds the Signals definition which are received in the Initialise phase.
     */
    ConfigurationDatabase signalsDatabase;

    /**
     * Holds the final GAM configuration data (see RealTimeApplicationConfigurationBuilder).
     */
    ConfigurationDatabase configuredDatabase;

    /**
     * Holds a pointer to the input signals memory.
     */
    void *inputSignalsMemory;

    /**
     * Holds a pointer to the output signals memory.
     */
    void *outputSignalsMemory;

    /**
     * Provides direct access to the desired input signal
     */
    void **inputSignalsMemoryIndexer;

    /**
     * Provides direct access to the desired output signal
     */
    void **outputSignalsMemoryIndexer;

    /**
     * Number of input signals.
     */
    uint32 numberOfInputSignals;

    /**
     * Number of output signals.
     */
    uint32 numberOfOutputSignals;

    /**
     * @brief Moves the configuredDatabase to the \a signalIdx
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the signal index.
     * @return true if the Move is successful.
     */
    bool MoveToSignalIndex(const SignalDirection direction,
    const uint32 signalIdx);

    /**
     * Brokers for signal reading.
     */
    ReferenceContainer inputBrokers;

    /**
     * Brokers for signal writing.
     */
    ReferenceContainer outputBrokers;

    /**
     * The gamHeap that is used to malloc the input and output signals.
     */
    HeapI *gamHeap;

    /**
     * Accelerator reference for the inputSignalsDatabaseNode.
     */
    ConfigurationDatabase inputSignalsDatabaseNode;

    /**
     * Accelerator reference for the outputSignalsDatabaseNode.
     */
    ConfigurationDatabase outputSignalsDatabaseNode;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* GAM_H_ */
