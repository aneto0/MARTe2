/**
 * @file GAMTest.cpp
 * @brief Source file for class GAMTest
 * @date 25/04/2016
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
 * the class GAMTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMTest.h"

#include "GAMDataSource.h"
#include "GAMTestHelper.h"
#include "IntrospectionT.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "RealTimeApplication.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct GAMTestStruct2 {
    uint32 Par1;
};

DECLARE_CLASS_MEMBER(GAMTestStruct2, Par1, uint32, "", "");
static const IntrospectionEntry *GAMTestStruct2Entries[] = { &GAMTestStruct2_Par1_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(GAMTestStruct2, GAMTestStruct2Entries)

struct GAMTestStruct1 {
    uint32 Par1;
    float32 Par2;
    GAMTestStruct2 Par3;
};

DECLARE_CLASS_MEMBER(GAMTestStruct1, Par1, uint32, "", "");
DECLARE_CLASS_MEMBER(GAMTestStruct1, Par2, float32, "", "");
DECLARE_CLASS_MEMBER(GAMTestStruct1, Par3, GAMTestStruct2, "", "");
static const IntrospectionEntry *GAMTestStruct1Entries[] = { &GAMTestStruct1_Par1_introspectionEntry, &GAMTestStruct1_Par2_introspectionEntry,
        &GAMTestStruct1_Par3_introspectionEntry, 0 };
DECLARE_STRUCT_INTROSPECTION(GAMTestStruct1, GAMTestStruct1Entries);

class BrokerITestGAMTest1: public BrokerI {
public:CLASS_REGISTER_DECLARATION()

    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 *const functionName,
                      void *gamMemoryAddress);

    virtual bool Execute() {
        return true;
    }
};

bool BrokerITestGAMTest1::Init(SignalDirection direction,
                               DataSourceI &dataSourceIn,
                               const char8 *const functionName,
                               void *gamMemoryAddress) {
    return true;
}
CLASS_REGISTER(BrokerITestGAMTest1, "1.0");

class DataSourceIGAMTest1: public DataSourceI {
public:CLASS_REGISTER_DECLARATION()

    DataSourceIGAMTest1();

    virtual ~DataSourceIGAMTest1();

    virtual bool AllocateMemory();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
                                       const uint32 bufferIdx,
                                       void *&signalAddress);

    virtual const char8* GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool PrepareNextState(const char8 *const currentStateName,
                                  const char8 *const nextStateName);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8 *const functionName,
                                 void *const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8 *const functionName,
                                  void *const gamMemPtr);

    virtual bool Synchronise();

};

DataSourceIGAMTest1::DataSourceIGAMTest1() :
        DataSourceI() {

}

DataSourceIGAMTest1::~DataSourceIGAMTest1() {

}

bool DataSourceIGAMTest1::AllocateMemory() {
    return true;
}

uint32 DataSourceIGAMTest1::GetNumberOfMemoryBuffers() {
    return 0u;
}

bool DataSourceIGAMTest1::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                const uint32 bufferIdx,
                                                void *&signalAddress) {
    return true;
}

const char8* DataSourceIGAMTest1::GetBrokerName(StructuredDataI &data,
                                                const SignalDirection direction) {
    if (direction == InputSignals) {
        return "BrokerITestGAMTest1";
    }
    return "BrokerITestGAMTest1";
}

bool DataSourceIGAMTest1::PrepareNextState(const char8 *const currentStateName,
                                           const char8 *const nextStateName) {
    return true;
}

bool DataSourceIGAMTest1::GetInputBrokers(ReferenceContainer &inputBrokers,
                                          const char8 *const functionName,
                                          void *const gamMemPtr) {
    ReferenceT<BrokerITestGAMTest1> broker("BrokerITestGAMTest1");
    bool ret = broker.IsValid();
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool DataSourceIGAMTest1::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                           const char8 *const functionName,
                                           void *const gamMemPtr) {
    ReferenceT<BrokerITestGAMTest1> broker("BrokerITestGAMTest1");
    bool ret = broker.IsValid();
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool DataSourceIGAMTest1::Synchronise() {
    return false;
}
CLASS_REGISTER(DataSourceIGAMTest1, "1.0");

/**
 * @brief Exposes the protected methods of GAM for testing
 */
class GAMTestGAM1: public GAM {
public:CLASS_REGISTER_DECLARATION()

    GAMTestGAM1();

    virtual bool Setup();

    virtual bool Execute();

    void* GetInputSignalsMemory();

    void* GetOutputSignalsMemory();

    void* GetInputSignalMemory(uint32 signalIdx);

    void* GetOutputSignalMemory(uint32 signalIdx);
};

GAMTestGAM1::GAMTestGAM1() :
        GAM() {
}

void* GAMTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void* GAMTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

void* GAMTestGAM1::GetInputSignalMemory(uint32 signalIdx) {
    return GAM::GetInputSignalMemory(signalIdx);
}

void* GAMTestGAM1::GetOutputSignalMemory(uint32 signalIdx) {
    return GAM::GetOutputSignalMemory(signalIdx);
}

bool GAMTestGAM1::Setup() {
    return true;
}

bool GAMTestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(GAMTestGAM1, "1.0")

class GAMTestScheduler1: public GAMSchedulerI {
public:CLASS_REGISTER_DECLARATION()

    GAMTestScheduler1();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState();

};

GAMTestScheduler1::GAMTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType GAMTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType GAMTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void GAMTestScheduler1::CustomPrepareNextState() {

}

CLASS_REGISTER(GAMTestScheduler1, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
static bool InitialiseGAMEnviroment(const char8 *const config) {
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

static const char8 *const gamTestConfig1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfDimensions = 0"
        "                   NumberOfElements = 1"
        "                   Default = 2"
        "                   Ranges = {{0 0}}"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
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
        "                    Functions = {GAMC GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "                  Defaults = {"
        "                      Signal1.Par3.Par1 = 5"
        "                  }"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "                  Default = 3"
        "               }"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "                  NumberOfElements = 10"
        "                  NumberOfDimensions = 1"
        "                  Ranges = {{0 0} {2 5} {9 9}}"
        "                  Default = {1 2 3 4 5 6 7 8 9 10}"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +DDB2 = {"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DS1"
        "                  Type = uint32"
        "                  Samples = 5"
        "                  Frequency = 10"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DS1"
        "                  Type = uint32"
        "                  Samples = 5"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +DS1 = {"
        "            Class = DataSourceIGAMTest1"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig4 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DS1"
        "                  Type = uint32"
        "                  Samples = 5"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DS1"
        "                  Type = uint32"
        "                  Samples = 5"
        "                  Frequency = 10"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +DS1 = {"
        "            Class = DataSourceIGAMTest1"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig5 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "                  Defaults = {"
        "                      Signal1.Par3.Par1 = 5"
        "                  }"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "                  Default = 3"
        "               }"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "                  NumberOfElements = 10"
        "                  NumberOfDimensions = 1"
        "                  Ranges = {{0 0} {2 5} {9 9}}"
        "                  Default = {1 2 3 4 5 6 7 8 9 10}"
        "               }"
        "               Signal4 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "               }"
        "               Signal4 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "        +DDB2 = {"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig6 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "                  Defaults = {"
        "                      Signal1.Par3.Par1 = 5"
        "                  }"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "                  Default = 3"
        "                  Samples = 10"
        "               }"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "                  NumberOfElements = 10"
        "                  NumberOfDimensions = 1"
        "                  Ranges = {{0 0} {2 5} {9 9}}"
        "                  Default = {1 2 3 4 5 6 7 8 9 10}"
        "               }"
        "               Signal4 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "            OutputSignals = {"
        "               Signal3 = {"
        "                  DataSource = DDB2"
        "                  Type = uint32"
        "               }"
        "               Signal2 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "                  Samples = 10"
        "               }"
        "               Signal1 = {"
        "                  DataSource = DDB2"
        "                  Type = GAMTestStruct1"
        "               }"
        "               Signal4 = {"
        "                  DataSource = DDB1"
        "                  Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal4 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMTestGAM1"
        "            OutputSignals = {"
        "               Signal5 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = DataSourceIGAMTest1"
        "        }"
        "        +DDB2 = {"
        "            Class = DataSourceIGAMTest1"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static const char8 *const gamTestConfig7 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +MyGAMCollection = {"
        "            Class = ReferenceContainer"
        "            +MyGAMCollection2 = {"
        "                Class = ReferenceContainer"
        "                +GAMA = {"
        "                   Class = GAMTestGAM1"
        "                   OutputSignals = {"
        "                        Signal0 = {"
        "                            DataSource = DDB1"
        "                            Type = uint32"
        "                        }"
        "                    }"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
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
        "                    Functions = {GAMB MyGAMCollection}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = GAMTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

bool GAMTest::TestConstructor() {
    GAMTestGAM1 gam;
    bool ret = (gam.GetNumberOfInputSignals() == 0u);
    if (ret) {
        ret = (gam.GetNumberOfOutputSignals() == 0u);
    }
    if (ret) {
        ret = (gam.GetInputSignalsMemory() == NULL_PTR(void*));
    }
    if (ret) {
        ret = (gam.GetOutputSignalsMemory() == NULL_PTR(void*));
    }
    return ret;
}

bool GAMTest::TestInitialise() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);
    if (ret) {
        ReferenceT<GAM> gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    return ret;
}

bool GAMTest::TestAddSignals() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);
    ReferenceT<GAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    ConfigurationDatabase test;
    if (ret) {
        ret = gam->AddSignals(test);
    }
    if (ret) {
        ret = !test.MoveAbsolute("Signals.InputSignals");
    }
    if (ret) {
        ret = test.MoveAbsolute("Signals.OutputSignals");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 1);
    }
    if (ret) {
        ret = test.MoveRelative("Signal0");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 2);
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
        ret = (test.Read("DataSource", value));
    }
    if (ret) {
        ret = (value == "DDB1");
        value = "";
    }
    if (ret) {
        test.MoveToRoot();
        test.Delete("Signals");
    }
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMD");
        ret = gam.IsValid();
    }
    if (ret) {
        ret = gam->AddSignals(test);
    }
    if (ret) {
        ret = !test.MoveAbsolute("Signals.OutputSignals");
    }
    if (ret) {
        ret = test.MoveAbsolute("Signals.InputSignals");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 1);
    }
    if (ret) {
        ret = test.MoveRelative("Signal1");
    }
    if (ret) {
        ret = (test.GetNumberOfChildren() == 6);
    }
    if (ret) {
        ret = (test.Read("Type", value));
    }
    if (ret) {
        ret = (value == "uint32");
        value = "";
    }
    if (ret) {
        ret = (test.Read("DataSource", value));
    }
    if (ret) {
        ret = (value == "DDB1");
        value = "";
    }
    uint32 valueNumber = 1u;
    if (ret) {
        ret = (test.Read("NumberOfDimensions", valueNumber));
    }
    if (ret) {
        ret = (valueNumber == 0u);
    }
    if (ret) {
        ret = (test.Read("NumberOfElements", valueNumber));
    }
    if (ret) {
        ret = (valueNumber == 1u);
    }
    if (ret) {
        ret = (test.Read("Default", valueNumber));
    }
    if (ret) {
        ret = (valueNumber == 2u);
    }
    if (ret) {
        Matrix<uint32> rangesMat(1u, 2u);
        if (ret) {
            ret = test.Read("Ranges", rangesMat);
        }
        uint32 rangesStart = 1u;
        uint32 rangesEnd = 1u;
        if (ret) {
            rangesStart = rangesMat[0][0u];
            rangesEnd = rangesMat[0][1u];
        }
        if (ret) {
            ret = (rangesStart == 0u);
        }
        if (ret) {
            ret = (rangesEnd == 0u);
        }
    }
    return ret;
}

bool GAMTest::TestSetConfiguredDatabase() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);
    ReferenceT<GAM> gam;
    if (ret) {
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }
    return ret;
}

bool GAMTest::TestGetNumberOfInputSignals() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig2);

    const uint32 numberOfGAMs = 4u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    uint32 numberOfInputs[] = { 5, 1, 1, 0 };

    uint32 n;
    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            ret = (gam->GetNumberOfInputSignals() == numberOfInputs[n]);
        }
    }
    return ret;
}

bool GAMTest::TestGetNumberOfOutputSignals() {
    const uint32 numberOfGAMs = 4u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    uint32 numberOfOutputs[] = { 1, 5, 0, 1 };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            ret = (gam->GetNumberOfOutputSignals() == numberOfOutputs[n]);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalName() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    StreamString name;
                    ret = (gam->GetSignalName(InputSignals, signalIdx, name));
                    if (ret) {
                        ret = (name == inputSignalsNames[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        StreamString name;
                        ret = (gam->GetSignalName(OutputSignals, signalIdx, name));
                        if (ret) {
                            ret = (name == outputSignalsNames[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        if (ret) {
            StreamString name;
            ret = !gam->GetSignalName(InputSignals, 10000, name);
        }
        if (ret) {
            StreamString name;
            ret = !gam->GetSignalName(OutputSignals, 10000, name);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalIndex() {
    bool ret = TestGetSignalName();
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 signalIdx;
            ret = !gam->GetSignalIndex(InputSignals, signalIdx, "DoesNotExist");
        }
        if (ret) {
            uint32 signalIdx;
            ret = !gam->GetSignalIndex(OutputSignals, signalIdx, "DoesNotExist");
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalDataSourceName() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    const char8 *const inputSignalsDataSourceNames[][maxNumberOfSignalNames] = { { "DDB2", "DDB2", "DDB2", "DDB1", "DDB2" }, { "DDB1", NULL, NULL, NULL,
    NULL}, {"DDB1", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsDataSourceNames[][maxNumberOfSignalNames] = { { "DDB1", NULL, NULL, NULL, NULL}, {"DDB2", "DDB1", "DDB2", "DDB2", "DDB2"}, {
    NULL, NULL, NULL, NULL, NULL}, {"DDB1", NULL, NULL, NULL, NULL}};

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    StreamString name;
                    ret = (gam->GetSignalDataSourceName(InputSignals, signalIdx, name));
                    if (ret) {
                        ret = (name == inputSignalsDataSourceNames[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        StreamString name;
                        ret = (gam->GetSignalDataSourceName(OutputSignals, signalIdx, name));
                        if (ret) {
                            ret = (name == outputSignalsDataSourceNames[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        if (ret) {
            StreamString name;
            ret = !gam->GetSignalDataSourceName(InputSignals, 10000, name);
        }
        if (ret) {
            StreamString name;
            ret = !gam->GetSignalDataSourceName(OutputSignals, 10000, name);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalType() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    const char8 *const inputSignalsType[][maxNumberOfSignalNames] = { { "uint32", "float32", "uint32", "uint32", "uint32" }, { "uint32", NULL, NULL, NULL,
    NULL}, {"uint32", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsType[][maxNumberOfSignalNames] = { { "uint32", NULL, NULL, NULL, NULL}, {"uint32", "uint32", "uint32", "float32", "uint32"}, {NULL, NULL, NULL, NULL, NULL}, {
    "uint32", NULL, NULL, NULL, NULL}};

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    TypeDescriptor desc;
                    desc = gam->GetSignalType(InputSignals, signalIdx);
                    ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName(inputSignalsType[n][i]));
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        TypeDescriptor desc;
                        desc = gam->GetSignalType(OutputSignals, signalIdx);
                        ret = (desc == TypeDescriptor::GetTypeDescriptorFromTypeName(outputSignalsType[n][i]));
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        if (ret) {
            TypeDescriptor desc = gam->GetSignalType(InputSignals, 10000);
            ret = (desc == InvalidType);
        }
        if (ret) {
            TypeDescriptor desc = gam->GetSignalType(OutputSignals, 10000);
            ret = (desc == InvalidType);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalNumberOfDimensions() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsNumberOfDimensions[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    uint32 outputSignalsNumberOfDimensions[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfDimensions = 2u;
                    ret = gam->GetSignalNumberOfDimensions(InputSignals, signalIdx, numberOfDimensions);
                    if (ret) {
                        ret = (numberOfDimensions == inputSignalsNumberOfDimensions[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfDimensions = 2u;
                        ret = gam->GetSignalNumberOfDimensions(OutputSignals, signalIdx, numberOfDimensions);
                        if (ret) {
                            ret = (numberOfDimensions == outputSignalsNumberOfDimensions[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 numberOfDimensions;
        if (ret) {
            ret = !gam->GetSignalNumberOfDimensions(InputSignals, 10000, numberOfDimensions);
        }
        if (ret) {
            ret = !gam->GetSignalNumberOfDimensions(OutputSignals, 10000, numberOfDimensions);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalNumberOfElements() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsNumberOfElements[][maxNumberOfSignalNames] = { { 1, 1, 1, 1, 10 }, { 1, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    uint32 outputSignalsNumberOfElements[][maxNumberOfSignalNames] = { { 1, 0, 0, 0, 0 }, { 10, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0 }, { 1, 0, 0, 0, 0 } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfElements = 2u;
                    ret = gam->GetSignalNumberOfElements(InputSignals, signalIdx, numberOfElements);
                    if (ret) {
                        ret = (numberOfElements == inputSignalsNumberOfElements[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfElements = 2u;
                        ret = gam->GetSignalNumberOfElements(OutputSignals, signalIdx, numberOfElements);
                        if (ret) {
                            ret = (numberOfElements == outputSignalsNumberOfElements[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 numberOfElements;
        if (ret) {
            ret = !gam->GetSignalNumberOfElements(InputSignals, 10000, numberOfElements);
        }
        if (ret) {
            ret = !gam->GetSignalNumberOfElements(OutputSignals, 10000, numberOfElements);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalByteSize() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsByteSize[][maxNumberOfSignalNames] = { { 4, 4, 4, 4, 24 }, { 4, 0, 0, 0, 0 }, { 4, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    uint32 outputSignalsByteSize[][maxNumberOfSignalNames] = { { 4, 0, 0, 0, 0 }, { 40, 4, 4, 4, 4 }, { 0, 0, 0, 0, 0 }, { 4, 0, 0, 0, 0 } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 byteSize = 2u;
                    ret = gam->GetSignalByteSize(InputSignals, signalIdx, byteSize);
                    if (ret) {
                        ret = (byteSize == inputSignalsByteSize[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 byteSize = 2u;
                        ret = gam->GetSignalByteSize(OutputSignals, signalIdx, byteSize);
                        if (ret) {
                            ret = (byteSize == outputSignalsByteSize[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 numberOfBytes;
        if (ret) {
            ret = !gam->GetSignalByteSize(InputSignals, 10000, numberOfBytes);
        }
        if (ret) {
            ret = !gam->GetSignalByteSize(OutputSignals, 10000, numberOfBytes);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalDefaultValue() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig2);

    ReferenceT<GAM> gam;
    if (ret) {
        StreamString gamFullName;
        gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gam.IsValid();
    }

    uint32 signalIdx;
    uint32 defaultValueUInt32;
    if (ret) {
        ret = gam->GetSignalIndex(InputSignals, signalIdx, "Signal1.Par1");
    }
    if (ret) {
        ret = !gam->GetSignalDefaultValue(InputSignals, signalIdx, defaultValueUInt32);
    }
    if (ret) {
        ret = gam->GetSignalIndex(InputSignals, signalIdx, "Signal1.Par3.Par1");
    }
    if (ret) {
        ret = gam->GetSignalDefaultValue(InputSignals, signalIdx, defaultValueUInt32);
    }
    if (ret) {
        ret = (defaultValueUInt32 == 5u);
    }
    if (ret) {
        ret = gam->GetSignalIndex(InputSignals, signalIdx, "Signal2");
    }
    if (ret) {
        ret = gam->GetSignalDefaultValue(InputSignals, signalIdx, defaultValueUInt32);
    }
    if (ret) {
        ret = (defaultValueUInt32 == 3u);
    }
    int32 defaultValueInt32Arr[10];
    if (ret) {
        ret = gam->GetSignalIndex(InputSignals, signalIdx, "Signal3");
    }
    if (ret) {
        ret = gam->GetSignalDefaultValue(InputSignals, signalIdx, defaultValueInt32Arr);
    }
    if (ret) {
        int32 i;
        for (i = 0; (i < 10) && ret; i++) {
            ret = (defaultValueInt32Arr[i] == (i + 1));
        }
    }
    if (ret) {
        ret = !gam->GetSignalDefaultValue(InputSignals, 1000, defaultValueInt32Arr);
    }
    if (ret) {
        ret = !gam->GetSignalDefaultValue(OutputSignals, 1000, defaultValueInt32Arr);
    }
    return ret;
}

bool GAMTest::TestGetSignalNumberOfByteOffsets() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsNumberOfByteOffsets[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    uint32 outputSignalsNumberOfByteOffsets[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfByteOffsets = 2u;
                    ret = gam->GetSignalNumberOfByteOffsets(InputSignals, signalIdx, numberOfByteOffsets);
                    if (ret) {
                        ret = (numberOfByteOffsets == inputSignalsNumberOfByteOffsets[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfByteOffsets = 2u;
                        ret = gam->GetSignalNumberOfByteOffsets(OutputSignals, signalIdx, numberOfByteOffsets);
                        if (ret) {
                            ret = (numberOfByteOffsets == outputSignalsNumberOfByteOffsets[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 numberOfBytesOffsets;
        if (ret) {
            ret = !gam->GetSignalNumberOfByteOffsets(InputSignals, 10000, numberOfBytesOffsets);
        }
        if (ret) {
            ret = !gam->GetSignalNumberOfByteOffsets(OutputSignals, 10000, numberOfBytesOffsets);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalByteOffsetInfo() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const uint32 maxNumberOfByteOffsetInfos = 3u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsStartIdx[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 8, 36 } },
            { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0,
                    0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 outputSignalsStartIdx[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
            { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0,
                    0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 inputSignalsSize[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 4, 16, 4 } }, { {
            0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 outputSignalsSize[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { {
            0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfByteOffsets = 2u;
                    ret = gam->GetSignalNumberOfByteOffsets(InputSignals, signalIdx, numberOfByteOffsets);
                    uint32 j;
                    for (j = 0u; (j < numberOfByteOffsets) && (ret); j++) {
                        uint32 startIdx;
                        uint32 byteSize;
                        ret = gam->GetSignalByteOffsetInfo(InputSignals, signalIdx, j, startIdx, byteSize);
                        if (ret) {
                            ret = (startIdx == inputSignalsStartIdx[n][i][j]);
                        }
                        if (ret) {
                            ret = (byteSize == inputSignalsSize[n][i][j]);
                        }
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfByteOffsets = 2u;
                        ret = gam->GetSignalNumberOfByteOffsets(OutputSignals, signalIdx, numberOfByteOffsets);
                        uint32 j;
                        for (j = 0u; (j < numberOfByteOffsets) && (ret); j++) {
                            uint32 startIdx;
                            uint32 byteSize;
                            ret = gam->GetSignalByteOffsetInfo(OutputSignals, signalIdx, j, startIdx, byteSize);
                            if (ret) {
                                ret = (startIdx == outputSignalsStartIdx[n][i][j]);
                            }
                            if (ret) {
                                ret = (byteSize == outputSignalsSize[n][i][j]);
                            }
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 startIdx;
        uint32 byteSize;
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(InputSignals, 4, 1000, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(OutputSignals, 4, 1000, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(InputSignals, 10000, 0, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(OutputSignals, 10000, 0, startIdx, byteSize);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalNumberOfRanges() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsNumberOfRanges[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 3 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };
    uint32 outputSignalsNumberOfRanges[][maxNumberOfSignalNames] = { { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfRanges = 2u;
                    ret = gam->GetSignalNumberOfRanges(InputSignals, signalIdx, numberOfRanges);
                    if (ret) {
                        ret = (numberOfRanges == inputSignalsNumberOfRanges[n][i]);
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfRanges = 2u;
                        ret = gam->GetSignalNumberOfRanges(OutputSignals, signalIdx, numberOfRanges);
                        if (ret) {
                            ret = (numberOfRanges == outputSignalsNumberOfRanges[n][i]);
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 numberOfBytesOffsets;
        if (ret) {
            ret = !gam->GetSignalNumberOfRanges(InputSignals, 10000, numberOfBytesOffsets);
        }
        if (ret) {
            ret = !gam->GetSignalNumberOfRanges(OutputSignals, 10000, numberOfBytesOffsets);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalRangesInfo() {
    const uint32 numberOfGAMs = 4u;
    const uint32 maxNumberOfSignalNames = 5u;
    const uint32 maxNumberOfByteOffsetInfos = 3u;
    const char8 *const gamNames[] = { "GAMA", "GAMB", "GAMC", "GAMD" };
    const char8 *const inputSignalsNames[][maxNumberOfSignalNames] = { { "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1", "Signal2", "Signal3" }, {
            "Signal0",
            NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}, {NULL, NULL, NULL, NULL, NULL}};
    const char8 *const outputSignalsNames[][maxNumberOfSignalNames] = { { "Signal0", NULL, NULL, NULL, NULL}, {"Signal3", "Signal2", "Signal1.Par1", "Signal1.Par2", "Signal1.Par3.Par1"}, {NULL,
    NULL, NULL, NULL, NULL}, {"Signal4", NULL, NULL, NULL, NULL}};
    uint32 inputSignalsStartIdx[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 2, 9 } }, {
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, {
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 outputSignalsStartIdx[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } },
            { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0,
                    0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 inputSignalsSize[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 5, 9 } }, { {
            0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };
    uint32 outputSignalsSize[][maxNumberOfSignalNames][maxNumberOfByteOffsetInfos] = { { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { {
            0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } }, { { 0, 0, 0 },
            { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } } };

    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    uint32 n;

    for (n = 0u; (n < numberOfGAMs) && (ret); n++) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gamFullName.Printf("Application1.Functions.%s", gamNames[n]);
            gam = ObjectRegistryDatabase::Instance()->Find(gamFullName.Buffer());
            ret = gam.IsValid();
        }
        if (ret) {
            uint32 i;
            uint32 signalIdx;
            for (i = 0u; (i < maxNumberOfSignalNames) && (ret); i++) {
                ret = gam->GetSignalIndex(InputSignals, signalIdx, inputSignalsNames[n][i]);
                if (inputSignalsNames[n][i] == NULL) {
                    ret = !ret;
                }
                else {
                    uint32 numberOfRanges = 2u;
                    ret = gam->GetSignalNumberOfRanges(InputSignals, signalIdx, numberOfRanges);
                    uint32 j;
                    for (j = 0u; (j < numberOfRanges) && (ret); j++) {
                        uint32 startIdx;
                        uint32 byteSize;
                        ret = gam->GetSignalRangesInfo(InputSignals, signalIdx, j, startIdx, byteSize);
                        if (ret) {
                            ret = (startIdx == inputSignalsStartIdx[n][i][j]);
                        }
                        if (ret) {
                            ret = (byteSize == inputSignalsSize[n][i][j]);
                        }
                    }
                }
                if (ret) {
                    ret = gam->GetSignalIndex(OutputSignals, signalIdx, outputSignalsNames[n][i]);
                    if (outputSignalsNames[n][i] == NULL) {
                        ret = !ret;
                    }
                    else {
                        uint32 numberOfRanges = 2u;
                        ret = gam->GetSignalNumberOfRanges(OutputSignals, signalIdx, numberOfRanges);
                        uint32 j;
                        for (j = 0u; (j < numberOfRanges) && (ret); j++) {
                            uint32 startIdx;
                            uint32 byteSize;
                            ret = gam->GetSignalRangesInfo(OutputSignals, signalIdx, j, startIdx, byteSize);
                            if (ret) {
                                ret = (startIdx == outputSignalsStartIdx[n][i][j]);
                            }
                            if (ret) {
                                ret = (byteSize == outputSignalsSize[n][i][j]);
                            }
                        }
                    }
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAM> gam;
        if (ret) {
            StreamString gamFullName;
            gam = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
            ret = gam.IsValid();
        }
        uint32 startIdx;
        uint32 byteSize;
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(InputSignals, 4, 1000, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(OutputSignals, 4, 1000, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(InputSignals, 10000, 0, startIdx, byteSize);
        }
        if (ret) {
            ret = !gam->GetSignalByteOffsetInfo(OutputSignals, 10000, 0, startIdx, byteSize);
        }
    }
    return ret;
}

bool GAMTest::TestGetSignalNumberOfSamples() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig3);

    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    uint32 signalIdx;
    uint32 numberOfSamples;
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamA->GetSignalNumberOfSamples(InputSignals, signalIdx, numberOfSamples);
    }
    if (ret) {
        ret = (numberOfSamples == 1);
    }
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamA->GetSignalNumberOfSamples(InputSignals, signalIdx, numberOfSamples);
    }
    if (ret) {
        ret = (numberOfSamples == 5u);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamB->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
    }
    if (ret) {
        ret = (numberOfSamples == 1);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamB->GetSignalNumberOfSamples(OutputSignals, signalIdx, numberOfSamples);
    }
    if (ret) {
        ret = (numberOfSamples == 5u);
    }

    return ret;
}

bool GAMTest::TestGetSignalFrequency_Input() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig3);

    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    uint32 signalIdx;
    float32 frequency;
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamA->GetSignalFrequency(InputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamA->GetSignalFrequency(InputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == 10);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamB->GetSignalFrequency(OutputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamB->GetSignalFrequency(OutputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }

    return ret;
}

bool GAMTest::TestGetSignalFrequency_Output() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig4);

    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    uint32 signalIdx;
    float32 frequency;
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamA->GetSignalFrequency(InputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }
    if (ret) {
        ret = gamA->GetSignalIndex(InputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamA->GetSignalFrequency(InputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal0");
    }
    if (ret) {
        ret = gamB->GetSignalFrequency(OutputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == -1);
    }
    if (ret) {
        ret = gamB->GetSignalIndex(OutputSignals, signalIdx, "Signal1");
    }
    if (ret) {
        ret = gamB->GetSignalFrequency(OutputSignals, signalIdx, frequency);
    }
    if (ret) {
        ret = (frequency == 10);
    }

    return ret;
}

bool GAMTest::TestAllocateInputSignalsMemory() {
    //Tested implicitly
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);
    ReferenceT<GAM> gamB;
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }
    if (ret) {
        //Cannot be called twice so it should fail.
        ret = !gamB->AllocateInputSignalsMemory();
    }
    return ret;
}

bool GAMTest::TestAllocateOutputSignalsMemory() {
    //Tested implicitly
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);
    ReferenceT<GAM> gamA;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        //Cannot be called twice so it should fail.
        ret = !gamA->AllocateOutputSignalsMemory();
    }
    return ret;

}

bool GAMTest::TestGetInputSignalsMemory() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);

    ReferenceT<GAMTestGAM1> gamA;
    ReferenceT<GAMTestGAM1> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }
    if (ret) {
        ret = (gamA->GetInputSignalsMemory() == NULL_PTR(void*));
    }
    if (ret) {
        ret = (gamB->GetInputSignalsMemory() != NULL_PTR(void*));
    }
    return ret;
}

bool GAMTest::TestGetOutputSignalsMemory() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig1);

    ReferenceT<GAMTestGAM1> gamA;
    ReferenceT<GAMTestGAM1> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }
    if (ret) {
        ret = (gamA->GetOutputSignalsMemory() != NULL_PTR(void*));
    }
    if (ret) {
        ret = (gamB->GetOutputSignalsMemory() == NULL_PTR(void*));
    }
    return ret;
}

bool GAMTest::TestGetInputSignalMemory() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig5);

    ReferenceT<GAMTestGAM1> gamA;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    char8 *gamAMemoryAddress = reinterpret_cast<char8*>(gamA->GetInputSignalsMemory());
    uint32 numberOfInputs = 6;
    uint32 offsets[] = { sizeof(uint32), sizeof(float32), sizeof(uint32), sizeof(uint32), 6 * sizeof(uint32), sizeof(uint32) };
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        for (i = 0; (i < numberOfInputs) && (ret); i++) {
            ret = (gamA->GetInputSignalMemory(i) == (gamAMemoryAddress + offset));
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (gamA->GetInputSignalMemory(1000) == NULL_PTR(void*));
    }

    return ret;
}

bool GAMTest::TestGetOutputSignalMemory() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig5);

    ReferenceT<GAMTestGAM1> gamB;
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    char8 *gamBMemoryAddress = reinterpret_cast<char8*>(gamB->GetOutputSignalsMemory());
    uint32 numberOfOutputs = 6;
    uint32 offsets[] = { 10 * sizeof(uint32), sizeof(uint32), sizeof(uint32), sizeof(float32), sizeof(uint32), sizeof(uint32) };
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        for (i = 0; (i < numberOfOutputs) && (ret); i++) {
            ret = (gamB->GetOutputSignalMemory(i) == (gamBMemoryAddress + offset));
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (gamB->GetOutputSignalMemory(1000) == NULL_PTR(void*));
    }

    return ret;
}

bool GAMTest::TestGetInputSignalMemory_Samples() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig6);

    ReferenceT<GAMTestGAM1> gamA;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }

    char8 *gamAMemoryAddress = reinterpret_cast<char8*>(gamA->GetInputSignalsMemory());
    uint32 numberOfInputs = 6;
    uint32 offsets[] = { sizeof(uint32), sizeof(float32), sizeof(uint32), sizeof(uint32) * 10, 6 * sizeof(uint32), sizeof(uint32) };
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        for (i = 0; (i < numberOfInputs) && (ret); i++) {
            ret = (gamA->GetInputSignalMemory(i) == (gamAMemoryAddress + offset));
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (gamA->GetInputSignalMemory(1000) == NULL_PTR(void*));
    }

    return ret;
}

bool GAMTest::TestGetOutputSignalMemory_Samples() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig6);

    ReferenceT<GAMTestGAM1> gamB;
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    char8 *gamBMemoryAddress = reinterpret_cast<char8*>(gamB->GetOutputSignalsMemory());
    uint32 numberOfOutputs = 6;
    uint32 offsets[] = { 10 * sizeof(uint32), sizeof(uint32) * 10, sizeof(uint32), sizeof(float32), sizeof(uint32), sizeof(uint32) };
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        for (i = 0; (i < numberOfOutputs) && (ret); i++) {
            ret = (gamB->GetOutputSignalMemory(i) == (gamBMemoryAddress + offset));
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (gamB->GetOutputSignalMemory(1000) == NULL_PTR(void*));
    }

    return ret;
}

bool GAMTest::TestAddInputBrokers() {
    GAMTestGAM1 gam;
    ReferenceT<BrokerITestGAMTest1> broker1("BrokerITestGAMTest1");
    ReferenceT<BrokerITestGAMTest1> broker2("BrokerITestGAMTest1");
    ReferenceT<BrokerITestGAMTest1> broker3("BrokerITestGAMTest1");
    ReferenceContainer twoBrokers;
    twoBrokers.Insert(broker1);
    twoBrokers.Insert(broker2);
    ReferenceContainer oneBroker;
    oneBroker.Insert(broker3);
    bool ret = gam.AddInputBrokers(twoBrokers);
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam.GetInputBrokers(inputBrokers);
        if (ret) {
            ret = (inputBrokers.Size() == 2u);
        }
        if (ret) {
            ret = (inputBrokers.Get(0) == broker1);
        }
        if (ret) {
            ret = (inputBrokers.Get(1) == broker2);
        }
    }
    ret = gam.AddInputBrokers(oneBroker);
    if (ret) {
        ReferenceContainer inputBrokers;
        ret = gam.GetInputBrokers(inputBrokers);
        if (ret) {
            ret = (inputBrokers.Size() == 3u);
        }
        if (ret) {
            ret = (inputBrokers.Get(0) == broker1);
        }
        if (ret) {
            ret = (inputBrokers.Get(1) == broker2);
        }
        if (ret) {
            ret = (inputBrokers.Get(2) == broker3);
        }
    }

    return ret;
}

bool GAMTest::TestAddInputBrokers_InvalidBroker() {
    GAMTestGAM1 gam;
    ReferenceT<BrokerITestGAMTest1> broker1("BrokerITestGAMTest1");
    ReferenceT<GAMTestGAM1> broker2("GAMTestGAM1");
    ReferenceContainer twoBrokers;
    twoBrokers.Insert(broker1);
    twoBrokers.Insert(broker2);
    return !gam.AddInputBrokers(twoBrokers);
}

bool GAMTest::TestAddOutputBrokers() {
    GAMTestGAM1 gam;
    ReferenceT<BrokerITestGAMTest1> broker1("BrokerITestGAMTest1");
    ReferenceT<BrokerITestGAMTest1> broker2("BrokerITestGAMTest1");
    ReferenceT<BrokerITestGAMTest1> broker3("BrokerITestGAMTest1");
    ReferenceContainer twoBrokers;
    twoBrokers.Insert(broker1);
    twoBrokers.Insert(broker2);
    ReferenceContainer oneBroker;
    oneBroker.Insert(broker3);
    bool ret = gam.AddOutputBrokers(twoBrokers);
    if (ret) {
        ReferenceContainer outputBrokers;
        ret = gam.GetInputBrokers(outputBrokers);
        if (ret) {
            ret = (outputBrokers.Size() == 2u);
        }
        if (ret) {
            ret = (outputBrokers.Get(0) == broker1);
        }
        if (ret) {
            ret = (outputBrokers.Get(1) == broker2);
        }
    }
    ret = gam.AddOutputBrokers(oneBroker);
    if (ret) {
        ReferenceContainer outputBrokers;
        ret = gam.GetOutputBrokers(outputBrokers);
        if (ret) {
            ret = (outputBrokers.Size() == 3u);
        }
        if (ret) {
            ret = (outputBrokers.Get(0) == broker1);
        }
        if (ret) {
            ret = (outputBrokers.Get(1) == broker2);
        }
        if (ret) {
            ret = (outputBrokers.Get(2) == broker3);
        }
    }

    return ret;
}

bool GAMTest::TestAddOutputBrokers_InvalidBroker() {
    GAMTestGAM1 gam;
    ReferenceT<BrokerITestGAMTest1> broker1("BrokerITestGAMTest1");
    ReferenceT<GAMTestGAM1> broker2("GAMTestGAM1");
    ReferenceContainer twoBrokers;
    twoBrokers.Insert(broker1);
    twoBrokers.Insert(broker2);
    return !gam.AddOutputBrokers(twoBrokers);
}

bool GAMTest::TestSortBrokers() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig2);
    if (ret) {
        ReferenceT<GAMTestGAM1> testGAM = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = testGAM.IsValid();
        if (ret) {
            ReferenceContainer inputBrokers;
            ret = testGAM->GetInputBrokers(inputBrokers);
            if (ret) {
                ReferenceT<BrokerI> element = inputBrokers.Get(0);
                ret = element.IsValid();
                if (ret) {
                    StreamString dsName = element->GetOwnerDataSourceName();
                    ret = dsName == "DDB2";
                }
                if(ret){
                    element = inputBrokers.Get(1);
                    ret = element.IsValid();
                    if (ret) {
                        StreamString dsName = element->GetOwnerDataSourceName();
                        ret = dsName == "DDB1";
                    }
                }
            }
            if(ret){
                ReferenceContainer outputBrokers;
                ret = testGAM->GetOutputBrokers(outputBrokers);
                ReferenceT<BrokerI> element = outputBrokers.Get(0);
                ret = element.IsValid();
                if (ret) {
                    StreamString dsName = element->GetOwnerDataSourceName();
                    ret = dsName == "DDB1";
                }
            }
        }
    }
    if (ret) {
        ReferenceT<GAMTestGAM1> testGAM = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = testGAM.IsValid();
        if (ret) {
            ReferenceContainer outputBrokers;
            ret = testGAM->GetOutputBrokers(outputBrokers);
            if (ret) {
                ReferenceT<BrokerI> element = outputBrokers.Get(0);
                ret = element.IsValid();
                if (ret) {
                    StreamString dsName = element->GetOwnerDataSourceName();
                    ret = dsName == "DDB2";
                }
                if(ret){
                    element = outputBrokers.Get(1);
                    ret = element.IsValid();
                    if (ret) {
                        StreamString dsName = element->GetOwnerDataSourceName();
                        ret = dsName == "DDB1";
                    }
                }
            }
            if(ret){
                ReferenceContainer inputBrokers;
                ret = testGAM->GetInputBrokers(inputBrokers);
                ReferenceT<BrokerI> element = inputBrokers.Get(0);
                ret = element.IsValid();
                if (ret) {
                    StreamString dsName = element->GetOwnerDataSourceName();
                    ret = dsName == "DDB1";
                }
            }
        }
    }
    return ret;
}

bool GAMTest::TestGetInputBrokers() {
    return TestAddInputBrokers();
}

bool GAMTest::TestGetOutputBrokers() {
    return TestAddOutputBrokers();
}

bool GAMTest::TestGetQualifiedName() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig7);

    ReferenceT<GAMTestGAM1> gamA;
    ReferenceT<GAMTestGAM1> gamB;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.MyGAMCollection.MyGAMCollection2.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        StreamString gamFullName;
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }
    if (ret) {
        StreamString qualifiedName;
        ret = gamA->GetQualifiedName(qualifiedName);
        if (ret) {
            ret = qualifiedName = "MyGAMCollection.MyGAMCollection2.GAMA";
        }
    }
    if (ret) {
        StreamString qualifiedName;
        ret = gamB->GetQualifiedName(qualifiedName);
        if (ret) {
            ret = qualifiedName = "GAMB";
        }
    }

    return ret;
}

#include <stdio.h>
bool GAMTest::TestExportData() {
    bool ret = InitialiseGAMEnviroment(gamTestConfig5);

    ReferenceT<GAMTestGAM1> gamA;
    if (ret) {
        StreamString gamFullName;
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    ConfigurationDatabase cdb;
    ret = gamA->ExportData(cdb);
    uint32 testi;
    StreamString test;
    ret &= cdb.Read("Name", test);
    ret &= (test == "GAMA");
    test = "";
    ret &= cdb.Read("Class", test);
    ret &= (test == "GAMTestGAM1");
    test = "";
    ret &= cdb.MoveAbsolute("InputSignals");
    ret &= cdb.Read("Signal1.Par1", testi);
    ret &= cdb.MoveAbsolute("OutputSignals");
    ret &= cdb.Read("Signal0", testi);
    return ret;
}
