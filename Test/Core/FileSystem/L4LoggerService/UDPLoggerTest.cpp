/**
 * @file UDPLoggerTest.cpp
 * @brief Source file for class UDPLoggerTest
 * @date 15/03/2017
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
 * the class UDPLoggerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "LoggerService.h"
#include "Threads.h"
#include "UDPLogger.h"
#include "UDPLoggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static bool udpLoggerTestServerOK = true;
static bool udpLoggerTestServerDone = false;
static bool udpLoggerTestServerSetup = false;
static void UDPLoggerTestServerThread(void *params) {
    using namespace MARTe;
    UDPSocket server;
    udpLoggerTestServerOK = server.Open();
    InternetHost thisHost(44448, "127.0.0.1");
    udpLoggerTestServerOK &= server.Listen(thisHost.GetPort());
    uint32 msecTimeout = 5000u;
    uint32 udpServerReadSize = 8000u;
    char8 *udpServerBufferRead[8000u];
    if (udpLoggerTestServerOK) {
        udpLoggerTestServerSetup = true;
        udpLoggerTestServerOK = server.Read((char8 *) udpServerBufferRead, udpServerReadSize, msecTimeout);
        if (udpLoggerTestServerOK) {
            udpLoggerTestServerOK = (udpServerReadSize > 0u);
        }
    }
    server.Close();
    udpLoggerTestServerSetup = true;
    udpLoggerTestServerDone = true;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool UDPLoggerTest::TestConstructor() {
    using namespace MARTe;
    UDPLogger test;
    return true;
}

bool UDPLoggerTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Address", "127.0.0.1");
    cdb.Write("Port", 44448);
    UDPLogger test;
    return test.Initialise(cdb);
}

bool UDPLoggerTest::TestInitialise_False_Address() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Port", 44448);
    UDPLogger test;
    return !test.Initialise(cdb);
}

bool UDPLoggerTest::TestInitialise_False_Port() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Address", "127.0.0.2");
    UDPLogger test;
    return !test.Initialise(cdb);
}

bool UDPLoggerTest::TestInitialise_False_Address_Port() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Address", "327.0.0.1");
    cdb.Write("Port", 44448);
    UDPLogger test;
    return !test.Initialise(cdb);
}

bool UDPLoggerTest::TestConsumeLogMessage() {
    using namespace MARTe;
    Threads::BeginThread((ThreadFunctionType) UDPLoggerTestServerThread, NULL);
    while (!udpLoggerTestServerSetup) {
        Sleep::MSec(5u);
    }
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtOofFRmC");
    cdb.Write("Address", "127.0.0.1");
    cdb.Write("Port", 44448);
    ReferenceT<UDPLogger> test(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<LoggerService> logService(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test->Initialise(cdb);
    logService->Insert(test);
    ConfigurationDatabase cdb2;
    cdb2.Write("CPUs", "0xf");
    logService->Initialise(cdb2);
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "UDPLoggerTest::TestConsumeLogMessage()");
    while (!udpLoggerTestServerDone) {
        Sleep::MSec(5u);
    }
    return udpLoggerTestServerOK;
}
