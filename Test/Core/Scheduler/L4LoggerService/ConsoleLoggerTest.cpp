/**
 * @file ConsoleLoggerTest.cpp
 * @brief Source file for class ConsoleLoggerTest
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
 * the class ConsoleLoggerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "ConsoleLogger.h"
#include "LoggerService.h"
#include "ConsoleLoggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ConsoleLoggerTest::TestConstructor() {
    using namespace MARTe;
    ConsoleLogger test;
    return (test.GetName() == NULL);
}

bool ConsoleLoggerTest::TestInitialise() {
    using namespace MARTe;
    ConsoleLogger test;
    ConfigurationDatabase cdb;
    cdb.Write("PrintFunctionName", 1);
    cdb.Write("PrintExpandedTime", 1);
    return test.Initialise(cdb);
}

bool ConsoleLoggerTest::TestConsumeLogMessage() {
    using namespace MARTe;
    LoggerService test;
    ReferenceT<ConsoleLogger> consumer(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Format", "EtOofFRmC");
    cdb2.Write("PrintKeys", 1);
    bool ok = consumer->Initialise(cdb2);
    ConfigurationDatabase cdb;
    test.Insert(consumer);
    cdb.Write("CPUs", 0x2);
    test.Initialise(cdb);
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "TestConsumeLogMessage");
    Sleep::Sec(0.2);

    return ok;
}

