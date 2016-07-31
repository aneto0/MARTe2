/**
 * @file MemoryMapDataSourceBroker.h
 * @brief Header file for class MemoryMapDataSourceBroker
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

 * @details This header file contains the declaration of the class MemoryMapDataSourceBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSTATEFULBROKER_H_
#define MEMORYMAPSTATEFULBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BrokerI.h"
#include "MemoryArea.h"
#include "ReferenceT.h"
#include "RealTimeApplication.h"
#include "FastPollingEventSem.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Helper structure which holds the memory pointers of the GAM and DataSource elements
 * that are to be copied by this MemoryMapStatefulBroker. Note that the dataSourcePointer will
 * hold the address of variable which has the pointer to the memory (and not the memory address itself).
 */
struct MemoryMapStatefulBrokerCopyTableEntry {
    void *gamPointer;
    void **dataSourcePointer[2];
    uint32 dataSourceOffset;
    uint32 copySize;
};

/**
 * @brief Stateful memory mapped BrokerI implementation.
 * @details This class knows how to copy from/to a DataSourceI memory address to/from a GAM signal memory address,
 *  provided that the associated DataSourceI supports a dual-buffer mechanism.
 * For each GAM signal, the signal name is searched in the provided DataSourceI (see Init) and, for each of the two buffers,
 *  the memory address of the variable containing the address of the signal is retrieved using the GetSignalMemoryBuffer function.
 *  The idea is that the DataSource can change this variable value (and thus the place to where it is pointing) and as a consequence
 *  the Broker will be able to read/write from this different location with minimum penalty on performance.
 *  The information of each element to be copied is stored in a MemoryMapStatefulBrokerCopyTableEntry.
 */
class DLL_API MemoryMapStatefulBroker: public BrokerI {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetNumberOfCopies() == 0
     */
    MemoryMapStatefulBroker();

    /**
     * @brief Destructor. Frees the created MemoryMapStatefulBrokerCopyTableEntry entries.
     */
    virtual ~MemoryMapStatefulBroker();

    /**
     * @brief Initialises the MemoryMapStatefulBroker.
     * @detail For each signal in the \a functionName, which wishes to use this MemoryMapBroker instance
     * (i.e. IsSupportedBroker(class inhering from MemoryMapBroker) == true), the signal name is searched
     *  in the provided \a dataSourceIn (see Init) and, for each of the two buffers, the memory address of the variable holding
     *  the signal memory pointer address retrieved using the GetSignalMemoryBuffer function.
     *  The information of each element to  be copied is stored in a MemoryMapStatefulBrokerCopyTableEntry.
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSource the DataSourceI to be queried.
     * @param[in] functionName the name of GAM the to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored).
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     */
    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void *gamMemoryAddress);

protected:

    /**
     * A table with all the elements to be copied
     */
    MemoryMapStatefulBrokerCopyTableEntry *copyTable;

    /**
     * The DataSourceI instance
     */
    DataSourceI* dataSource;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPSTATEFULBROKER_H_ */

