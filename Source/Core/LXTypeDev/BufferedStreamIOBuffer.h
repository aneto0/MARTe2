#if !defined BUFFEREDSTREAM_IOBUFFER
#define BUFFEREDSTREAM_IOBUFFER

//#include "TypeConversion.h"
#include "TimeoutType.h"
#include "BufferedStream.h"
#include "IOBuffer.h"
/**
 * @file BufferedStreamIOBuffer.h
 * @brief Implementation of the IOBuffer for buffered streams.
 */ 
class BufferedStream;

/**
 * @brief BufferedStreamIOBuffer class.
 * 
 * This class inherits from IOBuffer but implements NoMoreSpaceToWrite and NoMoreDataToRead accordingly
 * to the buffered stream requirements. In particular NoMoreSpaceToWrite acts as a flush on the stream,
 * while NoMoreDataToRead refills this buffer from the stream. Moreover also the Resync function is implemented
 * due to adjust the stream position after a buffered read or write operation.
 *
 * Since this buffers has to read and write on the associated stream, in the constructor must be passed a pointer to the associated stream. */
class BufferedStreamIOBuffer:public IOBuffer{
private:
    /** the stream related to the IOBuffer. */
    BufferedStream *stream;
    
       
public: // read buffer private methods

    /**
     * @brief Default constructor.
     * @param s is a pointer to the stream which uses this buffer. */
    BufferedStreamIOBuffer(BufferedStream *s){
        stream=s;
    }

    /**  
     * @brief Refills the buffer reading from the stream.
     * @param msecTimeout is the timeout not used here.
     * @return false if the buffer is null or in case of stream read error.
     * 
     * Empties the buffer, calls BufferedStream::UnBufferedRead with size = MaxUsableAmount.
     * In case of stream read error empties the buffer and returns false.
    */
    virtual bool 		NoMoreDataToRead( TimeoutType         msecTimeout     = TTDefault);
    
    /**
     * @brief User friendly function which simply calls NoMoreDataToRead.
     * @param msecTimeout is the timeout not used here.
     * @return the NoMoreDataToRead return. */
    inline bool Refill(TimeoutType         msecTimeout     = TTDefault){
    	return NoMoreDataToRead(msecTimeout);
    }
    
    /**
     * @brief Flushes the buffer writing on the stream.
     * @param neededSize is not used here.
     * @param msecTimeout is the timeout. 
     * @return false if the buffer is null or in case of stream write error.
     * 
     * Calls BufferedStream::UnBufferedWrite with size = the size of the filled memory.*/
    virtual bool 		NoMoreSpaceToWrite(
                uint32              neededSize      = 1,
                TimeoutType         msecTimeout     = TTDefault);
    
    /**
     * @brief User friendly function which simply calls NoMoreSpaceToWrite.
     * @param msecTimeout is the timeout not used here.
     * @return the NoMoreSpaceToWrite return. */
    inline bool Flush(TimeoutType         msecTimeout     = TTDefault){
    	return NoMoreSpaceToWrite(0, msecTimeout);
    }

    /**
      * @brief Adjusts the position of the stream.
      * @param msecTimeout is the timeout.
      * @return false if the stream seek fails.
      *
      * This function is called from the stream after a read operation because the position was shifted
      * of the buffer size because of the refill. Calls BufferedStream::Seek moving the cursor back of
      * UsedAmountLeft.
    */
    virtual bool 		Resync(TimeoutType      msecTimeout     = TTDefault);    
    
    /**
     * @brief Allocate or reallocate memory to the desired size.
     * @param size is the desired size for the buffer.
     * @return false in case of allocations errors.
     *
     * Calls IOBuffer::SetBufferHeapMemory passing size.  
    */ 
    bool SetBufferSize(uint32 size){
    	return IOBuffer::SetBufferHeapMemory(size);
    }
    
};



#endif
