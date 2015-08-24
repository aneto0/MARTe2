/**
 * @file Clock.h
 * @brief Header file for class Clock
 * @date 08/ago/2015
 * @author pc
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

 * @details This header file contains the declaration of the class Clock
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLOCK_H_
#define CLOCK_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_PLATFORM(PLATFORM,DriversDefinitionsP.h)

//#include "Platform/STM32F4-Discovery/DriversDefinitionsP.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class Clock {

public:

    /**
     * Initializes the rcc and other registers.
     */
    static void Reset(bool extRAMInit=false);

    /**
     * To change the sys clock configuration
     */
    static bool SysClockSet(uint32 clockType=1u);

    /**
     * To be called after a clock change.
     */
    static bool SysClockUpdate();

    static uint32 GetClockFrequency();

private:
    static bool initialized;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLOCK_H_ */

