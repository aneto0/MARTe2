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
 * @file CharBuffer.h
 * @brief Implementation of a buffer with allocation and reallocation and access functions.
 * 
 * This class implements a generic buffer which could be allocated and reallocated dinamically
 * on the heap or assigned passing a memory reference in input. It consists in a char* pointer
 * to the begin of the buffer and some attributes like the size of the buffer.
 */
#ifndef CHAR_BUFFER_H
#define CHAR_BUFFER_H

#include "GeneralDefinitions.h"

/**
 * @brief CharBuffer class.
 *
 *  A basic implementation of a buffer of character.
    A replacement for dealing directly with mallocs and reallocs.
    Access as char * both read-only and read-write.
    Supports up to 4G of ram. 
*/
class CharBuffer {
private:
	
	struct {
		/** true if memory was allocated by this class methods 
		 *  false means that it is a reference to a buffer*/
		bool 		allocated:1;		
		
		/** true if it is read-only memory */
		bool 		readOnly:1;		
				
	} bufferMode;
	
    /** the size of the allocated memory block  */
    uint32 		bufferSize;

    /**  a pointer at the beginning of the memory buffer  */
    char * 		buffer;
	   
private:

    /** 
     * @brief Clean the buffer.
     * 
     * Sets the size to zero.
     * In case of heap buffer, frees the memory too.
     */
    virtual void Clean();

public:

    /** 
     * @brief Default constructor.
     * 
     * At the beginning the buffer pointer is null. */
    CharBuffer() {
    	this->bufferSize 		= 0;
        this->buffer 			= NULL;
        bufferMode.readOnly 	= true;
        bufferMode.allocated 	= false;
    }

    /** 
     * @brief Default destructor.
     */
    virtual ~CharBuffer();
    
    /**
     * @brief Allocate or reallocate memory to the desired size on the heap.
     * @param desiredSize is the desired size of the buffer.
     * @param allocationGranularityMask impose granularity and maximum size.
     * @return false if desiredSize is greater than the maximum possible or if the allocation fails.
     *
     * AllocationGranularityMask should be composed by ones and at the end by the desired number of zeros 
     * (i.e 0xfffffff0 ==> 4 zeros ==> granularity: 2^4=16 bytes)
     * The granularity is defined as the 2 complement of AllocationGranularityMask (default 1).
     * The maximum possible size is allocationGranularityMask << 1.
     * The minimum possible size is 1, in this case it allocates the granularity, if desiredSize is zero frees the memory.
     * In case of no errors, it allocates k*granularity bytes with k integer such that k*granularity >= desiredSize.
     *
        Content is preserved by copy, if contiguus memory is not available, as long as it fits the newsize.
        AllocationGranularityMask defines how many bits to consider 
        for the buffer size, round up the others
    */
    virtual bool SetBufferAllocationSize(
    		uint32 			desiredSize,
            uint32 			allocationGranularityMask 		= 0xFFFFFFFF);
    
    /**
     * @brief Memory assignment of a preallocated buffer in read and write mode.
     * @param buffer is a pointer to the writable buffer.
     * @param bufferSize is the size of the buffer.
     *
     * If buffer is null just call clean and set allocated=false and readOnly=true,
     * If buffer is valid set the buffer pointer and the buffer size by input parameters and readOnly=false.
    */
    virtual void SetBufferReference(
    		char *			buffer, 
    		uint32 			bufferSize
    );
    
    /**
     * @brief Memory assignment of a preallocated buffer in read mode.
     * @param buffer is a pointer to the buffer without write access.
     * @param bufferSize is the size of the buffer.
     *
     * If buffer is null just call clean and set allocated=false and readOnly=true,
     * If buffer is valid set the buffer pointer and the buffer size by input parameters.
     * ReadOnly flag remains true.
    */
    virtual void SetBufferReference(
    		const char *	buffer, 
    		uint32 			bufferSize
    );
    
public:
    
    /** 
     * @brief Read Only access to the internal buffer.
     * @return The pointer to the buffer.
     */
    inline const char *Buffer() const {
        return buffer;
    }

    /**
     * @brief  Read Write access top the internal buffer
       @return The pointer to the buffer
     */
    inline char *BufferReference() const {
    	if (bufferMode.readOnly) return NULL;
        return buffer;
    }

    /**
     * @brief Get the size of the memory associated to the buffer.
     * @return the size of the memory associated to the buffer. */
    inline uint32 BufferSize() const{
        return bufferSize;
    }
    
    /**
     * @brief If the buffer is writable.
     * @return true if readOnly flag is false, false otherwise. */
    bool CanWrite() const {
    	return !bufferMode.readOnly;
    }

};

#endif

