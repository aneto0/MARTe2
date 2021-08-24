/**
 * @file RealTimeApplicationTest.cpp
 * @brief Source file for class RealTimeApplicationTest
 * @date 26/02/2016
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

 * @details This source file contains the definition of all the methods for
 * the class RealTimeApplicationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplicationTest.h"

#include "DataSourceI.h"
#include "GAMDataSource.h"
#include "GAMTestHelper.h"
#include "MessageI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeState.h"
#include "RealTimeThread.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class RealTimeApplicationTestScheduler: public GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

RealTimeApplicationTestScheduler    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    void SetThreadToExecute(uint32 tid);

    void ExecuteThreadCycle(uint32 threadId);

    bool Started();

    const char8 *GetStateName();

    virtual void CustomPrepareNextState() {

    }

private:
    uint32 threadId;
    bool started;

};

RealTimeApplicationTestScheduler::RealTimeApplicationTestScheduler() :
        GAMSchedulerI() {
    threadId = 0u;
    started = false;
}

bool RealTimeApplicationTestScheduler::Started() {
    return started;
}
MARTe::ErrorManagement::ErrorType RealTimeApplicationTestScheduler::StartNextStateExecution() {
    started = true;
    ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
    ExecuteSingleCycle(GetSchedulableStates()[realTimeAppT->GetIndex()]->threads[threadId].executables,
                       GetSchedulableStates()[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType RealTimeApplicationTestScheduler::StopCurrentStateExecution() {
    started = false;
    return MARTe::ErrorManagement::NoError;
}

const char8 *RealTimeApplicationTestScheduler::GetStateName() {
    ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
    return GetSchedulableStates()[realTimeAppT->GetIndex()]->name;
}

CLASS_REGISTER(RealTimeApplicationTestScheduler, "1.0")

/**
 * A DataSourceI that fails in the SetConfiguredDatabase
 */
class DataSourceIRealTimeApplicationTestHelper1: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceIRealTimeApplicationTestHelper1    () {

    }

    virtual ~DataSourceIRealTimeApplicationTestHelper1() {

    }

    virtual bool AllocateMemory() {
        return true;
    }

    virtual uint32 GetNumberOfMemoryBuffers() {
        return 1u;
    }

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress) {
        return true;
    }

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction) {
        if (direction == InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool Synchronise() {
        return true;
    }

    virtual bool SetConfiguredDatabase(StructuredDataI & data) {
        return false;
    }

};
CLASS_REGISTER(DataSourceIRealTimeApplicationTestHelper1, "1.0")

/**
 * A DataSourceI that fails in the AllocateMemory
 */
class DataSourceIRealTimeApplicationTestHelper2: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceIRealTimeApplicationTestHelper2    () {

    }

    virtual ~DataSourceIRealTimeApplicationTestHelper2() {

    }

    virtual bool AllocateMemory() {
        return false;
    }

    virtual uint32 GetNumberOfMemoryBuffers() {
        return 1u;
    }

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress) {
        return true;
    }

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction) {
        if (direction == InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = inputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
        bool ret = broker.IsValid();
        if (ret) {
            ret = outputBrokers.Insert(broker);
        }
        return ret;
    }

    virtual bool Synchronise() {
        return true;
    }

};
CLASS_REGISTER(DataSourceIRealTimeApplicationTestHelper2, "1.0")

/**
 * A DataSourceI that fails to AddBrokers
 */
class DataSourceIRealTimeApplicationTestHelper3: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

DataSourceIRealTimeApplicationTestHelper3    () {

    }

    virtual ~DataSourceIRealTimeApplicationTestHelper3() {

    }

    virtual bool AllocateMemory() {
        return true;
    }

    virtual uint32 GetNumberOfMemoryBuffers() {
        return 1u;
    }

    virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
            const uint32 bufferIdx,
            void *&signalAddress) {
        return true;
    }

    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction) {
        if (direction == InputSignals) {
            return "MemoryMapInputBroker";
        }
        return "MemoryMapOutputBroker";
    }

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        return false;
    }

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8* const functionName,
            void * const gamMemPtr) {
        return false;
    }

    virtual bool Synchronise() {
        return true;
    }

};
CLASS_REGISTER(DataSourceIRealTimeApplicationTestHelper3, "1.0")

/**
 * A GAM that fails in the Setup
 */
class GAMRealTimeApplicationTestHelper1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()GAMRealTimeApplicationTestHelper1 () {

    }
    virtual ~GAMRealTimeApplicationTestHelper1() {

    }

    virtual bool Execute() {
        return false;
    }

    virtual bool Setup() {
        return false;
    }
};
CLASS_REGISTER(GAMRealTimeApplicationTestHelper1, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationTest::RealTimeApplicationTest() {

    config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                    Default = 3"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                    Default = 5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add3"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMD = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add4"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAME = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                    Default = 8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                    Default = 13"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add5"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMF = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add6"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMG = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                    Default = 21"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                    Default = 34"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add7"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMH = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add8"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}"
            "+TestMessages = {"
            "    Class = ReferenceContainer"
            "    +MessageState1 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State1\""
            "        }"
            "    }"
            "    +MessageState2 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State2\""
            "        }"
            "    }"
            "    +MessageStart = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StartNextStateExecution"
            "    }"
            "    +MessageStop = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StopCurrentStateExecution"
            "    }"
            "}";

}

RealTimeApplicationTest::~RealTimeApplicationTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool RealTimeApplicationTest::TestConstructor() {
    RealTimeApplication app;

    return app.GetIndex() == 1;
}

bool RealTimeApplicationTest::Init() {
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
        if (!ret) {
        }
    }
    return ret;
}

bool RealTimeApplicationTest::TestInitialise() {

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    if (!app.Initialise(cdb)) {
        return false;
    }

    return true;

}

bool RealTimeApplicationTest::TestInitialiseFalse_NoStatesFound() {

    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoDataFound() {

    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoFunctionsFound() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoSchedulerFound() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestConfigureApplication() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    return true;

}

bool RealTimeApplicationTest::TestConfigureApplication_False_ConfigureAfterInitialisation() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication());
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeApplicationTest::TestConfigureApplication_False_PostConfigureDataSources() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSourceIRealTimeApplicationTestHelper1"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication());
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeApplicationTest::TestConfigureApplication_False_AllocateGAMMemory() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAMRealTimeApplicationTestHelper1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication());
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeApplicationTest::TestConfigureApplication_False_AllocateDataSourceMemory() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSourceIRealTimeApplicationTestHelper2"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication());
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}


bool RealTimeApplicationTest::TestConfigureApplication_False_ConfigureScheduler() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timingsz"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication());
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool RealTimeApplicationTest::TestConfigureApplicationNoInit() {
    if (!Init()) {
        return false;
    }
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }
    cdb.MoveAbsolute("$Fibonacci");
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    builder.ConfigureBeforeInitialisation();
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    builder.Copy(fcdb, dcdb);
    fcdb.MoveToRoot();
    dcdb.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication(fcdb, dcdb)) {
        return false;
    }
    return true;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_AssignBrokersToFunctions() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Default = 1"
            "                    Samples = 3"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_PostConfigureDataSources() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSourceIRealTimeApplicationTestHelper1"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_AllocateGAMMemory() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAMRealTimeApplicationTestHelper1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_AllocateDataSourceMemory() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +DDB2 = {"
            "            Class = DataSourceIRealTimeApplicationTestHelper2"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_AddBrokersToFunctions() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB2 = {"
            "            Class = DataSourceIRealTimeApplicationTestHelper3"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool RealTimeApplicationTest::TestConfigureApplicationAfterInitialisation_False_ConfigureScheduler() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                Signal1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timingz"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        cdb.MoveAbsolute("$Fibonacci");
    }
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    if (ret) {
        builder.ConfigureBeforeInitialisation();
    }
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    if (ret) {
        builder.Copy(fcdb, dcdb);
        fcdb.MoveToRoot();
        dcdb.MoveToRoot();
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ret) {
        ret = app.IsValid();
    }
    if (ret) {
        ret = (!app->ConfigureApplication(fcdb, dcdb));
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

static uint32 GetDsDefault(ReferenceT<DataSourceI> ddb, const char8 *signalName) {
    uint32 signalIndex;
    if (!ddb->GetSignalIndex(signalIndex, signalName)) {
        return false;
    }

    uint32 *addPtr;
    if (!ddb->GetSignalMemoryBuffer(signalIndex, 0, (void*&) addPtr)) {
        return false;
    }
    return *addPtr;

}

bool RealTimeApplicationTest::TestPrepareNextState() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }
    ReferenceT<DataSourceI> ddb1 = app->Find("Data.DDB1");
    ReferenceT<DataSourceI> ddb2 = app->Find("Data.DDB2");
    ReferenceT<GAM1> gams[8];
    gams[0] = app->Find("Functions.GAMA");
    gams[1] = app->Find("Functions.GAMB");
    gams[2] = app->Find("Functions.GAMC");
    gams[3] = app->Find("Functions.GAMD");
    gams[4] = app->Find("Functions.GAME");
    gams[5] = app->Find("Functions.GAMF");
    gams[6] = app->Find("Functions.GAMG");
    gams[7] = app->Find("Functions.GAMH");

    if (!app->PrepareNextState("State1")) {
        return false;
    }

    uint32 testContext1[] = { 1, 1, 1, 1, 1, 1, 0, 0 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext1[i]) {
            return false;
        }
    }

    if ((!ddb1.IsValid()) || (!ddb2.IsValid())) {
        return false;
    }

    if (GetDsDefault(ddb1, "add1") != 1) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 2) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add5") != 8) {
        return false;
    }

    if (GetDsDefault(ddb2, "add6") != 13) {
        return false;
    }

    app->StartNextStateExecution();

    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    if (StringHelper::Compare(scheduler->GetStateName(), "State1") != 0) {
        return false;
    }

    if (!app->PrepareNextState("State2")) {
        return false;
    }

    uint32 testContext2[] = { 2, 2, 2, 2, 1, 1, 1, 1 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext2[i]) {
            return false;
        }
    }

    if (GetDsDefault(ddb1, "add1") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add7") != 21) {
        return false;
    }

    if (GetDsDefault(ddb2, "add8") != 34) {
        return false;
    }

    app->StartNextStateExecution();

    if (StringHelper::Compare(scheduler->GetStateName(), "State2") != 0) {
        return false;
    }
    return true;

}

bool RealTimeApplicationTest::TestPrepareNextState_Message() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }
    ReferenceT<DataSourceI> ddb1 = app->Find("Data.DDB1");
    ReferenceT<DataSourceI> ddb2 = app->Find("Data.DDB2");
    ReferenceT<GAM1> gams[8];
    gams[0] = app->Find("Functions.GAMA");
    gams[1] = app->Find("Functions.GAMB");
    gams[2] = app->Find("Functions.GAMC");
    gams[3] = app->Find("Functions.GAMD");
    gams[4] = app->Find("Functions.GAME");
    gams[5] = app->Find("Functions.GAMF");
    gams[6] = app->Find("Functions.GAMG");
    gams[7] = app->Find("Functions.GAMH");

    ReferenceT<Message> messageState1 = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageState1");
    if (!messageState1.IsValid()) {
        return false;
    }
    MessageI::SendMessage(messageState1, NULL);

    uint32 testContext1[] = { 1, 1, 1, 1, 1, 1, 0, 0 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext1[i]) {
            return false;
        }
    }

    if ((!ddb1.IsValid()) || (!ddb2.IsValid())) {
        return false;
    }

    if (GetDsDefault(ddb1, "add1") != 1) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 2) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add5") != 8) {
        return false;
    }

    if (GetDsDefault(ddb2, "add6") != 13) {
        return false;
    }

    app->StartNextStateExecution();

    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    if (StringHelper::Compare(scheduler->GetStateName(), "State1") != 0) {
        return false;
    }

    ReferenceT<Message> messageState2 = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageState2");
    if (!messageState2.IsValid()) {
        return false;
    }
    MessageI::SendMessage(messageState2, NULL);

    uint32 testContext2[] = { 2, 2, 2, 2, 1, 1, 1, 1 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext2[i]) {
            return false;
        }
    }

    if (GetDsDefault(ddb1, "add1") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add7") != 21) {
        return false;
    }

    if (GetDsDefault(ddb2, "add8") != 34) {
        return false;
    }

    app->StartNextStateExecution();

    if (StringHelper::Compare(scheduler->GetStateName(), "State2") != 0) {
        return false;
    }
    return true;

}

bool RealTimeApplicationTest::TestStartNextStateExecution() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    ReferenceT<Message> messageStart = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageStart");
    if (!messageStart.IsValid()) {
        return false;
    }
    MessageI::SendMessage(messageStart, NULL);
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    return app->GetIndex() == 0;

}

bool RealTimeApplicationTest::TestStartNextStateExecution_Message() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    app->StartNextStateExecution();
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    return app->GetIndex() == 0;

}

bool RealTimeApplicationTest::TestStopCurrentStateExecution() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    app->StartNextStateExecution();
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    app->StopCurrentStateExecution();
    return !scheduler->Started();
}

bool RealTimeApplicationTest::TestStopCurrentStateExecution_Message() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    app->StartNextStateExecution();
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    ReferenceT<Message> messageStop = ObjectRegistryDatabase::Instance()->Find("TestMessages.MessageStop");
    if (!messageStop.IsValid()) {
        return false;
    }
    MessageI::SendMessage(messageStop, NULL);
    return !scheduler->Started();
}

bool RealTimeApplicationTest::TestGetStates() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    ReferenceContainer states;
    if (!app->GetStates(states)) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (!app->GetStates(states)) {
        return false;
    }

    return (states.Size() == 2);
}

bool RealTimeApplicationTest::TestGetIndex() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (app->GetIndex() != 0u) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }

    return (app->GetIndex() == 1u);
}

bool RealTimeApplicationTest::TestCheckSameGamInMoreThreads(){

    StreamString configSameGamInMoreThreads = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    CheckSameGamInMoreThreads = 0"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                    Default = 3"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                    Default = 5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add3"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMD = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add4"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAME = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                    Default = 8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                    Default = 13"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add5"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMF = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add6"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMG = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                    Default = 21"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                    Default = 34"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add7"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMH = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add8"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}"
            "+TestMessages = {"
            "    Class = ReferenceContainer"
            "    +MessageState1 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State1\""
            "        }"
            "    }"
            "    +MessageState2 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State2\""
            "        }"
            "    }"
            "    +MessageStart = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StartNextStateExecution"
            "    }"
            "    +MessageStop = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StopCurrentStateExecution"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    configSameGamInMoreThreads.Seek(0ull);
    StandardParser parser(configSameGamInMoreThreads, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    bool ret= (app.Initialise(cdb));
    if(ret){
        ret=!app.CheckSameGamInMoreThreads();
    }
    return ret;
}


bool RealTimeApplicationTest::TestCheckMultipleProducersWrites(){

    StreamString configMultipleProducersWrites = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    CheckMultipleProducersWrites = 0"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                    Default = 3"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                    Default = 5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add3"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMD = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add4"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAME = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                    Default = 8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                    Default = 13"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add5"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMF = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add6"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMG = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                    Default = 21"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                    Default = 34"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add7"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMH = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add8"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}"
            "+TestMessages = {"
            "    Class = ReferenceContainer"
            "    +MessageState1 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State1\""
            "        }"
            "    }"
            "    +MessageState2 = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = PrepareNextState"
            "        +Parameters = {"
            "             Class = ConfigurationDatabase"
            "             param1 = \"State2\""
            "        }"
            "    }"
            "    +MessageStart = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StartNextStateExecution"
            "    }"
            "    +MessageStop = {"
            "        Class = Message"
            "        Destination = \"Fibonacci\""
            "        Function = StopCurrentStateExecution"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    configMultipleProducersWrites.Seek(0ull);
    StandardParser parser(configMultipleProducersWrites, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    bool ret= (app.Initialise(cdb));
    if(ret){
        ret=!app.CheckMultipleProducersWrites();
    }
    return ret;
}
