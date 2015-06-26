/**
 * @file BasicConsoleOS.h
 * @brief Header file for class BasicConsoleOS
 * @date 22/06/2015
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

 * @details This header file contains the declaration of the class BasicConsoleOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICCONSOLEOS_H_
#define 		BASICCONSOLEOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief System dependent functions for console management.
 */
class BasicConsoleOS {

public:
    /**
     * @see BasicConsoleOpen
     * @brief Open a console.
     * @param[in,out] con is the console.
     * @param[in] numberOfColumns is the desired number of columns.
     * @param[in] numberOfRows is the desired number of rows.
     * @return true if the console is opened correctly.
     */
    static bool Open(BasicConsole &con,
                     int32 numberOfColumns,
                     int32 numberOfRows) {

        //In this case read immediately from the console without wait \n.
        if (con.openingMode & PerformCharacterInput) {
            if (ioctl(fileno(stdin), TCGETA, &(con.outputConsoleHandle)) < 0) {
                return False;
            }

            con.initialInfo = con.outputConsoleHandle;
            SetImmediateRead(con);

            ioctl(fileno(stdin), TCSETAW, &(con.outputConsoleHandle));
        }
        fflush (stdin);
        return True;
    }

    /**
     * @see BasicConsoleClose
     * @brief Close a console.
     * @param[in,out] con is the console to close.
     * @return true if the console is closed correctly.
     */
    static bool Close(BasicConsole &con) {
        if (con.openingMode & PerformCharacterInput) {
            //reset the original console modes
            // struct termio modifiedConsoleModes;
            ioctl(fileno(stdin), TCGETA, &(con.outputConsoleHandle));
            UnSetImmediateRead(con);
            ioctl(fileno(stdin), TCSETAW, &(con.outputConsoleHandle));
        }
        return True;
    }

    /**
     * @see BasicConsole::Show
     * @warning Not Implemented.
     * @brief Switch to a specified console.
     * @param[in,out] con is the console.
     * @return true.
     */
    static bool Show(BasicConsole &con) {
        return True;
    }

    /**
     * @see BasicConsole::Write
     * @brief Write on the console.
     * @param[in,out] con is the console.
     * @param[in] buffer is a pointer to the memory to write.
     * @param[in,out] size is the number of bytes to write on the console.
     * @return true if at least one byte is written.
     */
    static bool Write(BasicConsole &con,
                      const void* buffer,
                      uint32 &size) {
        char* buffString = (char*) buffer;
        char nextRow = '\n';
        int32 n = 0;
        uint32 index = 0, start = 0;
        uint32 sizeT = 0;
        uint32 columnLimit = (uint32) con.numberOfColumns;
        while (1) {
            while ((con.colCount + sizeT) < columnLimit && index < size) {
                if ((buffString + index) == NULL || buffString[index] == '\n') {
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
            if ((buffString + index) == NULL) {
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
     * @param[in,out] con is the console.
     * @param[in] buffer is the location of memory where store read bytes.
     * @param[in,out] size is the number of bytes to read.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if at least one byte is read.
     */
    static bool Read(BasicConsole &con,
                     void* buffer,
                     uint32 &size,
                     TimeoutType msecTimeout) {
        if (con.openingMode & PerformCharacterInput) {
            ((char *) buffer)[0] = (char) getchar();
            size = 1;
        }
        else {
            size = read(STDIN, (char*) buffer, size);
            /*
             char *temp = NULL;
             while ((temp = fgets((char *) buffer, n, stdin)) == NULL)
             ;
             if ((n = strlen(temp)) > 0) {
             size = n;
             }
             temp[n] = '\0';*/
        }
        return (size > 0);
    }

    /**
     * @warning Not implemented.
     * @brief Sets the title of the console.
     * @param[in,out] con is the console.
     * @param[in] title is the title.
     * @return false.
     */
    static bool SetTitleBar(BasicConsole &con,
                            const char *title) {
        return False;
    }

    /**
     * @see BasicConsole::SetWindowSize.
     * @brief Set the number of rows and columns.
     * @param[in,out] con is the console.
     * @param[in] numberOfColumns is the desired number of columns.
     * @param[in] numberOfRows is the desired number of rows.
     * @return true.
     */
    static bool SetWindowSize(BasicConsole &con,
                              int numberOfColumns,
                              int numberOfRows) {
        con.numberOfColumns = numberOfColumns;
        con.numberOfRows = numberOfRows;
        return True;
    }

    /**
     * @see BasicConsole::GetWindowSize.
     * @brief Get the number of columns and rows.
     * @param[in,out] con is the console.
     * @param[out] numberOfColumns is the numberOfColumns in return.
     * @param[out] numberOfRows is the numberOfRows in return.
     * @return true.
     */
    static bool GetWindowSize(BasicConsole &con,
                              int &numberOfColumns,
                              int &numberOfRows) {
        numberOfColumns = con.numberOfColumns;
        numberOfRows = con.numberOfRows;
        return True;
    }

    /**
     * @see BasicConsoleOSSetWindowSize
     * @see BasicConsole::SetWindowSize
     */
    static bool SetSize(BasicConsole &con,
                        int numberOfColumns,
                        int numberOfRows) {
        con.numberOfColumns = numberOfColumns;
        con.numberOfRows = numberOfRows;
        return True;
    }

    /**
     * @see BasicConsoleOSGetWindowSize
     * @see BasicConsole::GetWindowSize
     */
    static bool GetSize(BasicConsole &con,
                        int &numberOfColumns,
                        int &numberOfRows) {
        numberOfColumns = con.numberOfColumns;
        numberOfRows = con.numberOfRows;
        return True;
    }

    /**
     * @warning Not implemented.
     * @brief Sets the cursor position.
     * @param[in,out] con is the console.
     * @param[in] column is the desired x position.
     * @param[in] row is the desired y position.
     * @return false.
     */
    static bool SetCursorPosition(BasicConsole &con,
                                  int column,
                                  int row) {
        return False;
    }

    /**
     * @warning Not implemented.
     * @brief Gets the cursor position.
     * @param[in,out] con is the console.
     * @param[out] column is the cursor x position.
     * @param[out] row is the cursor y position.
     * @return false.
     */
    static bool GetCursorPosition(BasicConsole &con,
                                  int &column,
                                  int &row) {
        return False;
    }

    /**
     * @warning Not implemented.
     * @brief Sets the foreground and background colors.
     * @param[in,out] con is the console.
     * @param[in] foreGroundColour is the desired foreground color.
     * @param[in] backGroundColour is the desired background color.
     * @return false.
     */
    static bool SetColour(BasicConsole &con,
                          Colours foreGroundColour,
                          Colours backGroundColour) {
        return False;
    }

    /**
     * @see BasicConsole::Clear
     * @brief Clean the console.
     * @param[in,out] con is the console.
     * @return true.
     */
    static bool Clear(BasicConsole &con) {
        for (uint32 i = 0; i < BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
            write(STDOUT, "\n", 1);
        }
        return True;
    }

    /**
     * @warning Not implemented.
     * @brief Prints a character with specified colors and position.
     * @param[in,out] con is the console.
     * @param[in] c is the char to write.
     * @param[in] foreGroundColour is the desired foreground color.
     * @param[in] backGroundColour is the desired background color.
     * @param[in] column is the desired x position.
     * @param[in] row is the desired y position.
     * @return false.
     */
    static bool PlotChar(BasicConsole &con,
                         char c,
                         Colours foreGroundColour,
                         Colours backGroundColour,
                         int column,
                         int row) {
        return False;

    }
private:

    /**
     * Standard output file descriptor.
     */
    static const uint8 STDOUT = 1;
    /**
     * Standard input file descriptor.
     */
    static const uint8 STDIN = 0;
    /**
     * Number of rows that will be cleared when BasicConsoleOSClear is called
     */
    static const uint32 BASIC_CONSOLE_LINUX_CLEAR_ROWS = 40;

    /**
     * @brief Read immediately.
     * @details Sets equal to 1 the characters in the read buffer and equal to zero the time to wait between read operations.\n
     * It does not wait the newline char to read.
     * @param[in,out] con is the console.
     */
    static void SetImmediateRead(BasicConsole &con) {
        struct termio &consoleMode = con.outputConsoleHandle;

        //use the input handle to save default parameters
        struct termio &saveMode = con.inputConsoleHandle;
        consoleMode.c_lflag &= ~ICANON;
        saveMode.c_cc[VMIN] = consoleMode.c_cc[VMIN];
        consoleMode.c_cc[VMIN] = 1;
        saveMode.c_cc[VTIME] = consoleMode.c_cc[VTIME];
        consoleMode.c_cc[VTIME] = 0;
    }

    /**
     * @brief Restore default settings.
     * @param[in,out] con is the console.
     */
    static void UnSetImmediateRead(BasicConsole &con) {

        struct termio &consoleMode = con.outputConsoleHandle;
        struct termio &saveMode = con.inputConsoleHandle;
        consoleMode.c_lflag |= ICANON;
        consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
        consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICCONSOLEOS_H_ */

