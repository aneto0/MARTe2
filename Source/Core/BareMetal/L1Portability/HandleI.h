/**
 * @file HandleI.h
 * @brief Header file for class HandleI
 * @date 13/11/2015
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

 * @details This header file contains the declaration of the class HandleI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HANDLEI_H_
#define HANDLEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Interface which offers access to read and write filesystem handles.
 * @details This interface offers methods for getting system dependent read
 * and write handles for device classes like files, sockets and consoles.
 * @remark When the class that implements this interface has only one handle,
 * then is is expected that both methods (read and write handles) will return
 * the same handle.
 */
class DLL_API HandleI {
public:

    /**
     * Default constructor
     */
    HandleI() {
    }

    /**
     * @brief Default destructor
     */
    virtual ~HandleI() {
    }

    /**
     * @brief Queries the read handle
     */
    virtual Handle GetReadHandle() const = 0;

    /**
     * @brief Queries the write handle
     */
    virtual Handle GetWriteHandle() const = 0;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*HANDLEI_H_ */

