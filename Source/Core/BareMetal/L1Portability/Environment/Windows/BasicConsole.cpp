/**
 * @file BasicConsole.cpp
 * @brief Source file for class BasicConsole
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
 * the class BasicConsole (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

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

namespace MARTe{

struct BasicConsoleProperties {

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


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole() {
    handle = new BasicConsoleProperties();
    handle->nOfColumns = 0u;
    handle->nOfRows = 0u;
    memset(&handle->inputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&handle->outputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&handle->initialInfo, 0, sizeof(ConsoleHandle));
    lastPagingCounter = 0;
    lineCount = 0u;
}

BasicConsole::~BasicConsole() {
    if (handle != static_cast<BasicConsoleProperties *>(NULL)) {
        /*lint -e{534} possible closure failure is not handled in the destructor.*/
        /*lint -e{1551} exception not caught.*/
        Close();
        delete handle;
    }
}

ErrorManagement::ErrorType BasicConsole::SetSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    SHORT windowColumns = 0;
    SHORT windowRows = 0;

    //aligns the buffer and window sizes as much as possible to the specified parameters.

    //Note: the width and height specified cannot be less than the console screen buffer's ones.

    CONSOLE_SCREEN_BUFFER_INFO info;

    //get the console informations
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == 0) {
        error = ErrorManagement::OSError;

    }

    if (error == ErrorManagement::NoError) {

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
        if (!SetConsoleScreenBufferSize(handle->outputConsoleHandle, stage1BufferSize)) {
            // CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
            error = ErrorManagement::OSError;

        }

    }

    if (error == ErrorManagement::NoError) {
        windowColumns = info.srWindow.Right - info.srWindow.Left + 1;
        windowRows = info.srWindow.Bottom - info.srWindow.Top + 1;

        if (windowColumns < 0 || windowRows < 0) {
            error = ErrorManagement::OSError;

        }

    }

    if (error == ErrorManagement::NoError) {
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
        if (!SetConsoleWindowInfo(handle->outputConsoleHandle, TRUE, &srect)) {
            //  CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
            error = ErrorManagement::OSError;

        }

    }

    if (error == ErrorManagement::NoError) {
        //now if buffersize is greater than windows size they become aligned.
        COORD stage2BufferSize;
        stage2BufferSize.X = numberOfColumns;
        stage2BufferSize.Y = numberOfRows;

        if (!SetConsoleScreenBufferSize(handle->outputConsoleHandle, stage2BufferSize)) {
            error = ErrorManagement::OSError;

        }

    }

    return error;
}

ErrorManagement::ErrorType BasicConsole::Open(const FlagsType &mode) {

//    con.selectedStream      = NormalStreamMode;
    int32 shortMask = 0xffff;



    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    handle->openingMode = mode;

    if (handle->openingMode & BasicConsoleMode::CreateNewBuffer) {
        //get the console handles
        handle->inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (handle->inputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = ErrorManagement::OSError;

        }
        handle->outputConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
        if (handle->outputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = ErrorManagement::OSError;
        }
    }
    else {
        handle->inputConsoleHandle = GetStdHandle(STD_INPUT_HANDLE);
        if (handle->inputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = ErrorManagement::OSError;
        }
        handle->outputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (handle->outputConsoleHandle == INVALID_HANDLE_VALUE) {
            error = ErrorManagement::OSError;
        }
    }

    if (error == ErrorManagement::NoError) {
        CONSOLE_SCREEN_BUFFER_INFO info;
        if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &(handle->initialInfo)) == 0) {
            error = ErrorManagement::OSError;
        }
    }

    if (error == ErrorManagement::NoError) {
        int stdConsoleColumns;
        int stdConsoleRows;
        stdConsoleColumns = handle->initialInfo.dwSize.X;

        stdConsoleRows = handle->initialInfo.dwSize.Y;

        handle->nOfColumns = stdConsoleColumns;

        handle->nOfRows = stdConsoleRows;

        //set the console mode
        DWORD consoleMode = 0;
        if (handle->openingMode & BasicConsoleMode::PerformCharacterInput) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_ECHO_INPUT;
            consoleMode |= ENABLE_LINE_INPUT;
        }

        if (handle->openingMode & BasicConsoleMode::DisableControlBreak) {
            consoleMode |= 0;
        }
        else {
            consoleMode |= ENABLE_PROCESSED_INPUT;
        }

        SetConsoleMode(handle->inputConsoleHandle, consoleMode);

        FlushConsoleInputBuffer(handle->inputConsoleHandle);

    }

    return error;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return handle->openingMode;
}

ErrorManagement::ErrorType BasicConsole::ShowBuffer() {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (SetConsoleActiveScreenBuffer(handle->outputConsoleHandle) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::Write(const char8* buffer,
                              uint32 &size,
                              const TimeoutType &timeout) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (handle->openingMode & BasicConsoleMode::EnablePaging) {
        error = PagedWrite(buffer, size, timeout);
    }
    else {
        error = OSWrite(buffer, size, timeout);
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::OSWrite(const char8* const buffer,
                                uint32 &size,
                                const TimeoutType &timeout) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if ((size > 0) && (buffer != NULL)) {

        if (!WriteConsole(handle->outputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {

            error = ErrorManagement::OSError;
        }

    }
    else {
        error = ErrorManagement::Warning;
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::Read(char8* const buffer,
                             uint32 &size,
                             const TimeoutType &timeout) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    DWORD ret = 0;

    if (timeout.IsFinite()) {

        FlushConsoleInputBuffer(handle->inputConsoleHandle);
        ret = WaitForSingleObject(handle->inputConsoleHandle, (DWORD) timeout.GetTimeoutMSec());
        if (ret != 0) {
            size = 0;
            error = ErrorManagement::Timeout;
        }
    }

    if (error == ErrorManagement::NoError) {
        if (handle->openingMode & BasicConsoleMode::PerformCharacterInput) {
            size = 1;
            if (!ReadConsole(handle->inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {
                error = ErrorManagement::OSError;
            }
        }
        else {
            if (!ReadConsole(handle->inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL)) {
                error = ErrorManagement::OSError;
            }

        }

    }
    return error;

}

ErrorManagement::ErrorType BasicConsole::SetTitleBar(const char8 *title) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (!SetConsoleTitle(title)) {
        error = ErrorManagement::OSError;
    }

    return error;
}

ErrorManagement::ErrorType BasicConsole::GetTitleBar(char8 *title,
                                    const uint32 &size) const {

    if (title == NULL) {
        return ErrorManagement::Warning;
    }

    GetConsoleTitle(title, size);

    return ErrorManagement::NoError;

}

ErrorManagement::ErrorType BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                      const uint32 &numberOfRows) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    COORD max = GetLargestConsoleWindowSize(handle->outputConsoleHandle);

    uint32 numberOfColumnsUsed = numberOfColumns;
    uint32 numberOfRowsUsed = numberOfRows;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info);

    if (info.dwSize.X < 0 || info.dwSize.Y < 0) {
        error = ErrorManagement::OSError;
    }

    if (error == ErrorManagement::NoError) {
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

        if (!SetConsoleWindowInfo(handle->outputConsoleHandle, TRUE, &srect)) {
            //  CStaticPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:SetWindowSize:failed SetConsoleWindowInfo ");

            error = ErrorManagement::OSError;
        }
    }

    return error;
}

ErrorManagement::ErrorType BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                      uint32 &numberOfRows) const {

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info);

    numberOfColumns = info.srWindow.Right - info.srWindow.Left + 1;
    numberOfRows = info.srWindow.Bottom - info.srWindow.Top + 1;
    return ErrorManagement::NoError;
}

ErrorManagement::ErrorType BasicConsole::GetSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == FALSE) {
        //  CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
        error = ErrorManagement::OSError;
    }

    if (error == ErrorManagement::NoError) {
        numberOfColumns = info.dwSize.X;
        numberOfRows = info.dwSize.Y;
    }
    return error;

}

ErrorManagement::ErrorType BasicConsole::SetCursorPosition(const uint32 &column,
                                          const uint32 &row) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    COORD c = { column, row };
    if (SetConsoleCursorPosition(handle->outputConsoleHandle, c) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return error;
}

ErrorManagement::ErrorType BasicConsole::GetCursorPosition(uint32 &column,
                                          uint32 &row) const {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == FALSE) {
        error = ErrorManagement::OSError;
    }

    if (error == ErrorManagement::NoError) {
        column = info.dwCursorPosition.X;
        row = info.dwCursorPosition.Y;
    }

    return error;
}

ErrorManagement::ErrorType BasicConsole::SetColour(const Colours &foregroundColour,
                                  const Colours &backgroundColour) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    WORD attribute;
    attribute = (int) foregroundColour & 0xF;
    attribute |= (((int) backgroundColour & 0xF) << 4);

    if (!SetConsoleTextAttribute(handle->outputConsoleHandle, attribute)) {
        error = ErrorManagement::OSError;
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::Clear() {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    COORD c;
    c.X = 0;
    c.Y = 0;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info);
    int nOfChars = info.dwSize.Y * info.dwSize.X;
    DWORD NumberOfCharsWritten;
    FillConsoleOutputAttribute(handle->outputConsoleHandle, info.wAttributes, nOfChars, c, &NumberOfCharsWritten);
    if (!FillConsoleOutputCharacter(handle->outputConsoleHandle, ' ', nOfChars, c, &NumberOfCharsWritten)) {
        error = ErrorManagement::OSError;
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::PlotChar(const char8 &c,
                                 const Colours &foregroundColour,
                                 const Colours &backgroundColour,
                                 const uint32 &column,
                                 const uint32 &row) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
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

    if (WriteConsoleOutput(handle->outputConsoleHandle, &cInfo, bufferSize, coord, &writeRegion) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return error;

}

ErrorManagement::ErrorType BasicConsole::Close() {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    DWORD consoleMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
    if (handle->openingMode & BasicConsoleMode::CreateNewBuffer) {
        CloseHandle(handle->outputConsoleHandle);
    }
    else {
        //reset the initial conditions of the console.
        int nCol = handle->initialInfo.dwSize.X;
        int nRow = handle->initialInfo.dwSize.Y;
        SetSize(nCol, nRow);
        SetConsoleWindowInfo(handle->outputConsoleHandle, TRUE, &(handle->initialInfo.srWindow));
        SetConsoleMode(handle->outputConsoleHandle, consoleMode);
        SetConsoleTextAttribute(handle->outputConsoleHandle, handle->initialInfo.wAttributes);
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

}
