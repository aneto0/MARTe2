#include "StreamStringIOBuffer.h"

StreamStringIOBuffer::~StreamStringIOBuffer(){

}

/** 
 *  desiredSize is the desired buffer size
 *  strings will fit upto desiredSize-1 
 *  sets the size of the buffer to be desiredSize or greater up next granularity
 */
bool  StreamStringIOBuffer::SetBufferAllocationSize(
		uint32 			desiredSize,
		uint32 			allocationGranularityMask){
	
    //add one to desired size for the terminator character.
    bool ret = SetBufferHeapMemory(desiredSize+1,allocationGranularityMask,1);
    Terminate();
    return ret;
    
}

/** copies buffer of size size at the end of writeBuffer
 * before calling check that bufferPtr is not NULL
 */ 
void StreamStringIOBuffer::Write(const char *buffer, uint32 &size){
	
	if (size > AmountLeft()) {
		SetBufferAllocationSize(Position() + size);
	}
	
	IOBuffer::Write(buffer,size);

}

// read buffer private methods
/// if buffer is full this is called 
bool StreamStringIOBuffer::NoMoreSpaceToWrite(
    		uint32 		    neededSize   ,
    		TimeoutType         msecTimeout  ){

	// reallocate buffer
	// uses safe version of the function
	// implemented in this class
	if (!SetBufferAllocationSize(BufferSize()+neededSize)) {
		return false;
	}

	return true;
}

/** @brief Add the termination character. */
void StreamStringIOBuffer::Terminate(){
	if (BufferReference() != NULL)
		BufferReference()[UsedSize()]= 0;
}


