/**
 * @file DataSourceSignal.h
 * @brief Header file for class DataSourceSignal
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

#ifndef DATASOURCESIGNAL_H_
#define DATASOURCESIGNAL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalI.h"
#include "ReferenceT.h"
#include "RealTimeStateInfo.h"
#include "RealTimeApplication.h"
#include "MemoryArea.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief DataSourceSignalI implementation for the exchange of signals between GAM components.
 *
 * @details This implementation of the DataSourceSignalI interface is intend for interchange
 *  of signals in real-time between GAM components. The DataSourceSignal instances are managed by a
 *  single DataSource which offers the memory backend to store the signal data.
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
class DLL_API DataSourceSignal: public DataSourceSignalI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    DataSourceSignal();

    /**
     * @brief Destructor
     */
    virtual ~DataSourceSignal();

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
     * @details If this DataSourceSignal was already being used in this state and is to be used in the next state
     *  then
     *
     * is not used during the current state but will be used in the next, it will be
     * reset to its default value. The value of the signal will be preserved for the next state if it is used in the
     * current one. Since this function will be executed in a low priority thread in parallel with the
     * real-time execution, the operations on the data source memory will be executed on a currently unused buffer (double
     * buffer implementation) which will be available just after the state switch as the new data source to be used.
     */
    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    /**
     * @see DataSourceSignalI::Allocate(*)
     */
    virtual bool Allocate(MemoryArea &dsMemory);

    /**
     * @see DataSourceSignalI::WriteStart(*)
     */
    virtual void WriteStart();

    /**
     * @see DataSourceSignalI::ReadStart(*)
     */
    virtual void ReadStart();

    /**
     * @see DataSourceSignalI::WriteEnd(*)
     */
    virtual void WriteEnd();

    /**
     * @see DataSourceSignalI::ReadEnd(*)
     */
    virtual void ReadEnd();

    /**
     * @see ReferenceContainer::Initialise(*).
     * @details The following fields can be specified:\n
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
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @see Object::ToStructuredData(*)
     * @details The following leafs will be created in \a data:
     *
     *   - Type = "the variable type" (if != "")\n
     *   - Default = "the variable default value" (if != "")\n
     *   - Dimensions = "the variable dimensions" (if != "")\n
     *   - Samples = "how many samples will be produced for each cycle" (if != 1)\n
     */
    virtual bool ToStructuredData(StructuredDataI & data);

    /**
     * @see DataSourceSignalI::WaitOnEvent(*)
     */
    virtual bool WaitOnEvent(const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @see DataSourceSignalI::Configure(*)
     * @details The function returns false if this signal is already initialised with a different
     * type name and a different default value than the one specified in \a gamSignalIn. The function
     * will refresh the number of elements to be allocated if \a gamSignalIn specifies a greater
     * size to be read / write.
     */
    virtual bool Configure(Reference gamSignalIn);

    /**
     * @see DataSourceSignalI::GetInputReader(*).
     * @return a valid ReferenceT<MemoryMapInputReader> if signalIn is compatible with MemoryMapOutputWriter
     *  (i.e. if MemoryMapInputReader::AddSignal(signalOut) == true).
     */
    virtual ReferenceT<DataSourceBrokerI> GetInputReader(ReferenceT<GAMSignalI> signalIn,
                                                         void * varPtr = NULL_PTR(void*));

    /**
     * @see DataSourceSignalI::GetOutputWriter(*).
     * @return a valid ReferenceT<MemoryMapOutputWriter> if signalOut is compatible with MemoryMapOutputWriter
     *  (i.e. if MemoryMapOutputWriter::AddSignal(signalOut) == true).
     */
    virtual ReferenceT<DataSourceBrokerI> GetOutputWriter(ReferenceT<GAMSignalI> signalOut,
                                                          void * varPtr = NULL_PTR(void*));

    /**
     * @see DataSourceSignalI::IsSupportedBroker(*)
     * @details returns true if \a testBroker inherits from MemoryMapDataSourceBroker.
     */
    virtual bool IsSupportedBroker(DataSourceBrokerI &testBroker);

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

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCESIGNAL_H_ */

