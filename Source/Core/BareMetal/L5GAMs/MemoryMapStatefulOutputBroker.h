/**
 * @file MemoryMapStatefulOutputBroker.h
 * @brief Header file for class MemoryMapStatefulOutputBroker
 * @date Jul 18, 2016
 * @author aneto
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

 * @details This header file contains the declaration of the class MemoryMapStatefulOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSTATEFULOUTPUTBROKER_H_
#define MEMORYMAPSTATEFULOUTPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryMapStatefulBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Output MemoryMapStatefulBroker implementation.
 * @details This class copies all the signals declared on a MemoryMapStatefulBroker
 * from the GAM memory to the DataSourceI memory.
 */
class DLL_API MemoryMapStatefulOutputBroker: public MemoryMapStatefulBroker {
public:
CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    MemoryMapStatefulOutputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapStatefulOutputBroker();

    /**
     * @brief Sequentially copies all the signals from the GAM memory to the DataSourceI memory buffer[state].
     * This state is defined by the RealTimeApplication::index (which can be either 0/1).
     * @return true if all copies are successfully performed.
     */
    virtual bool Execute();
};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPSTATEFULOUTPUTBROKER_H_ */
	
