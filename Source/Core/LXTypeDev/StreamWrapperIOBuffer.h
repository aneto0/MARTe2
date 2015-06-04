#if !defined STREAMWRAPPER_IOBUFFER
#define STREAMWRAPPER_IOBUFFER

//#include "TypeConversion.h"
#include "TimeoutType.h"
#include "StreamInterface.h"
#include "IOBuffer.h"
/** 
 * @file StreamWrapperIOBuffer.h
 * @brief A generic buffer associated to a Streamable which could be allocated dinamically or by reference. */


/**
 * @brief StreamWrapperIOBuffer class.
 *
 * This type of buffer is used in Printf and GetToken functions when the streams does not provide 
 * their own buffers. It could be used as a temporary buffer for unbuffered streams. */
class StreamWrapperIOBuffer:public IOBuffer{
private:
	/** A pointer to the associated stream. */
	StreamInterface *stream;
       
public: // read buffer private methods

    /** 
     * @brief Constructor for dinamic allocated buffer.
     * @param s is the stream which uses this buffer.
     * @param size is the size to allocate for this buffer. 
     *
     * Calls IOBuffer::SetBufferHeapMemory.*/
    StreamWrapperIOBuffer(StreamInterface *s,uint32 size){
        stream=s;
        SetBufferHeapMemory(size);
    }

    /**
     * @brief Constructor for memory referenced buffer. 
     * @param s is the stream which uses this buffer.
     * @param buffer is the pointer to the preallocated memory.
     * @param size is the size of the buffer.
     */
    StreamWrapperIOBuffer(StreamInterface *s,char *buffer,uint32 size){
        stream=s;
        SetBufferReferencedMemory(buffer,size);
    }
    
    /**
     * @brief Refill the buffer reading from the stream.
     * @param msecTimeout is the timeout not used here.
     * @return false if the buffer is null or if the stream read fails.
     *
     * Calls the stream read function refilling the buffer.  
    */
    virtual bool 		NoMoreDataToRead( TimeoutType         msecTimeout     = TTDefault);
    
    /**
     * @brief Flushes the buffer on the stream.
     * @param neededSize is not used here.
     * @param msecTimeout is not used here.
     * 
     * Calls the stream write function writes all the used size. */
    virtual bool 		NoMoreSpaceToWrite(
                uint32              neededSize      = 1,
                TimeoutType         msecTimeout     = TTDefault);

    /**
     * @brief Adjusts the cursor position of the stream.
     * @param msecTimeout is the timeout not used here.
     * @return false if the stream seek fails.
     *
     * Calls the stream seek function moving the cursor back of UsedAmountLeft.
    */
    virtual bool 		Resync(TimeoutType      msecTimeout     = TTDefault);    
    
};



#endif 
