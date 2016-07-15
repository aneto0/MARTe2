/**
 * @file GAMTest.cpp
 * @brief Source file for class GAMTest
 * @date Jun 8, 2016
 * @author aneto
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
#include <stdio.h>
#include "CompilerTypes.h"
#include "ConfigurationDatabase.h"
#include "ErrorInformation.h"
#include "ErrorManagement.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
using namespace MARTe;
using namespace MARTe::ErrorManagement;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct TrackErrorPar3 {
    uint32 Par1;
};

DECLARE_CLASS_MEMBER(TrackErrorPar3, Par1, uint32, "", "");
static const IntrospectionEntry* TrackErrorPar3Entries[] = { &TrackErrorPar3_Par1_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(TrackErrorPar3, TrackErrorPar3Entries);
INTROSPECTION_REGISTER(TrackErrorPar3, "1.0", TrackErrorPar3_introspection)

struct TrackError {
    uint32 Par1;
    uint32 Par2;
    TrackErrorPar3 Par3;
};

DECLARE_CLASS_MEMBER(TrackError, Par1, uint32, "", "");
DECLARE_CLASS_MEMBER(TrackError, Par2, uint32, "", "");
DECLARE_CLASS_MEMBER(TrackError, Par3, TrackErrorPar3, "", "");
static const IntrospectionEntry* TrackErrorEntries[] = { &TrackError_Par1_introspectionEntry, &TrackError_Par2_introspectionEntry,
        &TrackError_Par3_introspectionEntry, 0 };
DECLARE_CLASS_INTROSPECTION(TrackError, TrackErrorEntries);
INTROSPECTION_REGISTER(TrackError, "1.0", TrackError_introspection)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
static StreamString config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                Signal1 = {"
        "                    Alias = SignalIntro1.Par1"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                }"
        "                Signal2 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    NumberOfDimensions = 1"
        "                    NumberOfElements = 10"
        "                    Ranges = {{0 0} {9 9}}"
        "                }"
        "                Signal3 = {"
        "                    DataSource = DDB1"
        "                    Alias = SignalIntro1.Par3.Par1"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "                SignalIntro1 = {"
        "                    Type = TrackError"
        "                    DataSource = DDB1"
        "                }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                SignalA1 = {"
        "                    Type=uint32"
        "                }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                SignalD1 = {"
        "                    Alias = SignalIntro1"
        "                }"
        "            }"
        "        }"
        "        +GAME = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                SignalA1 = {"
        "                    Type=uint32"
        "                }"
        "                SignalB1 = {"
        "                    Alias = SignalIntro1.Par1"
        "                    DataSource = DriverB"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                SignalE1 = {"
        "                    Alias = Signal1"
        "                    DataSource = DriverC"
        "                    Type = TrackError"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = DataSource1"
        "        }"
        "        +DriverA = {"
        "            Class = Driver1"
        "            Signals = {"
        "                SignalA1 = {"
        "                    Type = uint32"
        "                }"
        "                SignalA2 = {"
        "                    Type = uint32"
        "                }"
        "            }"
        "        }"
        "        +DriverB = {"
        "            Class = Driver1"
        "            Signals = {"
        "                Signal1 = {"
        "                    Type = TrackError"
        "                }"
        "            }"
        "        }"
        "        +DriverC = {"
        "            Class = Driver1"
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
        "                    Functions = {:Functions.GAMA, :Functions.GAMB, :Functions.GAMD, :Functions.GAME}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMB, :Functions.GAMC, :Functions.GAME}"
        "                }"
        "            }"
        "        }"
        "    }"
        "}";

static StreamString config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                    TimeCyclesSamples = {1 1}"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Signal2 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                    TimeCyclesSamples = {1 1}"
        "                }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "                Signal2 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                    TimeCyclesSamples = {1 1}"
        "                }"
        "            }"
        "            OutputSignals = {"
        "                Signal1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Ranges = {{0 0}}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
        "        DefaultDataSource = DDB1"
        "        +DDB1 = {"
        "            Class = DataSource1"
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
        "                    Functions = {:Functions.GAMA, :Functions.GAMB}"
        "                }"
        "            }"
        "        }"
        "    }"
        "}";

void MyErrorProcessFunction(const ErrorInformation &errorInfo,
                            const char8 * const errorDescription) {
    //printf("---->>%s\n", errorDescription);
}

int main(int argc,
         char **argv) {
    SetErrorProcessFunction(&MyErrorProcessFunction);
    ConfigurationDatabase cdb;
    config2.Seek(0);
    StandardParser parser(config2, cdb);
    if (!parser.Parse()) {
        printf("Parser failed!\n");
        return false;
    }
    cdb.MoveToRoot();

    /*cdb.MoveAbsolute("$Application1");
     printf("|%d|\n", cdb.MoveRelative("+Functions.+GAM3"));
     return 0;*/
    StreamString godStr;
    godStr.Printf("GlobalObjectDatabase\n--------------------\n%s", cdb);
    godStr.Seek(0);
    printf("%s\n", godStr.Buffer());

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<RealTimeApplication> application = god->Find("Application1");
    if (application.IsValid()) {
        if (application->ConfigureApplication()) {
            printf("OK, ready to start working!\n");
        }
        else {
            printf("Application not valid!\n");
        }
    }
    else {
        printf("Application not valid!\n");
    }
    return 0;
}

