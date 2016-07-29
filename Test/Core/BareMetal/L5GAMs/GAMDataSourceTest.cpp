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

    parser.Parse();

    ObjectRegistryDatabase::Instance()->CleanUp();

    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<RealTimeApplication> application = god->Find("Application1");

    return application->ConfigureApplication();
}

static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal1 = {"
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
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMB :Functions.GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMB :Functions.GAMA :Functions.GAMD :Functions.GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "}";

static const char8 * const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   Default = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = GAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = GAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = GAM1"
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
        "    }"
        "    +States = {"
        "        Class = ReferenceContainer"
        "        +State1 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMB :Functions.GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMB :Functions.GAMA :Functions.GAMD :Functions.GAMC}"
        "                }"
        "            }"
        "        }"
        "        +State3 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {:Functions.GAMD :Functions.GAMC}"
        "                }"
        "            }"
        "        }"
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
    void *signal0Buffer0PointerHolder = NULL_PTR(void *);
    void *signal0Buffer1PointerHolder = NULL_PTR(void *);
    uint32 **signal0Buffer0 = NULL_PTR(uint32 **);
    uint32 **signal0Buffer1 = NULL_PTR(uint32 **);
    void *signal1Buffer0PointerHolder = NULL_PTR(void *);
    void *signal1Buffer1PointerHolder = NULL_PTR(void *);
    uint32 **signal1Buffer0 = NULL_PTR(uint32 **);
    uint32 **signal1Buffer1 = NULL_PTR(uint32 **);

    if (ret) {
        //Remember that the dual-buffer mechanism returns a pointer to the variable holding the real memory address.
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, signal0Buffer0PointerHolder);
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 1, signal0Buffer1PointerHolder);
    }
    if (ret) {
        signal0Buffer0 = (reinterpret_cast<uint32 **>(signal0Buffer0PointerHolder));
    }
    if (ret) {
        signal0Buffer1 = (reinterpret_cast<uint32 **>(signal0Buffer1PointerHolder));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, signal1Buffer0PointerHolder);
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 1, signal1Buffer1PointerHolder);
    }
    if (ret) {
        signal1Buffer0 = (reinterpret_cast<uint32 **>(signal1Buffer0PointerHolder));
    }
    if (ret) {
        signal1Buffer1 = (reinterpret_cast<uint32 **>(signal1Buffer1PointerHolder));
    }
    if (ret) {
        ret = (**signal0Buffer0 == 0u);
    }
    if (ret) {
        ret = (**signal0Buffer1 == 0u);
    }
    if (ret) {
        ret = (**signal1Buffer0 == 0u);
    }
    if (ret) {
        ret = (**signal1Buffer1 == 0u);
    }
    RealTimeApplication::index = 0;
    RealTimeStateInfo stateInfo;
    if (ret) {
        **signal0Buffer0 = 1u;
        **signal0Buffer1 = 2u;
        **signal1Buffer0 = 3u;
        **signal1Buffer1 = 4u;
        stateInfo.currentState = "State1";
        stateInfo.nextState = "State2";
        ret = gamDataSource->PrepareNextState(stateInfo);
    }
    if (ret) {
        //As in the previous state, since we are changing to State2 (i.e. signal0Buffer0 -> memorySignalBuffer0)
        ret = (**signal0Buffer0 == 1u);
    }
    if (ret) {
        //Point to the memory buffer of the previous state, as the signal already existed (signal0Buffer1 -> memorySignalBuffer0)
        ret = (**signal0Buffer1 == 1u);
    }
    if (ret) {
        //This is not touched...
        ret = (**signal1Buffer0 == 3u);
    }
    if (ret) {
        //memoryBuffer1 is zeroed because no Default was set and signal1Buffer1 -> memorySignalBuffer1
        ret = (**signal1Buffer1 == 0u);
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
    void *signal0Buffer0PointerHolder = NULL_PTR(void *);
    void *signal0Buffer1PointerHolder = NULL_PTR(void *);
    uint32 **signal0Buffer0 = NULL_PTR(uint32 **);
    uint32 **signal0Buffer1 = NULL_PTR(uint32 **);
    void *signal1Buffer0PointerHolder = NULL_PTR(void *);
    void *signal1Buffer1PointerHolder = NULL_PTR(void *);
    uint32 **signal1Buffer0 = NULL_PTR(uint32 **);
    uint32 **signal1Buffer1 = NULL_PTR(uint32 **);

    if (ret) {
        //Remember that the dual-buffer mechanism returns a pointer to the variable holding the real memory address.
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, signal0Buffer0PointerHolder);
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 1, signal0Buffer1PointerHolder);
    }
    if (ret) {
        signal0Buffer0 = (reinterpret_cast<uint32 **>(signal0Buffer0PointerHolder));
    }
    if (ret) {
        signal0Buffer1 = (reinterpret_cast<uint32 **>(signal0Buffer1PointerHolder));
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 0, signal1Buffer0PointerHolder);
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(1, 1, signal1Buffer1PointerHolder);
    }
    if (ret) {
        signal1Buffer0 = (reinterpret_cast<uint32 **>(signal1Buffer0PointerHolder));
    }
    if (ret) {
        signal1Buffer1 = (reinterpret_cast<uint32 **>(signal1Buffer1PointerHolder));
    }
    if (ret) {
        ret = (**signal0Buffer0 == 0u);
    }
    if (ret) {
        ret = (**signal0Buffer1 == 0u);
    }
    if (ret) {
        ret = (**signal1Buffer0 == 0u);
    }
    if (ret) {
        ret = (**signal1Buffer1 == 0u);
    }
    RealTimeApplication::index = 0;
    RealTimeStateInfo stateInfo;
    if (ret) {
        **signal0Buffer0 = 1u;
        **signal0Buffer1 = 2u;
        **signal1Buffer0 = 3u;
        **signal1Buffer1 = 4u;
        stateInfo.currentState = "State1";
        stateInfo.nextState = "State2";
        ret = gamDataSource->PrepareNextState(stateInfo);
    }
    if (ret) {
        ret = (**signal0Buffer0 == 1u);
    }
    if (ret) {
        ret = (**signal0Buffer1 == 1u);
    }
    if (ret) {
        ret = (**signal1Buffer0 == 3u);
    }
    if (ret) {
        //Use the default value
        ret = (**signal1Buffer1 == 5u);
    }
    RealTimeApplication::index = 1;
    if (ret) {
        stateInfo.currentState = "State2";
        stateInfo.nextState = "State3";
        ret = gamDataSource->PrepareNextState(stateInfo);
    }
    if (ret) {
        //Keep the old value
        ret = (**signal1Buffer0 == 5u);
    }
    RealTimeApplication::index = 0;
    if (ret) {
        stateInfo.currentState = "State3";
        stateInfo.nextState = "State2";
        ret = gamDataSource->PrepareNextState(stateInfo);
    }
    if (ret) {
        ret = (**signal0Buffer0 == 1u);
    }
    if (ret) {
        //Load the default as this was not used in the previous state
        ret = (**signal0Buffer1 == 2u);
    }
    if (ret) {
        ret = (**signal1Buffer0 == 5u);
    }
    if (ret) {
        //Keep the old value
        ret = (**signal1Buffer1 == 5u);
    }
    return ret;
}
