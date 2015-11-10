/**
 * @file DirectoryScannerTest.cpp
 * @brief Source file for class DirectoryScannerTest
 * @date Nov 9, 2015
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
 * the class DirectoryScannerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DirectoryScannerTest.h"
#include "SortFilter.h"
#include "Directory.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class DirectoryNameSorter: public SortFilter {
    virtual int32 Compare(LinkedListable *data1,
                          LinkedListable *data2) {
        Directory *dir1 = static_cast<Directory *>(data1);
        Directory *dir2 = static_cast<Directory *>(data2);
        return StringHelper::Compare(dir1->GetName(), dir2->GetName());
    }
};

class DirectorySizeSorter: public SortFilter {
    virtual int32 Compare(LinkedListable *data1,
                          LinkedListable *data2) {
        Directory *dir1 = static_cast<Directory *>(data1);
        Directory *dir2 = static_cast<Directory *>(data2);
        return (dir1->GetSize() - dir2->GetSize());
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
const char8 * const BASE_PATH = "GTestDirectoryScannerTest";

DirectoryScannerTest::DirectoryScannerTest() {

}
DirectoryScannerTest::~DirectoryScannerTest() {

}

bool DirectoryScannerTest::TestDirectorySize() {
    char8 path[128];
    char8 path2[128];
    DirectoryCreateN(path, "TestDirectorySize");
    StringHelper::Copy(path2, path);
    Directory dir(path);
    dir.Create(false);
    DirectoryCreateN(path, "TestDirectorySize.txt");
    dir = Directory(path);
    dir.Create(true);
    StringHelper::Concatenate(path2, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(path2, "TestDirectorySize.txt");

    FILE *file;
    file = fopen(path2, "w");
    fputs("Hello this confirm my question", file);
    fclose(file);
    file = fopen(path, "w");
    fputs("Hello this confirm my question", file);
    fclose(file);

    Directory dir2(BASE_PATH);
    DirectoryScanner directoryScann(BASE_PATH);
    directoryScann.Scan(BASE_PATH);
    bool ok = (directoryScann.DirectorySize() == dir2.GetSize());
    //remove(path2);
    return ok;
}

bool DirectoryScannerTest::TestScan(char8 * pathin,
                                    char8 *fileMask) {
    char8 path[128];

    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(false);

    DirectoryScanner directoryScann(path);
    bool ok = directoryScann.Scan(path, fileMask);
    ok &= (directoryScann.DirectorySize() > 0);
    ok &= (StringHelper::Compare(directoryScann.BasePath(),path) == 0);
    return ok;
}

bool DirectoryScannerTest::TestScan_Filter() {
    char8 path[128];
    char8 pathA[128];
    char8 pathB[128];
    char8 pathC[128];

    DirectoryCreateN(pathB, "B");
    Directory dir(pathB);
    dir.Create(true);
    DirectoryCreateN(pathC, "C");
    Directory dir1(pathC);
    dir1.Create(true);
    DirectoryCreateN(pathA, "A");
    Directory dir2(pathA);
    dir2.Create(true);

    StringHelper::Copy(path, BASE_PATH);
    StringHelper::Concatenate(path, &DIRECTORY_SEPARATOR);
    DirectoryScanner directoryScann(path);
    directoryScann.Scan(path);
    Directory *file1AfterOrder = static_cast<Directory *>(directoryScann.ListPeek(0));
    Directory *file2AfterOrder = static_cast<Directory *>(directoryScann.ListPeek(1));
    Directory *file3AfterOrder = static_cast<Directory *>(directoryScann.ListPeek(2));

    bool ok = (StringHelper::Compare(file1AfterOrder->GetName(), pathA) == 0);
    ok &= (StringHelper::Compare(file2AfterOrder->GetName(), pathB) == 0);
    ok &= (StringHelper::Compare(file3AfterOrder->GetName(), pathC) == 0);
    return ok;
}

bool DirectoryScannerTest::TestBasePath(char8 * pathin) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    DirectoryScanner directoryScann(path);
    return (StringHelper::Compare(directoryScann.BasePath(), path) == 0);
}

bool DirectoryScannerTest::TestCleanUp(){
    DirectoryScanner dir(BASE_PATH);
    dir.CleanUp();
    bool ok = (dir.DirectorySize()==0);
    ok &= StringHelper::Compare(dir.BasePath(),NULL);
    return ok;
}

bool DirectoryScannerTest::CreateDirectoryFolder() {
    Directory dir(BASE_PATH);
    dir.Create(false);
    return dir.IsDirectory();
}

void DirectoryScannerTest::DirectoryCreateN(char8 *destination,
                                            char8 *path) {
    destination[0] = '\0';
    StringHelper::Concatenate(destination, BASE_PATH);
    StringHelper::Concatenate(destination, &DIRECTORY_SEPARATOR);
    StringHelper::Concatenate(destination, path);
}
