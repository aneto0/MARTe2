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
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,DirectoryCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** it represents a file or a subdirectory */
class Directory: public LinkedListable {

    friend class DirectoryScanner;

public:
    /** constructor */
    Directory(const char8 * const path = NULL);

    /** */
    ~Directory();

    /** file name */
    const char8 *Name() const;

    /** is it a directory */
    bool IsDirectory() const;

    /** is it a file */
    bool IsFile() const;

    /** is it read only */
    bool ReadOnly() const;

    /** the file size */
    uint64 Size() const;

    /** last write time */
    int32 Time() const;

    /** last access time */
    int32 LastAccessTime() const;

private:
    /** the file or directory name */
    char8 *fname;

    /** attributes of the file */
    DirectoryCore dyrectoryHandle;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORY_H_ */

