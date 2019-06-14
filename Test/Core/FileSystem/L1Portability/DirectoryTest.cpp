/**
 * @file DirectoryTest.cpp
 * @brief Source file for class DirectoryTest
 * @date Nov 4, 2015
 * @author Frank Perez Paz
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
 * the class DirectoryTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DirectoryTest.h"
#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

static CCString BASE_PATH = "GTestDirectoryTest";

DirectoryTest::DirectoryTest() {

}

DirectoryTest::~DirectoryTest() {

}

bool DirectoryTest::TestDirectoryTest(CCString pathin) {
    DynamicCString path;
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    return (dir.GetName() == path);
}

bool DirectoryTest::TestSetByName_Valid() {
    DynamicCString path;
    DirectoryCreateN(path, "TestSetByName_Valid");
    Directory dir(path);
    dir.Create(false);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (dir.GetName() == path);
    }
    return ok;
}

bool DirectoryTest::TestSetByName_NULL() {
    DynamicCString path;
    DirectoryCreateN(path, "");
    Directory dir(path);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (dir.GetName() == path);
    }
    return ok;
}

bool DirectoryTest::TestSetByName_Invalid() {
    DynamicCString path;
    DirectoryCreateN(path, "abc");
    Directory dir(path);
    bool ok = !dir.SetByName(path);
    return ok;
}

bool DirectoryTest::TestGetName_Valid() {
    DynamicCString path;
    DirectoryCreateN(path, "TestNameValid");
    Directory dir(path);
    dir.Create(false);
    return dir.GetName() == path;
}

bool DirectoryTest::TestGetName_Invalid() {
    const char8 * const path = NULL;
    Directory dir(path);
    return !dir.GetName().IsSameAs(path);
}

bool DirectoryTest::TestIsDirectory_Invalid() {
    DynamicCString path;
    DirectoryCreateN(path, "TestIsDirectoryInvalid.txt");
    Directory dir(path);
    dir.Create(true);
    return !dir.IsDirectory();
}

bool DirectoryTest::TestIsDirectory_Valid() {
    DynamicCString path;
    DirectoryCreateN(path, "TestIsDirectoryValid");
    Directory dir(path);
    dir.Create(false);
    return dir.IsDirectory();
}

bool DirectoryTest::TestIsFile_No() {
    DynamicCString path;
    DirectoryCreateN(path, "TestIsNoFile");
    Directory dir(path);
    dir.Create(false);
    return !dir.IsFile();
}

bool DirectoryTest::TestIsFile_Yes() {
    DynamicCString path;
    DirectoryCreateN(path, "TestIsFile.txt");
    Directory dir(path);
    dir.Create(true);
    return dir.IsFile();
}

bool DirectoryTest::TestGetSize_Dir() {
    DynamicCString path;
    DynamicCString path2;

    DirectoryCreateN(path, "TestGetSize_Dir1");
    Directory dir(path);
    dir.Create(false);

    DirectoryCreateN(path2, "TestGetSize_Dir2");
    Directory dir2(path2);
    dir2.Create(false);

    return(dir.GetSize() == dir2.GetSize());
}

static inline void Write2File(BasicFile &file, CCString message){
    uint32 size = message.GetSize();
    file.Write(message.GetList(), size);
}

bool DirectoryTest::TestGetSize_FileCorrect() {
    DynamicCString path;
    DynamicCString path2;
    DirectoryCreateN(path, "TestGetSize_File_C.txt");
    DirectoryCreateN(path2, "TestGetSize_File1_C.txt");
    Directory dir(path);
    dir.Create(true);
    Directory dir2(path2);
    dir2.Create(true);

    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "The sizes of files are equals");
    file.Close();

    file.Open(path2, file.ACCESS_MODE_W);
    Write2File(file, "The sizes of files are equals");
    file.Close();

    return (dir.GetSize() == dir2.GetSize());

}

bool DirectoryTest::TestGetSize_FileIncorrect() {
    DynamicCString path;
    DynamicCString path2;
    DirectoryCreateN(path, "TestGetSize_File_I.txt");
    DirectoryCreateN(path2, "TestGetSize_File1_I.txt");
    Directory dir(path);
    dir.Create(true);
    Directory dir2(path2);
    dir2.Create(true);

    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "the size of file are different in this file");
    file.Close();

    file.Open(path2, file.ACCESS_MODE_W);
    Write2File(file, "that in this");
    file.Close();

    return (dir.GetSize() != dir2.GetSize());
}

bool DirectoryTest::TestGetLastAccessTime() {
    DynamicCString path;
    DirectoryCreateN(path, "TestGetLastAccessTime.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "Confirm the lastAccess");
    file.Close();
    TimeStamp lastAccessTime = dir.GetLastAccessTime();

    Sleep::Short(1100,Units::ms);
    TimeStamp lastAccessTime2 = dir.GetLastAccessTime();

    return (lastAccessTime.GetSeconds() == lastAccessTime2.GetSeconds());
}

bool DirectoryTest::TestGetLastAccessTime_ReRead() {
    DynamicCString path;
    char8 pathR[128];
    DirectoryCreateN(path, "TestGetLastAccessTime_ReRead.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "Confirm the last access read again");
    file.Close();

    file.Open(path, BasicFile::ACCESS_MODE_R);
    uint32 size = 10;
    file.Read(pathR, size);
    file.Close();
    TimeStamp lastAccessTime = dir.GetLastAccessTime();

    Sleep::Short(1100,Units::ms);
    file.Open(path, BasicFile::ACCESS_MODE_R);
    file.Read(pathR, size);
    file.Close();
    TimeStamp lastAccessTime2 = dir.GetLastAccessTime();
    return (lastAccessTime.GetSeconds() != lastAccessTime2.GetSeconds());
}

bool DirectoryTest::TestGetLastWriteTime() {
    DynamicCString path;
    DirectoryCreateN(path, "TestGetLastWriteTime.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "Confirm the last Write");
    file.Close();
    TimeStamp lastWriteTime = dir.GetLastWriteTime();
    Sleep::Short(1100,Units::ms);
    TimeStamp lastWriteTime2 = dir.GetLastWriteTime();
    return (lastWriteTime.GetSeconds() == lastWriteTime2.GetSeconds());
}

bool DirectoryTest::TestGetLastWriteTime_ReWrite() {
    DynamicCString path;

    DirectoryCreateN(path, "TestGetLastWriteTime_ReWrite.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "Confirm writing again");
    file.Close();
    TimeStamp lastWriteTime = dir.GetLastWriteTime();

    Sleep::Short(1100,Units::ms);
    file.Open(path, file.ACCESS_MODE_W);
    Write2File(file, "to be confirmed it is need write again");
    file.Close();
    TimeStamp lastWriteTime2 = dir.GetLastWriteTime();

    return (lastWriteTime.GetSeconds() != lastWriteTime2.GetSeconds());
}

bool DirectoryTest::TestCreate(CCString pathin,const bool isFile) {
    DynamicCString path;
    DirectoryCreateN(path, pathin);
    Directory dir(path);
#if ENVIRONMENT == Windows
    if ((!dir.Exists()) && (pathin != NULL && (pathin.GetSize() > 0))) {
#elif ENVIRONMENT == Linux
        if (((!dir.Exists() || isFile)) && (pathin != NULL && (StringHelper::Compare(pathin, "") != 0))) {
#endif
        return dir.Create(isFile);
    }
    else {
        return !dir.Create(isFile);
    }
}

bool DirectoryTest::TestDelete(CCString pathin,
                               bool isFile) {
    DynamicCString path;
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(isFile);
    if ((pathin != NULL) && (pathin.GetSize() > 0)) {
        return dir.Delete();
    }
    else {
        return !dir.Delete();
    }
}

bool DirectoryTest::TestExists(CCString pathin, bool isFile) {
    DynamicCString path;
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(isFile);
    if (dir.GetName() == path) {
        return dir.Exists();
    }
    else {
        return !dir.Exists();
    }
}

void DirectoryTest::DirectoryCreateN(DynamicCString &destination,CCString path) {
	destination = "";
	destination().Append(BASE_PATH).Append(DIRECTORY_SEPARATOR).Append(path);
}

bool DirectoryTest::Create_Directory() {
    Directory dir(BASE_PATH);
    dir.Create(false);
    return dir.IsDirectory();
}

bool DirectoryTest::Delete_Directory() {
    DirectoryScanner directory;
    directory.Scan(BASE_PATH);

    uint32 i = 0;
    while (directory.ListPeek(i) != NULL) {
        Directory *direc = static_cast<Directory *>(directory.ListPeek(i));
        Directory dirDel(direc->GetName());
        dirDel.Delete();
        i++;
    }
    Directory dir(BASE_PATH);
    return dir.Delete();
}
