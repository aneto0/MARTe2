/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
#include "BasicConsole.h"
#include "HighResolutionTimer.h"
#include "StringHelper.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,BasicConsoleOS.h)

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
    return BasicConsoleOSOpen(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleClose(BasicConsole &con) {
    return BasicConsoleOSClose(con);
}

bool BasicConsoleShow(BasicConsole &con) {
    return BasicConsoleOSShow(con);
}

bool BasicConsoleWrite(BasicConsole &con, const void* buffer, uint32 &size,
                       TimeoutType msecTimeout) {

    int32 numberOfColumnsTmp;
    int32 numberOfRowsTmp;

	//The paging mechanism is portable. Just write a number of lines and then 
        //read a character to continue.
    if ((con.openingMode & EnablePaging) &&
        (BasicConsoleGetWindowSize(con,numberOfColumnsTmp,numberOfRowsTmp))){

	//-1 means the maximum size.
	uint32 numberOfRows=(uint32) numberOfRowsTmp;

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
            while ((con.lineCount < (numberOfRows - 1)) && (index < size)
                    && !end) {
                if (p[index] == '\n')
                    con.lineCount++;
                if (p[index] == '\0')
                    end = True;

                index++;
            }
            sizeT = index - start;
            BasicConsoleOSWrite(con, p + start, sizeT);
            if (end)
                return True;
            if (con.lineCount >= (numberOfRows - 1)) {
                start = index;
                con.lastPagingTime = t1;
                con.lineCount = 0;
                const char *message = "[PAGING] ENTER TO CONTINUE\015";
                sizeT = StringHelper::Length(message);
                BasicConsoleOSWrite(con, message, sizeT);
                char buffer[32];
		//The buffer should contain one char + /0 terminator.
                sizeT = 2;
                BasicConsoleRead(con, buffer, sizeT, msecTimeout);
            }
        }
        return True;
    }
    else {
        return BasicConsoleOSWrite(con, buffer, size);
    }
}

bool BasicConsoleRead(BasicConsole &con, void* buffer, uint32 &size,
                      TimeoutType msecTimeout) {
    return BasicConsoleOSRead(con, buffer, size, msecTimeout);
}

bool BasicConsoleSetTitleBar(BasicConsole &con, const char *title) {
    return BasicConsoleOSSetTitleBar(con, title);
}

bool BasicConsoleSetWindowSize(BasicConsole &con, int32 numberOfColumns,
                               int32 numberOfRows) {
    return BasicConsoleOSSetWindowSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleGetWindowSize(BasicConsole &con, int32 &numberOfColumns,
                               int32 &numberOfRows) {
    return BasicConsoleOSGetWindowSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleSetSize(BasicConsole &con, int32 numberOfColumns,
                         int32 numberOfRows) {
    return BasicConsoleOSSetSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleGetSize(BasicConsole &con, int32 &numberOfColumns,
                         int &numberOfRows) {
    return BasicConsoleOSGetSize(con, numberOfColumns, numberOfRows);
}

bool BasicConsoleSetCursorPosition(BasicConsole &con, int32 column, int32 row) {
    return BasicConsoleOSSetCursorPosition(con, column, row);
}

bool BasicConsoleGetCursorPosition(BasicConsole &con, int32 &column,
                                   int32 &row) {
    return BasicConsoleOSGetCursorPosition(con, column, row);
}

bool BasicConsoleSetColour(BasicConsole &con, Colours foreGroundColour,
                           Colours backGroundColour) {
    return BasicConsoleOSSetColour(con, foreGroundColour, backGroundColour);
}

bool BasicConsoleClear(BasicConsole &con) {
    return BasicConsoleOSClear(con);
}

bool BasicConsolePlotChar(BasicConsole &con, char c, Colours foreGroundColour,
                          Colours backGroundColour, int32 column, int32 row) {
    return BasicConsoleOSPlotChar(con, c, foreGroundColour, backGroundColour,
                                  column, row);
}

