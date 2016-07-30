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

#include "GAM.h"
#include "ExecutableI.h"
#include "GAMGroup.h"
#include "RealTimeApplication.h"
#include "BrokerContainer.h"
#include "GAMContextT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The MARTe application module.
 * @details Classes inheriting from GAM can be scheduled by a RealTimeThread and
 * exchange signals with other GAM instances.
 * The minimum configuration structure is (more information about the syntax in RealTimeApplicationConfigurationBuilder):
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
class DLL_API GAM: public ExecutableI {
public:
    /**
     * @brief Constructor
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
    bool SetConfiguredDatabase(StructuredDataI & data);

    /**
     * @brief Returns the number of input signals.
     * @return the number of input signals.
     */
    uint32 GetNumberOfInputSignals();

    /**
     * @brief Returns the number of output signals.
     * @return the number of output signals.
     */
    uint32 GetNumberOfOutputSignals();

    /**
     * @brief Gets the name of the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] signalName where to write the signal name.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalName(SignalDirection direction,
                       uint32 signalIdx,
                       StreamString &signalName);

    /**
     * @brief Gets the index of the signal with name \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[out] signalIdx the index of the signal.
     * @param[in] signalName the name of the signal.
     * @return true if the signalName can be found.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalIndex(SignalDirection direction,
                        uint32 &signalIdx,
                        const char8* const signalName);

    /**
     * @brief Gets the DataSourceI name of the signal with name \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] dataSourceName the name of the DataSourceI.
     * @return true if the signalIdx exists and the DataSource configuration field is defined.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalDataSourceName(SignalDirection direction,
                                 uint32 signalIdx,
                                 StreamString &dataSourceName);

    /**
     * @brief Gets the type of the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @return the signal type or InvalidType if the signal is not found.
     * @pre
     *   SetConfiguredDatabase
     */
    TypeDescriptor GetSignalType(SignalDirection direction,
                                 uint32 signalIdx);

    /**
     * @brief Gets the number of dimensions of the signal at position \a signalIdx.
     * @details The default number of dimensions of a signal is 0.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfDimensions the number of dimensions.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfDimensions(SignalDirection direction,
                                     uint32 signalIdx,
                                     uint32 &numberOfDimensions);

    /**
     * @brief Gets the number of elements of the signal at position \a signalIdx.
     * @details The default number of elements of a signal is 1.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] numberOfDimensions the number of elements.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfElements(SignalDirection direction,
                                   uint32 signalIdx,
                                   uint32 &numberOfElements);

    /**
     * @brief Gets the default of the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] defaultValue the default value of the signal.
     * @return true if the signalIdx exists and if a default value was set for this signal.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalDefaultValue(SignalDirection direction,
                               uint32 signalIdx,
                               const AnyType &defaultValue);

    /**
     * @brief Gets the number of bytes required to represent the signal at position \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal.
     * @param[out] byteSize the size in bytes.
     * @return true if the signalIdx exists.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalByteSize(SignalDirection direction,
                           uint32 signalIdx,
                           uint32 &byteSize);

    /**
     * @brief Gets the number of different byte offsets (one for each different Range) that were set for the signal with index \a signalIdx.
     * @details If no ranges were defined this function will return 0.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[out] numberOfByteOffsets the number of byte offsets.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfByteOffsets(SignalDirection direction,
                                      uint32 signalIdx,
                                      uint32 &numberOfByteOffsets);

    /**
     * @brief Gets the byte offset index and size (one for each different Range) that were set for the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[in] byteOffsetIndex the index of the offset in this signal.
     * @param[out] byteOffsetStart the offset starting index.
     * @param[out] byteOffsetSize the offset size.
     * @return true if the signalIdx and the byteOffsetIndex exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalByteOffsetInfo(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 byteOffsetIndex,
                                 uint32 &byteOffsetStart,
                                 uint32 &byteOffsetSize);

    /**
     * @brief Gets the number of ranges that were set for the signal with index \a signalIdx.
     * @details If no ranges were defined this function will return 0.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[out] numberOfRanges the number of ranges.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfRanges(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 &numberOfRanges);

    /**
     * @brief Gets the range start and end index (one for each different Range) that were set for the signal with index \a signalIdx.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this function.
     * @param[in] rangeIndex the index of the offset in this signal.
     * @param[out] rangeStart the offset starting index.
     * @param[out] rangeEnd the offset size.
     * @return true if the signalIdx and the rangeIndex exist in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalRangesInfo(SignalDirection direction,
                             uint32 signalIdx,
                             uint32 rangeIndex,
                             uint32 &rangeStart,
                             uint32 &rangeEnd);

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
     *   SetConfiguredDatabase
     */
    bool GetSignalNumberOfSamples(SignalDirection direction,
                                  uint32 signalIdx,
                                  uint32 &numberOfSamples);

    /**
     * @brief Gets the frequency that was set for the signal with index \a functionSignalIdx.
     * @details The Frequency parameter defines the rate at which the signal is expected to be produced.
     * @param[in] direction the signal direction.
     * @param[in] signalIdx the index of the signal in this GAM.
     * @param[out] frequency the frequency at which the signal is to be read/written.
     * @return true if the signalIdx exists in the specified direction.
     * @pre
     *   SetConfiguredDatabase
     */
    bool GetSignalFrequency(SignalDirection direction,
                            uint32 signalIdx,
                            float32 &frequency);

    /**
     * @brief Allocates all memory required to hold all the input signals of this GAM.
     * @return true if the memory can be successfully allocated.
     * @pre
     *   SetConfiguredDatabase &&
     *   GetInputSignalsMemory == NULL (this function cannot be called twice).
     */
    bool AllocateInputSignalsMemory();

    /**
     * @brief Allocates all memory required to hold all the output signals of this GAM.
     * @return true if the memory can be successfully allocated.
     * @pre
     *   SetConfiguredDatabase &&
     *   GetOutputSignalsMemory == NULL (this function cannot be called twice).
     */
    bool AllocateOutputSignalsMemory();

    /**
     * @brief Returns a pointer to the beginning of the input signals memory.
     * @return a pointer to the beginning of the input signals memory.
     * @pre
     *   SetConfiguredDatabase
     */
    void *GetInputSignalsMemory();

    /**
     * @brief Returns a pointer to the beginning of the output signals memory.
     * @return a pointer to the beginning of the output signals memory.
     * @pre
     *   SetConfiguredDatabase
     */
    void *GetOutputSignalsMemory();

    /**
     * @brief Returns a pointer to the beginning of the input signal memory with index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @return a pointer to the beginning of the input signal memory with index \a signalIdx or NULL if signalIdx >= GetNumberOfInputSignals().
     * @pre
     *   SetConfiguredDatabase &&
     *   signalIdx < GetNumberOfInputSignals()
     */
    void *GetInputSignalMemory(uint32 signalIdx);

    /**
     * @brief Returns a pointer to the beginning of the output signal memory with index \a signalIdx.
     * @param[in] signalIdx the index of the signal.
     * @return a pointer to the beginning of the output signal memory with index \a signalIdx or NULL if signalIdx >= GetNumberOfOutputSignals().
     * @pre
     *   SetConfiguredDatabase &&
     *   signalIdx < GetNumberOfOutputSignals()
     */
    void *GetOutputSignalMemory(uint32 signalIdx);

    /**
     * TODO
     */
    void AddInputBrokers(ReferenceContainer brokers);

    /**
     * TODO
     */
    void AddOutputBrokers(ReferenceContainer brokers);

    /**
     * TODO
     */
    ReferenceContainer GetInputBrokers();

    /**
     * TODO
     */
    ReferenceContainer GetOutputBrokers();

    /**
     * TODO
     */
    void *GetContext();

protected:

    /**
     * Holds the Signals definition which is received in the Initialise phase.
     */
    ConfigurationDatabase signalsDatabase;

    /**
     * Hols the final GAM configuration data (see RealTimeApplicationConfigurationBuilder).
     */
    ConfigurationDatabase configuredDatabase;

    /**
     * Input signals memory.
     */
    void *inputSignalsMemory;

    /**
     * Output signals memory.
     */
    void *outputSignalsMemory;

    /**
     * Provides direct access to the desired signal
     */
    void **inputSignalsMemoryIndexer;

    /**
     * Provides direct access to the desired signal
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
    bool MoveToSignalIndex(SignalDirection direction,
                           uint32 signalIdx);

    /**
     * Brokers for signal reading.
     */
    ReferenceContainer inputBrokers;

    /**
     * Brokers for signal writing.
     */
    ReferenceContainer outputBrokers;

    /**
     * The heap that is used to malloc the input and output signals.
     */
    HeapI *heap;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_GAM_H_ */

