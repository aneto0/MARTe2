/**
 * @file DirectoryScanner.h
 * @brief Header file for class DirectoryScanner
 * @date 04/11/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class DirectoryScanner
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORYSCANNER_H_
#define DIRECTORYSCANNER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "Directory.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class allows to scan, given a base path and a shell wildcard pattern, all the directories inside
 * will matches the pattern.
 *
 * @details The informations about found directories will be inserted in a list of Directory objects
 * and the user could peek all the elements using LinkedListHolder functions.
 */
class DirectoryScanner: public LinkedListHolder {

public:

    /**
     * @brief Default constructor.
     * @post
     *   basePath=NULL;
     *   size=0;
     */
    DirectoryScanner();

    /**
     * @brief Default destructor.
     * @post
     *   basePath=NULL;
     *   size=0;
     */
    virtual ~DirectoryScanner();


    /**
     * @brief Retrieve the size of the scanned directory.
     * @return the sum of the contained files / sub-directories sizes.
     */
    uint64 DirectorySize() const;

    /**
     * @brief Scan the files / sub-directories of a specific directory adding them to the list.
     * @param[in] path is the path of the directory to scan.
     * @param[in] fileMask is shell wildcard pattern to match the desired files / sub-directories.
     * @param[in] sorter specifies the desired sort of the elements in the list.
     * @return false in case of errors, true otherwise.
     */
    bool Scan(const char8 * const path,
              const char8 *fileMask = "*",
              SortFilter * const sorter = NULL);

    /**
     * @brief Retrieve the directory path.
     * @return the path of the set directory.
     */
    const char8 *BasePath() const;

    /**
     * @brief Deletes all elements from the list.
     * @post
     *   basePath = NULL;
     *   size = 0;
     */
    virtual void CleanUp();

private:

    /**
     * The directory path.
     */
    char8* basePath;

    /**
     * The total content size.
     */
    uint64 size;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORYSCANNER_H_ */

