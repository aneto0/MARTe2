/**
 * @file DataSourceSignalRecord.h
 * @brief Header file for class DataSourceSignalRecord
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

 * @details This header file contains the declaration of the class DataSourceSignalRecord
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCESIGNALRECORD_H_
#define DATASOURCESIGNALRECORD_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Contains the producer and consumer GAMs of a specific variable in
 * a specific RealTimeState state.
 */
class DLL_API DataSourceSignalRecord: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    DataSourceSignalRecord();

    /**
     * @brief Inserts a GAM as a consumer.
     * @param[in] gamConsumer is the GAM consumer to be inserted.
     * @return false if \a gamConsumer is correctly inserted in the GAM consumers container.
     */
    bool AddConsumer(ReferenceT<GAM> gamConsumer);

    /**
     * @brief Inserts a GAM as a producer.
     * @param[in] gamProducer is the GAM producer to be inserted.
     * @return false if \a gamProducer is correctly inserted in the GAM producers container.
     */
    bool AddProducer(ReferenceT<GAM> gamProducer);

    /**
     * @brief Retrieves the number of GAM consumers inserted.
     * @return The number of GAM consumers inserted
     */
    uint32 GetNumberOfConsumers();

    /**
     * @brief Retrieves the number of GAM producers inserted.
     * @return The number of GAM producers inserted
     */
    uint32 GetNumberOfProducers();

    /**
     * @brief Retrieves the consumers container.
     * @return The consumers container.
     */
    ReferenceT<ReferenceContainer> GetConsumers() const;

    /**
     * @brief Retrieves the producers container.
     * @return The producers container.
     */
    ReferenceT<ReferenceContainer> GetProducers() const;

private:

    /**
     * Accelerator to the producers container.
     */
    ReferenceT<ReferenceContainer> producers;

    /**
     * Accelerator to the consumers container.
     */
    ReferenceT<ReferenceContainer> consumers;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCESIGNALRECORD_H_ */

