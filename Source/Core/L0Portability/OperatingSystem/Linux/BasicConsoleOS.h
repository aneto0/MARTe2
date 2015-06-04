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
/**
 * @file
 * @brief Basic console implementation in linux
 */

#ifndef BASIC_CONSOLE_OS_H
#define BASIC_CONSOLE_OS_H

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "../../GeneralDefinitions.h"

#define STDOUT 1
#define STDIN  0
/**
 * Number of rows that will be cleared when BasicConsoleOSClear is called
 */
#define BASIC_CONSOLE_LINUX_CLEAR_ROWS 40

void SetImmediateRead(BasicConsole &con, struct termio &consoleMode){
	consoleMode.c_lflag &=~ ICANON;
	con.consoleDefaultParam.minReadByte=consoleMode.c_cc[VMIN];
	consoleMode.c_cc[VMIN] = 1;
	con.consoleDefaultParam.minTimeBetweenInputs=consoleMode.c_cc[VTIME];
	consoleMode.c_cc[VTIME] = 0;
}
		
void UnSetImmediateRead(BasicConsole &con, struct termio &consoleMode){
	consoleMode.c_lflag |= ICANON;
	consoleMode.c_cc[VMIN] = con.consoleDefaultParam.minReadByte;
	consoleMode.c_cc[VTIME] = con.consoleDefaultParam.minTimeBetweenInputs;
}


/**
 * @see BasicConsoleOpen
 * @brief Open a console.
 * @param con is the console.
 * @param numberOfColumns is the desired number of columns.
 * @param numberOfRows is the desired number of rows.
 * @return true if the console is opened correctly.
 */
bool BasicConsoleOSOpen(BasicConsole &con, int32 numberOfColumns,
                        int32 numberOfRows) {

    //In this case read immediately from the console without wait \n.
    if (con.openingMode & PerformCharacterInput) {
        struct termio modifyConsoleModes;
        if (ioctl(fileno(stdin), TCGETA, &modifyConsoleModes) < 0) {
            return False;
        }

	SetImmediateRead(con, modifyConsoleModes);

/*
        modifyConsoleModes = originalConsoleModes;
        //don't wait the ret line to read
        modifyConsoleModes.c_lflag &= ~ICANON;
        //read immediately
        modifyConsoleModes.c_cc[VMIN] = 1;
        modifyConsoleModes.c_cc[VTIME] = 0;*/
        ioctl(fileno(stdin), TCSETAW, &modifyConsoleModes);
    }
    fflush (stdin);
    return True;
}

/**
 * @see BasicConsoleClose
 * @brief Close a console.
 * @param con is the console to close.
 * @return true if the console is closed correctly.
 */
bool BasicConsoleOSClose(BasicConsole &con) {
    if (con.openingMode & PerformCharacterInput) {
        //reset the original console modes
	struct termio modifiedConsoleModes;
	ioctl(fileno(stdin), TCGETA, &modifiedConsoleModes);
	UnSetImmediateRead(con, modifiedConsoleModes);
        ioctl(fileno(stdin), TCSETAW, &modifiedConsoleModes);
    }
    return True;
}

/**
 * @see BasicConsole::Show
 * @brief Not Implemented.
 * @param con is the console.
 * @return true.
 */
bool BasicConsoleOSShow(BasicConsole &con) {
    return True;
}

/**
 * @see BasicConsole::Write
 * @brief Write on the console.
 * @param con is the console.
 * @param buffer is a pointer to the memory to write.
 * @param size is the number of bytes to write on the console.
 * @return true if at least one byte is written.
 */
bool BasicConsoleOSWrite(BasicConsole &con, const void* buffer, uint32 &size) {
    char* buffString = (char*) buffer;
    char nextRow = '\n';
    int32 n = 0;
    uint32 index = 0, start = 0;
    uint32 sizeT = 0;
    uint32 columnLimit = (uint32) con.numberOfColumns;
    while (1) {
        while ((con.colCount + sizeT) < columnLimit && index < size) {
            if (buffString[index] == '\0' || buffString[index] == '\n') {
                break;
	    }
            index++;
            sizeT = index - start;
        }

        if (sizeT > 0)
            n += write(STDOUT, buffString + start, sizeT);

        if (index >= size) {
	    con.colCount += sizeT;
            break;
	}
        if (buffString[index] == '\0') {
	    con.colCount = sizeT;
            break;
        }

        write(STDOUT, &nextRow, 1);
        if (buffString[index] == '\n') {
            index++;
            n++;
        }
        start = index;
        sizeT = 0;
	con.colCount = 0;
    }

    size = n;
    return n > 0;

}

/**
 * @see BasicConsole::Read.
 * @brief Read from the console.
 * @param con is the console.
 * @param buffer is the location of memory where store read bytes.
 * @param size is the number of bytes to read.
 * @return true if at least one byte is read.
 */
bool BasicConsoleOSRead(BasicConsole &con, void* buffer, uint32 &size,
                        TimeoutType msecTimeout) {
    uint32 n = size;
    if (con.openingMode & PerformCharacterInput) {
        ((char *) buffer)[0] = (char) getchar();
        size = 1;
    }
    else {
        char *temp = NULL;
        while ((temp = fgets((char *) buffer, n, stdin)) == NULL)
            ;
        if ((n = strlen(temp)) > 0) {
            size = n;
        }
        temp[n] = '\0';
    }
    return (n > 0);
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param title is the title.
 * @return false.
 */
bool BasicConsoleOSSetTitleBar(BasicConsole &con, const char *title) {
    return False;
}

/**
 * @see BasicConsole::SetWindowSize.
 * @brief Set the number of rows and columns.
 * @param con is the console.
 * @param numberOfColumns is the desired number of columns.
 * @param numberOfRows is the desired number of rows.
 * @return true.
 */
bool BasicConsoleOSSetWindowSize(BasicConsole &con, int numberOfColumns,
                                 int numberOfRows) {
    con.numberOfColumns = numberOfColumns;
    con.numberOfRows = numberOfRows;
    return True;
}

/**
 * @see BasicConsole::GetWindowSize.
 * @brief Get the number of columns and rows.
 * @param con is the console.
 * @param numberOfColumns is the numberOfColumns in return.
 * @param numberOfRows is the numberOfRows in return.
 * @return true.
 */
bool BasicConsoleOSGetWindowSize(BasicConsole &con, int &numberOfColumns,
                                 int &numberOfRows) {
    numberOfColumns = con.numberOfColumns;
    numberOfRows = con.numberOfRows;
    return True;
}

/**
 * @see BasicConsoleOSSetWindowSize BasicConsole::SetWindowSize
 */
bool BasicConsoleOSSetSize(BasicConsole &con, int numberOfColumns,
                           int numberOfRows) {
    con.numberOfColumns = numberOfColumns;
    con.numberOfRows = numberOfRows;
    return True;
}

/**
 * @see BasicConsoleOSGetWindowSize BasicConsole::GetWindowSize
 */
bool BasicConsoleOSGetSize(BasicConsole &con, int &numberOfColumns,
                           int &numberOfRows) {
    numberOfColumns = con.numberOfColumns;
    numberOfRows = con.numberOfRows;
    return True;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param column is the desired x position.
 * @param row is the desired y position.
 * @return false.
 */
bool BasicConsoleOSSetCursorPosition(BasicConsole &con, int column, int row) {
    return False;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param column is the cursor x position.
 * @param row is the cursor y position.
 * @return false.
 */
bool BasicConsoleOSGetCursorPosition(BasicConsole &con, int &column, int &row) {
    return False;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param foreGroundColour is the desired foreground color.
 * @param backGroundColour is the desired background color.
 * @return false.
 */
bool BasicConsoleOSSetColour(BasicConsole &con, Colours foreGroundColour,
                             Colours backGroundColour) {
    return False;
}

/**
 * @see BasicConsole::Clear
 * @brief Clean the console.
 * @param con is the console.
 * @return true.
 */
bool BasicConsoleOSClear(BasicConsole &con) {
    for (int32 i = 0; i < BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        write(STDOUT, "\n", 1);
    }
    return True;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param c is the char to write.
 * @param foreGroundColour is the desired foreground color.
 * @param backGroundColour is the desired background color.
 * @param column is the desired x position.
 * @param row is the desired y position.
 * @return false.
 */
bool BasicConsoleOSPlotChar(BasicConsole &con, char c, Colours foreGroundColour,
                            Colours backGroundColour, int column, int row) {
    return False;

}

#endif
