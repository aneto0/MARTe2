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
 * It supplements a low-level RawStream (which implements the low-level calls such as Read,
 * Write, Seek, ...) with a buffering scheme.
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
     * @details if the size of the buffer is zero the character is directly written from the low level RawStream.
     * @param[out] c the character read from the stream.
     * @return true if the character is successfully read to the stream.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Read(1), but
     * this inline implementation could be faster if the read buffer is not empty */
    inline bool GetC(char8 &c);

    /**
     * @brief Reads data from stream into buffer.
     * @param buffer is the output memory where datas must be written.
     * @param size is the number of bytes to read from stream.
     * @param msecTimeout is the timeout.
     * @param completeRead is a flag which specified is the read operation is completed.
     * @return true if successful, false otherwise.
     *
     * In unbuffered mode calls unbufferedStream->Read function.
     * In buffered mode reads from the readBuffer to the outpur buffer. If not all the desired
     * size is copied, the readBuffer is refilled again if the remained size is minor than
     * a quarter of the readBuffer size, otherwise calls unbufferedStream->Read which should copy
     * directly datas from the stream to the output buffer.
     *
     * As much as size byte are written, actual size
     * is returned in size. msecTimeout is how much the operation should last.
     * Timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
     * when noWait is used .... */
    virtual bool Read(char8 * bufferIn,
                      uint32 & size);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /**
     * @brief Write data from a buffer to the stream.
     * @param buffer contains the datas to write on the stream.
     * @param size is the desired number of bytes to write.
     * @param msecTimeout is the timeout.
     * @param completeWrite is a flac which specified is the write operations is completed.
     * @return true if successful, false otherwise.
     *
     * In unbuffered mode calls unbufferedStream->Write function.
     * In buffered mode writes from the input buffer to writeBuffer if the size to write is
     * minor than a quarter of the writeBuffer size. If not all the size can be written,
     * flushes the buffer on the stream and write the remained size on writeBuffer.
     * Again, If the size to copy is greater than a quarter of the writeBuffer size,
     * it flushes the writeBuffer and then calls unbufferedStream->Write
     * which should copy data from input buffer to the stream directly.
     *
     * As much as size byte are written, actual size
     * is returned in size. msecTimeout is how much the operation should last.
     * Timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
     *  when noWait is used .... */
    virtual bool Write(const char8* bufferIn,
                       uint32 & size);

    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    // RANDOM ACCESS INTERFACE

    /** @brief The size of the stream.
     * @return the size of the filled memory.
     *
     * Calls FlushAndResync and then UnBufferedSize. */
    virtual uint64 Size();

    /** @brief Moves within the file to an absolute location.
     * @param pos is the absolute desired position in the stream.
     * @return false if CanSeek returns false or in case of errors.
     *
     * If writeBuffer is not empty, flushes it and then calls UnBufferedSeek.
     * If readBuffer is not empty, if the position falls in the readBuffer range calls readBuffer.Seek
     * otherwise empties the readBuffer and calls UnBufferedSeek.
     */
    virtual bool Seek(uint64 pos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Moves within the file relative to current location.
     * @param deltaPos is the desired relative position from the current.
     * @return false if canSeek returns false or in case of errors.
     *
     * If writeBuffer is not empty, flushes it and calls UnBufferedSeek with the current position + deltaPos
     * If readBuffer is not empty, if the final position falls in the range calls readBuffer.RelativeSeek
     * otherwise calls UnBufferedSeek passing currentStreamPos-readBuffer.Size+readBuffer.Position+deltaPos  */
    virtual bool RelativeSeek(int32 deltaPos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Returns current position.
     * @return the current position in the stream.
     *
     * Flushed the writeBuffer if it is not empty, then return currentPos-readBuffer.Size+readBuffer.Position */
    virtual uint64 Position();
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Clip the stream size to a specified point.
     * @param size is the new desired size for the stream.
     *
     * Depends on UnBufferedSetSize. */
    virtual bool SetSize(uint64 size);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    virtual bool CanSeek() const;

    /** whether it can be written into */
    virtual bool CanWrite() const;

    /** whether it can be  read */
    virtual bool CanRead() const;

    /**
     *
     */
    TimeoutType GetTimeout() const;

protected:
    /**
     Defines the operation mode and status of a basic stream
     one only can be set of the first 4.
     */
    struct OperatingModes {

        /** writeBuffer is the active one.
         */
        bool mutexReadMode;

        /** writeBuffer is the active one.
         */
        bool mutexWriteMode;

    };
    /** set automatically on initialisation by calling of the Canxxx functions */
    OperatingModes operatingModes;

    // methods to be implemented by deriving classes

    /**
     * @brief Get the read buffer.
     * @return BufferedStreamIOBuffer readBuffer pointer.
     *
     * This function is used by Printf and GetToken functions.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Get the write buffer.
     * @return BufferedStreamIOBuffer writeBuffer pointer.
     *
     * This function is used by Printf and GetToken functions.
     */
    virtual IOBuffer *GetOutputBuffer();

    //TODO
    RawStream *unbufferedStream;

private:
    /**
     * @brief Switch to the write mode.
     * @return false if the re-synchronization goes wrong.
     *
     *  Sets the readBufferFillAmount to 0.
     *  Synchronize the position in the stream.
     *  Sets the mutexWriteMode.
     *  Does not check for mutexBuffering to be active
     */
    inline bool SwitchToWriteMode();

    /**
     * @brief Switch to the read mode.
     * @return false if the flush function fails.
     *
     *  Flushes writeBuffer.
     *  Resets mutexWriteMode.
     *  Does not refill the buffer nor check the mutexBuffering is active.
     */
    inline bool SwitchToReadMode();

    /**
     * The read buffer. It is used just like
     * a middle buffer between the stream and the output.
     * For each read operation this buffer is filled completely
     * and then the desired size is copied on the output.
     * Using the buffer mode, the GetC function always use this buffer,
     * while for Read function it is used only if the size to read is minor
     * than a quarter than the buffer size.
     * Function BufferedStreamIOBuffer::NoMoreSpaceToWrite acts
     * as a flush and the more confidential function Flush calls it.
     * @see BufferedStreamBuffer for more informations.*/
    BufferedStreamIOBuffer readBuffer;

    /**
     * The write buffer. It is used just like an
     * intermediate between the input and the stream. Write
     * operations copies data from the input to this buffer
     * and only when the buffer is full (or in case of an explicit
     * FlushAndResync call) the buffer is flushed on the stream.
     * Using the buffer mode, the PutC function always use this buffer,
     * while for Write function it is used only if the buffer is 4 times greater
     * than the size to write.
     * Function BufferedStreamIOBuffer::NoMoreSpaceToRead acts
     * as a refill and the more confidential function Refill calls it.
     */
    BufferedStreamIOBuffer writeBuffer;

    TimeoutType timeout;

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

