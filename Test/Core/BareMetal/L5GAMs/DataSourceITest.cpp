/**
 * @file DataSourceITest.cpp
 * @brief Source file for class DataSourceI
 * @date 24/07/2016
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

 * @details This source file contains the definition of all the methods for
 * the class DataSourceI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "DataSourceITest.h"
#include "GAMSchedulerI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Helper class for the broker tests
 */
class DataSourceITestBrokerRef: public Object {
public:
    CLASS_REGISTER_DECLARATION()

ReferenceContainer    brokers;
    StreamString gamName;
};
CLASS_REGISTER(DataSourceITestBrokerRef, "1.0")

class DataSourceITestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceITestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();
    virtual void CustomPrepareNextState();

};

DataSourceITestScheduler1::DataSourceITestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType DataSourceITestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType DataSourceITestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void DataSourceITestScheduler1::CustomPrepareNextState() {

}

CLASS_REGISTER(DataSourceITestScheduler1, "1.0")

class DataSourceITestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceITestGAM1    ();
    virtual bool Setup();
    virtual bool Execute();
};

DataSourceITestGAM1::DataSourceITestGAM1() :
        GAM() {

}

bool DataSourceITestGAM1::Setup() {
    return true;
}

bool DataSourceITestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(DataSourceITestGAM1, "1.0")

class DataSourceITestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceITestHelper    ();

    virtual ~DataSourceITestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr);

    virtual bool Synchronise();

    ReferenceContainer allInputBrokers;
    ReferenceContainer allOutputBrokers;
};

DataSourceITestHelper::DataSourceITestHelper() :
        DataSourceI() {

}

DataSourceITestHelper::~DataSourceITestHelper() {

}

bool DataSourceITestHelper::AllocateMemory() {
    return true;
}

uint32 DataSourceITestHelper::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool DataSourceITestHelper::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    return true;
}

const char8 *DataSourceITestHelper::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    float32 freq = 0;
    bool syncOrTrigger = false;
    data.Read("Frequency", freq);
    syncOrTrigger = (freq > 0);
    const char8 * brokerName = "";

    if (direction == InputSignals) {
        if (syncOrTrigger) {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    if (!syncOrTrigger) {
        uint32 trigger = 0;
        data.Read("Trigger", trigger);
        syncOrTrigger = (trigger > 0);
    }

    if (direction == OutputSignals) {
        if (syncOrTrigger) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    return brokerName;
}

bool DataSourceITestHelper::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool DataSourceITestHelper::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    bool ok = DataSourceI::GetInputBrokers(inputBrokers, functionName, gamMemPtr);
    if (ok) {
        uint32 i;
        ReferenceT<DataSourceITestBrokerRef> brokerRef(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        brokerRef->gamName = functionName;
        for (i = 0; i < inputBrokers.Size(); i++) {
            brokerRef->brokers.Insert(inputBrokers.Get(i));
        }
        allInputBrokers.Insert(brokerRef);
    }
    return ok;
}

bool DataSourceITestHelper::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
    bool ok = DataSourceI::GetOutputBrokers(outputBrokers, functionName, gamMemPtr);
    if (ok) {
        uint32 i;
        ReferenceT<DataSourceITestBrokerRef> brokerRef(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        brokerRef->gamName = functionName;
        for (i = 0; i < outputBrokers.Size(); i++) {
            brokerRef->brokers.Insert(outputBrokers.Get(i));
        }
        allOutputBrokers.Insert(brokerRef);
    }
    return ok;
}

bool DataSourceITestHelper::Synchronise() {
    return false;
}

CLASS_REGISTER(DataSourceITestHelper, "1.0");

static bool InitialiseDataSourceIEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "                   Trigger = 1"
        "                   Frequency = 5.0"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "                   Trigger = 1"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "                   Trigger = 1"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "                   Trigger = 1"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                    DataSource = Drv1"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Default = \"{3 6 9 12 15 18}\""
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = DataSourceITestHelper"
        "            Signals = {"
        "                Signal1A = {"
        "                    Type = uint32"
        "                }"
        "                Signal2 = {"
        "                    Type = float32"
        "                }"
        "                Signal3 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMC GAMA GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMB GAME GAMF}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal1A"
        "               }"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal4 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Alias = Signal4A"
        "                   NumberOfDimensions = 2"
        "                   NumberOfElements = 10"
        "                   Ranges = {{0 0} {2 5} {9 9}}"
        "               }"
        "               Signal5 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "                   Samples = 3"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = DataSourceITestGAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = DataSourceITestGAM1"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                    DataSource = Drv1"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Default = \"{3 6 9 12 15 18}\""
        "                    Frequency = 10.0"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = DataSourceITestHelper"
        "            Signals = {"
        "                Signal1A = {"
        "                    Type = uint32"
        "                }"
        "                Signal2 = {"
        "                    Type = float32"
        "                }"
        "                Signal3 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                }"
        "            }"
        "        }"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMC GAMA GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMB GAME GAMF}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool DataSourceITest::TestConstructor() {
    DataSourceITestHelper dataSource;
    return dataSource.GetNumberOfSignals() == 0u;
}

bool DataSourceITest::TestInitialise() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    if (ret) {
        ReferenceT<DataSourceITestHelper> dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    return ret;
}

bool DataSourceITest::TestAddSignals() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    ConfigurationDatabase test;
    if (ret) {
        ret = dataSource->AddSignals(test);
    }
    if (ret) {
        ret = test.MoveAbsolute("Signals");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 3);
    }
    if (ret) {
        ret = test.MoveRelative("Signal1A");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 1);
    }
    StreamString value;
    if (ret) {
        ret = (test.Read("Type", value));
    }
    if (ret) {
        ret = (value == "uint32");
        value = "";
    }
    if (ret) {
        ret = test.MoveToAncestor(1u);
    }
    if (ret) {
        ret = test.MoveRelative("Signal2");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 1);
    }
    if (ret) {
        ret = (test.Read("Type", value));
    }
    if (ret) {
        ret = (value == "float32");
        value = "";
    }
    if (ret) {
        ret = test.MoveToAncestor(1u);
    }
    if (ret) {
        ret = test.MoveRelative("Signal3");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 3);
    }
    if (ret) {
        ret = (test.Read("Type", value));
    }
    if (ret) {
        ret = (value == "int32");
        value = "";
    }
    if (ret) {
        ret = (test.Read("NumberOfDimensions", value));
    }
    if (ret) {
        ret = (value == "2");
        value = "";
    }
    if (ret) {
        ret = (test.Read("NumberOfElements", value));
    }
    if (ret) {
        ret = (value == "6");
        value = "";
    }
    if (ret) {
        ret = test.MoveToAncestor(1u);
    }
    return ret;
}

bool DataSourceITest::TestSetConfiguredDatabase() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    return ret;
}

bool DataSourceITest::TestGetNumberOfSignals() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 5u);
    }
    return ret;
}

bool DataSourceITest::TestGetSignalIndex() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    StreamString name;
    uint32 idx;
    uint32 n;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            name = "";
            ret = dataSource->GetSignalName(idx, name);
        }
        if (ret) {
            ret = (name == signalNames[n]);
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalIndex(idx, "DoesNotExist"));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalName() {
    bool ret = TestGetSignalIndex();
    if (ret) {
        ReferenceT<DataSourceITestHelper> dataSource;
        if (ret) {
            dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
            ret = dataSource.IsValid();
        }
        StreamString name;
        if (ret) {
            ret = !(dataSource->GetSignalName(100000, name));
        }
    }
    return ret;
}

bool DataSourceITest::TestGetSignalType() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalTypes[] = { "uint32", "float32", "int32", "uint32", "uint32" };
    StreamString name;
    uint32 idx;
    uint32 n;
    TypeDescriptor desc;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            desc = dataSource->GetSignalType(idx);
            ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName(signalTypes[n]));
        }
    }
    if (ret) {
        desc = dataSource->GetSignalType(10000);
        ret = (desc == InvalidType);
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfDimensions() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    uint32 signalDimensions[] = { 0, 0, 2, 2, 0 };
    StreamString name;
    uint32 idx;
    uint32 n;
    uint8 dimensions;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            ret = dataSource->GetSignalNumberOfDimensions(idx, dimensions);
        }
        if (ret) {
            ret = (dimensions == signalDimensions[n]);
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfDimensions(100000, dimensions));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfElements() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    uint32 signalElements[] = { 1, 1, 6, 10, 1 };
    StreamString name;
    uint32 idx;
    uint32 n;
    uint32 elements;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            ret = dataSource->GetSignalNumberOfElements(idx, elements);
        }
        if (ret) {
            ret = (elements == signalElements[n]);
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfElements(100000, elements));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalByteSize() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    uint32 byteSizes[] = { 4, 4, 24, 40, 4 };
    StreamString name;
    uint32 idx;
    uint32 n;
    uint32 byteSize;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            ret = dataSource->GetSignalByteSize(idx, byteSize);
        }
        if (ret) {
            ret = (byteSize == byteSizes[n]);
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfElements(100000, byteSize));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfStates() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    uint32 signalNStates[] = { 1, 1, 2, 1, 1 };
    StreamString name;
    uint32 idx;
    uint32 n;
    uint32 numberOfStates;
    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        if (ret) {
            ret = dataSource->GetSignalNumberOfStates(idx, numberOfStates);
        }
        if (ret) {
            ret = (numberOfStates == signalNStates[n]);
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfStates(100000, numberOfStates));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalStateName() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalStateNames[][2] = { { "State1", NULL }, {"State2", NULL}, {"State1", "State2"}, {"State1", NULL}, {"State1", NULL}};
    StreamString stateName;
    uint32 idx;
    uint32 n;
    uint32 numberOfStates = 2;

    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        uint32 s;
        for (s = 0u; (s < numberOfStates) && (ret); s++) {
            ret = dataSource->GetSignalStateName(idx, s, stateName);
            if (signalStateNames[n][s] == NULL) {
                ret = !ret;
            }
            else if (ret) {
                ret = (stateName == signalStateNames[n][s]);
            }
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalStateName(1000000, 0, stateName));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfConsumers() {

    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalStates[] = { "State1", "State2" };
    uint32 signalConsumers[][2] = { { 1, 0 }, { 0, 2 }, { 0, 0 }, { 1, 0 }, { 1, 0 } };
    uint32 numberOfStates = 2;

    uint32 numberOfConsumers;
    uint32 idx;
    uint32 n;

    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        uint32 s;
        for (s = 0u; (s < numberOfStates) && (ret); s++) {
            ret = (dataSource->GetSignalNumberOfConsumers(idx, signalStates[s], numberOfConsumers));
            if (signalConsumers[n][s] == 0) {
                ret = !ret;
            }
            else {
                ret = (numberOfConsumers == signalConsumers[n][s]);
            }
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(0, "NonExistentState", numberOfConsumers));
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(100000, "State1", numberOfConsumers));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfProducers() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalStates[] = { "State1", "State2" };
    uint32 signalProducers[][2] = { { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, 0 } };
    uint32 numberOfStates = 2;

    uint32 numberOfProducers;
    uint32 idx;
    uint32 n;

    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        uint32 s;
        for (s = 0u; (s < numberOfStates) && (ret); s++) {
            ret = (dataSource->GetSignalNumberOfProducers(idx, signalStates[s], numberOfProducers));
            if (signalProducers[n][s] == 0) {
                ret = !ret;
            }
            else {
                ret = (numberOfProducers == signalProducers[n][s]);
            }
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfProducers(0, "NonExistentState", numberOfProducers));
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfProducers(100000, "State1", numberOfProducers));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalConsumerName() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const uint32 numberOfStates = 2;
    const uint32 numberOfConsumers = 2;
    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalStates[] = { "State1", "State2" };
    const char8 *signalConsumerNames[][numberOfStates][numberOfConsumers] = { { { "GAMA", NULL }, {NULL, NULL}}, { {NULL, NULL}, {"GAMB", "GAME"}}, { {
        NULL, NULL}, {NULL, NULL}}, { {"GAMA", NULL}, {NULL, NULL}}, { {"GAMA", NULL}, {NULL, NULL}}};

    uint32 idx;
    uint32 n;
    StreamString name;

    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        uint32 s;
        for (s = 0u; (s < numberOfStates) && (ret); s++) {
            uint32 c;
            for (c = 0u; (c < numberOfConsumers) && (ret); c++) {
                ret = (dataSource->GetSignalConsumerName(idx, signalStates[s], c, name));
                if (signalConsumerNames[n][s][c] == NULL) {
                    ret = !ret;
                }
                else {
                    ret = (name == signalConsumerNames[n][s][c]);
                }
            }
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(0, "NonExistentState", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(100000, "State1", 0, name));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalProducerName() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfSignals = dataSource->GetNumberOfSignals();
    if (ret) {
        ret = (numberOfSignals == 5u);
    }

    const uint32 numberOfStates = 2;
    const char8 *signalNames[] = { "Signal1A", "Signal2", "Signal3", "Signal4A", "Signal5" };
    const char8 *signalStates[] = { "State1", "State2" };
    const char8 *signalProducerNames[][numberOfStates] = { { "GAMC", NULL }, {NULL, "GAMD"}, {"GAMF", "GAMF"}, {"GAMC", NULL}, {"GAMC", NULL}};

    uint32 idx;
    uint32 n;
    StreamString name;

    for (n = 0; (n < numberOfSignals) && (ret); n++) {
        ret = dataSource->GetSignalIndex(idx, signalNames[n]);
        uint32 s;
        for (s = 0u; (s < numberOfStates) && (ret); s++) {
            ret = (dataSource->GetSignalProducerName(idx, signalStates[s], 0, name));
            if (signalProducerNames[n][s] == NULL) {
                ret = !ret;
            }
            else {
                ret = (name == signalProducerNames[n][s]);
            }
        }
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(0, "NonExistentState", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(0, "State1", 10000, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(100000, "State1", 0, name));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalDefaultValue() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 5u);
    }

    uint32 defaultValueUInt32;
    if (ret) {
        ret = dataSource->GetSignalDefaultValue(0, defaultValueUInt32);
    }
    if (ret) {
        ret = (defaultValueUInt32 == 7u);
    }
    if (ret) {
        ret = !dataSource->GetSignalDefaultValue(1, defaultValueUInt32);
    }
    int32 defaultValueInt32Arr[6];
    if (ret) {
        ret = dataSource->GetSignalDefaultValue(2, defaultValueInt32Arr);
    }
    if (ret) {
        int32 i;
        for (i = 1; (i < 7) && ret; i++) {
            ret = (defaultValueInt32Arr[i - 1] == 3 * i);
        }
    }
    if (ret) {
        ret = !dataSource->GetSignalDefaultValue(1, defaultValueInt32Arr);
    }
    return ret;
}

bool DataSourceITest::TestGetSignalDefaultValueType() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 5u);
    }

    AnyType defaultValueAnyType = dataSource->GetSignalDefaultValueType(0);
    if (ret) {
        ret = (defaultValueAnyType.GetTypeDescriptor() == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }
    if (ret) {
        defaultValueAnyType = dataSource->GetSignalDefaultValueType(1);
        ret = (defaultValueAnyType.GetTypeDescriptor() == VoidType);
    }
    if (ret) {
        defaultValueAnyType = dataSource->GetSignalDefaultValueType(2);
        ret = (defaultValueAnyType.GetTypeDescriptor() == TypeDescriptor::GetTypeDescriptorFromTypeName("int32"));
    }
    if (ret) {
        ret = (defaultValueAnyType.GetNumberOfDimensions() == 1u);
    }
    if (ret) {
        ret = (defaultValueAnyType.GetNumberOfElements(0u) == 6u);
    }

    return ret;
}

bool DataSourceITest::TestGetNumberOfFunctions() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfFunctions() == 6u);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionIndex() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    uint32 idx;
    StreamString value;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };

    uint32 n;
    for (n = 0u; (n < numberOfFunctions) && ret; n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            ret = dataSource->GetFunctionName(idx, value);
        }
        if (ret) {
            ret = (value == functionNames[n]);
            value = "";
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionName(10000, value);
    }
    if (ret) {
        ret = !dataSource->GetFunctionIndex(idx, "GAMDoesNotExist");
    }

    return ret;
}

bool DataSourceITest::TestGetFunctionName() {
    return TestGetFunctionIndex();
}

bool DataSourceITest::TestGetFunctionNumberOfSignals() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    uint32 idx;
    StreamString value;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    uint32 numberOfSignalsInput[] = { 3, 1, 0, 0, 1, 0 };
    uint32 numberOfSignalsOutput[] = { 0, 0, 3, 1, 0, 1 };

    uint32 n;
    uint32 numberOfSignals = 0u;
    for (n = 0u; (n < numberOfFunctions) && ret; n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            ret = dataSource->GetFunctionNumberOfSignals(InputSignals, idx, numberOfSignals);
        }
        if (ret) {
            ret = (numberOfSignals == numberOfSignalsInput[n]);
        }
        if (ret) {
            ret = dataSource->GetFunctionNumberOfSignals(OutputSignals, idx, numberOfSignals);
        }
        if (ret) {
            ret = (numberOfSignals == numberOfSignalsOutput[n]);
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionNumberOfSignals(InputSignals, 100000u, numberOfSignals);
    }
    if (ret) {
        ret = !dataSource->GetFunctionNumberOfSignals(OutputSignals, 100000u, numberOfSignals);
    }

    return ret;
}

bool DataSourceITest::TestGetFunctionSignalsByteSize() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    uint32 idx;
    StreamString value;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    //Remember that the size is also multiplied by the number of samples
    uint32 numberOfBytesInput[] = { 40, 4, 0, 0, 4, 0 };
    uint32 numberOfBytesOutput[] = { 0, 0, 40, 4, 0, 24 };

    uint32 n;
    uint32 numberOfBytes = 0u;
    for (n = 0u; (n < numberOfFunctions) && ret; n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            ret = dataSource->GetFunctionSignalsByteSize(InputSignals, idx, numberOfBytes);
        }
        if (ret) {
            ret = (numberOfBytes == numberOfBytesInput[n]);
        }
        if (ret) {
            ret = dataSource->GetFunctionSignalsByteSize(OutputSignals, idx, numberOfBytes);
        }
        if (ret) {
            ret = (numberOfBytes == numberOfBytesOutput[n]);
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalsByteSize(InputSignals, 100000u, numberOfBytes);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalsByteSize(OutputSignals, 100000u, numberOfBytes);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalName() {
    return TestGetFunctionSignalIndex();
}

bool DataSourceITest::TestGetFunctionSignalIndex() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};

    uint32 n;
    uint32 functionSignalIdx;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    value = "";
                    ret = dataSource->GetFunctionSignalName(InputSignals, idx, functionSignalIdx, value);
                    if (ret) {
                        ret = (value == functionInputSignalNames[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    value = "";
                    ret = dataSource->GetFunctionSignalName(OutputSignals, idx, functionSignalIdx, value);
                    if (ret) {
                        ret = (value == functionOutputSignalNames[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(InputSignals, 0, functionSignalIdx, "NonExistentFunctionName");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(InputSignals, 100000, functionSignalIdx, "Signal4A");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(OutputSignals, 0, functionSignalIdx, "NonExistentFunctionName");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(OutputSignals, 100000, functionSignalIdx, "Signal4A");
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalAlias() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    const char8 *functionInputSignalAlias[][maxNumberOfInputSignals] = { { "Signal4A", "Signal5", "Signal1A" }, { "Signal2", "", "" }, { "", "", "" }, { "", "",
            "" }, { "Signal2", "", "" }, { "Signal3", "", "" } };
    const char8 *functionOutputSignalAlias[][maxNumberOfInputSignals] = { { "", "", "" }, { "", "", "" }, { "Signal1A", "Signal4A", "Signal5" }, { "Signal2",
            "", "" }, { "", "", "" }, { "Signal3", "", "" } };

    uint32 n;
    uint32 functionSignalIdx;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    value = "";
                    ret = dataSource->GetFunctionSignalAlias(InputSignals, idx, functionSignalIdx, value);
                    if (ret) {
                        ret = (value == functionInputSignalAlias[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    value = "";
                    ret = dataSource->GetFunctionSignalAlias(OutputSignals, idx, functionSignalIdx, value);
                    if (ret) {
                        ret = (value == functionOutputSignalAlias[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(InputSignals, 0, functionSignalIdx, "NonExistentFunctionName");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(InputSignals, 100000, functionSignalIdx, "Signal4");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(OutputSignals, 0, functionSignalIdx, "NonExistentFunctionName");
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalIndex(OutputSignals, 100000, functionSignalIdx, "Signal4");
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalNumberOfByteOffsets() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    uint32 numberOfBytesOffsetsInput[][maxNumberOfInputSignals] = { { 3, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
    uint32 numberOfBytesOffsetsOutput[][maxNumberOfOutputSignals] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 3, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

    uint32 n;
    uint32 functionSignalIdx;
    uint32 numberOfBytesOffsets;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalNumberOfByteOffsets(InputSignals, idx, functionSignalIdx, numberOfBytesOffsets);
                    if (ret) {
                        ret = (numberOfBytesOffsets == numberOfBytesOffsetsInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalNumberOfByteOffsets(OutputSignals, idx, functionSignalIdx, numberOfBytesOffsets);
                    if (ret) {
                        ret = (numberOfBytesOffsets == numberOfBytesOffsetsOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalNumberOfByteOffsets(InputSignals, 0, 10000, numberOfBytesOffsets);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalNumberOfByteOffsets(OutputSignals, 0, 10000, numberOfBytesOffsets);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalNumberOfByteOffsets(InputSignals, 10000, 0, numberOfBytesOffsets);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalNumberOfByteOffsets(OutputSignals, 10000, 0, numberOfBytesOffsets);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalByteOffsetInfo() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const uint32 maxNumberOfByteOffsets = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    uint32 byteOffsetStartIndexInput[][maxNumberOfInputSignals][maxNumberOfByteOffsets] = { { { 0, 8, 36 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0,
            0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 byteOffsetStartIndexOutput[][maxNumberOfInputSignals][maxNumberOfByteOffsets] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 8, 36 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0,
            0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 byteOffsetSizeInput[][maxNumberOfOutputSignals][maxNumberOfByteOffsets] = { { { 4, 16, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, {
            4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4,
            4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } } };
    uint32 byteOffsetSizeOutput[][maxNumberOfOutputSignals][maxNumberOfByteOffsets] = { { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, {
            4, 4, 4 } }, { { 4, 4, 4 }, { 4, 16, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { { 4, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } }, { {
            24, 4, 4 }, { 4, 4, 4 }, { 4, 4, 4 } } };

    uint32 n;
    uint32 functionSignalIdx;
    uint32 byteOffsetIdx;
    uint32 byteOffsetSize;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    uint32 b;
                    uint32 numberOfByteOffsets = 0;
                    if (ret) {
                        ret = dataSource->GetFunctionSignalNumberOfByteOffsets(InputSignals, idx, functionSignalIdx, numberOfByteOffsets);
                    }
                    for (b = 0u; (b < numberOfByteOffsets) && (ret); b++) {
                        ret = dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, idx, functionSignalIdx, b, byteOffsetIdx, byteOffsetSize);
                        if (ret) {
                            ret = (byteOffsetIdx == byteOffsetStartIndexInput[n][i][b]);
                        }
                        if (ret) {
                            ret = (byteOffsetSize == byteOffsetSizeInput[n][i][b]);
                        }
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    uint32 b;
                    uint32 numberOfByteOffsets = 0;
                    if (ret) {
                        ret = dataSource->GetFunctionSignalNumberOfByteOffsets(OutputSignals, idx, functionSignalIdx, numberOfByteOffsets);
                    }
                    for (b = 0u; (b < numberOfByteOffsets) && (ret); b++) {
                        ret = dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, idx, functionSignalIdx, b, byteOffsetIdx, byteOffsetSize);
                        if (ret) {
                            ret = (byteOffsetIdx == byteOffsetStartIndexOutput[n][i][b]);
                        }
                        if (ret) {
                            ret = (byteOffsetSize == byteOffsetSizeOutput[n][i][b]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, 0, 10000, 0, byteOffsetIdx, byteOffsetSize);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, 0, 10000, 0, byteOffsetIdx, byteOffsetSize);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, 10000, 0, 0, byteOffsetIdx, byteOffsetSize);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, 10000, 0, 0, byteOffsetIdx, byteOffsetSize);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(InputSignals, 0, 0, 10000, byteOffsetIdx, byteOffsetSize);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalByteOffsetInfo(OutputSignals, 0, 0, 10000, byteOffsetIdx, byteOffsetSize);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalSamples() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    uint32 numberOfSamplesInput[][maxNumberOfInputSignals] = { { 1, 3, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };
    uint32 numberOfSamplesOutput[][maxNumberOfOutputSignals] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 3 }, { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

    uint32 n;
    uint32 functionSignalIdx;
    uint32 numberOfSamples;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalSamples(InputSignals, idx, functionSignalIdx, numberOfSamples);
                    if (ret) {
                        ret = (numberOfSamples == numberOfSamplesInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalSamples(OutputSignals, idx, functionSignalIdx, numberOfSamples);
                    if (ret) {
                        ret = (numberOfSamples == numberOfSamplesOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalSamples(InputSignals, 0, 10000, numberOfSamples);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalSamples(OutputSignals, 0, 10000, numberOfSamples);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalSamples(InputSignals, 10000, 0, numberOfSamples);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalSamples(OutputSignals, 10000, 0, numberOfSamples);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalReadFrequencyInput() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    float32 frequencyInput[][maxNumberOfInputSignals] = { { -1.0, 5.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0,
            -1.0 }, { -1.0, -1.0, -1.0 } };
    float32 frequencyOutput[][maxNumberOfOutputSignals] = { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0,
            -1.0, -1.0 }, { -1.0, -1.0, -1.0 } };

    uint32 n;
    uint32 functionSignalIdx;
    float32 frequency;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalReadFrequency(InputSignals, idx, functionSignalIdx, frequency);
                    if (ret) {
                        ret = (frequency == frequencyInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalReadFrequency(OutputSignals, idx, functionSignalIdx, frequency);
                    if (ret) {
                        ret = (frequency == frequencyOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(InputSignals, 0, 10000, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(OutputSignals, 0, 10000, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(InputSignals, 10000, 0, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(OutputSignals, 10000, 0, frequency);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalReadFrequencyOutput() {
    bool ret = InitialiseDataSourceIEnviroment(config2);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    float32 frequencyInput[][maxNumberOfInputSignals] = { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0,
            -1.0 }, { -1.0, -1.0, -1.0 } };
    float32 frequencyOutput[][maxNumberOfOutputSignals] = { { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0, -1.0, -1.0 }, { -1.0,
            -1.0, -1.0 }, { 10.0, -1.0, -1.0 } };

    uint32 n;
    uint32 functionSignalIdx;
    float32 frequency;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalReadFrequency(InputSignals, idx, functionSignalIdx, frequency);
                    if (ret) {
                        ret = (frequency == frequencyInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalReadFrequency(OutputSignals, idx, functionSignalIdx, frequency);
                    if (ret) {
                        ret = (frequency == frequencyOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(InputSignals, 0, 10000, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(OutputSignals, 0, 10000, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(InputSignals, 10000, 0, frequency);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalReadFrequency(OutputSignals, 10000, 0, frequency);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalTrigger() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    uint32 triggerInput[][maxNumberOfInputSignals] = { { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    uint32 triggerOutput[][maxNumberOfOutputSignals] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

    uint32 n;
    uint32 functionSignalIdx;
    uint32 trigger;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalTrigger(InputSignals, idx, functionSignalIdx, trigger);
                    if (ret) {
                        ret = (trigger == triggerInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalTrigger(OutputSignals, idx, functionSignalIdx, trigger);
                    if (ret) {
                        ret = (trigger == triggerOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalTrigger(InputSignals, 0, 10000, trigger);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalTrigger(OutputSignals, 0, 10000, trigger);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalTrigger(InputSignals, 10000, 0, trigger);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalTrigger(OutputSignals, 10000, 0, trigger);
    }
    return ret;
}

bool DataSourceITest::TestGetFunctionSignalGAMMemoryOffset() {
    bool ret = InitialiseDataSourceIEnviroment(config2);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();

    if (ret) {
        ret = (numberOfFunctions == 6u);
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    uint32 offsetInput[][maxNumberOfInputSignals] = { { 0, 24, 40 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
    uint32 offsetOutput[][maxNumberOfOutputSignals] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 8, 32 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

    uint32 n;
    uint32 functionSignalIdx;
    uint32 offset;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalGAMMemoryOffset(InputSignals, idx, functionSignalIdx, offset);
                    if (ret) {
                        ret = (offset == offsetInput[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->GetFunctionSignalGAMMemoryOffset(OutputSignals, idx, functionSignalIdx, offset);
                    if (ret) {
                        ret = (offset == offsetOutput[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalGAMMemoryOffset(InputSignals, 0, 10000, offset);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalGAMMemoryOffset(OutputSignals, 0, 10000, offset);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalGAMMemoryOffset(InputSignals, 10000, 0, offset);
    }
    if (ret) {
        ret = !dataSource->GetFunctionSignalGAMMemoryOffset(OutputSignals, 10000, 0, offset);
    }
    return ret;
}

bool DataSourceITest::TestGetAddBrokers() {
    //Implicitly tested in the RealTimeApplicationConfigurationBuilder
    return InitialiseDataSourceIEnviroment(config1);
}

bool DataSourceITest::TestIsSupportedBroker() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        dataSource->IsSupportedBroker(InputSignals, 0, 0, "MemoryMapInputBroker");
    }
    StreamString value;
    uint32 idx;
    const uint32 maxNumberOfInputSignals = 3;
    const uint32 maxNumberOfOutputSignals = 3;
    const char8 *functionNames[] = { "GAMA", "GAMB", "GAMC", "GAMD", "GAME", "GAMF" };
    const char8 *functionInputSignalNames[][maxNumberOfInputSignals] = { { "Signal4", "Signal5", "Signal1" }, { "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"Signal2", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalNames[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"Signal1", "Signal4", "Signal5"}, {
    "Signal2", NULL, NULL}, {NULL, NULL, NULL}, {"Signal3", NULL, NULL}};
    const char8 *functionInputSignalBrokers[][maxNumberOfInputSignals] = {
            { "MemoryMapInputBroker", "MemoryMapSynchronisedInputBroker", "MemoryMapInputBroker" }, { "MemoryMapInputBroker", NULL, NULL}, {NULL, NULL, NULL}, {
    NULL, NULL, NULL}, {"MemoryMapInputBroker", NULL, NULL}, {NULL, NULL, NULL}};
    const char8 *functionOutputSignalBrokers[][maxNumberOfOutputSignals] = { { NULL, NULL, NULL}, {NULL, NULL, NULL}, {"MemoryMapOutputBroker", "MemoryMapSynchronisedOutputBroker", "MemoryMapOutputBroker"}, {
    "MemoryMapSynchronisedOutputBroker", NULL, NULL}, {NULL, NULL, NULL}, {"MemoryMapOutputBroker", NULL, NULL}};

    uint32 numberOfFunctions = dataSource->GetNumberOfFunctions();
    uint32 n;
    uint32 functionSignalIdx;
    for (n = 0u; (n < numberOfFunctions) && (ret); n++) {
        ret = dataSource->GetFunctionIndex(idx, functionNames[n]);
        if (ret) {
            uint32 i;
            for (i = 0; (i < maxNumberOfInputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(InputSignals, idx, functionSignalIdx, functionInputSignalNames[n][i]);
                if (functionInputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->IsSupportedBroker(InputSignals, idx, functionSignalIdx, functionInputSignalBrokers[n][i]);
                    if (ret) {
                        ret = !dataSource->IsSupportedBroker(OutputSignals, idx, functionSignalIdx, functionInputSignalBrokers[n][i]);
                    }
                }
            }
            for (i = 0; (i < maxNumberOfOutputSignals) && (ret); i++) {
                ret = dataSource->GetFunctionSignalIndex(OutputSignals, idx, functionSignalIdx, functionOutputSignalNames[n][i]);
                if (functionOutputSignalNames[n][i] == NULL) {
                    ret = !ret;
                }
                else if (ret) {
                    ret = dataSource->IsSupportedBroker(OutputSignals, idx, functionSignalIdx, functionOutputSignalBrokers[n][i]);
                    if (ret) {
                        ret = !dataSource->IsSupportedBroker(InputSignals, idx, functionSignalIdx, functionOutputSignalBrokers[n][i]);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = !dataSource->IsSupportedBroker(InputSignals, 0, 10000, "MemoryMapInputBroker");
    }
    if (ret) {
        ret = !dataSource->IsSupportedBroker(InputSignals, 100000, 0, "MemoryMapInputBroker");
    }
    if (ret) {
        ret = !dataSource->IsSupportedBroker(OutputSignals, 0, 10000, "MemoryMapOutputBroker");
    }
    if (ret) {
        ret = !dataSource->IsSupportedBroker(OutputSignals, 100000, 0, "MemoryMapOutputBroker");
    }
    return ret;
}

bool DataSourceITest::TestGetCurrentStateBuffer() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetCurrentStateBuffer() == 0u);
    }
    return ret;
}

bool DataSourceITest::TestGetNumberOfStatefulMemoryBuffers() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfStatefulMemoryBuffers() == 1u);
    }
    return ret;
}

bool DataSourceITest::TestPrepareInputOffsets() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        (dataSource->PrepareInputOffsets());
    }
    return ret;
}

bool DataSourceITest::TestPrepareOutputOffsets() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        (dataSource->PrepareOutputOffsets());
    }
    return ret;
}

bool DataSourceITest::TestGetInputOffset() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 offset;
        ret = (!dataSource->GetInputOffset(0, 0, offset));
    }
    return ret;
}

bool DataSourceITest::TestGetOutputOffset() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 offset;
        ret = (!dataSource->GetOutputOffset(0, 0, offset));
    }
    return ret;
}

bool DataSourceITest::TestTerminateInputCopy() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->TerminateInputCopy(0, 0, 0));
    }
    return ret;
}

bool DataSourceITest::TestTerminateOutputCopy() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->TerminateOutputCopy(0, 0, 0));
    }
    return ret;
}

bool DataSourceITest::TestGetNumberOfMemoryBuffers() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfMemoryBuffers() == 1u);
    }
    return ret;
}

bool DataSourceITest::TestGetInputBrokers() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 j;
        for (j = 0; (j < dataSource->allInputBrokers.Size()) && (ret); j++) {
            ReferenceT<DataSourceITestBrokerRef> brokerRef = dataSource->allInputBrokers.Get(j);
            if (brokerRef->gamName == "GAMA") {
                ReferenceT<MemoryMapSynchronisedInputBroker> broker = brokerRef->brokers.Get(0);
                if (ret) {
                    ret = broker.IsValid();
                }
                if (ret) {
                    ret = (broker->GetNumberOfCopies() == 1);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 2);
                }
                if (ret) {
                    ReferenceT<MemoryMapInputBroker> brokerNoSync = brokerRef->brokers.Get(1);
                    ret = (brokerNoSync->GetNumberOfCopies() == 4);
                }
            }
            if (brokerRef->gamName == "GAMB") {
                ReferenceT<MemoryMapInputBroker> broker = brokerRef->brokers.Get(0);
                if (ret) {
                    ret = broker.IsValid();
                }
                if (ret) {
                    ret = (broker->GetNumberOfCopies() == 1);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 1);
                }
            }
            if (brokerRef->gamName == "GAME") {
                ReferenceT<MemoryMapInputBroker> broker = brokerRef->brokers.Get(0);
                if (ret) {
                    ret = broker.IsValid();
                }
                if (ret) {
                    ret = (broker->GetNumberOfCopies() == 1);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 1);
                }
            }
        }
    }
    return ret;
}


bool DataSourceITest::TestGetOutputBrokers() {
    bool ret = InitialiseDataSourceIEnviroment(config1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        uint32 j;
        for (j = 0; (j < dataSource->allOutputBrokers.Size()) && (ret); j++) {
            ReferenceT<DataSourceITestBrokerRef> brokerRef = dataSource->allOutputBrokers.Get(j);
            if (brokerRef->gamName == "GAMC") {
                ReferenceT<MemoryMapOutputBroker> brokerNoSync = brokerRef->brokers.Get(0);
                ret = brokerNoSync.IsValid();
                if (ret) {
                    ret = (brokerNoSync->GetNumberOfCopies() == 2);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 2);
                }
                if (ret) {
                    ReferenceT<MemoryMapSynchronisedOutputBroker> broker = brokerRef->brokers.Get(1);
                    ret = (broker->GetNumberOfCopies() == 3);
                }
            }
            if (brokerRef->gamName == "GAMD") {
                ReferenceT<MemoryMapSynchronisedOutputBroker> broker = brokerRef->brokers.Get(0);
                ret = broker.IsValid();
                if (ret) {
                    ret = (broker->GetNumberOfCopies() == 1);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 1);
                }
            }
            if (brokerRef->gamName == "GAMF") {
                ReferenceT<MemoryMapOutputBroker> broker = brokerRef->brokers.Get(0);
                ret = broker.IsValid();
                if (ret) {
                    ret = (broker->GetNumberOfCopies() == 1);
                }
                if (ret) {
                    ret = (brokerRef->brokers.Size() == 1);
                }
            }
        }
    }
    return ret;
}

bool DataSourceITest::TestSynchronise() {
    DataSourceITestHelper test;
    return !test.Synchronise();
}

bool DataSourceITest::TestBrokerCopyTerminated() {
    DataSourceITestHelper test;
    return test.BrokerCopyTerminated();
}

