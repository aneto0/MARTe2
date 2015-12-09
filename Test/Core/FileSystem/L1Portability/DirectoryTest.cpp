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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DirectoryTest.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

static const char8 * const BASE_PATH = "GTestDirectoryTest";

DirectoryTest::DirectoryTest() {

}

DirectoryTest::~DirectoryTest() {

}

bool DirectoryTest::TestDirectoryTest(const char8 * pathin) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    return (StringHelper::Compare(dir.GetName(), path) == 0);
}

bool DirectoryTest::TestSetByName_Valid() {
    char8 path[128];
    DirectoryCreateN(path, "TestSetByName_Valid");
    Directory dir(path);
    dir.Create(false);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (StringHelper::Compare(dir.GetName(), path) == 0);
    }
    return ok;
}

bool DirectoryTest::TestSetByName_NULL() {
    char8 path[128];
    DirectoryCreateN(path, NULL);
    Directory dir(path);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (StringHelper::Compare(dir.GetName(), path) == 0);
    }
    return ok;
}

bool DirectoryTest::TestSetByName_Invalid() {
    char8 path[128];
    DirectoryCreateN(path, "abc");
    Directory dir(path);
    bool ok = !dir.SetByName(path);
    return ok;
}

bool DirectoryTest::TestGetName_Valid() {
    char8 path[128];
    DirectoryCreateN(path, "TestNameValid");
    Directory dir(path);
    dir.Create(false);
    return StringHelper::Compare(dir.GetName(), path) == 0;
}

bool DirectoryTest::TestGetName_Invalid() {
    const char8 * const path = NULL;
    Directory dir(path);
    return (StringHelper::Compare(dir.GetName(), path) == -1);
}

bool DirectoryTest::TestIsDirectory_Invalid() {
    char8 path[128];
    DirectoryCreateN(path, "TestIsDirectoryInvalid.txt");
    Directory dir(path);
    dir.Create(true);
    return !dir.IsDirectory();
}

bool DirectoryTest::TestIsDirectory_Valid() {
    char8 path[128];
    DirectoryCreateN(path, "TestIsDirectoryValid");
    Directory dir(path);
    dir.Create(false);
    return dir.IsDirectory();
}

bool DirectoryTest::TestIsFile_No() {
    char8 path[128];
    DirectoryCreateN(path, "TestIsNoFile");
    Directory dir(path);
    dir.Create(false);
    return !dir.IsFile();
}

bool DirectoryTest::TestIsFile_Yes() {
    char8 path[128];
    DirectoryCreateN(path, "TestIsFile.txt");
    Directory dir(path);
    dir.Create(true);
    return dir.IsFile();
}

bool DirectoryTest::TestGetSize_Dir() {
    char8 path[128];
    char8 path2[128];

    DirectoryCreateN(path, "TestGetSize_Dir1");
    Directory dir(path);
    dir.Create(false);

    DirectoryCreateN(path2, "TestGetSize_Dir2");
    Directory dir2(path2);
    dir2.Create(false);

    bool ok = (dir.GetSize() == dir2.GetSize());
    return ok;
}

bool DirectoryTest::TestGetSize_FileCorrect() {
    char8 path[128];
    char8 path2[128];
    DirectoryCreateN(path, "TestGetSize_File.txt");
    DirectoryCreateN(path2, "TestGetSize_File1.txt");
    Directory dir(path);
    dir.Create(true);
    Directory dir2(path2);
    dir2.Create(true);

    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("The sizes of files are equals");
    file.Write("The sizes of files are equals", size);
    file.Close();

    file.Open(path2, file.ACCESS_MODE_W);
    size = StringHelper::Length("The sizes of files are equals");
    file.Write("The sizes of files are equals", size);
    file.Close();

    bool ok = (dir.GetSize() == dir2.GetSize());

    return ok;
}

bool DirectoryTest::TestGetSize_FileIncorrect() {
    char8 path[128];
    char8 path2[128];
    DirectoryCreateN(path, "TestGetSize_File.txt");
    DirectoryCreateN(path2, "TestGetSize_File1.txt");
    Directory dir(path);
    dir.Create(true);
    Directory dir2(path2);
    dir2.Create(true);

    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("the size of file are different in this file");
    file.Write("the size of file are different in this file", size);
    file.Close();

    file.Open(path2, file.ACCESS_MODE_W);
    size = StringHelper::Length("that in this");
    file.Write("that in this", size);
    file.Close();

    bool ok = (dir.GetSize() == dir2.GetSize());
    return !ok;
}

bool DirectoryTest::TestGetLastAccessTime() {
    char8 path[128];
    DirectoryCreateN(path, "TestGetLastAccessTime.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("Confirm the lastAccess");
    file.Write("Confirm the lastAccess", size);
    file.Close();
    TimeStamp lastAccessTime = dir.GetLastAccessTime();

    Sleep::Sec(1.1e-0);
    TimeStamp lastAccessTime2 = dir.GetLastAccessTime();

    bool ok = (lastAccessTime.GetMicroseconds() == lastAccessTime2.GetMicroseconds());
    ok &= (lastAccessTime.GetSeconds() == lastAccessTime2.GetSeconds());
    return ok;
}

bool DirectoryTest::TestGetLastAccessTime_ReRead() {
    char8 path[128];
    char8 pathR[128];
    DirectoryCreateN(path, "TestGetLastAccessTime_ReRead.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("Confirm the last access read again");
    file.Write("Confirm the last access read again", size);
    file.Close();

    file.Open(path, BasicFile::ACCESS_MODE_R);
    file.Read(pathR, size);
    file.Close();
    TimeStamp lastAccessTime = dir.GetLastAccessTime();

    Sleep::Sec(1.1e-0);
    file.Open(path, BasicFile::ACCESS_MODE_R);
    file.Read(pathR, size);
    file.Close();
    TimeStamp lastAccessTime2 = dir.GetLastAccessTime();
    bool ok = (lastAccessTime.GetSeconds() != lastAccessTime2.GetSeconds());
    return ok;
}

bool DirectoryTest::TestGetLastWriteTime() {
    char8 path[128];
    DirectoryCreateN(path, "TestGetLastWriteTime.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("Confirm the last Write");
    file.Write("Confirm the last Write", size);
    file.Close();
    TimeStamp lastWriteTime = dir.GetLastWriteTime();
    Sleep::Sec(1.1e-0);
    TimeStamp lastWriteTime2 = dir.GetLastWriteTime();
    bool ok = (lastWriteTime.GetSeconds() == lastWriteTime2.GetSeconds());
    return ok;
}

bool DirectoryTest::TestGetLastWriteTime_ReWrite() {
    char8 path[128];

    DirectoryCreateN(path, "TestGetLastWriteTime_ReWrite.txt");
    Directory dir(path);
    dir.Create(true);
    BasicFile file;
    file.Open(path, file.ACCESS_MODE_W);
    uint32 size = StringHelper::Length("Confirm writing again");
    file.Write("Confirm writing again", size);
    file.Close();
    TimeStamp lastWriteTime = dir.GetLastWriteTime();

    Sleep::Sec(1.1e-0);
    file.Open(path, file.ACCESS_MODE_W);
    size = StringHelper::Length("to be confirmed it is need write again");
    file.Write("to be confirmed it is need write again", size);
    file.Close();
    TimeStamp lastWriteTime2 = dir.GetLastWriteTime();

    bool ok = (lastWriteTime.GetSeconds() != lastWriteTime2.GetSeconds());
    return ok;
}

bool DirectoryTest::TestCreate(const char8 *pathin,
                               const bool isFile) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
#if ENVIRONMENT == Windows
    if ((!dir.Exists()) && (pathin != NULL && (StringHelper::Compare(pathin, "") != 0))) {
#else if ENVIRONMENT == Linux
        if (((!dir.Exists() || isFile)) && (pathin != NULL && (StringHelper::Compare(pathin, "") != 0))) {
#endif
        return dir.Create(isFile);
    }
    else {
        return !dir.Create(isFile);
    }
}

bool DirectoryTest::TestDelete(const char8 * pathin,
                               bool isFile) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(isFile);
    if ((pathin != NULL) && (StringHelper::Compare(pathin, "") != 0)) {
        return dir.Delete();
    }
    else {
        return !dir.Delete();
    }
}

bool DirectoryTest::TestExists(const char8 * pathin,
                               bool isFile) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(isFile);
    if (StringHelper::Compare(dir.GetName(), path) == 0) {
        return dir.Exists();
    }
    else {
        return !dir.Exists();
    }
}

void DirectoryTest::DirectoryCreateN(char8 *destination,
                                     const char8 *path) {
    destination[0] = '\0';
    StringHelper::Concatenate(destination, BASE_PATH);
    StringHelper::Concatenate(destination, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(destination, path);
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
