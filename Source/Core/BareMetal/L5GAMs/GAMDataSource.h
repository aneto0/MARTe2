/**
 * @file GAMDataSource.h
 * @brief Header file for class GAMDataSource
 * @date 11/04/2016
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

 * @details This header file contains the declaration of the class DataSourceSignal
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMDATASOURCE_H_
#define GAMDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "MemoryArea.h"
#include "ReferenceT.h"
#include "StatefulI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief DataSource implementation for the exchange of signals between GAM components.
 *
 * @details This implementation of the DataSourceI interface is intend for the interchange
 *  of signals in real-time between GAM components.
 *
 * @details The syntax in the input configuration stream (see Initialise) has to be:
 *
 * +GAMDataSource_Name= {\n
 *    Class = GAMDataSource
 *    HeapName = "The name of the Heap to use" If not specified GlobalObjectsDatabase::GetStandardHeap() will be used.
 *    AllowNoProducers = 0 //Optional. If 1 the GAMDataSource will allow for signals not to be connected (only issuing a warning).
 *    ResetUnusedVariablesAtStateChange = 1 //Optional. If 1 the GAMDataSource will reset the value of any input to its default value if the producer was not executed in the current state. 
 * }
 */
class DLL_API GAMDataSource: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    GAMDataSource();

    /**
     * @brief Destructor
     */
    virtual ~GAMDataSource();

    /**
     * @brief Reads the HeapName. Calls the DataSourceI::Initialise()
     * @param[in] data the input configuration data.
     * @return true if DataSourceI::Initialise() returns true.
     * If the HeapName is specified and the HeapI cannot be instantiated this method will return false.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief See DataSourceI::GetNumberOfMemoryBuffers()
     * @return This method always returns 1 (single buffer).
     */
    virtual uint32 GetNumberOfMemoryBuffers();

    /**
     * @brief See DataSourceI::GetSignalMemoryBuffer().
     * @details This method returns the address of the variable containing the requested signal.
     * @param[in] signalIdx the index of the signal.
     * @param[in] bufferIdx the index of the buffer shall be zero.
     * @param[out] signalAddress a pointer to the memory address of this signal for this \a bufferIdx.
     * @return true if the signalIdx and the bufferIdx exist and the memory address can be retrieved for this signal.
     * @pre
     *   signalIdx < GetNumberOfSignals() &&
     *   bufferIdx < GetNumberOfMemoryBuffers() < 1
     */
    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

    /**
     * @brief Allocates the memory required to hold all the signal data allocated to this GAMDataSource.
     * @return true if the memory can be successfully allocated. This function will return false if it called more than once
     *  (to avoid memory leaks).
     */
    virtual bool AllocateMemory();

    /**
     * @brief See DataSourceI::GetBrokerName()
     * @return MemoryMapInputBroker if direction is InputSignals, MemoryMapOutputBroker if the direction is OutputSignals
     *  or NULL if Frequency != -1 and Samples != 1.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction);

    /**
     * @brief Initialises the signals that were not used in the previous state with their default value.
     * details For every signal that was not used in the previous state and that has a default value specified on its configuration,
     *  the first of value of this signal on the next state will be the specified the Default (see RealTimeApplicationConfigurationBuilder).
     *  If the Default is not specified then the signal memory is set to zero for the next state.
     * @param[in] currentStateName the name of the current state being executed.
     * @param[in] nextStateName the name of the next state to be executed.
     * @return true if the state change can be performed and all the default values successfully applied.
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Tries to initialise a MemoryMapInputBroker with the function input parameters.
     * @param[out] inputBrokers where the MemoryMapInputBroker instance will have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if a the MemoryMapInputBroker can be successfully initialised (see MemoryMapInputBroker::Init)
     */
    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8 * const functionName,
            void * const gamMemPtr);

    /**
     * @brief Tries to initialise a MemoryMapOutputBroker with the function input parameters.
     * @param[out] outputBrokers where the MemoryMapOutputBroker instance will have to be added to.
     * @param[in] functionName name of the function being queried.
     * @param[in] gamMemPtr the GAM memory where the signals will be read from.
     * @return true if a the MemoryMapOutputBroker can be successfully initialised (see MemoryMapOutputBroker::Init)
     */
    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8 * const functionName,
            void * const gamMemPtr);

    /**
     * @brief Not supported (see DataSourceI::Synchronise)
     * @return false.
     */
    virtual bool Synchronise();

    /**
     * @brief Calls DataSourceI::SetConfiguredDatabase and verifies that there is one and only one
     * producer for each consumer on each state.
     * @param[in] data see DataSourceI::SetConfiguredDatabase
     * @return true if DataSourceI::SetConfiguredDatabase returns true and if there is one and only one
     * producer for each consumer on each state.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);

protected:
    /**
     * The single buffer memory.
     */
    void *signalMemory;

    /**
     * The signal offsets
     */
    uint32 *signalOffsets;

    /**
     * The HeapI to allocate the signal memory.
     */
    HeapI *memoryHeap;

    /**
     * Allow no producers
     */
    bool allowNoProducers;

    /**
     * Reset unused variables to the default value?
     */
    bool resetUnusedVariablesAtStateChange;

    /**
     * Force the reset of the unused variables the first time.
     */
    bool forceResetUnusedVariablesAtStateChange;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMDATASOURCE_H_ */

