/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: ErrorManagement.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "BufferedStream.h"
#include "ErrorManagement.h"
#include "StringHelper.h"

/// default destructor
BufferedStream::~BufferedStream(){

//	writeBuffer.Flush();
}


bool BufferedStream::SetBufferSize(uint32 readBufferSize, uint32 writeBufferSize){

    operatingModes.canSeek = CanSeek(); 
	
    // mutex mode is enabled if CanSeek and both can Read and Write
	// in that case the stream is single and bidirectional
    if (CanSeek() && CanWrite() && CanRead()) {
    	operatingModes.mutexWriteMode = true;
	    operatingModes.mutexReadMode = false;
    }    	
    
    // minimum size = 8
    if (readBufferSize < 8)  readBufferSize  = 8;
    if (writeBufferSize < 8) writeBufferSize = 8;
    
    // do not allocate if not necessary
    if (!CanRead())  readBufferSize = 0;   
    if (!CanWrite()) writeBufferSize = 0;   

    // dump any data in the write Queue
    if (!FlushAndResync()) return false;
    
    // adjust readBufferSize
    readBuffer.SetBufferSize(readBufferSize);

    // adjust writeBufferSize
    writeBuffer.SetBufferSize(writeBufferSize);
    
    return true;
}

///
IOBuffer *BufferedStream::GetInputBuffer() {
    if (operatingModes.mutexWriteMode) {
       if (!SwitchToReadMode()) return NULL;
    }
	return &readBuffer;
}

///
IOBuffer *BufferedStream::GetOutputBuffer() {
    // check for mutually exclusive buffering and 
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
       if (!SwitchToWriteMode()) return NULL;
    }

    return &writeBuffer;
}


bool BufferedStream::Read(
                        char *              buffer,
                        uint32 &            size,
                        TimeoutType         msecTimeout,
                        bool                completeRead){
    // check for mutually exclusive buffering and 
    // whether one needs to switch to ReadMode
    if (operatingModes.mutexWriteMode) {
       if (!SwitchToReadMode()) return false;
    }

    // check whether we have a buffer
    if (readBuffer.BufferSize() > 0){ 

        // read from buffer first
        uint32 toRead = size;
    	
        // try once 
        readBuffer.Read(buffer, size);
    	
    	if (size == toRead ){
    		return true;
    	} else {  // partial only so continue
    		
    		// adjust toRead
    		toRead -= size;
    		
    		// decide whether to use the buffer again or just to read directly
    		if ((toRead*4) < readBuffer.MaxUsableAmount()){
    			if (!readBuffer.Refill()) return false;
    			
    			readBuffer.Read(buffer+size, toRead);
    			size += toRead;
    			
    			// should have completed
    			// as our buffer is at least 4x the need
    			return true;
    			
    		} else {
                // if needed read directly from stream
                if (!UnBufferedRead(buffer+size,toRead,msecTimeout)) return false;
                size += toRead;
    			return true;
    		}
    	} 
    }
    	
   // if needed read directly from stream
   return UnBufferedRead(buffer,size,msecTimeout);
}
  
    
/** Write data from a buffer to the stream. As much as size byte are written, actual size
    is returned in size. msecTimeout is how much the operation should last.
    timeout behaviour is class specific. I.E. sockets with blocking activated wait forever
    when noWait is used .... */
bool BufferedStream::Write(
                        const char*         buffer,
                        uint32 &            size,
                        TimeoutType         msecTimeout ,
                        bool                completeWrite)
{

    // check for mutually exclusive buffering and 
    // whether one needs to switch to WriteMode
    if (operatingModes.mutexReadMode) {
       if (!SwitchToWriteMode()) return false;
    }
    
    // buffering active?
    if (writeBuffer.BufferSize() > 0){
    	// separate input and output size
    	
    	uint32 toWrite = size;
    	// check available buffer size versus write size 
        // if size is comparable to buffer size there 
        // is no reason to use the buffering mechanism
        if (writeBuffer.MaxUsableAmount() > (4 *size)){
        	
        	// try writing the buffer
        	writeBuffer.Write(buffer, size);
        	
        	// all done! space available! 
        	if (size == toWrite) return true;
        	
        	// make space
        	if (!writeBuffer.Flush()) return false;

        	toWrite -= size;
        	uint32 leftToWrite = toWrite;
        	
        	// try writing the buffer
        	writeBuffer.Write(buffer+size, leftToWrite);

        	size+= leftToWrite;
        	
        	// should have been able to fill in it!!!
        	if (leftToWrite != toWrite) return false;
        	return true;               
        } else {
        	// write the buffer so far
        	if (!writeBuffer.Flush()) return false;
        }
        
    }
    return UnBufferedWrite(buffer,size,msecTimeout);

} 

/** The size of the stream */
int64 BufferedStream::Size() {
	// just commit all pending changes if any
	// so stream size will be updated     	
	FlushAndResync();
	// then call Size from unbuffered stream 
	return UnBufferedSize(); 
}


/** Moves within the file to an absolute location */
bool BufferedStream::Seek(int64 pos)
{
	// no point to go here if cannot seek
    if (!operatingModes.canSeek) return false;
    
    // if write mode on then just flush out data
    // then seek the stream
    if (writeBuffer.UsedSize() > 0){
    	writeBuffer.Flush();
    } else {
    	// if read buffer has some data, check whether seek can be within buffer
        if (readBuffer.UsedSize() > 0){
    		int64 currentStreamPosition = UnBufferedPosition();
    		int64 bufferStartPosition = currentStreamPosition - readBuffer.UsedSize();
    		
    		// if within range just update readBufferAccessPosition
    		if ((pos >= bufferStartPosition) &&
                    (pos < currentStreamPosition)){
    			readBuffer.Seek(pos - bufferStartPosition);
		
   			return true;
    		} else { // otherwise mark read buffer empty and proceed with normal seek
    			readBuffer.Empty();
    			// continues at the end of the function
    		}
    	}       	
    }
	
	return UnBufferedSeek(pos);
}

/** Moves within the file relative to current location */
bool  BufferedStream::RelativeSeek(int32 deltaPos){
    if (deltaPos == 0) return true;
    if (!operatingModes.canSeek) return false;
    
     // if write mode on then just flush out data
    if (writeBuffer.UsedSize() > 0){
    	// this will move the stream pointer ahead to the correct position
    	writeBuffer.Flush();
    } else {

	//save the current position because in case of out of range
        //the position becomes one of the buffer bounds.
	int64 currentPos=readBuffer.Position();

    	// on success it means we are in range
    	if (readBuffer.RelativeSeek(deltaPos)){
    		// no need to move stream pointer
    		return true;
    	}
    	// out of buffer range
		// adjust stream seek poistion to account for actual read buffer usage
		deltaPos -= (readBuffer.UsedSize()-currentPos);
		
		// empty buffer
		readBuffer.Empty();
    	
    }

    // seek 
	return UnBufferedSeek( UnBufferedPosition() + deltaPos);
}    

/** Returns current position */
int64 BufferedStream::Position() {
	// cannot seek
    if (!operatingModes.canSeek) return -1;
    
    // if write mode on then just flush out data
    if (writeBuffer.UsedSize() > 0){
    	return UnBufferedPosition() + writeBuffer.Position();
    }

    return UnBufferedPosition() - readBuffer.UsedSize() + readBuffer.Position();
}

/** Clip the stream size to a specified point */
bool BufferedStream::SetSize(int64 size)
{
    if (!operatingModes.canSeek) return false;
    
    // commit all changes
    FlushAndResync();
    
    return UnBufferedSetSize(size);
}


/** select the stream to read from. Switching may reset the stream to the start. */
bool BufferedStream::Switch(uint32 n)
{
    // commit all changes
    FlushAndResync();
	return UnBufferedSwitch(n);
}

/** select the stream to read from. Switching may reset the stream to the start. */
bool BufferedStream::Switch(const char *name)
{
    // commit all changes
    FlushAndResync();
    return UnBufferedSwitch(name);
}

/**  remove an existing stream .
    current stream cannot be removed 
*/
bool BufferedStream::RemoveStream(const char *name)
{
    // commit all changes
    FlushAndResync();
    return UnBufferedRemoveStream(name);
}



