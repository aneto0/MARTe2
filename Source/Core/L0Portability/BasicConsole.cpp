/**
 * @file BasicConsole.cpp
 * @brief Source file for class BasicConsole
 * @date 22/06/2015
 * @author Giuseppe Ferr�
 *
 * @copyright Copyright 2015 F4E | European Joint32 Undertaking for ITER and
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
 * the class BasicConsole (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicConsole.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
const Flags IBasicConsole::Mode::Default(0u);
const Flags IBasicConsole::Mode::CreateNewBuffer(1u);
const Flags IBasicConsole::Mode::PerformCharacterInput(2u);
const Flags IBasicConsole::Mode::DisableControlBreak(4u);
const Flags IBasicConsole::Mode::EnablePaging(8u);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
BasicConsole::BasicConsole() :
        BasicConsoleOS::BasicConsoleOS() {
    lastPagingCounter = 0;
    lineCount = 0u;
}

BasicConsole::~BasicConsole() {
}

bool BasicConsole::PagedWrite(const char8 *buffer, uint32 &size, const TimeoutType &timeout) {

    uint32 numberOfColumnsTmp = 0u;
    uint32 numberOfRowsTmp = 0u;
    bool ok = GetSize(numberOfColumnsTmp, numberOfRowsTmp);
    if (ok) {

        //-1 means the maximum size.
        uint32 numberOfRows = numberOfRowsTmp;

        int64 t0 = lastPagingCounter;
        int64 t1 = HighResolutionTimer::Counter();

        int64 dTicks = t1 - t0;
        float64 dTime = static_cast<float64>(dTicks) * HighResolutionTimer::Period();
        if (dTime > 0.05) {
            lineCount = 0u;
            lastPagingCounter = t1;
        }

        const char8 *p = buffer;
        uint32 index = 0u;
        uint32 start = 0u;
        uint32 sizeT;
        bool end = false;
        while ((index < size) && !end) {
            while ((lineCount < (numberOfRows - 1u)) && (index < size) && !end) {
                if (p[index] == '\n') {
                    lineCount++;
                }
                if (p[index] == '\0') {
                    end = true;
                }
                index++;
            }
            sizeT = index - start;
            ok = !BasicConsoleOS::Write(&p[start], sizeT, TTInfiniteWait);
            if (!ok) {
                end = true;
            }
            if (ok && (lineCount >= (numberOfRows - 1u))) {
                start = index;
                lastPagingCounter = t1;
                lineCount = 0u;
                const char8 *message = "[PAGING] ENTER TO CONTINUE\015";
                sizeT = StringHelper::Length(message);
                BasicConsoleOS::Write(message, sizeT, TTInfiniteWait);
                char8 buffer[32];
                sizeT = N_CHARS_NEWLINE;
                ok = Read(buffer, sizeT, timeout);
            }
        }
    }

    return ok;
}

bool BasicConsole::Write(const char * buffer, uint32& size, const TimeoutType &timeout) {
    uint32 numberOfRows = 0u;
    uint32 numberOfColumns = 0u;
    bool ok = GetSize(numberOfColumns, numberOfRows);
    if ((size > 0u) && (buffer != NULL) && (numberOfColumns > 0u) && (numberOfRows > 0u)) {
        Flags mode = GetOpeningMode();
        if ((mode & IBasicConsole::Mode::EnablePaging) != 0u) {
            ok = PagedWrite(buffer, size, timeout);
        }
        else {
            ok = BasicConsoleOS::Write(buffer, size, timeout);
        }
    }
    else {
        ok = false;
    }
    return ok;
}

bool BasicConsole::Read(char8 *buffer, uint32 &size, const TimeoutType &timeout) {
    return BasicConsoleOS::Read(buffer, size, timeout);
}
