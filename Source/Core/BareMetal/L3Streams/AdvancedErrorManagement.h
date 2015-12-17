/**
 * @file AdvancedErrorManagement.h
 * @brief Header file for class AdvancedErrorManagement
 * @date 21/10/2015
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

 * @details This header file contains the declaration of the class AdvancedErrorManagement
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ADVANCEDERRORMANAGEMENT_H_
#define ADVANCEDERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "StreamMemoryReference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
static const uint32 MAX_ERROR_MESSAGE_SIZE = 200u;

#define REPORT_ERROR_PARAMETERS(code, message,...)                                           \
{                                                                                           \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                                 \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);                               \
    if(smr.Printf(reinterpret_cast<const char8 *>(message),__VA_ARGS__)) {                       \
        buffer[smr.Size()]='\0';                                                               \
        ErrorManagement::ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);\
    }                                                                                       \
    else{                                                                                   \
        ErrorManagement::ReportError(code,reinterpret_cast<const char8 *>(message),__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);\
    }                                                                                       \
    \
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ADVANCEDERRORMANAGEMENT_H_ */

