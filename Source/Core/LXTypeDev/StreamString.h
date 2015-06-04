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
 * @file StreamString.h
 * @brief Basic implementation of strings. 
 *
 *  A basic implementation of a string.
 *  A replacement for dealing directly with mallocs
 *  StreamString simplified no confusing position 
 *  and derived from CharBuffer. 
 *  The string model is simple, It can do only:
 *  Construct as copy of char * or other BasicString
 *  Assign    as copy of char * or other BasicString
 *  Concatenate       a  char * or other BasicString
 *  Truncate to a given size
 *  Compare with      a  char *    
 *  Access as char * both read-only and read-write
*/




#ifndef STREAM_STRING_H
#define STREAM_STRING_H

#include "GeneralDefinitions.h"
#include "Memory.h"
#include "CharBuffer.h"
#include "StreamStringIOBuffer.h"
#include "Streamable.h"
#include "TimeoutType.h"
#include "StringHelper.h"
#include "AnyType.h"



/** @brief StreamString class. */
class StreamString: public Streamable {

private:    

    /**
     * All read and write operations are performed on this buffer.
     * Also if each streamable implementation has its own read and write buffers,
     * defining all of them as descendants of IOBuffers specific functions are called 
     * passing truth the virtual table.
     * The function IOBuffer::Write and IOBuffer::NoMoreSpaceToWrite are overloaded to reallocate new memory in case of full buffer.*/
     StreamStringIOBuffer 	buffer;	
   
protected: // methods to be implemented by deriving classes

    /**
     * @brief Returns buffer as the read buffer. It is both read and write buffer.
     * @return a pointer to the StreamStringIOBuffer buffer.
    */
    virtual IOBuffer *GetInputBuffer();

    /**
     * @brief Returns buffer as the write buffer.
     * @return a pointer to the StreamStringIOBuffer buffer. */
    virtual IOBuffer *GetOutputBuffer();

public: // usable constructors

    /** @brief Copy constructor from a const char*. */
    StreamString(const char *initialisationString=NULL){
    	if (initialisationString != NULL) AppendOrSet(initialisationString,false);
    }
    
    /** @brief Destructor */
    virtual ~StreamString() ;
    
    /** @brief Automatic cast to AnyType as a const char passing Buffer() return value. */
    operator AnyType(){
    	AnyType at(Buffer());
    	return at;
    }
    
public:
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
     *  actual read size is returned in size. (unless complete = True)
     *  msecTimeout is how much the operation should last - no more - if not any (all) data read then return false  
     *  timeout behaviour depends on class characteristics and sync mode.
    */
    virtual bool        Read(
                            char*               buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false);

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
    virtual bool        Write(
                            const char*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false);
    
    /**
      * @brief Specifies if the string can be written.
      * @return true.
      */
    virtual bool        CanWrite() const ;

    /**
     * @brief Specifies if the string can be read.
     * @return true. */
    virtual bool        CanRead() const ;
    
    /**
     * @brief The size of the string.
     * @return the current string size.
     *
     * StreamStringIOBuffer type allocates memory dinamically on the heap.
     * It returns the size of the filled memory. */
    virtual int64       Size() ;

    /**
     * @brief Moves within the string to an absolute location.
     * @param pos is the desired cursor position.
     * @return false in case of cursor out of ranges or other errors.
     * 
     * If the position is out of ranges, position becomes the end of the stream.
     * This function calls IOBuffer::Seek, see it for more informations. */
    virtual bool        Seek(int64 pos);
    
    /** 
     * @brief Moves within the string relative to current location.
     * @param deltaPos is the desired gap from the current position.
     * @return false in case of cursor out of ranges or other errors.
     * 
     * If the desired position falls out of ranges, position becomes one of the bounds.
     * This function calls IOBuffer::RelativeSeek, see it for more informations. */
    virtual bool        RelativeSeek(int32 deltaPos);
    
    /** 
     * @brief Returns current position.
     * @return the current position.
     *
     * This function calls IOBuffer::Position, see it for more informations. */
    virtual int64       Position() ;

    /**
     * @brief Set the size of the buffer.
     * @param size The desired size of the buffer.
     * @return True if successful. False otherwise.
     * 
     * This function calls IOBuffer::SetAllocationSize, passing size+1 for the final \0.
     */
    virtual bool        SetSize(int64 size);

    /** 
     * @brief Specifies if you can move the cursor.
     * @return true.
     */
    virtual bool        CanSeek() const ;
   
public: // DIRECT ACCESS FUNCTIONS
      

    /**
     * @brief Read Only access to the internal buffer.
     * @return The pointer to the buffer.
     *
     * This function returns the pointer at the beginning of the buffer. It is a CharBuffer function.
     * NOTE this pointer may not be conserved as it might be invalid after any write operation
     * this is because a realloc is used
     * The final \0 is added before the return.
     */
    inline const char *Buffer()  {
    	buffer.Terminate();
        return buffer.Buffer();
    }

    /**
     * @brief Read Write access top the internal buffer.
     * @return The pointer to the buffer.
     *
     * This function returns the pointer at the beginning of the buffer. It is a CharBuffer function.
     * The final \0 is added before the return.
     */
    inline char *BufferReference()  {
    	buffer.Terminate();
        return buffer.BufferReference();
    }


    /**
     * @brief Returns a pointer to the tail of the buffer.
     * @param  ix the offset from the end of buffer. valid ranges is 0 to Size()-1.
     * @return pointer to the tail of the buffer.
     */
    inline const char *Tail(int32 ix) const {
    	if (ix > 0) 				return 0;
    	if ((ix - buffer.UsedSize() -1)< 0) 	return 0;
    	return buffer.BufferReference() + buffer.UsedSize() - ix - 1;
    }

private: // DIRECT MANIPULATION FUNCTIONS

    /**
     * @brief Copy a character into the StreamString buffer or append it at the end.
     * @param  c the character to be copied.
     * @param append defines if the character must be appended or assigned.
     * @return True if successful. False otherwise.
     *
     * If append is true there is an IOBuffer::Seek at the buffer size and then a IOBuffer::PutC.
     * If append is false the string is cleaned calling IOBuffer::Empty and then used a IOBuffer::PutC.
     */
    virtual bool AppendOrSet(char c, bool append=true) ;

    /**
     * @brief Copy a string into the StreamString buffer.
     * @param  s The pointer to the string to be copied or appended.
     * @param append defines if s must be appended or assigned.
     * @return True if successful. False otherwise.
     */
    virtual bool AppendOrSet(const char *s, bool append=true) ;

    /**
     * @brief Copy a StreamString into a StreamString.
     * @param  s The StreamString to be copied.
     * @param append defines if s must be appended or assigned.
     * @return True if successful. False otherwise.
     */
    virtual bool AppendOrSet(const StreamString &s, bool append=true) ;
public:    
    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param c The character to copy.
     * @return True if successful. False otherwise.
     */
    inline bool operator=(char c) {
    	return AppendOrSet(c,false);
    }

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param s The string to copy.
     * @return True if successful. False otherwise.
     */
    inline bool operator=(const char *s) {
        return AppendOrSet(s,false);
    }

    /**
     * @brief Sets StreamString to be a copy of the input parameter.
     * @param s The StreamString to copy.
     * @return True if successful. False otherwise.
     */
    inline bool operator=(const StreamString &s) {	
        return AppendOrSet(s,false);
    }

    /**
     * @brief Concatenate the character to the string contained in the buffer.
     * @param  c The character to concatenate.
     * @return True if successful. False otherwise.
     */
    inline bool operator+=(const char c) {
    	return AppendOrSet(c,true);
    }

    /**
     * @brief Concatenate the string to the string contained in the buffer.
     * @param s The string to concatenate.
     * @return True if successful. False otherwise.
     */
    inline bool operator+=(const char *s) {
    	return AppendOrSet(s,true);
    }

    /**
     * @brief Concatenate the StreamString to the string contained in the buffer.
     * @param  s The StreamString to concatenate.
     * @return True if successful. False otherwise.
     */
    inline bool operator+=(StreamString &s) {
    	return AppendOrSet(s,true);
    }

    /**
     * @brief  Compare the buffer content with the input content.
     * @param s The buffer to be compared with.
     * @return True if the two buffers are the same. False otherwise.
     */
    inline bool operator==(StreamString &s) const {
        if (buffer.UsedSize() != s.buffer.UsedSize()){
            return false;
        }
        if (StringHelper::CompareN(buffer.Buffer(), s.buffer.Buffer(),buffer.UsedSize()) != 0){
            return false;
        }
        return true;
    }

    /**
     * @brief Compare the buffer content with the input content.
     * @param s The buffer to be compared with.
     * @return True if the two buffers are the same. False otherwise.
     */
    inline bool operator==(const char *s) const {
        if (s == NULL){
            return false;
        }
        if ((uint32)StringHelper::Length(s) != buffer.UsedSize()){
        	return false;
        }
        if (StringHelper::CompareN(buffer.Buffer(),s,buffer.UsedSize()) != 0){
            return false;
        }
        return true;
    }

    /**
     * @brief Compare the buffer content with the input content.
     * @param s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise. */
    inline bool operator!=(StreamString &s) const {
        return !((*this) == s);
    }
    /**
     * @brief Compare the buffer content with the input content.
     * @param s The StreamString to be compared with.
     * @return trye if the two buffer are different, false otherwise. */  
    inline bool operator!=(const char *s) const {
        return !((*this) == s);
    }

    /**
     * @brief Allows access to character within the buffer.
     * @param  pos The position in the buffer to be accessed.
     * @return 0 if the position is outside the buffer limits. The character at position pos otherwise.
     */
    inline char operator[](uint32 pos) {
        if (pos >= buffer.UsedSize()){
            return 0; 
        }
        return buffer.BufferReference()[pos];
    }
    
    /** 
     * @brief Checks if a char is in the string
     * @param c The character to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(char c) const ;

    /** 
     * @brief Checks if a string is contained in the string.
     * @param x The string to look for.
     * @return >0 the first position if found. -1 otherwise.
     */
    virtual int32 Locate(const StreamString &x) const ;
};

#endif

