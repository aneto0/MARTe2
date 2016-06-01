/**
 * @file DataSourceSignal.cpp
 * @brief Source file for class DataSourceSignal
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
 * the class DataSourceSignal (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <DataSourceSignal.h>
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

DataSourceSignal::DataSourceSignal() :
        ReferenceContainer() {
    application = NULL_PTR(RealTimeApplication *);
    numberOfSamples = 1u;
    numberOfElements = 1u;
}

DataSourceSignal::~DataSourceSignal() {

}

void DataSourceSignal::SetApplication(RealTimeApplication &app) {
    application = &app;
}

bool DataSourceSignal::AddConsumer(ReferenceT<GAM> gam) {
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

bool DataSourceSignal::AddProducer(ReferenceT<GAM> gamIn) {

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

uint32 DataSourceSignal::GetNumberOfConsumers(const char8 * const stateIn) {
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

uint32 DataSourceSignal::GetNumberOfProducers(const char8 * const stateIn) {
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

bool DataSourceSignal::Verify() {

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

const char8 *DataSourceSignal::GetType() {
    return type.Buffer();
}

uint32 DataSourceSignal::GetNumberOfElements() const {
    return numberOfElements;
}

uint32 DataSourceSignal::GetNumberOfSamples() const {
    return numberOfSamples;
}

bool DataSourceSignal::Initialise(StructuredDataI & data) {

    bool ret = ReferenceContainer::Initialise(data);

    bool atLeastOne = false;
    if (ret) {

        if (data.Read("Type", type)) {
            atLeastOne = true;
        }

        if (data.Read("Default", defaultValue)) {
            atLeastOne = true;
        }

        if (data.Read("Dimensions", dimensions)) {
            // use introspection entry to parse the modifiers
            IntrospectionEntry entry("", "", dimensions.Buffer(), "", 0u, 0u);
            for (uint32 i = 0u; i < 3u; i++) {
                numberOfElements *= entry.GetNumberOfElements(i);
            }
            if (entry.GetMemberPointerLevel() > 0u) {
                REPORT_ERROR(ErrorManagement::Warning, "Pointers not supported. The statement will be ignored");
            }
            atLeastOne = true;
        }

        if (data.Read("Samples", numberOfSamples)) {
            atLeastOne = true;
        }
        ret = atLeastOne;
    }
    return ret;
}

bool DataSourceSignal::ExportData(StructuredDataI& data) {

    const char8 * objName = GetName();
    StreamString objNameToPrint = (IsDomain()) ? ("$") : ("+");
    objNameToPrint += objName;
    bool ret = data.CreateRelative(objNameToPrint.Buffer());
    if (ret) {
        ret = data.Write("Class", "DataSourceSignal");
        if (ret) {
            if (type != "") {
                ret = data.Write("Type", type);
            }
        }

        if (ret) {
            if (defaultValue != "") {
                ret = data.Write("Default", defaultValue);
            }
        }

        if (ret) {
            if (numberOfSamples != 1) {
                ret = data.Write("Samples", numberOfSamples);
            }
        }

        if (ret) {
            if (dimensions != "") {
                ret = data.Write("Dimensions", dimensions);
            }
        }

        if (ret) {
            uint32 numberOfChildren = Size();
            for (uint32 i = 0u; i < numberOfChildren; i++) {
                Reference child = Get(i);
                ret = child.IsValid();
                if (ret) {
                    if (ret) {
                        ret = child->ExportData(data);
                    }
                }
            }
        }
        if (!data.MoveToAncestor(1u)) {
            ret = false;
        }
    }

    return ret;
}

void *DataSourceSignal::GetDataSourcePointer(uint8 bufferIndex) {
    void *ptr = dataSourcePointer0;
    if (bufferIndex > 0) {
        ptr = dataSourcePointer1;
    }
    return ptr;
}

void DataSourceSignal::SetDataSourcePointer(void *dataSourcePointer,
                                            uint8 bufferIndex) {
    if (bufferIndex == 0) {
        dataSourcePointer0 = dataSourcePointer;
    }
    else {
        dataSourcePointer1 = dataSourcePointer;
    }
}

}
