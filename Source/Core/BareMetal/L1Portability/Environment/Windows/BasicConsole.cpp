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
#include "StreamI.h"
#include "BasicConsole.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

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

BasicConsole::BasicConsole() :
        StreamI() {
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

bool BasicConsole::SetSceneSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {

    bool ret=true;
    SHORT windowColumns = 0;
    SHORT windowRows = 0;
    SHORT _numberOfColumns = static_cast<SHORT>(numberOfColumns);
    SHORT _numberOfRows = static_cast<SHORT>(numberOfRows);

    //aligns the buffer and window sizes as much as possible to the specified parameters.

    //Note: the width and height specified cannot be less than the console screen buffer's ones.

    CONSOLE_SCREEN_BUFFER_INFO info;


    //get the console informations
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == 0) {
        ret=false;
    }

    if (ret) {

        COORD stage1BufferSize;
        stage1BufferSize.X = info.dwSize.X;

        //enlarge the coordinates if they are minor than the desired size
        //now i can only enlarge them!
        if (stage1BufferSize.X < _numberOfColumns) {
            stage1BufferSize.X = _numberOfColumns;
        }

        stage1BufferSize.Y = info.dwSize.Y;
        if (stage1BufferSize.Y < _numberOfRows) {
            stage1BufferSize.Y = _numberOfRows;
        }

        //set the buffer size
        if (!SetConsoleScreenBufferSize(handle->outputConsoleHandle, stage1BufferSize)) {
            // CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
            ret = false;
        }

    }

    if (ret) {
        windowColumns = info.srWindow.Right - info.srWindow.Left + 1;
        windowRows = info.srWindow.Bottom - info.srWindow.Top + 1;

        if (windowColumns < 0 || windowRows < 0) {
            ret=false;

        }

    }

    if (ret) {
        if (windowColumns > _numberOfColumns) {
            windowColumns = _numberOfColumns;

        }
        if (windowRows > _numberOfRows) {
            windowRows = _numberOfRows;
        }

        SMALL_RECT srect;
        srect.Left = info.srWindow.Left;
        srect.Top = info.srWindow.Top;
        srect.Right = srect.Left + windowColumns - 1;
        srect.Bottom = srect.Top + windowRows - 1;

        //set the new windows size
        if (!SetConsoleWindowInfo(handle->outputConsoleHandle, TRUE, &srect)) {
            //  CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
            ret=false;

        }

    }

    if (ret) {
        //now if buffersize is greater than windows size they become aligned.
        COORD stage2BufferSize;
        stage2BufferSize.X = _numberOfColumns;
        stage2BufferSize.Y = _numberOfRows;

        if (!SetConsoleScreenBufferSize(handle->outputConsoleHandle, stage2BufferSize)) {
            ret=false;

        }

    }

    return (ret);
}

bool BasicConsole::Open(const FlagsType &mode) {

//    con.selectedStream      = NormalStreamMode;

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
        if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &(handle->initialInfo)) == 0) {
            error = ErrorManagement::OSError;
        }
    }

    if (error == ErrorManagement::NoError) {
        SHORT stdConsoleColumns;
        SHORT stdConsoleRows;
        stdConsoleColumns = handle->initialInfo.dwSize.X;

        stdConsoleRows = handle->initialInfo.dwSize.Y;

        handle->nOfColumns = static_cast<uint32>(stdConsoleColumns);
        handle->nOfRows    = static_cast<uint32>(stdConsoleRows);

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

    return (error == ErrorManagement::NoError);
}

FlagsType BasicConsole::GetOpeningMode() const {
    return handle->openingMode;
}

bool BasicConsole::ShowBuffer() {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (SetConsoleActiveScreenBuffer(handle->outputConsoleHandle) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);

}

bool BasicConsole::Write(const char8* buffer,
                         uint32 &size,
                         const MilliSeconds &timeout) {

    bool ret = true;


    if (handle->openingMode & BasicConsoleMode::EnablePaging) {
        ret = PagedWrite(buffer, size, timeout);
    }
    else {
        ret = OSWrite(buffer, size, timeout);
    }

    return ret;

}

bool BasicConsole::OSWrite(const char8* const buffer,
                           uint32 &size,
                           const MilliSeconds &timeout) {
    ErrorManagement::ErrorType ret;

    if ((size > 0) && (buffer != NULL)) {

        ret.OSError = !WriteConsole(handle->outputConsoleHandle, buffer, size, (unsigned long *) &size, NULL);

    } else {
        ret.warning = true;
    }

    return ret.ErrorsCleared();

}

bool BasicConsole::Read(char8* const buffer,
                        uint32 &size) {
    return Read(buffer, size, MilliSeconds::Infinite);
}

bool BasicConsole::Write(const char8* const buffer,
                         uint32 &size) {
    return Write(buffer, size, MilliSeconds::Infinite);
}

bool BasicConsole::Read(char8* const buffer,
                        uint32 &size,
                        const MilliSeconds &timeout) {
    ErrorManagement::ErrorType ret;

    if (timeout.IsValid()) {

        FlushConsoleInputBuffer(handle->inputConsoleHandle);
        DWORD rc = WaitForSingleObject(handle->inputConsoleHandle, (DWORD) timeout.GetTimeRaw());
        if (rc != 0) {
            size = 0;
            ret.timeout = true;
        }
    }

    if (ret) {
        if (handle->openingMode & BasicConsoleMode::PerformCharacterInput) {
            size = 1;
        }
        ret.OSError = !ReadConsole(handle->inputConsoleHandle, buffer, size, (unsigned long *) &size, NULL);

    }
    return ret.ErrorsCleared();

}

bool BasicConsole::SetTitleBar(CCString title) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (!SetConsoleTitle(title.GetList())) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);
}

bool BasicConsole::GetTitleBar(CString const title,
                               const uint32 &size) const {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    if (title.IsNullPtr()) {
        return false;
    }

    GetConsoleTitle(title.GetList(), size);

    return (error == ErrorManagement::NoError);

}

bool BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                 const uint32 &numberOfRows) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    COORD max = GetLargestConsoleWindowSize(handle->outputConsoleHandle);

    SHORT numberOfColumnsUsed = static_cast<SHORT>(numberOfColumns);
    SHORT numberOfRowsUsed    = static_cast<SHORT>(numberOfRows);

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

    return (error == ErrorManagement::NoError);
}

bool BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                 uint32 &numberOfRows) const {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info);

    numberOfColumns = static_cast<uint32>(info.srWindow.Right - info.srWindow.Left + 1);
    numberOfRows 	= static_cast<uint32>(info.srWindow.Bottom - info.srWindow.Top + 1);
    return (error == ErrorManagement::NoError);
}

bool BasicConsole::GetSceneSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == FALSE) {
        //  CStaticAssertPlatformErrorCondition(Errors::ErrorManagement::OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
        error = ErrorManagement::OSError;
    }

    if (error == ErrorManagement::NoError) {
        numberOfColumns = static_cast<uint32>(info.dwSize.X);
        numberOfRows 	= static_cast<uint32>(info.dwSize.Y);
    }
    return (error == ErrorManagement::NoError);
}

bool BasicConsole::SetCursorPosition(const uint32 &column,
                                     const uint32 &row) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    COORD c = { static_cast<SHORT>(column), static_cast<SHORT>(row) };
    if (SetConsoleCursorPosition(handle->outputConsoleHandle, c) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);;
}

bool BasicConsole::GetCursorPosition(uint32 &column,
                                     uint32 &row) const {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info) == FALSE) {
        error = ErrorManagement::OSError;
    }

    if (error == ErrorManagement::NoError) {
        column 	= static_cast<uint32>(info.dwCursorPosition.X);
        row 	= static_cast<uint32>(info.dwCursorPosition.Y);
    }

    return (error == ErrorManagement::NoError);
}

bool BasicConsole::SetColour(const Colours &foregroundColour,
                             const Colours &backgroundColour) {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    WORD attribute;
    attribute = static_cast<WORD>(foregroundColour & 0xF);
    attribute |= (static_cast<WORD>(backgroundColour & 0xF) << 4);

    if (!SetConsoleTextAttribute(handle->outputConsoleHandle, attribute)) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);

}

bool BasicConsole::Clear() {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;

    COORD c;
    c.X = 0;
    c.Y = 0;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle->outputConsoleHandle, &info);
    DWORD nOfChars = static_cast<DWORD>(info.dwSize.Y * info.dwSize.X);
    DWORD NumberOfCharsWritten;
    FillConsoleOutputAttribute(handle->outputConsoleHandle, info.wAttributes, nOfChars, c, &NumberOfCharsWritten);
    if (!FillConsoleOutputCharacter(handle->outputConsoleHandle, ' ', nOfChars, c, &NumberOfCharsWritten)) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);

}

bool BasicConsole::PlotChar(const char8 &c,
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
    attribute = static_cast<WORD>(foregroundColour & 0xF);
    attribute |= static_cast<WORD>(backgroundColour & 0xF) << 4;

    CHAR_INFO cInfo;
    cInfo.Char.AsciiChar = c;
    cInfo.Attributes = attribute;

    SMALL_RECT writeRegion;
    writeRegion.Left 	= static_cast<SHORT>(row);
    writeRegion.Right 	= static_cast<SHORT>(row);
    writeRegion.Top 	= static_cast<SHORT>(column);
    writeRegion.Bottom 	= static_cast<SHORT>(column);

    if (WriteConsoleOutput(handle->outputConsoleHandle, &cInfo, bufferSize, coord, &writeRegion) == FALSE) {
        error = ErrorManagement::OSError;
    }

    return (error == ErrorManagement::NoError);

}

bool BasicConsole::Close() {

    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    DWORD consoleMode = ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT;
    if (handle->openingMode & BasicConsoleMode::CreateNewBuffer) {
        CloseHandle(handle->outputConsoleHandle);
    }
    else {
        //reset the initial conditions of the console.
        uint32 nCol = static_cast<uint32>(handle->initialInfo.dwSize.X);
        uint32 nRow = static_cast<uint32>(handle->initialInfo.dwSize.Y);
        SetSceneSize(nCol, nRow);
        SetConsoleWindowInfo(handle->outputConsoleHandle, TRUE, &(handle->initialInfo.srWindow));
        SetConsoleMode(handle->outputConsoleHandle, consoleMode);
        SetConsoleTextAttribute(handle->outputConsoleHandle, handle->initialInfo.wAttributes);
    }

    return (error == ErrorManagement::NoError);
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

bool BasicConsole::SetSize(uint64 size) {
    return false;
}

uint64 BasicConsole::Size() const {
    return 0xffffffffffffffff;
}

bool BasicConsole::CanWrite() const {
    return true;
}

bool BasicConsole::CanRead() const {
    return true;
}

bool BasicConsole::CanSeek() const {
    return false;
}

bool BasicConsole::Seek(uint64 pos) {
    return false;
}

bool BasicConsole::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicConsole::Position() const {
    return 0xffffffffffffffff;
}

Handle BasicConsole::GetReadHandle() const {
    return handle->inputConsoleHandle;
}

Handle BasicConsole::GetWriteHandle() const {
    return handle->outputConsoleHandle;
}

}
