/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
 * @date 26/10/2015
 * @author Llorenç Capellà
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

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicFile.h"
#include "ErrorManagement.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {
static MARTe::int32 INVALID_FD = -1;

MARTe::uint32 ConvertToLinuxFlags(const MARTe::uint32 flags) {
    MARTe::uint32 linuxFlags = static_cast<MARTe::uint32>(0);
    if ((flags & (MARTe::BasicFile::ACCESS_MODE_R | MARTe::BasicFile::ACCESS_MODE_W)) == (MARTe::BasicFile::ACCESS_MODE_R | MARTe::BasicFile::ACCESS_MODE_W)) {
        linuxFlags |= static_cast<MARTe::uint32>(O_RDWR);
    }
    else if ((flags & MARTe::BasicFile::ACCESS_MODE_W) == MARTe::BasicFile::ACCESS_MODE_W) {
        linuxFlags |= static_cast<MARTe::uint32>(O_WRONLY);
    }
    else {
        //O_RDONLY is a 0, then at the end no operation is needed
        //linuxFlags |= static_cast<MARTe::uint32>(O_RDONLY)
    }
    if ((flags & MARTe::BasicFile::FLAG_APPEND) == MARTe::BasicFile::FLAG_APPEND) {
        linuxFlags |= static_cast<MARTe::uint32>(O_APPEND);
    }
    if ((flags & MARTe::BasicFile::FLAG_CREAT) == MARTe::BasicFile::FLAG_CREAT) {
        linuxFlags |= static_cast<MARTe::uint32>(O_CREAT);
    }
    if ((flags & MARTe::BasicFile::FLAG_TRUNC) == MARTe::BasicFile::FLAG_TRUNC) {
        linuxFlags |= static_cast<MARTe::uint32>(O_TRUNC);
    }
    if ((flags & MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE) == MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE) {
        linuxFlags |= static_cast<MARTe::uint32>(O_EXCL);
        linuxFlags |= static_cast<MARTe::uint32>(O_CREAT);
    }
    return linuxFlags;
}

MARTe::uint32 ConvertToBasicFileFlags(const MARTe::uint32 flags) {
    MARTe::uint32 BasicFileFlags = (0u);
    if ((flags & static_cast<MARTe::uint32>(O_RDWR)) == static_cast<MARTe::uint32>(O_RDWR)) {
        BasicFileFlags |= MARTe::BasicFile::ACCESS_MODE_R;
        BasicFileFlags |= MARTe::BasicFile::ACCESS_MODE_W;
    }
    else if ((flags & static_cast<MARTe::uint32>(O_WRONLY)) == static_cast<MARTe::uint32>(O_WRONLY)) {
        BasicFileFlags |= MARTe::BasicFile::ACCESS_MODE_W;
    }
    else {
        BasicFileFlags |= MARTe::BasicFile::ACCESS_MODE_R;
    }
    if ((flags & static_cast<MARTe::uint32>(O_APPEND)) == static_cast<MARTe::uint32>(O_APPEND)) {
        BasicFileFlags |= MARTe::BasicFile::FLAG_APPEND;
    }
    if ((flags & static_cast<MARTe::uint32>(O_CREAT)) == static_cast<MARTe::uint32>(O_CREAT)) {
        BasicFileFlags |= MARTe::BasicFile::FLAG_CREAT;
    }
    if ((flags & static_cast<MARTe::uint32>(O_TRUNC)) == static_cast<MARTe::uint32>(O_TRUNC)) {
        BasicFileFlags |= MARTe::BasicFile::FLAG_TRUNC;
    }
    if ((flags & static_cast<MARTe::uint32>(O_EXCL)) == static_cast<MARTe::uint32>(O_EXCL)) {
        BasicFileFlags |= MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE;
    }
    return BasicFileFlags;
}

MARTe::uint32 CheckFlags(MARTe::uint32 flags) {

    //If APPEND and Read mode are set at the same time the APPEND mode is deleted from the flags
    if ((flags & (MARTe::BasicFile::FLAG_APPEND | MARTe::BasicFile::ACCESS_MODE_R)) == (MARTe::BasicFile::FLAG_APPEND | MARTe::BasicFile::ACCESS_MODE_R)) {
        if ((flags & MARTe::BasicFile::ACCESS_MODE_W) != (MARTe::BasicFile::ACCESS_MODE_W)) {
            MARTe::uint32 clean_APPEND = ~MARTe::BasicFile::FLAG_APPEND;
            flags &= clean_APPEND;
        }
    }

    if ((flags & (MARTe::BasicFile::FLAG_CREAT | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE))
            == (MARTe::BasicFile::FLAG_CREAT | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE)) {
        MARTe::uint32 clean_CREAT_EXCLUSIVE = ~MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE;
        flags &= clean_CREAT_EXCLUSIVE;
    }
    if ((flags & (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::ACCESS_MODE_R)) == (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::ACCESS_MODE_R)) {
        if ((flags & MARTe::BasicFile::ACCESS_MODE_W) != (MARTe::BasicFile::ACCESS_MODE_W)) {
            MARTe::uint32 clean_TRUNC = ~MARTe::BasicFile::FLAG_TRUNC;
            flags &= clean_TRUNC;
        }
    }
    if ((flags & (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE))
            == (MARTe::BasicFile::FLAG_TRUNC | MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE)) {
        MARTe::uint32 clean_TRUNC = ~MARTe::BasicFile::FLAG_TRUNC;
        flags &= clean_TRUNC;
    }
    return flags;
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicFile::BasicFile() :
        StreamI(),
        HandleI() {
    properties.identifier = INVALID_FD;
    properties.pathName = "";
}

BasicFile::BasicFile(const BasicFile & bf) :
        /*lint -e{1738} THe StreamI does not have a copy constructor*/
        StreamI::StreamI(),
        HandleI::HandleI() {
    Handle handle;
    bool ok = true;
    if (bf.properties.identifier == INVALID_FD) {
        handle = INVALID_FD;
    }
    else {
        handle = dup(bf.properties.identifier);
        ok = (handle >= 0);
        if (!ok) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicFile::BasicFile(bf). Error::dup");
        }
    }
    if (ok) {
        properties.identifier = handle;
        properties.saveFlags = bf.properties.saveFlags;
        properties.pathName = bf.properties.pathName;
    }
    else {
        properties.identifier = INVALID_FD;
        properties.saveFlags = 0u;
        properties.pathName = "";
    }
}

BasicFile & BasicFile::operator=(const BasicFile &bf) {
    Handle handle;
    bool ok = true;
    if (&bf != this) {

        if (bf.properties.identifier == INVALID_FD) {
            handle = INVALID_FD;
        }
        else {
            handle = dup(bf.properties.identifier);
            ok = (handle >= 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicFile::BasicFile(bf). Error::dup");
            }
        }
        if (ok) {
            if (properties.identifier != INVALID_FD) {
                ok = static_cast<bool>(close(properties.identifier));
                if (!ok) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "BasicFile::BasicFile assignment. Error::close()");
                }
            }
            properties.identifier = handle;
            properties.saveFlags = bf.properties.saveFlags;
            properties.pathName = bf.properties.pathName;
        }
    }
    return *this;
}

BasicFile::~BasicFile() {
    /*lint -e{1551} Function may throw exceptions*/
    if (IsOpen()) {
        /*lint -e{534} Ignoring the return value of the function*/
        Close();
    }
}
/*lint -e{1762} The function changes the  status of the OS structure*/
bool BasicFile::SetFlags(const uint32 setFlags) {
    bool retVal = true;

    if (IsOpen()) {
        uint32 linuxFlags = ConvertToLinuxFlags(setFlags);
        uint32 clear = ~(static_cast<uint32>(O_APPEND));
        properties.saveFlags &= clear; //remove the APPEND. It is the only changed flag.
        properties.saveFlags |= linuxFlags; // Add the new flag to the existing saveFlags.
        uint32 retFcntl = static_cast<uint32>(fcntl(properties.identifier, F_SETFL, properties.saveFlags));
        if ((retFcntl & properties.saveFlags) != properties.saveFlags) {
            retVal = false;
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

uint32 BasicFile::GetFlags() const {
    uint32 retVal = 0u;
    if (IsOpen()) {
        retVal = ConvertToBasicFileFlags(properties.saveFlags);
    }
    return retVal;
}

bool BasicFile::CanWrite() const {
    bool retVal = true;
    if (!IsOpen()) {
        retVal = false;
    }
    else {
        uint32 flags = static_cast<uint32>(fcntl(properties.identifier, F_GETFL));
        retVal = (((flags & static_cast<uint32>(O_RDWR)) == static_cast<uint32>(O_RDWR))
                || ((flags & static_cast<uint32>(O_WRONLY)) == static_cast<uint32>(O_WRONLY)));
    }
    return retVal;
}

bool BasicFile::CanRead() const {
    bool retVal = true;
    if (!IsOpen()) {
        retVal = false;
    }
    else {
        uint32 flags = static_cast<uint32>(fcntl(properties.identifier, F_GETFL));
        retVal = (((flags & static_cast<uint32>(O_RDWR)) == static_cast<uint32>(O_RDWR))
                || ((flags & static_cast<uint32>(O_WRONLY)) == static_cast<uint32>(O_RDONLY)));
    }
    return retVal;
}

bool BasicFile::CanSeek() const {
    return IsOpen();
}

bool BasicFile::Open(const char8 * const pathname,
                     const uint32 flags) {
    bool retVal = true;

    if (!IsOpen()) {

        uint32 flagsChecked = CheckFlags(flags);
        uint32 linuxFlags = ConvertToLinuxFlags(flagsChecked);
        /*lint -e{9130} Signed value*/
        properties.identifier = open(pathname, static_cast<int32>(linuxFlags), (S_IRWXU | S_IRWXG | S_IRWXO));
        if (!IsOpen()) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Open(). File cannot be opened");
            retVal = false;
        }
        else {
            properties.pathName = pathname;
            properties.saveFlags = linuxFlags;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicFile::Open(). File is already opened");
    }
    return retVal;
}

bool BasicFile::IsOpen() const {
    return (properties.identifier >= 0);
}

bool BasicFile::Close() {
    bool retVal = true;
    if (IsOpen()) {
        int32 retClose = close(properties.identifier);
        if (retClose == -1) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Close().File cannot be closed");
            retVal = false;
        }
        else {
            properties.identifier = INVALID_FD;
            properties.saveFlags = 0U;
            properties.pathName = "";
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "BasicFile::Close().The file is not open");
    }
    return retVal;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    bool retVal = true;
    if (CanRead()) {
        size = static_cast<uint32>(read(properties.identifier, output, static_cast<size_t>(size)));
        retVal = (size != 0xFFFFFFFFU);
        if (retVal == false) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Read().File cannot be read");
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const TimeoutType &msecTimeout) {
    bool retVal;
    fd_set set1;
    struct timeval timeout;
    int32 retSelect;
    if (CanRead()) {
        /*lint -e{529} symbol subsequently not used*/
        /*lint -e{1960} violates MISRA 17-0-2*/
        /*lint -e{970} use modifier or type int outside of a typedef [MISRA C++ Rule 3-9-2]*/
        /*lint -e{717} while(0)*/
        /*lint -e{9146} multiple declaration in a declaration*/
        /*lint -e{909} Implicit conversion*/
        FD_ZERO(&set1);
        /*lint -e{530} Symbol not initialized*/
        /*lint -e{9130} bitwise operator applied to signed underlying type*/
        /*lint -e{970} use modifier or type int outside of a typedef [MISRA C++ Rule 3-9-2]*/
        /*lint -e{703} Shift left of a signed quantity*/
        /*lint -e{1924} C-style cast*/
        FD_SET(properties.identifier, &set1);
        /*lint -e{9114} implicit conversion of integer cvalue expression*/
        uint32 secs = msecTimeout.GetTimeoutMSec() / 1000u;
        uint32 usecs = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
        timeout.tv_sec = static_cast<int64>(secs);
        timeout.tv_usec = static_cast<int64>(usecs);
        retSelect = select((properties.identifier + 1), &set1, static_cast<fd_set *>(NULL), static_cast<fd_set *>(NULL), &timeout);
        if (retSelect == -1) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Read(). Error while waiting to read a file");
            retVal = false;
        }
        else if (retSelect == 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicFile::Read(). Timeout file not read");
            retVal = false;
        }
        else {
            retVal = BasicFile::Read(output, size);
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    bool retVal = true;
    if (CanWrite()) {
        size = static_cast<uint32>(write(properties.identifier, input, static_cast<size_t>(size)));
        if (size == 0xFFFFFFFFU) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Write(). File cannot be written");
            retVal = false;
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const TimeoutType &msecTimeout) {
    bool retVal;
    fd_set set;
    struct timeval timeout;
    int32 retSelect;
    if (CanWrite()) {
        /*lint -e{529} symbol subsequently not used*/
        /*lint -e{1960} violates MISRA 17-0-2*/
        /*lint -e{970} use modifier or type int outside of a typedef [MISRA C++ Rule 3-9-2]*/
        /*lint -e{717} while(0)*/
        /*lint -e{9146} multiple declaration in a declaration*/
        /*lint -e{909} Implicit conversion*/
        FD_ZERO(&set);
        /*lint -e{530} Symbol not initialized*/
        /*lint -e{9130} bitwise operator applied to signed underlying type*/
        /*lint -e{970} use modifier or type int outside of a typedef [MISRA C++ Rule 3-9-2]*/
        /*lint -e{703} Shift left of a signed quantity*/
        /*lint -e{1924} C-style cast*/
        FD_SET(properties.identifier, &set);
        /*lint -e{9114} implicit conversion of integer cvalue expression*/
        uint32 secs = msecTimeout.GetTimeoutMSec() / 1000u;
        uint32 usecs = (msecTimeout.GetTimeoutMSec() % 1000u) * 1000u;
        timeout.tv_sec = static_cast<int64>(secs);
        timeout.tv_usec = static_cast<int64>(usecs);
        retSelect = select(properties.identifier + 1, static_cast<fd_set *>(NULL), &set, static_cast<fd_set *>(NULL), &timeout);
        if (retSelect == -1) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Write. Error while waiting to write a file");
            retVal = false;
        }
        else if (retSelect == 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Timeout, "BasicFile::Write(). Timeout file not written");
            retVal = false;
        }
        else {
            retVal = BasicFile::Write(input, size);
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

uint64 BasicFile::Size() {
    uint64 numberOfCharactersFile = 0U;
    if (IsOpen()) {
        struct stat statusFile;
        int32 retFstat = fstat(properties.identifier, &statusFile);
        if (retFstat < 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Size(). Error while reading size");
            numberOfCharactersFile = 0xFFFFFFFFU;
        }
        else {
            numberOfCharactersFile = static_cast<uint64>(statusFile.st_size);
        }
    }
    return numberOfCharactersFile;
}

bool BasicFile::Seek(const uint64 pos) {
    int64 retSeek;
    bool retVal = true;
    if (CanSeek()) {
        //The offset of lseek is long int which is 8 signed bit in 64bit architecture.
        if (pos > static_cast<uint64>(MAX_INT64)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Seek(). Too large position");
            retVal = false;
        }
        else {
            int64 localPos;
            uint64 size = BasicFile::Size();
            if (size < pos) {
                localPos = static_cast<int64>(size);
            }
            else {
                localPos = static_cast<int64>(pos);
            }
            retSeek = lseek64(properties.identifier, localPos, SEEK_SET);
            if (retSeek < 0) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Seek(). The position cannot be set");
                retVal = false;
            }
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool BasicFile::RelativeSeek(const int64 deltaPos) {
    int64 retSeek;
    bool retVal = true;
    if (CanSeek()) {
        int64 localPos;
        int64 size = static_cast<int64>(BasicFile::Size());
        int64 position = static_cast<int64>(Position());
        if ((deltaPos + position) > size) {
            localPos = size;
        }
        else if (static_cast<int32>(deltaPos + position) < 0) {
            localPos = 0;
        }
        else {
            localPos = position + deltaPos;
        }
        retSeek = lseek(properties.identifier, localPos, SEEK_SET);
        if (retSeek < 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::RelativeSeek(). The position cannot be set");
            retVal = false;
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

uint64 BasicFile::Position() {
    int64 pos = 0;
    if (IsOpen()) {
        pos = lseek(properties.identifier, static_cast<int64>(0), SEEK_CUR);
        if (pos < 0) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Position(). The position cannot be read");
        }
    }
    return (static_cast<uint64>(pos));
}

bool BasicFile::SetSize(const uint64 size) {
    bool retVal = true;
    int32 ret = -1;
    if (IsOpen()) {
        if (size >= static_cast<uint64>(MAX_INT64)) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). The size is too large");
        }
        ret = ftruncate(properties.identifier, static_cast<off_t>(size));
        if (ret == -1) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). The size cannot be set");
            retVal = false;
        }
        //Update the new position if it is pointing outside of the size
        if (Position() > size) {
            if (!Seek(size)) {
                retVal = false;
            }
        }
    }
    else {
        retVal = false;
    }
    return retVal;
}

StreamString BasicFile::GetPathName() const {
    return properties.pathName;
}

Handle BasicFile::GetReadHandle() const {
    return properties.identifier;
}

Handle BasicFile::GetWriteHandle() const {
    return properties.identifier;
}
}

