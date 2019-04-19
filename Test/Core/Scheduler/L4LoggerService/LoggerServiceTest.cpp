/**
 * @file LoggerServiceTest.cpp
 * @brief Source file for class LoggerServiceTest
 * @date 10/03/2017
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
 * the class LoggerServiceTest (public, protected, and private). Be aware that some 
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
#include "LoggerServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * A dummy logger to the the LoggerService
 */
class LoggerConsumerITest: public MARTe::Object, public MARTe::LoggerConsumerI {
public:
    CLASS_REGISTER_DECLARATION()
    LoggerConsumerITest() : Object() {
        ok = false;
    }

    virtual ~LoggerConsumerITest() {

    }

    virtual void ConsumeLogMessage(MARTe::LoggerPage *logPage) {
        //Test local variable needed otherwise racing condition might happen here.
        bool test = (logPage->errorInfo.header.errorType == MARTe::ErrorManagement::Debug);
        MARTe::StreamString err = logPage->errorStrBuffer;
        test &= (err == "LoggerConsumerITest test");
        ok = test;
    }

    bool ok;
};
CLASS_REGISTER(LoggerConsumerITest, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool LoggerServiceTest::TestConstructor() {
    using namespace MARTe;
    LoggerService test;
    return (test.Size() == 0u);
}

bool LoggerServiceTest::TestInitialise() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = test.Insert(consumer);
    ok &= test.Initialise(cdb);
    ok &= (test.GetCPUMask() == 0x2);
    ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE * 2));
    ok &= (test.GetNumberOfLogPages() == (DEFAULT_NUMBER_OF_LOG_PAGES));
    ok &= (test.Size() == 1u);
    return ok;
}

bool LoggerServiceTest::TestInitialise_Defaults() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = test.Insert(consumer);
    ok &= test.Initialise(cdb);
    ok &= (test.GetCPUMask() == 0x2);
    ok &= (test.GetStackSize() == (THREADS_DEFAULT_STACKSIZE));
    ok &= (test.GetNumberOfLogPages() == (DEFAULT_NUMBER_OF_LOG_PAGES));
    ok &= (test.Size() == 1u);
    return ok;
}

bool LoggerServiceTest::TestInitialise_False_NoCPUs() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test.Insert(consumer);
    return !test.Initialise(cdb);
}

bool LoggerServiceTest::TestInitialise_False_StackSize_Zero() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", 0);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test.Insert(consumer);
    return !test.Initialise(cdb);
}

bool LoggerServiceTest::TestInitialise_False_NumberOfLoggerPages_Zero() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", 32768);
    cdb.Write("NumberOfLogPages", 0);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test.Insert(consumer);
    return !test.Initialise(cdb);
}

bool LoggerServiceTest::TestInitialise_False_NoConsumers() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    return !test.Initialise(cdb);
}

bool LoggerServiceTest::TestInitialise_False_NotLoggerConsumerI() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    ReferenceT<ReferenceContainer> rc(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    test.Insert(rc);
    return !test.Initialise(cdb);
}

bool LoggerServiceTest::TestGetNumberOfLogPages() {
    return TestInitialise();
}

bool LoggerServiceTest::TestGetCPUMask() {
    return TestInitialise();
}

bool LoggerServiceTest::TestGetStackSize() {
    return TestInitialise();
}

bool LoggerServiceTest::TestExecute() {
    using namespace MARTe;
    LoggerService test;
    ConfigurationDatabase cdb;
    cdb.Write("CPUs", 0x2);
    cdb.Write("StackSize", THREADS_DEFAULT_STACKSIZE * 2);
    cdb.Write("NumberOfLogPages", DEFAULT_NUMBER_OF_LOG_PAGES);
    ReferenceT<LoggerConsumerITest> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = test.Insert(consumer);
    ok &= test.Initialise(cdb);
    Logger *logger = Logger::Instance();
    LoggerPage *entry = logger->GetLogEntry();
    //Empty all the entries
    while (entry != NULL) {
        logger->ReturnPage(entry);
        entry = logger->GetLogEntry();
    }
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "LoggerConsumerITest test");
    }
    if (ok) {
        uint32 maxWait = 2000;
        uint32 i;
        ok = consumer->ok;
        for (i = 0; (i < maxWait) && (!ok); i++) {
            ok = consumer->ok;
            Sleep::Sec(1e-3);
        }
    }
    return ok;
}

