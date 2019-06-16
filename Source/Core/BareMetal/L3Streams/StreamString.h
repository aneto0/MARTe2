/**
 * @file StreamString.h
 * @brief Header file for class StreamString
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class StreamString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L3STREAMS_STRING_H_
#define L3STREAMS_STRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "CharBuffer.h"
#include "BufferedStreamI.h"
#include "StringHelper.h"
#include "AnyType.h"
#include "StreamStringIOBuffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A basic implementation of a stream which allows reading and writing
 * to an underlying unbounded string, which can grow dynamically.
 */
class DLL_API StreamString: public BufferedStreamI {

public:

    /**
     * @brief Default constructor
     * @pre
     *   true
     * @post
     *   Buffer() != NULL &&
     *   CanRead() &&
     *   CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == 0
     */
    StreamString();

    /**
     * @brief Constructor from a null terminated C-string
     * @param[in] initialisationString is the char8 pointer of the
     * null terminated C-string.
     * @pre
     *   true
     * @post
     *   Buffer() != NULL &&
     *   CanRead() &&
     *   CanWrite() &&
     *   CanSeek() &&
     *   Position() == 0 &&
     *   Size() == StringHelper::Length(initialisationString)
     */
    StreamString(CCString const initialisationString);

    /**
     * @brief Copy Constructor.
     *
     */
    StreamString(const StreamString &toCopy);

    /**
     * @brief Destructor
     */
    virtual ~StreamString();

    /**
     * @brief Automatic cast to AnyType as a const char8 passing Buffer() return value.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The StreamString is considered as a string and not as a standard stream*/
    operator AnyType();

    /*-----------------------------------------------------------------------*/

    /**
     * @brief Reads data into a buffer.
     * @param[out] output is the buffer where data must be written to.
     * @param[in,out] size is the number of bytes to copy. This value will be
     * updated with the bytes actually read.
     * @return false if errors on copying data
     */
    virtual bool Read(char8* const output, uint32 & size);

    /**
     * @brief Write from a buffer to the string.
     * @param[in] input is the buffer with the data to be written from.
     * @param[in,out] size is the number of bytes to copy. This value will be
     * updated with the bytes actually written.
     * @return false if errors on copying data
     */
    virtual bool Write(const char8 * const input, uint32 & size);

    /**
     * @brief Default implementation for buffered streams: calls StreamI::Read(*)
     * with infinite timeout.
     * @see StreamI::Read(*).
     */
    virtual bool Read(char8 * const output, uint32 & size, const MilliSeconds &timeout);

    /**
     * @brief Default implementation for buffered streams: calls StreamI::Write(*)
     * with infinite timeout.
     * @see StreamI::Write(*)
     */
    virtual bool Write(const char8 * const input, uint32 & size, const MilliSeconds &timeout);

    /**
     * @brief Queries if the stream is writable.
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @brief Queries if the stream is readable.
     * @return true.
     */
    virtual bool CanRead() const;

    /**
     * @brief Queries if seek operations can be performed on the stream.
     * @return true.
     */
    virtual bool CanSeek() const;

    /**
     * @brief Gets the size of the stream.
     * @return the current stream size.
     */
    virtual uint64 Size() const;

    /**
     * @brief Moves within the string to an absolute location.
     * @param[in] pos is the desired cursor position.
     * @return false in case of cursor out of ranges or other errors.
     */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Moves within the stream relative to current location.
     * @details Checks that the final position is >= 0 and <= UsedSize, then
     * moves the cursor.\n
     * -If the final position is < 0 moves the cursor at the beginning.\n
     * -If the final position is > UsedSize moves the cursor at the end.\n
     * @param[in] deltaPos is the gap from the current position.
     * @return false if the position falls out of bounds or if \a deltaPos is outside the int32
     * range of values (supported by IOBuffer::RealtiveSeek(*)).
     * @pre
     *   (deltaPos <= MAX_INT32) && (deltaPos >= MIN_INT32)
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Gets the current position.
     * @return the current position.
     */
    virtual uint64 Position() const ;

    /**
     * @brief Sets the size of the buffer.
     * @param[in] size The desired size of the buffer.
     * @return true if successful. false otherwise.
     */
    virtual bool SetSize(uint64 size);

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Gets a pointer to the beginning of the internal buffer with read
     * only access.
     * @details The final \0 is added before the return.
     * @return The pointer to the internal buffer.
     * @warning This pointer may not be conserved as it might be invalid after
     * any write operation, because a realloc is used.
     */
    inline CCString Buffer();

    /**
     * @brief Gets a pointer to the beginning of the internal buffer with read
     * write access.
     * @details The final \0 is added before the return.
     * @return The pointer to the internal buffer.
     */
    inline CString BufferReference();

    /**
     * @brief Gets a pointer to the tail of the internal buffer with read only
     * access.
     * @param[in] ix the offset from the end of buffer.
     * @return pointer to the tail of the internal buffer.
     * @pre ix >= 0 && ix < Size().
     */
    inline CCString Tail(const uint32 ix) const;

    /*-----------------------------------------------------------------------*/

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param[in] c The character to copy.
     * @return true if successful. false otherwise.
     */
    inline bool operator=(const char8 c);

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param[in] s The string to copy.
     * @return true if successful. false otherwise.
     */
    inline bool operator=(CCString s);

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param[in] s The StreamString to copy.
     * @return true if successful. false otherwise.
     */
    inline StreamString& operator=(const StreamString &s);

    /**
     * @brief Concatenate the character to the string contained in the buffer.
     * @param[in] c The character to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(const char8 c);

    /**
     * @brief Concatenate the string to the string contained in the buffer.
     * @param[in] s The string to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(CCString const  s);

    /**
     * @brief Concatenate the StreamString to the string contained in the buffer.
     * @param[in] s The StreamString to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(const StreamString &s);

    /**
     * @brief  Compare the buffer content with the input content.
     * @param[in] s The buffer to be compared with.
     * @return true if the two buffers are the same. false otherwise.
     */
    inline bool operator==(const StreamString &s) const;

    /**
     * @brief Compare the buffer content with the input content.
     * @param[in] s The buffer to be compared with.
     * @return true if the two buffers are the same. false otherwise.
     */
    inline bool operator==(CCString const  s) const;

    /**
     * @brief Compare the buffer content with the input content.SOURCE_
     * @param[in] s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise.
     */
    inline bool operator!=(const StreamString &s) const;

    /**
     * @brief Compare the buffer content with the input content.
     * @param[in] s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise.
     */
    inline bool operator!=(CCString const  s) const;

    /**
     * @brief Allows access to character within the buffer.
     * @param[in] pos The position in the buffer to be accessed.
     * @return 0 if the position is outside the buffer limits. The character at position pos otherwise.
     */
    inline char8 operator[](const uint32 pos) const;

    /**
     * @brief Checks if a character is contained in the string
     * @param[in] c The character to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(const char8 c) const;

    /**
     * @brief Checks if a StreamString is contained in the string.
     * @param[in] x The stream string to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(const StreamString &x) const;

protected:

    /**
     * @brief Gets the read buffer.
     * @return a pointer to the read buffer.
     */
    virtual IOBuffer *GetReadBuffer();

    /**
     * @brief Gets the write buffer.
     * @return a pointer to the write buffer.
     */
    virtual IOBuffer *GetWriteBuffer();

private:

    /**
     * @brief Appends a character at the end of the buffer.
     * @param[in] c the character to be copied.
     * @return true if successful. false otherwise.
     */
    bool Append(const char8 c);

    /**
     * @brief Sets a character to the buffer.
     * @param[in] c the character to be copied.
     * @return true if successful. false otherwise.
     */
    bool Set(const char8 c);

    /**
     * @brief Appends a C-string at the end of the buffer.
     * @param[in] s The pointer to the string to be copied or appended.
     * @return true if successful. false otherwise.
     */
    bool Append(CCString const  s);

    /**
     * @brief Sets a C-string to the buffer.
     * @param[in] s The pointer to the string to be copied or appended.
     * @return true if successful. false otherwise.
     */
    bool Set(CCString const  s);

    /**
     * @brief Appends a StreamString at the end of the buffer.
     * @param[in] s The StreamString to be copied.
     * @return true if successful. false otherwise.
     */
    bool Append(const StreamString &s);

    /**
     * @brief Sets a StreamString to the buffer.
     * @param[in] s The StreamString to be copied.
     * @return true if successful. false otherwise.
     */
    bool Set(const StreamString &s);

    /**
     * All read and write operations are performed on this buffer.
     * Also if each streamable implementation has its own read and write buffers,
     * defining all of them as descendants of IOBuffers specific functions are called
     * passing through the virtual table.
     * The function IOBuffer::Write and IOBuffer::NoMoreSpaceToWrite are overloaded to reallocate new memory in case of full buffer.
     */
    StreamStringIOBuffer buffer;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString StreamString::Buffer() {
    buffer.Terminate();
    return buffer.Buffer();
}

CString StreamString::BufferReference() {
    buffer.Terminate();
    return CString(buffer.BufferReference());
}

CCString StreamString::Tail(const uint32 ix) const {
    const char8* result;
    bool ok = (ix <= (buffer.UsedSize() - 1u));
    if (ok) {
        const char8* bufref = buffer.Buffer();
        result = &(bufref[(buffer.UsedSize() - ix) - 1u]);
    }
    else {
        result = static_cast<const char8 *>(NULL);
    }
    return result;
}

bool StreamString::operator=(CCString s) {
    return Set(s);
}

StreamString& StreamString::operator=(const StreamString &s) {
    if (&s != this) {
        if (!Set(s)) {
            //TODO
        }
    }
    return *this;
}

bool StreamString::operator+=(const char8 c) {
    return Append(c);
}

bool StreamString::operator+=(CCString const  s) {
    return Append(s);
}

bool StreamString::operator+=(const StreamString &s) {
    return Append(s);
}

bool StreamString::operator==(const StreamString &s) const {
    bool ok1 = (buffer.UsedSize() == s.buffer.UsedSize());
    bool ok2 = CCString(buffer.Buffer()).IsSameAs(s.buffer.Buffer(),buffer.UsedSize());
//    bool ok2 = (StringHelper::CompareN(buffer.Buffer(), s.buffer.Buffer(), buffer.UsedSize()) == 0);

    return ok1 && ok2;
}

bool StreamString::operator==(CCString const  s) const {
    bool ok1 = !(s.IsNullPtr());
    bool ok2 = (s.GetSize() == buffer.UsedSize());
    bool ok3 = s.IsSameAs(buffer.Buffer(),buffer.UsedSize());
//    bool ok3 = (StringHelper::CompareN(buffer.Buffer(), s, buffer.UsedSize()) == 0);
    return (ok1) && (ok2) && (ok3);
}

bool StreamString::operator!=(const StreamString &s) const {
    return !((*this) == s);
}

bool StreamString::operator!=(CCString const  s) const {
    return !((*this) == s);
}

char8 StreamString::operator[](const uint32 pos) const {
    char8 ret = static_cast<char8>(0);
    if (pos < buffer.UsedSize()) {
        ret = buffer.Buffer()[pos];
    }
    return ret;
}

}
#endif /* L3STREAMS_STRING_H_ */

