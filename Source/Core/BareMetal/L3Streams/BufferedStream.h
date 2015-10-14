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
 * @file BufferedStream.h
 * @brief Common father class for all streamable implementations.
 *
 * This functions implements the most useful and powerful functions like Printf and GetToken leaving
 * other functions implementation to the derived classes.
 *
 * Since StreamI performs just an interface, this class is the father of the
 * more specific streams like for example BufferedStream (file, socket, ecc.) StreamString (advanced strings manager with
 * heap memory allocation), StreamMemoryReference (stack memory).
 *
 * StreamI functions for the multi stream are overloaded protected because are not used by all descendents.
 */

/**
 * @brief Abstract super class for all streamables.
 *
 * It's the common point of implementation between BufferedStream descendent
 * and MemoryMappedStreams.
 * Provide common abstract buffering scheme.
 */
class BufferedStream {

public:

    /**
     * @brief Default constructor
     */
    BufferedStream();
    /**
     * default destructor
     */
    virtual ~BufferedStream();

    /**
     * @brief Pure virtual function. Defines if write operations can be performed on the stream.
     * @return return value depends from derived classes implementation.
     */
    virtual bool CanWrite() const =0;

    /**
     * @brief Pure virtual function. Defines if read operations can be performed on the stream.
     * @return return value depends from derived classes implementation.
     */
    virtual bool CanRead() const =0;

    /**
     * @brief Pure virtual method. Defines if seek operations can be performed on the stream.
     * @return return value depends on the derived classes implementation.
     */
    virtual bool CanSeek() const =0;

    virtual bool UnbufferedWrite(const char8 * const bufferIn,
                                 uint32 & size,
                                 const TimeoutType &msecTimeout)=0;

    virtual bool UnbufferedRead(char8 * const bufferIn,
                                uint32 & size,
                                const TimeoutType &msecTimeout)=0;

    /**
     * @brief Pure virtual method. The size of the stream.
     * @return the size of the stream depending on derived classes implementation.  */
    virtual uint64 UnbufferedSize() = 0;

    /**
     * @brief Pure virtual method. Moves within the stream to an absolute location.
     * @param pos is the desired absolute position.
     * @return return value depends on derived classes implementation. */
    virtual bool UnbufferedSeek(uint64 pos) = 0;

    /**
     * @brief Pure virtual method. Moves within the file relative to current location.
     * @param deltaPos is the gap from the current position.
     * @return return value depends on derived classes implementation. */
    virtual bool UnbufferedRelativeSeek(int32 deltaPos)=0;

    /**
     * @brief Pure virtual method. Returns current position.
     * @return the current position in the stream. */
    virtual uint64 UnbufferedPosition() = 0;

    /**
     * @brief Pure virtual method. Clip the stream size to the desired value.
     * @param size is the desired size.
     * @return return value depends on the derived classes implementation. */
    virtual bool UnbufferedSetSize(uint64 size) = 0;

    /**
     * @brief Pure virtual method. Reads data from the stream to a char8* buffer.
     * @param buffer is the buffer where datas must be copied.
     * @param size is the desired number of bytes to copy.
     * @param msecTimeout is the desired timeout.
     * @param complete is a flag.
     *
     * The behavior depends by derived classes implementation.
     *
     Reads data into buffer.
     As much as size byte are read,
     actual read size is returned in size. (unless complete = true)
     msecTimeout is how much the operation should last - no more - if not any (all) data read then return false
     timeout behavior depends on class characteristics and sync mode.
     return false implies failure to comply with minimum requirements:
     timeout and complete and data read  != size
     timeout and data read == 0
     error in the stream  ==> no point to try again
     parameters error, for instance buffer = NULL
     */
    virtual bool Read(char8* bufferIn,
                      uint32 & size)=0;

    /**
     * @brief Pure virtual method. Writes from a const char8* buffer to the stream.
     * @param buffer contains the datas which must be copied on the stream.
     * @param size is the desired number of bytes to write.
     * @param msecTimeout is the desired timeout.
     * @param complete is a flag.
     *
     * The behavior depends by derived classes implementation.
     *
     Write data from a buffer to the stream.
     As much as size byte are written,
     actual written size is returned in size.
     msecTimeout is how much the operation should last.
     timeout behavior depends on class characteristics and sync mode.
     return false implies failure to comply with minimum requirements:
     timeout and complete and data written  != size
     timeout and data written == 0
     error in the stream ==> no point to try again
     parameters error, for instance buffer = NULL
     */
    virtual bool Write(const char8* bufferIn,
                       uint32 & size) = 0;

    /**
     * @brief Pure virtual method. The size of the stream.
     * @return the size of the stream depending on derived classes implementation.
     */
    virtual uint64 Size() = 0;

    /**
     * @brief Pure virtual method. Moves within the stream to an absolute location.
     * @param pos is the desired absolute position.
     * @return return value depends on derived classes implementation.
     */
    virtual bool Seek(uint64 pos) = 0;

    /**
     * @brief Pure virtual method. Moves within the file relative to current location.
     * @param deltaPos is the gap from the current position.
     * @return return value depends on derived classes implementation.
     */
    virtual bool RelativeSeek(int32 deltaPos)=0;

    /**
     * @brief Pure virtual method. Returns current position.
     * @return the current position in the stream.
     */
    virtual uint64 Position() = 0;

    /**
     * @brief Pure virtual method. Clip the stream size to the desired value.
     * @param size is the desired size.
     * @return return value depends on the derived classes implementation.
     */
    virtual bool SetSize(uint64 size) = 0;

    /**
     * @brief Automatic cast to AnyType for a generic stream Printf.
     */
    inline operator AnyType();

    /**
     * @brief Writes a character on the stream.
     * @param c is the character to write on the stream.
     * @return depends from derived classes implementation.
     *
     * Uses the derived class implementation of Write function with one as size parameter,
     * then the function behavior depends from the derived class Write function.*/
    inline bool PutC(const char8 c);

    /**
     * @brief Reads a character from the stream.
     * @param c is the character in return.
     * @return depends from derived classes implementation.
     *
     * Uses the derived class implementation of Read function with one as size parameter,
     * then the function behavior depends from the derived class Read function. */
    inline bool GetC(char8 &c);

    /**
     * @brief Reads a token from the stream and writes it on the char8* buffer provided.
     * @param terminator is a list of terminator characters.
     * @param outputBufferSize is the maximum size of the output buffer.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of characters to be skipped.
     * @return false if no data read, true otherwise.
     *
     * This function is performed for buffered streams, namely streams with an IOBuffer type as read buffer.
     * If the function GetInputBuffer returns NULL a StreamWrapperIOBuffer is created at the moment on the stack
     * with a dimension of 64 bytes and it substitutes the absent IOBuffer.
     *
     Extract a token from the stream into a string data until a terminator or 0 is found.
     Skips all skip characters, if you want to skip also terminator characters at the begging add them to skip characters.
     Returns true if some data was read before any error or file termination. false only on error and no data available
     The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
     skipCharacters=NULL is equivalent to skipCharacters = terminator.
     */
    virtual bool GetToken(char8 * const outputBuffer,
                          const char8 * const terminator,
                          const uint32 outputBufferSize,
                          char8 * const saveTerminator,
                          const char8 * const skipCharacters);

    /**
     * @brief Reads a token from the stream and writes it on another stream.
     * @param terminator is a list of terminator characters.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of characters to be skipped.
     * @return false if no data read, true otherwise.
     *
     * This function is performed for buffered streams, namely this stream should have an IOBuffer type as read buffer
     * and the output stream an IOBuffer as write buffer.
     * If the function GetInputBuffer returns NULL for this stream a StreamWrapperIOBuffer is created at the moment on the stack
     * with a dimension of 64 bytes and it substitutes the absent IOBuffer for read operations.
     * If the function GetOutputBuffer returns NULL for the output stream a StreamWrapperIOBuffer is created at the moment
     * on the stack with a dimension of 64 bytes and it substitutes the absent IOBuffer for write operations.
     *
     ** extract a token from the stream into a string data until a terminator or 0 is found.
     Skips all skip characters, if you want to skip also terminator characters at the beginning add them to the skip characters.
     returns true if some data was read before any error or file termination. false only on error and no data available
     The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
     skipCharacters=NULL is equivalent to skipCharacters = terminator
     A character can be found in the terminator or in the skipCharacters list  in both or in none
     0) none                 the character is copied
     1) terminator           the character is not copied the string is terminated
     2) skip                 the character is not copied
     3) skip + terminator    the character is not copied, the string is terminated only if not empty
     */
    bool GetToken(BufferedStream & output,
                  const char8 * const terminator,
                  char8 * const saveTerminator = static_cast<char8 *>(NULL),
    const char8 * const skipCharacters=static_cast<const char8 *>(NULL));

    /**
     * @brief Skips a series of tokens delimited by terminators or 0.
     * @param count is the number of tokens to be skipped.
     * @param terminator is a list of terminator characters.
     * @return false if no data read, true otherwise.
     */
    bool SkipTokens(const uint32 count,
    const char8 * const terminator);

    /**
     * @brief Prints a list of elements looking to a specified format.
     * @param format is a printf like string format.
     * @param pars is a list of elements to be printed.
     * @see Print.
     *
     Pars is a vector terminated by voidAnyType value.
     Format follows the TypeDescriptor::InitialiseFromString.
     Prints all data pointed to by pars.
     */
    bool PrintFormatted(const char8 * const format,
    const AnyType pars[]);

    /**
     * @brief Copies a const char8* into this stream from current position.
     * @param buffer is the buffer to be copied on the stream.
     * @return the result of the Write operation which depends on derived classes implementation.
     */
    bool Copy(const char8 * const buffer);

    /**
     * @brief Copies from stream current Position to end.
     * @param stream is the stream to be copied on this stream.
     * @param return false if the results of Read and Write streams operations fails.
     */
    bool Copy(BufferedStream &stream);

    /**
     * @see Printf with two elements to print.
     */
    inline bool Printf(const char8 * const format,
    const AnyType& par1,
    const AnyType& par2);

    /**
     * @see Printf with three elements to print.
     */
    inline bool Printf(const char8 * const format,
    const AnyType& par1,
    const AnyType& par2,
    const AnyType& par3);

    /**
     * @see Printf with four element to print.
     */
    inline bool Printf(const char8 * const format,
    const AnyType& par1,
    const AnyType& par2,
    const AnyType& par3,
    const AnyType& par4);

    /**
     * @brief Inline method which use pure virtual GetToken. Extract a line and write it on another stream.
     * @param outputBuffer is the output stream where the line must be written.
     * @param skipTerminators defines if the \r char8 should be skipped (true) or not (false).
     * @return depends on the derived classes implementation.
     */
    inline bool GetLine(BufferedStream & output,
    bool skipTerminators = true);

    /**
     * @brief Inline method which use pure virtual GetToken. Extract a line and write it on a char8* buffer.
     * @param outputBuffer is the buffer where the line must be written.
     * @param outputBufferSize is the maximum size of the output buffer.
     * @param skipTerminators defines if the \r char8 should be skipped (true) or not (false).
     * @return depends on the derived classes implementation.
     */
    inline bool GetLine(char8 *outputBuffer,
    const uint32 outputBufferSize,
    bool skipTerminators = true);

protected:
    /**
     * @brief Pure virtual method. Gets the read buffer.
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetInputBuffer() = 0;

    /**
     * @brief Pure virtual method. Get the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetOutputBuffer() = 0;

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

bool BufferedStream::GetLine(BufferedStream & output,
                             const bool skipTerminators) {
    const char8 *skipCharacters = "\r";
#if defined (_WIN32)
    if (!skipTerminators) {
        skipCharacters = "\r";
    }
#else
    if (!skipTerminators) {
        skipCharacters = "";
    }
#endif
    return GetToken(output, "\n", static_cast<char8 *>(NULL),skipCharacters);
}

bool BufferedStream::GetLine(char8 * const outputBuffer,
                             const uint32 outputBufferSize,
                             const bool skipTerminators) {
    const char8 *skipCharacters = "\r";
#if defined (_WIN32)
    if (!skipTerminators) {
        skipCharacters = "\r";
    }
#else
    if (!skipTerminators) {
        skipCharacters = "";
    }
#endif
    return GetToken(outputBuffer, "\n", outputBufferSize, static_cast<char8 *>(NULL),skipCharacters);
}

}

#endif /* BUFFEREDSTREAM_H_ */

