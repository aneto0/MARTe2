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
#include "StringHelper.h"
#include "SortFilter.h"
#include "BasicFile.h"
#include "stdio.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,DirectoryCore.h)
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the DirectoryScanner class.
 */
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
     * @brief Check the size of the scanned directory.
     */
    bool TestDirectorySize();

    /**
     * @brief Check if Scan add all file/directory to the List.
     * @param[in] path is the path of the directory to scan.
     * @param[in] fileMask is shell wildcard pattern to match the desired files / sub-directories.
     */
    bool TestScan(const char8 * path,
                  const char8 *fileMask = "*");

    /**
     * @brief Check if Scan add all file/directory to the List.
     * @param[in] path Contain NULL.
     * @param[in] fileMask is shell wildcard pattern to match the desired files / sub-directories.
     */
    bool TestScan_NULL(const char8 * path,
                       const char8 *fileMask = "*");

    /**
     * @brief Check if Scan add all file/directory to the List.
     * @param[in] path Contain a path that not exist.
     * @param[in] fileMask is shell wildcard pattern to match the desired files / sub-directories.
     */
    bool TestScan_NoExist(const char8 * path,
                          const char8 *fileMask = "*");

    /**
     * @brief Check if the elements in the list are sort, in this occasion by size.
     */
    bool TestScan_Filter();

    /**
     * @brief Check if after the mask, in the list only appear the elements searched.
     */
    bool TestScan_Mask();

    /**
     * @brief Check if after the scan, the basePath are the same that the pathin .
     * @param[in] pathin is the path to directory.
     */
    bool TestBasePath(const char8 * pathin);

    /**
     * @brief Check if all elements from the list are been deleted.
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

