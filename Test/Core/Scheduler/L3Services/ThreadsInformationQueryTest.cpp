/**
 * @file ThreadsInformationQueryTest.cpp
 * @brief Source file for class ThreadsInformationQueryTest
 * @date 01/04/2019
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
 * the class ThreadsInformationQueryTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Sleep.h"
#include "Threads.h"
#include "ThreadsInformationQueryTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static MARTe::uint32 threadInformationQueryTestLock = 0u;
static void ThreadsInformationQueryTestCb(const void * const parameters) {
    threadInformationQueryTestLock = 1u;
    while (threadInformationQueryTestLock == 1u) {
        MARTe::Sleep::Sec(0.1);
    }
    threadInformationQueryTestLock = 1u;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ThreadsInformationQueryTest::TestExportData() {
    using namespace MARTe;
    Threads::BeginThread(ThreadsInformationQueryTestCb, NULL, THREADS_DEFAULT_STACKSIZE, "ThreadsInformationQueryTestCb");
    while (threadInformationQueryTestLock != 1u) {
        MARTe::Sleep::Sec(0.1);
    }
    ThreadsInformationQuery tinfoQuery;
    tinfoQuery.SetName("TInfoQuery");
    ConfigurationDatabase cdb;
    bool ok = tinfoQuery.ExportData(cdb);
    if (ok) {
        ok = cdb.MoveRelative("0");
    }
    if (ok) {
        StreamString test;
        ok = cdb.Read("Name", test);
        ok &= (test == "ThreadsInformationQueryTestCb");
    }
    if (ok) {
        uint32 affinity;
        ok = cdb.Read("Affinity", affinity);
    }
    if (ok) {
        uint32 pclass;
        ok = cdb.Read("PriorityClass", pclass);
    }
    if (ok) {
        uint32 state;
        ok = cdb.Read("State", state);
    }
    if (ok) {
        uint32 plevel;
        ok = cdb.Read("PriorityLevel", plevel);
    }
    threadInformationQueryTestLock = 2u;
    while (threadInformationQueryTestLock != 1u) {
        MARTe::Sleep::Sec(0.1);
    }
    ConfigurationDatabase cdb2;
    ok &= tinfoQuery.ExportData(cdb2);
    if (ok) {
        ok = !cdb2.MoveRelative("0");
    }
    return ok;
}

