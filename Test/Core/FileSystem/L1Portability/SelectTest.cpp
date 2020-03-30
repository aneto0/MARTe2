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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocket.h"
#include "Directory.h"
#include "SelectTest.h"
#include "Sleep.h"
#include "Threads.h"
#include "ErrorType.h"
#include "CompositeErrorManagement.h"
#include "ErrorManagement.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static const char8 LOCALHOST_IP[] = "127.0.0.1";
static const uint16 ACTUAL_TESTING_PORT = 49152;
static const uint16 DUMMY_TESTING_PORT_1 = 49153;
static const uint16 DUMMY_TESTING_PORT_2 = 49154;

static void ThreadWrite(MilliSeconds *defaultTo) {
	ErrorManagement::ErrorType ret;
    BasicUDPSocket bUDPsWrite;
    bool opened = bUDPsWrite.Open();
    ret.OSError = !opened;
    REPORT_ERROR(ret,"bUDPsWrite.Open() failed");

    if (ret){
    	ret.OSError = !bUDPsWrite.Connect(LOCALHOST_IP, ACTUAL_TESTING_PORT);
        COMPOSITE_REPORT_ERROR(ret,"bUDPsWrite.Connect(",LOCALHOST_IP,",",ACTUAL_TESTING_PORT,") failed");
    }
    uint32 size = 3;
    if (ret){
    	Sleep::Short(*defaultTo * 0.5);
    	ret.OSError = !bUDPsWrite.Write("Hey", size);
        REPORT_ERROR(ret,"bUDPsWrite.Write() failed");
    }
    if (opened){
        bUDPsWrite.Close();
    }
    return;
}

static void ThreadWriteFile(BasicFile *basicFile) {
    Sleep::Short(100,Units::ms);
    CCString message("Write File\n");
    uint32 size = message.GetSize();
    basicFile->Write("Write File\n", size);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

SelectTest::SelectTest() {
    retVal = true;
    defaultTo = MilliSeconds(2000,Units::ms);
}

SelectTest::~SelectTest() {
}

bool SelectTest::TestDefaultConstructor() {
    BasicConsole bc1;
    bc1.Open(BasicConsoleMode::Default);
    bool ok = (!sel.IsSet(bc1.GetWriteHandle()));
    bc1.Close();
    return ok;
}

bool SelectTest::TestAddReadHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddReadHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddReadHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddReadHandle(bc.GetReadHandle());
    retVal &= !sel.AddReadHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddReadHandle_InvalidHandle() {
    retVal &= !sel.AddReadHandle(bf.GetReadHandle());
    return retVal;
}

bool SelectTest::TestAddWriteHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddWriteHandle(bc.GetWriteHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddWritedHandle_SameHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddWriteHandle(bc.GetWriteHandle());
    retVal &= !sel.AddWriteHandle(bc.GetWriteHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddWritedHandle_InvalidHandle() {
    retVal = !sel.AddReadHandle(bf.GetReadHandle());
    return retVal;
}

bool SelectTest::TestAddExceptionHandle() {
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddExceptionHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_SameHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    retVal = sel.AddExceptionHandle(bc.GetReadHandle());
    retVal &= !sel.AddExceptionHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestAddExceptionHandle_Invalidle() {
	BasicFile bf;
    return !sel.AddExceptionHandle(bf.GetReadHandle());
}

bool SelectTest::TestRemoveReadHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddReadHandle(bc.GetReadHandle());
    retVal = sel.RemoveReadHandle(bc.GetReadHandle());
    retVal &= !sel.IsSet(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveReadHandle_SameHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddReadHandle(bc.GetReadHandle());
    sel.RemoveReadHandle(bc.GetReadHandle());
    retVal = !sel.RemoveReadHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveReadHandle_InvalidHandle() {
	BasicFile bf;
    return !sel.RemoveReadHandle(bf.GetReadHandle());
}

bool SelectTest::TestRemoveWriteHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddWriteHandle(bc.GetWriteHandle());
    retVal = sel.RemoveWriteHandle(bc.GetWriteHandle());
    retVal &= !sel.IsSet(bc.GetWriteHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveWriteHandle_SameHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddWriteHandle(bc.GetWriteHandle());
    sel.RemoveWriteHandle(bc.GetWriteHandle());
    retVal = !sel.RemoveWriteHandle(bc.GetWriteHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveWriteHandle_InvalidHandle() {
	BasicFile bf;
    return !sel.RemoveWriteHandle(bf.GetWriteHandle());
}

bool SelectTest::TestRemoveExceptionHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddExceptionHandle(bc.GetReadHandle());
    retVal = sel.RemoveExceptionHandle(bc.GetReadHandle());
    retVal &= !sel.IsSet(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveExceptionHandle_SameHandle() {
	BasicConsole bc;
    bc.Open(BasicConsoleMode::Default);
    sel.AddExceptionHandle(bc.GetReadHandle());
    sel.RemoveExceptionHandle(bc.GetReadHandle());
    retVal = !sel.RemoveExceptionHandle(bc.GetReadHandle());
    bc.Close();
    return retVal;
}

bool SelectTest::TestRemoveExceptionHandle_InvalidHandle() {
    retVal = !sel.RemoveExceptionHandle(bf.GetReadHandle());
    return retVal;
}

bool SelectTest::TestClearAllHandles() {
	ErrorManagement::ErrorType ret;
    Directory dir;
    BasicFile bf;
    BasicConsole bc;
    bool fopened = false;
    bool opened = bc.Open(BasicConsoleMode::Default);
    ret.OSError= !opened;
    REPORT_ERROR(ret,"BasicConsoleMode::Open failed");

    if (ret) {
    	fopened = bf.Open("Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::FLAG_CREAT);
        REPORT_ERROR(ret,"BasicFile::Open failed");
        ret.OSError= !fopened;
    }
    if (ret) {
    	ret.OSError = !sel.AddReadHandle(bc.GetReadHandle());
        REPORT_ERROR(ret,"AddReadHandle failed");
    }
    if (ret) {
    	ret.OSError = !sel.AddReadHandle(bf.GetReadHandle());
        REPORT_ERROR(ret,"AddReadHandle failed");
    }
    if (ret) {
    	ret.OSError = !sel.AddWriteHandle(bc.GetWriteHandle());
        REPORT_ERROR(ret,"AddWriteHandle failed");
    }
    if (ret) {
    	ret.OSError = !sel.AddReadHandle(bf.GetReadHandle());
        REPORT_ERROR(ret,"AddWriteHandle failed");
    }
    if (ret) {
    	ret.OSError = !sel.AddExceptionHandle(bc.GetReadHandle());
        REPORT_ERROR(ret,"AddExceptionHandle failed");
    }
    if (ret) {
    	ret.OSError = !sel.AddExceptionHandle(bf.GetReadHandle());
        REPORT_ERROR(ret,"AddExceptionHandle failed");
    }
    if (ret) {
    	sel.ClearAllHandles();
    }

    if (ret) {
    	ret.internalSetupError = sel.IsSet(bc.GetReadHandle());
        REPORT_ERROR(ret,"IsSet succeeded");
    }

    if (ret) {
    	ret.internalSetupError = sel.IsSet(bf.GetReadHandle());
        REPORT_ERROR(ret,"IsSet succeeded");
    }

    if (ret) {
    	ret.internalSetupError = sel.IsSet(bc.GetWriteHandle());
        REPORT_ERROR(ret,"IsSet succeeded");
    }

    if (ret) {
    	ret.internalSetupError = sel.IsSet(bf.GetWriteHandle());
        REPORT_ERROR(ret,"IsSet succeeded");
    }

    if (opened){
    	bc.Close();
    }
    if (fopened){
    	bf.Close();
    }
    return retVal;
}

bool SelectTest::TestIsSet() {
    Directory d;
    BasicFile bf;
    bf.Open("Test.txt", BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    retVal &= sel.AddReadHandle(bf.GetReadHandle());
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWriteFile, &bf);
    sel.WaitUntil(defaultTo);
    retVal &= sel.IsSet(bf.GetReadHandle());
    while (Threads::IsAlive(tid)) {
        MilliSeconds(1,Units::ms);
    }
    bf.Close();
    d.SetByName("Test.txt");
    d.Delete();
    return retVal;
}

bool SelectTest::TestWaitUntil_waitTimeout() {
	ErrorManagement::ErrorType ret;
	BasicUDPSocket bc;
	Select sel;
	ret.OSError = !bc.Open();
	REPORT_ERROR(ret,"BasicUDPSocket::Open failed");
//    ret.OSError = !bc.Open(BasicConsoleMode::Default);
//    REPORT_ERROR(ret,"BasicConsole::Open failed");

    char buffer[256];
    uint32 size= sizeof(buffer);
    bc.Read(buffer,size);

    if (ret){
        ret.fatalError = !sel.AddReadHandle(bc.GetSocket());
        REPORT_ERROR(ret,"AddReadHandle failed");
    }

    if (ret){
        ret.fatalError = (0 != sel.WaitUntil(defaultTo));
        REPORT_ERROR(ret,"WaitUntil succeeded when it should have been a timeout");
    }

    bc.Close();
    return ret;
}

bool SelectTest::TestWaitUntil_waitRead(MilliSeconds timeout) {
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
    retVal &= (sel.WaitUntil(timeout) == 1);
    retVal &= sel.IsSet(bUDPsRead);
    char8 buffer[32];
    uint32 size = 32;
    bUDPsRead.Read(buffer, size);
    while (Threads::IsAlive(tid)) {
        MilliSeconds(1,Units::ms);
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
        MilliSeconds(1,Units::ms);
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

    ErrorManagement::ErrorType ret;

    ret.fatalError = !bUDPsRead.Open();
    REPORT_ERROR(ret,"bUDPsRead.Open failed");

    if (ret){
    	ret.fatalError = !dummy1.Open();
        REPORT_ERROR(ret,"dummy1.Open failed");
    }

    if (ret){
    	ret.fatalError = !dummy2.Open();
        REPORT_ERROR(ret,"dummy2.Open failed");
    }

    if (ret){
    	ret.fatalError = !bUDPsRead.Listen(ACTUAL_TESTING_PORT);
        COMPOSITE_REPORT_ERROR(ret,"bUDPsRead.Listen(",ACTUAL_TESTING_PORT,") failed");
    }

    if (ret){
    	ret.fatalError = !dummy1.Listen(DUMMY_TESTING_PORT_1);
        COMPOSITE_REPORT_ERROR(ret,"dummy1.Listen(",DUMMY_TESTING_PORT_1,") failed");
    }

    if (ret){
    	ret.fatalError = !dummy2.Listen(DUMMY_TESTING_PORT_2);
        COMPOSITE_REPORT_ERROR(ret,"dummy2.Listen(",DUMMY_TESTING_PORT_2,") failed");
    }

    if (ret){
    	ret.fatalError = !sel.AddReadHandle(bUDPsRead);
        REPORT_ERROR(ret,"sel.AddReadHandle(bUDPsRead) failed");
    }

    if (ret){
    	ret.fatalError = !sel.AddReadHandle(dummy1);
        REPORT_ERROR(ret,"sel.AddReadHandle(dummy1) failed");
    }

    if (ret){
    	ret.fatalError = !sel.AddReadHandle(dummy2);
        REPORT_ERROR(ret,"sel.AddReadHandle(dummy2) failed");
    }

    if (ret){
    	ret.fatalError = !sel.RemoveReadHandle(dummy2);
        REPORT_ERROR(ret,"sel.RemoveReadHandle(dummy2) failed");
    }

    if (ret){
    	ret.fatalError = !sel.RemoveReadHandle(dummy1);
        REPORT_ERROR(ret,"sel.RemoveReadHandle(dummy1) failed");
    }

    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadWrite, &defaultTo);
    Threads::SetPriority(tid,Threads::NormalPriorityClass,8U);


    if (ret){
    	ret.fatalError = (sel.WaitUntil(defaultTo) != 1);
        REPORT_ERROR(ret,"sel.WaitUntil(defaultTo) != 1");
    }

    if (ret){
    	bool s1 = sel.IsSet(bUDPsRead);
    	bool s2 = sel.IsSet(dummy1);
    	bool s3 = sel.IsSet(dummy2);
    	ret.fatalError = !s1 || s2 || s3;
        COMPOSITE_REPORT_ERROR(ret,"sel.IsSet failed: bUDPsRead= ",s1," dummy1= ",s2," dummy2= ",s3);
    }

    while (Threads::IsAlive(tid)) {
        MilliSeconds(1,Units::ms);
    }

    bUDPsRead.Close();
    dummy1.Close();
    dummy2.Close();
    return ret;
}

bool SelectTest::TestWaitUntil_emptyList() {
    int32 retSel = sel.WaitUntil(defaultTo);
    return (retSel == -1);
}
