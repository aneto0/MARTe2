/**
 * @file BrokerContainer.cpp
 * @brief Source file for class BrokerContainer
 * @date 11/apr/2016
 * @author pc
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
 * the class BrokerContainer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BrokerContainer.h"
#include "GAMSignalI.h"
#include "DataSourceSignalI.h"
#include "AdvancedErrorManagement.h"
#include "GAMGenericSignal.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static const uint32 brokersGranularity = 2u;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BrokerContainer::BrokerContainer() {
    brokers = NULL_PTR(ReferenceT<DataSourceBrokerI> *);
    numberOfBrokers = 0u;
    synchronized = false;
    //reader by default
    isInput = true;
    application = NULL_PTR(RealTimeApplication *);
    numberOfSignals = 0u;
}

BrokerContainer::~BrokerContainer() {
    if (brokers != NULL) {
        delete[] brokers;
    }
    numberOfBrokers = 0u;
}

bool BrokerContainer::AddSignal(Reference defIn,
                                void * const ptr) {

    ReferenceT<GAMSignalI> def = defIn;

    bool ret = def.IsValid();
    if (ret) {

        ret = false;
        for (uint32 i = 0u; (i < numberOfBrokers) && (!ret); i++) {
            if (brokers[i].IsValid()) {
                // try to add the variable only to the broker belonging
                // to the data source of the def is related to.
                uint32 lastSignalIndex = brokers[i]->GetNumberOfSignals();
                ret = (brokers[i]->AddSignal(def, ptr));
                if (ret) {
                    ret = containerIndexer.Add(i);
                }
                if (ret) {
                    ret = containerSignalIndexer.Add(lastSignalIndex);
                }
                if (ret) {
                    numberOfSignals++;
                }
            }
        }

        if (!ret) {
            uint32 numberOfSubSignals = def->Size();
            StreamString path = "Data.";
            ReferenceT<GAMGenericSignal> testDef = def;
            ret = true;

            // take one of the data source signals of the children
            // and call on it the get broker passing the full definition
            while ((numberOfSubSignals != 0) && (ret)) {
                // consider the GAMGenericSignal
                // if this is false we have a not generic signal with children (not allowed)
                ret = testDef.IsValid();
                if (ret) {
                    ReferenceT<GAMGenericSignal> subDef = testDef->Get(0);
                    ret = subDef.IsValid();
                    if (ret) {
                        numberOfSubSignals = subDef->Size();
                        testDef = subDef;
                    }
                }
            }

            if (ret) {
                path += (testDef.IsValid()) ? (testDef->GetPath()) : (def->GetPath());
                ret = (application != NULL);
                if (ret) {
                    ReferenceT<DataSourceSignalI> ds = application->Find(path.Buffer());
                    ret = ds.IsValid();
                    if (ret) {
                        // get the new reader
                        // it will insert the variable
                        ReferenceT<DataSourceBrokerI> newBroker;
                        if (isInput) {
                            newBroker = ds->GetInputReader(def, ptr);
                        }
                        else {
                            newBroker = ds->GetOutputWriter(def, ptr);
                        }
                        ret = newBroker.IsValid();
                        if (ret) {
                            ret = InsertNewBroker(newBroker);
                        }
                        else {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "Invalid broker returned from the data source signal %s for the GAM signal %s",
                                                    ds->GetName(), def->GetName())
                        }
                    }
                }
            }
            else {

            }
        }
    }

    return ret;

}

void *BrokerContainer::GetSignal(const uint32 n) {
    void* ret = NULL_PTR(void*);

    uint32 brokerIndex = 0u;
    if (containerIndexer.Peek(n, brokerIndex)) {
        uint32 pos = 0u;
        if (containerSignalIndexer.Peek(n, pos)) {
            ret = brokers[brokerIndex]->GetSignal(pos);
        }
    }

    return ret;
}

void *BrokerContainer::GetSignalByName(const char8 * name,
                                       uint32 &index) {
    void* ret = NULL_PTR(void*);
    uint32 indexInBroker = 0u;
    uint32 foundIndexBroker = 0u;
    for (foundIndexBroker = 0u; (foundIndexBroker < numberOfBrokers); foundIndexBroker++) {
        if (brokers[foundIndexBroker].IsValid()) {
            ret = (brokers[foundIndexBroker]->GetSignalByName(name, indexInBroker));
            if (ret != NULL) {
                break;
            }
        }
    }

// retrieve the index
    if (ret != NULL) {
        for (uint32 j = 0u; (j < numberOfSignals); j++) {
            uint32 brokerIndex = 0u;
            if (containerIndexer.Peek(j, brokerIndex)) {
                uint32 pos = 0u;
                if (containerSignalIndexer.Peek(j, pos)) {
                    if ((brokerIndex == foundIndexBroker) && (pos == indexInBroker)) {
                        index = j;
                        break;
                    }
                }
            }
        }
    }

    return ret;
}

uint32 BrokerContainer::GetSignalNumberOfSamples(const uint32 n) {
    uint23 ret = 0u;
    uint32 brokerIndex = 0u;
    if (containerIndexer.Peek(n, brokerIndex)) {
        uint32 pos = 0u;
        if (containerSignalIndexer.Peek(n, pos)) {
            ret = brokers[brokerIndex]->GetSignalNumberOfSamples(pos);
        }
    }
    return ret;
}

bool BrokerContainer::Finalise() {
    bool ret = true;
    uint32 syncBrokers = 0u;
    for (uint32 k = 0u; (k < numberOfBrokers) && (ret); k++) {
        if (brokers[k].IsValid()) {
            ret = (brokers[k]->Finalise());
            if (brokers[k]->IsSync()) {
                syncBrokers++;
            }
        }
    }
    if (syncBrokers > 1u) {
        ret = false;
        //TODO Error more than one sync broker
    }
    else if (syncBrokers == 1u) {
        synchronized = true;
    }
    else {
        synchronized = false;
    }
    return ret;
}

bool BrokerContainer::IsSync() const {
    return synchronized;
}

bool BrokerContainer::Read(const uint8 activeDataSourceBuffer,
                           const TimeoutType &timeout) {
    bool ret = isInput;
    if (ret) {
        for (uint32 k = 0u; (k < numberOfBrokers) && (ret); k++) {
            if (brokers[k].IsValid()) {
                ret = (brokers[k]->Read(activeDataSourceBuffer, timeout));
            }
        }
    }

    return ret;

}

bool BrokerContainer::Write(const uint8 activeDataSourceBuffer,
                            const TimeoutType &timeout) {
    bool ret = (!isInput);
    if (ret) {
        for (uint32 k = 0u; (k < numberOfBrokers) && (ret); k++) {
            if (brokers[k].IsValid()) {
                ret = (brokers[k]->Write(activeDataSourceBuffer, timeout));
            }
        }
    }

    return ret;
}

bool BrokerContainer::InsertNewBroker(ReferenceT<DataSourceBrokerI> item) {
    bool ret = item.IsValid();
    if (ret) {
        ret = Insert(item);
        if (ret) {
            uint32 brokerIndex = numberOfBrokers;
            if (numberOfBrokers % brokersGranularity == 0u) {
                uint32 newSize = (numberOfBrokers + brokersGranularity);
                ReferenceT<DataSourceBrokerI> *temp = new ReferenceT<DataSourceBrokerI> [newSize];
                for (uint32 i = 0u; i < numberOfBrokers; i++) {
                    temp[i] = brokers[i];
                }
                if (brokers != NULL) {
                    delete[] brokers;
                }
                brokers = temp;
            }

            brokers[numberOfBrokers] = item;
            numberOfBrokers++;

            uint32 brokerNSignals = item->GetNumberOfSignals();

            for (uint32 i = 0u; i < brokerNSignals; i++) {
                ret = containerIndexer.Add(brokerIndex);
                if (ret) {
                    ret = containerSignalIndexer.Add(i);
                }
                if (ret) {
                    numberOfSignals++;
                }
            }

        }
    }

    return ret;
}

void BrokerContainer::SetAsInput(bool isInputIn) {
    isInput = isInputIn;
}

void BrokerContainer::SetApplication(RealTimeApplication &app) {
    application = &app;
    for (uint32 i = 0u; i < numberOfBrokers; i++) {
        brokers[i]->SetApplication(app);
    }
}

uint32 BrokerContainer::GetNumberOfSignals() {
    return numberOfSignals;
}

CLASS_REGISTER(BrokerContainer, "1.0")

}
