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

class DirectoryScanner: public LinkedListHolder {


public:
    /** constructor */
    DirectoryScanner();

    /** destructor */
    virtual ~DirectoryScanner();

    /** Fills the data in the object*/
    static bool Create(const char8 * const address);

    /** check for directory existance */
    static bool DirectoryExists(const char8 * const address);

    /** total size of files in the directory. not recursive */
    uint64 DirectorySize() const;

    bool Scan(const char8 * const address,
              const char8 *fileMask = "*",
              SortFilter * const sorter = NULL);

    const char8 *BaseAddress() const;

    virtual void CleanUp();


private:

    char8* baseAddress;

    /** total content size */
    uint64 size;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORYSCANNER_H_ */

