/**
 * @file HardwareI.h
 * @brief Header file for module HardwareI
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the module HardwareI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HARDWAREI_H_
#define HARDWAREI_H_ 

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L1Portability,ARCHITECTURE,HardwareIA.h)

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /*lint -e(766) the header file HardwareIA.h is not used here. This file is just for declaration and documentation. */
    /*lint -e(762) these declarations are redundant. */
    namespace HardwareI {

        /**
         * @brief Allows to initialise the Architecture before any MARTe function is called.
         */
        void InitArchitecture();

        /**
         * @brief Allows to initialise the Environment before any MARTe function is called.
         */
        void InitEnvironment();
    }

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HARDWAREI_H_ */
