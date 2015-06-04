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
 * @file StreamMemoryReference.h
 * @brief Implementation of a stream on a preallocated buffer passed in the constructor. 
 *
 * The buffer associated to this class is a normal IOBuffer built by a memory reference,
 * then it is a stream associated to a preallocated memory with a maximum fixed size.
 */
#ifndef STREAM_MEMORY_REFERENCE_H
#define STREAM_MEMORY_REFERENCE_H

#include "GeneralDefinitions.h"
#include "MemoryReferenceIOBuffer.h"
#include "Streamable.h"
#include "TimeoutType.h"

/**
    @brief A basic implementation of a stream which allows reading and writing to a memory location
*/
class StreamMemoryReference: public Streamable {

private:    

	/**
         * Simply a normal IOBuffer. Nothing is overloaded.
         * It is allocated by IOBuffer::SetBufferReferencedMemory or IOBuffer::SetBufferReadOnlyReferencedMemory
         * @see IOBuffer.cpp for the naked implementation of this buffer.  */
	MemoryReferenceIOBuffer 	buffer;	
   
protected: // methods to be implemented by deriving classes

    /**
     * @brief Returns the read buffer.
     * @return a pointer to the MemoryReferenceIOBuffer buffer. */
    virtual IOBuffer *GetInputBuffer();

    /** 
     * @brief Returns the write buffer.
     * @return a pointer to the MemoryReferenceIOBuffer buffer. */
    virtual IOBuffer *GetOutputBuffer();

public: // usable constructors

    /**
     * @brief Binds this object to a memory area in read and write mode.
     * @param buffer is the char pointer of the preallocated memory.
     * @param bufferSize is the usable size of the buffer.
     *
     * The function assumes that the area of memory is empty and therefore the Stream::Size is 0
     * */
    StreamMemoryReference(char *buffer,uint32 bufferSize){
    	this->buffer.SetBufferReferencedMemory(buffer,bufferSize);
    }

    /** 
     * @brief Binds this object to a memory area in read only mode.
     * @param buffer is the const char pointer to a preallocated read only memory.
     * @param bufferSize is the usable size of the buffer.
     *
     * The function assumes that the area of memory is full and therefore the Stream::Size is bufferSize
     * */
    StreamMemoryReference(const char *buffer,uint32 bufferSize){
    	this->buffer.SetBufferReadOnlyReferencedMemory(buffer,bufferSize);
    	this->buffer.SetUsedSize(bufferSize);
    }
    
    /** Destructor */
    virtual ~StreamMemoryReference() ;
    
public:
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
        actual read size is returned in size. (unless complete = True)
        msecTimeout is how much the operation should last - no more - if not any (all) data read then return false  
        timeout behaviour depends on class characteristics and sync mode.
    */
    virtual bool        Read(
                            char*               buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false);

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
    virtual bool        Write(
                            const char*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false);
    
    /**
     * @brief Write operations allowed.
     * @return true if the preallocated memory is valid (pointer != NULL). */
    virtual bool        CanWrite() const ;

    /** 
     * @brief Read operations allowed.
     * @return true if the preallocated memory is valid (pointer != NULL). */
    virtual bool        CanRead() const ;
    
    /** 
     * @brief The size of the filled memory.
     * @return the size of the stream.
     *
     * For this object the size is always minor than the buffer dimension passed in the constructor. */
    virtual int64       Size();

    /** 
     * @brief Moves within the file to an absolute location.
     * @param pos is the desired absolute position.
     * @return false in case of position out of bounds.
     *
     * If the desired position falls out of the range, the position becomes the end of the filled stream. */
    virtual bool        Seek(int64 pos);
    
    /** 
     * @brief Moves within the stream relative to current location.
     * @param deltaPos is the gap from the current position.
     * @return false if the position falls out of bounds.
     *
     * If the final position falls out of the stream bounds, it becomes one of the bounds. */
    virtual bool        RelativeSeek(int32 deltaPos);
    
    /**
     * @brief  Returns current position.
     * @return the current position. */
    virtual int64       Position() ;

    /**
     * @brief Set the used size.
     * @param size is the desired stream size.
     * @return true.
     *
     * Setting the size manually you can read until that position. */
    virtual bool        SetSize(int64 size);

    /**
     * @brief Seek operations are allowed
     * @return true. */
    virtual bool        CanSeek() const ;

public: // DIRECT ACCESS FUNCTIONS
      

    /**
     * @brief Read Only access to the internal buffer. It calls a CharBuffer function.
     * @return The pointer to the internal buffer.
     */
    inline const char *Buffer() const {
        return buffer.Buffer();
    }

    /** 
     * @brief Read Write access top the internal buffer. It calls a CharBuffer function.
     * @return The pointer to the internal buffer.
     */
    inline char *BufferReference() const {
        return buffer.BufferReference();
    }


    /** 
     * @brief Returns a pointer to the tail of the buffer.
     * @param  ix the offset from the end of buffer. valid ranges is 0 to Size()-1
     * @return pointer to the tail of the buffer
     */
    inline const char *Tail(int32 ix) const {
    	if (ix > 0) 				return 0;
    	if ((ix - buffer.UsedSize() -1)< 0) 	return 0;
    	return buffer.BufferReference() + buffer.UsedSize() - ix - 1;
    }


};

#endif

