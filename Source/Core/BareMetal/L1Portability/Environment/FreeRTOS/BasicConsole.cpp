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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/**
 * TODO. NOT COMPLETE
 */

BasicConsole::BasicConsole() :
        StreamI(),
        HandleI() {
}

BasicConsole::~BasicConsole() {
}

bool BasicConsole::ColourSupported() const {
    return false;
}
bool BasicConsole::SetColour(const MARTe::Colours &foregroundColour, const MARTe::Colours &backgroundColour) {
    return false;
}
bool BasicConsole::Write(const MARTe::char8 * const buffer, MARTe::uint32 & size, const MARTe::TimeoutType &timeout) {
    return false;
}
bool BasicConsole::Open(const MARTe::FlagsType &mode) {
    return false;
}
bool BasicConsole::Clear() {
    return false;
}

Handle BasicConsole::GetReadHandle() const {
    return 0;
}
Handle BasicConsole::GetWriteHandle() const {
    return 0;
}
FlagsType BasicConsole::GetOpeningMode() const {
    return FlagsType(0);
}
bool BasicConsole::Close() {
    return false;
}
bool BasicConsole::SetSceneSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    return false;
}
bool BasicConsole::GetSceneSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    return false;
}
bool BasicConsole::SetWindowSize(const uint32 &numberOfColumns, const uint32 &numberOfRows) {
    return false;
}
bool BasicConsole::GetWindowSize(uint32 &numberOfColumns, uint32 &numberOfRows) const {
    return false;
}
bool BasicConsole::WindowSizeSupported() const {
    return false;
}
bool BasicConsole::SetCursorPosition(const uint32 &column, const uint32 &row) {
    return false;
}
bool BasicConsole::GetCursorPosition(uint32 & column, uint32 & row) const {
    return false;
}
bool BasicConsole::CursorPositionSupported() const {
    return false;
}
bool BasicConsole::ShowBuffer() {
    return false;
}
bool BasicConsole::ConsoleBufferSupported() const {
    return false;
}
bool BasicConsole::PlotChar(const char8 &c, const Colours &foregroundColour, const Colours &backgroundColour, const uint32 &column, const uint32 &row) {
    return false;
}
bool BasicConsole::SetTitleBar(const char8 *title) {
    return false;
}
bool BasicConsole::GetTitleBar(char8 *title, const uint32 &size) const {
    return false;
}
bool BasicConsole::TitleBarSupported() const {
    return false;
}
bool BasicConsole::TimeoutSupported() const {
    return false;
}
bool BasicConsole::CanWrite() const {
    return false;
}
bool BasicConsole::CanRead() const {
    return false;
}
bool BasicConsole::CanSeek() const {
    return false;
}
bool BasicConsole::Read(char8 * const output, uint32 & size) {
    return false;
}
bool BasicConsole::Write(const char8 * const input, uint32 & size) {
    return Write(input, size, TTInfiniteWait);
}
bool BasicConsole::Read(char8 * const output, uint32 & size, const TimeoutType &timeout) {
    return false;
}
uint64 BasicConsole::Size() {
    return -1;
}
bool BasicConsole::Seek(uint64 pos) {
    return false;
}
bool BasicConsole::RelativeSeek(const int64 deltaPos) {
    return false;
}
uint64 BasicConsole::Position() {
    return -1;
}
bool BasicConsole::SetSize(uint64 size) {
    return false;
}

}
