/**
 * @file MemoryMapBroker.h
 * @brief Header file for class MemoryMapBroker
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

 * @details This header file contains the declaration of the class MemoryMapBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPBROKER_H_
#define MEMORYMAPBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BrokerI.h"
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Helper structure which holds the memory pointers of the GAM and DataSource elements
 * that are to be copied by this MemoryMapBroker
 */
struct MemoryMapBrokerCopyTableEntry {
    /**
     * The pointer to the GAM.
     */
    void *gamPointer;
    /**
     * The pointer to the datasource
     */
    void *dataSourcePointer;
    /**
     * The size of the copy
     */
    uint32 copySize;
    /**
     * The signal type
     */
    TypeDescriptor type;
};

/**
 * @brief Memory mapped BrokerI implementation.
 * @details This class knows how to copy from/to a DataSourceI memory address to/from a GAM signal memory address.
 * For each GAM signal, the signal name is searched in the provided DataSourceI (see Init) and the memory
 * memory address of the signal retrieved using the GetSignalMemoryBuffer function. The information of each element to
 *  be copied is stored in a MemoryMapBrokerCopyTableEntry.
 */
class DLL_API MemoryMapBroker: public BrokerI {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetNumberOfCopies() == 0
     */
    MemoryMapBroker();

    /**
     * @brief Destructor. Frees the created MemoryMapBrokerCopyTableEntry entries.
     */
    virtual ~MemoryMapBroker();

    /**
     * @brief Initialises the MemoryMapBroker.
     * @details For each signal in the \a functionName, which wishes to use this MemoryMapBroker instance
     * (i.e. IsSupportedBroker(class inhering from MemoryMapBroker) == true), the signal name is searched
     *  in the provided \a dataSourceIn (see Init) and the memory address of the signal retrieved using the
     *  GetSignalMemoryBuffer function. The information of each element to  be copied is stored in a MemoryMapBrokerCopyTableEntry.
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSourceIn the DataSourceI to be queried.
     * @param[in] functionName the name of GAM the to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored)
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     * @post
     *   GetNumberOfCopies() > 0
     */
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *const gamMemoryAddress);

    /**
     * @brief Initialises the MemoryMapBroker.
     * @details For each signal in the \a functionName, which wishes to use this MemoryMapBroker instance
     * (i.e. IsSupportedBroker(class inhering from MemoryMapBroker) == true), the signal name is searched
     *  in the provided \a dataSourceIn (see Init) and the memory address of the signal retrieved using the
     *  GetSignalMemoryBuffer function. The information of each element to  be copied is stored in a MemoryMapBrokerCopyTableEntry.
     * @param[in] direction the signal direction (InputSignals or OutputSignals).
     * @param[in] dataSourceIn the DataSourceI to be queried.
     * @param[in] functionName the name of GAM the to which this BrokerI is being allocated to.
     * @param[in] gamMemoryAddress the base address of the GAM memory (where signal data is stored)
     * @param[in] optim if true the algorithm tries to optimise the number of copies by means of grouping consecutive memory..
     * @return true if all the copy information related to \a functionName can be successfully retrieved.
     * @post
     *   GetNumberOfCopies() > 0
     */
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *const gamMemoryAddress,
                      const bool optim);

protected:

    /**
     * A table with all the elements to be copied
     */
    MemoryMapBrokerCopyTableEntry *copyTable;

    /**
     * The DataSourceI instance
     */
    DataSourceI *dataSource;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPBROKER_H_ */

