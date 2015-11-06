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
    Directory dir("GTestDirectoryTest");
    dir.Create(false);
}

DirectoryTest::~DirectoryTest() {
    Directory dir("GTestDirectoryTest");
    dir.Create(false);
    dir.Delete();
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

bool DirectoryTest::TestNameInvalid() {
    const char8 * const path = NULL;
    Directory dir(path);
    return (StringHelper::Compare(dir.GetName(), path) == 0);
}

bool DirectoryTest::TestNameValid() {
    char8 path[128];
    DirectoryCreateN(path, "TestNameValid");
    Directory dir(path);
    dir.Create(false);
    return StringHelper::Compare(dir.GetName(), path) == 0;
}

bool DirectoryTest::TestIsDirectoryInvalid() {
    const char8 * const path = "GTestDirectoryTest/TestIsDirectoryInvalid.txt";
    Directory dir(path);
    dir.Create(true);
    return !dir.IsDirectory();
}
bool DirectoryTest::TestIsDirectoryValid() {
    const char8 * const path = "GTestDirectoryTest/TestIsDirectoryValid";
    Directory dir(path);
    dir.Create(false);
    return dir.IsDirectory();
}

bool DirectoryTest::TestIsNoFile() {
    const char8 * const path = "GTestDirectoryTest/TestIsNoFile";
    Directory dir(path);
    dir.Create(false);
    return !dir.IsFile();
}
bool DirectoryTest::TestIsFile() {
    const char8 * const path = "GTestDirectoryTest/TestIsFile.txt";
    Directory dir(path);
    dir.Create(true);
    return dir.IsFile();
}
///no tiene sentido el readOnly del cpp
bool DirectoryTest::TestReadOnly() {
    Directory dir;
    return !dir.ReadOnly();
}

bool DirectoryTest::TestGetSize_Dir(const char8 * const path) {
    Directory dir(path);
    dir.Create(false);
    DIR *directory;
    directory = opendir(path);
    seekdir(directory, 0L);
    return (dir.GetSize() == (uint64) telldir(directory));
}

bool DirectoryTest::TestGetSize_File(const char8 * const path) {
    Directory dir(path);
    dir.Create(true);
    FILE *file;
    file = fopen(path, "r+");
    fputs("Hello this is to confirm the size of file", file);
    fseek(file, 0L, SEEK_END);
    return (dir.GetSize() == (uint64) ftell(file));
}

bool DirectoryTest::TestGetLastAccessTime() {
    const char8 * const path = "GTestDirectoryTest/TestF18.txt";
    struct stat buf;
    Directory dir(path);
    dir.Create(true);
    FILE *fich;
    fich = fopen(path, "r+");
    fputc('a', fich);
    fclose(fich);
    stat(path, &buf);
    time_t secondsFromEpoch32 = static_cast<time_t>(buf.st_atime);
    const struct tm *tValues = localtime(&secondsFromEpoch32);
    if(tValues->tm_sec == (int32)dir.GetLastAccessTime().seconds){
        if (tValues->tm_min == (int32)dir.GetLastAccessTime().minutes){
            if(tValues->tm_hour == (int32)dir.GetLastAccessTime().hours){
                if (tValues->tm_mday == (int32)dir.GetLastAccessTime().days){
                    if(tValues->tm_mon == (int32)dir.GetLastAccessTime().month){
                        if(tValues->tm_year == (int32)dir.GetLastAccessTime().year){
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool DirectoryTest::TestGetLastWriteTime() {
    const char8 * const path = "GTestDirectoryTest/TestF19.txt";
        struct stat buf;
        Directory dir(path);
        dir.Create(true);
        FILE *fich;
        fich = fopen(path, "r+");
        fputc('a', fich);
        fclose(fich);
        stat(path, &buf);
        time_t secondsFromEpoch32 = static_cast<time_t>(buf.st_mtime);
        const struct tm *tValues = localtime(&secondsFromEpoch32);
        if(tValues->tm_sec == (int32)dir.GetLastWriteTime().seconds){
            if (tValues->tm_min == (int32)dir.GetLastWriteTime().minutes){
                if(tValues->tm_hour == (int32)dir.GetLastWriteTime().hours){
                    if (tValues->tm_mday == (int32)dir.GetLastWriteTime().days){
                        if(tValues->tm_mon == (int32)dir.GetLastWriteTime().month){
                            if(tValues->tm_year == (int32)dir.GetLastWriteTime().year){
                                return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
}

bool DirectoryTest::TestCreate(const char8 * const path,
                               const bool isFile) {
    Directory dir(path);
    if (path == NULL) {
        return !dir.Create(isFile);
    }
    else {
        return dir.Create(isFile);
    }
}

bool DirectoryTest::TestDelete(const char8 * const path,
                               bool file) {
    Directory dir(path);
    dir.Create(file);
    if (dir.Exists()) {
        return dir.Delete();
    }
    else {
        return !dir.Delete();
    }
}

bool DirectoryTest::TestExists(const char8 * const path) {
    Directory dir(path);
    dir.Create(false);
    if (StringHelper::Compare(dir.GetName(), path) == 0) {
        return dir.Exists();
    }
    else {
        return !dir.Exists();
    }
}

void DirectoryTest::DirectoryCreateN(char8 *destination, char8 *path) {
    Directory dir;
    dir.Create(BASE_PATH);
    StringHelper::Concatenate(destination, BASE_PATH);
    StringHelper::Concatenate(destination, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(destination, path);
}
