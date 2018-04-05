/**
 * @file RealTimeLoaderTest.cpp
 * @brief Source file for class RealTimeLoaderTest
 * @date 05/04/2018
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
 * the class RealTimeLoaderTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "RealTimeApplication.h"
#include "RealTimeLoader.h"
#include "RealTimeLoaderTest.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class RealTimeLoaderTestMessageObject1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()RealTimeLoaderTestMessageObject1() {
        using namespace MARTe;
        ReferenceT<RegisteredMethodsMessageFilter> registeredMethodsMessageFilter(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        registeredMethodsMessageFilter->SetDestination(this);

        if (registeredMethodsMessageFilter.IsValid()) {
            InstallMessageFilter(registeredMethodsMessageFilter);
        }
        functionCalled = false;
    }

    MARTe::ErrorManagement::ErrorType Callback() {
        functionCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    bool functionCalled;
};
CLASS_REGISTER(RealTimeLoaderTestMessageObject1, "1.0")
CLASS_METHOD_REGISTER(RealTimeLoaderTestMessageObject1, Callback)

/**
 * Dummy scheduler for the tests
 */
class RealTimeLoaderTestScheduler: public MARTe::GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

RealTimeLoaderTestScheduler    () {
        startCalled = false;
        stopCalled = false;
    }

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution() {
        startCalled = true;
        return MARTe::ErrorManagement::NoError;
    }

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution() {
        stopCalled = true;
        return MARTe::ErrorManagement::NoError;

    }

    virtual void CustomPrepareNextState() {

    }

    bool startCalled;
    bool stopCalled;

};
CLASS_REGISTER(RealTimeLoaderTestScheduler, "1.0")

/**
 * Dummy GAM
 */
/**
 * A GAM that fails in the Setup
 */
class RealTimeLoaderTestGAM: public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()RealTimeLoaderTestGAM () {

    }
    virtual ~RealTimeLoaderTestGAM() {

    }

    virtual bool Execute() {
        return true;
    }

    virtual bool Setup() {
        return true;
    }
};
CLASS_REGISTER(RealTimeLoaderTestGAM, "1.0")

//Default configuration
static const MARTe::char8 * const config1 = "$RTApp = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = RealTimeLoaderTestGAM"
        "            OutputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = RealTimeLoaderTestGAM"
        "            InputSignals = {"
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
        "                    Functions = {GAMA, GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeLoaderTestScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+B={"
        "   Class = RealTimeLoaderTestMessageObject1"
        "}";

//Wrong configuration
static const MARTe::char8 * const config2 = "$RTApp = {"
        "    Class = ealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = RealTimeLoaderTestGAM"
        "            OutputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = RealTimeLoaderTestGAM"
        "            InputSignals = {"
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
        "                    Functions = {GAMA, GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeLoaderTestScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool RealTimeLoaderTest::TestConstructor() {
    using namespace MARTe;
    RealTimeLoader l;
    return true;
}

bool RealTimeLoaderTest::TestConfigure() {
    using namespace MARTe;
    StreamString config = config1;
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    RealTimeLoader l;
    bool ok = l.Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool RealTimeLoaderTest::TestConfigure_False_FailedConfiguration() {
    using namespace MARTe;
    StreamString config = config2;
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    RealTimeLoader l;
    bool ok = !l.Configure(params, config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool RealTimeLoaderTest::TestStart_FirstState() {
    using namespace MARTe;
    StreamString config = config1;
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("FirstState", "State1");
    RealTimeLoader l;
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = l.Start();
    }
    ReferenceT<RealTimeLoaderTestScheduler> scheduler;
    if (ok) {
        scheduler = ObjectRegistryDatabase::Instance()->Find("RTApp.Scheduler");
        ok = scheduler.IsValid();
        if (ok) {
            ok = scheduler->startCalled;
        }
    }
    if (ok) {
        ok = l.Stop();
        if (ok) {
            ok = scheduler->stopCalled;
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool RealTimeLoaderTest::TestStart_Message() {

    using namespace MARTe;
    StreamString config = config1;
    ConfigurationDatabase params;
    params.Write("Parser", "cdb");
    params.Write("MessageDestination", "B");
    params.Write("MessageFunction", "Callback");
    RealTimeLoader l;
    bool ok = l.Configure(params, config);
    if (ok) {
        ok = l.Start();
    }
    ReferenceT<RealTimeLoaderTestMessageObject1> obj;
    if (ok) {
        obj = ObjectRegistryDatabase::Instance()->Find("B");
        ok = obj.IsValid();
    }
    if (ok) {
        ok = obj->functionCalled;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool RealTimeLoaderTest::TestStop() {
    return TestStart_FirstState();
}
