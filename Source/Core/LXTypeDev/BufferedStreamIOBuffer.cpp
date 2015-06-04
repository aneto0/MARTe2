#include "BufferedStream.h"
#include "BufferedStreamIOBuffer.h"

/**
    sets the readBufferFillAmount to 0
    adjust the seek position
*/
bool BufferedStreamIOBuffer::Resync(TimeoutType         msecTimeout){
	// empty!
    if (MaxUsableAmount() == 0) {
    	return true; 
    }
    
    // distance to end 
    uint32 deltaToEnd = UsedAmountLeft();   
    
    // adjust seek position
    // in read mode the actual stream 
    // position is to the character after the buffer end
    if (!stream->UnBufferedSeek (stream->UnBufferedPosition()-deltaToEnd)) {
        Empty();
    	return false;
    }
                                                                          
    // mark it as empty
    Empty();
    return true;
} 

/**  
    refill readBuffer
    assumes that the read position is now at the end of buffer
*/
bool BufferedStreamIOBuffer::NoMoreDataToRead(TimeoutType         msecTimeout){
	// can we write on it?
    if (BufferReference() == NULL) {
        return false;
    }
	
    // move all pointers and indexes to empty status
    Empty();

    uint32 readSize = MaxUsableAmount();
 
    if (stream->UnBufferedRead(BufferReference(),readSize)){
    	SetUsedSize(readSize);
    	return true;
    }  

    Empty();
    return false;
    	
}

/**  
    empty writeBuffer
    only called internally when no more space available 
*/
bool BufferedStreamIOBuffer::NoMoreSpaceToWrite(
    		uint32 		    neededSize   ,
    		TimeoutType         msecTimeout  ){
    // no buffering!
    if (Buffer()== NULL) return true;
	
    // how much was written?
    uint32 writeSize = UsedSize();
    
    // write
    if (!stream->UnBufferedWrite(Buffer(),writeSize,msecTimeout,true)) {
    	return False;
    }

    Empty();
    return True;  
}

