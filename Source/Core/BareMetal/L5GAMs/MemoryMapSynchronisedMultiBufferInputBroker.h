/**
 * @file MemoryMapSynchronisedMultiBufferInputBroker.h
 * @brief Header file for class MemoryMapSynchronisedMultiBufferInputBroker
 * @date 12/04/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class MemoryMapSynchronisedMultiBufferInputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSYNCHRONISEDMULTIBUFFERINPUTBROKER_H_
#define MEMORYMAPSYNCHRONISEDMULTIBUFFERINPUTBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryMapMultiBufferBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Synchronised input implementation of the MemoryMapMultiBufferBroker.
 * @details Calls Synchronise on the DataSource and subsequently calls MemoryMapMultiBufferInputBroker::CopyInputs.
 *
 * Note that the DataSource::PrepareOffsets is not called.
 *
 */
class MemoryMapSynchronisedMultiBufferInputBroker: public MemoryMapMultiBufferBroker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    MemoryMapSynchronisedMultiBufferInputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapSynchronisedMultiBufferInputBroker();

    /**
     * @brief Calls DataSourceI::Synchronise and then MemoryMapMultiBufferBroker::CopyInputs.
     * @return the return value of MemoryMapMultiBufferBroker::CopyInputs if DataSourceI::Synchronise is successful.
     * @pre
     *     Init()
     */
    virtual bool Execute();

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYMAPSYNCHRONISEDMULTIBUFFERINPUTBROKER_H_ */

