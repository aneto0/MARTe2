/**
 * @file MemoryDataSourceITest.cpp
 * @brief Source file for class MemoryDataSourceITest
 * @date 11/05/2018
 * @author Andre' Neto
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
 * the class MemoryDataSourceITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "MemoryDataSourceI.h"
#include "MemoryDataSourceITest.h"
#include "MemoryMapInputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief MemoryDataSourceI implementation
 */
class MemoryDataSourceIDataSourceTest: public MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

MemoryDataSourceIDataSourceTest    () :
    MemoryDataSourceI() {

    }

    virtual ~MemoryDataSourceIDataSourceTest() {

    }

    virtual bool Synchronise() {
        return true;
    }

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
        return "MemoryMapInputBroker";
    }

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
        ReferenceT<MemoryMapInputBroker> broker(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        bool ok = broker.IsValid();
        if (ok) {
            ok = (broker->Init(InputSignals, *this, functionName, gamMemPtr));
        }
        if (ok) {
            inputBrokers.Insert(broker);
        }
        return ok;
    }

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
        return false;
    }

    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
        return true;
    }

    virtual uint32 GetNumberOfStatefulMemoryBuffers() {
        return 3u;
    }

    uint32 GetMemorySize() {
        return totalMemorySize;
    }

};
CLASS_REGISTER(MemoryDataSourceIDataSourceTest, "1.0")

/**
 * @brief Dummy GAM to test with cfg.
 */
class MemoryDataSourceITestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

MemoryDataSourceITestGAM1    ();
    virtual bool Setup();
    virtual bool Execute();
};

MemoryDataSourceITestGAM1::MemoryDataSourceITestGAM1() :
        GAM() {

}

bool MemoryDataSourceITestGAM1::Setup() {
    return true;
}

bool MemoryDataSourceITestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(MemoryDataSourceITestGAM1, "1.0")

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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool MemoryDataSourceITest::TestConstructor() {
    MemoryDataSourceIDataSourceTest test;
    return true;
}

bool MemoryDataSourceITest::TestInitialise() {
    MemoryDataSourceIDataSourceTest test;
    ConfigurationDatabase cdb;
    return test.Initialise(cdb);
}

bool MemoryDataSourceITest::TestInitialise_NumberOfBuffers() {
    MemoryDataSourceIDataSourceTest test;
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfBuffers", 3);
    bool ok = test.Initialise(cdb);
    if (ok) {
        ok = (test.GetNumberOfMemoryBuffers() == 3);
    }
    return ok;
}

bool MemoryDataSourceITest::TestInitialise_BadHeapName() {
    MemoryDataSourceIDataSourceTest test;
    ConfigurationDatabase cdb;
    cdb.Write("HeapName", "BAD");
    bool ok = !test.Initialise(cdb);
    return ok;
}

bool MemoryDataSourceITest::TestGetNumberOfMemoryBuffers() {
    return TestInitialise_NumberOfBuffers();
}

bool MemoryDataSourceITest::TestAllocateMemory() {
    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryDataSourceITestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0 0} {2 5} {9 9}}"
            "                   Frequency = 5.0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryDataSourceIDataSourceTest"
            "            NumberOfBuffers = 2"
            "            Signals = {"
            "                Signal1 = {"
            "                    Type = uint32"
            "                }"
            "                Signal2 = {"
            "                    Type = uint32"
            "                }"
            "            }"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMDataSourceTestScheduler1"
            "    }"
            "}";

    bool ok = InitialiseDataSourceIEnviroment(config);
    ReferenceT<MemoryDataSourceIDataSourceTest> dataSourceTest = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
    if (ok) {
        ok = dataSourceTest.IsValid();
    }
    if (ok) {
        uint32 memorySize = dataSourceTest->GetNumberOfStatefulMemoryBuffers() * dataSourceTest->GetNumberOfMemoryBuffers() * (10 + 1) * sizeof(uint32);
        ok = (dataSourceTest->GetMemorySize() == memorySize);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool MemoryDataSourceITest::TestGetSignalMemoryBuffer() {
    static const char8 * const config = ""
            "$Application1 = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = MemoryDataSourceITestGAM1"
            "            InputSignals = {"
            "               Signal1 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   NumberOfElements = 10"
            "                   Ranges = {{0 0} {2 5} {9 9}}"
            "                   Frequency = 5.0"
            "               }"
            "               Signal2 = {"
            "                   DataSource = Drv1"
            "                   Type = uint32"
            "                   Samples = 3"
            "               }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        +Drv1 = {"
            "            Class = MemoryDataSourceIDataSourceTest"
            "            NumberOfBuffers = 2"
            "            Signals = {"
            "                Signal1 = {"
            "                    Type = uint32"
            "                }"
            "                Signal2 = {"
            "                    Type = uint32"
            "                }"
            "            }"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        TimingDataSource = Timings"
            "        Class = GAMDataSourceTestScheduler1"
            "    }"
            "}";

    bool ok = InitialiseDataSourceIEnviroment(config);
    ReferenceT<MemoryDataSourceIDataSourceTest> dataSourceTest = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Drv1");
    if (ok) {
        ok = dataSourceTest.IsValid();
    }
    uint8 *signal1AddressB0;
    uint8 *signal1AddressB1;
    uint8 *signal1AddressB2;
    uint8 *signal2AddressB0;
    uint8 *signal2AddressB1;
    uint8 *signal2AddressB2;

    if (ok) {
        dataSourceTest->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal1AddressB0));
        dataSourceTest->GetSignalMemoryBuffer(0, 1, reinterpret_cast<void *&>(signal1AddressB1));
        dataSourceTest->GetSignalMemoryBuffer(0, 2, reinterpret_cast<void *&>(signal1AddressB2));
        dataSourceTest->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal2AddressB0));
        dataSourceTest->GetSignalMemoryBuffer(1, 1, reinterpret_cast<void *&>(signal2AddressB1));
        dataSourceTest->GetSignalMemoryBuffer(1, 2, reinterpret_cast<void *&>(signal2AddressB2));
    }
    int32 stateBufferSize = 0u;
    if (ok) {
        stateBufferSize = static_cast<int32>(dataSourceTest->GetNumberOfMemoryBuffers() * (10 + 1) * sizeof(uint32));
    }

    if (ok) {
        ok = ((signal2AddressB0 - signal1AddressB0) == static_cast<int32>(stateBufferSize - dataSourceTest->GetNumberOfMemoryBuffers() * sizeof(uint32)));
    }
    if (ok) {
        ok = ((signal1AddressB1 - signal1AddressB0) == stateBufferSize);
    }
    if (ok) {
        ok = ((signal2AddressB1 - signal2AddressB0) == stateBufferSize);
    }
    if (ok) {
        ok = ((signal1AddressB2 - signal1AddressB0) == 2 * stateBufferSize);
    }
    if (ok) {
        ok = ((signal2AddressB2 - signal1AddressB0) == static_cast<int32>(3 * stateBufferSize - dataSourceTest->GetNumberOfMemoryBuffers() * sizeof(uint32)));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}
