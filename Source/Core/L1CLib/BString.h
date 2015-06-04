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
 * @brief Basic implementation of strings 
 */
#ifndef BASIC_STRING_H
#define BASIC_STRING_H

#include "GeneralDefinitions.h"
#include "Memory.h"
#include <string.h>


/** Predefinition of the class */
class BString;

extern "C" {
/** @brief Exported function to allocate memory for BString.
  * @param  s The BString class
  * @param  allocSize The size of the memory to allocate
  * @return True if successful. False otherwise. */
bool FSAlloc(BString &s, int32 allocSize);

/** @brief Exported function to write the content of buffer into the BString.
  * @param  s The BString class
  * @param  buffer The pointer to the buffer where to read the data from
  * @param  size   The number of bytes to read
  * @return True if successful. False otherwise. */
bool FSRead(BString &s, void* buffer, uint32 &size);

/** @brief Exported function to write the content of BString into a buffer.
  * @param  s The BString class
  * @param  buffer The pointer to the buffer where to write the data to
  * @param  size   The number of bytes to write
  * @return True if successful. False otherwise. */
bool FSWrite(BString &s, const void* buffer, uint32 &size);

}


/**
 * @brief A class to implement basic string operations by operators. 
 *
 * This class contains a char* attribute which represent the string and allows
 * to modify it managing dinamically the memory associated to it. Futhermore the
 * overloading of operators here defined allows to simplify the string operations:
 * assignment, concatenation, comparation ecc. */

class BString {

    friend bool FSAlloc(BString &s, int32 allocSize);
    friend bool FSRead(BString &s, void* buffer, uint32 &size);
    friend bool FSWrite(BString &s, const void* buffer, uint32 &size);

protected:

    /** the size of the allocated memory block  */
    uint32 allocatedSize;

    /** the size of the used memory block -1 (It excludes the 0)  */
    uint32 size;

    /**  the memory buffer  */
    char * buffer;

    /**  the seek position */
    uint32 position;

    /** @brief Used for constructors */
    void InitMembers() {
        size = 0;
        allocatedSize = 0;
        buffer = NULL;
        position = 0;
        FSAlloc(*this, 0);
    }

    /** @brief Used for destructors */
    void FinishMembers() {
        if (buffer != NULL)
            MemoryFree((void *&) buffer);
        size = 0;
        allocatedSize = 0;
        buffer = NULL;
        position = 0;
    }

    /** @brief Copy a character into the BString buffer.
      * @param  c the character to be copied
      * @return True if successful. False otherwise. */
    bool Copy(char c) {
        uint32 wsize = 1;
        size = 0;
        position = 0;
        bool ret = FSWrite(*this, &c, wsize);
        position = 0;
        return ret;
    }

    /** @brief Copy a string into the BString buffer.
      * @param  s The pointer to the string to be copied
      * @return True if successful. False otherwise. */
    bool Copy(const char *s) {
        if (s == NULL)
            return False;
        uint32 wsize = strlen(s);
        size = 0;
        position = 0;
        bool ret = FSWrite(*this, s, wsize);
        position = 0;
        return ret;
    }

    /** @brief Copy a BString into a BString.
      * @param  s The BString to be copied
      * @return True if successful. False otherwise. */
    bool Copy(const BString &s) {
        uint32 wsize = s.size;
        size = 0;
        position = 0;
        bool ret = FSWrite(*this, s.Buffer(), wsize);
        position = 0;
        return ret;
    }

public:

    /** @brief Creates an empty string. */
    inline BString() {
        InitMembers();
    }

    /** @brief Creates a BString as a copy of a BString.
      * @param x The BString to use for initialization. */
    inline BString(const BString &x) {
        InitMembers();
        Copy(x);
    }

    /** @brief Creates a BString as a copy of string.
      * @param x The pointer to the string to use for initialization. */
    inline BString(const char *x) {
        InitMembers();
        Copy(x);
    }

    /** @brief Destructor. */
    virtual ~BString() {
        FinishMembers();
    }

    /** @brief Get the size of the BString.
      * @return The size of the buffer contained in BString. */
    inline uint32 Size() const {
        return size;
    }

    /** @brief Read Only access to the internal buffer.
      * @return The pointer to the buffer. */
    inline const char *Buffer() const {
        return buffer;
    }

    /** @brief Read Write access top the internal buffer.
      * @return The pointer to the buffer. */
    inline char *BufferReference() const {
        return buffer;
    }

    /** @brief Clip the string size to a specified point.
      * @param  size The size of the buffer.
      * @return True if successful. False otherwise. */
    inline bool SetSize(uint32 size) {
        if (FSAlloc(*this, size) == False)
            return False;
        if (size < position)
            position = size;
        this->size = size;
        buffer[size] = 0;
        return True;
    }

    /** @brief Returns a pointer to the tail of the buffer.
      * @param  ix the offset from the end of buffer.
      * @return pointer to the tail of the buffer. */
    inline const char *Tail(int32 ix) const {
        return buffer + size - ix - 1;
    }

    /** @brief Sets BString to be a copy of the input parameter.
      * @param c The character to copy.
      * @return True if successful. False otherwise. */
    inline bool operator=(char c) {
        return Copy(c);
    }

    /** @brief Sets BString to be a copy of the input parameter.
      * @param s The string to copy.
      * @return True if successful. False otherwise. */
    inline bool operator=(const char *s) {
        return Copy(s);
    }

    /** @brief Sets BString to be a copy of the input parameter.
      * @param s The BString to copy
      * @return True if successful. False otherwise. */
    inline bool operator=(const BString &s) {
        return Copy(s);
    }

    /** @brief Concatenate the character to the string contained in the buffer.
      * @param  c The character to concatenate.
      * @return True if successful. False otherwise. */
    inline bool operator+=(const char c) {
        position = size;
        uint32 wsize = 1;
        char temp = c;
        return FSWrite(*this, &temp, wsize);
    }

    /** @brief Concatenate the string to the string contained in the buffer.
      * @param  s The string to concatenate.
      * @return True if successful. False otherwise. */
    inline bool operator+=(const char *s) {
        if (s == NULL)
            return False;
        position = size;
        uint32 wsize = strlen(s);
        return FSWrite(*this, s, wsize);
    }

    /** @brief Concatenate the BString to the string contained in the buffer.
      * @param  s The BString to concatenate.
      * @return True if successful. False otherwise. */
    inline bool operator+=(BString &s) {
        position = size;
        uint32 wsize = s.Size();
        return FSWrite(*this, s.Buffer(), wsize);
    }

    /** @brief Compare the buffer content with the input content.
      * @param s The buffer to be compared with.
      * @return True if the two buffers are the same. False otherwise. */
    inline bool operator==(BString &s) const {
        if (size != s.size)
            return False;
        if (strcmp(buffer, s.buffer) != 0)
            return False;
        return True;
    }

    /** @brief Compare the buffer content with the input content.
      * @param s The buffer to be compared with.
      * @return True if the two buffers are the same. False otherwise. */
    inline bool operator==(const char *s) const {
        if (s == NULL)
            return False;
        if (strcmp(buffer, s) != 0)
            return False;
        return True;
    }

    /** @brief Compare the buffer content with the input content.
      * @param s The buffer to be compared with.
      * @return True if the two buffers are different. False otherwise. */
    inline bool operator!=(BString &s) const {
        return !((*this) == s);
    }

    /** @brief Compare the buffer content with the input content.
      * @param s The buffer to be compared with.
      * @return True if the two buffers are different. False otherwise. */
    inline bool operator!=(const char *s) const {
        return !((*this) == s);
    }

    /** @brief Allows access to character within the buffer.
      * @param  pos The position in the buffer to be accessed.
      * @return 0 if the position is outside the buffer limits. The character at position pos otherwise. */
    inline char operator[](uint32 pos) {
        if (pos >= size)
            return 0; // was -1 ?? Anton ??
        return buffer[pos];
    }

    /** @brief Checks if a char is in the string.
      * @param c The character to look for.
      * @return True if found. False otherwise. */
    inline bool In(char c) const {
        for (uint32 i = 0; i < size; i++)
            if (buffer[i] == c)
                return True;
        return False;
    }

    /** Checks if a string is contained in the string.
      * @param x The string to look for.
      * @return True if the string is found. False otherwise. */
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

