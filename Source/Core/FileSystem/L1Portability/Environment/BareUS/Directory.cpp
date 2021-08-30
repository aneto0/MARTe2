/**
 * @file Directory.cpp
 * @brief Source file for class Directory
 * @date 04/11/2015
 * @author Giuseppe Ferrò
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
 * the class Directory (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "ff.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Directory.h"
#include "HeapManager.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

uint32 SimpleStringCopy(char8* destination, char8* source, uint32 maxLength) {
    if((source != NULL) && (destination != NULL)) {
        bool foundEnd = false;
        uint32 i = 0;

        for(; (i < maxLength) && !foundEnd; i++) {
            destination[i] = source[i];
            foundEnd = (source[i] == '\0');
        }

        return i;
    }
    else {
        return 0u;
    }
}

bool MARTe2ToFatFsFileInfoConverter(DirectoryCore &directoryCoreIn, FILINFO &fileInfoOut) {
    bool retVal = MemoryOperationsHelper::Set(&fileInfoOut, 0, sizeof(FILINFO));

    if(retVal) {
        fileInfoOut.fsize = static_cast<FSIZE_t>(directoryCoreIn.size);
        WORD tempA = 0u;
        WORD tempB = 0u;
        WORD tempC = 0u;

        tempA = (((directoryCoreIn.dateYear - 1980) & 0xFE00) << 9);
        tempB = (((directoryCoreIn.dateMonth) & 0x01E0) << 5);
        tempC = ((directoryCoreIn.dateDay) & 0x001F);
        fileInfoOut.fdate = tempA | tempB | tempC;

        tempA = tempB = tempC = 0u;

        tempA = ((directoryCoreIn.timeHour & 0xF800) << 11);
        tempB = (((directoryCoreIn.timeMinute) & 0x07E0) << 5);
        tempC = ((directoryCoreIn.timeSecond) & 0x001F);
        fileInfoOut.ftime = tempA | tempB | tempC;

        fileInfoOut.fattrib |= (directoryCoreIn.isReadOnly)?1u:0u;
        fileInfoOut.fattrib |= (directoryCoreIn.isHidden)?1u:0u;
        fileInfoOut.fattrib |= (directoryCoreIn.isSystem)?1u:0u;
        fileInfoOut.fattrib |= (directoryCoreIn.isArchive)?1u:0u;
        fileInfoOut.fattrib |= (directoryCoreIn.isDirectory)?1u:0u;

        #if FF_USE_LFN
        retVal = SimpleStringCopy(fileInfoOut.altname, directoryCoreIn.alternativeName, FF_SFN_BUF + 1) != 0u;    
        if(retVal) {
            retVal = SimpleStringCopy(fileInfoOut.fname, directoryCoreIn.fileName, FF_LFN_BUF + 1) != 0u;
        }
        #else
        retVal = SimpleStringCopy(fileInfoOut.fname, directoryCoreIn.fileName, 12 + 1) != 0u;
        #endif
    }

    return retVal;
}

bool FatFsToMARTe2FileInfoConverter(FILINFO &fileInfoIn, DirectoryCore &directoryCoreOut) {
    //Let's assume DirectoryCore is a structure and clean it
    //TODO Do something better than set, foreseeing DirectoryCore as class
    bool retVal = MemoryOperationsHelper::Set(&directoryCoreOut, 0, sizeof(DirectoryCore));
    
    if(retVal) {
        directoryCoreOut.size = static_cast<uint64>(fileInfoIn.fsize);

        directoryCoreOut.dateYear = static_cast<uint16>(((fileInfoIn.fdate & 0xFE00) >>  9) + 1980);
        directoryCoreOut.dateMonth = static_cast<uint8>((fileInfoIn.fdate & 0x01E0) >> 5);
        directoryCoreOut.dateDay = static_cast<uint8>(fileInfoIn.fdate & 0x001F);
        
        directoryCoreOut.timeHour = static_cast<uint8>((fileInfoIn.ftime & 0xF800) >> 11);
        directoryCoreOut.timeMinute = static_cast<uint8>((fileInfoIn.ftime & 0x07E0) >> 5);
        directoryCoreOut.timeSecond = static_cast<uint8>(fileInfoIn.ftime & 0x001F);

        directoryCoreOut.isReadOnly = ((fileInfoIn.fattrib & AM_RDO) != 0);
        directoryCoreOut.isHidden = ((fileInfoIn.fattrib & AM_HID) != 0);
        directoryCoreOut.isSystem = ((fileInfoIn.fattrib & AM_SYS) != 0);
        directoryCoreOut.isArchive = ((fileInfoIn.fattrib & AM_ARC) != 0);
        directoryCoreOut.isDirectory = ((fileInfoIn.fattrib & AM_DIR) != 0);

        #if FF_USE_LFN
        retVal = SimpleStringCopy(directoryCoreOut.alternativeName, fileInfoIn.altname, FF_SFN_BUF + 1) != 0u;    
        if(retVal) {
            retVal = SimpleStringCopy(directoryCoreOut.fileName, fileInfoIn.fname, FF_LFN_BUF + 1) != 0u;
        }
        #else
        retVal = SimpleStringCopy(directoryCoreOut.fileName, fileInfoIn.fname, 12 + 1) != 0u;
        #endif
    }

    return retVal;
}

Directory::Directory(const char8 * const path) :
        LinkedListable() {

    if (path != NULL) {
        fname = StringHelper::StringDup(path);
        FILINFO fileInfo;
        FRESULT statResult = f_stat(static_cast<TCHAR*>(fname), &fileInfo);

        if(statResult != FR_OK) {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Directory::Directory() stat failure");

            if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
                REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Directory::Directory() Failure deallocating fname in destructor after failure");
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::Information, "Directory::Directory() Path name reset");
            }
        }
        else {
            bool ok = FatFsToMARTe2FileInfoConverter(fileInfo, directoryHandle);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Directory::Directory() Failure converting FatFs to MARTe2 handle");
            }
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Directory::Directory() NULL path given, cannot proceed");
        
        if(!MemoryOperationsHelper::Set(&directoryHandle, '\0', static_cast<uint32>(sizeof(DirectoryCore)))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Failed initialization of directory handle");
        }

        fname=NULL_PTR(char8*);
    }

}

Directory::~Directory() {
    if (fname != NULL) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Directory::~Directory() Failure deallocating fname in destructor");
        }
    }
    fname = NULL_PTR(char8*);
}

bool Directory::SetByName(const char8 * const path) {
    bool ret = (path != NULL);
    if (fname != NULL_PTR(char8*)) {
        if (!HeapManager::Free(reinterpret_cast<void *&>(fname))) {
            REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "Directory::SetByName() Failure deallocating fname");
            ret = false;
        }
    }

    if (ret) {
        fname = StringHelper::StringDup(path);
        FILINFO fileInfo;
        FRESULT statResult = f_stat(static_cast<TCHAR*>(fname), &fileInfo);

        if(statResult != FR_OK) {
            ret = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Directory::SetByName() Failed stat");
        }
        else {
            ret = FatFsToMARTe2FileInfoConverter(fileInfo, directoryHandle);
        }
    }
    return ret;
}

const char8 *Directory::GetName() const {
    return fname;
}

bool Directory::IsDirectory() const {
    return directoryHandle.isDirectory;
}

bool Directory::IsFile() const {
    return !directoryHandle.isDirectory;
}

uint64 Directory::GetSize() {
    return directoryHandle.size;
}

TimeStamp Directory::GetLastWriteTime() {
        TimeStamp timeStamp;

        timeStamp.SetYear(directoryHandle.dateYear);
        timeStamp.SetMonth(directoryHandle.dateMonth);
        timeStamp.SetDay(directoryHandle.dateDay);
        timeStamp.SetHour(directoryHandle.timeHour);
        timeStamp.SetMinutes(directoryHandle.timeMinute);
        timeStamp.SetSeconds(directoryHandle.timeSecond);

        return timeStamp;
}

    TimeStamp Directory::GetLastAccessTime() {
        TimeStamp timeStamp;

        timeStamp.SetYear(directoryHandle.dateYear);
        timeStamp.SetMonth(directoryHandle.dateMonth);
        timeStamp.SetDay(directoryHandle.dateDay);
        timeStamp.SetHour(directoryHandle.timeHour);
        timeStamp.SetMinutes(directoryHandle.timeMinute);
        timeStamp.SetSeconds(directoryHandle.timeSecond);

        return timeStamp;
    }

    bool Directory::Create(const bool isFile) {
        bool ret = (fname != NULL_PTR(char8 *));

        if (ret) {
            if (isFile) {
                FIL filePointer;
                FRESULT createResult = f_open(&filePointer, static_cast<TCHAR*>(fname), FA_CREATE_ALWAYS | FA_WRITE);
                if(createResult != FR_OK) {
                    ret = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Directory::Create() File creation failed");
                }
                else {
                    createResult = f_close(&filePointer);
                    if(createResult != FR_OK) {
                        ret = false;
                        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Directory::Create() File close after creation failed");
                    }
                }
            }
            else {
                DIR directoryPointer;
                FRESULT mkdirResult = f_opendir(&directoryPointer, static_cast<TCHAR*>(fname));
                if(mkdirResult != FR_OK) {
                    ret = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Directory::Create() Directory creation failure");
                }
            }

            if(ret) {
                FILINFO fileInfo;
                FRESULT statResult = f_stat(static_cast<TCHAR*>(fname), &fileInfo);
                
                if(statResult != FR_OK) {
                    ret = false;
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Directory::Create() Failed stat");
                }
                else {
                    ret = FatFsToMARTe2FileInfoConverter(fileInfo, directoryHandle);
                }
            }
        }
        return ret;
    }

    bool Directory::Exists() {
        bool retVal = true;

        FILINFO fileInfo;
        FRESULT statResult = f_stat(static_cast<TCHAR*>(fname), &fileInfo);
        
        if(statResult != FR_OK) {
            retVal = false;
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Directory::Exists() Cannot stat");
        }
        else {
            retVal = FatFsToMARTe2FileInfoConverter(fileInfo, directoryHandle);
        }

        return retVal;
    }

    bool Directory::Delete() {
        bool retVal = (fname != NULL_PTR(char8 *));

        if (retVal) {
            FRESULT deleteResult = f_unlink(static_cast<TCHAR*>(fname));
            
            if(deleteResult != FR_OK) {
                retVal = false;
                REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "Directory::Delete() cannot unlink");
            }
        }

        return retVal;
    }

}

