/**
 * @file PlainLoggerServiceTest.cpp
 * @brief Source file for class PlainLoggerServiceTest
 * @date 01/07/2024
 * @author Giuseppe Avon
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
 * the class PlainLoggerServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "LoggerService.h"
#include "ObjectRegistryDatabase.h"
#include "PlainLoggerServiceTest.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * A dummy logger to the the LoggerService
 */
class PlainLoggerConsumerITest: public MARTe::Object, public MARTe::LoggerConsumerI {
public:
    CLASS_REGISTER_DECLARATION()
    PlainLoggerConsumerITest() : Object() {
        ok = false;
    }

    virtual ~PlainLoggerConsumerITest() {

    }

    virtual void ConsumeLogMessage(MARTe::LoggerPage *logPage) {
        //Test local variable needed otherwise racing condition might happen here.
        bool test = (logPage->errorInfo.header.errorType == MARTe::ErrorManagement::Debug);
        MARTe::StreamString err = logPage->errorStrBuffer;
        test &= (err == "PlainLoggerConsumerITest test");
        ok = test;
    }

    bool ok;
};
CLASS_REGISTER(PlainLoggerConsumerITest, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

    // ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    // if (ok) {
    //     god->Purge();
    //     ok = god->Initialise(cdb);
    // }
static const MARTe::char8 *const loggerTestConfig1 = ""
    "+LoggerService = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}";

static const MARTe::char8 *const loggerTestConfig2 = ""
    "+LoggerService = {"
    "    Class = PlainLoggerService"
    "}";


static const MARTe::char8 *const loggerTestConfig3 = ""
    "+LoggerService = {"
    "    Class = PlainLoggerService"
    "    +NotARealConsumer = {"
    "        Class = ReferenceContainer"
    "    }"
    "}";

static const MARTe::char8 *const loggerTestConfig4 = ""
    "+LoggerServiceA = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest0 = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}"
    "+LoggerServiceB = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest0 = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}"
    "+LoggerServiceC = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest0 = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}"
    "+LoggerServiceD = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest0 = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}"
    "+LoggerServiceE = {"
    "    Class = PlainLoggerService"
    "    +LoggerConsumerTest0 = {"
    "        Class = PlainLoggerConsumerITest"
    "        Format = \"EtOoRmCP\""
    "        PrintKeys = 1"
    "    }"
    "}";

static bool InitialiseTestEnvironment(const MARTe::char8 *const config) {
    using namespace MARTe;
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

    return ok;
}

static void CleanupTestEnvironment() {
    using namespace MARTe;
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    god->Purge();
}

bool PlainLoggerServiceTest::TestConstructor() {
    using namespace MARTe;
    bool ok = InitialiseTestEnvironment(loggerTestConfig1);
    ReferenceT<PlainLoggerService> plainLoggerService;

    if(ok) {
        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        plainLoggerService = god->Find("LoggerService");
        ok = plainLoggerService.IsValid();
    }

    if(ok) {
        uint32 tmpSize = plainLoggerService->Size();
        ok = (tmpSize == 1u);
    }
 
    CleanupTestEnvironment();

    return ok;    
}

static bool ConfigFileBasedTest(const MARTe::char8 *const config) {
    using namespace MARTe;
    bool ok = InitialiseTestEnvironment(config);
    ReferenceT<PlainLoggerService> plainLoggerService;

    if(ok) {
        ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
        plainLoggerService = god->Find("LoggerService");
        ok = plainLoggerService.IsValid();
    }
 
    CleanupTestEnvironment();

    return ok;     
}

bool PlainLoggerServiceTest::TestInitialise_False_NoConsumers() {
    //Initialise shall fail if there are no consumers at all
    return !ConfigFileBasedTest(loggerTestConfig2);
}

bool PlainLoggerServiceTest::TestInitialise_False_NotLoggerConsumerI() {
    //Initialise shall fail if the consumer is an instance of the wrong class
    return !ConfigFileBasedTest(loggerTestConfig3);
}

bool PlainLoggerServiceTest::TestInitialise_False_TooManyPlainLoggerServiceInstances() {
    //Initialise shall fail if there are more than the maximum allowable PlainLoggerService
    return !ConfigFileBasedTest(loggerTestConfig4);
}

bool PlainLoggerServiceTest::TestExecute() {
    using namespace MARTe;
    bool ok = InitialiseTestEnvironment(loggerTestConfig1);
    ObjectRegistryDatabase *god;
    ReferenceT<PlainLoggerService> plainLoggerService;
    ReferenceT<PlainLoggerConsumerITest> consumer;

    if(ok) {
        god = ObjectRegistryDatabase::Instance();
        plainLoggerService = god->Find("LoggerService");
        ok = plainLoggerService.IsValid();
        printf("HEREA %d\r\n", ok);
    }

    if(ok) {
        consumer = god->Find("LoggerService.LoggerConsumerTest");
        ok = consumer.IsValid();
        printf("HEREB %d\r\n", ok);
    }
    if(ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "PlainLoggerConsumerITest test");
        ok = consumer->ok;
    }
 
    CleanupTestEnvironment();

    return ok;   
}

// bool PlainLoggerServiceTest::TestInitialise() {
//     using namespace MARTe;
//     PlainLoggerService test;
//     ConfigurationDatabase cdb;
//     ReferenceT<PlainLoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
//     bool ok = test.Insert(consumer);
//     ok &= test.Initialise(cdb);
//     return ok;
// }

// bool PlainLoggerServiceTest::TestInitialise_Defaults() {
//     using namespace MARTe;
//     PlainLoggerService test;
//     ConfigurationDatabase cdb;
//     ReferenceT<PlainLoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
//     bool ok = test.Insert(consumer);
//     ok &= test.Initialise(cdb);
//     ok &= (test.Size() == 1u);
//     return ok;
// }

// bool PlainLoggerServiceTest::TestInitialise_False_NoConsumers() {
//     using namespace MARTe;
//     PlainLoggerService test;
//     ConfigurationDatabase cdb;
//     return !test.Initialise(cdb);
// }

// bool PlainLoggerServiceTest::TestInitialise_False_NotLoggerConsumerI() {
//     using namespace MARTe;
//     PlainLoggerService test;
//     ConfigurationDatabase cdb;
//     ReferenceT<ReferenceContainer> rc(GlobalObjectsDatabase::Instance()->GetStandardHeap());
//     test.Insert(rc);
//     return !test.Initialise(cdb);
// }

// bool PlainLoggerServiceTest::TestExecute() {
//     using namespace MARTe;
//     PlainLoggerService test;
//     ConfigurationDatabase cdb;
//     ReferenceT<PlainLoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
//     bool ok = test.Insert(consumer);
//     ok &= test.Initialise(cdb);
//     Logger *logger = Logger::Instance();
//     LoggerPage *entry = logger->GetLogEntry();
//     //Empty all the entries
//     while (entry != NULL) {
//         logger->ReturnPage(entry);
//         entry = logger->GetLogEntry();
//     }
//     if (ok) {
//         REPORT_ERROR_STATIC(ErrorManagement::Debug, "PlainLoggerConsumerITest test");
//     }
//     if (ok) {
//         ok = consumer->ok;
//     }

//     return ok;
// }

