/**
 * @file BasicConsoleOS.h
 * @brief Header file for class BasicConsoleOS
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

 * @details This header file contains the declaration of the class BasicConsoleOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICCONSOLEOS_H_
#define BASICCONSOLEOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <Windows.h>
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
     * @brief Sets the buffer size of the console.
     * @details Since the buffer size must be always greater or equal to the window size, if the current
     * window size is greater than the desired buffer size it will be reduced.
     * @param[in,out] con is the console.
     * @param[in] numberOfColumns is the desired x axis size.
     * @param[in] numberOfRows is the desired y axis size.
     * return true if successful, false otherwise.
     */
    static bool SetSize(BasicConsole &con,
                        int numberOfColumns,
                        int numberOfRows) {

        //do nothing in case of negative parameters
        if (numberOfColumns < 0 || numberOfRows < 0) {
            return false;
        }

        //aligns the buffer and window sizes as much as possible to the specified parameters.

        //Note: the width and height specified cannot be less than the console screen buffer's ones.

        CONSOLE_SCREEN_BUFFER_INFO info;

        //get the console informations
        if (GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info) == 0) {
            return false;
        }

        COORD stage1BufferSize;
        stage1BufferSize.X = info.dwSize.X;

        //enlarge the coordinates if they are minor than the desired size
        //now i can only enlarge them!
        if (stage1BufferSize.X < numberOfColumns) {
            stage1BufferSize.X = numberOfColumns;
        }

        stage1BufferSize.Y = info.dwSize.Y;
        if (stage1BufferSize.Y < numberOfRows) {
            stage1BufferSize.Y = numberOfRows;
        }

        //set the buffer size
        if (!SetConsoleScreenBufferSize(con.outputConsoleHandle, stage1BufferSize)) {
            // CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
            return false;
        }

        SHORT windowColumns = info.srWindow.Right - info.srWindow.Left + 1;
        SHORT windowRows = info.srWindow.Bottom - info.srWindow.Top + 1;

        if (windowColumns < 0 || windowRows < 0) {
            return false;
        }

        if (windowColumns > numberOfColumns) {
            windowColumns = numberOfColumns;

        }
        if (windowRows > numberOfRows) {
            windowRows = numberOfRows;
        }

        SMALL_RECT srect;
        srect.Left = info.srWindow.Left;
        srect.Top = info.srWindow.Top;
        srect.Right = srect.Left + windowColumns - 1;
        srect.Bottom = srect.Top + windowRows - 1;

        //set the new windows size
        if (!SetConsoleWindowInfo(con.outputConsoleHandle, TRUE, &srect)) {
            //  CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
            return false;
        }

        //now if buffersize is greater than windows size they become aligned.
        COORD stage2BufferSize;
        stage2BufferSize.X = numberOfColumns;
        stage2BufferSize.Y = numberOfRows;

        return SetConsoleScreenBufferSize(con.outputConsoleHandle, stage2BufferSize) != FALSE;

    }

    /**
     * @brief Open a console with the specified properties.
     * @param[in,out] con is the console.
     * @param[in] numberOfColumns is the desired x axis size for the buffer.
     * @param[in] numberOfRows is the desired y axis size for the buffer.
     * return true if successful, false otherwise.
     */
    static bool Open(BasicConsole &con) {

//    con.selectedStream      = NormalStreamMode;
        int32 shortMask = 0xffff;

        int stdConsoleColumns;
        int stdConsoleRows;

        if (con.openingMode & CreateNewBuffer) {
            //get the console handles
            con.inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
            if (con.inputConsoleHandle == INVALID_HANDLE_VALUE) {
                return false;
            }
            con.outputConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
            if (con.outputConsoleHandle == INVALID_HANDLE_VALUE) {
                return false;
            }
        }
        else {
            con.inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
            if (con.inputConsoleHandle == INVALID_HANDLE_VALUE) {
                return false;
            }
            con.outputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (con.outputConsoleHandle == INVALID_HANDLE_VALUE) {
                return false;
            }
        }

        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(con.outputConsoleHandle, &(con.initialInfo)) == 0) {
            return false;
        }

        stdConsoleColumns = con.initialInfo.dwSize.X;

        stdConsoleRows = con.initialInfo.dwSize.Y;

        //if they are <= 0 set them as the default.
        if (numberOfColumns <= 0) {
            numberOfColumns = stdConsoleColumns;
        }
        if (numberOfRows <= 0) {
            numberOfRows = stdConsoleRows;
        }

        SetSize(con, numberOfColumns, numberOfRows);

        //set the console mode
        DWORD consoleMode = 0;
        if (con.openingMode & PerformCharacterInput) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_ECHO_INPUT;
            consoleMode |= ENABLE_LINE_INPUT;
        }

        if (con.openingMode & DisableControlBreak) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_PROCESSED_INPUT;
        }

        SetConsoleMode(con.inputConsoleHandle, consoleMode);

        FlushConsoleInputBuffer(con.inputConsoleHandle);

        return true;
    }

    /**
     * @brief Switch to a specified console.
     * @param[in,out] con is the console to show.
     * @return true if successful, false otherwise.
     */
    static bool Show(BasicConsole &con) {
        return (SetConsoleActiveScreenBuffer(con.outputConsoleHandle) == TRUE);
    }

    /**
     * @brief Writes the specified number of bytes on the console.
     * @param[in,out] con is the console.
     * @param[in] buffer contains the data to be printed on the console.
     * @param[in,out] size is the number of bytes to be printed.
     * @return true if successful, false otherwise.
     * @post size = number of bytes written.
     */
    static bool Write(BasicConsole &con,
                      const void* buffer,
                      uint32 &size) {
        bool ok = false;
        if((size > 0) && (buffer != NULL)){
            ok = WriteConsole(con.outputConsoleHandle, buffer, size, (unsigned long *) &size, NULL);
        }
        return ok;

    }

    /**
     * @brief Reads the specified number of bytes from the console.
     * @param[in,out] con is the console.
     * @param[out] buffer is the buffer which contains the read data in return.
     * @param[in,out] size is the number of bytes to read.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if successful, false otherwise.
     */
    static bool Read(BasicConsole &con,
                     void* buffer,
                     uint32 &size,
                     TimeoutType msecTimeout) {
        DWORD ret = 0;
        if (con.msecTimeout.IsFinite()) {

            FlushConsoleInputBuffer(con.inputConsoleHandle);
            ret = WaitForSingleObject(con.inputConsoleHandle, (DWORD) con.msecTimeout.msecTimeout);
            if (ret != 0) {
                size = 0;
                return false;
            }
        }

        if (con.openingMode & PerformCharacterInput) {
            size = 1;
            ret = ReadConsole(con.inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL);
        }
        else {
            ret = ReadConsole(con.inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL);

        }
        return ret;

    }

    /**
     * @brief Sets the title of the console.
     * @param[in,out] con is the console.
     * @param[in] title is the desired title.
     * @return true if successful, false otherwise.
     */
    static bool SetTitleBar(BasicConsole &con,
                            const char8 *title) {
        return SetConsoleTitle(title);

    }

    /**
     * @brief Sets the window size.
     * @details Since the window size must be smaller than the buffer size, if the desired
     * size is greater than the current buffer size it will be saturated.
     * @param[in,out] con is the console.
     * @param[in] numberOfColumns is the desired x axis size.
     * @param[in] numberOfRows is the desired y axis size.
     * @return true if successful, false otherwise.
     */
    static bool SetWindowSize(BasicConsole &con,
                              int numberOfColumns,
                              int numberOfRows) {

        if (numberOfColumns < 0 || numberOfRows < 0) {
            return false;
        }

        COORD max = GetLargestConsoleWindowSize(con.outputConsoleHandle);
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info);

        if (info.dwSize.X < 0 || info.dwSize.Y < 0) {
            return false;
        }

        //saturate values at the max or at the buffer size
        if (numberOfColumns > max.X) {
            numberOfColumns = max.X;
        }
        if (numberOfColumns > info.dwSize.X) {
            numberOfColumns = info.dwSize.X;
        }
        if (numberOfRows > max.Y) {
            numberOfRows = max.Y;
        }
        if (numberOfRows > info.dwSize.Y) {
            numberOfRows = info.dwSize.Y;
        }
        if (numberOfColumns < 1) {
            numberOfColumns = 1;
        }
        if (numberOfRows < 1) {
            numberOfRows = 1;
        }

        SMALL_RECT srect;
        srect.Left = info.srWindow.Left;
        srect.Top = info.srWindow.Top;
        srect.Right = srect.Left + numberOfColumns - 1;
        srect.Bottom = srect.Top + numberOfRows - 1;

        if (!SetConsoleWindowInfo(con.outputConsoleHandle, TRUE, &srect)) {
            //  CStaticPlatformErrorCondition(OSError,"BasicConsole:SetWindowSize:failed SetConsoleWindowInfo ");
            return false;
        }

        return true;
    }

    /**
     * @brief Gets the window size.
     * @param[in,out] con is the console.
     * @param[out] numberOfColumns is the x axis window size in return.
     * @param[out] numberOfRows is the y axis window size in return.
     * @return true if successful, false otherwise.
     */
    static bool GetWindowSize(BasicConsole &con,
                              int &numberOfColumns,
                              int &numberOfRows) {

        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info);

        numberOfColumns = info.srWindow.Right - info.srWindow.Left + 1;
        numberOfRows = info.srWindow.Bottom - info.srWindow.Top + 1;
        return true;

    }

    /**
     * @brief Gets the buffer size.
     * @param[in,out] con is the console.
     * @param[out] numberOfColumns is the x axis buffer size in return.
     * @param[out] numberOfRows is the y axis buffer size in return.
     * @return true if successful, false otherwise.
     */
    static bool GetSize(BasicConsole &con,
                        int &numberOfColumns,
                        int &numberOfRows) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info) == FALSE) {
            //  CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
            return false;
        }
        numberOfColumns = info.dwSize.X;
        numberOfRows = info.dwSize.Y;
        return true;

    }

    /**
     * @brief Sets the cursor position.
     * @param[in,out] con is the console.
     * @param[in] column is the x axis position.
     * @param[in] row is the y axis position.
     * @return true if successful, false otherwise.
     */
    static bool SetCursorPosition(BasicConsole &con,
                                  int column,
                                  int row) {
        COORD c = { column, row };
        return (SetConsoleCursorPosition(con.outputConsoleHandle, c) == TRUE);
    }


    /**
     * @brief Gets the cursor position.
     * @param[in,out] con is the console.
     * @param[out] column is the cursor x position.
     * @param[out] row is the cursor y position.
     * @return true if successful, false otherwise.
     */
    static bool GetCursorPosition(BasicConsole &con,
                                  int &column,
                                  int &row) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info) == FALSE) {
            return false;
        }
        column = info.dwCursorPosition.X;
        row = info.dwCursorPosition.Y;
        return true;
    }

    /**
     * @brief Sets the foreground and background colors.
     * @param[in,out] con is the console.
     * @param[in] foregroundColour is the desired foreground color.
     * @param[in] backgroundColour is the desired background color.
     * @return true if successful, false otherwise.
     */
    static bool SetColour(BasicConsole &con,
                          Colours foregroundColour,
                          Colours backgroundColour) {

        WORD attribute;
        attribute = (int) foregroundColour & 0xF;
        attribute |= ((int) backgroundColour & 0xF) << 4;

        if (!SetConsoleTextAttribute(con.outputConsoleHandle, attribute))
            return false;

        return true;

    }

    /**
     * @brief Cleans the console screen.
     * @param[in,out] con is the console.
     * @return true if successful, false otherwise.
     */
    static bool Clear(BasicConsole &con) {
        COORD c;
        c.X = 0;
        c.Y = 0;

        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo(con.outputConsoleHandle, &info);
        int nOfChars = info.dwSize.Y * info.dwSize.X;

        FillConsoleOutputAttribute(con.outputConsoleHandle, info.wAttributes, nOfChars, c, NULL);
        return (FillConsoleOutputCharacter(con.outputConsoleHandle, ' ', nOfChars, c, NULL));

    }

    /**
     * @brief Prints a character with specified colors and position.
     * @param[in,out] con is the console.
     * @param[in] c is the character to be printed.
     * @param[in] foregroundColour is the desired foreground color.
     * @param[in] backgroundColour is the desired background color.
     * @param[in] column is the x axis desired position.
     * @param[in] row is the y axis desired position.
     * @return true if successful, false otherwise.
     */
    static bool PlotChar(BasicConsole &con,
                         char8 c,
                         Colours foregroundColour,
                         Colours backgroundColour,
                         int column,
                         int row) {

        COORD coord;
        coord.X = 0;
        coord.Y = 0;

        COORD bufferSize;
        bufferSize.X = 1;
        bufferSize.Y = 1;

        WORD attribute;
        attribute = (int) foregroundColour & 0xF;
        attribute |= ((int) backgroundColour & 0xF) << 4;

        CHAR_INFO cInfo;
        cInfo.Char.AsciiChar = c;
        cInfo.Attributes = attribute;

        SMALL_RECT writeRegion;
        writeRegion.Left = row;
        writeRegion.Right = row;
        writeRegion.Top = column;
        writeRegion.Bottom = column;

        return (WriteConsoleOutput(con.outputConsoleHandle, &cInfo, bufferSize, coord, &writeRegion) != FALSE);

    }

    /**
     * @brief Closes the console resetting the previous properties.
     * @param[in,out] con is the console.
     * @return true if successful, false otherwise.
     */
    static bool Close(BasicConsole &con) {

        DWORD consoleMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
        if (con.openingMode & CreateNewBuffer) {
            CloseHandle(con.outputConsoleHandle);
        }
        else {
            //reset the initial conditions of the console.
            int nCol = con.initialInfo.dwSize.X;
            int nRow = con.initialInfo.dwSize.Y;
            SetSize(con, nCol, nRow);
            SetConsoleWindowInfo(con.outputConsoleHandle, TRUE, &(con.initialInfo.srWindow));
            SetConsoleMode(con.outputConsoleHandle, consoleMode);
            SetConsoleTextAttribute(con.outputConsoleHandle, con.initialInfo.wAttributes);
        }

        return true;
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICCONSOLEOS_H_ */

