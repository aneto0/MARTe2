/**
 * @file HighResolutionTimer.h
 * @brief Header file for module HighResolutionTimer
 * @date 05/07/2015
 * @author André Neto
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

 * @details This header file contains the declaration of the module HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifdef RSTA2_CIRCLE
    #include "HighResolutionTimerCircle.h"
    #define CUSTOMIZED_HIGHRESOLUTIONTIMER_HEADER
#endif

#ifdef XILINX_ULTRASCALE
    #include "HighResolutionTimerXil.h"
    #define CUSTOMIZED_HIGHRESOLUTIONTIMER_HEADER
#endif

#ifndef CUSTOMIZED_HIGHRESOLUTIONTIMER_HEADER
    #include "HighResolutionTimerDefault.h"
#endif


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

