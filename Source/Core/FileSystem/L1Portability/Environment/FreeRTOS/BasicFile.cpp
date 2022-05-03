/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
 * @date 20/04/2019
 * @author Andre Neto
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
 * the class BasicFile (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicFile.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicFile::BasicFile() :
        StreamI(),
        HandleI() {
}

BasicFile::BasicFile(const BasicFile & bf) :
        /*lint -e{1738} THe StreamI does not have a copy constructor*/
        StreamI::StreamI(),
        HandleI::HandleI() {
}

BasicFile & BasicFile::operator=(const BasicFile &bf) {
    Handle handle;
    bool ok = true;
    if (&bf != this) {
    }
    return *this;
}

BasicFile::~BasicFile() {
}

bool BasicFile::SetFlags(const uint32 setFlags) {
    return false;
}

uint32 BasicFile::GetFlags() const {
    uint32 retVal = 0u;
    return retVal;
}

bool BasicFile::CanWrite() const {
    return false;
}

bool BasicFile::CanRead() const {
    return false;
}

bool BasicFile::CanSeek() const {
    return false;
}

bool BasicFile::Open(const char8 * const pathname,
                     const uint32 flags) {
    return false;
}

bool BasicFile::IsOpen() const {
    return false;
}

bool BasicFile::Close() {
    return false;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    return false;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const TimeoutType &msecTimeout) {
    return false;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    return false;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const TimeoutType &msecTimeout) {
    return false;
}

uint64 BasicFile::Size() {
    return 0LLU;
}

bool BasicFile::Seek(const uint64 pos) {
    return false;
}

bool BasicFile::RelativeSeek(const int64 deltaPos) {
    return false;
}

uint64 BasicFile::Position() {
    return 0LLU;
}

bool BasicFile::SetSize(const uint64 size) {
    return false;
}

StreamString BasicFile::GetPathName() const {
    return "";
}

Handle BasicFile::GetReadHandle() const {
    return 0;
}

Handle BasicFile::GetWriteHandle() const {
    return 0;
}
}

