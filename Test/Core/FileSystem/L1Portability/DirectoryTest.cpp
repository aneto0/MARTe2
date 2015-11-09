/**
 * @file DirectoryTest.cpp
 * @brief Source file for class DirectoryTest
 * @date Nov 4, 2015
 * @author fperez
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

const char8 * const BASE_PATH = "GTestDirectoryTest";

DirectoryTest::DirectoryTest() {
    fname = NULL;
}

DirectoryTest::~DirectoryTest() {
}

bool DirectoryTest::TestSetByName() {
    char8 path[128];
    DirectoryCreateN(path, "TestSetByName");
    Directory dir(path);
    dir.Create(false);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (StringHelper::Compare(dir.GetName(), path) == 0);
    }
    return ok;
}

bool DirectoryTest::TestName_Invalid() {
    const char8 * const path = NULL;
    Directory dir(path);
    return (StringHelper::Compare(dir.GetName(), path) == -1);
}

bool DirectoryTest::TestName_Valid() {
    char8 path[128];
    DirectoryCreateN(path, "TestNameValid");
    Directory dir(path);
    dir.Create(false);
    return StringHelper::Compare(dir.GetName(), path) == 0;
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
    //path2[0] = '\0';
    struct stat structure;
    DirectoryCreateN(path, "TestGetSize_Dir");
    StringHelper::Copy(path2, path);
    Directory dir(path);
    dir.Create(false);
    StringHelper::Concatenate(path2, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(path2, "TestGetSize_Dir.txt");
    FILE *file;
    file = fopen(path2, "w");
    fputs("Hello this confirm my question", file);
    fclose(file);
    DIR *directory;
    directory = opendir(path);
    stat(readdir(directory)->d_name, &structure);
    bool ok = (dir.GetSize() == (uint64) structure.st_size);
    return ok;
}

bool DirectoryTest::TestGetSize_File() {
    char8 path[128];
    DirectoryCreateN(path, "TestGetSize_File.txt");
    struct stat structure;
    Directory dir(path);
    dir.Create(true);
    FILE *file;
    file = fopen(path, "w");
    fputs("Hello this confirm my question", file);
    fclose(file);
    stat(path, &structure);
    uint64 leng = dir.GetSize();
    return (leng == (uint64) structure.st_size);
}

bool DirectoryTest::TestGetLastAccessTime() {
    char8 path[128];
    DirectoryCreateN(path, "TestGetLastAccessTime.txt");
    struct stat buf;
    Directory dir(path);
    dir.Create(true);
    FILE *file;
    file = fopen(path, "r+");
    fputc('a', file);
    fclose(file);
    stat(path, &buf);
    time_t secondsFromEpoch32 = static_cast<time_t>(buf.st_atime);
    const struct tm *tValues = localtime(&secondsFromEpoch32);
    TimeValues lastAccessTime = dir.GetLastAccessTime();
    bool ok = (tValues->tm_sec == (int32) lastAccessTime.seconds);
    ok &= (tValues->tm_min == (int32) lastAccessTime.minutes);
    ok &= (tValues->tm_hour == (int32) lastAccessTime.hours);
    ok &= (tValues->tm_mday == (int32) lastAccessTime.days);
    ok &= (tValues->tm_mon == (int32) lastAccessTime.month);
    ok &= (tValues->tm_year == (int32) lastAccessTime.year);
    return ok;
}

bool DirectoryTest::TestGetLastWriteTime() {
    char8 path[128];
    DirectoryCreateN(path, "TestGetLastWriteTime.txt");
    struct stat buf;
    Directory dir(path);
    dir.Create(true);
    FILE *file;
    file = fopen(path, "r+");
    fputc('a', file);
    fclose(file);
    stat(path, &buf);
    time_t secondsFromEpoch32 = static_cast<time_t>(buf.st_mtime);
    const struct tm *tValues = localtime(&secondsFromEpoch32);
    TimeValues lastWriteTime = dir.GetLastWriteTime();
    bool ok = (tValues->tm_sec == (int32) lastWriteTime.seconds);
    ok &= (tValues->tm_min == (int32) lastWriteTime.minutes);
    ok &= (tValues->tm_hour == (int32) lastWriteTime.hours);
    ok &= (tValues->tm_mday == (int32) lastWriteTime.days);
    ok &= (tValues->tm_mon == (int32) lastWriteTime.month);
    ok &= (tValues->tm_year == (int32) lastWriteTime.year);
    return ok;
}

bool DirectoryTest::TestCreate(char8 *pathin,
                               const bool isFile) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    if ((!dir.Exists()) && (pathin != NULL)) {
        return dir.Create(isFile);
    }
    else {
        return !dir.Create(isFile);
    }
}

bool DirectoryTest::TestDelete(char8 * pathin,
                               bool file) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(file);
    if (pathin != NULL) {
        return dir.Delete();
    }
    else {
        return !dir.Delete();
    }
}

bool DirectoryTest::TestExists(char8 * pathin) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(false);
    if (StringHelper::Compare(dir.GetName(), pathin) == 0) {
        return dir.Exists();
    }
    else {
        return !dir.Exists();
    }
}

void DirectoryTest::DirectoryCreateN(char8 *destination,
                                     char8 *path) {
    Directory dir(BASE_PATH);
    dir.Create(false);
    destination[0] = '\0';
    StringHelper::Concatenate(destination, BASE_PATH);
    StringHelper::Concatenate(destination, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(destination, path);
}
