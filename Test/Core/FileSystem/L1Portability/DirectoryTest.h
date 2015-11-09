/**
 * @file DirectoryTest.h
 * @brief Header file for class DirectoryTest
 * @date 04/11/2015
 * @author Frank Perez
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
#include "Directory.h"
#include "LinkedListHolder.h"
#include "stdio.h"
#include "time.h"
#include "StringHelper.h"
#include <sys/stat.h>
#include "dirent.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,DirectoryCore.h)
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/** it represents a file or a subdirectory */
class DirectoryTest {

public:
    /** constructor */
    DirectoryTest();

    /** */
    ~DirectoryTest();

    /**
     * @brief Set this file-directory providing its path.
     * @param[in] path is the path of the file-directory in input.
     * @return true if the file-directory is set correctly, false otherwise.
     * @post
     *  fname = path
     */
    bool TestSetByName();

    /**
     * @brief Retrieve the file-directory name.
     */
    bool TestName_Invalid();
    bool TestName_Valid();

    /**
     * @brief Check if this is a directory.
     * @return true if this is a directory, false otherwise.
     */
    bool TestIsDirectory_Valid();
    bool TestIsDirectory_Invalid();

    /**
     * @brief Check if this is a file.
     * @return true if this is a file, false otherwise.
     */
    bool TestIsFile_Yes();
    bool TestIsFile_No();

    /**
     * @brief Retrieve the file-directory size.
     * @return the file-directory size
     */
    bool TestGetSize_Dir();
    bool TestGetSize_File();

    /**
     * @brief Retrieve the last write time.
     * @return the last write time.
     */
    bool TestGetLastWriteTime();

    /**
     * @brief Retrieve the last access time.
     * @return the last access time.
     */
    bool TestGetLastAccessTime();

    /**
     * @brief Create a new file-directory.
     * @param[in] path is the path of the new folder to be created.
     * @param[in] isFile specifies if a file or a folder should be created.
     * @return true if the file-directory is created correctly, false otherwise.
     */
    bool TestCreate(char8 * path,
                    const bool isFile = false);

    /**
     * @brief Delete an existent file-directory.
     * @param[in] path is the path of the file / directory to be deleted.
     * @return true if the file-directory is deleted correctly, false otherwise.
     */
    bool TestDelete(char8 *  path,
                    bool file);

    /**
     * @brief Check if a file-directory exists.
     * @param[in] path is the path of the file-directory to check.
     * @return true if the file-directory exists, false otherwise.
     * @post
     *   isFile = true if path refers to a file.
     */
    bool TestExists(char8 * path);

    /**
     * @brief Create a test directory
     */
    void DirectoryCreateN(char8 *destination, char8 *path);

private:
    /** the file or directory name */
    char8 *fname;

    /** attributes of the file */
    DirectoryCore dyrectoryHandle;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORYTEST_H_ */

