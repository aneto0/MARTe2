/**
 * @file DoubleBufferedStream.h
 * @brief Header file for class DoubleBufferedStream
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

 * @details This header file contains the declaration of the class DoubleBufferedStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOUBLEBUFFEREDSTREAM_H_
#define DOUBLEBUFFEREDSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "IOBuffer.h"
#include "AnyType.h"
#include "StreamI.h"
#include "FormatDescriptor.h"
#include "BufferedStreamIOBuffer.h"
#include "StreamI.h"
#include "OperatingSystemCallbacksI.h"

namespace MARTe {

/**
 * @brief Buffered stream implementation (double buffer).
 * @details This class offers a buffering mechanism for character streams.
 * It supplements two independent low-level RawStreams (which implement the low-level calls
 * such as Read, Write, ...) with a buffering scheme.
 */
class DoubleBufferedStream: public StreamI, public OperatingSystemCallbacksI {

public:
    /**
     * @brief Default constructor.
     * @post
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetOutputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == TTInfiniteWait
     *   GetReadBufferSize() == 32u
     *   GetWriteBufferSize() == 32u
     */
    DoubleBufferedStream();

    /**
     * @brief Default constructor.
     * @post
     *   CanRead() == false
     *   CanWrite() == false
     *   GetInputBuffer() == BufferedStreamIOBuffer
     *   GetOutputBuffer() == BufferedStreamIOBuffer
     *   GetTimeout() == timeoutIn
     *   GetReadBufferSize() == 32u
     *   GetWriteBufferSize() == 32u
     */
    DoubleBufferedStream(const TimeoutType &timeoutIn);

    /**
     * @brief Default destructor.
     */
    virtual ~DoubleBufferedStream();

    /**
     * @brief Sets the buffer size.
     * @param[in] bufferSize the desired size for the buffer.
     * @return true if the buffer memory is reallocated correctly.
     * @pre
     *    readBufferSize > 8u
     *    writeBufferSize > 8u
     *    CanRead() == true || CanWrite() == true
     * @post
     *    GetInputStream()->GetBufferSize() == readBufferSize
     *    GetOutputStream()->GetBufferSize() == writeBufferSize
     */
    virtual bool SetBufferSize(uint32 readBufferSize,
                               uint32 writeBufferSize);


    /**
     * @see StreamI::Read
     * @post
     *   Position() == this'old->Position() + size
     */
    virtual bool Read(char8 * const output,
                      uint32 & size);

    /**
     * @see StreamI::Write
     * @post
     *   Position() == this'old->Position() + size
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size);

    /**
     * @brief Gets the read buffer size.
     * @return the read buffer size.
     */
    uint32 GetReadBufferSize() const;

    /**
     * @brief Gets the write buffer size.
     * @return the write buffer size.
     */
    uint32 GetWriteBufferSize() const;

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
    virtual bool RelativeSeek(int32 deltaPos);

    /**
     * @see StreamI::Position
     */
    virtual uint64 Position();

    /**
     * @see StreamI::SetSize
     */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Flushes the write buffer.
     * @return true if the Flush of the buffer succeeds.
     */
    inline bool Flush();

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

private:

    /**
     * The read buffer. It is used as
     * a middle buffer between the stream and the input.
     */
    BufferedStreamIOBuffer readBuffer;

    /**
     * The write buffer. It is used as
     * a middle buffer between the stream and the output.
     * For each write operation this buffer is filled completely
     * and then the desired size is copied on the output.
     */
    BufferedStreamIOBuffer writeBuffer;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool DoubleBufferedStream::Flush() {

    bool ret = true;
    // some data in writeBuffer
    // we can flush in all cases then
    if (writeBuffer.UsedSize() > 0u) {
        ret = writeBuffer.Flush();
    }
    return ret;
}

}
#endif /* DOUBLEBUFFEREDSTREAM_H_ */

