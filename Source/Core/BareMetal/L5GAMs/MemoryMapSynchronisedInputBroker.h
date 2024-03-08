/**
 * @file MemoryMapSynchronisedInputBroker.h
 * @brief Header file for class MemoryMapSynchronisedInputBroker
 * @date 18/07/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class MemoryMapSynchronisedInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */
#ifndef MEMORYMAPISYNCHRONISEDNPUTBROKER_H_
#define MEMORYMAPISYNCHRONISEDNPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapInputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Synchronous input MemoryMapBroker implementation.
 * @details This class calls Synchronise on the DataSourceI and copies all
 * the signals declared on this MemoryMapBroker (from the DataSourceI memory to the GAM memory).
 */
class MemoryMapSynchronisedInputBroker: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    MemoryMapSynchronisedInputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapSynchronisedInputBroker();

    /**
     * @brief Calls Synchronise on the DataSourceI and sequentially copies all the
     * signals from the DataSourceI memory to the GAM  memory.
     * @return true if the synchronisation call is successful and if all copies are successfully performed.
     */
    virtual bool Execute();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPISYNCHRONISEDNPUTBROKER_H_ */

