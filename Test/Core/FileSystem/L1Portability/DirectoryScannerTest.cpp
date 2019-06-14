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

        return dir1->GetName().CompareContent(dir2->GetName());
    }
};

class DirectorySizeSorter: public SortFilter {
    virtual int32 Compare(LinkedListable *data1,
                          LinkedListable *data2) {
        Directory *dir1 = static_cast<Directory *>(data1);
        Directory *dir2 = static_cast<Directory *>(data2);

        int32 result = 0;
        if (dir1->GetSize() > dir2->GetSize()) {
        	result = 1;
        } else
            if (dir1->GetSize() < dir2->GetSize()) {
            	result = -1;
            }

        return result;
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
static CCString BASE_PATH = "GTestDirectoryScannerTest";

DirectoryScannerTest::DirectoryScannerTest() {

}
DirectoryScannerTest::~DirectoryScannerTest() {

}

bool DirectoryScannerTest::TestDirectorySize() {
    DynamicCString path;
    DynamicCString path2;
    DynamicCString path3;
    DirectoryCreateN(path, "TestDirectorySize");
    Directory dir(path);
    dir.Create(false);

    path2().Append(path).Append(DIRECTORY_SEPARATOR).Append("TestDirectorySize.txt");
    path3().Append(path).Append(DIRECTORY_SEPARATOR).Append("TestDirectorySize_Inside");
    Directory dir3(path3);
    dir3.Create(false);

    BasicFile file;
    file.Open(path2, file.ACCESS_MODE_W | file.FLAG_CREAT);
    CCString message("TestDirectorySize");
    uint32 size = message.GetSize();
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

static inline void Write2File(BasicFile &file, CCString message){
    uint32 size = message.GetSize();
    file.Write(message.GetList(), size);
}

bool DirectoryScannerTest::TestScan(CCString pathin,
                                    CCString fileMask) {
    DynamicCString path;
    DynamicCString path2;;
    DynamicCString path3;
    DynamicCString pathDot;
    DynamicCString pathDoubleDot;
    DirectoryCreateN(path, pathin);

    path2().Append(path).Append(DIRECTORY_SEPARATOR).Append("TestScan");
    path3().Append(path).Append(DIRECTORY_SEPARATOR).Append("TestScan.txt");
    pathDot().Append(path).Append(DIRECTORY_SEPARATOR).Append(".");
    pathDoubleDot().Append(path).Append(DIRECTORY_SEPARATOR).Append("..");

    Directory dir(path);
    Directory dir2(path2);
    Directory dir3(path3);
    dir.Create(false);
    dir2.Create(false);

    BasicFile file;
    file.Open(path3, file.ACCESS_MODE_W | file.FLAG_CREAT);
    Write2File(file, "File to scan");
    file.Close();

    DirectoryScanner directoryScann;
    directoryScann.Scan(path, fileMask);
    uint32 i = 0;
    bool ok = true;
    ///Compare the result of the search with the created files
    while (directoryScann.ListPeek(i) != NULL) {
        Directory *test = static_cast<Directory *>(directoryScann.ListPeek(i));
        if (test->GetName() == path2) {
            ok &= true;
        }
        else if (test->GetName() == path3) {
            ok &= true;
        }
        else if ((test->GetName()==pathDot) || (test->GetName()==pathDoubleDot)) {
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

bool DirectoryScannerTest::TestScan_NoExist(CCString pathin,
                                            CCString fileMask) {
    DynamicCString path;
    DynamicCString compare;
    DirectoryCreateN(path, pathin);
    compare().Append(path).Append(DIRECTORY_SEPARATOR);
    Directory directory(path);
    DirectoryScanner dir;
    bool ok = dir.Scan(path, fileMask);
    ok &= (dir.BasePath() == compare);
    if (!directory.Exists()) {
        ok = !ok;
    }
    return ok;
}

bool DirectoryScannerTest::TestScan_NULL(CCString pathin,
                                         CCString fileMask) {
    DirectoryScanner dir;
    bool ok = dir.Scan(pathin, fileMask);
    CCString basePath = dir.BasePath();
    ok &= (basePath==DIRECTORY_SEPARATOR);
    return ok;
}

bool DirectoryScannerTest::TestScan_Mask() {
    DynamicCString path;
    DynamicCString pathA;
    DynamicCString pathB;
    DynamicCString pathC;
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
    Write2File(file, "TestScan_Mask");
    file.Close();

    file.Open(pathC, file.ACCESS_MODE_W | file.FLAG_CREAT);
    Write2File(file, "TestScan_Mask");
    file.Close();

    file.Open(pathA, file.ACCESS_MODE_W | file.FLAG_CREAT);
    Write2File(file, "TestScan_Mask");
    file.Close();

    path = BASE_PATH;
    DirectoryScanner directoryScann;

    uint32 i = 0;
    directoryScann.Scan(path, "B");
    ///Look at list in search of the correct file
    while (directoryScann.ListPeek(i) != NULL) {
        Directory *fileB = static_cast<Directory *>(directoryScann.ListPeek(i));
        if (fileB->GetName() == pathB) {
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
        if (fileC->GetName() == pathC) {
            ok &= true;
        }
        else if (fileC->GetName() == pathA) {
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

    DynamicCString pathA;
    DynamicCString pathB;
    DynamicCString pathC;
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
    Write2File(file, "Medium size");
    file.Close();

    file.Open(pathC, file.ACCESS_MODE_W | file.FLAG_CREAT);
    Write2File(file, "Long size are the file C");
    file.Close();

    file.Open(pathA, file.ACCESS_MODE_W | file.FLAG_CREAT);
    Write2File(file, "Little");
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

bool DirectoryScannerTest::TestBasePath(CCString pathin) {
    DynamicCString path;
    DirectoryCreateN(path, pathin);
    Directory dir(path);
    dir.Create(false);
    DirectoryScanner directoryScann;
    directoryScann.Scan(path);
    path().Append(DIRECTORY_SEPARATOR);
    bool ok = (directoryScann.BasePath() == path);
    ok &= dir.Delete();
    return ok;
}

bool DirectoryScannerTest::TestCleanUp() {
    DynamicCString path;
    DirectoryCreateN(path, "TestCleanUp");
    Directory dir1(path);
    dir1.Create(false);
    DirectoryScanner dir;
    dir.CleanUp();
    bool ok = (dir.BasePath().IsNullPtr());
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

void DirectoryScannerTest::DirectoryCreateN(DynamicCString &destination,CCString path) {
    destination = "";
    destination().Append(BASE_PATH).Append(DIRECTORY_SEPARATOR).Append(path);
}
