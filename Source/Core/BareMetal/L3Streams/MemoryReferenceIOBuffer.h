#if !defined (MEMORYREFERENCE_IO_BUFFER)
#define MEMORYREFERENCE_IO_BUFFER

#include "GeneralDefinitions.h"
#include "HeapManager.h"
#include "IOBuffer.h"
/**
 * @file MemoryReferenceIOBuffer.h
 * 
 * A buffer for StreamMemoryReference objects. */

namespace MARTe{


/**
 * @brief MemoryReferenceIOBuffer class.
 *
 * It is a simple IOBuffer. The allocation by memory reference is completely
 * performed by CharBuffer and IOBuffer classes. */
class MemoryReferenceIOBuffer:public IOBuffer {
       
public: // 

	/** @brief Default constructor. */
	MemoryReferenceIOBuffer()
	{
	}
	
	/** @brief Default destructor. */
	virtual ~MemoryReferenceIOBuffer();		
   
   
};

}

#endif
