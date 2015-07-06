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
/*const Flags IBasicConsole::Mode::Default(0u);
const Flags IBasicConsole::Mode::CreateNewBuffer(1u);
const Flags IBasicConsole::Mode::PerformCharacterInput(2u);
const Flags IBasicConsole::Mode::DisableControlBreak(4u);
const Flags IBasicConsole::Mode::EnablePaging(8u);*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
BasicConsole::BasicConsole(Flags openingMode, uint32 numberOfColumns, uint32 numberOfRows) {
    lastPagingCounter = 0;
    lineCount = 0u;
    SetSize(numberOfColumns, numberOfRows);
    Open(openingMode);
}

BasicConsole::~BasicConsole() {
    Close();
}

bool BasicConsole::PagedWrite(const void *buffer, uint32 &size, const TimeoutType &timeout) {

    uint32 numberOfColumnsTmp = 0;
    uint32 numberOfRowsTmp = 0;
    bool ok = false;

    ok = GetSize(numberOfColumnsTmp, numberOfRowsTmp);
    if (ok) {

        //-1 means the maximum size.
        uint32 numberOfRows = numberOfRowsTmp;

        int64 t0 = lastPagingCounter;
        int64 t1 = HighResolutionTimer::Counter();

        int64 dT = t1 - t0;
        float64 dt = dT * HighResolutionTimer::Period();
        if (dt > 0.05) {
            lineCount = 0;
            lastPagingCounter = t1;
        }

        char8 *p = (char8 *) buffer;
        uint32 index = 0;
        uint32 start = 0;
        uint32 sizeT;
        bool end = false;
        while (index < size) {
            while ((lineCount < (numberOfRows - 1)) && (index < size) && !end) {
                if (p[index] == '\n')
                    lineCount++;
                if (p[index] == '\0')
                    end = true;

                index++;
            }
            sizeT = index - start;
            BasicConsoleOS::Write(p + start, sizeT);
            if (end) {
                ok = true;
                break;
            }
            if (lineCount >= (numberOfRows - 1)) {
                start = index;
                lastPagingCounter = t1;
                lineCount = 0;
                const char8 *message = "[PAGING] ENTER TO CONTINUE\015";
                sizeT = StringHelper::Length(message);
                BasicConsoleOS::Write(message, sizeT);
                char8 buffer[32];
                sizeT = N_CHARS_NEWLINE;
                Read(buffer, sizeT, timeout);
            }
        }
        ok = true;
    }

    return ok;
}

bool BasicConsole::Write(const void* buffer, uint32& size, const TimeoutType &timeout) {
    uint32 numberOfRows = 0;
    uint32 numberOfColumns = 0;
    bool ok = GetSize(numberOfColumns, numberOfRows);
    if ((size > 0) && (buffer != NULL) && (numberOfColumns > 0) && (numberOfRows > 0)) {
        Flags mode = GetOpeningMode();
        if (mode & BasicConsolePaging) {
            ok = PagedWrite(buffer, size, timeout);
        }
        else {
            ok = BasicConsoleOS::Write(buffer, size);
        }
    }
    else {
        ok = false;
    }
    return ok;
}
