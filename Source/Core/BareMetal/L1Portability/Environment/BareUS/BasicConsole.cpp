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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
    struct ConsoleHandle {
        int dummyVal;
    };
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



namespace MARTe {

//TODO: Check if a console abstraction is available at Xilinx lib level

BasicConsole::BasicConsole() :
        StreamI(),
        HandleI() {
}

BasicConsole::~BasicConsole() {
}

bool BasicConsole::Open(const FlagsType &mode) {
    //Once initialized, it is already opened
    return true;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return BasicConsoleMode::Default;
}

bool BasicConsole::Close() { 
    return true;
}

bool BasicConsole::Read(char8 * const output,
                        uint32 & size,
                        const TimeoutType &timeout) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicConsole::Read() Cannot read from this console");
    return false;
}

bool BasicConsole::Write(const char8 * const input,
                         uint32 & size,
                         const TimeoutType &timeout) {
    return false;
    //return OSWrite(input, size, TTInfiniteWait);
}

bool BasicConsole::Write(const char8 * const input,
                         uint32 & size) {
    return false;
    //return OSWrite(input, size, TTInfiniteWait);
}

bool BasicConsole::OSWrite(const char8* const buffer,
                           uint32 &size,
                           const TimeoutType &timeout) {
    return false;
}

bool BasicConsole::Read(char8 * const output,
                        uint32 & size) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicConsole::Read() Read function not supported");
    return false;
}

bool BasicConsole::SetSceneSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicConsole::SetSceneSize() Set scene size not supported, autodetection of resolution already took place");
    return false;
}

bool BasicConsole::GetSceneSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {
    return false;
}

bool BasicConsole::Clear() {
    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicConsole::Clear() Clear not supported");
    return false;
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

bool BasicConsole::ShowBuffer() {
    return false;
}

/*lint -e{715} function not implemented in Linux*/
bool BasicConsole::SetColour(const Colours &foregroundColour,
                             const Colours &backgroundColour) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::SetTitleBar(const char8 * const title) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::GetTitleBar(char8 * const title,
                               const uint32 &size) const {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::SetCursorPosition(const uint32 &column,
                                     const uint32 &row) {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::GetCursorPosition(uint32 &column,
                                     uint32 &row) const {
    return false;
}

bool BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                 const uint32 &numberOfRows) {
    return false;
}

bool BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                 uint32 &numberOfRows) const {
    return false;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: Not implemented. */
bool BasicConsole::PlotChar(const char8 &c,
                            const Colours &foregroundColour,
                            const Colours &backgroundColour,
                            const uint32 &column,
                            const uint32 &row) {
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
    return NULL;
}

Handle BasicConsole::GetWriteHandle() const {
    return NULL;
}

bool BasicConsole::CanWrite() const {
    return true;
}

bool BasicConsole::CanRead() const {
    return false;
}

bool BasicConsole::CanSeek() const {
    return false;
}

}
