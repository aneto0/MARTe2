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
#include "StreamI.h"
#include "FormatDescriptor.h"
#include "BufferedStreamIOBuffer.h"
#include "BufferedStreamI.h"
#include "OperatingSystemCallbacksI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Buffered stream (single buffer version).
 * @details This class offers a buffering mechanism for character streams.
 * It supplements a low-level OperatingSystemStream (which implements the
 * low-level calls such as Read, Write, Seek, ...) with a buffering scheme.
 */
class DLL_API SingleBufferedStream: public BufferedStreamI, public OperatingSystemCallbacksI {

public:
    /**
     * @brief Default constructor.
     * @post
     *   CanRead() &&
     *   not CanWrite() &&
     *   GetInputBuffer() == BufferedStreamIOBuffer &&
     *   GetTimeout() == TTInfiniteWait &&
     *   GetInputBuffer()->GetBufferSize() == 32u
     */
    SingleBufferedStream();

    /**
     * @brief Initialises object with a specified timeout.
     * @param[in] timeoutIn the timeout for the read and write operations.
     * @post
     *   CanRead() &&
     *   not CanWrite() &&
     *   GetInputBuffer() == BufferedStreamIOBuffer &&
     *   GetTimeout() == timeoutIn &&
     *   GetInputBuffer()->GetBufferSize() == 32u
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
     *    bufferSize > 8u &&
     *    (CanRead() || CanWrite())
     * @post
     *    GetInputStream()->GetBufferSize() == bufferSize
     */
    virtual bool SetBufferSize(uint32 bufferSize);

    /**
     * @brief Gets the buffer size.
     * @return the currently set buffer size
     */
    virtual uint32 GetBufferSize() const;

    /**
     * @see StreamI::Read
     */
    virtual bool Read(char8 * const output,
                      uint32 & size);

    /**
     * @see StreamI::Read
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      const TimeoutType &timeout);

    /**
     * @see StreamI::Write
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size);

    /**
     * @see StreamI::Write
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       const TimeoutType &timeout);

    /**
     * @see StreamI::Size
     */
    virtual uint64 Size();

    /**
     * @see StreamI::Seek
     */
    virtual bool Seek(uint64 pos);

    /**
     * @see StreamI::RelativeSeek
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @see StreamI::Position
     */
    virtual uint64 Position();

    /**
     * @see StreamI::SetSize
     */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Re-synchronisation and flushing of the read/write buffer.
     * @return true if the buffer is successfully flushed and resynch.
     */
    inline bool FlushAndResync();

    /**
     * @brief Gets the timeout value for read/write operations.
     * @return the timeout value for read/write operations.
     */
    TimeoutType GetTimeout() const;

    /**
     * @brief Sets the read/write timeout value.
     * @param[in] timeoutIn the new read/write timeout value.
     * @pre true
     * @post GetTimeout() == timeout
     */
    void SetTimeout(const TimeoutType &timeoutIn);

protected:
    /**
     * @brief Gets the read buffer.
     * @return the BufferedStreamIOBuffer readBuffer pointer.
     */
    virtual IOBuffer *GetReadBuffer();

    /**
     * @brief Gets the write buffer.
     * @return the BufferedStreamIOBuffer writeBuffer pointer.
     */
    virtual IOBuffer *GetWriteBuffer();

    /**
     * Stream is exclusive read mode.
     */
    bool mutexReadMode;

    /**
     * Stream is exclusive write mode.
     */
    bool mutexWriteMode;

    /**
     * Set to true if the buffer size could be successfully set
     */
    bool bufferSizeSet;

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

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool SingleBufferedStream::FlushAndResync() {

    bool ret = true;
    // if there is something in the buffer, and canSeek it means we can and need to resync
    // if the buffers are separated (!canseek) than resync cannot be done
    if (mutexWriteMode) {
        ret = internalBuffer.Flush();
    }
    if (mutexReadMode) {
        ret = internalBuffer.Resync();
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
    bool ret = internalBuffer.Flush();
    if (ret) {
        mutexWriteMode = false;
        mutexReadMode = true;
    }
    return ret;
}

}
#endif /* SINGLEBUFFEREDSTREAM_H_ */

