/**
 * @file Bootstrap.h
 * @brief Header file for class Bootstrap
 * @date 04/04/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class Bootstrap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_BOOTSTRAP_H_
#define L6APP_BOOTSTRAP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamI.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief TODO.
 *
 * @details TODO.
 *
 */
class DLL_API Bootstrap {
public:

    /**
     * @brief TODO
     */
    Bootstrap();

    /**
     * @brief TODO
     */
    ~Bootstrap();

    /**
     * @brief TODO
     */
    ErrorManagement::ErrorType ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters, StreamI &configurationStream);

    /**
     * @brief TODO
     */
    ErrorManagement::ErrorType Run();

    /**
     * @brief TODO
     */
    void Printf(const char8 * const msg);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_BOOTSTRAP_H_ */
