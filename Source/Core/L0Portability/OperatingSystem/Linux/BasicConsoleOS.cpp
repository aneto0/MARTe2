/**
 * @file BasicConsoleOS.cpp
 * @brief Source file for class BasicConsole
 * @date 05/07/2015
 * @author André Neto
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
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{9109} forward declaration in BasicConsole.h is required to define the class*/
/*lint -esym(9150, BasicConsoleOSProperties::*) */

struct BasicConsoleOSProperties {
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
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    osProperties = new BasicConsoleOSProperties();
    osProperties->columnCount = 0u;
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

ErrorType BasicConsole::Open(const FlagsType &mode) {
    ErrorType err = NoError;

    osProperties->openingMode = mode;
    //In this case read immediately from the console without wait.
    bool charactedInputSelected = (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u;
    if (charactedInputSelected) {
        bool ok = ioctl(fileno(stdin), static_cast<osulong>(TCGETA), &osProperties->outputConsoleHandle) >= 0;
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
            if (!ok) {
                err = OSError;
            }
        }
        else {
            err = OSError;
        }
    }
    if (err == NoError) {
        bool ok = (fflush(stdout) == 0);
        if (!ok) {
            err = OSError;
        }
    }
    return err;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return osProperties->openingMode;
}

ErrorType BasicConsole::Close() {
    bool charactedInputSelected = (osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u;
    ErrorType err = NoError;
    if (charactedInputSelected) {
        //reset the original console modes
        struct termio &consoleMode = osProperties->outputConsoleHandle;
        struct termio &saveMode = osProperties->inputConsoleHandle;
        uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
        flag |= static_cast<uint16>(ICANON);
        consoleMode.c_lflag = flag;
        consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
        consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
        bool ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &osProperties->initialInfo) >= 0);
        if (!ok) {
            err = OSError;
        }
    }
    return err;
}

/*lint -e{715} timeout is not used in Linux*/
ErrorType BasicConsole::Write(const char8 * const buffer,
                              uint32 & size,
                              const TimeoutType &timeout) {
    ErrorType err = NoError;
    if ((osProperties->openingMode & BasicConsole::Mode::EnablePaging) == BasicConsole::Mode::EnablePaging) {
        err = PagedWrite(buffer, size, timeout);
    }
    else {
        err = OSWrite(buffer, size, timeout);
    }
    return err;
}

/*lint -e{715} timeout is not used in Linux*/
ErrorType BasicConsole::OSWrite(const char8* const buffer,
                                uint32 &size,
                                const TimeoutType &timeout) {
    const char8 *bufferString = buffer;
    const char8 newLine = '\n';

    ssize_t writtenBytes = 0;

    char8 currentChar = '\0';
    uint32 currentColumn = osProperties->columnCount;
    uint32 index = 0u;
    uint32 start = 0u;
    uint32 sizeToWrite = 0u;

    bool sink = false;
    ErrorType err = NoError;
    while ((err == NoError) && (index < size)) {
        currentChar = bufferString[index];
        if (currentChar == '\n') {
            //reset the currentColumn
            currentColumn = 0u;
        }

        if ((currentChar == '\n') || (index == (size - 1u))) {
            sink = true;
        }

        if (currentColumn == osProperties->nOfColumns) {
            sink = true;
        }

        if (sink) {
            sizeToWrite = (index - start) + 1u;

            if (sizeToWrite > 0u) {
                ssize_t wbytes = write(BasicConsoleOSProperties::STDOUT, &bufferString[start], static_cast<osulong>(sizeToWrite));
                if (wbytes == -1) {
                    err = OSError;
                }
                writtenBytes += wbytes;
            }
            start = index + 1u;
            sink = false;
        }
        if (currentColumn == osProperties->nOfColumns) {
            ssize_t wbytes = write(BasicConsoleOSProperties::STDOUT, &newLine, static_cast<osulong>(1));
            if (wbytes == -1) {
                err = OSError;
            }
            currentColumn = 0u;
        }

        currentColumn++;
        index++;
    }
    osProperties->columnCount = currentColumn;

    size = static_cast<uint32>(writtenBytes);
    if (size == 0u) {
        err = Warning;
    }
    return err;
}

/*lint -e{715} timeout is not used...*/
ErrorType BasicConsole::Read(char8 * const buffer,
                             uint32 & size,
                             const TimeoutType &timeout) {
    ErrorType err = NoError;
    if ((buffer != NULL) && (size > 0u)) {
        if ((osProperties->openingMode & BasicConsole::Mode::PerformCharacterInput) != 0u) {
            char8 *readChar = buffer;
            *readChar = static_cast<char8>(getchar());
            int32 eofCheck = static_cast<int32>(*readChar);
            if (eofCheck == EOF) {
                err = OSError;
            }
            else {
                size = 1u;
            }
        }
        else {
            ssize_t readBytes = read(BasicConsoleOSProperties::STDIN, buffer, static_cast<osulong>(size));
            if (readBytes == -1) {
                err = OSError;
            }
            else if (readBytes == 0) {
                err = Warning;
            }
            else {
                size = static_cast<uint32>(readBytes);
            }
        }
    }
    else {
        err = Warning;
    }
    return err;
}

ErrorType BasicConsole::SetSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {
    osProperties->nOfColumns = numberOfColumns;
    osProperties->nOfRows = numberOfRows;
    return NoError;
}

ErrorType BasicConsole::GetSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {
    numberOfColumns = osProperties->nOfColumns;
    numberOfRows = osProperties->nOfRows;
    return NoError;
}

ErrorType BasicConsole::Clear() {
    ErrorType err = NoError;
    for (uint32 i = 0u; i < BasicConsoleOSProperties::BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        ssize_t writtenBytes = write(BasicConsoleOSProperties::STDOUT, "\n", static_cast<osulong>(1u));
        if (writtenBytes == -1) {
            err = OSError;
        }
    }
    return err;
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

bool BasicConsole::TimeoutSupported() const {
    return false;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::ShowBuffer() {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::SetColour(const Colours &foregroundColour,
                                  const Colours &backgroundColour) {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::SetTitleBar(const char8 * const title) {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::GetTitleBar(char8 * const title,
                                    const uint32 &size) const {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::SetCursorPosition(const uint32 &column,
                                          const uint32 &row) {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::GetCursorPosition(uint32 &column,
                                          uint32 &row) const {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                      const uint32 &numberOfRows) {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                      uint32 &numberOfRows) const {
    return UnsupportedFeature;
}

/*lint -e{715} function not implemented in Linux*/
ErrorType BasicConsole::PlotChar(const char8 &c,
                                 const Colours &foregroundColour,
                                 const Colours &backgroundColour,
                                 const uint32 &column,
                                 const uint32 &row) {
    return UnsupportedFeature;
}
