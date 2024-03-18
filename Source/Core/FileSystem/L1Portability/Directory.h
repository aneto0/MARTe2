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
#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "LinkedListHolder.h"
#include "MemoryOperationsHelper.h"
#include "StringHelper.h"
#include "TimeStamp.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,DirectoryCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief A class which encapsulates attributes and information about
     * a file or a directory.
     *
     * @details The instances of this class are meant to be bound to a given
     * filesystem item (file or directory), so it will be able to be queried
     * about its properties. Is is also available to create and delete any
     * filesystem item.
     *
     * @warning This class needs the target environment being properly
     * configured, in order to execute some of its methods. For instance,
     * getting the last access time for a file on Windows could fail,
     * because it is a feature that can be disabled on that operating
     * system.
     *
     * **How to verify if the last access time stamping is enabled on
     * Windows?**
     *
     * Execute the following command:
     *
     *     fsutil behavior query disablelastaccess
     *
     * **How to enable the last access time stamping on Windows?**
     *
     * Execute the following command as administrator and restart the computer.
     *
     *     fsutil behavior set disablelastaccess 0
     */
    class DLL_API Directory: public LinkedListable {

    public:
        /**
         * @brief Default constructor.
         * @param[in] path is the path of the file/directory
         * @post
         *   GetName() = path
         */
        Directory(const char8 * const path = NULL);

        /**
         * @brief Default destructor.
         * @post
         *   GetName() = NULL
         */
        ~Directory();

        /**
         * @brief Sets this file/directory absolute path.
         * @param[in] path is the absolute path of the file/directory.
         * @return true if the file/directory path is correctly set.
         * @post
         *   GetName() = path
         */
        bool SetByName(const char8 * const path);

        /**
         * @brief Gets the file/directory name.
         * @return the file/directory absolute path name.
         */
        const char8 *GetName() const;

        /**
         * @brief Check if this instance represents is a directory.
         * @return true if this instance represents is a directory.
         */
        bool IsDirectory() const;

        /**
         * @brief Check if this instance represents a file.
         * @return true if this instance represents a file.
         */
        bool IsFile() const;

        /**
         * @brief Gets the file/directory size.
         * @return the file/directory size.
         */
        uint64 GetSize();

        /**
         * @brief Gets the last write time.
         * @return the last write time.
         */
        TimeStamp GetLastWriteTime();

        /**
         * @brief Gets the last access time.
         * @return the last access time.
         */
        TimeStamp GetLastAccessTime();

        /**
         * @brief Creates a new file/directory.
         * @param[in] isFile specifies if a file or a folder should be created.
         * @return true if the file/directory is created correctly. Note that if a file is being
         * created and it already exists, this will not affect the creation of the file.
         * @pre
         *    IsDirectory() = true && Exists() = false
         * @post
         *    GetName() = path
         */
        bool Create(const bool isFile = false);

        /**
         * @brief Deletes an existent file/directory.
         * @return true if the file/directory is deleted correctly, false otherwise.
         */
        bool Delete();

        /**
         * @brief Check if a file/directory exists.
         * @return true if the file/directory exists, false otherwise.
         */
        bool Exists();

    private:
        /**
         * The file or directory name
         */
        char8 *fname;

        /**
         * File/directory attributes.
         */
        DirectoryCore directoryHandle;

    };

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORY_H_ */

