#if !defined BUFFERED_STREAM
#define BUFFERED_STREAM

//#include "TypeConversion.h"
#include "TimeoutType.h"
#include "StreamInterface.h"
#include "IOBuffer.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "Streamable.h"
#include "BufferedStreamIOBuffer.h"
/**
 * @file BufferedStream.h
 * @brief Implementation of streamable type functions.
 * 
 * Implementation of streamable type functions for read, write and seek operations in buffered and unbuffered modes for streamable types
 * i.e files. Most of the functions depends from the virtual UnBuffered functions which must be implemented in the derived classes
 * because could be differents for different streamable types. 
 *
 * BufferedStream uses two IOBuffer in buffered modes, one for reading and one for writing operations and they are used togheter if the stream is defined as
 * readable, writable and seekable.*/ 


/**
  * @brief BufferedStream class.

    Replaces CStream and Streamable of BL1
    Inherits from pure virtual StreamInterface and does not resolve all pure virtual functions
   
    Uses the following StreamInterface methods to operate:
        CanRead  Read
        CanWrite Write
        CanSeek  Seek 
    But only allows using GetC and PutC as it masks the whole StreamInterface
    To use the whole StreamInterface one needs to implement safe versions of 
    all functions. This is left for for further class derivations. 

    Note also that this class supports partially or fully disabled buffering.
    Just set the buffer size to 0 and the buffer to NULL.

    It is a buffering mechanism for character streams
    It operates in 6 modes (+variants) depending on the Stream capabilities 
       CanSeek CanRead CanWrite
    and user Buffering choices

    1) Separate Buffering Output Mode
        CanWrite !CanSeek
        Used for devices where in and out streams are separate (for instance console )
        writeBuffer.Buffer()!=NULL && operatingMode.MutexBuffering() = False   
        !Can be combined with Separate Buffering Input Mode
    1a) writeBuffer.Buffer()== NULL just call directly StreamInterface::Write  

    2) Separate buffering Input Mode
        CanRead  !CanSeek
        Used for devices where in and out streams are separate (for instance console )
        readBuffer.Buffer()!=NULL  &&  operatingMode.mutexBuffering = False   
        !Can be combined with Separate Buffering Output Mode
    2a) readBuffer.Buffer()== NULL just call directly StreamInterface::Read  

    3) Dual separate buffers Input and Output Mode
        CanRead CanWrite !CanSeek
        Mode 1/1a and 2/2a combined

    4) Joint buffering Mode
        CanRead CanWrite CanSeek
        readBuffer.Buffer()!=NULL  && writeBuffer.Buffer()!=NULL && 
        operatingMode.mutexBuffering() = True   
        operatingMode.mutexWriteMode and
        operatingMode.mutexReadMode  determines whether the read 
        or write buffering is active
        Get and Put toggle the two flags
        everytime the flags are changed a proper Flush operation is 
        triggered to clean the buffers
    4a-b) one of readBuffer or writeBuffer is NULL
        same toggling of flags and flushing

    5) Joint buffering Read Only Mode
        CanRead !CanWrite CanSeek
        readBuffer.Buffer()!=NULL  && writeBuffer.Buffer()==NULL && 
        operatingMode.mutexBuffering() = false   
        Operates identically to mode 2 but cannot be active together
        with mode 6 
    5a) same as 2a

    6) Joint buffering Write Only Mode
        !CanRead CanWrite CanSeek
        readBuffer.Buffer()==NULL  && writeBuffer.Buffer()!=NULL && 
        operatingMode.mutexBuffering() = false   
        Operates identically to mode 1 but cannot be active together
        with mode 5
    6a) same as 1a 

*/
class BufferedStream: public Streamable {
protected:    
    /**
       Defines the operation mode and status of a basic stream
       one only can be set of the first 4.
    */
    struct OperatingModes{

    	/** cache of canSeek() used in all the buffering functions 
    	for accelleration sake
    	*/
    	bool canSeek:1;

        /** writeBuffer is the active one.
        */
        bool mutexReadMode:1;

        /** writeBuffer is the active one.
        */
        bool mutexWriteMode:1;
    	

    };
    /** set automatically on initialisation by calling of the Canxxx functions */
    OperatingModes           operatingModes;
    
private: // read and write buffers
friend class BufferedStreamIOBuffer; 
//friend class StreamableWriteBuffer; 
       
	/** 
         * The read buffer. It is used just like
         * a middle buffer between the stream and the output.
         * For each read operation this buffer is filled completely
         * and then the desired size is copied on the output.
         * Using the buffer mode, the GetC function always use this buffer,
         * while for Read function it is used only if the size to read is minor
         * than a quarter than the buffer size.
         * Function BufferedStreamIOBuffer::NoMoreSpaceToWrite acts
         * as a flush and the more confidencial function Flush calls it.
         * @see BufferedStreamBuffer for more informations.*/
	BufferedStreamIOBuffer		readBuffer;
    

        /**
         * The write buffer. It is used just like an
         * intermediate between the input and the stream. Write 
         * operations copies data from the input to this buffer 
         * and only when the buffer is full (or in case of an explicit
         * FlushAndResync call) the buffer is flushed on the stream.
         * Using the buffer mode, the PutC function always use this buffer,
         * while for Write function it is used only if the buffer is 4 times greater
         * than the size to write.
	 * Function BufferedStreamIOBuffer::NoMoreSpaceToRead acts
	 * as a refill and the more confidential function Refill calls it.
        */
	BufferedStreamIOBuffer   	writeBuffer;
    
protected: // methods to be implemented by deriving classes

    /**
     * @brief Get the read buffer.
     * @return BufferedStreamIOBuffer readBuffer pointer.
     *
     * This function is used by Printf and GetToken functions.
    */
    virtual IOBuffer *			GetInputBuffer();
    
    /** 
     * @brief Get the write buffer.
     * @return BufferedStreamIOBuffer writeBuffer pointer. 
     *
     * This function is used by Printf and GetToken functions.
     */ 
    virtual IOBuffer *			GetOutputBuffer();
    
protected: // methods to be implemented by deriving classes
    
    /** 
     * @brief Reads data into an output buffer directly.
     * @param buffer is the buffer where stream datas must be written
     * @param size is the number of bytes to read.
     * @param msecTimeout is the timeout unused here.
     * @param complete is a flag unused here.
     * 
     *  As much as size byte are read, 
     *  actual read size is returned in size. 
     *  msecTimeout is how much the operation should last - no more.
     *  Timeout behaviour depends on class characteristics and sync mode. 
     *  I.E. sockets with blocking activated wait forever when noWait is used .... 
    */
    virtual bool        UnBufferedRead(
                            char*               buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false)=0;

  
    /** 
     * @brief Write data from an input buffer to the stream directly.
     * @param buffer is the buffer which contains datas to write on the stream.
     * @param size is the number of bytes to write.
     * @param msecTimeout is the timeout.
     * @param complete is a flag unused here.
     *
     *  This function is not implemented here but depends from the derived classes.
     *  As much as size byte are written, 
     *  actual written size is returned in size. 
     *  msecTimeout is how much the operation should last.
     *  Timeout behaviour depends on class characteristics and sync mode. 
     *  I.E. sockets with blocking activated wait forever when noWait is used .... 
    */
    virtual bool        UnBufferedWrite(
                            const char*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false)=0;

    // RANDOM ACCESS INTERFACE

    /** @brief Pure virtual function. The size of the stream.
      * 
      * Not implemented here. */
    virtual int64       UnBufferedSize()const =0;

    /** @brief Pure virtual function. Moves within the file to an absolute location.
     *
     * Not implemented here. */
    virtual bool        UnBufferedSeek(int64 pos)=0;

    /** @brief Pure virtual function. Returns current position.
     *
     * Not implemented here. */
    virtual int64       UnBufferedPosition()const =0;

    /** @brief Pure virtual function. Clip the stream size to a specified point.
      * 
      * Not implemented here. */
    virtual bool        UnBufferedSetSize(int64 size)=0;

    // Extended Attributes or Multiple Streams INTERFACE

    /** @brief Pure virtual function. Select the stream to read from. Switching may reset the stream to the start.
      * 
      * Not implemented here. */
    virtual bool        UnBufferedSwitch(uint32 n)=0;

    /** @brief Pure virtual function. Select the stream to read from. Switching may reset the stream to the start.
      * 
      * Not implemented here. */
    virtual bool        UnBufferedSwitch(const char *name)=0;
    
    /** @brief Pure virtual function. Remove a stream.
      * 
      * Not implemented here. */
    virtual bool        UnBufferedRemoveStream(const char *name)=0;
 
    
private: // mode switch methods
    
    /** 
     * @brief Switch to the write mode.
     * @return false if the resyncronization goes wrong.
     *
     *  Sets the readBufferFillAmount to 0.
     *  Syncronize the position in the stream.
     *  Sets the mutexWriteMode.
     *  Does not check for mutexBuffering to be active
    */
    inline bool SwitchToWriteMode(){
        if (!readBuffer.Resync()) return false;
        operatingModes.mutexWriteMode = true;
        operatingModes.mutexReadMode = false;
        return true;
    }
    
    /** 
     * @brief Switch to the read mode.
     * @return false if the flush function fails.
     *
     *  Flushes writeBuffer.
     *  Resets mutexWriteMode.
     *  Does not refill the buffer nor check the mutexBuffering is active.
    */
    inline bool SwitchToReadMode(){
        if (!writeBuffer.Flush()) return false;
        operatingModes.mutexWriteMode = false;
        operatingModes.mutexReadMode = true; 
        return true;
    }
   
protected:
    /**
     * @brief Default constructor.
     * 
     * At the beginning the stream is monodirectional. */
    BufferedStream() : readBuffer(this),writeBuffer(this)
    {
    	operatingModes.canSeek      	= false;
    	operatingModes.mutexReadMode 	= false;
    	operatingModes.mutexWriteMode 	= false;
    }

    /** @brief Default destructor. */
    virtual ~BufferedStream();
    
    /**
     * @brief Sets the buffers size, impose the buffered modality.
     * @param readBufferSize is the desired size for the read buffer.
     * @param readBufferSize is the desired size for the write buffer.
     * @return true if the memory is allocated correctly.
     * 
     * If readBufferSize and writeBufferSize are minor than 8, they becomes equalt to 8.
     * If CanRead return false readBufferSize becomes 0.
     * If CanWrite return false writeBufferSize becomes 0.
     * If both returns true and also CanSeek return true, the mutex mode is activated.
     * The function calls FlushAndResync and then allocates the desired sizes for
     * readBuffer and writeBuffer using IOBuffer::SetBufferHeapMemory.
    */
    virtual bool SetBufferSize(uint32 readBufferSize=0, uint32 writeBufferSize=0);

public:  // special inline methods for buffering
    
    /** 
     * @brief Resyncronization and flush of the buffers.
     * @param msecTimeout is the timeout.
     * @return true if the Resync and Flush operations for buffers goes fine.
     *
     * On dual separate buffering (CanSeek=False) just Flush output.
     * On joint buffering (CanSeek= True) depending on read/write mode 
     * either Resync or Flush. 
     * The function BufferedStream::Resync adjusts the position on the stream
     * after a read operation, shifted because of the Refill.
     */
    inline bool FlushAndResync(TimeoutType         msecTimeout     = TTDefault){
    	// if there is something in the buffer, and canSeek it means we can and need to resync
    	// if the buffers are separated (!canseek) than resync cannot be done
    	if (readBuffer.UsedSize() && operatingModes.canSeek){
    		return readBuffer.Resync();
    	} 
        // some data in writeBuffer
    	// we can flush in all cases then
    	if (writeBuffer.UsedSize() ){
       		return writeBuffer.Flush();    		
    	}
    	return true;
    }

    /**
     * @brief Simply write a character to the stream if space exist and if operatingModes allows.
     * @param c is the character to be written on the stream.
     * @return false in case of errors.
     *
     * In buffered mode uses the inline IOBuffer::PutC of writeBuffer
     * but with the specific implementations of BufferedStreamIOBuffer.
     */  
    inline bool         PutC(char c)
    {   	
    	if (operatingModes.mutexReadMode) {
    	     if (!SwitchToWriteMode()) return false;
    	}
    	
        if (writeBuffer.BufferSize() > 0){
        	return writeBuffer.PutC(c);
        }
        
        uint32 size = 1;
        return UnBufferedWrite(&c,size);
    }    

    /**
     * @brief Simply read a character from stream.
     * @param c is the character by reference in return.
     * @return false in case of errors.
     *
     * In buffered mode uses the inline IOBuffer::GetC of readBuffer
     * but with the specific implementations of BufferedStreamIOBuffer.
     */
    inline bool         GetC(char &c) {

    	if (operatingModes.mutexWriteMode) {
    	     if (!SwitchToReadMode()) return false;
    	}

        if (readBuffer.BufferSize() > 0){
        	return readBuffer.GetC(c);
        }
        
        uint32 size = 1;
        return UnBufferedRead(&c,size);
    }    

public:
    // PURE STREAMING  built upon UnBuffered version 

    /** @brief Reads data from stream into buffer.
      * @param buffer is the output memory where datas must be written.
      * @param size is the number of bytes to read from stream.
      * @param msecTimeout is the timeout.
      * @param completeRead is a flag which specified is the read operation is completed.
      * @return true if successful, false otherwise.
      *
      * In unbuffered mode calls UnBufferedRead function.
      * In buffered mode reads from the readBuffer to the outpur buffer. If not all the desired
      * size is copied, the readBuffer is refilled again if the remained size is minor than
      * a quarter of the readBuffer size, otherwise calls UnBufferedRead which should copy
      * directly datas from the stream to the output buffer.
      *
      * As much as size byte are written, actual size
      * is returned in size. msecTimeout is how much the operation should last.
      * Timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
      * when noWait is used .... */
    virtual bool         Read(
                            char *              buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                completeRead    = false);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 
    

    /**
     * @brief Write data from a buffer to the stream.
     * @param buffer contains the datas to write on the stream.
     * @param size is the desired number of bytes to write.
     * @param msecTimeout is the timeout.
     * @param completeWrite is a flac which specified is the write operations is completed.
     * @return true if successful, false otherwise. 
     *
     * In unbuffered mode calls UnBufferedWrite function.
     * In buffered mode writes from the input buffer to writeBuffer if the size to write is
     * minor than a quarter of the writeBuffer size. If not all the size can be written,
     * flushes the buffer on the stream and write the remained size on writeBuffer.
     * Again, If the size to copy is greater than a quarter of the writeBuffer size,
     * it flushes the writeBuffer and then calls UnBufferedWrite
     * which should copy data from input buffer to the stream directly. 
     *
     * As much as size byte are written, actual size
     * is returned in size. msecTimeout is how much the operation should last.
     * Timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
     *  when noWait is used .... */
    virtual bool         Write(
                            const char*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                completeWrite   = false);

    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 


    // RANDOM ACCESS INTERFACE

    /** @brief The size of the stream.
      * @return the size of the filled memory.
      * 
      * Calls FlushAndResync and then UnBufferedSize. */
    virtual int64       Size() ;

    /** @brief Moves within the file to an absolute location.
      * @param pos is the absolute desired position in the stream.
      * @return false if CanSeek returns false or in case of errors.
      *
      * If writeBuffer is not empty, flushes it and then calls UnBufferedSeek.
      * If readBuffer is not empty, if the position falls in the readBuffer range calls readBuffer.Seek
      * otherwise empties the readBuffer and calls UnBufferedSeek.  
      */
    virtual bool        Seek(int64 pos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 

    /** @brief Moves within the file relative to current location.
      * @param deltaPos is the desired relative position from the current.
      * @return false if canSeek returns false or in case of errors.
      *
      * If writeBuffer is not empty, flushes it and calls UnBufferedSeek with the current position + deltaPos
      * If readBuffer is not empty, if the final position falls in the range calls readBuffer.RelativeSeek
      * otherwise calls UnBufferedSeek passing currentStreamPos-readBuffer.Size+readBuffer.Position+deltaPos  */
    virtual bool        RelativeSeek(int32 deltaPos);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 
    
    /** @brief Returns current position.
      * @return the current position in the stream.
      *
      * Flushed the writeBuffer if it is not empty, then return currentPos-readBuffer.Size+readBuffer.Position */
    virtual int64       Position() ;
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 

    /** @brief Clip the stream size to a specified point.
      * @param size is the new desired size for the stream.
      *
      * Depends on UnBufferedSetSize. */
    virtual bool        SetSize(int64 size);
    // NOTE: Implemented in .cpp but no need to have c- mangling functions as function will be normally acceessed via VT 

    // MULTISTREAM INTERFACE

    /** @brief Select the stream to read from. Switching may reset the stream to the start.ù
      * @param n is the number of the desired stream.
      * 
      * Depends on UnBufferedSwitch. */
    virtual bool        Switch(uint32 n);

    /** @brief Select the stream to read from. Switching may reset the stream to the start.
      * @param name is the name of the desired stream.
      * 
      * Depends on UnBufferedSwitch. */
    virtual bool        Switch(const char *name);

    /** @brief Remove an existing stream.
      * @param name is the name of the stream which should be removed.
      *
      * Depends on UnBufferedRemoveStream.
    */
    virtual bool        RemoveStream(const char *name);

};


#endif
