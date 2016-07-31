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

struct CopyTableEntry {
    void *gamPointer;
    void **dataSourcePointer[2];
    uint32 dataSourceOffset;
    uint32 copySize;
};

/**
 * @brief Memory mapped Broker implementation.
 * @details The configuration of this element has to be performed after the
 * RealTimeApplication::ConfigureDataSource() step. It allows to read (write) data blocks
 * of each element and to read (write) blocks of samples. It supports both single and dual buffering mechanisms
 *  for the interconnection between the DataSourceSignalI signals and the GAMSignalI signals.
 */
class DLL_API MemoryMapStatefulBroker: public BrokerI {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetData(*) == NULL &&
     *   GetMemoryPointer(*) == NULL &&
     *   GetNumberOfSignals() == 0 &&
     *   IsSync() == false
     */
    MemoryMapStatefulBroker();

    /**
     * @brief Destructor
     */
    virtual ~MemoryMapStatefulBroker();

    /**
     *
     */
    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void *gamMemoryAddress);


protected:

    CopyTableEntry *copyTable;

    uint32 numberOfDataSourceSignalBuffers;

    DataSourceI* dataSource;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPSTATEFULBROKER_H_ */

