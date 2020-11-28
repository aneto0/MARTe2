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
static const char8 * const BASE_PATH = "GTestDirectoryScannerTest";

DirectoryScannerTest::DirectoryScannerTest() {

}
DirectoryScannerTest::~DirectoryScannerTest() {

}

bool DirectoryScannerTest::TestDirectorySize() {
    char8 path[128];
    char8 path2[128];
    char8 path3[128];
    DirectoryCreateN(path, "TestDirectorySize");
    Directory dir(path);
    dir.Create(false);

    StringHelper::Copy(path2, path);
    StringHelper::Copy(path3, path);
    StringHelper::ConcatenateN(path2, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(path2, "TestDirectorySize.txt");
    StringHelper::ConcatenateN(path3, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(path3, "TestDirectorySize_Inside");
    Directory dir3(path3);
    dir3.Create(false);

    BasicFile file;
    file.Open(path2, file.ACCESS_MODE_W | file.FLAG_CREAT);
    uint32 size = StringHelper::Length("TestDirectorySize");
    file.Write("TestDirectorySize", size);
    file.Close();

    Directory dir2(path2);
    DirectoryScanner directoryScann;
    directoryScann.Scan(path);
    ///Compare the directoryScanner size with the sum of the both files/directory created
    bool ok = (directoryScann.DirectorySize() == (dir2.GetSize() + dir3.GetSize()));
    ok &= dir2.Delete();
    ok &= dir3.Delete();
    ok &= dir.Delete();
    return ok;
}

bool DirectoryScannerTest::TestScan(const char8 * pathin,
                                    const char8 *fileMask) {
    char8 path[128];
    char8 path2[128];
    char8 path3[128];
    char8 pathDot[128];
    char8 pathDoubleDot[128];
    DirectoryCreateN(path, pathin);
    StringHelper::Copy(path2, path);
    StringHelper::Copy(path3, path);
    StringHelper::Copy(pathDot, path);
    StringHelper::Copy(pathDoubleDot, path);
    StringHelper::ConcatenateN(pathDot, &DIRECTORY_SEPARATOR, 1);
    StringHelper::ConcatenateN(pathDoubleDot, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(pathDot, ".");
    StringHelper::Concatenate(pathDoubleDot, "..");
    StringHelper::ConcatenateN(path2, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(path2, "TestScan");
    StringHelper::ConcatenateN(path3, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(path3, "TestScan.txt");

    Directory dir(path);
    Directory dir2(path2);
    Directory dir3(path3);
    dir.Create(false);
    dir2.Create(false);

    BasicFile file;
    file.Open(path3, file.ACCESS_MODE_W | file.FLAG_CREAT);
    uint32 size = StringHelper::Length("File to scan");
    file.Write("File to scan", size);
    file.Close();

    DirectoryScanner directoryScann;
    directoryScann.Scan(path, fileMask);
    uint32 i = 0;
    bool ok = true;
    ///Compare the result of the search with the created files
    while (directoryScann.ListPeek(i) != NULL) {
        Directory *test = static_cast<Directory *>(directoryScann.ListPeek(i));
        if (StringHelper::Compare(test->GetName(), path2) == 0) {
            ok &= true;
        }
        else if (StringHelper::Compare(test->GetName(), path3) == 0) {
            ok &= true;
        }
        else if ((StringHelper::Compare(test->GetName(), pathDot) == 0) || (StringHelper::Compare(test->GetName(), pathDoubleDot) == 0)) {
            ok &= true;
        }
        else {
            ok &= false;
        }
        i++;
    }
    ok &= dir3.Delete();
    ok &= dir2.Delete();
    ok &= dir.Delete();
    return ok;
}

bool DirectoryScannerTest::TestScan_NoExist(const char8 * pathin,
                                            const char8 *fileMask) {
    char8 path[128];
    char8 compare[128];
    DirectoryCreateN(path, pathin);
    StringHelper::Copy(compare, path);
    StringHelper::ConcatenateN(compare, &DIRECTORY_SEPARATOR, 1);
    Directory directory(path);
    DirectoryScanner dir;
    bool ok = dir.Scan(path, fileMask);
    ok &= (StringHelper::Compare(dir.BasePath(), compare) == 0);
    if (!directory.Exists()) {
        ok = !ok;
    }
    return ok;
}

bool DirectoryScannerTest::TestScan_NULL(const char8 * pathin,
                                         const char8 *fileMask) {
    DirectoryScanner dir;
    bool ok = dir.Scan(pathin, fileMask);
    ok &= (StringHelper::CompareN(dir.BasePath(), &DIRECTORY_SEPARATOR, 1) == 0);
    return ok;
}

bool DirectoryScannerTest::TestScan_Mask() {
    char8 path[128];
    char8 pathA[128];
    char8 pathB[128];
    char8 pathC[128];
    bool ok = true;
    DirectoryCreateN(pathC, "C");
    Directory dir1(pathC);
    dir1.Create(true);
    DirectoryCreateN(pathB, "B");
    Directory dir(pathB);
    dir.Create(true);
    DirectoryCreateN(pathA, "CA");
    Directory dir2(pathA);
    dir2.Create(true);

    BasicFile file;
    file.Open(pathB, file.ACCESS_MODE_W | file.FLAG_CREAT);
    uint32 size = StringHelper::Length("TestScan_Mask");
    file.Write("TestScan_Mask", size);
    file.Close();

    file.Open(pathC, file.ACCESS_MODE_W | file.FLAG_CREAT);
    size = StringHelper::Length("TestScan_Mask");
    file.Write("TestScan_Mask", size);
    file.Close();

    file.Open(pathA, file.ACCESS_MODE_W | file.FLAG_CREAT);
    size = StringHelper::Length("TestScan_Mask");
    file.Write("TestScan_Mask", size);
    file.Close();

    StringHelper::Copy(path, BASE_PATH);
    DirectoryScanner directoryScann;

    uint32 i = 0;
    directoryScann.Scan(path, "B");
    ///Look at list in search of the correct file
    while (directoryScann.ListPeek(i) != NULL) {
        Directory *fileB = static_cast<Directory *>(directoryScann.ListPeek(i));
        if (StringHelper::Compare(fileB->GetName(), pathB) == 0) {
            ok &= true;
        }
        else {
            ok &= false;
        }
        i++;
    }

    i = 0;
    directoryScann.Scan(path, "C*");
    while (directoryScann.ListPeek(i) != NULL) {
        Directory *fileC = static_cast<Directory *>(directoryScann.ListPeek(i));
        if (StringHelper::Compare(fileC->GetName(), pathC) == 0) {
            ok &= true;
        }
        else if (StringHelper::Compare(fileC->GetName(), pathA) == 0) {
            ok &= true;
        }
        else {
            ok &= false;
        }
        i++;
    }

    ok &= dir.Delete();
    ok &= dir1.Delete();
    ok &= dir2.Delete();
    return ok;
}

bool DirectoryScannerTest::TestScan_Filter() {

    char8 pathA[128];
    char8 pathB[128];
    char8 pathC[128];
    bool ok = false;

    DirectoryCreateN(pathB, "B.txt");
    Directory dir(pathB);
    dir.Create(true);
    DirectoryCreateN(pathC, "C.txt");
    Directory dir1(pathC);
    dir1.Create(true);
    DirectoryCreateN(pathA, "A.txt");
    Directory dir2(pathA);
    dir2.Create(true);

    BasicFile file;
    file.Open(pathB, file.ACCESS_MODE_W | file.FLAG_CREAT);
    uint32 size = StringHelper::Length("Medium size");
    file.Write("Medium size", size);
    file.Close();

    file.Open(pathC, file.ACCESS_MODE_W | file.FLAG_CREAT);
    size = StringHelper::Length("Long size are the file C");
    file.Write("Long size are the file C", size);
    file.Close();

    file.Open(pathA, file.ACCESS_MODE_W | file.FLAG_CREAT);
    size = StringHelper::Length("Little");
    file.Write("Little", size);
    file.Close();

    DirectoryScanner directoryScann;
    DirectorySizeSorter sorter;
    directoryScann.Scan(BASE_PATH, "*", &sorter);

    uint32 i = 0;
    while (directoryScann.ListPeek(i + 1) != NULL) {
        Directory *file = static_cast<Directory *>(directoryScann.ListPeek(i));
        Directory *file2 = static_cast<Directory *>(directoryScann.ListPeek(i + 1));
        //Compare file size, the file2 > file always
        if (file2->GetSize() >= file->GetSize()) {
            ok = true;
        }
        else {
            ok = false;
        }
        i++;
    }

    ok &= dir.Delete();
    ok &= dir1.Delete();
    ok &= dir2.Delete();
    return ok;
}

bool DirectoryScannerTest::TestBasePath(const char8 * pathin) {
    char8 path[128];
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(false);
    DirectoryScanner directoryScann;
    directoryScann.Scan(path);
    StringHelper::ConcatenateN(path, &DIRECTORY_SEPARATOR, 1);
    bool ok = (StringHelper::Compare(directoryScann.BasePath(), path) == 0);
    ok &= dir.Delete();
    return ok;
}

bool DirectoryScannerTest::TestCleanUp() {
    char8 path[128];
    DirectoryCreateN(path, "TestCleanUp");
    Directory dir1(path);
    dir1.Create(false);
    DirectoryScanner dir;
    dir.CleanUp();
    bool ok = (dir.BasePath() == NULL);
    DirectoryScanner directory;
    directory.Scan(BASE_PATH);

    uint32 i = 0;
    while (directory.ListPeek(i) != NULL) {
        Directory *direc = static_cast<Directory *>(directory.ListPeek(i));
        Directory dirDel(direc->GetName());
        dirDel.Delete();
        i++;
    }

    Directory dirBase(BASE_PATH);
    ok &= dirBase.Delete();
    return ok;
}

bool DirectoryScannerTest::CreateDirectoryFolder() {
    Directory dir(BASE_PATH);
    dir.Create(false);
    return dir.IsDirectory();
}

void DirectoryScannerTest::DirectoryCreateN(char8 *destination,
                                            const char8 *path) {
    destination[0] = '\0';
    StringHelper::Concatenate(destination, BASE_PATH);
    StringHelper::ConcatenateN(destination, &DIRECTORY_SEPARATOR, 1);
    StringHelper::Concatenate(destination, path);
}
