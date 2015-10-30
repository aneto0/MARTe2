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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::uint32 ConvertToLinuxFlags(const MARTe::uint32 flags){
    MARTe::uint32 linuxFlags = 0;
    if((flags & MARTe::ACCESS_MODE_R) == MARTe::ACCESS_MODE_R){
        linuxFlags |= O_RDONLY;
    }else if ((flags & MARTe::ACCESS_MODE_W) == MARTe::ACCESS_MODE_W){
        linuxFlags |= O_WRONLY;
    }else if ((flags & MARTe::ACCESS_MODE_R_W) == MARTe::ACCESS_MODE_R_W){
        linuxFlags |= O_RDWR;
    }else {
        MARTe::REPORT_ERROR(MARTe::ErrorManagement::InitialisationError, "BasicFile::ConvertToLinuxFlags(). Incorrect ACCESS_MODE");
    }
    if((flags & MARTe::FLAG_APPEND) == MARTe::FLAG_APPEND){
        linuxFlags |= O_APPEND;
    }else if ((flags & MARTe::FLAG_CREAT) == MARTe::FLAG_CREAT){
        linuxFlags |= O_CREAT;
    }else if ((flags & MARTe::FLAG_TRUNC) == MARTe::FLAG_TRUNC){
            linuxFlags |= O_TRUNC;
    }else if ((flags & MARTe::FLAG_EXCL) == MARTe::FLAG_EXCL){
        linuxFlags |= O_EXCL;
    }else{
        MARTe::REPORT_ERROR(MARTe::ErrorManagement::InitialisationError, "BasicFile::ConvertToLinuxFlags(). Incorrect FLAG_MODE");
    }
    return linuxFlags;
}

static MARTe::uint32 ConvertToBasicFileFlags(const MARTe::uint32 flags){
    MARTe::uint32 BasicFileFlags = 0;
    if((flags & O_RDONLY) == O_RDONLY){
        BasicFileFlags |= MARTe::ACCESS_MODE_R;
    }else if ((flags & O_WRONLY) == O_WRONLY){
        BasicFileFlags |= MARTe::ACCESS_MODE_W;
    }else if ((flags & O_RDWR) == O_RDWR){
        BasicFileFlags |= MARTe::ACCESS_MODE_R_W;
    }else {
        MARTe::REPORT_ERROR(MARTe::ErrorManagement::InitialisationError, "BasicFile::ConvertToLinuxFlags(). Incorrect ACCESS_MODE");
    }
    if((flags & O_APPEND) == O_APPEND){
        BasicFileFlags |= MARTe::FLAG_APPEND;
    }else if ((flags & O_CREAT) == O_CREAT){
        BasicFileFlags |= MARTe::FLAG_CREAT;
    }else if ((flags & O_TRUNC) == O_TRUNC){
        BasicFileFlags |= MARTe::FLAG_TRUNC;
    }else if ((flags & O_EXCL) == O_EXCL){
        BasicFileFlags |= MARTe::FLAG_EXCL;
    }else{
        MARTe::REPORT_ERROR(MARTe::ErrorManagement::InitialisationError, "BasicFile::ConvertToLinuxFlags(). Incorrect FLAG_MODE");
    }
    return BasicFileFlags;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicFile::BasicFile(){

}

void BasicFile::SetMode(const uint32 setMode) {
    mode = setMode;
    if (mode != ACCESS_MODE_W || mode != ACCESS_MODE_R || mode != ACCESS_MODE_R_W) {
        REPORT_ERROR(ErrorManagement::InitialisationError, "BasicFile::SetMode(). Incorrect mode");
    }
}

void BasicFile::SetFlags(const uint32 setFlags){
    //the MSB have information about which bits have to be cleaned.
    uint32 mask = setFlags >> 16;
    uint32 clean = ~mask;
    uint32 value = setFlags & 0xFFFF;
    //Set to 0 the bits that will be set again.
    flags &= clean;
    //Set the values given by the less significant bits.
    flags |= value;
}

uint32 BasicFile::getFlags(){
    uint32 retVal = 0xFFFFFFFF;
    if (fileProperties < 0){

    }else {
        retVal = fcntl(fileProperties, F_GETFL);
        retVal = ConvertToBasicFileFlags(retVal);
    }
    return retVal;
}

bool BasicFile::CanWrite() const{
    return ((((flags & 0xF) == ACCESS_MODE_W) || ((flags & 0xF) == ACCESS_MODE_R_W)) && (fileProperties > 0));
}

bool BasicFile::CanRead() const{
    return ((((flags & 0xF) == ACCESS_MODE_R) || ((flags & 0xF) == ACCESS_MODE_R_W)) && (fileProperties > 0));
}

bool BasicFile::CanSeek() const{
    return (fileProperties > 0);
}

bool BasicFile::Open(const char * pathname, const int32 flags) {
    uint32 linuxMode = 0;
    uint32 linuxFlags = 0;
    bool retVal;
    linuxFlags = ConvertToLinuxFlags(flags);
    fileProperties = open(pathname, linuxMode);
    retVal = (fileProperties > 0);
    if (retVal == false) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Open(). File cannot be opened");
    }
    return retVal;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    bool retVal;
    size = read(fileProperties, output, size);
    retVal = (size >= 0);
    if (retVal == false) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Read().File cannot be read");
    }
    return (retVal);
}

virtual bool BasicFile::Read(char8 * const output,
                             uint32 & size,
                             const TimeoutType &msecTimeout) {
    bool retVal;
    fd_set set;
    struct timeval timeout;
    int32 retSelect;
    FD_ZERO(&set);
    FD_SET(fileProperties, &set);
    timeout.tv_usec = msecTimeout.GetTimeoutMSec() * 1000;
    retSelect = select(fileProperties + 1, &set, NULL, NULL, &timeout);
    if (retSelect == -1) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Read(). Error while waiting to read a file");
        retVal = false;
    }
    else if (retSelect == 0) {
        REPORT_ERROR(ErrorManagement::Timeout, "BasicFile::Read(). Timeout file not read");
        retVal = false;
    }
    else {
        retVal = BasicFile::Read(output, size);
    }
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    bool retVal;
    size = write(fileProperties, input, size);
    retVal = (size >= 0);
    if (size == false) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Write(). File cannot be written");
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
    FD_ZERO(&set);
    FD_SET(fileProperties, &set);
    timeout.tv_usec = msecTimeout.GetTimeoutMSec() * 1000;
    retSelect = select(fileProperties + 1, NULL, &set, NULL, &timeout);
    if (retSelect == -1) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Wirte. Error while waiting to write a file");
        retVal = false;
    }
    else if (retSelect == 0) {
        REPORT_ERROR(ErrorManagement::Timeout, "BasicFile::Write(). Timeout file not written");
        retVal = false;
    }
    else {
        retVal = BasicFile::Write(input, size);
    }
    return retVal;
}

uint64 BasicFile::Size() {
    struct stat statusFile;
    int retFstat = -1;
    uint64 numberOfCharactersFile = -1;
    retFstat = fstat(fileProperties, &statusFile);
    if (retFstat < 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Size(). Error while reading size");
    }
    else {
        numberOfCharactersFile = statusFile.st_size;
    }
    return numberOfCharactersFile;
}

bool BasicFile::Seek(uint64 pos) {
    int64 retSeek;
    bool retVal = true;
    //The offset of lseek is long int which is 8 signed bit in 64bit architecture.
    if (pos > 9223372036854775807) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Seek(). Too large position");
        retVal = false;
    }
    else {
        retSeek = lseek64(fileProperties, pos, SEEK_SET);
        if (retSeek < 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Seek(). The position cannot be set");
            retVal = false;
        }
    }
    return retVal;
}

bool BasicFile::RelativeSeek(const int32 deltaPos) {
    int64 retSeek;
    bool retVal = true;

    retSeek = lseek(fileProperties, static_cast<int64>(deltaPos), SEEK_CUR);
    if (retSeek < 0) {
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::RelativeSeek(). The position cannot be set");
        retVal = false;
    }
    return retVal;
}

uint64 BasicFile::Position() {
    int64 pos = 0;
    pos = lseek(fileProperties, 0,SEEK_CUR);
    if (pos < 0){
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::Position(). The position cannot be read");
    }
    return (static_cast<uint64>(pos));
}

bool BasicFile::SetSize(uint64 size) {
    bool retVal = true;
    int32 ret = -1;
    if(size >= (1 << 64)){
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::SetSize(). The size is too large");
    }
    ret = ftruncate(fileProperties, static_cast<int64>(size));
    if(ret == -1){
        REPORT_ERROR(ErrorManagement::FatalError, "BasicFile::SetSize(). The size cannot be set");
        retVal = false;
    }
    return retVal;
}
}

