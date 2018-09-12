/**
 * @file MemoryMapSynchronisedMultiBufferOutputBroker.h
 * @brief Header file for class MemoryMapSynchronisedMultiBufferOutputBroker
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

 * @details This header file contains the declaration of the class MemoryMapSynchronisedMultiBufferOutputBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKER_H_
#define MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKER_H_

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
 * @brief Synchronised output implementation of the MemoryMapMultiBufferBroker.
 * @details Calls MemoryMapMultiBufferInputBroker::CopyOutputs and subsequently calls Synchronise on the DataSource.
 *
 * Note that the DataSource::PrepareOffsets is not called.
 *
 */
class MemoryMapSynchronisedMultiBufferOutputBroker: public MemoryMapMultiBufferBroker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    MemoryMapSynchronisedMultiBufferOutputBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~MemoryMapSynchronisedMultiBufferOutputBroker();

    /**
     * @brief Calls DataSourceI::Synchronise and then MemoryMapMultiBufferBroker::CopyOutputs.
     * @return the return value of DataSourceI::Synchronise if MemoryMapMultiBufferBroker::CopyOutputs is successful.
     */
    virtual bool Execute();

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_MEMORYMAPSYNCHRONISEDMULTIBUFFEROUTPUTBROKER_H_ */

