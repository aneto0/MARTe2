/**
 * @file LoggerConsumerITest.cpp
 * @brief Source file for class LoggerConsumerITest
 * @date 13/03/2017
 * @author Andre Neto
 * @author Edward Jones 
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
 * the class LoggerConsumerITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "LoggerConsumerI.h"
#include "LoggerConsumerITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class LoggerConsumerITestHelper: public MARTe::LoggerConsumerI {
public:
    virtual void ConsumeLogMessage(MARTe::LoggerPage *logPage) {
    }

    void PrintToStream(MARTe::LoggerPage *logPage, MARTe::BufferedStreamI &err) const {
        MARTe::LoggerConsumerI::PrintToStream(logPage, err);
    }

    bool LoadPrintPreferences(MARTe::StructuredDataI &data) {
        return MARTe::LoggerConsumerI::LoadPrintPreferences(data);
    }

    bool IsPrintKeys() const {
        return MARTe::LoggerConsumerI::IsPrintKeys();
    }

    bool IsInfo() {
        return GetFormatPreferences().info;
    }

    bool IsTimeShort() {
        return GetFormatPreferences().timeShort;
    }

    bool IsTimeFull() {
        return GetFormatPreferences().timeFull;
    }

    bool IsObjectName() {
        return GetFormatPreferences().objectName;
    }

    bool IsObjectPointer() {
        return GetFormatPreferences().objectPointer;
    }

    bool IsFunctionName() {
        return GetFormatPreferences().functionName;
    }

    bool IsFileName() {
        return GetFormatPreferences().fileName;
    }

    bool IsMessage() {
        return GetFormatPreferences().message;
    }

    bool IsThreadId() {
        return GetFormatPreferences().threadId;
    }

    bool IsClassName() {
        return GetFormatPreferences().className;
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool LoggerConsumerITest::TestLoadPrintPreferences() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "ETtOoFfRmC");
    bool ok = test.LoadPrintPreferences(cdb);
    ok &= test.IsInfo();
    ok &= test.IsTimeShort();
    ok &= test.IsTimeFull();
    ok &= test.IsObjectName();
    ok &= test.IsObjectPointer();
    ok &= test.IsFunctionName();
    ok &= test.IsFileName();
    ok &= test.IsMessage();
    ok &= test.IsThreadId();
    ok &= test.IsClassName();
    return ok;
}

bool LoggerConsumerITest::TestLoadPrintPreferences_SomeKeys() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EtofmC");
    bool ok = test.LoadPrintPreferences(cdb);
    ok &= test.IsInfo();
    ok &= !test.IsTimeShort();
    ok &= test.IsTimeFull();
    ok &= !test.IsObjectName();
    ok &= test.IsObjectPointer();
    ok &= test.IsFunctionName();
    ok &= !test.IsFileName();
    ok &= test.IsMessage();
    ok &= !test.IsThreadId();
    ok &= test.IsClassName();
    return ok;
}

bool LoggerConsumerITest::TestLoadPrintPreferences_False_TooLarge() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "ETtOoFfRmCQ");
    return !test.LoadPrintPreferences(cdb);
}

bool LoggerConsumerITest::TestLoadPrintPreferences_False_InvalidKey() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "EQ");
    return !test.LoadPrintPreferences(cdb);
}

bool LoggerConsumerITest::TestPrintToStream() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "ETtOoFfRmC");
    cdb.Write("PrintKeys", 0);
    bool ok = test.LoadPrintPreferences(cdb);
    if (ok) {
        ok = !test.IsPrintKeys();
    }
    if (ok) {
        LoggerPage page;
        page.errorInfo.className = "LoggerConsumerITest";
        page.errorInfo.fileName = "LoggerConsumerITest.cpp";
        page.errorInfo.functionName = "TestPrintToStream";
        page.errorInfo.header.errorType = ErrorManagement::Debug;
        page.errorInfo.header.isObject = true;
        page.errorInfo.header.lineNumber = 163;
        page.errorInfo.hrtTime = 1000000000;
        page.errorInfo.timeSeconds = 123;
        page.errorInfo.objectName = "Obj1";
        page.errorInfo.objectPointer = (const void *) 0xAABBCCDD;
        StringHelper::Copy(&page.errorStrBuffer[0], "LoggerConsumerITest::TestPrintToStream\0");
        StreamString err;
        test.PrintToStream(&page, err);
        //printf("%s\n", err.Buffer());
        //Note that the Thread identified is currently not being handled.
        //Depending on the localtime, this test can fail due to people working in other timezones
        //Adding a method to check the time and instead use a non-static version of getHours should fix this
        TimeStamp ts;
        ts.ConvertFromEpoch(static_cast<oslong>(page.errorInfo.timeSeconds));
        StreamString expectedPrint;
        if (sizeof(void *) == 8u) {
            expectedPrint.Printf("|Debug|1000000000|%d:2:3 (1000000000)|Obj1|0x00000000AABBCCDD|LoggerConsumerITest||TestPrintToStream|LoggerConsumerITest.cpp:163|LoggerConsumerITest::TestPrintToStream", ts.GetHour());
        }
        else {
            expectedPrint.Printf("|Debug|1000000000|%d:2:3 (1000000000)|Obj1|0xAABBCCDD|LoggerConsumerITest||TestPrintToStream|LoggerConsumerITest.cpp:163|LoggerConsumerITest::TestPrintToStream", ts.GetHour());
        }
        ok = (err == expectedPrint);
    }

    return ok;
}

bool LoggerConsumerITest::TestPrintToStream_WithKeys() {
    using namespace MARTe;
    LoggerConsumerITestHelper test;
    ConfigurationDatabase cdb;
    cdb.Write("Format", "ETtOoFfRmC");
    cdb.Write("PrintKeys", 1);
    bool ok = test.LoadPrintPreferences(cdb);
    if (ok) {
        ok = test.IsPrintKeys();
    }
    if (ok) {
        LoggerPage page;
        page.errorInfo.className = "LoggerConsumerITest";
        page.errorInfo.fileName = "LoggerConsumerITest.cpp";
        page.errorInfo.functionName = "TestPrintToStream";
        page.errorInfo.header.errorType = ErrorManagement::Debug;
        page.errorInfo.header.isObject = true;
        page.errorInfo.header.lineNumber = 163;
        page.errorInfo.hrtTime = 1000000000;
        page.errorInfo.timeSeconds = 123;
        page.errorInfo.objectName = "Obj1";
        page.errorInfo.objectPointer = (const void *) 0xAABBCCDD;
        StringHelper::Copy(&page.errorStrBuffer[0], "LoggerConsumerITest::TestPrintToStream\0");
        StreamString err;
        test.PrintToStream(&page, err);
        //printf("%s\n", err.Buffer());
        //Note that the Thread identified is currently not being handled.
        //Depending on the localtime, this test can fail due to people working in other timezones
        //Adding a method to check the time and instead use a non-static version of getHours should fix this
        TimeStamp ts;
        ts.ConvertFromEpoch(static_cast<oslong>(page.errorInfo.timeSeconds));
        StreamString expectedPrint;
        if (sizeof(void *) == 8u) {
            expectedPrint.Printf("|E=Debug|TM=1000000000|TM=%d:2:3 (1000000000)|o=Obj1|O=0x00000000AABBCCDD|C=LoggerConsumerITest|T=|f=TestPrintToStream|F=LoggerConsumerITest.cpp:163|D=LoggerConsumerITest::TestPrintToStream", ts.GetHour());
        }
        else {
            expectedPrint.Printf("|E=Debug|TM=1000000000|TM=%d:2:3 (1000000000)|o=Obj1|O=0xAABBCCDD|C=LoggerConsumerITest|T=|f=TestPrintToStream|F=LoggerConsumerITest.cpp:163|D=LoggerConsumerITest::TestPrintToStream", ts.GetHour());
        }
        ok = (err == expectedPrint);
    }

    return ok;
}
