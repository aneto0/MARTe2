/**
 * @file DirectoryScannerTest.h
 * @brief Header file for class DirectoryScannerTest
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

 * @details This header file contains the declaration of the class DirectoryScannerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORYSCANNERTEST_H_
#define DIRECTORYSCANNERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Directory.h"
#include "DirectoryScanner.h"
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

class DirectoryScannerTest {
public:
    /**
     * Constructor of DirectoryScannerTest
     */
    DirectoryScannerTest();

    /**
     * Destructor of DirectoryScannerTest
     */
    ~DirectoryScannerTest();

    /**
     * @brief Retrieve the size of the scanned directory.
     * @return true if the variable private size is equal to sum of the contents of directory.
     */
    bool TestDirectorySize();

    /**
     * @brief Scan the files / sub-directories of a specific directory adding them to the list.
     * @param[in] path is the path of the directory to scan.
     * @param[in] fileMask is shell wildcard pattern to match the desired files / sub-directories.
     * @return true if Scanner found the elements of directory concrete.
     */
    bool TestScan(const char8 * path,
                  const char8 *fileMask = "*");
    bool TestScan_NULL(const char8 * path,
                       const char8 *fileMask = "*");
    bool TestScan_NoExist(const char8 * path,
                          const char8 *fileMask = "*");

    /**
     * @brief Sorter specifies the desired sort of the elements in the list.
     * return true if the order of elements in the list are correct.
     */
    bool TestScan_Filter();

    /**
     * @brief Is shell wildcard pattern to match the desired files / sub-directories.
     * return true if the search was successful.
     */
    bool TestScan_Mask();

    /**
     * @brief Retrieve the directory path.
     * @param[in] pathin is the path to directory.
     * @return true if basePath is equal to pathin directory.
     */
    bool TestBasePath(const char8 * pathin);

    /**
     * @brief Deletes all elements from the list.
     * @post
     *   basePath = NULL;
     *   size = 0;
     */
    bool TestCleanUp();

    /**
     * @brief Create test directory folder
     */
    bool CreateDirectoryFolder();

    /**
     * @brief Create test directory
     */
    void DirectoryCreateN(char8 *destination,
                          const char8 *path);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_FILESYSTEM_L1PORTABILITY_DIRECTORYSCANNERTEST_H_ */

