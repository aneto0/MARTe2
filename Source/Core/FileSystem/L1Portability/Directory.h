/**
 * @file Directory.h
 * @brief Header file for class Directory
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

 * @details This header file contains the declaration of the class Directory
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListHolder.h"
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,DirectoryCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class which represent a file or a directory.
 *
 * @details Providing the path name of a specific file or directory, this class can
 * obtain all its main attributes (size, last access time, ecc).
 *
 * @details Some static methods allows the user to create, delete and check the existence of files-directories.
 */
class Directory: public LinkedListable {


public:
    /**
     * @brief Default constructor.
     * @param[in] path is the path of the file-directory
     * @post
     *   Name() = path
     */
    Directory(const char8 * const path = NULL);

    /**
     * @brief Default destructor.
     * @post
     *   Name() = NULL
     */
    ~Directory();

    /**
     * @brief Set this file-directory providing its path.
     * @param[in] path is the path of the file-directory in input.
     * @return true if the file-directory is set correctly, false otherwise.
     * @post
     *   Name() = path
     */
    bool SetByName(const char8 * const path);

    /**
     * @brief Gets the file-directory path name (as set in the constructor or by SetByName).
     * @return the file-directory path name (as set in the constructor or by SetByName).
     */
    const char8 *GetName() const;

    /**
     * @brief Check if this is a directory.
     * @return true if this is a directory, false otherwise.
     */
    bool IsDirectory() const;

    /**
     * @brief Check if this is a file.
     * @return true if this is a file, false otherwise.
     */
    bool IsFile() const;

    /**
     * @brief Retrieve the file-directory size.
     * @return the file-directory size
     */
    uint64 GetSize();

    /**
     * @brief Retrieve the last write time.
     * @return the last write time.
     */
    TimeValues GetLastWriteTime();

    /**
     * @brief Retrieve the last access time.
     * @return the last access time.
     */
    TimeValues GetLastAccessTime();


    /**
     * @brief Create a new file-directory.
     * @param[in] path is the path of the new folder to be created.
     * @param[in] isFile specifies if a file or a folder should be created.
     * @return true if the file-directory is created correctly, false otherwise.
     * @post
     *    Name() = path
     */
    bool Create(const bool isFile = false);

    /**
     * @brief Delete an existent file-directory.
     * @param[in] path is the path of the file / directory to be deleted.
     * @return true if the file-directory is deleted correctly, false otherwise.
     */
    bool Delete();

    /**
     * @brief Check if a file-directory exists.
     * @param[in] path is the path of the file-directory to check.
     * @return true if the file-directory exists, false otherwise.
     * @post
     *   isFile = true if path refers to a file.
     */
    bool Exists();

private:
    /**
     * The file or directory name
     */
    char8 *fname;

    /**
     * File-directory attributes
     */
    DirectoryCore directoryHandle;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORY_H_ */

