/**
 * @file DirectoryCore.h
 * @brief Header file for class DirectoryCore
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

 * @details This header file contains the declaration of the class DirectoryCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DIRECTORYCORE_H_
#define DIRECTORYCORE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ff.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

    /**
     * @brief Structure to MARTe2-ize the FatFs file information structure 
     */
    typedef struct {
        uint64  size;                               /* File size */

        uint16  dateYear;                           /* Last modified date */
        uint8   dateMonth;
        uint8   dateDay;

        uint8   timeHour;                           /* Last modified time */
        uint8   timeMinute;
        uint8   timeSecond;

        bool    isReadOnly;                         /* Attribute flag combination */
        bool    isHidden;
        bool    isSystem;
        bool    isArchive;
        bool    isDirectory;

        #if FF_USE_LFN                              /* FatFs has LongFileName support */
        char8   alternativeName[FF_SFN_BUF + 1];    /* Alternative object name */ 
        char8   fileName[FF_LFN_BUF + 1];           /* Primary object */
        #else
        char8   fileName[12 + 1];                   /* Object name in 8.3 */
        #endif
    }DirectoryCore;

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DIRECTORYCORE_H_ */

