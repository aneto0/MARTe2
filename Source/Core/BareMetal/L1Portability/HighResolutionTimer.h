/**
 * @file HighResolutionTimer.h
 * @brief Header file for module HighResolutionTimer
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

 * @details This header file contains the declaration of the module HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIME_H_
#define HIGHRESOLUTIONTIME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//#include "ErrorManagement.h"
#include "GeneralDefinitions.h"
#include "TimeStamp.h"
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L1Portability,ARCHITECTURE,HighResolutionTimerA.h)
#include INCLUDE_FILE_ENVIRONMENT(BareMetal,L1Portability,ENVIRONMENT,HighResolutionTimer.h)

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /*lint -e{762} the functions are declared in the public header file and defined in the header file of the specific architecture.*/
    namespace HighResolutionTimer {

        /**
         * @brief A high resolution 64-bit time counter.
         * @details Reads the cpu ticks on an 64 bits integer.
         */
        inline uint64 Counter();

        /**
         * @brief A high resolution 32-bit time counter.
         * @details Reads the cpu ticks on an 32 bits integer.
         */
        inline uint32 Counter32();

        /**
         * @brief Gets the cpu clock period in seconds.
         * @return the current period of the cpu.
         */
        inline float64 Period();

        /**
         * @brief Gets the cpu clock frequency.
         * @return the current frequency of the cpu.
         */
        inline uint64 Frequency();

        /**
         * @brief Converts HighResolutionTimer ticks to time in seconds.
         * @param[in] tStop is the final ticks number.
         * @param[in] tStart is the initial ticks number.
         * @return the time elapsed in seconds
         */
        inline float64 TicksToTime(uint64 tStop, uint64 tStart = 0u);

        /**
         * @brief Gets the current time stamp [microseconds, seconds, minutes, hour, day, month, year].
         * @see TimeValues.
         * @param[out] date is a TimeValues structure which must be filled by this method.
         * @return true if the operating system calls returns with no errors.
         */
        inline bool GetTimeStamp(TimeStamp &date);
    }

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIME_H_ */
