/**
 * @file BasicFile.h
 * @brief Header file for class BasicFile
 * @date 23/10/2015
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

 * @details This header file contains the declaration of the class BasicFile
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICFILE_H_
#define BASICFILE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** To enable the change of the opening mode    */
const uint32 openingFlagsMask1 = 0x00000003;
/** creates a file. fails if it already exists  */
const uint32 createNewFile = 0x00030000;
/** creates a file. overwrites existing files   */
const uint32 createOverwrite = 0x00030001;
/** opens an existing file. fails if it does not exist */
const uint32 openFile = 0x00030002;
/** opens an existing file. creates if it does not exist */
const uint32 openCreate = 0x00030003;
/** */
const uint32 openingModeMask1 = 0x00000030;
/** */
const uint32 localityNone = 0x00300000;
/** */
const uint32 localitySequential = 0x00300010;
/** */
const uint32 localityRandom = 0x00300020;
/** */
const uint32 localityMixed = 0x00300030;
/** */
const uint32 openingModeMask2 = 0x000000C0;
/** */
const uint32 shareModeNoRW = 0x00C00000;
/** */
const uint32 shareModeNoW = 0x00C00040;
/** */
const uint32 shareModeNoR = 0x00C00080;
/** */
const uint32 shareModeAll = 0x00C000C0;
/** */
const uint32 openingModeMask3 = 0x00000300;
/** */
const uint32 accessModeR = 0x03000000;
/** */
const uint32 accessModeW = 0x03000100;
/** */
const uint32 accessModeRW = 0x03000200;
/** */
const uint32 devDriverMode = 0x0400D000;

/** */
const uint32 openWasOpen = 0;
/** */
const uint32 openWasCreate = 1;
/** */
const uint32 openWasOverWrite = 2;

class BasicFile;

extern "C" {
/** BasicFile::Open */
bool FileOpen(BasicFile &f,
              const char *name);

/** BasicFile::Lock */
bool FileLock(BasicFile &f,
              int64 start,
              int64 size,
              TimeoutType msecTimeout = TTInfiniteWait);

/** BasicFile::UnLock */
bool FileUnLock(BasicFile &f,
                int64 start,
                int64 size,
                TimeoutType msecTimeout = TTInfiniteWait);

/** Set the system value for the number of files */
bool FileSetMaxNumberOfFiles(uint32 number);

/** BasicFile::EraseFile */
bool FileEraseFile(const char *fname,
                   ...);

/** */
void FileSetFileName(BasicFile &f,
                     const char *name);

/** */
bool FileRead(BasicFile &f,
              void* buffer,
              uint32 &size,
              TimeoutType msecTimeout);

/** */
bool FileWrite(BasicFile &f,
               const void* buffer,
               uint32 &size,
               TimeoutType msecTimeout);

/** */
int64 FileSize(BasicFile &f);

/** */
bool FileSeek(BasicFile &f,
              int64 pos);

/** */
int64 FilePosition(BasicFile &f);

/** */
bool FileSetSize(BasicFile &f,
                 int64 size);

/** */
bool FileRelativeSeek(BasicFile &f,
                      int64 pos);

/** */
bool FilePositionSeek(BasicFile &f,
                      int64 &pos);
}

/** The file */
class BasicFile {

    friend bool FileOpen(BasicFile &f,
                         const char *name);
    friend bool FileLock(BasicFile &f,
                         int64 start,
                         int64 size,
                         TimeoutType msecTimeout);
    friend bool FileUnLock(BasicFile &f,
                           int64 start,
                           int64 size,
                           TimeoutType msecTimeout);
    friend bool FileRead(BasicFile &f,
                         void* buffer,
                         uint32 &size,
                         TimeoutType msecTimeout);
    friend void FileSetFileName(BasicFile &f,
                                const char *name);
    friend bool FileWrite(BasicFile &f,
                          const void* buffer,
                          uint32 &size,
                          TimeoutType msecTimeout);
    friend int64 FileSize(BasicFile &f);
    friend bool FileSeek(BasicFile &f,
                         int64 pos);
    friend int64 FilePosition(BasicFile &f);
    friend bool FileSetSize(BasicFile &f,
                            int64 size);
    friend bool FileRelativeSeek(BasicFile &f,
                                 int64 pos);
    friend bool FilePositionSeek(BasicFile &f,
                                 int64 &pos);

protected:
    /** the file handle */
    HFILE file;

    /** the mode the file will be opened or has been opened */
    uint32 fileMode;

    /** what happened during a creation, or the error code in case of failure */
    int32 action;

    /** the name of the file opened */
    char *fileName;

    /** Change the file name*/
    void SetFileName(const char *name) {
        FileSetFileName(*this, name);
    }

public:
    /** constructor */
    BasicFile() {
        file = (HFILE) 0;
        fileMode = openCreate | localityMixed | accessModeRW | shareModeAll;
        action = 0xFFFF;
        fileName = NULL;
    }

    /** */
    virtual ~BasicFile() {
        Close();
        SetFileName(NULL);
    }

    /**
     * Returns the file handle
     */
    const HFILE Handle() {
        return file;
    }

    // PURE STREAMING
    inline bool Read(void* buffer,
                     uint32 & size,
                     TimeoutType msecTimeout = TTDefault) {
        return FileRead(*this, buffer, size, msecTimeout);
    }

    /** This function is for the final class to implement. */
    inline bool Write(const void* buffer,
                      uint32 & size,
                      TimeoutType msecTimeout = TTDefault) {
        return FileWrite(*this, buffer, size, msecTimeout);
    }

    /** Can you read from this file? */
    inline bool CanRead() {
        uint32 rwaccess = fileMode & (accessModeR >> 16);
        return ((rwaccess == (accessModeR & 0xFFFF)) || (rwaccess == (accessModeRW & 0xFFFF)));
    }

    /** Can you write to this file */
    inline bool CanWrite() {
        uint32 rwaccess = fileMode & (accessModeW >> 16);
        return ((rwaccess == (accessModeW & 0xFFFF)) || (rwaccess == (accessModeRW & 0xFFFF)));
    }

    // RANDOM ACCESS INTERFACE

    /** The file size */
    inline int64 Size() {
        return FileSize(*this);
    }

    /** Move to specified file position */
    inline bool Seek(int64 pos) {
        return FileSeek(*this, pos);
    }

    /** The current file position */
    inline int64 Position(void) {
        return FilePosition(*this);
    }

    /** Set the file size by either truncating or extending */
    inline bool SetSize(int64 size) {
        return FileSetSize(*this, size);
    }

    /** Can you seek in this file ?*/
    inline bool CanSeek() {
        return true;
    }

    // Extended Attributes or Multiple Streams INTERFACE

    /** The file name */
    inline const char * FileName() {
        return fileName;
    }

    /** Moves upwards or backwards so many bytes */
    inline bool RelativeSeek(int64 pos) {
        return FileRelativeSeek(*this, pos);
    }

    /** Moves to position pos and returns new position on pos */
    inline bool PositionSeek(int64 &pos) {
        return FilePositionSeek(*this, pos);
    }

    /** Clip the file size to the current seek point */
    inline bool Truncate() {
        return SetSize(Position());
    }

    /**
     set opening modes: use a selctions of flags
     example : accessModeRW | createNewFile   */
    inline void SetOpeningModes(uint32 modeSet) {
        uint32 mask = modeSet >> 16;
        uint32 value = modeSet & 0xFFFF;
        fileMode &= ~mask;
        fileMode |= value;
    }

    /** open for read/write create if missing */
    inline bool Open(const char *fname,
                     ...) {
        if (fname == NULL)
            return false;
        char name[256];
        va_list argList;
        va_start(argList, fname);
        vsnprintf(name, 256, fname, argList);
        va_end(argList);
        return FileOpen(*this, name);
    }

    /** Close the file handle */
    inline bool Close() {
        if (file != (HFILE) 0)
            file = (HFILE) 0;
        return true;
    }

    /** open for read-write */
    inline bool OpenWrite(const char *fname,
                          ...) {
        if (fname == NULL)
            return false;
        SetOpeningModes(openCreate | accessModeRW);
        char name[256];
        va_list argList;
        va_start(argList, fname);
        vsnprintf(name, 256, fname, argList);
        va_end(argList);
        return Open(name);
    }

    /** open for read-only create never */
    inline bool OpenRead(const char *fname,
                         ...) {
        if (fname == NULL)
            return false;
        SetOpeningModes(openFile | accessModeR);
        char name[256];
        va_list argList;
        va_start(argList, fname);
        vsnprintf(name, 256, fname, argList);
        va_end(argList);
        return Open(name);
    }

    /** open for read/write create always */
    inline bool OpenNew(const char *fname,
                        ...) {
        if (fname == NULL)
            return false;
        SetOpeningModes(createOverwrite | accessModeRW);
        char name[256];
        va_list argList;
        va_start(argList, fname);
        vsnprintf(name, 256, fname, argList);
        va_end(argList);
        return Open(name);
    }

    /** What happened during opening? {openWasOpen,openWasCreate,openWasOverWrite} */
    inline int32 OpenAction() {
        return action;
    }

    /** What happened during an action. Only supported on open for now */
    int32 ErrorReason() {
        return action;
    }

//@{ Locking region of a file
    /**
     Locks to the application the access of the filke region starting from start
     and size bytes long. If it was locked wait as much as timeout
     */
    inline bool Lock(int64 start,
                     int64 size,
                     TimeoutType msecTimeout = TTInfiniteWait) {
        return FileLock(*this, start, size, msecTimeout);
    }

    /** Undo the locking. */
    inline bool UnLock(int64 start,
                       int64 size,
                       TimeoutType msecTimeout = TTInfiniteWait) {
        return FileUnLock(*this, start, size, msecTimeout);
    }
//@}

    /** Set the system value for the number of files handles (only WIN32 and OS2) */
    static inline bool SetMaxNumberOfFiles(uint32 number) {
        return FileSetMaxNumberOfFiles(number);
    }

};

}
#endif /* BASICFILE_H_ */

