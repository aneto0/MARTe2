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
        "                    Functions = {:Functions.GAMB :Functions.GAMA }"
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
    void *gamBSignal0Buffer0PointerHolder = NULL_PTR(void *);
    void *gamBSignal0Buffer1PointerHolder = NULL_PTR(void *);
    uint32 *gamBSignal0Buffer0 = NULL_PTR(uint32 *);
    uint32 *gamBSignal0Buffer1 = NULL_PTR(uint32 *);

    if (ret) {
        //Remember that the dual-buffer mechanism returns a pointer to the variable holding the real memory address.
        ret = gamDataSource->GetSignalMemoryBuffer(0, 0, gamBSignal0Buffer0PointerHolder);
    }
    if (ret) {
        ret = gamDataSource->GetSignalMemoryBuffer(0, 1, gamBSignal0Buffer1PointerHolder);
    }
    if (ret) {
        gamBSignal0Buffer0 = *(reinterpret_cast<uint32 **>(gamBSignal0Buffer0PointerHolder));
    }
    if (ret) {
        gamBSignal0Buffer1 = *(reinterpret_cast<uint32 **>(gamBSignal0Buffer1PointerHolder));
    }
    if (ret) {
        ret = (*gamBSignal0Buffer0 == 0u);
    }
    if (ret) {
        ret = (*gamBSignal0Buffer1 == 0u);
    }
    return ret;
}

