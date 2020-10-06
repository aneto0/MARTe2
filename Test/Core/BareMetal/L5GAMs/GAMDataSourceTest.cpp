/**
 * @file GAMDataSourceTest.cpp
 * @brief Source file for class GAMDataSourceTest
 * @date 28/07/2016
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
 * the class GAMDataSourceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "GAMDataSourceTest.h"
#include "GAMSchedulerI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static bool InitialiseGAMDataSourceEnviroment(const char8 * const config) {
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

class GAMDataSourceTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

GAMDataSourceTestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();
    virtual void CustomPrepareNextState();

};

GAMDataSourceTestScheduler1::GAMDataSourceTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType GAMDataSourceTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType GAMDataSourceTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void GAMDataSourceTestScheduler1::CustomPrepareNextState() {

}

CLASS_REGISTER(GAMDataSourceTestScheduler1, "1.0")

class GAMDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

GAMDataSourceTestGAM1    ();
    virtual bool Setup();
    virtual bool Execute();
};

GAMDataSourceTestGAM1::GAMDataSourceTestGAM1() :
        GAM() {

}

bool GAMDataSourceTestGAM1::Setup() {
    return true;
}

bool GAMDataSourceTestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(GAMDataSourceTestGAM1, "1.0")

static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   Default = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   Default = 5"
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
        "        +State3 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "                   Default = {{2 3 4 5 6} {7 8 9 10 11}}"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {1 2 3 4 5 6 7 8 9 10}"
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
        "        +State3 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config4 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM0 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               SignalAlone = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {{2 3 4 5 6} {7 8 9 10 11}}"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {1 2 3 4 5 6 7 8 9 10}"
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
        "                    Functions = {GAM0 }"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config5 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM0 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               SignalAlone = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "                   Default = {{2 3 4 5 6 7} {7 8 9 10 11 12}}"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {1 2 3 4 5 6 7 8 9 10}"
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
        "                    Functions = {GAM0}"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config6 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM0 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               SignalAlone = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "                   Default = {{2 3 4 5 6} {7 8 9 10 11}}"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {1 2 3 4 5 6 7 8 9 10 11}"
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
        "                    Functions = {GAM0 }"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

//No Producer in state 1
static const char8 * const config7 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "                    Functions = {GAMB}"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

//No Producer in state 2
static const char8 * const config8 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "                    Functions = {GAMB GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

//More than one producer in state 1
static const char8 * const config9 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA2 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "                    Functions = {GAMB GAMA GAMA2 }"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

//More than one producer in state 2
static const char8 * const config10 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA2 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "                    Functions = {GAMB GAMA2 GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

//GAM is not executed and thus has no consumers/producers in any given state. That is OK!
static const char8 * const config11 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = GAMDataSourceTestGAM1"
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
        "        Class = GAMDataSourceTestScheduler1"
        "        TimingDataSource = Timings"
        "    }"
        "}";

//No Producer in state 1 but allow no producers
static const char8 * const config12 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "            AllowNoProducers = 1"
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
        "                    Functions = {GAMB}"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config13 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "            ResetUnusedVariablesAtStateChange = 0"
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
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";


//Different producer in state 1 and 2
static const char8 * const config14 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMA2 = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
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
        "                    Functions = {GAMB GAMA  }"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA2 GAMB }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const config15 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = char8"
        "                   NumberOfElements = 16"
        "                   NumberOfDimensions = 1"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = char8"
        "                   NumberOfElements = 16"
        "                   NumberOfDimensions = 1"
        "                   Default = \"Test\"" 
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAMDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAMDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "                   Default = {1 2 3 4 5 6 7 8 9 10}"
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
        "                    Functions = {GAMD GAMC}"
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
        "        +State3 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMD GAMC}"
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

bool GAMDataSourceTest::TestConstructor() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    if (ret) {
        ReferenceT<GAMDataSource> gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_NoDefault() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    uint32 *signal0 = NULL_PTR(uint32 *);
    uint32 *signal1 = NULL_PTR(uint32 *);
    uint32 *signal2 = NULL_PTR(uint32 *);
    uint32 *signal3 = NULL_PTR(uint32 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal1));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(2, 0, reinterpret_cast<void *&>(signal2));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(3, 0, reinterpret_cast<void *&>(signal3));
    }
    if (ret) {
        ret = (*signal0 == 0u);
    }
    if (ret) {
        ret = (*signal2 == 0u);
    }
    const uint32 numberOfElements = 10;
    uint32 i;
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == 0);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal3[i] == 0);
    }

    if (ret) {
        //Dirty the memory
        *signal0 = 1u;
        *signal2 = 2u;
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = (i + 1);
            signal3[i] = (i + 1);
        }
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }
    if (ret) {
        //Signal was already being used keep the value
        ret = (*signal0 == 1u);
    }
    if (ret) {
        //Signal was not used and there is no default, reset to zero
        ret = (*signal2 == 0u);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == 0);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal3[i] == 0);
    }
    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_Default_Basic() {
    bool ret = InitialiseGAMDataSourceEnviroment(config2);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    uint32 *signal0 = NULL_PTR(uint32 *);
    uint32 *signal1 = NULL_PTR(uint32 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal1));
    }
    if (ret) {
        ret = (*signal0 == 0u);
    }
    if (ret) {
        ret = (*signal1 == 0u);
    }

    if (ret) {
        //Dirty the memory
        *signal0 = 1u;
        *signal1 = 2u;
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }
    if (ret) {
        //Signal was already being used keep the value
        ret = (*signal0 == 1u);
    }
    if (ret) {
        //Signal has default
        ret = (*signal1 == 5u);
    }
    if (ret) {
        //Dirty the memory
        *signal0 = 7u;
        *signal1 = 8u;
        ret = gamDataSource->PrepareNextState("State2", "State3");
    }
    if (ret) {
        //Signal was used. Keep the value
        ret = (*signal0 == 7u);
    }
    if (ret) {
        //Signal was used. Keep the value
        ret = (*signal1 == 8u);
    }

    if (ret) {
        //Dirty the memory
        *signal1 = 9u;
        ret = gamDataSource->PrepareNextState("State3", "State1");
    }
    if (ret) {
        //Signal was not used. Use default
        ret = (*signal0 == 2u);
    }
    if (ret) {
        //Signal was used. Keep the value
        ret = (*signal1 == 9u);
    }

    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_Default_Dimensions() {
    bool ret = InitialiseGAMDataSourceEnviroment(config3);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    uint32 *signal0 = NULL_PTR(uint32 *);
    uint32 *signal1 = NULL_PTR(uint32 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal1));
    }

    const uint32 numberOfElements = 10u;
    uint32 i;

    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal0[i] == 0);
    }

    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == 0);
    }

    if (ret) {
        //Dirty the memory
        for (i = 0u; i < numberOfElements; i++) {
            signal0[i] = i * 100;
        }
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = i * 200;
        }
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }

    //Signal was already being used keep the value
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal0[i] == i * 100);
    }

    //Signal has default
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == (i + 1));
    }

    if (ret) {
        //Dirty the memory
        //Dirty the memory
        for (i = 0u; i < numberOfElements; i++) {
            signal0[i] = i * 700;
        }
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = i * 800;
        }
        ret = gamDataSource->PrepareNextState("State2", "State3");
    }

    //Signal was used. Keep the value
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal0[i] == i * 700);
    }

    //Signal was used. Keep the value
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == i * 800);
    }

    if (ret) {
        //Dirty the memory
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = i * 900;
        }
        ret = gamDataSource->PrepareNextState("State3", "State1");
    }

    //Signal was not used. Use default
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal0[i] == (i + 2));
    }

    //Signal was used. Keep the value
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == i * 900);
    }

    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_Default_Char8Arr() {
    bool ret = InitialiseGAMDataSourceEnviroment(config15);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    char8 *signal0 = NULL_PTR(char8 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }

    const uint32 numberOfElements = 16u;
    uint32 i;

    if (ret) {
        //Clean the memory
        for (i = 0u; i < numberOfElements; i++) {
            signal0[i] = 1;
        }
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }

    //Signal was already being used keep the value
    if (ret) {
        ret = (signal0[0] == 'T');
    }
    if (ret) {
        ret = (signal0[1] == 'e');
    }
    if (ret) {
        ret = (signal0[2] == 's');
    }
    if (ret) {
        ret = (signal0[3] == 't');
    }
    return ret;
}


bool GAMDataSourceTest::TestPrepareNextState_Default_Dimensions_Mismatch_1() {
    bool ret = InitialiseGAMDataSourceEnviroment(config4);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }

    if (ret) {
        //Dimension mismatch between default value of and NumberOfDimensions
        ret = !gamDataSource->PrepareNextState("State1", "State2");
    }

    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_Default_Dimensions_Mismatch_2() {
    bool ret = InitialiseGAMDataSourceEnviroment(config5);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }

    if (ret) {
        //Dimension mismatch between default value of and NumberOfDimensions
        ret = !gamDataSource->PrepareNextState("State1", "State2");
    }

    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_Default_Elements_Mismatch() {
    bool ret = InitialiseGAMDataSourceEnviroment(config6);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }

    if (ret) {
        //Dimension mismatch between the number of elements
        ret = !gamDataSource->PrepareNextState("State1", "State2");
    }

    return ret;
}

bool GAMDataSourceTest::TestGetNumberOfMemoryBuffers() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    ret = (gamDataSource->GetNumberOfMemoryBuffers() == 1u);
    return ret;
}

bool GAMDataSourceTest::TestGetSignalMemoryBuffer() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    uint32 *signal0 = NULL_PTR(uint32 *);
    uint32 *signal1 = NULL_PTR(uint32 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = (signal0 != NULL_PTR(uint32 *));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal1));
    }
    if (ret) {
        ret = (signal1 != NULL_PTR(uint32 *));
    }
    if (ret) {
        ret = !gamDataSource->GetSignalMemoryBuffer(200, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = !gamDataSource->GetSignalMemoryBuffer(0, 1, reinterpret_cast<void *&>(signal0));
    }

    return ret;
}

bool GAMDataSourceTest::TestAllocateMemory() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    if (ret) {
        ret = !gamDataSource->AllocateMemory();
    }
    return ret;
}

bool GAMDataSourceTest::TestGetBrokerName() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 1u);
        cdb.Write("Frequency", -1.0f);
        StreamString brokerName = gamDataSource->GetBrokerName(cdb, InputSignals);
        ret = (brokerName == "MemoryMapInputBroker");
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 1u);
        cdb.Write("Frequency", -1.0f);
        StreamString brokerName = gamDataSource->GetBrokerName(cdb, OutputSignals);
        ret = (brokerName == "MemoryMapOutputBroker");
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 2u);
        cdb.Write("Frequency", -1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, InputSignals) == NULL_PTR(char8 *));
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 2u);
        cdb.Write("Frequency", -1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, OutputSignals) == NULL_PTR(char8 *));
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 1u);
        cdb.Write("Frequency", 1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, InputSignals) == NULL_PTR(char8 *));
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 1u);
        cdb.Write("Frequency", 1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, OutputSignals) == NULL_PTR(char8 *));
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 2u);
        cdb.Write("Frequency", 1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, InputSignals) == NULL_PTR(char8 *));
    }
    if (ret) {
        ConfigurationDatabase cdb;
        cdb.Write("Samples", 2u);
        cdb.Write("Frequency", 1.0f);
        ret = (gamDataSource->GetBrokerName(cdb, OutputSignals) == NULL_PTR(char8 *));
    }
    return ret;
}

bool GAMDataSourceTest::TestGetInputBrokers() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    ReferenceT<GAMDataSource> gamDataSource;

    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    ReferenceContainer inputBrokersA;
    ReferenceContainer inputBrokersB;
    if (ret) {
        ret = !gamDataSource->GetInputBrokers(inputBrokersA, "GAMA", reinterpret_cast<void *>(0xAA));
    }
    if (ret) {
        ret = (inputBrokersA.Size() == 0u);
    }
    if (ret) {
        ret = gamDataSource->GetInputBrokers(inputBrokersB, "GAMB", reinterpret_cast<void *>(0xAA));
    }
    if (ret) {
        ret = (inputBrokersB.Size() > 0u);
    }
    ReferenceT<MemoryMapInputBroker> broker;
    if (ret) {
        broker = inputBrokersB.Get(0);
    }
    if (ret) {
        ret = broker.IsValid();
    }
    return ret;
}

bool GAMDataSourceTest::TestGetOutputBrokers() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    ReferenceT<GAMDataSource> gamDataSource;

    if (ret) {
        gamA = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMA");
        ret = gamA.IsValid();
    }
    if (ret) {
        gamB = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMB");
        ret = gamB.IsValid();
    }

    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    ReferenceContainer outputBrokersA;
    ReferenceContainer outputBrokersB;
    if (ret) {
        ret = gamDataSource->GetOutputBrokers(outputBrokersA, "GAMA", reinterpret_cast<void *>(0xAA));
    }
    if (ret) {
        ret = (outputBrokersA.Size() > 0u);
    }
    ReferenceT<MemoryMapOutputBroker> broker;
    if (ret) {
        broker = outputBrokersA.Get(0);
    }
    if (ret) {
        ret = broker.IsValid();
    }
    if (ret) {
        ret = !gamDataSource->GetOutputBrokers(outputBrokersB, "GAMB", reinterpret_cast<void *>(0xAA));
    }
    if (ret) {
        ret = (outputBrokersB.Size() == 0u);
    }
    return ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase() {
    bool ret = InitialiseGAMDataSourceEnviroment(config1);
    ReferenceT<GAM> gamA;
    ReferenceT<GAM> gamB;
    ReferenceT<GAMDataSource> gamDataSource;

    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }

    return ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_NoProducer_State_1() {
    bool ret = InitialiseGAMDataSourceEnviroment(config7);
    return !ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_NoProducer_State_2() {
    bool ret = InitialiseGAMDataSourceEnviroment(config8);
    return !ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_MoreThanOneProducer_State_1() {
    bool ret = InitialiseGAMDataSourceEnviroment(config9);
    return !ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_MoreThanOneProducer_State_2() {
    bool ret = InitialiseGAMDataSourceEnviroment(config10);
    return !ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_GAM_Never_Executed() {
    bool ret = InitialiseGAMDataSourceEnviroment(config11);
    return ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_NoProducer_State_1_AllowNoProducers() {
    bool ret = InitialiseGAMDataSourceEnviroment(config12);
    return ret;
}

bool GAMDataSourceTest::TestPrepareNextState_NoResetUnusedVariables() {
    bool ret = InitialiseGAMDataSourceEnviroment(config13);
    ReferenceT<GAMDataSource> gamDataSource;
    if (ret) {
        gamDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.DDB1");
        ret = gamDataSource.IsValid();
    }
    uint32 *signal0 = NULL_PTR(uint32 *);
    uint32 *signal1 = NULL_PTR(uint32 *);
    uint32 *signal2 = NULL_PTR(uint32 *);
    uint32 *signal3 = NULL_PTR(uint32 *);

    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, reinterpret_cast<void *&>(signal0));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, reinterpret_cast<void *&>(signal1));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(2, 0, reinterpret_cast<void *&>(signal2));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(3, 0, reinterpret_cast<void *&>(signal3));
    }
    if (ret) {
        ret = (*signal0 == 0u);
    }
    if (ret) {
        ret = (*signal2 == 0u);
    }
    const uint32 numberOfElements = 10;
    uint32 i;
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == 0);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal3[i] == 0);
    }

    if (ret) {
        //Dirty the memory
        *signal0 = 1u;
        *signal2 = 2u;
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = (i + 1);
            signal3[i] = (i + 1);
        }
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }
    if (ret) {
        //Signal was already being used keep the value
        ret = (*signal0 == 1u);
    }
    if (ret) {
        //Signal was not used and there is no default, reset to zero and it is the first time.
        ret = (*signal2 == 0u);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == 0);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal3[i] == 0);
    }

    //Go back again
    if (ret) {
        ret = gamDataSource->PrepareNextState("State2", "State1");
    }

    if (ret) {
        //Dirty the memory
        *signal0 = 1u;
        *signal2 = 2u;
        for (i = 0u; i < numberOfElements; i++) {
            signal1[i] = (i + 1);
            signal3[i] = (i + 1);
        }
        ret = gamDataSource->PrepareNextState("State1", "State2");
    }
    if (ret) {
        //Signal was already being used keep the value
        ret = (*signal0 == 1u);
    }
    if (ret) {
        //Signal was not used and there is no default, reset to zero and it is the first time.
        ret = (*signal2 == 2u);
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal1[i] == (i + 1));
    }
    for (i = 0u; (i < numberOfElements) && (ret); i++) {
        ret = (signal3[i] == (i + 1));
    }
    return ret;
}

bool GAMDataSourceTest::TestSetConfiguredDatabase_MoreThanOneProducer_Different_States() {
    bool ret = InitialiseGAMDataSourceEnviroment(config14);
    return ret;
}
