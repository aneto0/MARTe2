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
#include "ff.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicFile.h"
#include "ErrorManagement.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BYTE MARTe2ToFatFsFlagConverter(uint32 marte2Flags) {
    /*
    * MARTe2 flags are
    * - ACCESS_MODE_R         = Read only mode
    * - ACCESS_MODE_W         = Write only mode
    * - FLAG_APPEND           = Write at the end of the file (append)
    * - FLAG_CREAT            = Write always (open or create)
    * - FLAG_TRUNC            = Truncate if exists
    * - FLAG_CREAT_EXCLUSIVE  = Create only if not exists
    */

   BYTE fatfsFlags = 0;

   if((marte2Flags & MARTe::BasicFile::ACCESS_MODE_R) != 0u) {
       fatfsFlags |= FA_READ;
   }

   if((marte2Flags & MARTe::BasicFile::ACCESS_MODE_W) != 0u) {
       fatfsFlags |= FA_WRITE;

       if((marte2Flags & MARTe::BasicFile::FLAG_APPEND) != 0u) {
           fatfsFlags |= FA_OPEN_APPEND;
       }
       if((marte2Flags & MARTe::BasicFile::FLAG_CREAT) != 0u) {
           fatfsFlags |= FA_OPEN_ALWAYS;
       }
       if((marte2Flags & MARTe::BasicFile::FLAG_TRUNC) != 0u) {
           fatfsFlags |= FA_CREATE_ALWAYS;
       }
       if((marte2Flags & MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE) != 0u) {
           fatfsFlags |= FA_CREATE_NEW;
       }
   }

    return fatfsFlags;
}

uint32 FatFsToMARTe2FlagConverter(BYTE fatfsFlags) {
    uint32 marte2Flags = 0u;
    
    if((fatfsFlags & FA_READ) != 0) {
        marte2Flags |= MARTe::BasicFile::ACCESS_MODE_R;
    }

    if((fatfsFlags & FA_WRITE) != 0) {
        marte2Flags |= MARTe::BasicFile::ACCESS_MODE_W;

        if((fatfsFlags & FA_OPEN_APPEND) != 0) {
            marte2Flags |= MARTe::BasicFile::FLAG_APPEND;
        }
        if((fatfsFlags & FA_OPEN_ALWAYS) != 0) {
            marte2Flags |= MARTe::BasicFile::FLAG_CREAT;
        }
        if((fatfsFlags & FA_CREATE_ALWAYS) != 0) {
            marte2Flags |= MARTe::BasicFile::FLAG_TRUNC;
        }
        if((fatfsFlags & FA_CREATE_NEW) != 0) {
            marte2Flags |= MARTe::BasicFile::FLAG_CREAT_EXCLUSIVE;
        }
    }

    return marte2Flags;
}

BasicFile::BasicFile() :
        StreamI(),
        HandleI() {
    
    properties.isOpen = false;
    properties.canRead = false;
    properties.canWrite = false;
    properties.canSeek = false;

    properties.marte2FileProperties = 0u;
    properties.filePath = "";
}

BasicFile::BasicFile(const BasicFile & bf) :
        /*lint -e{1738} THe StreamI does not have a copy constructor*/
        StreamI::StreamI(),
        HandleI::HandleI() {

    REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Warning, file descriptor duplication may lead to erratic behaviour");
    REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "DUP is not supported by FatFs module");
    properties = bf.properties;
}

BasicFile & BasicFile::operator=(const BasicFile &bf) {
    Handle handle;

    if (&bf != this) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Warning, file descriptor duplication may lead to erratic behaviour");
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "DUP is not supported by FatFs module");

        properties = bf.properties;
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

    if (!IsOpen()) {
        properties.marte2FileProperties = setFlags;
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicFile::SetFlags() Only FLAG_APPEND can be changed if file is already opened.");
        retVal = false;
    }

    return retVal;
}

uint32 BasicFile::GetFlags() const {
    return properties.marte2FileProperties;
}

bool BasicFile::CanWrite() const {
    return properties.canWrite;
}

bool BasicFile::CanRead() const {
    return properties.canRead;
}

bool BasicFile::CanSeek() const {
    return properties.canSeek;
}

bool BasicFile::Open(const char8 * const pathname,
                     const uint32 flags) {
    bool retVal = true;
    StreamString ss;
    
    if (!IsOpen()) {
        BYTE fatfsFlag = MARTe2ToFatFsFlagConverter(flags);
        FRESULT openResult = f_open(&properties.fileHandle, pathname, fatfsFlag);

        if(openResult == FR_OK) {
            properties.marte2FileProperties = flags;
            properties.filePath = pathname;
            
            properties.isOpen = true;
            properties.canSeek = true;
            properties.canRead = ((flags & ACCESS_MODE_R) != 0u);
            properties.canWrite = ((flags & ACCESS_MODE_W) != 0u);
        }
        else {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Open(). File open error");   
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Open(). File is already opened");
    }
    return retVal;
}

bool BasicFile::IsOpen() const {
    return (properties.isOpen);
}

bool BasicFile::Close() {
    bool retVal = true;

    if (IsOpen()) {
        properties.canRead = false;
        properties.canWrite = false;
        properties.canSeek = false;
        properties.isOpen = false;
        properties.marte2FileProperties = 0u;
        properties.filePath = "";

        FRESULT closeResult = f_close(&properties.fileHandle);
        if(closeResult != FR_OK) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Open(). File close error");
            retVal = false;
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::Information, "BasicFile::Close(). Cannot close as the file is not open");
    }

    return retVal;
}

bool BasicFile::Read(char8* const output,
                     uint32 & size) {
    bool retVal = true;
    UINT effectiveRead = 0u;

    if (CanRead()) {    
        FRESULT readResult = f_read(&properties.fileHandle, static_cast<void*>(output), static_cast<UINT>(size), &effectiveRead);
       
        if(readResult != FR_OK) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Read(). Read failure");
            retVal = false;
            effectiveRead = 0u;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Read(). Requesting read on non-readable file");
    }

    size = static_cast<uint32>(effectiveRead);

    return retVal;
}

bool BasicFile::Read(char8 * const output,
                     uint32 & size,
                     const TimeoutType &msecTimeout) {
    
    //TODO Handle reading timeout
    bool retVal = BasicFile::Read(output, size);
    
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size) {
    bool retVal = true;

    if (CanWrite()) {
        UINT effectiveWrite;
        FRESULT writeResult = f_write(&properties.fileHandle, input, static_cast<UINT>(size), &effectiveWrite);

        if(writeResult != FR_OK) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Write(). Write failure");
            properties.canWrite = false;
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Write(). Requesting write on non-writable file");
    }
    return retVal;
}

bool BasicFile::Write(const char8 * const input,
                      uint32 & size,
                      const TimeoutType &msecTimeout) {
    
    REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicFile::Read(). Timeout on write not supported");
    return BasicFile::Write(input, size);
}

uint64 BasicFile::Size() {
    uint64 fileByteSize = 0u;

    if (IsOpen()) {
        fileByteSize = static_cast<uint64>(f_size(&properties.fileHandle));
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Size(). Requesting size() on a non-opened file");
    }

    return fileByteSize;
}

bool BasicFile::Seek(const uint64 pos) {
    int64 retSeek;
    bool retVal = true;

    if (CanSeek()) {
        FRESULT seekResult = f_lseek(&properties.fileHandle, static_cast<FSIZE_t>(pos));

        if(seekResult != FR_OK) {
            retVal = false;
            properties.canSeek = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Seek(). Failure during seek");
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::Seek(). Requesting seek() on a non-seekable file");
    }
    return retVal;
}

bool BasicFile::RelativeSeek(const int64 deltaPos) {
    bool retVal = true;

    if (CanSeek()) {
        FSIZE_t position = f_tell(&properties.fileHandle);
        
        if(static_cast<int32>(deltaPos + position) < 0) {
            position = 0u;
        }
        else {
            position += static_cast<FSIZE_t>(deltaPos);
        }
        
        FRESULT relSeekResult = f_lseek(&properties.fileHandle, position);
        
        if(relSeekResult != FR_OK) {
            retVal = false;
            properties.canSeek = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::RelativeSeek(). Failure in relative seek");
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::RelativeSeek(). Requesting seek on a non-seekable file");
    }

    return retVal;
}

uint64 BasicFile::Position() {
    uint64 currentPosition = 0u;

    if (IsOpen()) {
        currentPosition = static_cast<uint64>(f_tell(&properties.fileHandle));
    }

    return currentPosition;
}

bool BasicFile::SetSize(const uint64 size) {
    bool retVal = true;
    
    if (IsOpen() && CanWrite()) {
        FRESULT operationResult;

        FSIZE_t currentSize = f_size(&properties.fileHandle);

        if(size > currentSize) {
            //We are going to expand the size, using lseek beyond
            operationResult = f_lseek(&properties.fileHandle, size);
            if(operationResult != FR_OK) {
                retVal = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). File set size failed during lseek");
            }
        }
        else if (size < currentSize) {
            //First we need to move the file pointer to the truncation point
            operationResult = f_lseek(&properties.fileHandle, size);

            if(operationResult == FR_OK) {
                //Then we are going to reduce size, using truncation
                operationResult = f_truncate(&properties.fileHandle);
                if(operationResult != FR_OK) {
                    retVal = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). File set size failed during truncation");
                }
            }
            else {
                retVal = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). File set size failed during lseek");
            }
        }
        else { /* Size remained the same as it already was */
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "BasicFile::SetSize(). Specified set size matches actual");
        }
    }
    else {
        retVal = false;
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "BasicFile::SetSize(). Setting size on a non-opened or non-writeable file.");
    }

    return retVal;
}

StreamString BasicFile::GetPathName() const {
    return properties.filePath;
}

Handle BasicFile::GetReadHandle() const {
    //TODO: support them better
    return 0u;
}

Handle BasicFile::GetWriteHandle() const {
    //TODO: support them better
    return 0u;
}
}

