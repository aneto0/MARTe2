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
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class DataSourceITestHelper: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceITestHelper    ();

    virtual ~DataSourceITestHelper();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
            uint32 bufferIdx,
            void **&signalAddress);

    virtual const char8 *Negotiate(StructuredDataI &data,
            SignalDirection direction);

    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    virtual bool AddInputBrokerToGAM(ReferenceT<GAM> gam,
            const char8* functionName,
            void * gamMemPtr);

    virtual bool AddOutputBrokerToGAM(ReferenceT<GAM> gam,
            const char8* functionName,
            void * gamMemPtr);

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
    return 0u;
}

bool DataSourceITestHelper::GetSignalMemoryBuffer(uint32 signalIdx,
                                                  uint32 bufferIdx,
                                                  void **&signalAddress) {
    return true;
}

const char8 *DataSourceITestHelper::Negotiate(StructuredDataI &data,
                                              SignalDirection direction) {
    return "MemoryMapInputBroker";
}

bool DataSourceITestHelper::PrepareNextState(const RealTimeStateInfo &status) {
    return true;
}

bool DataSourceITestHelper::AddInputBrokerToGAM(ReferenceT<GAM> gam,
                                                const char8* functionName,
                                                void * gamMemPtr) {
    return true;
}

bool DataSourceITestHelper::AddOutputBrokerToGAM(ReferenceT<GAM> gam,
                                                 const char8* functionName,
                                                 void * gamMemPtr) {
    return true;
}
CLASS_REGISTER(DataSourceITestHelper, "1.0");

static bool InitialiseDataSourceIEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    parser.Parse();

    ObjectRegistryDatabase::Instance()->CleanUp();

    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");

    return application->ConfigureApplication();
}

static const char8 * const startedTestConfig1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Default = 7"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Drv1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal2 = {"
        "                   DataSource = Drv1"
        "                   Type = float32"
        "               }"
        "            }"
        "        }"
        "        +GAMF = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                    DataSource = Drv1"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Frequency = 10"
        "                    Default = \"{3 6 9 12 15 18\"}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +Drv1 = {"
        "            Class = DataSourceITestHelper"
        "            Signals = {"
        "                Signal1 = {"
        "                    Type = uint32"
        "                }"
        "                Signal2 = {"
        "                    Type = float32"
        "                }"
        "                Signal3 = {"
        "                    Type = int32"
        "                    NumberOfDimensions = 2"
        "                    NumberOfElements = 6"
        "                    Frequency = 10"
        "                }"
        "            }"
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
        "                    Functions = {:Functions.GAMC :Functions.GAMA :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMD :Functions.GAMB :Functions.GAME :Functions.GAMF}"
        "                }"
        "            }"
        "        }"
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
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    if (ret) {
        ReferenceT<DataSourceITestHelper> dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    return ret;
}

bool DataSourceITest::TestAddSignals() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
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
        ret = test.MoveRelative("Signal1");
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
        ret = (test.GetNumberOfChildren() == 4);
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
        ret = (test.Read("Frequency", value));
    }
    if (ret) {
        ret = (value == "10");
        value = "";
    }
    if (ret) {
        ret = test.MoveToAncestor(1u);
    }
    return ret;
}

bool DataSourceITest::TestSetConfiguredDatabase() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    return ret;
}

bool DataSourceITest::TestGetNumberOfSignals() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    return ret;
}

bool DataSourceITest::TestGetSignalIndex() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 idx;
    if (ret) {
        ret = dataSource->GetSignalIndex(idx, "Signal1");
    }
    if (ret) {
        ret = (idx == 0);
    }
    if (ret) {
        ret = dataSource->GetSignalIndex(idx, "Signal2");
    }
    if (ret) {
        ret = (idx == 1);
    }
    if (ret) {
        ret = dataSource->GetSignalIndex(idx, "Signal3");
    }
    if (ret) {
        ret = (idx == 2);
    }
    if (ret) {
        ret = !(dataSource->GetSignalIndex(idx, "DoesNotExist"));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalName() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    StreamString value;
    if (ret) {
        ret = dataSource->GetSignalName(0, value);
    }
    if (ret) {
        ret = (value == "Signal1");
        value = "";
    }
    if (ret) {
        ret = dataSource->GetSignalName(1, value);
    }
    if (ret) {
        ret = (value == "Signal2");
        value = "";
    }
    if (ret) {
        ret = dataSource->GetSignalName(2, value);
    }
    if (ret) {
        ret = (value == "Signal3");
        value = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalName(100000, value));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalType() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    TypeDescriptor desc;
    if (ret) {
        desc = dataSource->GetSignalType(0);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
    }
    if (ret) {
        desc = dataSource->GetSignalType(1);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("float32"));
    }
    if (ret) {
        desc = dataSource->GetSignalType(2);
        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName("int32"));
    }
    if (ret) {
        desc = dataSource->GetSignalType(10000);
        ret = (desc == InvalidType);
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfDimensions() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 dimensions;
    if (ret) {
        ret = dataSource->GetSignalNumberOfDimensions(0, dimensions);
    }
    if (ret) {
        ret = (dimensions == 0u);
    }
    if (ret) {
        ret = dataSource->GetSignalNumberOfDimensions(1, dimensions);
    }
    if (ret) {
        ret = (dimensions == 0u);
    }
    if (ret) {
        ret = dataSource->GetSignalNumberOfDimensions(2, dimensions);
    }
    if (ret) {
        ret = (dimensions == 2u);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfDimensions(100000, dimensions));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfElements() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 elements;
    if (ret) {
        ret = dataSource->GetSignalNumberOfElements(0, elements);
    }
    if (ret) {
        ret = (elements == 1u);
    }
    if (ret) {
        ret = dataSource->GetSignalNumberOfElements(1, elements);
    }
    if (ret) {
        ret = (elements == 1u);
    }
    if (ret) {
        ret = dataSource->GetSignalNumberOfElements(2, elements);
    }
    if (ret) {
        ret = (elements == 6u);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfElements(100000, elements));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalByteSize() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 byteSize;
    if (ret) {
        ret = dataSource->GetSignalByteSize(0, byteSize);
    }
    if (ret) {
        ret = (byteSize == 4u);
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(1, byteSize);
    }
    if (ret) {
        ret = (byteSize == 4u);
    }
    if (ret) {
        ret = dataSource->GetSignalByteSize(2, byteSize);
    }
    if (ret) {
        ret = (byteSize == 24u);
    }
    if (ret) {
        ret = !(dataSource->GetSignalByteSize(100000, byteSize));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfStates() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 numberOfStates;
    if (ret) {
        ret = (dataSource->GetSignalNumberOfStates(0, numberOfStates));
    }
    if (ret) {
        ret = (numberOfStates == 1u);
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfStates(1, numberOfStates));
    }
    if (ret) {
        ret = (numberOfStates == 1u);
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfStates(2, numberOfStates));
    }
    if (ret) {
        ret = (numberOfStates == 2u);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfStates(100000, numberOfStates));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalStateName() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    StreamString stateName;
    if (ret) {
        ret = (dataSource->GetSignalStateName(0, 0, stateName));
    }
    if (ret) {
        ret = (stateName == "State1");
        stateName = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalStateName(0, 1, stateName));
    }
    if (ret) {
        ret = (dataSource->GetSignalStateName(1, 0, stateName));
    }
    if (ret) {
        ret = (stateName == "State2");
        stateName = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalStateName(1, 1, stateName));
    }
    if (ret) {
        ret = (dataSource->GetSignalStateName(2, 0, stateName));
    }
    if (ret) {
        ret = (stateName == "State1");
        stateName = "";
    }
    if (ret) {
        ret = (dataSource->GetSignalStateName(2, 1, stateName));
    }
    if (ret) {
        ret = (stateName == "State2");
        stateName = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalStateName(2, 2, stateName));
    }
    if (ret) {
        ret = !(dataSource->GetSignalStateName(3, 0, stateName));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalNumberOfConsumers() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 numberOfConsumers;
    if (ret) {
        ret = (dataSource->GetSignalNumberOfConsumers(0, "State1", numberOfConsumers));
    }
    if (ret) {
        ret = (numberOfConsumers == 1);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(0, "State2", numberOfConsumers));
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(1, "State1", numberOfConsumers));
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfConsumers(1, "State2", numberOfConsumers));
    }
    if (ret) {
        ret = (numberOfConsumers == 2);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(2, "State1", numberOfConsumers));
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfConsumers(2, "State2", numberOfConsumers));
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
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    uint32 numberOfProducers;
    if (ret) {
        ret = (dataSource->GetSignalNumberOfProducers(0, "State1", numberOfProducers));
    }
    if (ret) {
        ret = (numberOfProducers == 1);
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfProducers(0, "State2", numberOfProducers));
    }
    if (ret) {
        ret = !(dataSource->GetSignalNumberOfProducers(1, "State1", numberOfProducers));
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfProducers(1, "State2", numberOfProducers));
    }
    if (ret) {
        ret = (numberOfProducers == 1);
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfProducers(2, "State1", numberOfProducers));
    }
    if (ret) {
        ret = (numberOfProducers == 1);
    }
    if (ret) {
        ret = (dataSource->GetSignalNumberOfProducers(2, "State2", numberOfProducers));
    }
    if (ret) {
        ret = (numberOfProducers == 1);
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
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    StreamString name;
    if (ret) {
        ret = (dataSource->GetSignalConsumerName(0, "State1", 0, name));
    }
    if (ret) {
        ret = (name == "GAMA");
        name = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(0, "State2", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(1, "State1", 0, name));
    }
    if (ret) {
        ret = (dataSource->GetSignalConsumerName(1, "State2", 0, name));
    }
    if (ret) {
        ret = (name == "GAMB");
        name = "";
    }
    if (ret) {
        ret = (dataSource->GetSignalConsumerName(1, "State2", 1, name));
    }
    if (ret) {
        ret = (name == "GAME");
        name = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(2, "State1", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(2, "State2", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(2, "State1", 1, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalConsumerName(2, "State2", 1, name));
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
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }
    StreamString name;
    if (ret) {
        ret = (dataSource->GetSignalProducerName(0, "State1", 0, name));
    }
    if (ret) {
        ret = (name == "GAMC");
        name = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(0, "State2", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(1, "State1", 0, name));
    }
    if (ret) {
        ret = (dataSource->GetSignalProducerName(1, "State2", 0, name));
    }
    if (ret) {
        ret = (name == "GAMD");
        name = "";
    }
    if (ret) {
        ret = (dataSource->GetSignalProducerName(2, "State1", 0, name));
    }
    if (ret) {
        ret = (name == "GAMF");
        name = "";
    }
    if (ret) {
        ret = (dataSource->GetSignalProducerName(2, "State2", 0, name));
    }
    if (ret) {
        ret = (name == "GAMF");
        name = "";
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(2, "State1", 1, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(2, "State2", 1, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(0, "NonExistentState", 0, name));
    }
    if (ret) {
        ret = !(dataSource->GetSignalProducerName(100000, "State1", 0, name));
    }
    return ret;
}

bool DataSourceITest::TestGetSignalDefaultValue() {
    bool ret = InitialiseDataSourceIEnviroment(startedTestConfig1);
    ReferenceT<DataSourceITestHelper> dataSource;
    if (ret) {
        dataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
        ret = dataSource.IsValid();
    }
    if (ret) {
        ret = (dataSource->GetNumberOfSignals() == 3u);
    }

    uint32 defaultValueUInt32;
    if(ret){
        ret = dataSource->GetSignalDefaultValue(0, defaultValueUInt32);
    }
    if(ret){
        ret = (defaultValueUInt32 == 7u);
    }
    return ret;
}
