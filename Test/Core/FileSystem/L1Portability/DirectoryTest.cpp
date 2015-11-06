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
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
DirectoryTest::DirectoryTest() {
    fname = NULL;
    Directory dir("WWWW");
    dir.Create(false);
}

DirectoryTest::~DirectoryTest() {
    Directory dir("WWWW");
    dir.Create(false);
    dir.Delete();
}

bool DirectoryTest::TestSetByName() {
    const char8 * const path = "WWWW/TestSetByName";
    Directory dir(path);
    dir.Create(false);
    bool ok = dir.SetByName(path);
    if (ok) {
        ok = (StringHelper::Compare(dir.Name(), path) == 0);
    }
    return ok;
}

bool DirectoryTest::TestNameInvalid() const {
    const char8 * const path = NULL;
    Directory dir(path);
    return (dir.Name() == path);
}

bool DirectoryTest::TestNameValid() const {
    const char8 * const path = "WWWW/TestNameValid";
    Directory dir(path);
    dir.Create(false);
    return StringHelper::Compare(dir.Name(), path) == 0;
}

bool DirectoryTest::TestIsDirectoryInvalid() const {
    const char8 * const path = "WWWW/TestIsDirectoryInvalid.txt";
    Directory dir(path);
    dir.Create(true);
    return !dir.IsDirectory();
}
bool DirectoryTest::TestIsDirectoryValid() const {
    const char8 * const path = "WWWW/TestIsDirectoryValid";
    Directory dir(path);
    dir.Create(false);
    return dir.IsDirectory();
}

bool DirectoryTest::TestIsNoFile() const {
    const char8 * const path = "WWWW/TestIsNoFile";
    Directory dir(path);
    dir.Create(false);
    return !dir.IsFile();
}
bool DirectoryTest::TestIsFile() const {
    const char8 * const path = "WWWW/TestIsFile.txt";
    Directory dir(path);
    dir.Create(true);
    return dir.IsFile();
}
///no tiene sentido el readOnly del cpp
bool DirectoryTest::TestReadOnly() const {
    Directory dir;
    return !dir.ReadOnly();
}

bool DirectoryTest::TestSizeDir(const char8 * const path) const {
    Directory dir(path);
    dir.Create(false);
    DIR *directory;
    directory = opendir(path);
    seekdir(directory, 0L);
    return (dir.Size() == (uint64)telldir(directory));
}

bool DirectoryTest::TestSizeFile(const char8 * const path) const {
    Directory dir(path);
    dir.Create(true);
    FILE *file;
    file = fopen(path, "r+");
    fputs("Hello this is to confirm the size of file", file);
    fseek(file, 0L, SEEK_END);
    return (dir.Size() == (uint64) ftell(file));
}
/*
bool TestLastWriteTime() const {
    const char8 * const path = "TestF18.txt";
    struct stat buf;
    Directory dir(path);
    dir.Create(path, true);
    FILE *fich;
    fich = fopen(path, "r+");
    fputc('a', fich);
    fclose(fich);
    stat(path, &buf);
    return (dir.LastAccessTime() == buf.st_atim);
}

bool TestLastAccessTime() const {
    return 0;
}
*/
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
    if (StringHelper::Compare(dir.Name(), path) == 0) {
        return dir.Exists();
    }
    else {
        return !dir.Exists();
    }
}

