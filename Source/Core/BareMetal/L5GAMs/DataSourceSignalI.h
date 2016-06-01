/**
 * @file DataSourceSignalII.h
 * @brief Header file for class DataSourceSignalII
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

 * @details This header file contains the declaration of the class DataSourceSignalII
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCESIGNALI_H_
#define DATASOURCESIGNALI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RealTimeStateInfo.h"
#include "MemoryArea.h"
#include "RealTimeApplication.h"
#include "DataSourceBrokerI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface of a data source signal shared between GAMs.
 *
 * @details Provides the implementation of the functions shared by all data source
 * signal types but the functions which can change between different type of signals
 * are declared pure virtual due to allow custom implementations.
 */
class DLL_API DataSourceSignalI: public ReferenceContainer {
public:

    /**
     * @brief Constructor
     * @post
     *   GetNumberOfElements() == 0 &&
     *   GetType() == "" &&
     *   GetNumberOfSamples == 1
     */
    DataSourceSignalI();

    /**
     * @brief Destructor
     */
    virtual ~DataSourceSignalI();

    /**
     * @brief Sets a pointer to the RealTimeApplication containing this signal.
     * @param[in] app is the reference to the RealTimeApplication.
     */
    void SetApplication(RealTimeApplication &app);

    /**
     * @brief Adds a GAM as a consumer.
     * @param[in] stateIn the state name where \a gamIn is involved as a consumer of this signal.
     * @param[in] gamIn is the GAM which will consume this variable in the state \a stateIn.
     * @return false in case of errors, false otherwise.
     */
    bool AddConsumer(Reference gamIn);

    /**
     * @brief Adds a GAM as a producer.
     * @param[in] stateIn the state name where \a gamIn is involved as a producer of this signal.
     * @param[in] gamIn is the GAM which will produce this variable in the state \a stateIn.
     * @return false in case of errors, false otherwise.
     */
    bool AddProducer(Reference gamIn);

    /**
     * @brief Retrieves the number of consumer GAMs of this signal in the specified state.
     * @param[in] statIn is the name of the state to be checked.
     * @return the number of consumer GAMs of this variable in \a stateIn.
     */
    uint32 GetNumberOfConsumers(const char8 * const stateIn);

    /**
     * @brief Retrieves the number of producer GAMs of this variable in the specified state.
     * @param[in] statIn is the name of the state to be checked.
     * @return the number of producer GAMs of this variable in \a stateIn.
     */
    uint32 GetNumberOfProducers(const char8 * const stateIn);

    /**
     * @brief Checks that for each state, at most one producer GAM is defined.
     * @return true if there is no more than one producer GAM for each state.
     */
    bool Verify();

    /**
     * @brief Retrieves the signal number of elements.
     * @details The signal can be read (write) to (from) GAMSignalsI which could be vectors or matrices.
     * @return the number of elements.
     */
    uint32 GetNumberOfElements() const;

    /**
     * @brief Retrieves the signal number of samples.
     * @return the signal number of samples.
     */
    uint32 GetNumberOfSamples() const;

    /**
     * @brief Retrieves the variable type name.
     * @return the variable type name.
     */
    const char8 *GetType();

    /**
     * @brief Prepares the environment for the next state.
     * @param[in] status contains informations about the current, the next state and the current
     * active buffer.
     * @return true if the default value set is compatible with the variable type, false otherwise.
     */
    virtual bool PrepareNextState(const RealTimeStateInfo &status)=0;

    /**
     * @brief Allocates the memory for this signal.
     * @param[in] dsMemory is the memory where this signal has to be allocated.
     * @return false if the type name is not recognized or in case of allocation errors, true otherwise.
     */
    virtual bool Allocate(MemoryArea &dsMemory)=0;

    /**
     * @brief The routine to be executed at the begin of a write operation.
     */
    virtual void WriteStart()=0;

    /**
     * @brief The routine to be executed at the begin of a read operation.
     */
    virtual void ReadStart()=0;

    /**
     * @brief The routine to be executed at the end of a write operation
     */
    virtual void WriteEnd()=0;

    /**
     * @brief The routine to be executed at the end of a read operation
     */
    virtual void ReadEnd()=0;

    /**
     * @brief The synchronising routine.
     * @details If a GAMSignalI linked to this signal specifies a number of cycles
     * different than 0, the read (write) operation will be synchronised and this function
     * will be called from the broker (DataSourceBrokerI) before the read (write) operation.
     * @return false in case of errors or timeout expire. True otherwise.
     */
    virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait)=0;

    /**
     * @brief Retrieves the reader for the signal passed in input.
     * @details Generally a data source can supports a certain number of interfaces to interact with GAMs.
     * This function retrieves the first input reader compatible with this data source and with the GAM
     * signal in input, or an invalid reference if the two are incompatible. The GAM signal \a defIn in input
     * will be added to eventually generated input reader.
     * @param[in] defIn is a GAMSignalI to be linked to this signal.
     * @param[in] varPtr is the pointer of the GAMSignalI memory (if NULL the broker will allocate the memory).
     * @return a reference to a reader compatible with \a defIn, an invalid reference in case of incompatibility.
     */
    virtual Reference GetInputReader(Reference defIn,
                                     void * varPtr = NULL_PTR(void*))=0;

    /**
     * @brief Retrieves the writer for the signal passed in input.
     * @details Generally a data source can supports a certain number of interfaces to interact with GAMs.
     * This function retrieves the first output writer compatible with this data source and with the GAM
     * signal in input, or an invalid reference if the two are incompatible. The GAM signal \a defIn in input
     * will be added to eventually generated output writer.
     * @param[in] defIn is a GAMSignalI to be linked to this signal.
     * @param[in] varPtr is the pointer of the GAMSignalI memory (if NULL the broker will allocate the memory).
     * @return a reference to a writer compatible with \a defIn, an invalid reference in case of incompatibility.
     */
    virtual Reference GetOutputWriter(Reference defIn,
                                      void * varPtr = NULL_PTR(void*))=0;

    /**
     * @brief Configures the signal from a GAMSignalI definition.
     * @details If the signal is not fully configured, this function completes the definition
     * reading the fields of a GAMSignalI linked to this signal passed in input.
     * @param[in] gamSignalIn is a GAMSignalI to be linked to this signal.
     * @return false if the current configuration is not compatible with the \a gamSignalIn one.
     */
    virtual bool Configure(Reference gamSignalIn)=0;

    /**
     * @brief Retrieves the pointer to the pointer of the signal memory buffer to be used
     * within the specified index.
     * @param[in] bufferIndex is the index of the signal memory buffer current in use.
     * @pre
     *   bufferIndex == 0 ||
     *   bufferIndex == 1;
     */
    virtual void **GetDataSourcePointer(uint8 bufferIndex)=0;

    /**
     * @brief Checks if the broker in input is compatible with this signal.
     * @param[in] testBroker is the broker to be checked.
     * @return true if \a testBroker is supported by this signal, false otherwise.
     */
    virtual bool IsSupportedBroker(DataSourceBrokerI &testBroker)=0;

protected:

    /**
     * The pointer to the application.
     */
    RealTimeApplication *application;

    /**
     * The variable type
     */
    StreamString type;

    /**
     * The signal number of elements
     */
    uint32 numberOfElements;

    /**
     * The number of samples for each cycle
     */
    uint32 numberOfSamples;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCESIGNALI_H_ */

