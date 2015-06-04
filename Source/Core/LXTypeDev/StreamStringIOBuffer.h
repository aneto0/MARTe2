#if !defined (STREAMSTRING_IO_BUFFER)
#define STREAMSTRING_IO_BUFFER

#include "GeneralDefinitions.h"
#include "Memory.h"
#include "CharBuffer.h"
#include "IOBuffer.h"
/**
 * @file StreamStringIOBuffer.h
 * @brief Implementation of basic functions of the buffer used for StreamString class.
 *
 * These functions allows to allocate new space in the heap for the buffer, read and write operations.
 * This buffer is an attribute of StreamString class.
 */


/**
 * @brief StreamStringIOBuffer class.
 *
 * This class inherits from IOBuffer and implements NoMoreSpaceToWrite accordingly to
 * the string requirements, namely allocating new space when the buffer is full in case of write operations.
 *
 * It implements also a Terminate function to put the final \0 at the end of the string.
 *
 * For memory allocations it adds one to the desired size passed by argument and sets reservedSpaceAtEnd = 1 for the 
 * final \0 character.   */
class StreamStringIOBuffer:public IOBuffer {
       
public: // 

	/** @brief Default constructor */
	StreamStringIOBuffer(){	}
	
	/** @brief Default destructor. */
	virtual ~StreamStringIOBuffer();
		
	/**
         * @brief Sets the size of the buffer to be desiredSize or greater up next granularity. 
         *
         * Truncates stringSize to desiredSize-1.
         *
         * @param desiredSize is the desired size to allocate without considering the final \0.
         * @param allocationGranularityMask defines the desired granularity (see CharBuffer::SetBufferAllocationSize), default is granularity=64 bytes.
         * @return false in case of errors in the allocation.
         *
         * This function calls IOBuffer::SetBufferHeapMemory with desiredSize+1 and reservedSpaceAtEnd=1.
	*/
    virtual bool  		SetBufferAllocationSize(
                uint32 			desiredSize,
                uint32 			allocationGranularityMask 		= 0xFFFFFFC0);
	
public: // read buffer private methods
    
    /** 
     * @brief If buffer is full this is called to allocate new memory.
     * @param neededSize is the desired size to allocate.
     * @param msecTimeout is the timeout not used here. 
     * @return false in case of errors in the allocation.
     *
     * This function calls SetBufferAllocationSize passing neededSize. */    
    virtual bool 		NoMoreSpaceToWrite(
                uint32              neededSize      = 1,
                TimeoutType         msecTimeout     = TTDefault);
     
    /**
     * @brief Copies buffer the end of writeBuffer.
     * @param buffer contains data to be written.
     * @param size is the desired number of bytes to copy.
     * 
     * If size is greater than amountLeft allocates new memory calling SetBufferAllocationSize passing the size of the filled memory + size;
     * then calls IOBuffer::Write.
     */ 
    virtual void 		Write(const char *buffer, uint32 &size);    

    /** @brief Add the termination character at the end of the filled memory. */
    virtual void 		Terminate();

};



#endif
