/**
 * @file FileTest.cpp
 * @brief Source file for class FileTest
 * @date 16/11/2016
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
 * the class FileTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Directory.h"
#include "DirectoryScanner.h"
#include "FileTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//Verifies if the template can solve the stream interface.
static bool CheckStream(MARTe::StreamI &stream) {
    return true;
}

static bool CheckFile(MARTe::BasicFile &file) {
    return true;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
FileTest::FileTest() {
    using namespace MARTe;
    DirectoryScanner toDelete;
    toDelete.Scan("./", "_Test.txt");
    for (uint32 i = 0u; i < toDelete.ListSize(); i++) {
        Directory* entry = (Directory*) (toDelete.ListPeek(i));
        entry->Delete();
    }
}

bool FileTest::TestConstructor() {
    MARTe::File f;
    return true;
}

bool FileTest::TestConstructor_Stream() {
    MARTe::File f;
    return CheckStream(f);
}

bool FileTest::TestConstructor_BasicFile() {
    MARTe::File f;
    return CheckFile(f);
}

bool FileTest::TestRead() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    uint32 size = 5;
    bool ok = f.Write("Test\0", size);
    ok &= f.Flush();
    ok &= f.Seek(0);
    char8 buffer[5];
    size = 5;
    ok &= f.Read(buffer, size);
    f.Close();
    ok &= (StringHelper::Compare(buffer, "Test") == 0);
    return ok;
}

bool FileTest::TestRead_Timeout() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    uint32 size = 5;
    bool ok = f.Write("Test\0", size, TTInfiniteWait);
    ok &= f.Flush();
    f.Seek(0);
    char8 buffer[5];
    ok &= f.Read(buffer, size, TTInfiniteWait);
    f.Close();
    ok &= (StringHelper::Compare(buffer, "Test") == 0);
    return ok;
}

bool FileTest::TestWrite() {
    return TestRead();
}

bool FileTest::TestWrite_Timeout() {
    return TestRead_Timeout();
}

bool FileTest::TestSize() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    uint32 size = 5;
    f.Write("Test\0", size);
    f.Flush();
    f.Seek(0);
    bool ok = (f.Size() == 5);
    f.Close();
    return ok;
}

bool FileTest::TestSeek() {
    return TestSize();
}

bool FileTest::TestRelativeSeek() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    uint32 size = 5;
    f.Write("Test\0", size);
    f.Flush();
    f.Seek(0);
    bool ok = f.RelativeSeek(2);
    char8 buffer[3];
    size = 3;
    f.Read(buffer, size);
    f.Close();
    ok &= (StringHelper::Compare(buffer, "st") == 0);
    return ok;
}

bool FileTest::TestPosition() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    uint32 size = 5;
    f.Write("Test\0", size);
    f.Flush();
    f.Seek(0);
    bool ok = f.RelativeSeek(2);
    ok &= (f.Position() == 2);
    f.Close();
    return ok;
}

bool FileTest::TestSetSize() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    bool ok = (f.SetSize(10));
    f.Close();
    return ok;
}

bool FileTest::TestCanWrite() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    bool ok = f.CanWrite();
    f.Close();
    return ok;
}

bool FileTest::TestCanSeek() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    bool ok = f.CanSeek();
    f.Close();
    return ok;
}

bool FileTest::TestCanRead() {
    using namespace MARTe;
    File f;
    f.Open("_Test.txt", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_CREAT);
    bool ok = f.CanRead();
    f.Close();
    return ok;
}
