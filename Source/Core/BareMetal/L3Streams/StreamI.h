/**
 * @file StreamI.h
 * @brief Header file for class StreamI
 * @date 05/10/2015
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

 * @details This header file contains the declaration of the class StreamI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMI_H_
#define STREAMI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TimeoutType.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract super class for all buffered streams.
 *
 * @details This class provides the common interface specification for all the
 * streams which support buffering.
 *
 * The stream represents a sequence of bytes which can be mapped to arrays,
 * strings, files, sockets, and so on, but that it can be accessed uniformly
 * using a common read/write interface. A stream is also meant to be able to
 * have the notion of cursor, which can be positioned randomly on any byte of
 * the stream; size, which allows to know the actual size of the stream; and
 * timeout, which allows to define a time out for read and write operations,
 * so they will not last for ever in case of I/O problems. Moreover, the
 * stream have implicit reading and writing buffers which are expected to be
 * defined by subclasses of the stream, but used by upper level functions
 * like read or write.
 *
 * Attributes:
 * - Size: The size of the stream, i.e. the number of bytes that it holds.
 * - Position: The position of the cursor, i.e. the zero-based index of the
 *   byte where the next read/write operation will occur.
 * - Timeout: It is the time out for read and write operations, infinite
 *   by default.
 * - ReadBuffer: It is the buffer for reading data, which will be used by
 *   the specific implementations of the read function.
 * - WriteBuffer: It is the buffer for writing data, which will be used by
 *   the specific implementations of the write function.
 *
 * Operators:
 * - AnyType()
 *
 * Operations:
 * - Read
 * - Write
 * - Seek
 * - Copy (*)
 * - GetToken (*)
 * - SkipTokens (*)
 * - GetLine (*)
 * - Printf (*)
 * - PrintFormatted (*)
 *
 * (*) The class supplies a standard implementation for these operations.
 *
 * @warning The generic capabilities for reading, writing, and seeking can not
 * assured to be available on all kind of mapped streams, so an additional set
 * of "CanXXX" methods will advise the user what can be done with the stream.
 *
 * - CanRead: Queries if the stream is readable, i.e. if the operation Read
 *   is implemented.
 * - CanWrite: Queries if the stream is writable, i.e. if the operation Write
 *   is implemented.
 * - CanSeek: Queries if the stream is seekable, i.e. if the operations Seek
 *   and RelativeSeek are implemented.
 *
 * @warning The size and position attributes can be meaningless in some kind
 * of mapped streams, so in these cases, size and position will be set to the
 * maximum value of uint64. This also implies that operations like read and
 * write, which increments position and/or size, will saturate them at the
 * maximum value on these cases.
 */
class DLL_API StreamI {

public:

    /**
     * Default constructor
     * @post
     *   GetTimeout() == TTInfiniteWait
     */
    StreamI();

    /**
     * @brief Default destructor
     */
    virtual ~StreamI();

    /**
     * @brief Queries if the stream is writable, i.e. if write operations can
     * be performed on the stream.
     * @return true if the stream is writable.
     */
    virtual bool CanWrite() const = 0;

    /**
     * @brief Queries if the stream is readable, i.e. if read operations can
     * be performed on the stream.
     * @return if the stream is readable.
     */
    virtual bool CanRead() const = 0;

    /**
     * @brief Queries if the stream is seekable, i.e. if seek operations can
     * be performed on the stream.
     * @return true if the stream is seekable.
     */
    virtual bool CanSeek() const = 0;

    /**
     * @brief Reads data from the stream.
     * @detail Reads up to \a size bytes into \a output. The actual read size
     * is returned in \a size.
     * @param[out] output the buffer where to write the data into.
     * @param[in,out] size the number of bytes to read. Upon return of the
     * function \a size contains the number of bytes actually read.
     * @pre CanRead()
     * @post Position() == this'old->Position() + size
     * @return true if \a size bytes are successfully read from the stream and
     * written into \a output within the specified timeout (see SetTimeout).
     */
    virtual bool Read(char8 * const output,
                      uint32 & size) = 0;

    /**
     * @brief Writes data into the stream.
     * @detail Writes up to \a size bytes into \a input. The actual written
     * size is returned in \a size.
     * @param[in] input the buffer where to read the data from.
     * @param[in,out] size the number of bytes to write. Upon return of the
     * function \a size contains the number of bytes actually written.
     * @pre CanWrite()
     * @post Position() == this'old->Position() + size &&
     *       this'old->Position() + size > Size() => Size() == Position()
     * @return true if \a size bytes are successfully read from \a input and
     * written into the stream within the specified timeout (see SetTimeout).
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size) = 0;

    /**
     * @brief Gets the size of the stream.
     * @return the size of the stream.
     */
    virtual uint64 Size() = 0;

    /**
     * @brief Moves within the stream to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     * @pre CanSeek()
     * @post
     *   pos <= Size() => Position() == pos &&
     *   pos > Size() => Position() == Size()
     */
    virtual bool Seek(uint64 pos) = 0;

    /**
     * @brief Moves within the stream to a position that is relative to the
     * current location.
     * @details Checks that the final position is >= 0 and <= UsedSize, then
     * moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     * @pre CanSeek()
     * @post
     *   pos + deltaPos < 0 => Position() == 0 &&
     *   pos + deltaPos <= Size() =>
     *   Position() == this'old->Position() + deltaPos &&
     *   pos + deltaPos > Size() => Position() == Size()
     * @warning (1) The deltaPos is a signed integer, so it will always have a
     * half addressable space with respect to its unsigned counterpart, i.e.
     * the Seek() method.
     * @warning (2) Moreover, the deltaPos is a 32 bits integer, less than the
     * 64 bits integer used in Seek(), so it will have a shorter addressable
     * space, anyway.
     */
    virtual bool RelativeSeek(const int32 deltaPos) = 0;

    /**
     * @brief Gets the current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position() = 0;

    /**
     * @brief Clips the stream size.
     * @param size the new size of the stream.
     * @return true if the size of the stream is set to \a size.
     * @pre true
     * @post Size() == size
     */
    virtual bool SetSize(uint64 size) = 0;

    /**
     * @brief Casts the stream to AnyType.
     * @return an AnyType representation of the stream.
     */
    inline operator AnyType();

    /**
     * @brief Reads a token from the stream into a character buffer.
     * @details Extracts a token from the stream until a terminator or \0
     * is found.
     * @param[out] outputBuffer the buffer where to write the retrieved
     * tokens into.
     * @param[in] terminator a list of terminator characters, i.e. characters
     * that allow to distinguish tokens.
     * @param[in] outputBufferSize the maximum size of the output buffer.
     * @param[out] saveTerminator if not NULL the found terminator (from the
     * terminator list) is stored in this variable.
     * @param[in] skipCharacters a list of characters to be removed from the
     * \a outputBuffer. As a consequence, if a skipCharacter is also a
     * terminator, when consecutive instances of the same terminator are found
     * (e.g. ::A:::B:C, where : is the terminator), the terminator is skipped
     * until the next token is found (in the previous example, the first token
     * to be found would be A).
     * @return false if no data is stored in the outputBuffer, true otherwise
     * (meaning that a token was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    virtual bool GetToken(char8 * const outputBuffer,
                          const char8 * const terminator,
                          const uint32 outputBufferSize,
                          char8 &saveTerminator,
                          const char8 * const skipCharacters);

    /**
     * @brief Reads a token from the stream into another stream.
     * @details Extracts a token from the stream until a terminator or \0 is
     * found.
     * @param[out] outputBuffer the buffer where to write the retrieved
     * tokens into.
     * @param[in] terminator a list of terminator characters, i.e. characters
     * that allow to distinguish tokens.
     * @param[in] outputBufferSize the maximum size of the output buffer.
     * @param[out] saveTerminator if not NULL the found terminator (from the
     * terminator list) is stored in this variable.
     * @param[in] skipCharacters a list of characters to be removed from the
     * \a outputBuffer. As a consequence, if a skipCharacter is also a
     * terminator, when consecutive instances of the same terminator are found
     * (i.e. without a token in-between) (e.g. ::A:::B:C, where : is the
     * terminator), the terminator is skipped until the next token is found
     * (in the previous example, the first token to be found would be A).
     * @return false if no data is stored in the outputBuffer, true otherwise
     * (meaning that a token was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetToken(StreamI & output,
                  const char8 * const terminator,
                  char8 &saveTerminator,
                  const char8 * const skipCharacters = NULL_PTR(const char8 *));

    /**
     * @brief Skips a series of tokens delimited by terminators or \0.
     * @details The functionality skipCharacters=terminator (see GetToken) is
     * automatically set, so that consecutive instances of the same terminator
     * @param[in] count the number of tokens to be skipped.
     * @param[in] terminator a list of terminator characters.
     * @return false if no data is read, true (meaning that at least a token
     * was found).
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool SkipTokens(const uint32 count,
                    const char8 * const terminator);

    /**
     * @brief Extracts a line from this stream into another stream.
     * @param[out] output is the output stream where the line must be written.
     * @param[in] skipTerminators if true the "\r" is skipped.
     * @return true if a line is successfully read from this stream and written
     * into \a output.
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetLine(StreamI & output,
                 bool skipTerminators = true);

    /**
     * @brief Extracts a line from this stream into a character buffer.
     * @param[out] outputBuffer is the character buffer where the line must be
     * written into.
     * @param[in] outputBufferSize the size of \a outputBuffer.
     * @param[in] skipTerminators if true the "\r" is skipped.
     * @return true if a line is successfully read from this stream and
     * written into \a outputBuffer.
     * @pre CanRead() && GetReadBuffer() != NULL
     * @post see brief
     */
    bool GetLine(char8 *outputBuffer,
                 const uint32 outputBufferSize,
                 bool skipTerminators = true);

    /**
     * @brief Printf implementation.
     * @param[in] format printf format as specified
     * in FormatDescriptor::InitialiseFromString.
     * @param[in] pars the list of elements that are to be replaced in the
     * \a format string. It must be terminated by a voidAnyType element
     * (i.e. pars[length(pars)-1] == voidAnyType).
     * @return true if the string is successfully printed.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool PrintFormatted(const char8 * const format,
                        const AnyType pars[]);

    /**
     * @brief Copies a character buffer.
     * @detail Copies a character buffer into this stream (from the
     * current position).
     * @param[in] buffer is the buffer to be copied into the stream.
     * @return true if buffer is successfully copied into the stream.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool Copy(const char8 * const buffer);

    /**
     * @brief Copies from a stream.
     * @detail Copies a stream into this stream (from the current
     * position).
     * @param[in,out] stream is the stream to be copied into the stream.
     * @return true if stream is successfully copied into the stream.
     * @pre CanWrite() && GetWriteBuffer() != NULL
     * @post see brief
     */
    bool Copy(StreamI &stream);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(const char8 * const format,
                       const AnyType& par1);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(const char8 * const format,
                       const AnyType& par1,
                       const AnyType& par2);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(const char8 * const format,
                       const AnyType& par1,
                       const AnyType& par2,
                       const AnyType& par3);

    /**
     * @see PrintFormatted.
     */
    inline bool Printf(const char8 * const format,
                       const AnyType& par1,
                       const AnyType& par2,
                       const AnyType& par3,
                       const AnyType& par4);

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
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetReadBuffer() = 0;

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetWriteBuffer() = 0;

private:

    /**
     * Timeout for the read/write operations
     */
    TimeoutType timeout;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamI::operator AnyType() {
    void *dataPointer = static_cast<void *>(this);
    TypeDescriptor dataDescriptor(false, Stream, 0u);

    return AnyType(dataDescriptor, 0u, dataPointer);
}

bool StreamI::Printf(const char8 * const format,
                     const AnyType& par1) {
    AnyType pars[2] = { par1, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool StreamI::Printf(const char8 * const format,
                     const AnyType& par1,
                     const AnyType& par2) {
    AnyType pars[3] = { par1, par2, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool StreamI::Printf(const char8 * const format,
                     const AnyType& par1,
                     const AnyType& par2,
                     const AnyType& par3) {
    AnyType pars[4] = { par1, par2, par3, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool StreamI::Printf(const char8 * const format,
                     const AnyType& par1,
                     const AnyType& par2,
                     const AnyType& par3,
                     const AnyType& par4) {
    AnyType pars[5] = { par1, par2, par3, par4, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

}

#endif /* BUFFEREDSTREAM_H_ */

