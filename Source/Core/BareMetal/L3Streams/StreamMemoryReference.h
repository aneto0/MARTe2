/**
 * @file StreamMemoryReference.h
 * @brief Header file for class StreamMemoryReference
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

 * @details This header file contains the declaration of the class StreamMemoryReference
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMMEMORYREFERENCE_H_
#define STREAMMEMORYREFERENCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BufferedStream.h"
#include "TimeoutType.h"
#include "IOBuffer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 @brief A basic implementation of a stream which allows reading and writing to a memory location
 */
class StreamMemoryReference: public BufferedStream {



public:

    StreamMemoryReference();
    /**
     * @brief Binds this object to a memory area in read and write mode.
     * @param buffer is the char8 pointer of the preallocated memory.
     * @param bufferSize is the usable size of the buffer.
     *
     * The function assumes that the area of memory is empty and therefore the Stream::Size is 0
     * */
    StreamMemoryReference(char8 * const bufferIn,
                          const uint32 bufferSize);

    /**
     * @brief Binds this object to a memory area in read only mode.
     * @param buffer is the const char8 pointer to a preallocated read only memory.
     * @param bufferSize is the usable size of the buffer.
     *
     * The function assumes that the area of memory is full and therefore the Stream::Size is bufferSize
     * */
    StreamMemoryReference(const char8 * const bufferIn,
                          const uint32 bufferSize);

    /** Destructor */
    virtual ~StreamMemoryReference();


    /** @brief Automatic cast to AnyType as a const char8 passing Buffer() return value. */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The StreamMemoryReference is considered as a string and not as a standard stream*/
    operator AnyType();
    /*---------------------------------------------------------------------------*/

    /**
     * @brief Reads data from the stream to the buffer.
     * @param buffer is the buffer where data must be written.
     * @param is the desired number of bytes to read.
     * @param msecTimeout is the desired timeout unused here.
     * @param complete is a flag unused here.
     * @return true.
     *
     * This function calls IOBuffer::Read, see it for more informations.
     *
     As much as size byte are read,
     actual read size is returned in size. (unless complete = true)
     msecTimeout is how much the operation should last - no more - if not any (all) data read then return false
     timeout behaviour depends on class characteristics and sync mode.
     */
    virtual bool Read(char8* bufferIn,
                      uint32 & size);

    /**
     * @brief Write data from a buffer to the stream.
     * @param buffer contains the data to write on the stream.
     * @param size is the number of bytes to write on the stream.
     * @param msecTimeout is the desired timeout unused here.
     * @param complete is a flag unused here.
     * @return true.
     *
     * This function calls IOBuffer::Write, see it for more informations.
     *
     As much as size byte are written,
     actual written size is returned in size.
     msecTimeout is how much the operation should last.
     timeout behaviour depends on class characteristics and sync mode.
     */
    virtual bool Write(const char8* bufferIn,
                       uint32 & size);

    /**
     * @brief Write operations allowed.
     * @return true if the preallocated memory is valid (pointer != NULL). */
    virtual bool CanWrite() const;

    /**
     * @brief Read operations allowed.
     * @return true if the preallocated memory is valid (pointer != NULL). */
    virtual bool CanRead() const;

    /**
     * @brief The size of the filled memory.
     * @return the size of the stream.
     *
     * For this object the size is always minor than the buffer dimension passed in the constructor. */
    virtual uint64 Size();

    /**
     * @brief Moves within the file to an absolute location.
     * @param pos is the desired absolute position.
     * @return false in case of position out of bounds.
     *
     * If the desired position falls out of the range, the position becomes the end of the filled stream. */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Moves within the stream relative to current location.
     * @param deltaPos is the gap from the current position.
     * @return false if the position falls out of bounds.
     *
     * If the final position falls out of the stream bounds, it becomes one of the bounds. */
    virtual bool RelativeSeek(int32 deltaPos);

    /**
     * @brief  Returns current position.
     * @return the current position. */
    virtual uint64 Position();

    /**
     * @brief Set the used size.
     * @param size is the desired stream size.
     * @return true.
     *
     * Setting the size manually you can read until that position. */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Seek operations are allowed
     * @return true. */
    virtual bool CanSeek() const;

    /*---------------------------------------------------------------------------*/


    /**
     * @brief Read Only access to the internal buffer. It calls a CharBuffer function.
     * @return The pointer to the internal buffer.
     */
    inline const char8 *Buffer() const;

    /**
     * @brief Read Write access top the internal buffer. It calls a CharBuffer function.
     * @return The pointer to the internal buffer.
     */
    inline char8 *BufferReference() const;

    /**
     * @brief Returns a pointer to the tail of the buffer.
     * @param  ix the offset from the end of buffer. valid ranges is 0 to Size()-1
     * @return pointer to the tail of the buffer
     */
    inline const char8 *Tail(const uint32 ix) const;


protected:
    // methods to be implemented by deriving classes

    /**
     * @brief Returns the read buffer.
     * @return a pointer to the MemoryReferenceIOBuffer buffer. */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Returns the write buffer.
     * @return a pointer to the MemoryReferenceIOBuffer buffer. */
    virtual IOBuffer *GetOutputBuffer();

private:

    /**
     * Simply a normal IOBuffer. Nothing is overloaded.
     * It is allocated by IOBuffer::SetBufferReferencedMemory or IOBuffer::SetBufferReadOnlyReferencedMemory
     * @see IOBuffer.cpp for the naked implementation of this buffer.  */
    IOBuffer buffer;



};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
const char8 *StreamMemoryReference::Buffer() const {
    return buffer.Buffer();
}

char8 *StreamMemoryReference::BufferReference() const {
    return buffer.BufferReference();
}

const char8 *StreamMemoryReference::Tail(const uint32 ix) const {
    bool ok = (ix <= (buffer.UsedSize() - 1u));

    return (ok) ? (&(buffer.BufferReference()[(buffer.UsedSize() - ix) - 1u])) : static_cast<const char8 *>(NULL);
}


}
#endif /* STREAMMEMORYREFERENCE_H_ */

