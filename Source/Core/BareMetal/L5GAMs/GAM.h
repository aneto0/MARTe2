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
#include "GAMSignalI.h"
#include "RealTimeApplication.h"
#include "BrokerContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Implementation of GAM with BasicInputReader and BasicOutputWriter
 * used to link the GAM with the RealTimeDataSource.
 */
class DLL_API GAM: public ExecutableI {
public:
    /**
     * @brief Constructor
     */
    GAM();

    /**
     * @brief Destructor
     * @details Frees the array containing the supported states names.
     */
    virtual ~GAM();

    /**
     * @brief calls the Initialise(*) function for each sub-node, then calls the functions
     * SetUp(*) and ConfigureFunction(*) due to initialise the local environment.
     * @param[in] data contains the initialisation data.
     * @brief returns false if the eventual merge with the local configuration data fails, true otherwise.
     */
    virtual bool Initialise(StructuredDataI & data);

    virtual bool Finalise();

    bool AddSignals(StructuredDataI & data);

    /**
     * This function will be called once per DataSource!
     */
    void * AllocateInputSignalsMemory(uint32 numberOfBytes);

    /**
     * This function will be called once per DataSource!
     */
    void * AllocateOutputSignalsMemory(uint32 numberOfBytes);

    const void * GetInputSignalsBuffer();

    void * GetOutputSignalsBuffer();

    bool SetConfiguredDatabase(StructuredDataI & data);

    uint32 GetNumberOfInputSignals();

    uint32 GetNumberOfOutputSignals();

    bool GetSignalName(SignalDirection direction,
                       uint32 signalIdx,
                       StreamString &signalName);

    bool GetSignalIndex(SignalDirection direction,
                        uint32 &signalIdx,
                        const char8* const signalName);

    bool GetSignalDataSourceName(SignalDirection direction,
                                 uint32 signalIdx,
                                 StreamString &dataSourceName);

    TypeDescriptor GetSignalType(SignalDirection direction,
                                 uint32 signalIdx);

    bool GetSignalNumberOfDimensions(SignalDirection direction,
                                     uint32 signalIdx,
                                     uint32 &numberOfDimensions);

    bool GetSignalNumberElements(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 &numberOfElements);

    bool GetSignalByteSize(SignalDirection direction,
                           uint32 signalIdx,
                           uint32 &byteSize);

    bool GetSignalNumberOfByteOffsets(SignalDirection direction,
                                      uint32 signalIdx,
                                      uint32 &numberOfByteOffsets);

    bool GetSignalByteOffsetInfo(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 byteOffsetIndex,
                                 uint32 &byteOffsetStart,
                                 uint32 &byteOffsetSize);

    bool GetSignalNumberOfRanges(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 &numberOfRanges);

    bool GetSignalRangesInfo(SignalDirection direction,
                             uint32 signalIdx,
                             uint32 rangeIndex,
                             uint32 &rangeStart,
                             uint32 &rangeEnd);

    bool GetSignalTimeCyclesInfo(SignalDirection direction,
                                 uint32 signalIdx,
                                 uint32 &timeCycles,
                                 uint32 &timeSamples);

    void AddInputBrokers(ReferenceContainer brokers);

    void AddOutputBrokers(ReferenceContainer brokers);

    ReferenceContainer GetInputBrokers();

    ReferenceContainer GetOutputBrokers();

    bool AllocateInputSignalsMemory2();

    bool AllocateOutputSignalsMemory2();

    void *GetInputMemoryPointer();

    void *GetOutputMemoryPointer();

#if 0
    /**
     * @brief Links the signals of this GAM to the RealTimeApplication DataSourceContainer (+Data).
     * @details @see DataSourceContainer::AddDataDefinition(*).
     * @return true if the RealTimeApplication where this GAM belongs to is valid and if calling
     * DataSourceContainer.AddDataDefinition for this GAM returns true.
     */
    virtual bool ConfigureDataSource();

    /**
     * @brief Links this GAM to its RealTimeApplication.
     * @param[in] rtApp is a reference to the RealTimeApplication where this GAM is declared into.
     */
    void SetApplication(RealTimeApplication &rtApp);

    /**
     * @brief Links this GAM to its GAMGroup.
     * @param[in] gamGroup is the GAMGroup involving this GAM.
     */
    void SetGAMGroup(ReferenceT<GAMGroup> gamGroup);

    /**
     * @brief Adds the name of a RealTimeState where this GAM is declared into.
     * @param[in] stateName is the RealTimeState name.
     * @param[in] threadName is the RealTimeThread name.
     * @return false if the same state was already added with a different thread name. This means that the
     * configuration is wrong: one GAM can be declared in only one RealTimeThread for each RealTimeState.
     * Returns true otherwise.
     */
    bool AddState(const char8 * const stateName,
            const char8 * const threadName);

    /**
     * @brief Retrieves the states names where this class is declared into.
     * @return the states names where this class is declared into.
     */
    StreamString *GetSupportedStates();

    /**
     * @brief Retrieves the thread names where this class is declared into. The threads
     * are related one by one with the state names array returned by GetSupportedStates().
     * @return the thread names where this class is declared into.
     */
    StreamString *GetSupportedThreads();

    /**
     * @brief Returns the number of the supported states.
     * @return the number of the supported states.
     */
    uint32 GetNumberOfSupportedStates() const;

    /**
     * @brief Links the GAM with RealTimeDataSource.
     * @details Configures the input (RealTimeDataInputReader) and output interfaces (RealTimeDataOutputWriter)
     * to communicate with the RealTimeDataSource.
     */
    // can make accelerators to internal reader and writer
    virtual bool ConfigureDataSourceLinks();

    /**
     * @brief Retrieves the input interface with the RealTimeDataSource.
     * @return the input interface with the RealTimeDataSource.
     */
    virtual Reference GetInputReader();

    /**
     * @brief Retrieves the output interface with the RealTimeDataSource.
     * @return the output interface with the RealTimeDataSource.
     */
    virtual Reference GetOutputWriter();

    /**
     * @brief Specifies if this GAM is synchronising.
     * @return true if this GAM is linked to a synchronising RealTimeDataSourceDef, false otherwise.
     */

    RealTimeApplication *GetApplication();

    virtual bool IsSync();

protected:
    /**
     * @brief Setup the GAM.
     * @details Initialises the local status (memory allocation
     * of the IO structures, local configuration file, ecc)
     */
    virtual void SetUp()=0;

    /**
     * The input interface with the RealTimeDataSource
     */
    ReferenceT<BrokerContainer> inputReaders;

    ReferenceT<BrokerContainer> outputWriters;

    /**
     * The names of the supported states
     */
    StreamString *supportedStates;

    /**
     * The thread associated to each supported state.
     */
    StreamString *supportedThreads;

    /**
     * How many supported states
     */
    uint32 numberOfSupportedStates;

    /**
     * The local configuration
     */
    StructuredDataI* localData;

    /**
     * Link to the RealTimeApplication
     */
    RealTimeApplication *application;

    /**
     * Link to the GAMGroup
     */
    GAMGroup *group;

private:
    /**
     * @brief Completes the IO structure definitions (see GAMSignalI) found in the global CDB
     * with the definitions in the local CDB and check their consistency.
     * @param[in] localData is the local StructuredData.
     * @return false in case of conflicts between the local and the global definitions, or
     * if the definitions are inconsistent with registered types. True otherwise.
     */
    bool ConfigureFunction();
#endif
    ConfigurationDatabase signalsDatabase;

    void *inputSignalsMemory;

    void *outputSignalsMemory;

    uint32 numberOfInputSignals;

    uint32 numberOfOutputSignals;

    ConfigurationDatabase configuredDatabase;

    bool MoveToSignalIndex(SignalDirection direction,
                           uint32 signalIdx);

    ReferenceContainer inputBrokers;

    ReferenceContainer outputBrokers;

    HeapI *heap;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_GAM_H_ */

