/**
 * @file BasicConsoleOS.cpp
 * @brief Source file for class BasicConsoleOS
 * @date 20/06/2015
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
 * the class BasicConsoleOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <Windows.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicConsole.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct BasicConsoleOSProperties {

    /**
     * The input console handle
     */
    ConsoleHandle inputConsoleHandle;

    /**
     * The output console handle
     */
    ConsoleHandle outputConsoleHandle;

    /**
     * Initial settings of the console. The destructor will restores this initial configurations.
     */
    ConsoleInfo initialInfo;

    /**
     * Flag which describes the console status.
     */
    FlagsType openingMode;

    /**
     * The number of columns currently set.
     */
    uint32 nOfColumns;

    /**
     * The number of rows currently set.
     */
    uint32 nOfRows;

};

const FlagsType BasicConsole::Mode::Default(0u);
const FlagsType BasicConsole::Mode::CreateNewBuffer(1u);
const FlagsType BasicConsole::Mode::PerformCharacterInput(2u);
const FlagsType BasicConsole::Mode::DisableControlBreak(4u);
const FlagsType BasicConsole::Mode::EnablePaging(8u);

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole() {
    osProperties = new BasicConsoleOSProperties();
    osProperties->nOfColumns = 0u;
    osProperties->nOfRows = 0u;
    memset(&osProperties->inputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&osProperties->outputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&osProperties->initialInfo, 0, sizeof(ConsoleHandle));
    lastPagingCounter = 0;
    lineCount = 0u;
}

BasicConsole::~BasicConsole() {
    if (osProperties != static_cast<BasicConsoleOSProperties *>(NULL)) {
        /*lint -e{534} possible closure failure is not handled in the destructor.*/
        /*lint -e{1551} exception not caught.*/
        Close();
        delete osProperties;
    }
}

ErrorType BasicConsole::SetSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {

    ErrorType error = NoError;
    SHORT windowColumns = 0;
    SHORT windowRows = 0;

    //aligns the buffer and window sizes as much as possible to the specified parameters.

    //Note: the width and height specified cannot be less than the console screen buffer's ones.

    CONSOLE_SCREEN_BUFFER_INFO info;

    //get the console informations
    if (GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info) == 0) {
        error = OSError;

    }

    if (error == NoError) {

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
        if (!SetConsoleScreenBufferSize(osProperties->outputConsoleHandle, stage1BufferSize)) {
            // CStaticAssertPlatformErrorCondition(Errors::OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
            error = OSError;

        }

    }

    if (error == NoError) {
        windowColumns = info.srWindow.Right - info.srWindow.Left + 1;
        windowRows = info.srWindow.Bottom - info.srWindow.Top + 1;

        if (windowColumns < 0 || windowRows < 0) {
            error = OSError;

        }

    }

    if (error == NoError) {
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
        if (!SetConsoleWindowInfo(osProperties->outputConsoleHandle, TRUE, &srect)) {
            //  CStaticAssertPlatformErrorCondition(Errors::OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
            error = OSError;

        }

    }

    if (error == NoError) {
        //now if buffersize is greater than windows size they become aligned.
        COORD stage2BufferSize;
        stage2BufferSize.X = numberOfColumns;
        stage2BufferSize.Y = numberOfRows;

        if (!SetConsoleScreenBufferSize(osProperties->outputConsoleHandle, stage2BufferSize)) {
            error = OSError;

        }

    }

    return error;
}

ErrorType BasicConsole::Open(const FlagsType &mode) {

//    con.selectedStream      = NormalStreamMode;
    int32 shortMask = 0xffff;

    int stdConsoleColumns;
    int stdConsoleRows;

    ErrorType error = NoError;
    osProperties->openingMode = mode;

    if (osProperties->openingMode & BasicConsole::Mode::CreateNewBuffer) {
        //get the console handles
        osProperties->inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (osProperties->inputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = OSError;

        }
        osProperties->outputConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        if (osProperties->outputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = OSError;
        }
    }
    else {
        osProperties->inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (osProperties->inputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = OSError;
        }
        osProperties->outputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (osProperties->outputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = OSError;
        }
    }

    if (error == NoError) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &(osProperties->initialInfo)) == 0) {
            error = OSError;
        }
    }

    if (error == NoError) {
        stdConsoleColumns = osProperties->initialInfo.dwSize.X;

        stdConsoleRows = osProperties->initialInfo.dwSize.Y;

        osProperties->nOfColumns = stdConsoleColumns;

        osProperties->nOfRows = stdConsoleRows;

        //set the console mode
        DWORD consoleMode = 0;
        if (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_ECHO_INPUT;
            consoleMode |= ENABLE_LINE_INPUT;
        }

        if (osProperties->openingMode & BasicConsole::Mode::DisableControlBreak) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_PROCESSED_INPUT;
        }

        SetConsoleMode(osProperties->inputConsoleHandle, consoleMode);

        FlushConsoleInputBuffer(osProperties->inputConsoleHandle);

    }

    return error;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return osProperties->openingMode;
}

ErrorType BasicConsole::ShowBuffer() {
    ErrorType error = NoError;

    if (SetConsoleActiveScreenBuffer(osProperties->outputConsoleHandle) == FALSE) {
        error = OSError;
    }

    return error;

}

ErrorType BasicConsole::Write(const char8* buffer,
                              uint32 &size,
                              const TimeoutType &timeout) {

    ErrorType error = NoError;

    if (osProperties->openingMode & BasicConsole::Mode::EnablePaging) {
        error = PagedWrite(buffer, size, timeout);
    }
    else {
        error = OSWrite(buffer, size, timeout);
    }

    return error;

}

ErrorType BasicConsole::OSWrite(const char8* const buffer,
                                uint32 &size,
                                const TimeoutType &timeout) {
    ErrorType error = NoError;

    if ((size > 0) && (buffer != NULL)) {

        if (!WriteConsole(osProperties->outputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {

            error = OSError;
        }

    }
    else {
        error = Warning;
    }

    return error;

}

ErrorType BasicConsole::Read(char8* const buffer,
                             uint32 &size,
                             const TimeoutType &timeout) {
    ErrorType error = NoError;
    DWORD ret = 0;

    if (timeout.IsFinite()) {

        FlushConsoleInputBuffer(osProperties->inputConsoleHandle);
        ret = WaitForSingleObject(osProperties->inputConsoleHandle, (DWORD) timeout.GetTimeoutMSec());
        if (ret != 0) {
            size = 0;
            error = Timeout;
        }
    }

    if (error == NoError) {
        if (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) {
            size = 1;
            if (!ReadConsole(osProperties->inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {
                error = OSError;
            }
        }
        else {
            if (!ReadConsole(osProperties->inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {
                error = OSError;
            }

        }

    }
    return error;

}

ErrorType BasicConsole::SetTitleBar(const char8 *title) {
    ErrorType error = NoError;

    if (!SetConsoleTitle(title)) {
        error = OSError;
    }

    return error;
}

ErrorType BasicConsole::GetTitleBar(char8 *title,
                                    const uint32 &size) const {

    if (title == NULL) {
        return Warning;
    }

    GetConsoleTitle(title, size);

    return NoError;

}

ErrorType BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                      const uint32 &numberOfRows) {

    ErrorType error = NoError;

    COORD max = GetLargestConsoleWindowSize(osProperties->outputConsoleHandle);

    uint32 numberOfColumnsUsed = numberOfColumns;
    uint32 numberOfRowsUsed = numberOfRows;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info);

    if (info.dwSize.X < 0 || info.dwSize.Y < 0) {
        error = OSError;
    }

    if (error == NoError) {
        //saturate values at the max or at the buffer size
        if (numberOfColumnsUsed > max.X) {
            numberOfColumnsUsed = max.X;
        }
        if (numberOfColumnsUsed > info.dwSize.X) {
            numberOfColumnsUsed = info.dwSize.X;
        }
        if (numberOfRowsUsed > max.Y) {
            numberOfRowsUsed = max.Y;
        }
        if (numberOfRowsUsed > info.dwSize.Y) {
            numberOfRowsUsed = info.dwSize.Y;
        }
        if (numberOfColumnsUsed < 1) {
            numberOfColumnsUsed = 1;
        }
        if (numberOfRowsUsed < 1) {
            numberOfRowsUsed = 1;
        }

        SMALL_RECT srect;
        srect.Left = info.srWindow.Left;
        srect.Top = info.srWindow.Top;
        srect.Right = srect.Left + numberOfColumnsUsed - 1;
        srect.Bottom = srect.Top + numberOfRowsUsed - 1;

        if (!SetConsoleWindowInfo(osProperties->outputConsoleHandle, TRUE, &srect)) {
            //  CStaticPlatformErrorCondition(Errors::OSError,"BasicConsole:SetWindowSize:failed SetConsoleWindowInfo ");

            error = OSError;
        }
    }

    return error;
}

ErrorType BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                      uint32 &numberOfRows) const {

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info);

    numberOfColumns = info.srWindow.Right - info.srWindow.Left + 1;
    numberOfRows = info.srWindow.Bottom - info.srWindow.Top + 1;
    return NoError;
}

ErrorType BasicConsole::GetSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {

    ErrorType error = NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info) == FALSE) {
        //  CStaticAssertPlatformErrorCondition(Errors::OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
        error = OSError;
    }

    if (error == NoError) {
        numberOfColumns = info.dwSize.X;
        numberOfRows = info.dwSize.Y;
    }
    return error;

}

ErrorType BasicConsole::SetCursorPosition(const uint32 &column,
                                          const uint32 &row) {
    ErrorType error = NoError;
    COORD c = { column, row };
    if (SetConsoleCursorPosition(osProperties->outputConsoleHandle, c) == FALSE) {
        error = OSError;
    }

    return error;
}

ErrorType BasicConsole::GetCursorPosition(uint32 &column,
                                          uint32 &row) const {

    ErrorType error = NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info) == FALSE) {
        error = OSError;
    }

    if (error == NoError) {
        column = info.dwCursorPosition.X;
        row = info.dwCursorPosition.Y;
    }

    return error;
}

ErrorType BasicConsole::SetColour(const Colours &foregroundColour,
                                  const Colours &backgroundColour) {

    ErrorType error = NoError;

    WORD attribute;
    attribute = (int) foregroundColour & 0xF;
    attribute |= (((int) backgroundColour & 0xF) << 4);

    if (!SetConsoleTextAttribute(osProperties->outputConsoleHandle, attribute)) {
        error = OSError;
    }

    return error;

}

ErrorType BasicConsole::Clear() {

    ErrorType error = NoError;

    COORD c;
    c.X = 0;
    c.Y = 0;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(osProperties->outputConsoleHandle, &info);
    int nOfChars = info.dwSize.Y * info.dwSize.X;
    DWORD NumberOfCharsWritten;
    FillConsoleOutputAttribute(osProperties->outputConsoleHandle, info.wAttributes, nOfChars, c, &NumberOfCharsWritten);
    if (!FillConsoleOutputCharacter(osProperties->outputConsoleHandle, ' ', nOfChars, c, &NumberOfCharsWritten)) {
        error = OSError;
    }

    return error;

}

ErrorType BasicConsole::PlotChar(const char8 &c,
                                 const Colours &foregroundColour,
                                 const Colours &backgroundColour,
                                 const uint32 &column,
                                 const uint32 &row) {

    ErrorType error = NoError;
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

    if (WriteConsoleOutput(osProperties->outputConsoleHandle, &cInfo, bufferSize, coord, &writeRegion) == FALSE) {
        error = OSError;
    }

    return error;

}

ErrorType BasicConsole::Close() {

    ErrorType error = NoError;
    DWORD consoleMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
    if (osProperties->openingMode & BasicConsole::Mode::CreateNewBuffer) {
        CloseHandle(osProperties->outputConsoleHandle);
    }
    else {
        //reset the initial conditions of the console.
        int nCol = osProperties->initialInfo.dwSize.X;
        int nRow = osProperties->initialInfo.dwSize.Y;
        SetSize(nCol, nRow);
        SetConsoleWindowInfo(osProperties->outputConsoleHandle, TRUE, &(osProperties->initialInfo.srWindow));
        SetConsoleMode(osProperties->outputConsoleHandle, consoleMode);
        SetConsoleTextAttribute(osProperties->outputConsoleHandle, osProperties->initialInfo.wAttributes);
    }

    return error;
}

bool BasicConsole::CursorPositionSupported() const {
    return true;
}

bool BasicConsole::TitleBarSupported() const {
    return true;
}

bool BasicConsole::ColourSupported() const {
    return true;
}

bool BasicConsole::WindowSizeSupported() const {
    return true;
}

bool BasicConsole::ConsoleBufferSupported() const {
    return true;
}

bool BasicConsole::TimeoutSupported() const {
    return true;
}
