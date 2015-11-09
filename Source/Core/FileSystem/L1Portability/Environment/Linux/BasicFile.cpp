/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
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
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



namespace MARTe{


void FileSetFileName(BasicFile &f,const char *name){
    if (f.fileName != NULL) free((void *&)f.fileName);
    f.fileName = NULL;
    if (name!=NULL) f.fileName = strdup((char *)name);
}

bool FileRead(BasicFile &f,void* buffer, uint32 &size,TimeoutType msecTimeout){
    uint32 ret = read(f.file, (void *)buffer, size);
    if (ret<0) {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"FileRead");
    }
    else
        size=ret;
    return (ret>0);
}


bool FileWrite(BasicFile &f,const void* buffer, uint32 &size,TimeoutType msecTimeout){

    int ret = write(f.file, (void *)buffer, size);
    if (ret<0) {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"FileWrite");
    }
    else
        size=ret;
    return (ret>=0);
}

int64 FileSize(BasicFile &f){
    struct stat statistiche;
    int ret;
    int64 size;
    statistiche.st_size = 0;
    ret = fstat(f.file, &statistiche);
    if(ret < 0)
        return 0;
    size=(int64)statistiche.st_size;
    return size;
}

bool FileSeek(BasicFile &f,int64 pos){

    uint32 position = pos;
    uint32 newpos;

    newpos = lseek(f.file, position, SEEK_SET);
    if (newpos<0) {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"BasicFile::Seek");
    }
    if (pos != newpos){
        pos = newpos;
        return false;
    } else {
        return true;
    }
}

int64 FilePosition(BasicFile &f){
    int sz = lseek(f.file,0,SEEK_CUR);
    if (sz<0) {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"FilePosition");
    }
    return sz;
}

/// Clip the file size to a specified point
bool FileSetSize(BasicFile &f,int64 size){
    int ret = ftruncate(f.file,size);
    return (ret == 0);
}

/// Moves upwards or backwards so many bytes
bool  FileRelativeSeek(BasicFile &f,int64 pos){
    uint32 position = pos;
    int newpos = lseek(f.file, position,SEEK_CUR);
    if (newpos<0) {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"BasicFile::RelativeSeek");
    }
    return true;
}

/// Moves to position pos and returns new position on pos
bool  FilePositionSeek(BasicFile &f,int64 &pos){
    int p = pos;
    int pp = lseek(f.file,p,SEEK_SET);
    if (pp<0)  {
        f.action = OSError;
        CStaticAssertPlatformErrorCondition(OSError,"BasicFile::PositionSeek");
        return false;
    }
    pos = pp;
    return true;
}

bool FileOpen(BasicFile &f,const char *name){
    f.SetFileName(name);

    int flags = 0;
    int mode = 0777;

    uint32 tmp = f.fileMode & openingFlagsMask1;
    tmp = (tmp | (openingFlagsMask1 << 16));
    switch (tmp) {
        case createNewFile   : flags |= O_CREAT; break;
        case createOverwrite : flags |= O_CREAT; break;
    }

    tmp = f.fileMode & openingModeMask3;
    tmp = tmp | (openingModeMask3 << 16);

    switch (tmp) {
        case accessModeR  : flags |= O_RDONLY; break;
        case accessModeW  : flags |= O_WRONLY; break;
        case accessModeRW : flags |= O_RDWR;   break;
    }

    bool openDevDriver = f.fileMode & (devDriverMode & 0xFFFF);
    f.file = open (name, flags, mode);
    if (f.file != -1) {
        if(openDevDriver){
            return true;
        }

        struct stat statistiche;
        int ret = fstat(f.file, &statistiche);
        if(ret < 0 || !S_ISREG(statistiche.st_mode)){
            close(f.file);
            return false;
        }

        if (flags & O_CREAT) {
            f.action = openWasCreate;
            f.SetSize(0);
        } else {
            f.action = openWasOpen;
        }
        return true;
    }

    if ((flags & O_CREAT) == 0){

        tmp = f.fileMode & openingFlagsMask1;
        tmp = (tmp | (openingFlagsMask1 << 16));
        switch (tmp){
            case openCreate : flags |= O_CREAT; break;
        }

        f.file = open(name,flags,mode);
        if (f.file != -1) {
            struct stat statistiche;
            int ret = fstat(f.file, &statistiche);
            if(ret < 0 || !S_ISREG(statistiche.st_mode)){
                close(f.file);
                return false;
            }
            f.action = openWasCreate;
            return true;
        }
    }

    int32 errCode = OSError;
    switch (errCode){
        case  EACCES:{
            errCode = ErrorAccessDenied;
        } break;
    }
    return false;

}


bool FileLock(BasicFile &f,int64 start,int64 size,TimeoutType msecTimeout){
    // blocca, ma senza timeout!!!
    int ret=flock(f.file, LOCK_EX);
    if (ret<0) return false;
    return true;
}

/// Undo the locking.
bool FileUnLock(BasicFile &f,int64 start,int64 size,TimeoutType msecTimeout){
    // blocca, ma senza timeout!!!
    int ret=flock(f.file, LOCK_UN);
    if (ret<0) return false;
    return true;

}
//@}

/// Set the system value for the number of files
bool FileSetMaxNumberOfFiles(uint32 number){
    return false;
}

///
bool FileEraseFile(const char *fname,...){
    char name[256];
    va_list argList;
    va_start(argList,fname);
    vsnprintf(name,256,fname,argList);
    va_end(argList);

    int ret = remove(name);
    if (ret<0) return false;
    return true;
}
}
