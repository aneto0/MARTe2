/**
 * @file BufferedStream.h
 * @brief Header file for class BufferedStream
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

 * @details This header file contains the declaration of the class BufferedStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFEREDSTREAM_H_
#define BUFFEREDSTREAM_H_

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
 * @details This class provides the interface specification for all the
 * streams which support buffering. It also offers a standard implementation
 * of the GetToken and GetLine functions.
 */
class BufferedStream {

public:

    /**
     * Default constructor
     * @post
     *   GetTimeout() == TTInfiniteWait
     */
    BufferedStream();

    /**
     * @brief Default destructor
     */
    virtual ~BufferedStream();

    /**
     * @brief Queries if the stream is writable.
     * @return true if the stream is writable.
     */
    virtual bool CanWrite() const =0;

    /**
     * @brief Queries if the stream is readable.
     * @return if the stream is readable.
     */
    virtual bool CanRead() const =0;

    /**
     * @brief Queries if seek operations can be performed on the stream.
     * @return true if seek operations can be performed on the stream.
     */
    virtual bool CanSeek() const =0;

    /**
     * @brief Reads data from the stream.
     * @detail Reads up to \a size bytes into \a bufferIn. The actual read size is
     * returned in \a size.
     * @param[out] output the buffer where to read the data into.
     * @param[in,out] size the number of bytes to read. Upon return of the function \a size contains the number of bytes actually read.
     * @return true if \a size bytes are successfully read into \a bufferIn within the specified timeout (see SetTimeout).
     */
    virtual bool Read(char8* output,
                      uint32 & size)=0;

    /**
     * @brief Writes data into the stream.
     * @detail Writes up to \a size bytes into \a bufferIn. The actual written size is
     * returned in \a size.
     * @param[in] input the buffer where to read the data from.
     * @param[in,out] size the number of bytes to write. Upon return of the function \a size contains the number of bytes actually written.
     * @return true if \a size bytes are successfully read from \a bufferIn and written into the stream within the specified timeout (see SetTimeout).
     */
    virtual bool Write(const char8* input,
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
     * @post
     *   Position() == pos
     */
    virtual bool Seek(uint64 pos) = 0;

    /**
     * @brief Moves within the stream to a position that is relative to the current location.
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     * @post
     *   Position() == this'old->Position() + deltaPos
     */
    virtual bool RelativeSeek(int32 deltaPos)=0;

    /**
     * @brief Gets the current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position() = 0;

    /**
     * @brief Clips the stream size.
     * @param size the new size of the stream.
     * @return true if the size of the stream is set to \a size.
     * @post
     *   GetSize() == size
     */
    virtual bool SetSize(uint64 size) = 0;

    /**
     * @brief Casts the stream to AnyType.
     * @return an AnyType representation of the stream.
     */
    inline operator AnyType();

    /**
     * @brief Writes a character to the stream.
     * @param[in] c the character to write on the stream.
     * @return true if the character is successfully written into the stream.
     */
    inline bool PutC(const char8 c);

    /**
     * @brief Reads a character from the stream.
     * @param[in] c the character to read from the stream.
     * @return true if the character is successfully read from the stream.
     */
    inline bool GetC(char8 &c);

    /**
     * @brief Reads a token from the stream.
     * @details Extracts a token from the stream until a terminator or \0 is found.
     *
     * @param[out] outputBuffer the buffer where to write the retrieved tokens into.
     * @param[in] terminator a list of terminator characters, i.e. characters that allow to distinguish tokens.
     * @param[in] outputBufferSize the maximum size of the output buffer.
     * @param[in] saveTerminator if true the terminators are also included in the string saved into \a outputBuffer.
     * @param[in] skipCharacters a list of characters to be skipped.
     * @return false if no data is read, true otherwise.
     */
    virtual bool GetToken(char8 * const outputBuffer,
                          const char8 * const terminator,
                          const uint32 outputBufferSize,
                          char8 &saveTerminator,
                          const char8 * const skipCharacters);

    /**
     * @brief Reads a token from the stream.
     * @details Extracts a token from the stream until a terminator or \0 is found.
     *
     * @param[out] outputBuffer the buffer where to write the retrieved tokens into.
     * @param[in] terminator a list of terminator characters, i.e. characters that allow to distinguish tokens.
     * @param[in] outputBufferSize the maximum size of the output buffer.
     * @param[in] saveTerminator if true the terminators are also included in the string saved into \a outputBuffer.
     * @param[in] skipCharacters a list of characters to be skipped.
     * @return false if no data is read, true otherwise.
     */
    bool GetToken(BufferedStream & output,
                  const char8 * const terminator,
                  char8 &saveTerminator,
                  const char8 * const skipCharacters = NULL_PTR(const char8 *));

    /**
     * @brief Skips a series of tokens delimited by terminators or \0.
     * @param[in] count the number of tokens to be skipped.
     * @param[in] terminator a list of terminator characters.
     * @return false if no data read, true otherwise.
     */
    bool SkipTokens(const uint32 count,
                    const char8 * const terminator);

    /**
     * @brief Extracts a line from this stream into another stream.
     * @param[out] output is the output stream where the line must be written.
     * @param[in] skipTerminators if true \r is skipped.
     * @return true if a line is successfully read from this stream and written into \a output.
     */
    bool GetLine(BufferedStream & output,
                 bool skipTerminators = true);

    /**
     * @brief Extracts a line from this stream into a character buffer.
     * @param[out] outputBuffer is the character buffer where the line must be written into.
     * @param[in] outputBufferSize the size of \a outputBuffer.
     * @param[in] skipTerminators if true \r is skipped.
     * @return true if a line is successfully read from this stream and written into \a outputBuffer.
     */
    bool GetLine(char8 *outputBuffer,
                 const uint32 outputBufferSize,
                 bool skipTerminators = true);

    /**
     * @brief Printf implementation.
     * @param format printf format as specified in FormatDescriptor::InitialiseFromString.
     * @param pars the list of elements that are to be replaced in the \a format string. It must be terminated
     * by a voidAnyType element (i.e. pars[length(pars)-1] == voidAnyType).
     * @return true if the string is successfully printed.
     */
    bool PrintFormatted(const char8 * const format,
                        const AnyType pars[]);

    /**
     * @brief Copies a character buffer.
     * @detail Copies a character buffer into this stream (from the current position).
     * @param[out] buffer is the buffer to be copied into the stream.
     * @return true if buffer is successfully copied into the stream.
     */
    bool Copy(const char8 * const buffer);

    /**
     * @brief Copies from a stream.
     * @detail Copies a stream into this stream (from the current position).
     * @param[out] stream is the stream to be copied into the stream.
     * @return true if stream is successfully copied into the stream.
     */
    bool Copy(BufferedStream &stream);

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
     * @brief Writes without buffering.
     * @param[in] data the array of bytes to write.
     * @param[in,out] size as input is the number of bytes to write. In output the number of bytes actually written.
     * @return true if \a size bytes of data are successfully written within the specified \a timeout (see SetTimeout).
     */
    virtual bool UnbufferedWrite(const char8 * const data,
                                 uint32 & size)=0;

    /**
     * @brief Reads without buffering.
     * @param[out] data destination array where the read data will be put.
     * @param[in,out] size as input is the number of bytes to read. In output the number of bytes actually read.
     * @return true if \a size bytes of data are successfully read within the specified \a timeout (see SetTimeout).
     */
    virtual bool UnbufferedRead(char8 * const data,
                                uint32 & size)=0;

    /**
     * @brief Retrieves the size of the low-level, unbuffered, stream implementation.
     * @return the size of the low-level stream.
     */
    virtual uint64 UnbufferedSize() = 0;

    /**
     * @brief Moves within the low-level, unbuffered, stream implementation to an absolute location.
     * @param[in] pos the desired absolute position.
     * @return true if the stream is successfully moved to \a pos.
     * @post
     *   UnbufferedPosition() == pos
     */
    virtual bool UnbufferedSeek(uint64 pos) = 0;

    /**
     * @brief Moves within the low-level, unbuffered, stream to a position that is relative to the current location.
     * @param[in] deltaPos is the distance from the current position.
     * @return true if the stream is successfully moved to \a deltaPos.
     * @post
     *   UnbufferedPosition() == this'old->UnbufferedPosition() + deltaPos
     */
    virtual bool UnbufferedRelativeSeek(int32 deltaPos)=0;

    /**
     * @brief Gets the current position in the low-level, unbuffered, stream.
     * @return the current position in the low-level stream.
     */
    virtual uint64 UnbufferedPosition() = 0;

    /**
     * @brief Clips the low-level, unbuffered, stream size.
     * @param size the new size of the low-level stream.
     * @return true if the size of the low-level stream is set to \a size.
     * @post
     *   UnbufferedSize() == size
     */
    virtual bool UnbufferedSetSize(uint64 size) = 0;

    /**
     * @brief Gets the read/write timeout value.
     * @return the read/write timeout value.
     */
    TimeoutType GetTimeout() const;

    /**
     * @brief Sets the read/write timeout value.
     * @param[in] timeoutIn the new read/write timeout value.
     * @post
     *   GetTimeout() == timeout
     */
    void SetTimeout(const TimeoutType timeoutIn);

protected:

    /**
     * @brief Gets the read buffer.
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetInputBuffer() = 0;

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetOutputBuffer() = 0;

private:
    /**
     * Timeout for the read/write operations
     */
    TimeoutType timeout;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

BufferedStream::operator AnyType() {
    void *dataPointer = static_cast<void *>(this);
    TypeDescriptor dataDescriptor(false, Stream, 0u);

    return AnyType(dataDescriptor, 0u, dataPointer);
}

bool BufferedStream::PutC(const char8 c) {
    uint32 size = 1u;
    return Write(&c, size);
}

bool BufferedStream::GetC(char8 &c) {
    uint32 size = 1u;
    return Read(&c, size);

}

bool BufferedStream::Printf(const char8 * const format,
                            const AnyType& par1,
                            const AnyType& par2) {
    AnyType pars[3] = { par1, par2, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStream::Printf(const char8 * const format,
                            const AnyType& par1,
                            const AnyType& par2,
                            const AnyType& par3) {
    AnyType pars[4] = { par1, par2, par3, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

bool BufferedStream::Printf(const char8 * const format,
                            const AnyType& par1,
                            const AnyType& par2,
                            const AnyType& par3,
                            const AnyType& par4) {
    AnyType pars[5] = { par1, par2, par3, par4, voidAnyType };
    return PrintFormatted(format, &pars[0]);
}

}

#endif /* BUFFEREDSTREAM_H_ */

