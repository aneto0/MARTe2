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
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == TTInfiniteWait
     */
    SingleBufferedStream();

    /**
     * @brief Initialises object with a specified timeout.
     * @param[in] timeoutIn the timeout for the read and write operations.
     * @post
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == timeoutIn
     */
    SingleBufferedStream(const TimeoutType & timeoutIn);

    /** 
     * @brief Default destructor. 
     */
    virtual ~SingleBufferedStream();

    /**
     * @brief Sets the buffer size.
     * @param[in] bufferSize the desired size for the buffer.
     * @return true if the buffer memory is reallocated correctly.
     * @pre
     *    bufferSize > 8u
     *    CanRead() == true || CanWrite() == true
     * @post
     *    GetInputStream()->BufferSize() == bufferSize
     */
    virtual bool SetBufferSize(uint32 bufferSize);

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
     */
    virtual bool Read(char8 * output,
                      uint32 & size);

    /**
     * @see BufferedStream::Write
     * @post
     *   Position() == this'old->Position() + size
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
    virtual bool RelativeSeek(int32 deltaPos);

    /**
     * @see BufferedStream::Position
     */
    virtual uint64 Position();

    /**
     * @see BufferedStream::SetSize
     */
    virtual bool SetSize(uint64 size);

protected:
    /**
     * @brief Gets the read buffer.
     * @return the BufferedStreamIOBuffer readBuffer pointer.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Gets the write buffer.
     * @return the BufferedStreamIOBuffer writeBuffer pointer.
     */
    virtual IOBuffer *GetOutputBuffer();

private:

    /**
     * Stream is exclusive read mode.
     */
    bool mutexReadMode;

    /**
     * Stream is exclusive write mode.
     */
    bool mutexWriteMode;

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
     * The write buffer. It is used as an
     * intermediate between the input and the stream. Write
     * operations copies data from the input to this buffer
     * and only when the buffer is full (or in case of an explicit
     * FlushAndResync call) the buffer is flushed on the stream.
     */
    BufferedStreamIOBuffer internalBuffer;

    /**
     * @brief Re-synchronisation and flushing of the read/write buffer.
     * @return true if the buffer is successfully flushed and resynch.
     */
    inline bool FlushAndResync();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool SingleBufferedStream::FlushAndResync() {

    bool ret = true;
    // if there is something in the buffer, and canSeek it means we can and need to resync
    // if the buffers are separated (!canseek) than resync cannot be done
    if (internalBuffer.UsedSize() > 0u) {
        ret = internalBuffer.Resync();
    }
    else {
        // some data in internalBuffer
        // we can flush in all cases then
        if (internalBuffer.UsedSize() > 0u) {
            ret = internalBuffer.Flush();
        }
    }
    return ret;
}

bool SingleBufferedStream::PutC(const char8 c) {
    bool ret = true;

    if (mutexReadMode) {
        if (!SwitchToWriteMode()) {
            ret = false;
        }
    }

    if (ret) {
        if (internalBuffer.BufferSize() > 0u) {
            ret = internalBuffer.PutC(c);
        }
        else {
            uint32 size = 1u;
            ret = UnbufferedWrite(&c, size);
        }
    }

    return ret;
}

bool SingleBufferedStream::GetC(char8 &c) {

    bool ret = true;
    if (mutexWriteMode) {
        if (!SwitchToReadMode()) {
            ret = false;
        }
    }

    if (ret) {
        if (internalBuffer.BufferSize() > 0u) {
            ret = internalBuffer.GetC(c);
        }
        else {
            uint32 size = 1u;
            ret = UnbufferedRead(&c, size);
        }
    }
    return ret;
}

bool SingleBufferedStream::SwitchToWriteMode() {
    bool ret = internalBuffer.Resync();
    if (ret) {
        mutexWriteMode = true;
        mutexReadMode = false;
    }
    return ret;
}

bool SingleBufferedStream::SwitchToReadMode() {
    bool ret = !internalBuffer.Flush();
    if (ret) {
        mutexWriteMode = false;
        mutexReadMode = true;
    }
    return ret;
}

}
#endif /* SINGLEBUFFEREDSTREAM_H_ */

