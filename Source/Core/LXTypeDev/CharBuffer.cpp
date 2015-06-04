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
 * Implementation of a buffer with allocation and reallocation and access functions
 */

#include "CharBuffer.h"
#include "Memory.h"

    /** 
     * deallocates memory if appropriate
     * sets all members to default  
     */
void CharBuffer::Clean() {
	if (bufferMode.allocated){
		if (buffer != NULL){
            MemoryFree((void *&) buffer);    			
		}
	}
    bufferSize = 0;
}


/** Destructor */
CharBuffer::~CharBuffer() {
	Clean();
}

    
/**
    allocate or reallocate memory to the desired size
    content is preserved by copy, if contiguus memory is not available, as long as it fits the newsize
    allocationGranularityMask defines how many bits to consider 
    for the buffer size. round up the others
*/
bool CharBuffer::SetBufferAllocationSize(
		uint32 			desiredSize,
        uint32 			allocationGranularityMask
){
	// if memory referenc is present remove it 
	// otherwise we end up reallocating others's memory!!
	if ((bufferSize > 0) && (!bufferMode.allocated)){
		Clean();
	}	
	
    // the mask is the 2 complement of the actual granularity
    uint32 allocationGranularity = ~allocationGranularityMask + 1;
//    uint32 allocationBoundary    = ~(2 * allocationGranularity - 1);


    //If you want to allocate allocationGranularityMask neededMemory= (allocationGranularityMask+ ~(allocationGranularityMask))&(allocationGranularityMask)
    //= allocationGranularityMask  then you can allocate it but no more (otherwise it's overflow).      
    uint32 allocationBoundary = allocationGranularityMask;

    // stay within matematical limits
    if (desiredSize > allocationBoundary ) {
    	return false;
    }
    
    uint32 neededMemory = 
            desiredSize 
             +  allocationGranularity // to increase up to granularity boundaries
             -  1; // so that 0 still stays below granularity 

    neededMemory &= allocationGranularityMask;
    
    if (neededMemory == 0){
        if (buffer != NULL) MemoryFree((void *&)buffer);
        bufferSize = neededMemory;
    }

    if (buffer == NULL) {
        buffer = (char *) MemoryMalloc(neededMemory);
    }
    else {
        buffer = (char *) MemoryRealloc((void *&) buffer, neededMemory);
    }
    // if the pointer is not NULL it means we have been successful
    if (buffer != NULL){
    	bufferSize 				= neededMemory;
        bufferMode.readOnly 	= false;
        bufferMode.allocated 	= true;        	
    } else {
    	bufferSize = 0;
    	return false;
    }
    return true;
}

/**
    allocate or reallocate memory to the desired size
    content is preserved by copy, if contiguus memory is not available, as long as it fits the newsize
    allocationGranularityMask defines how many bits to consider 
    for the buffer size. round up the others
*/
void CharBuffer::SetBufferReference(
		char *			buffer, 
		uint32 			bufferSize
){
	Clean();
    this->buffer = buffer;
    bufferMode.readOnly 	= true;
    bufferMode.allocated 	= false;
	if (buffer != NULL){
        this->bufferSize 	= bufferSize;
        bufferMode.readOnly = false;
	}  
}

/**
    allocate or reallocate memory to the desired size
    content is preserved by copy, if contiguus memory is not available, as long as it fits the newsize
    allocationGranularityMask defines how many bits to consider 
    for the buffer size. round up the others
*/
void CharBuffer::SetBufferReference(
		const char *	buffer, 
		uint32 			bufferSize
){
	Clean();
    this->buffer = (char *)buffer;
    bufferMode.readOnly 	= true;
    bufferMode.allocated 	= false;
	if (buffer != NULL){
        this->bufferSize 	= bufferSize;
	}  
}
