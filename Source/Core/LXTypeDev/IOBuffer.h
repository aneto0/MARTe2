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
 * @file IOBuffer.h
 * @brief Adding stream support and features to CharBuffer class.
*/ 
#ifndef IO_BUFFER_H
#define IO_BUFFER_H

#include "CharBuffer.h"
#include "Memory.h"
#include "TimeoutType.h"

/**
 * @brief IOBuffer class.
 *
 * This class inherits from CharBuffer which represents a naked memory buffer with a pointer
 * at the beginning. It adds a movable pointer across the memory area and attributes
 * to get the size of the filled area and the readable area of the buffer.
 * 
 * This class implements basic methods for read, write and seek operations on the buffer which are virtual
 * to avoid overloading in descendents.
 *
 * The most important functions are the inline PutC and GetC which are used in Printf and GetToken
 * functions and are shared by all descendents (besides the implementation of NoMoreSpaceToWrite and 
 * NoMoreDataToRead depends on the specific derived class implementation)
 */
class IOBuffer: protected CharBuffer
{
private:	
    /**
     * The size of the usable memory in the buffer.
     * Usually it is equal to the CharBuffer::BufferSize 
     * but for example in StreamString considering the final 0
     * the usable memory is CharBuffer::BufferSize - 1. 
    */
    uint32              	maxUsableAmount;
   
    /**
     * The remained size from the current position
     * to the end of the usable memory.
    */
    uint32               	amountLeft;
    
    /**
     * The remained size from the filled memory to the end
     * of the usable memory.
     */
    uint32                  fillLeft;

    /**
     * Allows to call NoMoreDataToRead or NoMoreSpaceToWrite when
     * the remained size from the cursor (amountLeft) is <= to it.
     */
    uint32 					undoLevel;
    
    /**
     * The pointer to the current position in the buffer (cursor).
    */
    char *					bufferPtr;

public:
    
    /** 
     * @brief The routine executed in PutC when amountLeft is <= undoLevel, namely the cursor arrived to a specific position.
     * @param msecTimeout is the timeout not used here. 
     * @return false in this implementation.
     *
     * This implementation is basic and only returns false.
     *
     * In StreamStringIOBuffer undoLevel is zero, so when the cursor arrived at the end of the memory,
     * this function allocated a new portion of memory in the queue.
     *
     * In BufferedStreamIOBuffer undoLevel is zero, so when the cursor arrived at the end of the memory
     * this function flushes this buffer to the stream.
     *
     * In general the implementation depends on descendents.  
     * */
    virtual bool 		NoMoreSpaceToWrite(
                uint32              neededSize      = 1,
                TimeoutType         msecTimeout     = TTDefault);

    /** 
     * @brief The routine executed in GetC when amountLeft is <= undoLevel, namely the cursor arrived to a specific position.
     * @param msecTimeout is the timeout not used here.
     * @return false in this implementation.
     *
     * This implementation is basic and only returns false.
     * 
     * In BufferedStreamIOBuffer undoLevel is zero, so when the cursor arrived at the end of the memory
     * this function refill this buffer from the stream for a new read.
     *
     * In general the implementation depends on descendents.  
     * */
    virtual bool 		NoMoreDataToRead(TimeoutType         msecTimeout     = TTDefault);
    
    /**
     * @brief Syncronizes the stream position with this buffer position.
     * @param msecTimeout is the timeout not used here.
     * @return false in this implementation.
     *
     * This implementation is basic and only returns false.
     *
     * In BufferedStreamIOBuffer flushes the write buffer after write operations or adjusts the 
     * stream position shifted after a refill because of a read operations.
     *
     * In general the implementation depends on descendents.
    */ 
    virtual bool 		Resync(TimeoutType         msecTimeout     = TTDefault);   
    
   
    /**
     * @brief Moves the cursor to an absolute position.
     * @param position is the desired position in the filled memory.
     * @return false if position is greater than the size of the filled memory.
     *
     * Adjusts amountLeft = maxUsableAmount - position and sets bufferPtr to Char::Buffer (the beginning of the buffer) + position. 
     */
    virtual bool 		Seek(uint32 position);
    
    /**
     * @brief Moves the cursor relatively from the current position.
     * @param delta is the gap from the current position.
     * @return false if the final position falls out of ranges.
     *
     * Checks that the final position is >= 0 and <= UsedSize, then moves the cursor.
     */
    virtual bool 		RelativeSeek(int32 delta);
    
    /**
     * @brief Sets manually the size of the filled memory.
     * @param size is the desired used size.
     * @return true.
     *
     * If the desired size is greater than the maxUsableAmount it clips the desired size.
     */
    virtual bool 		SetUsedSize(uint32 size);
    
public:
    
    /** 
     * @brief Default constructor.
     */
    IOBuffer(){
    	amountLeft 		= 0;
    	maxUsableAmount = 0;
    	bufferPtr 		= NULL;
    	fillLeft 		= 0;
    	undoLevel 		= 0;
    }
    
    /**
     * @brief Default constructor. */
    virtual ~IOBuffer();
    

public:    
    
    /**
     * @brief Allocates dinamically a memory portion on the heap.
     * @param desiredSize is the desired size to be allocated.
     * @param reservedSpaceAtEnd is the space allocated but not avaiable (maxUsableAmount does not consider it).
     * @return false if the allocation fails.
     *
     * If the new avaiable size (desiredSize-reservedSpaceAtEnd) is minor than the current used size, used size
     * is clipped and if the position was over it becomes the end of the new avaiable size.
     * This function calls CharBuffer::SetBufferAllocationSize, then fixes maxUsableAmount to desiredSize-reservedSpaceAtEnd.
    */
    virtual bool SetBufferHeapMemory(
            uint32 			desiredSize,
            uint32 			allocationGranularityMask 		= 0xFFFFFFFF,
            uint32          reservedSpaceAtEnd              = 0
    );    
    
    /**
     * @brief Assigns a preallocated memory with read and write access.
     * @param buffer is a pointer to a preallocated memory.
     * @param bufferSize is the size of the memory.
     * @param reservedSpaceAtEnd is the memory allocated but not avaiable.
     * @return true.
     *
     * Calls CharBuffer::SetBufferReference, then sets the buffer as empty and maxUsableAmount = bufferSize - reservedSpaceAtEnd.
    */
    virtual bool SetBufferReferencedMemory(
    	    char *			buffer, 
    	    uint32 			bufferSize,
            uint32          reservedSpaceAtEnd              = 0
    );
    
    /**
     * @brief Assigns a preallocated memory only with read access.
     * @param buffer is a pointer to a preallocated memory.
     * @param bufferSize is the size of the memory.
     * @param reservedSpaceAtEnd is the memory allocated but not avaiable.
     * @return true.
     *
     * Calls CharBuffer::SetBufferReference, then sets the buffer as empty and maxUsableAmount = bufferSize - reservedSpaceAtEnd.
     * Thanks to the overload, with const char* the flag readOnly is setted true then also CharBuffer::CanWrite returns true.
    */
    virtual bool SetBufferReadOnlyReferencedMemory(
    		const char *	buffer, 
    		uint32 			bufferSize,
            uint32          reservedSpaceAtEnd              = 0
    );
    
public:    
    
    /**
     * @brief The size of the allocated memory.
     * @return the size of the allocated memory. */
    uint32 				BufferSize() 		const{
    	return CharBuffer::BufferSize();
    }
    
     /**
      * @brief Get the avaiable space size from the beginning to the end of memory (without reserved space).
      * @return the avaiable space in the buffer.      
     */
    inline uint32 		MaxUsableAmount() 	const {
    	return maxUsableAmount;
    }
   
     /**
      * @brief Get the avaiable space size from the cursor to the end of memory (without reserved space).
      * @return the avaiable space from the current position.      
     */  
    inline uint32 		AmountLeft() 		const {
    	return amountLeft;
    }  

    /**
     * @brief Get the space size from the cursor to the end of filled area.
     * @return the space size from the current position to the end of the filled memory in the buffer.
    */
    inline uint32 		UsedAmountLeft() 	const {
    	return amountLeft - fillLeft;
    }  
    
    /**
     * @brief Get the cursor position.
     * @return the current position, namely the size from the beginning to the cursor. */
    inline uint32 		Position() 			const {
    	return maxUsableAmount - amountLeft;
    }

    /**
     * @brief Get the size of the filled area.
     * @return the size of the filled area.*/
    inline uint32 		UsedSize() 			const{
    	return maxUsableAmount - fillLeft;
    }

    /**
     * @brief Get the pointer at the beginning of the buffer only in read access.
     * @return the pointer at the beginning of the buffer. */
    inline const char *Buffer() 			const {
    	return CharBuffer::Buffer();    	
    }

  
    /**
     * @brief Get the pointer at the beginning of the buffer with write access.
     * @return the pointer at the beginning of the buffer. */
    inline char *BufferReference() 			const {
    	return CharBuffer::BufferReference();    	
    }

public:    
    
    /** 
     * @brief Put a character on the buffer.
     * @param c is the character to copy on this buffer.
     * @return false if there is no space to write or the buffer is not writable.
     * 
     * This inline function is called by Printf functions for streamables. The 
     * implementation of NoMoreSpaceToWrite depends on descendents and it could be
     * for example a flush (BufferedStreamIOBuffer) or a new allocation (StreamStringIOBuffer).
     *
     * The value of undoLevel could be used to trigger the call to NoMoreSpaceToWrite when
     * the cursor is at a specific position.
     */ 
    inline bool         PutC(char c) {

        // check if buffer needs updating and or saving            
    	if (amountLeft <= undoLevel) {
    		NoMoreSpaceToWrite();
    		
            // check if we can continue or must abort
        	if (amountLeft <= 0) {
        		return false;
        	}
        }
        
        // check later so that to give a chance to allocate memory
        // if that is the policy of this buffering scheme
        if (!CanWrite()) {
        	return false;
        }
       
        *bufferPtr = c;
        
        bufferPtr++;
        amountLeft--;
        if (fillLeft > amountLeft){
        	fillLeft = amountLeft;
        }

        return True;    	
    }	

    /**
     * @brief If possible remove the last character from the buffer.
     * @return false if the position is at the beginning.
     * 
     * Increments amountLeft and decrements the cursor.
     */
    inline bool         UnPutC() {
    	// can I still do it?
    	if (Position() <= 0 ){
    		return false;
    	}
    	
    	if (amountLeft == fillLeft){
    		fillLeft++;
    	}
        bufferPtr--;
        amountLeft++;

        return True;    	
    }
    
    /** 
     * @brief Writes from an input buffer.
     * @param buffer contains the data to be written write in this buffer.
     * @param size is the number of bytes to be copied.
     * 
     * The function does nothing if CharBuffer::CanWrite returns false,
     * otherwise copy size bytes from the input buffer, sets the cursor size
     * positions forward and adjusts fillLeft and amountLeft accordingly.
     * If size is greater than amountLeft is clipped.
    */
    virtual void 		Write(const char *buffer, uint32 &size);


    /**
     * @brief Writes all the size in the argument from an input buffer.
     * @param buffer contains data to be written in this buffer.
     * @param size is the number of byte to be copied.
     *
     * This function calls Write and NoMoreSpaceToWrite until size is consumed. */
    inline bool 		WriteAll(const char *buffer, uint32 &size){
	//size to be copied.
    	uint32 leftSize = size;
    	while (leftSize > 0){
		//if the cursor is at the end call NoMoreSpaceToWrite
                // flushes the buffer or allocates new memory.
		if(amountLeft == 0){
			NoMoreSpaceToWrite(leftSize);
                        //Something wrong, no more avaiable space, return false.
			if(amountLeft == 0){
				return false;
			}
		}
		
    		uint32 toDo = leftSize;
    		Write(buffer,toDo);
    		buffer   += toDo;
		//if all the size is copied leftSize becomes 0 and return true
    		leftSize -= toDo;
    	}
		return true;
    }
    
    /** 
     * @brief Get a character from the buffer.
     * @param c is the character in return.
     * @return false if the cursor is at the end of the filled memory.
     *
     * This function calls NoMoreDataToRead which can for example
     * refill this buffer from the stream (BufferedStreamIOBuffer)
     * when the cursor is in a specific position defined by undoLevel. 
    */ 
    inline bool         GetC(char &c) {
    	
        // check if buffer needs updating and or saving            
    	if (UsedAmountLeft() <= undoLevel) {
    		NoMoreDataToRead();
    		
        	if (UsedAmountLeft() <= 0) {
        		return false;
        	}
        }
    	
        c = *bufferPtr;
        
        bufferPtr++;
        amountLeft--;

        return True;    	
    }	

    /** 
     * @brief If possible decrements the cursor.
     * @return false if the cursor is at the beginning.
     *
     * It decrements the cursor and increments amountLeft. 
    */ 
    inline bool         UnGetC() {
    	// can I still do it?
    	if (Position() <= 0 ){
    		return false;
    	}
    	
        bufferPtr--;
        amountLeft++;

        return True;    	
    }
    
    /** 
     * @brief Reads from this buffer to an output buffer.
     * @param buffer is the output buffer where data must be written.
     * @param size is the number of bytes to be read.
     * 
     * If size is greater than the size from the cursor to the end of the filled memory,
     * it is clipped.
    */
    inline void 		Read(char *buffer, uint32 &size){
	    
    	uint32 maxSize = UsedAmountLeft();
	    // clip to available space
	    if (size > maxSize) {
	    	size = maxSize;
	    }
		    
		// fill the buffer with the remainder 
		if (size > 0) {
			MemoryCopy(buffer, bufferPtr,size);

			amountLeft -= size;
			bufferPtr += size;
		}
    }
    
    /**
     * @brief Empties the buffer.
     *
     * Sets amountLeft and fill to maxUsableAmount, sets the buffer pointer at the beginning.
    */
    inline void 		Empty(){
		amountLeft = maxUsableAmount;   // Seek 0 
		fillLeft   = maxUsableAmount;   // SetSize 0
        bufferPtr  = ( char *)Buffer(); // seek 0
    }
    
};
#endif 
