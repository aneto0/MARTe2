/**
 * @file StreamString.h
 * @brief Header file for class StreamString
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

 * @details This header file contains the declaration of the class StreamString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMSTRING_H_
#define STREAMSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "CharBuffer.h"
#include "StreamStringIOBuffer.h"
#include "BufferedStream.h"
#include "TimeoutType.h"
#include "StringHelper.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/** @brief StreamString class. */
class StreamString: public BufferedStream {

public:
    // usable constructors

    StreamString();

    /** @brief Copy constructor from a const char8*. */
    StreamString(const char8 * const initialisationString);

    /** @brief Destructor */
    virtual ~StreamString();

    /** @brief Automatic cast to AnyType as a const char8 passing Buffer() return value. */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The StreamString is considered as a string and not as a standard stream*/
    operator AnyType();

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Reads data into buffer.
     * @param buffer is the output buffer.
     * @param size is the number of bytes to copy.
     * @param msecTimeout is the timeout.
     * @param complete is a flag which specifies if the read operation is completed.
     * @return false in case of errors.
     *
     * This function calls IOBuffer::Read. See it for more informations.
     *  As much as size byte are read,
     *  actual read size is returned in size. (unless complete = true)
     *  msecTimeout is how much the operation should last - no more - if not any (all) data read then return false
     *  timeout behaviour depends on class characteristics and sync mode.
     */
    virtual bool Read(char8* bufferIn,
                      uint32 & size);

    /**
     * @brief Write from a buffer to the string.
     * @param buffer is the input buffer.
     * @param size is the number of bytes to copy.
     * @param msecTimeout is the timeout.
     * @param complete is a flag which specifies if the write operation is completed.
     * @return false in case of errors.
     *
     * This function calls IOBuffer::Write. See it for more infomations.
     *  As much as size byte are written,
     *  actual written size is returned in size.
     *  msecTimeout is how much the operation should last.
     *  timeout behaviour depends on class characteristics and sync mode.
     */
    virtual bool Write(const char8* bufferIn,
                       uint32 & size);

    /**
     * @brief Specifies if the string can be written.
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @brief Specifies if the string can be read.
     * @return true. */
    virtual bool CanRead() const;

    /**
     * @brief The size of the string.
     * @return the current string size.
     *
     * StreamStringIOBuffer type allocates memory dinamically on the heap.
     * It returns the size of the filled memory. */
    virtual uint64 Size();

    /**
     * @brief Moves within the string to an absolute location.
     * @param pos is the desired cursor position.
     * @return false in case of cursor out of ranges or other errors.
     *
     * If the position is out of ranges, position becomes the end of the stream.
     * This function calls IOBuffer::Seek, see it for more informations. */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Moves within the string relative to current location.
     * @param deltaPos is the desired gap from the current position.
     * @return false in case of cursor out of ranges or other errors.
     *
     * If the desired position falls out of ranges, position becomes one of the bounds.
     * This function calls IOBuffer::RelativeSeek, see it for more informations. */
    virtual bool RelativeSeek(int32 deltaPos);

    /**
     * @brief Returns current position.
     * @return the current position.
     *
     * This function calls IOBuffer::Position, see it for more informations. */
    virtual uint64 Position();

    /**
     * @brief Set the size of the buffer.
     * @param size The desired size of the buffer.
     * @return true if successful. false otherwise.
     *
     * This function calls IOBuffer::SetAllocationSize, passing size+1 for the final \0.
     */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Specifies if you can move the cursor.
     * @return true.
     */
    virtual bool CanSeek() const;

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Read Only access to the internal buffer.
     * @return The pointer to the buffer.
     *
     * This function returns the pointer at the beginning of the buffer. It is a CharBuffer function.
     * NOTE this pointer may not be conserved as it might be invalid after any write operation
     * this is because a realloc is used
     * The final \0 is added before the return.
     */
    inline const char8 *Buffer();

    /**
     * @brief Read Write access top the internal buffer.
     * @return The pointer to the buffer.
     *
     * This function returns the pointer at the beginning of the buffer. It is a CharBuffer function.
     * The final \0 is added before the return.
     */
    inline char8 *BufferReference();

    /**
     * @brief Returns a pointer to the tail of the buffer.
     * @param  ix the offset from the end of buffer. valid ranges is 0 to Size()-1.
     * @return pointer to the tail of the buffer.
     */
    inline const char8 *Tail(uint32 ix) const;

    /*---------------------------------------------------------------------------*/

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param c The character to copy.
     * @return true if successful. false otherwise.
     */
    inline bool operator=(const char8 c);

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param s The string to copy.
     * @return true if successful. false otherwise.
     */
    inline bool operator=(const char8 * const s);

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param s The StreamString to copy.
     * @return true if successful. false otherwise.
     */
    inline StreamString& operator=(const StreamString &s);

    /**
     * @brief Concatenate the character to the string contained in the buffer.
     * @param  c The character to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(const char8 c);

    /**
     * @brief Concatenate the string to the string contained in the buffer.
     * @param s The string to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(const char8 * const s);

    /**
     * @brief Concatenate the StreamString to the string contained in the buffer.
     * @param  s The StreamString to concatenate.
     * @return true if successful. false otherwise.
     */
    inline bool operator+=(const StreamString &s);

    /**
     * @brief  Compare the buffer content with the input content.
     * @param s The buffer to be compared with.
     * @return true if the two buffers are the same. false otherwise.
     */
    inline bool operator==(const StreamString &s) const;

    /**
     * @brief Compare the buffer content with the input content.
     * @param s The buffer to be compared with.
     * @return true if the two buffers are the same. false otherwise.
     */
    inline bool operator==(const char8 * const s) const;

    /**
     * @brief Compare the buffer content with the input content.SOURCE_
     * @param s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise. */
    inline bool operator!=(const StreamString &s) const;

    /**
     * @brief Compare the buffer content with the input content.
     * @param s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise. */
    inline bool operator!=(const char8 * const s) const;

    /**
     * @brief Allows access to character within the buffer.
     * @param  pos The position in the buffer to be accessed.
     * @return 0 if the position is outside the buffer limits. The character at position pos otherwise.
     */
    inline char8 operator[](const uint32 pos) const;

    /**
     * @brief Checks if a char8 is in the string
     * @param c The character to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(const char8 c) const;

    /**
     * @brief Checks if a string is contained in the string.
     * @param x The string to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(const StreamString &x) const;

protected:
    // methods to be implemented by deriving classes

    /**
     * @brief Returns buffer as the read buffer. It is both read and write buffer.
     * @return a pointer to the StreamStringIOBuffer buffer.
     */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Returns buffer as the write buffer.
     * @return a pointer to the StreamStringIOBuffer buffer. */
    virtual IOBuffer *GetOutputBuffer();

private:
    /**
     * @brief Copy a character into the StreamString buffer or append it at the end.
     * @param  c the character to be copied.
     * @param append defines if the character must be appended or assigned.
     * @return true if successful. false otherwise.
     *
     * If append is true there is an IOBuffer::Seek at the buffer size and then a IOBuffer::PutC.
     * If append is false the string is cleaned calling IOBuffer::Empty and then used a IOBuffer::PutC.
     */
    bool AppendOrSet(const char8 c,
                     const bool append);

    /**
     * @brief Copy a string into the StreamString buffer.
     * @param  s The pointer to the string to be copied or appended.
     * @param append defines if s must be appended or assigned.
     * @return true if successful. false otherwise.
     */
    bool AppendOrSet(const char8 * const s,
                     const bool append);

    /**
     * @brief Copy a StreamString into a StreamString.
     * @param  s The StreamString to be copied.
     * @param append defines if s must be appended or assigned.
     * @return true if successful. false otherwise.
     */
    bool AppendOrSet(const StreamString &s,
                     const bool append);

    /**
     * All read and write operations are performed on this buffer.
     * Also if each streamable implementation has its own read and write buffers,
     * defining all of them as descendants of IOBuffers specific functions are called
     * passing truth the virtual table.
     * The function IOBuffer::Write and IOBuffer::NoMoreSpaceToWrite are overloaded to reallocate new memory in case of full buffer.*/
    StreamStringIOBuffer buffer;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

const char8 *StreamString::Buffer() {
    buffer.Terminate();
    return buffer.Buffer();
}

char8 *StreamString::BufferReference() {
    buffer.Terminate();
    return buffer.BufferReference();
}

const char8 *StreamString::Tail(const uint32 ix) const {
    bool ok = (ix <= (buffer.UsedSize() - 1u));

    return ok ? &(buffer.BufferReference()[(buffer.UsedSize() - ix) - 1u]) : static_cast<const char8 *>(NULL);
}

bool StreamString::operator=(const char8 c) {
    return AppendOrSet(c, false);

}

bool StreamString::operator=(const char8 * const s) {
    return AppendOrSet(s, false);
}

StreamString& StreamString::operator=(const StreamString &s) {
    if (!AppendOrSet(s, false)) {
        //TODO
    }
    return *this;
}

bool StreamString::operator+=(const char8 c) {
    return AppendOrSet(c, true);
}

bool StreamString::operator+=(const char8 * const s) {
    return AppendOrSet(s, true);
}

bool StreamString::operator+=(const StreamString &s) {
    return AppendOrSet(s, true);
}

bool StreamString::operator==(const StreamString &s) const {
    bool ok1 = (buffer.UsedSize() == s.buffer.UsedSize());
    bool ok2 = (StringHelper::CompareN(buffer.Buffer(), s.buffer.Buffer(), buffer.UsedSize()) == 0);

    return ok1 && ok2;
}

bool StreamString::operator==(const char8 * const s) const {
    bool ok1 = (s != NULL);
    bool ok2=(StringHelper::Length(s) == buffer.UsedSize());
    bool ok3=(StringHelper::CompareN(buffer.Buffer(),s,buffer.UsedSize()) == 0);
    return (ok1) && (ok2) && (ok3);

}

bool StreamString::operator!=(const StreamString &s) const {
    return !((*this) == s);
}

bool StreamString::operator!=(const char8 * const s) const {
    return !((*this) == s);
}

char8 StreamString::operator[](const uint32 pos) const {
    char8 ret = static_cast<char8>(0);
    if (pos < buffer.UsedSize()) {
        ret = buffer.BufferReference()[pos];
    }
    return ret;
}

}
#endif /* STREAMSTRING_H_ */

