/**
 * @file DirectoryTest.h
 * @brief Header file for class DirectoryTest
 * @date 04/11/2015
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

 * @details This header file contains the declaration of the class DirectoryTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORYTEST_H_
#define DIRECTORYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicFile.h"
#include "Directory.h"
#include "DirectoryScanner.h"
#include "LinkedListHolder.h"
#include "Sleep.h"
#include "stdio.h"
#include "StringHelper.h"
#include "TimeStamp.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,DirectoryCore.h)
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the Directory class.
 * @warning In case some tests fail, check Directory class' documentation
 * for operating system configuration requirements.
 */
class DirectoryTest {

public:
    /**
     * Constructor */
    DirectoryTest();

    /**
     * @brief Constructor test
     * param[in] pathin Content the directory that will create for be tested.
     */
    bool TestDirectoryTest(const char8 * pathin);

    /**
     * @brief Destructor
     */
    ~DirectoryTest();

    /**
     * @brief Test to set directory correctly.
     */
    bool TestSetByName_Valid();

    /**
     * @brief Test to set directory NULL.
     */
    bool TestSetByName_NULL();

    /**
     * @brief Test to set directory Invalid.
     */
    bool TestSetByName_Invalid();

    /**
     * @brief Tests the GetName function with an invalid path.
     */
    bool TestGetName_Invalid();

    /**
     * @brief Tests the GetName with a valid path.
     */
    bool TestGetName_Valid();

    /**
     * @brief Check if this is a valid directory.
     */
    bool TestIsDirectory_Valid();

    /**
     * @brief Check if this is a invalid directory.
     */
    bool TestIsDirectory_Invalid();

    /**
     * @brief Check if this is a file with a file.
     */
    bool TestIsFile_Yes();

    /**
     * @brief Check if this is a file with a folder.
     */
    bool TestIsFile_No();

    /**
     * @brief Check the size of a directory.
     */
    bool TestGetSize_Dir();

    /**
     * @brief Check the size of two file with the same length.
     */
    bool TestGetSize_FileCorrect();

    /**
     * @brief Check the size of two file with different length.
     */
    bool TestGetSize_FileIncorrect();

    /**
     * @brief Check if the file was wrote one second after without write again.
     */
    bool TestGetLastWriteTime();

    /**
     * @brief Check if the file was wrote one second after write again.
     */
    bool TestGetLastWriteTime_ReWrite();

    /**
     * @brief Check if the file was read one second after without read again.
     */
    bool TestGetLastAccessTime();

    /**
     * @brief Check if the file was read one second after read again.
     */
    bool TestGetLastAccessTime_ReRead();

    /**
     * @brief Create a new file-directory.
     * @param[in] path is the path of the new folder to be created.
     * @param[in] isFile specifies if a file or a folder should be created.
     */
    bool TestCreate(const char8 * path,
                    const bool isFile = false);

    /**
     * @brief Check is deleted an existent file-directory.
     * @param[in] path is the path of the file / directory to be deleted.
     * @param[in] file is for know if it will create a file or a folder to be deleted.
     */
    bool TestDelete(const char8 * path,
                    bool file);

    /**
     * @brief Check if a file-directory exists.
     * @param[in] path is the path of the file-directory to will be create.
     * @param[in] file is for know if it will create a file or a folder to check if exist.
     */
    bool TestExists(const char8 * path,
                    bool isFile);

    /**
     * @brief Create a test directory
     * @param[in] destination is the path of the file-directory to will be create.
     * @param[in] path is the sub-directory that we want create.
     */
    void DirectoryCreateN(char8 *destination,
                          const char8 *path);

    /**
     * @brief Create test directory folder
     */
    bool Create_Directory();

    /**
     * @brief Delete test directory folder
     */
    bool Delete_Directory();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORYTEST_H_ */

