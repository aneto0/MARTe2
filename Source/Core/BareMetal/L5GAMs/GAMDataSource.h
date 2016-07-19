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
#include "RealTimeApplication.h"
#include "RealTimeStateInfo.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief DataSourceSignalI implementation for the exchange of signals between GAM components.
 *
 * @details This implementation of the DataSourceSignalI interface is intend for the interchange
 *  of signals in real-time between GAM components. The DataSourceSignal instances are managed by a
 *  single DataSource which offers the memory back-end to store the signal data. It is implemented
 *  with a dual buffering mechanism which allows to load a default value when changing to a new state
 *  (if this signal was not being used in the current state).
 *
 * @details The syntax in the input configuration stream (see Initialise) has to be:
 *
 * +DataSourceSignal_Name= {\n
 *    Class = DataSourceSignal
 *    Type = "the variable type" (default "")\n
 *    Default = "the variable default value" (default "")\n
 *    Dimensions = "the variable dimensions" (default "")\n
 *    Samples = "how many samples will be produced for each cycle" (default 1)\n
 *     ...\n
 * }\n
 *
 * and it has to be contained inside a [DataSource] declaration.
 */
class DLL_API GAMDataSource: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    GAMDataSource();

    /**
     * @brief Destructor
     */
    virtual ~GAMDataSource();

    virtual uint32 GetCurrentBufferIndex();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx, uint32 bufferIdx, void *&signalAddress);

    virtual bool AllocateMemory();

    virtual ReferenceContainer GetInputReaders(const char8 * const functionName);

    virtual ReferenceContainer GetOutputWriters(const char8 * const functionName);

    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    virtual bool ChangeState();

#if 0
    /**
     * @brief Retrieves to the memory address containing the signal data.
     * @details Given that the DataSourceSignal uses a double-buffering memory, this method
     * will return to the memory buffer for \a bufferIndex.
     * @param[in] bufferIndex index of the buffer.
     * @return the memory address containing the signal data for \a bufferIndex.
     */
    virtual void **GetDataSourcePointer(uint8 bufferIndex);

    /**
     * @brief Prepare the DataSourceSignal for the next state.
     * @details If this DataSourceSignal was being used in this state and is also going to be used in the next state
     *  then the same data buffer will be used to store the signal data, i.e. GetDataSourcePointer(0) == GetDataSourcePointer(1)
     * If this DataSourceSignal was not being used in this state and is going to be used in the next state and it has no default value
     *  then the same data buffer will be used to store the signal data, i.e. GetDataSourcePointer(0) == GetDataSourcePointer(1)
     * If this DataSourceSignal was not being used in this state and is going to be used in the next state and it has a default value
     *  then a new data buffer will be used to store the signal data, i.e. GetDataSourcePointer(0) != GetDataSourcePointer(1) and it will
     *  be pre-loaded with the specified default value.
     * @param[in] status runtime information about the current and next state.
     * @return true if the memory is the data buffer to store the signal can be successfully set and, if a default value is specified,
     *  it can be successfully pre-load.
     */
    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    /**
     * @brief Sets up the dual buffering mechanism allocating memory to store a signal in each of the buffers.
     * @param[in] dsMemory the MemoryArea where the buffers are to be stored.
     * @return true if the memory can be successfully allocated.
     */
    virtual bool Allocate(MemoryArea &dsMemory);

    /**
     * @brief Configures the DataSourceSignal from StructuredDataI input.
     * @details When configuring the DataSource any of the fields defined here will be later merged
     * with other definitions of the same signal (e.g. generated by a GAM) and any inconsistencies reported (see Configure).
     *
     * The following fields may be specified:\n
     *
     *   - Type = "the variable type" (default "")\n
     *   - Default = "the variable default value" (default "")\n
     *   - Dimensions = "the variable dimensions" (default "")\n
     *   - Samples = "how many samples will be produced for each cycle" (default 1)\n
     *
     * The Dimensions parameter follows the same format of the modifiers string in IntrospectionEntry. In particular in this
     * case Dimensions = "[n]" denotes an array with n elements, Dimensions = "[n][m]" denotes a matrix with n rows and m columns.
     * The function GetNumberOfElements(*) will return n*m.
     * See IntrospectionEntry::GetModifiers() for more documentation.
     * @return true if at least one of the optional fields is specified.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @see Object::ExportData(*)
     * @details The following leafs will be created in \a data:
     *
     *   - Type = "the variable type" (if != "")\n
     *   - Default = "the variable default value" (if != "")\n
     *   - Dimensions = "the variable dimensions" (if != "")\n
     *   - Samples = "how many samples will be produced for each cycle" (if != 1)\n
     *  @return true if the data can be successfully exported.
     */
    virtual bool ExportData(StructuredDataI & data);

    /**
     * @brief NOOP. @see DataSourceSignalI::WaitOnEvent(*)
     * @return true
     */
    virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @brief Configures this DataSourceSignal against the parameters provided by the \a gamSignalIn.
     * @details Sets the signal type, default value and dimension against the parameters provided by the \a gamSignalIn.
     * @param[in] gamSignalIn the GAMSignalI where to retrieve the information from.
     * @return false if this signal was already initialised with a different
     * type name or a different default value than the one specified in \a gamSignalIn. The function
     * will refresh the number of elements to be allocated if \a gamSignalIn specifies a greater
     * size to be read / write.
     */
    virtual bool Configure(ReferenceT<GAMSignalI> gamSignalIn);

    /**
     * @see DataSourceSignalI::GetInputReader(*).
     * @return a valid ReferenceT<MemoryMapInputReader> if signalIn is compatible with MemoryMapInputReader
     *  (i.e. if MemoryMapInputReader::AddSignal(signalOut) == true).
     */
    virtual ReferenceT<BrokerI> GetInputReader(ReferenceT<GAMSignalI> signalIn,
                                                         void * varPtr = NULL_PTR(void*));

    /**
     * @see DataSourceSignalI::GetOutputWriter(*).
     * @return a valid ReferenceT<MemoryMapOutputWriter> if signalOut is compatible with MemoryMapOutputWriter
     *  (i.e. if MemoryMapOutputWriter::AddSignal(signalOut) == true).
     */
    virtual ReferenceT<BrokerI> GetOutputWriter(ReferenceT<GAMSignalI> signalOut,
                                                          void * varPtr = NULL_PTR(void*));

    /**
     * @see DataSourceSignalI::IsSupportedBroker(*)
     * @details returns true if \a testBroker inherits from MemoryMapDataSourceBroker.
     */
    virtual bool IsSupportedBroker(BrokerI &testBroker);

protected:

    /**
     * The variable default value
     */
    StreamString defaultValue;

    /**
     * The variable dimensions
     */
    StreamString dimensions;

    /**
     * The positions of the buffers related to this variable in the
     * global memory area
     */
    uint32 bufferPtrOffset[2];

    /**
     * Pointers to the buffer to be used.
     */
    void *usedBuffer[2];

    /**
     * A pointer to the global memory area.
     */
    MemoryArea *memory;
#endif

    void **signalMemory[2];

    uint32 currentBufferIndex;

    HeapI *heap;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMDATASOURCE_H_ */

