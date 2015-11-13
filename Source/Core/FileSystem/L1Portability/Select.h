/**
 * @file Select.h
 * @brief Header file for class Select
 * @date 12/11/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class Select
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SELECT_H_
#define SELECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT, SelectProperties.h)

namespace MARTe {

    class DLL_API Select {
    public:
        /**
         * @Brief Default constructor
         */
        Select();

        bool AddReadHandle(const StreamI &stream);
        bool AddWriteHandle(const StreamI &stream);
        bool AddExceptionHandle(const StreamI &stream);
        bool RemoveReadHandle(const StreamI &stream);
        bool RemoveWriteHandle(const StreamI &stream);
        bool RemoveExceptionHandle(const StreamI &stream);
        bool ClearAllHandle();
        int32 WaitUntil(TimeoutType msecTimeout = TTInfiniteWait);

    private:
        /*Contains informations about the read handles used in the select. Every AddReadHandle() action an handle informations is added to readHadle.*/
        SetIdentifier readHandle;

        /*Contains informations about the write handles used in the select. Every AddWriteHandle() action an handle informations is added to writeHandle.*/
        SetIdentifier writeHandle;

        /*Contains informations about the exceptions handles used in the select. Every AddExceptionHandle() action an handle informations is added to exceptionHandle.*/
        SetIdentifier exceptionHandle;

        /*It is the highest number of a handle that readHandle, writeHandle or exceptionHandle contains*/
        int32 highestHandle;
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* _SELECT_H_ */

