/**
 * @file ObjectRegistryDatabase.h
 * @brief Header file for class ObjectRegistryDatabase
 * @date 18/02/2016
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
 *
 * @details This header file contains the declaration of the class ObjectRegistryDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OS_INITIALIZER_H_
#define OS_INITIALIZER_H_

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
 * @brief Object built automatically. Construction initialises static variables containing generally usable variables.
 */
extern class DLL_API OSInitializer {

public:

    /**
     * Number of cpu ticks in a second
     */
    static uint64 frequency;

    /**
     * Time between two ticks in seconds
     */
    static float64 period;

    /**
     * Stores the seconds (counting from the epoch) at which a framework instance was executed.
     */
    static oslong initialSecs;

    /**
     * Stores the microseconds (counting from the epoch) at which a framework instance was executed.
     */
    static oslong initialUSecs;

    /**
     * Number of elapsed ticks at the time at which a framework instance was executed.
     */
    static uint64 initialTicks;

    /**
     * typical worst case number of ticks the OS will consume during a sleep if CPU not used.
     */
    static uint64 osSleepTicks;

    /**
     * typical worst case number of usec the OS will consume during a sleep if CPU not used.
     */
    static uint32 osSleepUsec;

    /**
     * minimum value o be used in a sleep call to guarantee some sleep is actually performed.
     */
    static uint32 osMinSleepUsec;

    /**
     * @brief Default constructor.
     */
    OSInitializer();

    /**
     * @brief Destructor.
     */
    ~OSInitializer();

private:

} OSInit;

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OS_INITIALIZER_H_ */

