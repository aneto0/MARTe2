/**
 * @file ConfigurationLoaderTCPTest.cpp
 * @brief Source file for class ConfigurationLoaderTCPTest
 * @date 16/02/2022
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
 * the class ConfigurationLoaderTCPTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicTCPSocket.h"
#include "ConfigurationDatabase.h"
#include "ConfigurationLoaderTCP.h"
#include "ConfigurationLoaderTCPTest.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ConfigurationLoaderTCPTest::TestConstructor() {
    using namespace MARTe;
    ConfigurationLoaderTCP test;
    return true;
}

bool ConfigurationLoaderTCPTest::TestPostInit() {
    using namespace MARTe;
    StreamString config = ""
        "+LoaderPostInit = {"
        "  Class = ReferenceContainer"
        "  +Parameters = {"
        "    Class = ConfigurationDatabase"
        "    Port = 24682"
        "  }"
        "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    ConfigurationLoaderTCP configLoader;
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        ret = (configLoader.PostInit() == ErrorManagement::NoError);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    
    return ret;
}

bool ConfigurationLoaderTCPTest::TestPostInit_NoPort() {
    using namespace MARTe;
    StreamString config = ""
        "+LoaderPostInit = {"
        "  Class = ReferenceContainer"
        "  +Parameters = {"
        "    Class = ConfigurationDatabase"
        "  }"
        "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    ConfigurationLoaderTCP configLoader;
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        ret = !(configLoader.PostInit() == ErrorManagement::NoError);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    
    return ret;
}


bool ConfigurationLoaderTCPTest::TestPostInit_InvalidPort() {
    using namespace MARTe;
    StreamString config = ""
        "+LoaderPostInit = {"
        "  Class = ReferenceContainer"
        "  +Parameters = {"
        "    Class = ConfigurationDatabase"
        "    Port = 1"
        "  }"
        "}";
    config.Seek(0LLU);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    ConfigurationLoaderTCP configLoader;
    bool ret = parser.Parse();
    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    if (ret) {
        ret = !(configLoader.PostInit() == ErrorManagement::NoError);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    
    return ret;
}

bool ConfigurationLoaderTCPTest::TestExecute() {
    using namespace MARTe;
    StreamString config = ""
        "+LoaderPostInit = {"
        "  Class = ReferenceContainer"
        "  +Parameters = {"
        "    Class = ConfigurationDatabase"
        "    Port = 24682"
        "  }"
        "}"
        "$App = {"
        "  Class = RealTimeApplication"
        "  +Functions = {"
        "    Class = ReferenceContainer"
        "    +GAMA = {"
        "      Class = GAM1"
        "      InputSignals = {"
        "        SignalIn1 = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "          Default = 1"
        "        }"
        "        SignalIn2 = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "          Default = 2"
        "        }"
        "      }"
        "      OutputSignals = {"
        "        SignalOut = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "        }"
        "      }"
        "    }"
        "  }"
        "  +Data = {"
        "    Class = ReferenceContainer"
        "    DefaultDataSource = DDB1"
        "    +DDB1 = {"
        "      Class = GAMDataSource"
        "      AllowNoProducers = 1"
        "    }"
        "    +Timings = {"
        "      Class = TimingDataSource"
        "    }"
        "  }"
        "  +States = {"
        "    Class = ReferenceContainer"
        "    +State1 = {"
        "      Class = RealTimeState"
        "      +Threads = {"
        "        Class = ReferenceContainer"
        "        +Thread1 = {"
        "          Class = RealTimeThread"
        "          Functions = {GAMA}"
        "        }"
        "      }"
        "    }"
        "  }"
        "  +Scheduler = {"
        "    Class = GAMScheduler"
        "    TimingDataSource = Timings"
        "  }"
        "}";
    config.Seek(0LLU);
    ReferenceT<ConfigurationLoaderTCP> configLoader = Reference("ConfigurationLoaderTCP", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdbLoader;
    cdbLoader.Write("Parser", "cdb");
    bool ret = configLoader->Configure(cdbLoader, config);
    BasicTCPSocket tcpSocket;
    if (ret) {
        ret = tcpSocket.Open();
    }
    if (ret) {
        Sleep::Sec(3.0);//Wait for the server to start
        ret = tcpSocket.Connect("127.0.0.1", 24682);
    }
    if (ret) {
        uint32 wsize = static_cast<uint32>(config.Size());
        ret = tcpSocket.Write(config.Buffer(), wsize);
    }
    uint32 size = 64;
    char8 buff[size];
    if (ret) {
        ret = tcpSocket.Read(&buff[0], size);
    }
    if (ret) {
        ret = (buff[0] == 'O');
    }
    if (ret) {
        ret = (buff[1] == 'K');
    }
    tcpSocket.Close();

    ObjectRegistryDatabase::Instance()->Purge();
    
    return ret;
}

bool ConfigurationLoaderTCPTest::TestExecuteErr() {
    using namespace MARTe;
    StreamString config = ""
        "+LoaderPostInit = {"
        "  Class = ReferenceContainer"
        "  +Parameters = {"
        "    Class = ConfigurationDatabase"
        "    Port = 24682"
        "  }"
        "}"
        "$App = {"
        "  Class = RealTimeApplication"
        "  +Functions = {"
        "    Class = ReferenceContainer"
        "    +GAMA = {"
        "      Class = GAM1"
        "      InputSignals = {"
        "        SignalIn1 = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "          Default = 1"
        "        }"
        "        SignalIn2 = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "          Default = 2"
        "        }"
        "      }"
        "      OutputSignals = {"
        "        SignalOut = {"
        "          DataSource = DDB1"
        "          Type = uint32"
        "        }"
        "      }"
        "    }"
        "  }"
        "  +Data = {"
        "    Class = ReferenceContainer"
        "    DefaultDataSource = DDB1"
        "    +DDB1 = {"
        "      Class = GAMDataSource"
        "      AllowNoProducers = 1"
        "    }"
        "    +Timings = {"
        "      Class = TimingDataSource"
        "    }"
        "  }"
        "  +States = {"
        "    Class = ReferenceContainer"
        "    +State1 = {"
        "      Class = RealTimeState"
        "      +Threads = {"
        "        Class = ReferenceContainer"
        "        +Thread1 = {"
        "          Class = RealTimeThread"
        "          Functions = {GAMA}"
        "        }"
        "      }"
        "    }"
        "  }"
        "  +Scheduler = {"
        "    Class = GAMScheduler"
        "    TimingDataSource = Timings"
        "  }"
        "}";
    config.Seek(0LLU);
    ReferenceT<ConfigurationLoaderTCP> configLoader = Reference("ConfigurationLoaderTCP", GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdbLoader;
    cdbLoader.Write("Parser", "cdb");
    bool ret = configLoader->Configure(cdbLoader, config);
    BasicTCPSocket tcpSocket;
    if (ret) {
        ret = tcpSocket.Open();
    }
    if (ret) {
        Sleep::Sec(3.0);//Wait for the server to start
        ret = tcpSocket.Connect("127.0.0.1", 24682);
    }
    if (ret) {
        StreamString configErr = ""
            "+LoaderPostInit = {"
            "  Class = ReferenceContainer"
            "  +Parameters = {"
            "    Class = ConfigurationDatabase"
            "    Port = 24682"
            "  }"
            "}"
            "$App = {"
            "  Class = RealTimeApplication"
            "  +Functions = {"
            "    Class = ReferenceContainer"
            "    +GAMA = {"
            "      Class = GAM1"
            "      InputSignals = {"
            "        SignalIn1 = {"
            "          DataSource = DDB1"
            "          Type = uint32"
            "          Default = 1"
            "        }"
            "        SignalIn2 = {"
            "          DataSource = DDB1"
            "          Type = uint32"
            "          Default = 2"
            "        }"
            "      }"
            "      OutputSignals = {"
            "        SignalOut = {"
            "          DataSource = DDB1"
            "          Type = uint32"
            "        }"
            "      }"
            "    }"
            "  }"
            "  +States = {"
            "    Class = ReferenceContainer"
            "    +State1 = {"
            "      Class = RealTimeState"
            "      +Threads = {"
            "        Class = ReferenceContainer"
            "        +Thread1 = {"
            "          Class = RealTimeThread"
            "          Functions = {GAMA}"
            "        }"
            "      }"
            "    }"
            "  }"
            "  +Scheduler = {"
            "    Class = GAMScheduler"
            "    TimingDataSource = Timings"
            "  }"
            "}";
        uint32 wsize = static_cast<uint32>(configErr.Size());
        ret = tcpSocket.Write(configErr.Buffer(), wsize);
    }
    uint32 size = 64;
    char8 buff[size];
    if (ret) {
        ret = tcpSocket.Read(&buff[0], size);
    }
    if (ret) {
        ret = (buff[0] == 'E');
    }
    if (ret) {
        ret = (buff[1] == 'R');
    }
    if (ret) {
        ret = (buff[2] == 'R');
    }
    tcpSocket.Close();

    ObjectRegistryDatabase::Instance()->Purge();
    
    return ret;
}

