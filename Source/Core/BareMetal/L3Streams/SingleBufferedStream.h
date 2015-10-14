/**
 * @file SingleBufferedStream.h
 * @brief Header file for class SingleBufferedStream
 * @date 06/10/2015
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

 * @details This header file contains the declaration of the class SingleBufferedStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SINGLEBUFFEREDSTREAM_H_
#define SINGLEBUFFEREDSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "IOBuffer.h"
#include "AnyType.h"
#include "BufferedStream.h"
#include "FormatDescriptor.h"
#include "BufferedStreamIOBuffer.h"
#include "BufferedStream.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Buffered stream implementation (single buffer).
 * @details This class offers a buffering mechanism for character streams.
 * It supplements a low-level RawStream (which implements the low-level calls
 * such as Read, Write, Seek, ...) with a buffering scheme.
 */
class SingleBufferedStream: public BufferedStream {

public:

    /**
     * @brief Default constructor.
     * @post
     *   GetInputBuffer() == NULL &&
     *   GetOutputBuffer() == NULL &&
     *   GetTimeout() == TTDefault
     */
    //TODO do we really need it? CanRead, CanWrite, ... would crash given that the lowLevelStream is NULL...
    SingleBufferedStream();

    /**
     * @brief Initialises the stream to work with an existing RawStream.
     * @param[in] lowLevelStream the RawStream responsible for implementing the low-level calls.
     * @post
     *   GetInputBuffer() == lowLevelStream &&
     *   GetOutputBuffer() == lowLevelStream &&
     *   GetTimeout() == TTDefault
     */
    SingleBufferedStream(RawStream* const lowLevelStream);

    /**
     * @brief Initialises the stream to work with an existing RawStream.
     * @param[in] lowLevelStream the RawStream responsible for implementing the low-level calls.
     * @param[in] msecTimeout the timeout to be used in the Read and Write operations.
     * @post
     *   GetInputBuffer() == lowLevelStream &&
     *   GetOutputBuffer() == lowLevelStream &&
     *   GetTimeout() == msecTimeout
     */
    //TODO the Read and the Write functions might deserve their own timeout.
    SingleBufferedStream(RawStream* const lowLevelStream,
                         const TimeoutType & msecTimeout);

    /**
     * @brief Default destructor.
     */
    virtual ~SingleBufferedStream();

    /**
     * @brief Sets the buffers size.
     * @param readBufferSize the desired size for the read buffer.
     * @param readBufferSize the desired size for the write buffer.
     * @return true if the buffer memory is allocated correctly.
     */
    //TODO this must be wrong? For the signle buffered stream only one size should be set, no?
    virtual bool SetBufferSize(uint32 readBufferSize,
                               uint32 writeBufferSize);

    /**
     * @brief Re-synchronisation and flushing of the read/write buffer.
     * @return true if the buffer is successfully flushed and resynch.
     */
    //TODO is this really a public function?
    inline bool FlushAndResync();

    /**
     * @brief Writes a single character to the stream.
     * @details if the size of the buffer is zero the character is directly written in the low level RawStream.
     * @param[in] c the character to be written on the stream.
     * @return true if the character is successfully written to the stream.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Write(1), but
     * this inline implementation could be faster if the write buffer is not full */
    inline bool PutC(const char8 c);

    /**
     * @brief Reads a single character from the stream.
     * @details if the size of the buffer (see SetBufferSize) is zero the character is directly written to the low level RawStream.
     * @param[out] c the character read from the stream.
     * @return true if the character is successfully read to the stream.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Read(1), but
     * this inline implementation could be faster if the read buffer is not empty */
    inline bool GetC(char8 &c);

    /**
     * @see BufferedStream::Read
     * @post
     *   Position() == this'old->Position() + size
     *   TODO check if this post condition is always true
     */
    virtual bool Read(char8 * output,
                      uint32 & size);

    /**
     * @see BufferedStream::Write
     * @post
     *   Position() == this'old->Position() + size
     *   TODO check if this post condition is always true
     */
    virtual bool Write(const char8* input,
                       uint32 & size);

    /**
     * @see BufferedStream::Size
     */
    virtual uint64 Size();

    /**
     * @see BufferedStream::Seek
     */
    virtual bool Seek(uint64 pos);

    /**
     * @see BufferedStream::RelativeSeek
     */
    virtual bool RelativeSeek(int32 delta);

    /**
     * @see BufferedStream::Position
     */
    virtual uint64 Position();

    /**
     * @see BufferedStream::SetSize
     */
    virtual bool SetSize(uint64 size);

    /**
     * @see BufferedStream::CanSeek
     */
    virtual bool CanSeek() const;

    /**
     * @see BufferedStream::CanWrite
     */
    virtual bool CanWrite() const;

    /**
     * @see BufferedStream::CanRead
     */
    virtual bool CanRead() const;

    /**
     * @brief Gets the timeout associated to the operations on this stream.
     * @return the currently set timeout.
     */
    TimeoutType GetTimeout() const;

protected:

    /**
     * @brief Gets the read buffer.
     * @return BufferedStreamIOBuffer readBuffer pointer.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Gets the write buffer.
     * @return BufferedStreamIOBuffer writeBuffer pointer.
     */
    virtual IOBuffer *GetOutputBuffer();

//TODO This was protected before. Check if can be private.
private:
    //TODO these OperatingModes are just providing a namespace. Simplify and just have two booleans please.
    /**
     Defines the operation mode and status of a basic stream
     one only can be set of the first 4.
     */
    struct OperatingModes {

        /** readBuffer is the active one.
         */
        bool mutexReadMode;

        /** writeBuffer is the active one.
         */
        bool mutexWriteMode;

    };

    /** set automatically on initialisation by calling of the Canxxx functions */
    OperatingModes operatingModes;

    /**
     * @brief Switches the stream to write mode.
     * @return true if the re-synchronization of the buffer from read to write mode is successful.
     */
    inline bool SwitchToWriteMode();

    /**
     * @brief Switches the stream to read mode.
     * @return true if the re-synchronization of the buffer from write to read mode is successful.
     */
    inline bool SwitchToReadMode();

    /**
     * The read buffer. It is used as
     * a middle buffer between the stream and the output.
     * For each read operation this buffer is filled completely
     * and then the desired size is copied on the output.
     */
    BufferedStreamIOBuffer readBuffer;

    /**
     * The write buffer. It is used as an
     * intermediate between the input and the stream. Write
     * operations copies data from the input to this buffer
     * and only when the buffer is full (or in case of an explicit
     * FlushAndResync call) the buffer is flushed on the stream.
     */
    BufferedStreamIOBuffer writeBuffer;

    /**
     * Timeout for the read and write operations.
     */
    TimeoutType timeout;

    /**
     * The low-level stream associated to this stream.
     */
    RawStream *unbufferedStream;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool SingleBufferedStream::FlushAndResync() {

    bool ret = true;
    // if there is something in the buffer, and canSeek it means we can and need to resync
    // if the buffers are separated (!canseek) than resync cannot be done
    if (readBuffer.UsedSize() > 0u) {
        ret = readBuffer.Resync();
    }
    else {
        // some data in writeBuffer
        // we can flush in all cases then
        if (writeBuffer.UsedSize() > 0u) {
            ret = writeBuffer.Flush();
        }
    }
    return ret;
}

bool SingleBufferedStream::PutC(const char8 c) {
    bool ret = true;

    if (operatingModes.mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = false;
        }
    }

    if (ret) {
        if (writeBuffer.BufferSize() > 0u) {
            ret = writeBuffer.PutC(c);
        }
        else {
            uint32 size = 1u;
            ret = unbufferedStream->Write(&c, size, timeout);
        }
    }

    return ret;
}

bool SingleBufferedStream::GetC(char8 &c) {

    bool ret = true;
    if (operatingModes.mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = false;
        }
    }

    if (ret) {
        if (readBuffer.BufferSize() > 0u) {
            ret = readBuffer.GetC(c);
        }
        else {
            uint32 size = 1u;
            ret = unbufferedStream->Read(&c, size, timeout);
        }
    }
    return ret;
}

bool SingleBufferedStream::SwitchToWriteMode() {
    bool ret = readBuffer.Resync();
    if (ret) {
        operatingModes.mutexWriteMode = true;
        operatingModes.mutexReadMode = false;
    }
    return ret;
}

bool SingleBufferedStream::SwitchToReadMode() {
    bool ret = !writeBuffer.Flush();
    if (ret) {

        operatingModes.mutexWriteMode = false;
        operatingModes.mutexReadMode = true;
    }
    return ret;
}

}

#endif /* SINGLEBUFFEREDSTREAM_H_ */

