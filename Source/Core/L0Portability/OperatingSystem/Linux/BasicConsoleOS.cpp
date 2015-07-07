/**
 * @file BasicConsole.cpp
 * @brief Source file for class BasicConsole
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
 * the class BasicConsole (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct OperatingSystemProperties {
    /**
     * Standard output file descriptor.
     */
    static const int32 STDOUT = 1;
    /**
     * Standard input file descriptor.
     */
    static const int32 STDIN = 0;
    /**
     * Number of rows that will be cleared when BasicConsoleOSClear is called
     */
    static const uint32 BASIC_CONSOLE_LINUX_CLEAR_ROWS = 40u;

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
    ConsoleHandle initialInfo;

    /**
     * The column counter keeps track of the column where the character was written to.
     */
    uint32 columnCount;

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
    osProperties = new OperatingSystemProperties();
    if (osProperties != static_cast<OperatingSystemProperties *>(NULL)) {
        osProperties->columnCount = 0u;
        osProperties->nOfColumns = 0u;
        osProperties->nOfRows = 0u;
        memset(&osProperties->inputConsoleHandle, 0, sizeof(ConsoleHandle));
        memset(&osProperties->outputConsoleHandle, 0, sizeof(ConsoleHandle));
        memset(&osProperties->initialInfo, 0, sizeof(ConsoleHandle));
    }
    lastPagingCounter =  0;
    lineCount = 0;
}

BasicConsole::~BasicConsole() {
    if (osProperties != static_cast<OperatingSystemProperties *>(NULL)) {
        delete osProperties;
    }
    /*lint -e{534} closure failure of a console is not handled*/
    Close();
}

bool BasicConsole::Open(const FlagsType &mode) {
    bool ok = true;
    osProperties->openingMode = mode;
    //In this case read immediately from the console without wait.
    bool charactedInputSelected = (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u;
    if (charactedInputSelected) {
        ok = ioctl(fileno(stdin), static_cast<osulong>(TCGETA), &osProperties->outputConsoleHandle) >= 0;
        if (ok) {
            osProperties->initialInfo = osProperties->outputConsoleHandle;
            struct termio &consoleMode = osProperties->outputConsoleHandle;

            //use the input handle to save default parameters
            struct termio &saveMode = osProperties->inputConsoleHandle;
            uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
            flag &= ~static_cast<uint16>(ICANON);
            consoleMode.c_lflag = flag;
            saveMode.c_cc[VMIN] = consoleMode.c_cc[VMIN];
            consoleMode.c_cc[VMIN] = 1u;
            saveMode.c_cc[VTIME] = consoleMode.c_cc[VTIME];
            consoleMode.c_cc[VTIME] = 0u;

            ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &(osProperties->outputConsoleHandle)) >= 0);
        }
    }
    if (ok) {
        ok = (fflush(stdin) == 0);
    }
    return ok;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return osProperties->openingMode;
}

bool BasicConsole::Close() {
    bool charactedInputSelected = (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u;
    bool ok = true;
    if (charactedInputSelected) {
        //reset the original console modes
        struct termio &consoleMode = osProperties->outputConsoleHandle;
        struct termio &saveMode = osProperties->inputConsoleHandle;
        uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
        flag |= static_cast<uint16>(ICANON);
        consoleMode.c_lflag = flag;
        consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
        consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
        ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &osProperties->initialInfo) >= 0);
    }
    return ok;
}

/*lint -e{715} timeout is not used...*/
bool BasicConsole::Write(const char8* const buffer, uint32 &size, const TimeoutType &timeout) {
    const char8 *bufferString = buffer;
    const char8 newLine = '\n';

    ssize_t writtenBytes = 0;

    char8 currentChar = '\0';
    uint32 currentColumn = osProperties->columnCount;
    uint32 index = 0u;
    uint32 start = 0u;
    uint32 sizeToWrite = 0u;

    bool sink = false;

    while (index < size) {
        currentChar = bufferString[index];
        if ((currentChar == '\0') || (currentChar == '\n') || (index == (size - 1))) {
            sink = true;
        }

        if (currentColumn == osProperties->nOfColumns) {
            sink = true;
        }

        if (sink) {
            sizeToWrite = index - start + 1;
            if (sizeToWrite > 0u) {
                writtenBytes += write(osProperties->STDOUT, &bufferString[start], static_cast<osulong>(sizeToWrite));
            }
            start = index + 1;
            sink = false;
        }
        if (currentColumn == osProperties->nOfColumns) {
            write(osProperties->STDOUT, &newLine, static_cast<osulong>(1));
            currentColumn = 0u;
        }

        currentColumn++;
        index++;
    }
    osProperties->columnCount = currentColumn;

    size = static_cast<uint32>(writtenBytes);
    return (size > 0u);
}

/*lint -e{715} timeout is not used...*/
bool BasicConsole::Read(char8 * const buffer, uint32 & size, const TimeoutType &timeout) {
    bool ok = false;
    if ((buffer != NULL) && (size > 0u)) {
        if ((osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u) {
            char8 *readChar = buffer;
            *readChar = static_cast<char8>(getchar());
            size = 1u;
        }
        else {
            ssize_t readBytes = read(osProperties->STDIN, buffer, static_cast<osulong>(size));
            size = static_cast<uint32>(readBytes);
        }
        ok = (size > 0u);
    }
    else {
        ok = false;
    }
    return ok;
}

bool BasicConsole::SetSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    osProperties->nOfColumns = numberOfColumns;
    osProperties->nOfRows = numberOfRows;
    return true;
}

bool BasicConsole::GetSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    numberOfColumns = osProperties->nOfColumns;
    numberOfRows = osProperties->nOfRows;
    return true;
}

bool BasicConsole::Clear() {
    for (uint32 i = 0u; i < osProperties->BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        write(osProperties->STDOUT, "\n", static_cast<osulong>(1u));
    }
    return true;
}

bool BasicConsole::CursorPositionSupported() const {
    return false;
}

bool BasicConsole::TitleBarSupported() const {
    return false;
}

bool BasicConsole::ColourSupported() const {
    return false;
}

bool BasicConsole::WindowSizeSupported() const {
    return false;
}

bool BasicConsole::ConsoleBufferSupported() const {
    return false;
}

bool BasicConsole::Show() {
    return true;
}

bool BasicConsole::SetColour(const Colours &foregroundColour, const Colours &backgroundColour) {
    return true;
}

bool BasicConsole::SetTitleBar(const char8 * const title) {
    return true;
}

bool BasicConsole::GetTitleBar(char8 * const title) const {
    return true;
}

bool BasicConsole::SetCursorPosition(const uint32 &column, const uint32 &row) {
    return true;
}

bool BasicConsole::GetCursorPosition(uint32 &column, uint32 &row) const {
    return true;
}

bool BasicConsole::SetWindowSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    return true;
}

bool BasicConsole::GetWindowSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    return true;
}

bool BasicConsole::PlotChar(const char8 &c, const Colours &foregroundColour, const Colours &backgroundColour, const uint32 &column, const uint32 &row) {
    return true;
}

