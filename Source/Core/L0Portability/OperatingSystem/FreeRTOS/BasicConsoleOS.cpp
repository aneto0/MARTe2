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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct BasicConsoleOSProperties {

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicConsole::BasicConsole() {

}

BasicConsole::~BasicConsole() {

}

ErrorType BasicConsole::Open(const FlagsType &mode) {
    return UnsupportedFeature;
}

FlagsType BasicConsole::GetOpeningMode() const {
    return FlagsType(0);
}

ErrorType BasicConsole::Close() {
    return UnsupportedFeature;
}

ErrorType BasicConsole::Write(const char8 * const buffer,
                              uint32 & size,
                              const TimeoutType &timeout) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::OSWrite(const char8* const buffer,
                                uint32 &size,
                                const TimeoutType &timeout) {
    return UnsupportedFeature;
}

/*lint -e{715} timeout is not used...*/
ErrorType BasicConsole::Read(char8 * const buffer,
                             uint32 & size,
                             const TimeoutType &timeout) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::SetSize(const uint32 &numberOfColumns,
                                const uint32 &numberOfRows) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::GetSize(uint32 &numberOfColumns,
                                uint32 &numberOfRows) const {
    return UnsupportedFeature;
}

ErrorType BasicConsole::Clear() {
    return UnsupportedFeature;
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

ErrorType BasicConsole::ShowBuffer() {
    return UnsupportedFeature;
}

ErrorType BasicConsole::SetColour(const Colours &foregroundColour,
                                  const Colours &backgroundColour) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::SetTitleBar(const char8 * const title) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::GetTitleBar(char8 * const title,
                                    const uint32 &size) const {
    return UnsupportedFeature;
}

ErrorType BasicConsole::SetCursorPosition(const uint32 &column,
                                          const uint32 &row) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::GetCursorPosition(uint32 &column,
                                          uint32 &row) const {
    return UnsupportedFeature;
}

ErrorType BasicConsole::SetWindowSize(const uint32 &numberOfColumns,
                                      const uint32 &numberOfRows) {
    return UnsupportedFeature;
}

ErrorType BasicConsole::GetWindowSize(uint32 &numberOfColumns,
                                      uint32 &numberOfRows) const {
    return UnsupportedFeature;
}

ErrorType BasicConsole::PlotChar(const char8 &c,
                                 const Colours &foregroundColour,
                                 const Colours &backgroundColour,
                                 const uint32 &column,
                                 const uint32 &row) {
    return UnsupportedFeature;
}
