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

#include "StreamMemoryReference.h"
#include "AdvancedErrorManagement.h"

/** Destructor */
StreamMemoryReference::~StreamMemoryReference() {
}

///
IOBuffer *StreamMemoryReference::GetInputBuffer() {
	return &buffer;
}

///
IOBuffer *StreamMemoryReference::GetOutputBuffer() {
	return &buffer;
}


/** 
    Reads data into buffer. 
    As much as size byte are read, 
    actual read size is returned in size. (unless complete = True)
    msecTimeout is how much the operation should last - no more - if not any (all) data read then return false  
    timeout behaviour depends on class characteristics and sync mode.
*/
bool StreamMemoryReference::Read(
                        char*               buffer,
                        uint32 &            size,
                        TimeoutType         msecTimeout,
                        bool                complete){
	this->buffer.Read(buffer,size);
	return true;
}

/** 
    Write data from a buffer to the stream. 
    As much as size byte are written, 
    actual written size is returned in size. 
    msecTimeout is how much the operation should last.
    timeout behaviour depends on class characteristics and sync mode. 
*/
bool StreamMemoryReference::Write(
                        const char*         buffer,
                        uint32 &            size,
                        TimeoutType         msecTimeout,
                        bool                complete){
	this->buffer.Write(buffer,size);	
	return true;
	
}

/** whether it can be written into */
bool StreamMemoryReference::CanWrite()const { 
	return (buffer.BufferReference() != NULL);
};

/** whether it can be  read */
bool StreamMemoryReference::CanRead()const { 
	return (buffer.Buffer() != NULL); 
};

/** The size of the stream */
int64 StreamMemoryReference::Size(){ 
	return buffer.UsedSize(); 
}

bool StreamMemoryReference::SetSize(int64 size){
    if (size < 0) size = 0;	
    buffer.SetUsedSize((uint32)size);
    return true; 
}

/** Moves within the file to an absolute location */
bool StreamMemoryReference::Seek(int64 pos){
	uint32 usedSize = buffer.UsedSize(); 
	if (pos > usedSize) {
		REPORT_ERROR_PARAMETERS(ParametersError,"pos=%i out of range=[0-%i] , moving to end of stream",pos,usedSize)
		buffer.Seek(usedSize);
		return false;
	}
	
	return buffer.Seek((uint32)pos);
}

/** Moves within the file relative to current location */
bool StreamMemoryReference::RelativeSeek(int32 deltaPos){
	return buffer.RelativeSeek(deltaPos);
}

/** Returns current position */
int64 StreamMemoryReference::Position() { 
	return buffer.Position(); 
}


/** can you move the pointer */
bool StreamMemoryReference::CanSeek() const {
	return true; 
};



