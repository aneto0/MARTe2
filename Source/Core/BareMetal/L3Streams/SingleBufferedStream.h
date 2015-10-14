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
 * @file SingleBufferedStream.h
 * @brief Implementation of streamable type functions.
 *
 * Implementation of streamable type functions for read, write and seek operations in buffered and unbuffered modes for streamable types
 * i.e files. Most of the functions depends from the virtual UnBuffered functions which must be implemented in the derived classes
 * because could be differents for different streamable types.
 *
 * SingleBufferedStream uses two IOBuffer in buffered modes, one for reading and one for writing operations and they are used togheter if the stream is defined as
 * readable, writable and seekable.*/

/**
 * @brief SingleBufferedStream class.

 Replaces CStream and Streamable of BL1
 Inherits from pure virtual StreamInterface and does not resolve all pure virtual functions

 Uses the following StreamInterface methods to operate:
 CanRead  Read
 CanWrite Write
 CanSeek  Seek
 But only allows using GetC and PutC as it masks the whole StreamInterface
 To use the whole StreamInterface one needs to implement safe versions of
 all functions. This is left for for further class derivations.

 Note also that this class supports partially or fully disabled buffering.
 Just set the buffer size to 0 and the buffer to NULL.

 It is a buffering mechanism for character streams
 It operates in 6 modes (+variants) depending on the Stream capabilities
 CanSeek CanRead CanWrite
 and user Buffering choices

 1) Separate Buffering Output Mode
 CanWrite !CanSeek
 Used for devices where in and out streams are separate (for instance console )
 internalBuffer.Buffer()!=NULL && operatingMode.MutexBuffering() = false
 !Can be combined with Separate Buffering Input Mode
 1a) internalBuffer.Buffer()== NULL just call directly StreamInterface::Write

 2) Separate buffering Input Mode
 CanRead  !CanSeek
 Used for devices where in and out streams are separate (for instance console )
 internalBuffer.Buffer()!=NULL  &&  operatingMode.mutexBuffering = false
 !Can be combined with Separate Buffering Output Mode
 2a) internalBuffer.Buffer()== NULL just call directly StreamInterface::Read

 3) Dual separate buffers Input and Output Mode
 CanRead CanWrite !CanSeek
 Mode 1/1a and 2/2a combined

 4) Joint buffering Mode
 CanRead CanWrite CanSeek
 internalBuffer.Buffer()!=NULL  && internalBuffer.Buffer()!=NULL &&
 operatingMode.mutexBuffering() = true
 operatingMode.mutexWriteMode and
 operatingMode.mutexReadMode  determines whether the read
 or write buffering is active
 Get and Put toggle the two flags
 everytime the flags are changed a proper Flush operation is
 triggered to clean the buffers
 4a-b) one of internalBuffer or internalBuffer is NULL
 same toggling of flags and flushing

 5) Joint buffering Read Only Mode
 CanRead !CanWrite CanSeek
 internalBuffer.Buffer()!=NULL  && internalBuffer.Buffer()==NULL &&
 operatingMode.mutexBuffering() = false
 Operates identically to mode 2 but cannot be active together
 with mode 6
 5a) same as 2a

 6) Joint buffering Write Only Mode
 !CanRead CanWrite CanSeek
 internalBuffer.Buffer()==NULL  && internalBuffer.Buffer()!=NULL &&
 operatingMode.mutexBuffering() = false
 Operates identically to mode 1 but cannot be active together
 with mode 5
 6a) same as 1a

 */
class SingleBufferedStream: public BufferedStream {

public:
    /**
     * @brief Default constructor.
     *
     * At the beginning the stream is monodirectional. */
    //TODO the construction of internalBuffer and internalBuffer has to be changed.
    SingleBufferedStream();

    SingleBufferedStream(const TimeoutType & msecTimeout);

    /** @brief Default destructor. */
    virtual ~SingleBufferedStream();

    /**
     * @brief Sets the buffers size, impose the buffered modality.
     * @param readBufferSize is the desired size for the read buffer.
     * @param readBufferSize is the desired size for the write buffer.
     * @return true if the memory is allocated correctly.
     *
     * If readBufferSize and writeBufferSize are minor than 8, they becomes equalt to 8.
     * If CanRead return false readBufferSize becomes 0.
     * If CanWrite return false writeBufferSize becomes 0.
     * If both returns true and also CanSeek return true, the mutex mode is activated.
     * The function calls FlushAndResync and then allocates the desired sizes for
     * internalBuffer and internalBuffer using IOBuffer::SetBufferHeapMemory.
     */
    virtual bool SetBufferSize(uint32 readBufferSize,
                               uint32 writeBufferSize);

    // special inline methods for buffering

    /**
     * @brief Resyncronization and flush of the buffers.
     * @param msecTimeout is the timeout.
     * @return true if the Resync and Flush operations for buffers goes fine.
     *
     * On dual separate buffering (CanSeek=false) just Flush output.
     * On joint buffering (CanSeek= true) depending on read/write mode
     * either Resync or Flush.
     * The function SingleBufferedStream::Resync adjusts the position on the stream
     * after a read operation, shifted because of the Refill.
     */
    inline bool FlushAndResync();
    /**
     * @brief Simply write a character to the stream if space exist and if operatingModes allows.
     * @param c is the character to be written on the stream.
     * @return false in case of errors.
     *
     * In buffered mode uses the inline IOBuffer::PutC of internalBuffer
     * but with the specific implementations of BufferedStreamIOBuffer.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Write(1), but
     * this inline implementation could be faster if the write buffer is not full */
    inline bool PutC(const char8 c);

    /**
     * @brief Simply read a character from stream.
     * @param c is the character by reference in return.
     * @return false in case of errors.
     *
     * In buffered mode uses the inline IOBuffer::GetC of internalBuffer
     * but with the specific implementations of BufferedStreamIOBuffer.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The Printf function uses the standard Read(1), but
     * this inline implementation could be faster if the read buffer is not empty */
    inline bool GetC(char8 &c);

    /** @brief Reads data from stream into buffer.
     * @param buffer is the output memory where datas must be written.
     * @param size is the number of bytes to read from stream.
     * @param msecTimeout is the timeout.
     * @param completeRead is a flag which specified is the read operation is completed.
     * @return true if successful, false otherwise.
     *
     * In unbuffered mode calls UnbufferedRead function.
     * In buffered mode reads from the internalBuffer to the outpur buffer. If not all the desired
     * size is copied, the internalBuffer is refilled again if the remained size is minor than
     * a quarter of the internalBuffer size, otherwise calls UnbufferedRead which should copy
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
     * In unbuffered mode calls UnbufferedWrite function.
     * In buffered mode writes from the input buffer to internalBuffer if the size to write is
     * minor than a quarter of the internalBuffer size. If not all the size can be written,
     * flushes the buffer on the stream and write the remained size on internalBuffer.
     * Again, If the size to copy is greater than a quarter of the internalBuffer size,
     * it flushes the internalBuffer and then calls UnbufferedWrite
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
     * If internalBuffer is not empty, flushes it and then calls UnBufferedSeek.
     * If internalBuffer is not empty, if the position falls in the internalBuffer range calls internalBuffer.Seek
     * otherwise empties the internalBuffer and calls UnBufferedSeek.
     */
    virtual bool Seek(uint64 pos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Moves within the file relative to current location.
     * @param deltaPos is the desired relative position from the current.
     * @return false if canSeek returns false or in case of errors.
     *
     * If internalBuffer is not empty, flushes it and calls UnBufferedSeek with the current position + deltaPos
     * If internalBuffer is not empty, if the final position falls in the range calls internalBuffer.RelativeSeek
     * otherwise calls UnBufferedSeek passing currentStreamPos-internalBuffer.Size+internalBuffer.Position+deltaPos  */
    virtual bool RelativeSeek(int32 deltaPos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Returns current position.
     * @return the current position in the stream.
     *
     * Flushed the internalBuffer if it is not empty, then return currentPos-internalBuffer.Size+internalBuffer.Position */
    virtual uint64 Position();
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT

    /** @brief Clip the stream size to a specified point.
     * @param size is the new desired size for the stream.
     *
     * Depends on UnBufferedSetSize. */
    virtual bool SetSize(uint64 size);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT



protected:
    /**
     Defines the operation mode and status of a basic stream
     one only can be set of the first 4.
     */
    struct OperatingModes {

        /** internalBuffer is the active one.
         */
        bool mutexReadMode;

        /** internalBuffer is the active one.
         */
        bool mutexWriteMode;

    };
    /** set automatically on initialisation by calling of the Canxxx functions */
    OperatingModes operatingModes;

    // methods to be implemented by deriving classes

    /**
     * @brief Get the read buffer.
     * @return BufferedStreamIOBuffer internalBuffer pointer.
     *
     * This function is used by Printf and GetToken functions.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Get the write buffer.
     * @return BufferedStreamIOBuffer internalBuffer pointer.
     *
     * This function is used by Printf and GetToken functions.
     */
    virtual IOBuffer *GetOutputBuffer();

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
     *  Flushes internalBuffer.
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
    BufferedStreamIOBuffer internalBuffer;

    TimeoutType timeout;

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

    if (operatingModes.mutexReadMode) {
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
            ret = UnbufferedWrite(&c, size, timeout);
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
        if (internalBuffer.BufferSize() > 0u) {
            ret = internalBuffer.GetC(c);
        }
        else {
            uint32 size = 1u;
            ret = UnbufferedRead(&c, size, timeout);
        }
    }
    return ret;
}

bool SingleBufferedStream::SwitchToWriteMode() {
    bool ret = internalBuffer.Resync();
    if (ret) {
        operatingModes.mutexWriteMode = true;
        operatingModes.mutexReadMode = false;
    }
    return ret;
}

bool SingleBufferedStream::SwitchToReadMode() {
    bool ret = !internalBuffer.Flush();
    if (ret) {

        operatingModes.mutexWriteMode = false;
        operatingModes.mutexReadMode = true;
    }
    return ret;
}

}
#endif /* SINGLEBUFFEREDSTREAM_H_ */

