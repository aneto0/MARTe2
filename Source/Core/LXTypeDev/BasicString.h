/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/

/** 
 * @file
 * Basic implementation of strings 
 */
#ifndef BASIC_STRING_H
#define BASIC_STRING_H

#include "GeneralDefinitions.h"
#include "Memory.h"
#include "CharBuffer.h"
//#include <string.h>

/** Predefinition of the class */
class BasicString;

extern "C" {


/** Exported function to read the content of BString from a buffer.
 @param  s The BString class
 @param  buffer The pointer to the buffer where to read the data from
 @param  position where to write from
 @param  size   The number of bytes to write
 @return True if successful. False otherwise.
 */
bool BSWrite(BasicString &s, const void* buffer, uint32 position, uint32 &size);

}
/**
    A basic implementation of a string.
    A replacement for dealing directly with mallocs
    BString simplified no confusing position 
           and derived from CharBuffer 
    The string model is simple, It can do only:
    Construct as copy of char * or other BasicString
    Assign    as copy of char * or other BasicString
    Concatenate       a  char * or other BasicString
    Truncate to a given size
    Compare with      a  char *    
    Access as char * both read-only and read-write
*/
class BasicString: public CharBuffer {

    friend bool BSWrite(BasicString &s, const void* buffer, uint32 position, uint32 &size);

protected:
    /** sets the size of the buffer so that to fit a fitStringSize
        accounts for string terminator
    */
    inline void SetStringBufferSize(uint32 fitStringSize){
        uint32 desiredBufferSize = fitStringSize+1;
        // 32 bytes granularity
        CharBuffer::SetBufferSize(desiredBufferSize, 0xFFFFFFE0);
    }

    /** the size of the used memory block -1 (It excludes the 0)  */
    uint32 size;

    /** used for constructors */
    void InitMembers() {
        size = 0;
        SetStringBufferSize(0);
    }

    /** used for destructors */
    void FinishMembers() {
        size = 0;
    }

    /** Copy a character into the BString buffer.
     @param  c the character to be copied
     @return True if successful. False otherwise.
     */
    bool Copy(char c) {
        uint32 wsize = 1;
        size = 0;
        bool ret = BSWrite(*this, &c, 0, wsize);
        return ret;
    }

    /** Copy a string into the BString buffer.
     @param  s The pointer to the string to be copied
     @return True if successful. False otherwise.
     */
    bool Copy(const char *s) {
        if (s == NULL)
            return False;
        uint32 wsize = strlen(s);
        size = 0;
        bool ret = BSWrite(*this, s, 0, wsize);
        return ret;
    }

    /** Copy a BString into a BString.
     @param  s The BString to be copied
     @return True if successful. False otherwise.
     */
    bool Copy(const BasicString &s) {
        uint32 wsize = s.size;
        size = 0;
        bool ret = BSWrite(*this, s.Buffer(), 0, wsize);
        return ret;
    }

public:

    /** Creates an empty string */
    inline BasicString() {
        InitMembers();
    }

    /** Creates a BString as a copy of a BString.
     @param x The BString to use for initialisation
     */
    inline BasicString(const BString &x) {
        InitMembers();
        Copy(x);
    }

    /** Creates a BString as a copy of string
     @param x The pointer to the string to use for initialisation
     */
    inline BasicString(const char *x) {
        InitMembers();
        Copy(x);
    }

    /** Destructor */
    virtual ~BasicasicString() {
        FinishMembers();
    }

    /** The size of the string.
     @return The size of the buffer contained in BString
     */
    inline uint32 Size() const {
        return size;
    }

    /** Read Only access to the internal buffer
     @return The pointer to the buffer
     */
    inline const char *Buffer() const {
        return buffer;
    }

    /** Read Write access top the internal buffer
     @return The pointer to the buffer
     */
    inline char *BufferReference() const {
        return buffer;
    }

    /** Clip the string size to a specified point
     @param newStringSize The size of the buffer.
     @return True if successful. False otherwise.
     */
    inline bool SetSize(uint32 newStringSize) {
        SetStringBufferSize(newStringSize);
        size = newStringSize;
        uint32 maxSize = BufferAllocatedSize() - 1;
        if (size > maxSize) size = maxSize;
        buffer[size] = 0;
        return True;
    }

    /** Returns a pointer to the tail of the buffer.
     @param  ix the offset from the end of buffer
     @return pointer to the tail of the buffer
     */
    inline const char *Tail(int32 ix) const {
        return buffer + size - ix - 1;
    }

    /** Sets BString to be a copy of the input parameter.
     @param c The character to copy
     @return True if successful. False otherwise.
     */
    inline bool operator=(char c) {
        return Copy(c);
    }

    /** Sets BString to be a copy of the input parameter.
     @param s The string to copy
     @return True if successful. False otherwise.
     */
    inline bool operator=(const char *s) {
        return Copy(s);
    }

    /** Sets BString to be a copy of the input parameter.
     @param s The BString to copy
     @return True if successful. False otherwise.
     */
    inline bool operator=(const BString &s) {
        return Copy(s);
    }

    /** Concatenate the character to the string contained in the buffer
     @param  c The character to concatenate
     @return True if successful. False otherwise.
     */
    inline bool operator+=(const char c) {
        uint32 wsize = 1;
        char temp = c;
        return BSWrite(*this, &temp, size, wsize);
    }

    /** Concatenate the string to the string contained in the buffer
     @param  s The string to concatenate
     @return True if successful. False otherwise.
     */
    inline bool operator+=(const char *s) {
        if (s == NULL)
            return False;
        uint32 wsize = strlen(s);
        return BSWrite(*this, s, size, wsize);
    }

    /** Concatenate the BString to the string contained in the buffer
     @param  s The BString to concatenate
     @return True if successful. False otherwise.
     */
    inline bool operator+=(BString &s) {
        uint32 wsize = s.Size();
        return BSWrite(*this, s.Buffer(), size, wsize);
    }

    /** Compare the buffer content with the input content
     @param s The buffer to be compared with
     @return True if the two buffers are the same. False otherwise.
     */
    inline bool operator==(BString &s) const {
        if (size != s.size)
            return False;
        if (strcmp(buffer, s.buffer) != 0)
            return False;
        return True;
    }

    /** Compare the buffer content with the input content
     @param s The buffer to be compared with
     @return True if the two buffers are the same. False otherwise.
     */
    inline bool operator==(const char *s) const {
        if (s == NULL)
            return False;
        if (strcmp(buffer, s) != 0)
            return False;
        return True;
    }

    inline bool operator!=(BString &s) const {
        return !((*this) == s);
    }

    inline bool operator!=(const char *s) const {
        return !((*this) == s);
    }

    /** Allows access to character within the buffer
     @param  pos The position in the buffer to be accessed.
     @return 0 if the position is outside the buffer limits. The character at position pos otherwise.
     */
    inline char operator[](uint32 pos) {
        if (pos >= size)
            return 0; // was -1 ?? Anton ??
        return buffer[pos];
    }

    /** Checks if a char is in the string
     @param c The character to look for.
     @return True if found. False otherwise.
     */
    inline bool In(char c) const {
        for (uint32 i = 0; i < size; i++)
            if (buffer[i] == c)
                return True;
        return False;
    }

    /** Checks if a string is contained in the string.
     @param x The string to look for.
     @return True if the string is found. False otherwise.
     */
    inline bool In(BString &x) const {
        if (x.Size() == 0)
            return False;
        for (uint32 i = 0; i < (size - x.Size() + 1); i++)
            if (memcmp(&buffer[i], x.Buffer(), x.Size()) == 0)
                return True;
        return False;
    }

};

#endif

