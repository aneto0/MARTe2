/**
 * @file BasicFileTest.cpp
 * @brief Source file for class TestBasicFile
 * @date 04/11/2015
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
 * the class TestBasicFile (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicFileTest.h"
#include "StringHelper.h"
#include "iostream"
using namespace std;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicFileTest::BasicFileTest() :
        size(9) {
    retVal = true;
    defaultRWFlags = BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W;
    defaultWFlags = BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W;
    defaultRFlags = BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R;
    str1 = "../TargetFile.txt";
    str2 = "abcdefghi";
    nameFileTarget = str1.Buffer();
    stringTarget = str2.Buffer();
}

BasicFileTest::~BasicFileTest() {
    // Auto-generated destructor stub for TestBasicFile
    // TODO Verify if manual additions are needed
}

bool BasicFileTest::TestDefaultConstructor() {
    String char1 = "";
    String char2 = bf.GetPathName();
    retVal &= (char1 == char2);
    retVal &= !bf.CanRead();
    retVal &= !bf.CanSeek();
    retVal &= !bf.CanWrite();
    retVal &= !bf.IsOpen();
    retVal &= (bf.GetFlags() == 0xFFFFFFFF);

    return retVal;
}

bool BasicFileTest::TestCopyConstructor() {
    BasicFile copybf(bf);
    String char1 = bf.GetPathName();
    String char2 = copybf.GetPathName();
    retVal &= (char1 == char2);
    retVal &= !copybf.CanRead();
    retVal &= !copybf.CanSeek();
    retVal &= !copybf.CanWrite();
    retVal &= !copybf.IsOpen();
    retVal &= (copybf.GetFlags() == 0xFFFFFFFF);

    return retVal;
}

bool BasicFileTest::TestAssignmentOperator() {
    BasicFile copybf;
    const char8 * writeString = str2.Buffer();
    uint32 size1 = 3;
    char8 buffer[10];
    char8 * const readString = buffer;
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    copybf = bf;
    bf.Write(writeString, size1);
    //Move back the pointer (after write it was moved forward)
    copybf.Seek(0);
    copybf.Read(readString, size1);
    retVal &= ((StringHelper::CompareN(readString, writeString, size1)) == 0);
    bf.Close();
    copybf.Close();
    return retVal;
}

bool BasicFileTest::TestSetFlags() {
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    //First check that the flag is not set to APPEND
    uint32 retFlags = bf.GetFlags();
    retVal &= ((retFlags & BasicFile::FLAG_APPEND) == BasicFile::FLAG_APPEND);
    //Remove the BasicFile::FLAG_APPEND
    bf.SetFlags(0);
    retFlags = bf.GetFlags();
    retVal &= !((retFlags & BasicFile::FLAG_APPEND) == BasicFile::FLAG_APPEND);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestSetFlags_Close() {
    return !bf.SetFlags(0U);
}

bool BasicFileTest::TestGetFlags() {
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == (BasicFile::FLAG_APPEND | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestGetFlags_defaultWFlags() {
    bf.Open("../BasicFileTest.txt", defaultWFlags);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == (BasicFile::FLAG_APPEND | BasicFile::ACCESS_MODE_W));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestGetFlags_defaultRFlags() {
    bf.Open("../BasicFileTest.txt", defaultRFlags);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == (BasicFile::ACCESS_MODE_R));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanRead_OnClosedFile() {
    return !bf.CanRead();
}

bool BasicFileTest::TestCanRead_OnOpenedReadbleFile() {
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal = bf.CanRead();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanRead_OnOpenWritableFile() {
    bf.Open("../BasicFileTest.txt", defaultWFlags);
    retVal = !bf.CanRead();
    bf.Close();
    return !bf.CanRead();
}

bool BasicFileTest::TestCanWrite_Close() {
    return !bf.CanWrite();
}

bool BasicFileTest::TestCanWrite_OpenRead() {
    bf.Open("../BasicFileTest.txt", defaultRFlags);
    retVal = !bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanWrite_OpenWrite() {
    bf.Open("../BasicFileTest.txt", defaultWFlags);
    retVal = bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanSeek_Close() {
    return bf.CanSeek();
}

bool BasicFileTest::TestCanSeek_Open() {
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal &= bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen() {
    retVal &= bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal &= bf.CanRead();
    retVal &= bf.CanWrite();
    retVal &= bf.CanSeek();
    retVal &= bf.IsOpen();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_FLAG_CREAT_EXCLUSIVE() {
    retVal &= bf.Open("../TestOpen_FLAG_CREAT_EXCLUSIVE.txt", BasicFile::FLAG_CREAT_EXCLUSIVE | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W);
    retVal &= bf.CanRead();
    retVal &= bf.CanWrite();
    retVal &= bf.CanSeek();
    retVal &= bf.IsOpen();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_twice() {
    retVal &= bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal &= !bf.Open("../BasicFileTest.txt", defaultRWFlags);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags1() {
    //If not exist create the file
    bf.Open("../TestOpen_IncompatibleFlags1.txt", defaultRWFlags);
    bf.Close();
    //If the file exist and the activated flag is FLAG_CREAT_EXCLUSIVE the Open should fail. If it doesn't fail means FLAG_CREAT is activated.
    retVal &= bf.Open("../TestOpen_IncompatibleFlags1.txt", BasicFile::FLAG_CREAT | BasicFile::FLAG_CREAT_EXCLUSIVE);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags2() {
    //If not exist create the file
    bf.Open("../TestOpen_IncompatibleFlags2.txt", defaultRWFlags);
    bf.Close();
    //The open should fail because the file already exists.
    retVal &= !(bf.Open("../TestOpen_IncompatibleFlags2.txt", BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT_EXCLUSIVE));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestIsOpen() {
    retVal &= !bf.IsOpen();
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal &= bf.IsOpen();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestClose() {
    bf.Open("../BasicFileTest.txt", defaultRWFlags);
    retVal &= bf.Close();
    retVal &= !bf.CanRead();
    retVal &= !bf.CanWrite();
    retVal &= !bf.CanSeek();
    retVal &= !bf.IsOpen();
    retVal &= !bf.Close();
    return retVal;
}

bool BasicFileTest::TestWrite() {
    uint32 oldSize = size;
    uint32 newSize = size;
    bf.Open(nameFileTarget, BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W);
    retVal &= bf.Write(stringTarget, newSize);
    retVal &= (oldSize >= newSize);
    retVal &= (bf.Position() == newSize);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestWrite_close() {
    uint32 NewSize = size;
    return !bf.Write(stringTarget, NewSize);
}

bool BasicFileTest::TestWrite_timeout() {
    uint32 oldSize = size;
    uint32 newSize = size;
    bf.Open(nameFileTarget, BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W);
    retVal &= bf.Write(stringTarget, newSize, 100);
    retVal &= (oldSize >= newSize);
    retVal &= (bf.Position() == newSize);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestWrite_timeout_close() {
    uint32 NewSize = size;
    return !bf.Write(stringTarget, NewSize, 100);
}

bool BasicFileTest::TestRead() {
    char8 buffer[size + 1];
    uint32 newSize = size;
    retVal &= TestWrite();
    bf.Open(nameFileTarget, BasicFile::ACCESS_MODE_R);
    retVal &= bf.Read(buffer, newSize);
    retVal &= (0 == StringHelper::CompareN(stringTarget, buffer, newSize));
    return retVal;
}

bool BasicFileTest::TestRead_close() {
    char8 buffer[size + 1];
    uint32 newSize = size;
    return !bf.Read(buffer, newSize);
}

bool BasicFileTest::TestRead_timeout() {
    char8 buffer[size + 1];
    uint32 newSize = size;
    retVal &= TestWrite();
    bf.Open(nameFileTarget, BasicFile::ACCESS_MODE_R);
    retVal &= bf.Read(buffer, newSize, 100);
    retVal &= (0 == StringHelper::CompareN(stringTarget, buffer, newSize));
    return retVal;
}

bool BasicFileTest::TestRead_timeout_close() {
    char8 buffer[size + 1];
    uint32 newSize = size;
    return !bf.Read(buffer, newSize, 100);
}

bool BasicFileTest::TestSize() {
    uint32 newSize = size;
    retVal &= TestWrite();
    bf.Open(nameFileTarget, BasicFile::ACCESS_MODE_R);
    retVal &= ((bf.Size() == newSize));
    return retVal;
}

bool BasicFileTest::TestSize_close() {
    retVal &= ((bf.Size() == 0));
    return retVal;
}

bool BasicFileTest::TestSeek_small() {
    uint64 sizeFile = 10;
    uint64 pos = sizeFile - 1;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    retVal &= bf.Seek(pos);
    retVal &= ((pos) == bf.Position());
    return retVal;
}

bool BasicFileTest::TestSeek_large() {
    uint64 sizeFile = 10;
    uint64 pos = sizeFile + 1;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    retVal &= bf.Seek(pos);
    retVal &= (bf.Size() == bf.Position());
    return retVal;
}

bool BasicFileTest::TestSeek_Close() {
    uint64 sizeFile = 10;
    uint64 pos = sizeFile + 1;
    bf.SetSize(sizeFile);
    retVal &= !bf.Seek(pos);
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_small() {
    uint64 sizeFile = 10;
    int32 detlaPos = -2;
    uint32 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(detlaPos);
    cout << bf.Position() <<" \n";
//    retVal &= ((pos + detlaPos) == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_larger() {
    uint64 sizeFile = 10;
    int32 deltaPos = sizeFile;
    uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(deltaPos);
    retVal &= (bf.Size() == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_NegativePosition() {
    uint64 sizeFile = 10;
    int32 deltaPos = -sizeFile;
    uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(deltaPos);
    retVal &= (bf.Size() == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_Close() {
    return !bf.RelativeSeek(4);
}

bool BasicFileTest::TestPosition() {
    uint64 sizeFile = 10;
    uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    bf.Seek(pos);

    return (bf.Position() == pos);
}

bool BasicFileTest::TestSetSize_enlarging() {
    uint64 sizeFile = 10;
    uint64 newSizeFile = sizeFile + 5;
    uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal &= bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.SetSize(newSizeFile);
    retVal &= (bf.Position() == pos);
    retVal &= (bf.Size() == newSizeFile);

    return (bf.Position() == pos);
}

bool BasicFileTest::TestSetSize_reducing() {
    uint64 sizeFile = 10;
    uint64 newSizeFile = sizeFile - 7;
    uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal &= bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.SetSize(newSizeFile);
    retVal &= (bf.Position() == bf.Size());
    retVal &= (bf.Size() == newSizeFile);

    return (bf.Position() == pos);
}

bool BasicFileTest::TestGetPathName() {
    bf.Open(nameFileTarget, defaultRWFlags);

    return (bf.GetPathName() == nameFileTarget);
}

