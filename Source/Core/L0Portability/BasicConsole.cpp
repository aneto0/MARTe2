/**
 * @file BasicConsole.cpp
 * @brief Source file for class BasicConsole
 * @date 22/06/2015
 * @author Giuseppe Ferr�
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
#include "HighResolutionTimer.h"
#include "StringHelper.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,BasicConsoleOS.h)
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool BasicConsoleOpen(BasicConsole &con, ConsoleOpeningMode openingMode,
        int32 numberOfColumns, int32 numberOfRows,
        TimeoutType msecTimeout)
{

    con.msecTimeout = msecTimeout;
    con.lineCount = 0;
    con.colCount = 0;
    con.lastPagingTime = 0;
    con.openingMode = openingMode;
    con.numberOfColumns=numberOfColumns;
    con.numberOfRows=numberOfRows;

    return BasicConsoleOS::Open(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleClose(BasicConsole &con) {
    return BasicConsoleOS::Close(con);
}

bool BasicConsoleShow(BasicConsole &con) {
    return BasicConsoleOS::Show(con);
}

bool BasicConsoleWrite(BasicConsole &con,
                       const void* buffer,
                       uint32 &size,
                       TimeoutType msecTimeout) {

    int32 numberOfColumnsTmp;
    int32 numberOfRowsTmp;

    //The paging mechanism is portable. Just write a number of lines and then
    //read a character to continue.
    if ((con.openingMode & EnablePaging) && (BasicConsoleGetWindowSize(con, numberOfColumnsTmp, numberOfRowsTmp))) {

        //-1 means the maximum size.
        uint32 numberOfRows = (uint32) numberOfRowsTmp;

        int64 t0 = con.lastPagingTime;
        int64 t1 = HighResolutionTimer::Counter();

        int64 dT = t1 - t0;
        double dt = dT * HighResolutionTimer::Period();
        if (dt > 0.05) {
            con.lineCount = 0;
            con.lastPagingTime = t1;
        }

        char *p = (char *) buffer;
        uint32 index = 0;
        int start = 0;
        uint32 sizeT;
        bool end = False;
        while (index < size) {
            while ((con.lineCount < (numberOfRows - 1)) && (index < size) && !end) {
                if (p[index] == '\n')
                    con.lineCount++;
                if (p[index] == '\0')
                    end = True;

                index++;
            }
            sizeT = index - start;
            BasicConsoleOS::Write(con, p + start, sizeT);
            if (end)
                return True;
            if (con.lineCount >= (numberOfRows - 1)) {
                start = index;
                con.lastPagingTime = t1;
                con.lineCount = 0;
                const char *message = "[PAGING] ENTER TO CONTINUE\015";
                sizeT = StringHelper::Length(message);
                BasicConsoleOS::Write(con, message, sizeT);
                char buffer[32];
                sizeT = N_CHARS_NEWLINE;
                BasicConsoleRead(con, buffer, sizeT, msecTimeout);
            }
        }
        return True;
    }
    else {
        return BasicConsoleOS::Write(con, buffer, size);
    }
}

bool BasicConsoleRead(BasicConsole &con,
                      void* buffer,
                      uint32 &size,
                      TimeoutType msecTimeout) {
    return BasicConsoleOS::Read(con, buffer, size, msecTimeout);
}

bool BasicConsoleSetTitleBar(BasicConsole &con,
                             const char *title) {
    return BasicConsoleOS::SetTitleBar(con, title);
}

bool BasicConsoleSetWindowSize(BasicConsole &con,
                               int32 numberOfColumns,
                               int32 numberOfRows) {
    return BasicConsoleOS::SetWindowSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleGetWindowSize(BasicConsole &con,
                               int32 &numberOfColumns,
                               int32 &numberOfRows) {
    return BasicConsoleOS::GetWindowSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleSetSize(BasicConsole &con,
                         int32 numberOfColumns,
                         int32 numberOfRows) {
    return BasicConsoleOS::SetSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleGetSize(BasicConsole &con,
                         int32 &numberOfColumns,
                         int32 &numberOfRows) {
    return BasicConsoleOS::GetSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleSetCursorPosition(BasicConsole &con,
                                   int32 column,
                                   int32 row) {
    return BasicConsoleOS::SetCursorPosition(con, column, row);
}

bool BasicConsoleGetCursorPosition(BasicConsole &con,
                                   int32 &column,
                                   int32 &row) {
    return BasicConsoleOS::GetCursorPosition(con, column, row);
}

bool BasicConsoleSetColour(BasicConsole &con,
                           Colours foreGroundColour,
                           Colours backGroundColour) {
    return BasicConsoleOS::SetColour(con, foreGroundColour, backGroundColour);
}

bool BasicConsoleClear(BasicConsole &con) {
    return BasicConsoleOS::Clear(con);
}

bool BasicConsolePlotChar(BasicConsole &con,
                          char c,
                          Colours foreGroundColour,
                          Colours backGroundColour,
                          int32 column,
                          int32 row) {
    return BasicConsoleOS::PlotChar(con, c, foreGroundColour, backGroundColour, column, row);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole(ConsoleOpeningMode openingMode,
                           int32 numberOfColumns,
                           int32 numberOfRows,
                           TimeoutType msecTimeout) {
    BasicConsoleOpen(*this, openingMode, numberOfColumns, numberOfRows, msecTimeout);
}


BasicConsole::~BasicConsole() {
    BasicConsoleClose(*this);
}
