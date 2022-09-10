/**
 * @file BasicFileTest.cpp
 * @brief Source file for class TestBasicFile
 * @date 04/11/2015
 * @author Lloren� Capell�
 * @author Ivan Herrero
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

const uint32 BasicFileTest::size = 9;

//static CompareWrapperStatus

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

BasicFileTest::BasicFileTest() {
    retVal = true;
    defaultRWFlags = BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W;
    defaultWFlags = BasicFile::FLAG_APPEND | BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W;
    defaultRFlags = BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R;
    str1 = "TargetFile_Test.txt";
    str2 = "abcdefghi";
    nameFileTarget = str1.Buffer();
    stringTarget = str2.Buffer();
}

BasicFileTest::~BasicFileTest() {
}

bool BasicFileTest::TestDefaultConstructor() {
    retVal &= !bf.CanRead();
    retVal &= !bf.CanSeek();
    retVal &= !bf.CanWrite();
    retVal &= !bf.IsOpen();
    retVal &= (bf.Size() == 0);
    retVal &= (bf.Position() == 0);
    retVal &= (bf.GetFlags() == 0);
    retVal &= (bf.GetPathName() == "");

    return retVal;
}

bool BasicFileTest::TestCopyConstructor_withOpenFile() {
    bf.Open("TestCopyConstructor_Test.txt", defaultRWFlags);
    BasicFile copybf(bf);
    retVal &= (copybf.CanRead() == bf.CanRead());
    retVal &= (copybf.CanSeek() == bf.CanSeek());
    retVal &= (copybf.CanWrite() == bf.CanWrite());
    retVal &= (copybf.IsOpen() == bf.IsOpen());
    retVal &= (copybf.Size() == bf.Size());
    retVal &= (copybf.Position() == bf.Position());
    retVal &= (copybf.GetFlags() == bf.GetFlags());
    retVal &= (copybf.GetPathName() == bf.GetPathName());
    return retVal;
}

bool BasicFileTest::TestCopyConstructor_withClosedFile() {
    BasicFile copybf(bf);
    retVal &= (copybf.CanRead() == bf.CanRead());
    retVal &= (copybf.CanSeek() == bf.CanSeek());
    retVal &= (copybf.CanWrite() == bf.CanWrite());
    retVal &= (copybf.IsOpen() == bf.IsOpen());
    retVal &= (copybf.Size() == bf.Size());
    retVal &= (copybf.Position() == bf.Position());
    retVal &= (copybf.GetFlags() == bf.GetFlags());
    retVal &= (copybf.GetPathName() == bf.GetPathName());
    return retVal;
}

bool BasicFileTest::TestAssignmentOperator_withOpenFile() {
    bf.Open("TestAssignmentOperator_withOpenFile_Test.txt", defaultRWFlags);
    BasicFile copybf;
    copybf.Open("TestAssignmentOperator_withOpenFile1_Test.txt", defaultRWFlags);
    copybf = bf;
    retVal &= (copybf.CanRead() == bf.CanRead());
    retVal &= (copybf.CanSeek() == bf.CanSeek());
    retVal &= (copybf.CanWrite() == bf.CanWrite());
    retVal &= (copybf.IsOpen() == bf.IsOpen());
    retVal &= (copybf.Size() == bf.Size());
    retVal &= (copybf.Position() == bf.Position());
    retVal &= (copybf.GetFlags() == bf.GetFlags());
    retVal &= (copybf.GetPathName() == bf.GetPathName());
    bf.Close();
    copybf.Close();
    return retVal;
}

bool BasicFileTest::TestAssignmentOperator_withClosedFile() {
    BasicFile copybf;
    copybf = bf;
    retVal &= (copybf.CanRead() == bf.CanRead());
    retVal &= (copybf.CanSeek() == bf.CanSeek());
    retVal &= (copybf.CanWrite() == bf.CanWrite());
    retVal &= (copybf.IsOpen() == bf.IsOpen());
    retVal &= (copybf.Size() == bf.Size());
    retVal &= (copybf.Position() == bf.Position());
    retVal &= (copybf.GetFlags() == bf.GetFlags());
    retVal &= (copybf.GetPathName() == bf.GetPathName());
    bf.Close();
    copybf.Close();
    return retVal;
}

bool BasicFileTest::TestAssignmentOperator_checkPointsToSameFile() {
    BasicFile copybf;
    const char8 * writeString = str2.Buffer();
    uint32 size1 = 3;
    char8 buffer[10];
    char8 * const readString = buffer;
    bf.Open("TestAssignmentOperator_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
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
    bf.Open("TestSetFlags_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W | BasicFile::FLAG_APPEND /*defaultRWFlags*/);
    //First check that the flag is set to APPEND
    uint32 retFlags = bf.GetFlags();
    retVal &= ((retFlags & BasicFile::FLAG_APPEND) == BasicFile::FLAG_APPEND);
    //Remove the BasicFile::FLAG_APPEND
    bf.SetFlags(0);
    retFlags = bf.GetFlags();
    retVal &= ((retFlags & BasicFile::FLAG_APPEND) != BasicFile::FLAG_APPEND);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestSetFlags_Close() {
    return !bf.SetFlags(0U);
}

bool BasicFileTest::TestGetFlags() {
    bf.Open("TestGetFlags_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == (BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestGetFlags_defaultWFlags() {
    bf.Open("TestGetFlags_defaultWFlags_Test.txt", defaultWFlags);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == (defaultWFlags));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestGetFlags_defaultRFlags() {
    bf.Open("TestGetFlags_defaultRFlags_Test.txt", defaultRFlags);
    uint32 retFlags = bf.GetFlags();
    retVal &= (retFlags == defaultRFlags);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanRead_OnClosedFile() {
    return !bf.CanRead();
}

bool BasicFileTest::TestCanRead_OnOpenedReadbleFile() {
    bf.Open("TestCanRead_OnOpenedReadbleFile_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R /*defaultRWFlags*/);
    retVal = bf.CanRead();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanRead_OnOpenWritableFile() {
    bf.Open("TestCanRead_OnOpenWritableFile_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W /*defaultWFlags*/);
    retVal = !bf.CanRead();
    bf.Close();
    return !bf.CanRead();
}

bool BasicFileTest::TestCanWrite_Close() {
    return !bf.CanWrite();
}

bool BasicFileTest::TestCanWrite_OpenRead() {
    bf.Open("TestCanWrite_OpenRead_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R /*defaultRFlags*/);
    retVal = !bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanWrite_OpenWrite() {
    bf.Open("TestCanWrite_OpenWrite_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W /*defaultWFlags*/);
    retVal = bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestCanSeek_Close() {
    return bf.CanSeek();
}

bool BasicFileTest::TestCanSeek_Open() {
    bf.Open("TestCanSeek_Open_Test.txt", BasicFile::FLAG_CREAT /*defaultRWFlags*/);
    retVal &= bf.CanWrite();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen() {
    retVal &= bf.Open("TestOpen_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
    retVal &= bf.CanRead();
    retVal &= bf.CanWrite();
    retVal &= bf.CanSeek();
    retVal &= bf.IsOpen();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_FLAG_CREAT_EXCLUSIVE() {
    retVal &= bf.Open("TestOpen_FLAG_CREAT_EXCLUSIVE_Test.txt", BasicFile::FLAG_CREAT_EXCLUSIVE | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W);
    retVal &= ((bf.GetFlags() & BasicFile::FLAG_CREAT_EXCLUSIVE) == BasicFile::FLAG_CREAT_EXCLUSIVE);
    retVal &= bf.CanRead();
    retVal &= bf.CanWrite();
    retVal &= bf.CanSeek();
    retVal &= bf.IsOpen();
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_twice() {
    retVal &= bf.Open("TestOpen_twice_Test.txt", BasicFile::FLAG_CREAT /*defaultRWFlags*/);
    retVal &= bf.Open("TestOpen_twice_Test.txt", BasicFile::FLAG_CREAT /*defaultRWFlags*/);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags1() {
    //If not exist create the file
    bf.Open("TestOpen_IncompatibleFlags1_Test.txt", defaultRWFlags);
    bf.Close();
    //If the file exist and the activated flag is FLAG_CREAT_EXCLUSIVE the Open should fail. If it doesn't fail means FLAG_CREAT is activated.
    retVal &= bf.Open("TestOpen_IncompatibleFlags1_Test.txt", BasicFile::FLAG_CREAT | BasicFile::FLAG_CREAT_EXCLUSIVE);
    retVal &= ((bf.GetFlags() & BasicFile::FLAG_CREAT) == BasicFile::FLAG_CREAT);
    retVal &= ((bf.GetFlags() & BasicFile::FLAG_CREAT_EXCLUSIVE) != BasicFile::FLAG_CREAT_EXCLUSIVE);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags2() {
    //If not exist create the file
    bf.Open("TestOpen_IncompatibleFlags2_Test.txt", defaultRWFlags);
    bf.Close();
    //The open should fail because the file already exists.
    retVal &= !(bf.Open("TestOpen_IncompatibleFlags2_Test.txt", BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT_EXCLUSIVE));
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags3() {
    //If not exist create the file
    bf.Open("TestOpen_IncompatibleFlags3_Test.txt", defaultRWFlags);
    bf.Close();
    //The open should fail because the file already exists.
    retVal &= (bf.Open("TestOpen_IncompatibleFlags3_Test.txt", BasicFile::FLAG_APPEND | BasicFile::ACCESS_MODE_R));
    retVal &= ((bf.GetFlags() & BasicFile::ACCESS_MODE_R) == BasicFile::ACCESS_MODE_R);
    retVal &= ((bf.GetFlags() & BasicFile::FLAG_APPEND) != BasicFile::FLAG_APPEND);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestOpen_IncompatibleFlags4() {
    //If not exist create the file
    bf.Open("TestOpen_IncompatibleFlags4_Test.txt", defaultRWFlags);
    bf.Close();
    //The open should fail because the file already exists.
    retVal &= (bf.Open("TestOpen_IncompatibleFlags4_Test.txt", BasicFile::FLAG_TRUNC | BasicFile::ACCESS_MODE_R));
    retVal &= ((bf.GetFlags() & BasicFile::ACCESS_MODE_R) == BasicFile::ACCESS_MODE_R);
    retVal &= ((bf.GetFlags() & BasicFile::FLAG_TRUNC) != BasicFile::FLAG_TRUNC);
    bf.Close();
    return retVal;
}

bool BasicFileTest::TestIsOpen() {
    //Checks if the default constructed BasicFile object is into closed state (not open):
    retVal &= !bf.IsOpen();

    //Binds a file to the BasicFile object (i.e. opens it):
    bf.Open("TestIsOpen_Test.txt", BasicFile::FLAG_CREAT /*defaultRWFlags*/);

    //Checks if the BasicFile object is into open state after binding a file to it:
    retVal &= bf.IsOpen();

    //Unbinds the file associated to the BasicFile object (i.e. closes it):
    bf.Close();

    //Checks if the BasicFile object is into closed state after unbinding its associated file:
    retVal &= !bf.IsOpen();

    return retVal;
}

bool BasicFileTest::TestClose() {
    bf.Open("TestClose_Test.txt", BasicFile::FLAG_CREAT /*defaultRWFlags*/);
    bf.Close();
    retVal &= !bf.CanRead();
    retVal &= !bf.CanWrite();
    retVal &= !bf.CanSeek();
    retVal &= !bf.IsOpen();
    retVal &= (bf.Size() == 0);
    retVal &= (bf.Position() == 0);
    retVal &= (bf.GetFlags() == 0);
    retVal &= (bf.GetPathName().Size() == 0);
    return retVal;
}

bool BasicFileTest::TestWrite() {
    uint32 oldSize = size;
    uint32 newSize = size;
    bf.Open(nameFileTarget, BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | /*BasicFile::ACCESS_MODE_R |*/BasicFile::ACCESS_MODE_W);
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
    bf.Open(nameFileTarget, BasicFile::FLAG_TRUNC | BasicFile::FLAG_CREAT | /*BasicFile::ACCESS_MODE_R |*/BasicFile::ACCESS_MODE_W);
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
    retVal &= (bf.Position() == newSize);
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
    bf.Open(nameFileTarget, BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R);
    retVal &= ((bf.Size() == newSize));
    return retVal;
}

bool BasicFileTest::TestSize_close() {
    retVal &= ((bf.Size() == 0));
    return retVal;
}

bool BasicFileTest::TestSeek_small() {
    const uint64 sizeFile = 10;
    const uint64 pos = sizeFile - 1;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    retVal &= bf.Seek(pos);
    retVal &= ((pos) == bf.Position());
    return retVal;
}

bool BasicFileTest::TestSeek_large() {
    const uint64 sizeFile = 10;
    const uint64 pos = sizeFile + 1;
    bf.Open(nameFileTarget, defaultRWFlags);
    bf.SetSize(sizeFile);
    retVal &= bf.Seek(pos);
    retVal &= (bf.Size() == bf.Position());
    return retVal;
}

bool BasicFileTest::TestSeek_Close() {
    const uint64 sizeFile = 10;
    const uint64 pos = sizeFile + 1;
    bf.SetSize(sizeFile);
    retVal &= !bf.Seek(pos);
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_small() {
    const uint64 sizeFile = 10;
    const int32 deltaPos = -2;
    const uint32 pos = 5;
    bf.Open("TestRelativeSeek_small_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(deltaPos);
    retVal &= ((pos + deltaPos) == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_larger() {
    const uint64 sizeFile = 10;
    const int32 deltaPos = 10;
    const uint64 pos = 5;
    bf.Open("TestRelativeSeek_larger_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(deltaPos);
    retVal &= (bf.Size() == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_NegativePosition() {
    const uint64 sizeFile = 10;
    const int32 deltaPos = -10;
    const uint64 pos = 5;
    bf.Open("TestRelativeSeek_NegativePosition_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W /*defaultRWFlags*/);
    bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.RelativeSeek(deltaPos);
    retVal &= (0 == bf.Position());
    return retVal;
}

bool BasicFileTest::TestRelativeSeek_Close() {
    return !bf.RelativeSeek(4);
}

bool BasicFileTest::TestPosition() {
    uint64 sizeFile = 10;

    //Opens and sets the size for the target file
    bf.Open("TestPosition_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
    bf.SetSize(sizeFile);

    //Checks getting position at beginning
    {
        uint64 pos = 0;
        bf.Seek(pos);
        retVal &= (bf.Position() == pos);
    }

    //Checks getting position at middle
    {
        uint64 pos = 5;
        bf.Seek(pos);
        retVal &= (bf.Position() == pos);
    }

    //Checks getting position at end
    {
        uint64 pos = 9;
        bf.Seek(pos);
        retVal &= (bf.Position() == pos);
    }

    //Checks getting position at end
    {
        uint64 pos = 10;
        bf.Seek(pos);
        retVal &= (bf.Position() == pos);
    }

    return retVal;
}

bool BasicFileTest::TestSetSize_enlarging() {
    const uint64 sizeFile = 10;
    const uint64 newSizeFile = sizeFile + 5;
    const uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal &= bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.SetSize(newSizeFile);
    retVal &= (bf.Position() == pos);
    retVal &= (bf.Size() == newSizeFile);

    return retVal;
}

bool BasicFileTest::TestSetSize_reducing() {
    const uint64 sizeFile = 10;
    const uint64 newSizeFile = sizeFile - 7;
    const uint64 pos = 5;
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal &= bf.SetSize(sizeFile);
    bf.Seek(pos);
    retVal &= bf.SetSize(newSizeFile);
    retVal &= (bf.Position() == newSizeFile);
    retVal &= (bf.Size() == newSizeFile);

    return retVal;
}

bool BasicFileTest::TestGetPathName() {
    bf.Open(nameFileTarget, defaultRWFlags);
    return (bf.GetPathName() == nameFileTarget);
}

bool BasicFileTest::TestGetReadHandle() {
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal = (bf.GetReadHandle() >= 0);
    return retVal;
}

bool BasicFileTest::TestGetWriteHandle() {
    bf.Open(nameFileTarget, defaultRWFlags);
    retVal = (bf.GetWriteHandle() >= 0);
    return retVal;
}

