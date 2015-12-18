/**
 * @file SelectTest.cpp
 * @brief Source file for class SelectTest
 * @date 19/11/2015
 * @author Llorenç Capellà
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
 * the class SelectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SelectTest.h"
#include "Select.h"
#include "Threads.h"
#include "Sleep.h"
#include "BasicUDPSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static const char8 LOCALHOST_IP[] = "127.0.0.1";
static const uint16 ACTUAL_TESTING_PORT = 49155;//49152;
static const uint16 DUMMY_TESTING_PORT_1 = 49153;
static const uint16 DUMMY_TESTING_PORT_2 = 49154;

static void ThreadWrite(TimeoutType &defaultTo) {
    BasicUDPSocket bUDPsWrite;
    if (!bUDPsWrite.Open()) {
        return;
    }
    if (!bUDPsWrite.Connect(LOCALHOST_IP, ACTUAL_TESTING_PORT)) {
        return;
    }
    uint32 size = 3;
    Sleep::MSec(defaultTo.GetTimeoutMSec() / 2);
    bUDPsWrite.Write("Hey", size);
    bUDPsWrite.Close();
    return;
}

static void ThreadWriteFile(BasicFile &basicFile) {
    Sleep::MSec(100);
    uint32 size = StringHelper::Length("Write File\n");
    basicFile.Write("Write File\n", size);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

SelectTest::SelectTest() {
    retVal = true;
    defaultTo = 2000;
}

SelectTest::~SelectTest() {
}

bool SelectTest::TestDefaultConstructor() {
    BasicConsole bc1;
    bc1.Open(BasicConsoleMode::Default);
    bool ok = (!sel.IsSet(bc1));
    bc1.Close();
    return ok;
}

bool SelectTest::TestAddReadHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddReadHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddReadHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddReadHandle(bc);
    retVal &= !sel.AddReadHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddReadHandle_InvalidHandle() {
    retVal &= !sel.AddReadHandle(bf);
    return retVal;
}

bool SelectTest::TestAddWriteHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddWriteHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddWritedHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddWriteHandle(bc);
    retVal &= !sel.AddWriteHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddWritedHandle_InvalidHandle() {
    retVal &= !sel.AddWriteHandle(bf);
    return retVal;
}

bool SelectTest::TestAddExceptionHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddExceptionHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddExceptionHandle(bc);
    retVal &= !sel.AddExceptionHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_Invalidle() {
    retVal &= !sel.AddExceptionHandle(bf);
    return retVal;
}

bool SelectTest::TestRemoveReadHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddReadHandle(bc);
    retVal &= sel.RemoveReadHandle(bc);
    retVal &= !sel.IsSet(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveReadHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddReadHandle(bc);
    sel.RemoveReadHandle(bc);
    retVal &= !sel.RemoveReadHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveReadHandle_InvalidHandle() {
    retVal &= !sel.RemoveReadHandle(bf);
    return retVal;
}

bool SelectTest::TestRemoveWriteHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddWriteHandle(bc);
    retVal &= sel.RemoveWriteHandle(bc);
    retVal &= !sel.IsSet(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveWriteHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddWriteHandle(bc);
    sel.RemoveWriteHandle(bc);
    retVal &= !sel.RemoveWriteHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveWriteHandle_InvalidHandle() {
    retVal &= !sel.RemoveWriteHandle(bf);
    return retVal;
}

bool SelectTest::TestRemoveExceptionHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddExceptionHandle(bc);
    retVal &= sel.RemoveExceptionHandle(bc);
    retVal &= !sel.IsSet(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveExceptionHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddExceptionHandle(bc);
    sel.RemoveExceptionHandle(bc);
    retVal &= !sel.RemoveExceptionHandle(bc);
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveExceptionHandle_InvalidHandle() {
    retVal &= !sel.RemoveExceptionHandle(bf);
    return retVal;
}

bool SelectTest::TestClearAllHandles() {
#if ENVIRONMENT == Windows
    bc.Open(BasicConsoleMode::Default);
    retVal &= sel.AddReadHandle(bc);
    BasicFile bf;
    bf.Open("Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::FLAG_CREAT);
    retVal &= sel.AddReadHandle(bf);
#else
    retVal &= sel.AddWriteHandle(bc);
    retVal &= sel.AddExceptionHandle(bc);
    retVal &= sel.AddReadHandle(bc);
#endif
    sel.ClearAllHandles();
#if ENVIRONMENT == Windows
    retVal &= !sel.IsSet(bc);
    retVal &= !sel.IsSet(bf);
    bf.Close();
    bc.Close();
    DeleteFile("Test.txt");
#else
    retVal &= !sel.IsSet(bc);
#endif
    return retVal;
}

bool SelectTest::TestIsSet() {
    BasicFile bf;
    bf.Open("Test.txt", BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    retVal &= sel.AddReadHandle(bf);
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWriteFile, &bf);
    sel.WaitUntil(defaultTo);
    retVal &= sel.IsSet(bf);
    bf.Close();
    DeleteFile("Test.txt");
    return retVal;
}

bool SelectTest::TestWaitUntil_waitTimeout() {
    bc.Open(BasicConsoleMode::Default);
    sel.AddReadHandle(bc);
    retVal &= (0 == sel.WaitUntil(defaultTo));
    bc.Close();
    return retVal;
}

bool SelectTest::TestWaitUntil_waitRead() {
    BasicUDPSocket bUDPsRead;
    if (!bUDPsRead.Open()) {
        retVal = false;
        return retVal;
    }
    bUDPsRead.SetBlocking(true);
    if (!bUDPsRead.Listen(ACTUAL_TESTING_PORT)) {
        retVal = false;
        return retVal;
    }
    sel.AddReadHandle(bUDPsRead);
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWrite, &defaultTo);
    retVal &= (sel.WaitUntil(defaultTo) == 1);
    retVal &= sel.IsSet(bUDPsRead);
    char8 buffer[32];
    uint32 size = 32;
    bUDPsRead.Read(buffer, size);
    while (Threads::IsAlive(tid)) {
        Sleep::MSec(1);
    }
    bUDPsRead.Close();
    return retVal;
}

bool SelectTest::TestWaitUntil_severaDifferentWaitRead() {
    BasicUDPSocket bUDPsRead;
    BasicUDPSocket dummy1;
    BasicUDPSocket dummy2;

    if (!bUDPsRead.Open()) {
        retVal = false;
        return retVal;
    }
    if (!bUDPsRead.Listen(ACTUAL_TESTING_PORT)) {
        retVal = false;
        return retVal;
    }
    if (!dummy1.Open()) {
        retVal = false;
        return retVal;
    }
    if (!dummy1.Listen(DUMMY_TESTING_PORT_1)) {
        retVal = false;
        return retVal;
    }
    if (!dummy2.Open()) {
        retVal = false;
        return retVal;
    }
    if (!dummy2.Listen(DUMMY_TESTING_PORT_2)) {
        retVal = false;
        return retVal;
    }

    sel.AddReadHandle(bUDPsRead);
    sel.AddReadHandle(dummy1);
    sel.AddReadHandle(dummy2);
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWrite, &defaultTo);
    retVal &= (sel.WaitUntil(defaultTo) == 1);

    retVal &= sel.IsSet(bUDPsRead);
    retVal &= !sel.IsSet(dummy1);
    retVal &= !sel.IsSet(dummy2);
    while (Threads::IsAlive(tid)) {
        Sleep::MSec(1);
    }
    bUDPsRead.Close();
    dummy1.Close();
    dummy2.Close();
    return retVal;
}

bool SelectTest::TestWaitUntil_removeSomeWaitRead() {
    BasicUDPSocket bUDPsRead;
    BasicUDPSocket dummy1;
    BasicUDPSocket dummy2;

    if (!bUDPsRead.Open()) {
        retVal = false;
        return retVal;
    }
    if (!bUDPsRead.Listen(ACTUAL_TESTING_PORT)) {
        retVal = false;
        return retVal;
    }
    if (!dummy1.Open()) {
        retVal = false;
        return retVal;
    }
    if (!dummy1.Listen(DUMMY_TESTING_PORT_1)) {
        retVal = false;
        return retVal;
    }
    if (!dummy2.Open()) {
        retVal = false;
        return retVal;
    }
    if (!dummy2.Listen(DUMMY_TESTING_PORT_2)) {
        retVal = false;
        return retVal;
    }

    sel.AddReadHandle(bUDPsRead);
    sel.AddReadHandle(dummy1);
    sel.AddReadHandle(dummy2);
    sel.RemoveReadHandle(dummy2);
    sel.RemoveReadHandle(dummy1);
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWrite, &defaultTo);
    retVal &= (sel.WaitUntil(defaultTo) == 1);
    retVal &= sel.IsSet(bUDPsRead);
    retVal &= !sel.IsSet(dummy1);
    retVal &= !sel.IsSet(dummy2);
    while (Threads::IsAlive(tid)) {
        Sleep::MSec(1);
    }
    bUDPsRead.Close();
    dummy1.Close();
    dummy2.Close();
    return retVal;
}

bool SelectTest::TestWaitUntil_emptyList() {
    int32 retSel = sel.WaitUntil(defaultTo);
    return (retSel == -1);
}
