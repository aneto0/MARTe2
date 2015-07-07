/**
 * @file BasicConsoleOS.cpp
 * @brief Source file for class BasicConsoleOS
 * @date 05/07/2015
 * @author Andre' Neto
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicConsoleOS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsoleOS::BasicConsoleOS() : IBasicConsole::IBasicConsole() {
    columnCount = 0u;
    nOfColumns = 0u;
    nOfRows = 0u;
    memset(&inputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&outputConsoleHandle, 0, sizeof(ConsoleHandle));
    memset(&initialInfo, 0, sizeof(ConsoleHandle));
}

BasicConsoleOS::~BasicConsoleOS() {
    try {
        /*lint -e{534} too late to handle a closure failure*/
        BasicConsoleOS::Close();
    }
    catch (...) {

    }
}

bool BasicConsoleOS::Open(const FlagsType &mode) {
    bool ok = true;
    this->openingMode = mode;
    //In this case read immediately from the console without wait.
    bool charactedInputSelected = (openingMode & IBasicConsole::Mode::PerformCharacterInput) != 0u;
    if (charactedInputSelected) {
        ok = ioctl(fileno(stdin), static_cast<osulong>(TCGETA), &outputConsoleHandle) >= 0;
        if (ok) {
            initialInfo = outputConsoleHandle;
            SetImmediateRead();

            ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &(outputConsoleHandle)) >= 0);
        }
    }
    if (ok) {
        ok = (fflush(stdin) == 0);
    }
    return ok;
}

FlagsType BasicConsoleOS::GetOpeningMode() const {
    return openingMode;
}

bool BasicConsoleOS::Close() {
    bool charactedInputSelected = (openingMode & IBasicConsole::Mode::PerformCharacterInput) != 0u;
    bool ok = true;
    if (charactedInputSelected) {
        //reset the original console modes
        UnSetImmediateRead();
        ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &initialInfo) >= 0);
    }
    return ok;
}

/*lint -e{715} timeout is not used...*/
bool BasicConsoleOS::Write(const char8* const buffer, uint32 &size, const TimeoutType &timeout) {
    const char8 *bufferString = buffer;
    const char8 newLine = '\n';

    ssize_t writtenBytes = 0;

    char8 currentChar = '\0';
    uint32 currentColumn = columnCount;
    uint32 index = 0u;
    uint32 start = 0u;
    uint32 sizeToWrite = 0u;

    bool sink = false;

    while (index < (size + 1u)) {
        currentChar = bufferString[index];
        if ((currentChar == '\0') || (currentChar == '\n')) {
            sink = true;
        }

        if (currentColumn == nOfColumns) {
            sink = true;
        }

        if (sink) {
            sizeToWrite = index - start;
            if (sizeToWrite > 0u) {
                writtenBytes += write(STDOUT, &bufferString[start], static_cast<osulong>(sizeToWrite));
            }
            start = index;
        }
        if (currentColumn == nOfColumns) {
            write(STDOUT, &newLine, static_cast<osulong>(1));
            currentColumn = 0u;
        }

        currentColumn++;
        index++;
    }
    columnCount = currentColumn;

    size = static_cast<uint32>(writtenBytes);
    return (size > 0u);
}

/*lint -e{715} timeout is not used...*/
bool BasicConsoleOS::Read(char8 * const buffer, uint32 & size, const TimeoutType &timeout) {
    bool ok = false;
    if ((buffer != NULL) && (size > 0u)) {
        if ((openingMode & IBasicConsole::Mode::PerformCharacterInput) != 0u) {
            char8 *readChar = buffer;
            *readChar = static_cast<char8>(getchar());
            size = 1u;
        }
        else {
            ssize_t readBytes = read(STDIN, buffer, static_cast<osulong>(size));
            size = static_cast<uint32>(readBytes);
        }
        ok = (size > 0u);
    }
    else {
        ok = false;
    }
    return ok;
}

bool BasicConsoleOS::SetSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    this->nOfColumns = numberOfColumns;
    this->nOfRows = numberOfRows;
    return true;
}

bool BasicConsoleOS::GetSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    numberOfColumns = this->nOfColumns;
    numberOfRows = this->nOfRows;
    return true;
}

bool BasicConsoleOS::Clear() {
    for (uint32 i = 0u; i < BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        write(STDOUT, "\n", static_cast<osulong>(1u));
    }
    return true;
}

bool BasicConsoleOS::CursorPositionSupported() const {
    return false;
}

bool BasicConsoleOS::TitleBarSupported() const {
    return false;
}

bool BasicConsoleOS::ColourSupported() const {
    return false;
}

bool BasicConsoleOS::WindowSizeSupported() const {
    return false;
}

bool BasicConsoleOS::ConsoleBufferSupported() const {
    return false;
}

bool BasicConsoleOS::Show() {
    return true;
}

bool BasicConsoleOS::SetColour(const Colours &foregroundColour, const Colours &backgroundColour) {
    return true;
}

bool BasicConsoleOS::SetTitleBar(const char8 * const title) {
    return true;
}

bool BasicConsoleOS::GetTitleBar(char8 * const title) const {
    return true;
}

bool BasicConsoleOS::SetCursorPosition(const uint32 &column, const uint32 &row) {
    return true;
}

bool BasicConsoleOS::GetCursorPosition(uint32 &column, uint32 &row) const {
    return true;
}

bool BasicConsoleOS::SetWindowSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    return true;
}

bool BasicConsoleOS::GetWindowSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    return true;
}

bool BasicConsoleOS::PlotChar(const char8 &c, const Colours &foregroundColour, const Colours &backgroundColour, const uint32 &column, const uint32 &row) {
    return true;
}

void BasicConsoleOS::SetImmediateRead() {
    struct termio &consoleMode = outputConsoleHandle;

    //use the input handle to save default parameters
    struct termio &saveMode = inputConsoleHandle;
    uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
    flag &= ~static_cast<uint16>(ICANON);
    consoleMode.c_lflag = flag;
    saveMode.c_cc[VMIN] = consoleMode.c_cc[VMIN];
    consoleMode.c_cc[VMIN] = 1u;
    saveMode.c_cc[VTIME] = consoleMode.c_cc[VTIME];
    consoleMode.c_cc[VTIME] = 0u;
}

void BasicConsoleOS::UnSetImmediateRead() {

    struct termio &consoleMode = outputConsoleHandle;
    struct termio &saveMode = inputConsoleHandle;
    uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
    flag |= static_cast<uint16>(ICANON);
    consoleMode.c_lflag = flag;
    consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
    consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
}
