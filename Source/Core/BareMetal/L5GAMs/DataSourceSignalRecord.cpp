/**
 * @file DataSourceSignalRecord.cpp
 * @brief Source file for class DataSourceSignalRecord
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

 * @details This source file contains the definition of all the methods for
 * the class DataSourceSignalRecord (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalRecord.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceSignalRecord::DataSourceSignalRecord(): ReferenceContainer() {

    // can be explored
    ReferenceT<ReferenceContainer> prod(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    prod->SetName("Producers");
    if (prod.IsValid()) {
        if (Insert(prod)) {
            producers = prod;
        }
    }

    ReferenceT<ReferenceContainer> cons(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    cons->SetName("Consumers");
    if (cons.IsValid()) {
        if (Insert(cons)) {
            consumers = cons;
        }
    }
}

bool DataSourceSignalRecord::AddConsumer(ReferenceT<GAM> gamConsumer) {

    bool ret = consumers.IsValid();
    if (ret) {
        ret = consumers->Insert(gamConsumer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Consumers container not found or invalid");
    }
    return ret;
}

bool DataSourceSignalRecord::AddProducer(ReferenceT<GAM> gamProducer) {
    bool ret = producers.IsValid();
    if (ret) {
        ret = producers->Insert(gamProducer);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Producers container not found or invalid");
    }
    return ret;
}

uint32 DataSourceSignalRecord::GetNumberOfConsumers() {
    return consumers->Size();
}

uint32 DataSourceSignalRecord::GetNumberOfProducers() {
    return producers->Size();
}

ReferenceT<ReferenceContainer> DataSourceSignalRecord::GetConsumers() const {
    return consumers;
}

ReferenceT<ReferenceContainer> DataSourceSignalRecord::GetProducers() const {
    return producers;
}

CLASS_REGISTER(DataSourceSignalRecord, "1.0")

}
