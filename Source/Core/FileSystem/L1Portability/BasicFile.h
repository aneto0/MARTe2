/**
 * @file BasicFile.h
 * @brief Header file for class BasicFile
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

#include "GeneralDefinitions.h"
#include "StreamI.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,BasicFileProperties.h)


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

const uint32 ACCESS_MODE_R   = 0x00000000;
/** */
const uint32 ACCESS_MODE_W   = 0x00000001;
/** */
const uint32 ACCESS_MODE_R_W = 0x00000002;
//TODO: Perhaps the APPEND flag is not necessary, because it can be achieved with a Seek(Size()) call anyway.
const uint32 FLAG_APPEND     = 0x00000010;

const uint32 FLAG_CREAT      = 0x00000020;

const uint32 FLAG_TRUNC      = 0x00000040;

const uint32 FLAG_EXCL       = 0x00000080;

//TODO: Define what is a BasicFile:
/**
 * It is a wrapper for a specific OS file handle.
 * The OS handle is a kind of pointer/identifier of a memory structure
 * which maintains state of an open file.
 * The OS structure and its handle is created when the file is opened.
 * While the handles are always open by definition, the class, as a wrapper,
 * maintains a double state {open, closed}, which means that when in closed
 * state, it wraps an invalid handle.
 * It is not intended that this class made cache of any file properties,
 * like mode, flags, pathname, etc.
 *
 */

class BasicFile: public StreamI {
public:
    /**
     * @brief Default constructor
     * @post
     *   not CanRead() &&
     *   not CanWrite() &&
     *   not CanSeek() &&
     *   Size() == 0 &&
     *   Position() == 0
     */
     
     //TODO: Copy constructor?? --> BasicFile(const BasicFile& obj);
    //Define copy semantics:
    //  a) Copy the handle as is?
    //  b) Use a handle duplication function supplied by the OS, like dup() in Linux?
    BasicFile();

    void SetMode(const uint32 setMode);

    /**
     * @brief ...
     * @pre IsOpen()
     * @post
     */
    void SetFlags(const uint32 setFlags);

    uint32 getFlags();

    virtual bool CanWrite() const;

    virtual bool CanRead() const;

    virtual bool CanSeek() const;

    /**
     * @brief ...
     * @post
     *   CanRead() == (flags | ACCESS_MODE_R) &&
     *   not CanWrite() &&
     *   not CanSeek() &&
     *   Size() == 0 &&
     *   Position() == 0
     */
    bool Open(const char * pathname, const uint32 flags);

    bool IsOpen();

    bool Close();

    virtual bool Read(char8* const output,
                      uint32 & size);

    virtual bool Read(char8 * const output,
                      uint32 & size,
                      const TimeoutType &msecTimeout);

    virtual bool Write(const char8 * const input,
                       uint32 & size);

    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       const TimeoutType &msecTimeout);

    virtual uint64 Size();

    virtual bool Seek(uint64 pos);

    virtual bool RelativeSeek(const int32 deltaPos);

    virtual uint64 Position();
    
     /**
     * @pre IsOpen() && CanWrite && �CanSeek()?
     * @post Size() == size &&
     *       this'old->Position() >= size => Position() == size &&
     *       this'old->Position() < size => Position() == this'old->Position()
     */

    virtual bool SetSize(uint64 size);

private:
//TODO: Hide the OS specific implementation
    BasicFileProperties fileProperties;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /*BASICFILE_H_ */

