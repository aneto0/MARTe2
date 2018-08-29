/**
 * @file Sleep.cpp
 * @brief Source file for module Sleep
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

 * @details This source file contains the definition of all the methods for
 * the module Sleep (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <string.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "Sleep.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

void Sleep::OsUsleep(const uint32 usecTime) {
    if (usecTime > 0u) {
#if 0
        struct timespec tspec;
        /*lint -e{9115} -e{9122} conversion from float to integer allowed*/
        tspec.tv_sec = static_cast<time_t>(usecTime / 1e6);
        tspec.tv_nsec = static_cast<long>(usecTime - (tspec.tv_sec * 1000000u)) * 1000u;

        struct timespec rem;
        while (nanosleep(&tspec, &rem) == -1) {
            if (errno == EINTR) {
                tspec = rem;
            }
            else {
                break;
            }
        }
#endif
        (void) usleep(usecTime);
    }

}

int32 Sleep::GetDateSeconds() {
    return static_cast<int32>(time(static_cast<time_t *>(NULL)));
}

}
