/**
 * @file DataSourceSignalI.cpp
 * @brief Source file for class DataSourceSignalI
 * @date 18/apr/2016
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
 * the class DataSourceSignalI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DataSourceSignalI.h"
#include "ReferenceT.h"
#include "DataSourceSignalRecord.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "AdvancedErrorManagement.h"
#include "GAM.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceSignalI::DataSourceSignalI() :
        ReferenceContainer() {
    application = NULL_PTR(RealTimeApplication *);
    numberOfSamples = 1u;
    numberOfElements = 1u;
}

DataSourceSignalI::~DataSourceSignalI() {

}

void DataSourceSignalI::SetApplication(RealTimeApplication &app) {
    application = &app;
}

bool DataSourceSignalI::AddConsumer(ReferenceT<GAM> gam) {
    uint32 index;
    bool ret = gam.IsValid();
    if (ret) {
        uint32 numberOfGAMStates = gam->GetNumberOfSupportedStates();
        StreamString *stateNames = gam->GetSupportedStates();
        for (uint32 k = 0u; (k < numberOfGAMStates) && (ret); k++) {
            ReferenceT<DataSourceSignalRecord> record;
            uint32 numberOfStates = Size();
            bool found = false;

            for (index = 0u; (index < numberOfStates) && (!found); index++) {
                record = Get(index);
                if (record.IsValid()) {
                    if (stateNames[k] == record->GetName()) {
                        found = true;
                    }
                }
            }
            if (found) {
                ret = record->AddConsumer(gam);
            }
            else {
                record = ReferenceT<DataSourceSignalRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                if (record.IsValid()) {
                    record->SetName(stateNames[k].Buffer());
                    ret = record->AddConsumer(gam);
                    if (ret) {
                        ret = Insert(record);
                    }
                }
            }
        }
    }
    return ret;
}

bool DataSourceSignalI::AddProducer(ReferenceT<GAM> gamIn) {

    uint32 index;

    ReferenceT<GAM> gam = gamIn;
    bool ret = gam.IsValid();

    if (ret) {
        uint32 numberOfGAMStates = gam->GetNumberOfSupportedStates();
        StreamString *stateNames = gam->GetSupportedStates();
        for (uint32 k = 0u; (k < numberOfGAMStates) && (ret); k++) {

            ReferenceT<DataSourceSignalRecord> record;
            uint32 numberOfStates = Size();
            bool found = false;

            for (index = 0u; (index < numberOfStates) && (!found); index++) {
                record = Get(index);
                if (record.IsValid()) {
                    if (stateNames[k] == record->GetName()) {
                        found = true;
                    }
                }
            }
            if (found) {
                ret = record->AddProducer(gam);
            }
            else {
                record = ReferenceT<DataSourceSignalRecord>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
                if (record.IsValid()) {
                    record->SetName(stateNames[k].Buffer());
                    ret = record->AddProducer(gam);
                    if (ret) {
                        ret = Insert(record);
                    }
                }
            }
        }
    }
    return ret;

}

uint32 DataSourceSignalI::GetNumberOfConsumers(const char8 * const stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<DataSourceSignalRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfRecords) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                found = true;
            }
        }
    }

    if (found) {
        ret = record->GetNumberOfConsumers();
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "RealTimeState %s not configured", stateIn)
    }
    return ret;

}

uint32 DataSourceSignalI::GetNumberOfProducers(const char8 * const stateIn) {
    uint32 ret = 0u;
    uint32 numberOfRecords = Size();
    ReferenceT<DataSourceSignalRecord> record;
    bool found = false;
    for (uint32 i = 0u; (i < numberOfRecords) && (!found); i++) {
        record = Get(i);
        if (record.IsValid()) {
            if (StringHelper::Compare(record->GetName(), stateIn) == 0) {
                found = true;
            }
        }
    }
    if (found) {
        ret = record->GetNumberOfProducers();
    }
    else {
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "RealTimeState %s not configured", stateIn)
    }

    return ret;
}

bool DataSourceSignalI::Verify() {

    bool ret = true;
    uint32 numberOfRecords = Size();
    for (uint32 i = 0u; (i < numberOfRecords) && (ret); i++) {
        ReferenceT<DataSourceSignalRecord> record = Get(i);
        if (record.IsValid()) {
            // no more than one producer for each state
            if (record->GetNumberOfProducers() > 1u) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The DataSourceSignal %s has %d producers in the state %s", GetName(),
                                        record->GetNumberOfProducers(), record->GetName())
                ret = false;
            }
            if (record->GetNumberOfConsumers() == 0u) {
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "The DataSourceSignal %s is not consumed in the state %s", GetName(), record->GetName())
            }
        }
    }

    return ret;
}

const char8 *DataSourceSignalI::GetType() {
    return type.Buffer();
}

uint32 DataSourceSignalI::GetNumberOfElements() const {
    return numberOfElements;
}

uint32 DataSourceSignalI::GetNumberOfSamples() const {
    return numberOfSamples;
}

}
