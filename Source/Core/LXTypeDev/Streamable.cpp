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

#include "Streamable.h"
#include "AdvancedErrorManagement.h"
#include "StringHelper.h"
#include "IOBufferFunctions.h"
#include "StreamWrapperIOBuffer.h"


/// default destructor
Streamable::~Streamable(){
}

/** how many streams are available */
uint32 Streamable::NOfStreams() { return 0; }

/** select the stream to read from. Switching may reset the stream to the start. */
bool Streamable::Switch(uint32 n){ return false; }

/** select the stream to read from. Switching may reset the stream to the start. */
bool Streamable::Switch(const char *name){ return false; }

/** how many streams are available */
uint32 Streamable::SelectedStream(){ return 0; }

/** the name of the stream we are using */
bool Streamable::StreamName(uint32 n,char *name,int nameSize)const { return false; }

/**  add a new stream to write to. */
bool Streamable::AddStream(const char *name){ return false; }

/**  remove an existing stream . */
bool Streamable::RemoveStream(const char *name){ return false; }


/** extract a token from the stream into a string data until a terminator or 0 is found.
    Skips all skip characters and those that are also terminators at the beginning
    returns true if some data was read before any error or file termination. False only on error and no data available
    The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
    skipCharacters=NULL is equivalent to skipCharacters = terminator
    {BUFFERED}    */
bool Streamable::GetToken(
                            char *              outputBuffer,
                            const char *        terminator,
                            uint32              outputBufferSize,
                            char *              saveTerminator,
                            const char *        skipCharacters){
	
	// retrieve stream mechanism
	IOBuffer *inputIOBuffer = GetInputBuffer(); 
	if (inputIOBuffer == NULL){
		char stackBuffer[64];		
		StreamWrapperIOBuffer inputIOBuffer (this,stackBuffer,sizeof (stackBuffer));
		
		bool ret = GetTokenFromStream(inputIOBuffer, outputBuffer,terminator,outputBufferSize,saveTerminator,skipCharacters);
		
		return ret;
	}
	
	return GetTokenFromStream(*inputIOBuffer, outputBuffer,terminator,outputBufferSize,saveTerminator,skipCharacters);
}


/** extract a token from the stream into a string data until a terminator or 0 is found.
    Skips all skip characters and those that are also terminators at the beginning
    returns true if some data was read before any error or file termination. False only on error and no data available
    The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
    skipCharacters=NULL is equivalent to skipCharacters = terminator
    {BUFFERED}
    A character can be found in the terminator or in the skipCharacters list  in both or in none
    0) none                 the character is copied
    1) terminator           the character is not copied the string is terminated
    2) skip                 the character is not copied
    3) skip + terminator    the character is not copied, the string is terminated if not empty
*/
bool Streamable::GetToken(
							StreamInterface &   output,
                            const char *        terminator,
                            char *              saveTerminator,
                            const char *        skipCharacters){

	// retrieve stream mechanism
	IOBuffer *inputIOBuffer   = GetInputBuffer();
	IOBuffer *outputIOBuffer  = output.GetOutputBuffer();

	bool ret = false; 
	
	if (inputIOBuffer == NULL){
		char stackBuffer[64];
		//create a buffer on the stack as the read buffer.		
		StreamWrapperIOBuffer inputIOBufferS (this,stackBuffer,sizeof (stackBuffer));
		inputIOBuffer   = &inputIOBufferS;
		
		if (outputIOBuffer == NULL){
			char stackBuffer[64];
			//create a buffer on the stack as the write buffer		
			StreamWrapperIOBuffer outputIOBufferS (&output,stackBuffer,sizeof (stackBuffer));		
			outputIOBuffer   = &outputIOBufferS;
			
			ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);		
			
			
		} else { 	
			ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
		}
	} else {
		
		if (outputIOBuffer == NULL){
			char stackBuffer[64];		
			//create a buffer on the stack as write buffer.
			StreamWrapperIOBuffer outputIOBufferS (&output,stackBuffer,sizeof (stackBuffer));		
			outputIOBuffer = &outputIOBufferS; 

			ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);
			
		} else { 
		
			ret = GetTokenFromStream(*inputIOBuffer, *outputIOBuffer,terminator,saveTerminator,skipCharacters);		
		}		
	}

	return ret;
}
	

/** to skip a series of tokens delimited by terminators or 0
    {BUFFERED}    */
bool Streamable::SkipTokens(
                            uint32              count,
                            const char *        terminator){

	// retrieve stream mechanism
	IOBuffer *inputBuffer = GetInputBuffer(); 
	if (inputBuffer == NULL){
		char stackBuffer[64];		
		StreamWrapperIOBuffer inputBuffer (this,stackBuffer,sizeof (stackBuffer));
		
		return SkipTokensInStream(inputBuffer,count,terminator);
	}
	
	return SkipTokensInStream(*inputBuffer,count,terminator);
}

bool Streamable::Print(const AnyType& par,FormatDescriptor fd){

	// retrieve stream mechanism
	IOBuffer *outputBuffer = GetOutputBuffer(); 
	if (outputBuffer == NULL){
		char stackBuffer[64];		
		StreamWrapperIOBuffer outputBuffer (this,stackBuffer,sizeof (stackBuffer));

		return PrintToStream(outputBuffer,par,fd);
	}
		
	return PrintToStream(*outputBuffer,par,fd);
}

bool Streamable::PrintFormatted(const char *format, const AnyType pars[]){

	// retrieve stream mechanism
	// the output buffer is flushed in streamable.
	IOBuffer *outputBuffer = GetOutputBuffer(); 
	if (outputBuffer == NULL){
		char stackBuffer[64];
		StreamWrapperIOBuffer outputBuffer (this,stackBuffer,sizeof (stackBuffer));
		
		return PrintFormattedToStream(outputBuffer,format,pars);
		
	}
	return PrintFormattedToStream(*outputBuffer,format,pars);
}


/**
 * copies a const char* into this stream from current position
*/
bool Streamable::Copy(const char *buffer){
	if (buffer == NULL){
		return false;
	}
	
	uint32 len = (uint32 )StringHelper::Length(buffer);
	
	return Write(buffer,len,TTDefault,true);	
}

/**
 * copies from stream current Position to end
*/
bool Streamable::Copy(StreamInterface &stream){

	char buffer[256];
	uint32 size = sizeof(buffer);

	//read in buffer
	bool ret = stream.Read(buffer,size);
	while ((ret) && (size > 0)){
		
		//write in buffer
		ret = Write(buffer,size,TTDefault,true);
	        //in case of ret false don't write again
         	size = 0;

		//if successful, read again and size becomes zero if 
                //there is no more data to read
		if (ret){
			size = sizeof(buffer);
			ret = stream.Read(buffer,size);
		}
	}
	//if exit because ret is false
        //something was read in buffer
	if (size > 0){
		//write on the stream
		//ret = ret && Write(buffer,size,TTDefault,true);
	        Write(buffer,size,TTDefault,true);
	//	size = 0;
	}
	
	return ret;
	
} 













