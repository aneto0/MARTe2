/**
 * @file BasicConsole.cpp
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{9109} forward declaration in BasicConsole.h is required to define the class*/
/*lint -esym(9150, MARTe::BasicConsoleProperties::*) */
struct BasicConsoleProperties {
    /**
     * Standard output file descriptor.
     */
    static const Handle STDOUT = 1;

    /**
     * Standard input file descriptor.
     */
    static const Handle STDIN = 0;

    /**
     * Number of rows that will be cleared when BasicConsoleClear is called
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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole() :
        StreamI(),
        HandleI() {
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    handle = new BasicConsoleProperties();
    handle->columnCount = 0u;
    handle->nOfColumns = 0u;
    handle->nOfRows = 0u;
    memset(&handle->inputConsoleHandle, 0, static_cast<size_t>(sizeof(ConsoleHandle)));
    memset(&handle->outputConsoleHandle, 0, static_cast<size_t>(sizeof(ConsoleHandle)));
    memset(&handle->initialInfo, 0, static_cast<size_t>(sizeof(ConsoleHandle)));
    lastPagingCounter = 0;
    lineCount = 0u;
}

BasicConsole::~BasicConsole() {
    if (handle != static_cast<BasicConsoleProperties *>(NULL)) {
        /*lint -e{534} possible closure failure is not handled in the destructor.*/
        /*lint -e{1551} exception not caught.*/
        BasicConsole::Close();
        delete handle;
    }
}

bool BasicConsole::Open(const FlagsType &mode) {
    bool err = true;

    handle->openingMode = mode;
    //In this case read immediately from the console without wait.
    bool charactedInputSelected = (handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u;
    if (charactedInputSelected) {
        bool ok = ioctl(fileno(stdin), static_cast<osulong>(TCGETA), &handle->outputConsoleHandle) >= 0;
        if (ok) {
            handle->initialInfo = handle->outputConsoleHandle;
            struct termio &consoleMode = handle->outputConsoleHandle;

            //use the input handle to save default parameters
            struct termio &saveMode = handle->inputConsoleHandle;
            uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
            flag &= ~static_cast<uint16>(ICANON);
            consoleMode.c_lflag = flag;
            saveMode.c_cc[VMIN] = consoleMode.c_cc[VMIN];
            consoleMode.c_cc[VMIN] = 1u;
            saveMode.c_cc[VTIME] = consoleMode.c_cc[VTIME];
            consoleMode.c_cc[VTIME] = 0u;

            ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &(handle->outputConsoleHandle)) >= 0);
            if (!ok) {
                err = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed iocl()");
            }
        }
        else {
            err = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed iocl()");
        }
    }
    if (err) {
        bool ok = (fflush(stdout) == 0);
        if (!ok) {
            err = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed fflush()");
        }
    }
    return err;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return handle->openingMode;
}

bool BasicConsole::Close() {
    bool charactedInputSelected = (handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u;
    bool err = true;
    if (charactedInputSelected) {
        //reset the original console modes
        struct termio &consoleMode = handle->outputConsoleHandle;
        struct termio &saveMode = handle->inputConsoleHandle;
        uint16 flag = static_cast<uint16>(consoleMode.c_lflag);
        flag |= static_cast<uint16>(ICANON);
        consoleMode.c_lflag = flag;
        consoleMode.c_cc[VMIN] = saveMode.c_cc[VMIN];
        consoleMode.c_cc[VTIME] = saveMode.c_cc[VTIME];
        bool ok = (ioctl(fileno(stdin), static_cast<osulong>(TCSETAW), &handle->initialInfo) >= 0);
        if (!ok) {
            err = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed iocl()");
        }
    }
    return err;
}

bool BasicConsole::Read(char8 * const output,
                        uint32 & size,
                        const TimeoutType &timeout) {
    bool ret = false;
    if (!timeout.IsFinite()) {
        ret = Read(output, size);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Cannot read within timeout");
    }

    return ret;
}

bool BasicConsole::Write(const char8 * const input,
                         uint32 & size,
                         const TimeoutType &timeout) {

    bool ret = false;
    if (!timeout.IsFinite()) {
        ret = Write(input, size);
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Cannot write within timeout");
    }

    return ret;
}

bool BasicConsole::Write(const char8 * const input,
                         uint32 & size) {
    bool err = true;
    if ((handle->openingMode & BasicConsoleMode::EnablePaging) == BasicConsoleMode::EnablePaging) {
        err = PagedWrite(input, size, TTInfiniteWait);
    }
    else {
        err = OSWrite(input, size, TTInfiniteWait);
    }
    return err;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Write with timeout not implemented. */
bool BasicConsole::OSWrite(const char8* const buffer,
                           uint32 &size,
                           const TimeoutType &timeout) {
    const char8 *bufferString = buffer;
    const char8 newLine = '\n';

    ssize_t writtenBytes = 0;

    char8 currentChar = '\0';
    uint32 currentColumn = handle->columnCount;
    uint32 index = 0u;
    uint32 start = 0u;
    uint32 sizeToWrite = 0u;

    bool sink = false;
    bool err = true;
    while ((err == true) && (index < size)) {
        currentChar = bufferString[index];
        if (currentChar == '\n') {
            //reset the currentColumn
            currentColumn = 0u;
        }

        if ((currentChar == '\n') || (index == (size - 1u))) {
            sink = true;
        }

        if (currentColumn == handle->nOfColumns) {
            sink = true;
        }

        if (sink) {
            sizeToWrite = (index - start) + 1u;

            if (sizeToWrite > 0u) {
                ssize_t wbytes = write(BasicConsoleProperties::STDOUT, &bufferString[start], static_cast<size_t>(sizeToWrite));
                if (wbytes == -1) {
                    err = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed write()");
                }
                writtenBytes += wbytes;
            }
            start = index + 1u;
            sink = false;
        }
        if (currentColumn == handle->nOfColumns) {
            ssize_t wbytes = write(BasicConsoleProperties::STDOUT, &newLine, static_cast<size_t>(1));
            if (wbytes == -1) {
                err = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed write()");
            }
            currentColumn = 0u;
        }

        currentColumn++;
        index++;
    }
    handle->columnCount = currentColumn;

    size = static_cast<uint32>(writtenBytes);
    if (size == 0u) {
        err = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicConsole: Zero bytes written");
    }
    return err;
}

bool BasicConsole::Read(char8 * const output,
                        uint32 & size) {
    bool err = true;
    if ((output != NULL) && (size > 0u)) {
        if ((handle->openingMode & BasicConsoleMode::PerformCharacterInput) != 0u) {
            char8 *readChar = output;
            *readChar = static_cast<char8>(getchar());
            int32 eofCheck = static_cast<int32>(*readChar);
            if (eofCheck == EOF) {
                err = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed getchar()");
            }
            else {
                size = 1u;
            }
        }
        else {
            ssize_t readBytes = read(BasicConsoleProperties::STDIN, output, static_cast<size_t>(size));
            if (readBytes == -1) {
                err = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed read()");
            }
            else if (readBytes == 0) {
                err = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicConsole: Zero bytes read");
            }
            else {
                size = static_cast<uint32>(readBytes);
            }
        }
    }
    else {
        err = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicConsole: Invalid input parameters");
    }
    return err;
}

bool BasicConsole::SetSceneSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {
    handle->nOfColumns = numberOfColumns;
    handle->nOfRows = numberOfRows;
    return true;
}

bool BasicConsole::GetSceneSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {
    numberOfColumns = handle->nOfColumns;
    numberOfRows = handle->nOfRows;
    return true;
}

bool BasicConsole::Clear() {
    bool err = true;
    for (uint32 i = 0u; i < BasicConsoleProperties::BASIC_CONSOLE_LINUX_CLEAR_ROWS; i++) {
        ssize_t writtenBytes = write(BasicConsoleProperties::STDOUT, "\n", static_cast<size_t>(1u));
        if (writtenBytes == -1) {
            err = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicConsole: Failed write()");
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

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::ShowBuffer() {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");
    return false;
}

/*lint -e{715} function not implemented in Linux*/
bool BasicConsole::SetColour(const Colours &foregroundColour,
                             const Colours &backgroundColour) {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::SetTitleBar(const char8 * const title) {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::GetTitleBar(char8 * const title,
                               const uint32 &size) const {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::SetCursorPosition(const uint32 &column,
                                     const uint32 &row) {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");

    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::GetCursorPosition(uint32 &column,
                                     uint32 &row) const {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                 const uint32 &numberOfRows) {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");

    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                 uint32 &numberOfRows) const {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");

    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::PlotChar(const char8 &c,
                            const Colours &foregroundColour,
                            const Colours &backgroundColour,
                            const uint32 &column,
                            const uint32 &row) {
    REPORT_ERROR_STATIC_0(ErrorManagement::UnsupportedFeature, "BasicConsole: Function not implemented");

    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: console cannot seek. */
bool BasicConsole::Seek(const uint64 pos) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: console cannot seek. */
bool BasicConsole::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicConsole::Position() {
    return 0xffffffffffffffffu;
}

uint64 BasicConsole::Size() {
    return 0xffffffffffffffffu;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: size not defined for console. */
bool BasicConsole::SetSize(const uint64 size) {
    return false;
}

Handle BasicConsole::GetReadHandle() const {
    return BasicConsoleProperties::STDIN;
}

Handle BasicConsole::GetWriteHandle() const {
    return BasicConsoleProperties::STDOUT;
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

}
