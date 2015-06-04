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
 * $Id: StreamInterface.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file StreamInterface.h 
 * @brief The base interface for the streams
 *
 * Defines the prototipes of a generic stream class.
 */
#ifndef STREAM_INTERFACE_H
#define STREAM_INTERFACE_H

#include "TimeoutType.h"
#include "AnyType.h"
#include "FormatDescriptor.h"
#include "IOBuffer.h"


/** @brief A more abstract version of Streamable. It is used to allow referring to streams at lower levels */
class StreamInterface{
//friend class Streamable;
public:
// BUFFERED STREAMING 
    /**
     * @brief Pure virtual method. Get the read buffer.
     * @return a pointer to the read buffer. */ 
    virtual IOBuffer *GetInputBuffer() = 0;

    /**
     * @brief Pure virtual method. Get the write buffer.
     * @return a pointer to the write buffer. */
    virtual IOBuffer *GetOutputBuffer() = 0;
	
public:
    /** @brief Default destructor */
    virtual             ~StreamInterface(){};

public: 

    // PURE STREAMING (UNBUFFERED)

    /** 
     * @brief Pure virtual method. Reads data from the stream to a char* buffer.
     * @param buffer is the buffer where datas must be copied.
     * @param size is the desired number of bytes to copy.
     * @param msecTimeout is the desired timeout.
     * @param complete is a flag.
     *
     * The behavior depends by derived classes implementation. 
     * 
        Reads data into buffer. 
        As much as size byte are read, 
        actual read size is returned in size. (unless complete = True)
        msecTimeout is how much the operation should last - no more - if not any (all) data read then return false  
        timeout behaviour depends on class characteristics and sync mode.
        return false implies failure to comply with minimum requirements:
        timeout and complete and data read  != size
        timeout and data read == 0
        error in the stream  ==> no point to try again      
        parameters error, for instance buffer = NULL 
    */
    virtual bool        Read(
                            char*               buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false)=0;

    /** 
     * @brief Pure virtual method. Writes from a const char* buffer to the stream.
     * @param buffer contains the datas which must be copied on the stream.
     * @param size is the desired number of bytes to write.
     * @param msecTimeout is the desired timeout.
     * @param complete is a flag.
     *
     * The behavior depends by derived classes implementation. 
     *
        Write data from a buffer to the stream. 
        As much as size byte are written, 
        actual written size is returned in size. 
        msecTimeout is how much the operation should last.
        timeout behaviour depends on class characteristics and sync mode. 
        return false implies failure to comply with minimum requirements:
        timeout and complete and data written  != size
        timeout and data written == 0
        error in the stream ==> no point to try again       
        parameters error, for instance buffer = NULL 
    */
    virtual bool        Write(
                            const char*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault,
                            bool                complete        = false) = 0;

    /**
     * @brief Pure virtual function. Defines if write operations can be performed on the stream.
     * @return return value depends from derived classes implementation. */
    virtual bool        CanWrite()const =0;

    /**
     * @brief Pure virtual function. Defines if read operations can be performed on the stream.
     * @return return value depends from derived classes implementation. */
    virtual bool        CanRead()const =0;
        
    /**
     * @brief Writes a character on the stream.
     * @param c is the character to write on the stream.
     * @return depends from derived classes implementation.
     *
     * Uses the derived class implementation of Write function with one as size parameter,
     * then the function behavior depends from the derived class Write function.*/
    inline bool         PutC(char c){
    	uint32 size = 1;
    	return Write(&c,size);
    }

    /**
     * @brief Reads a character from the stream.
     * @param c is the character in return.
     * @return depends from derived classes implementation.     
     *
     * Uses the derived class implementation of Read function with one as size parameter,
     * then the function behavior depends from the derived class Read function. */
    inline bool         GetC(char &c) {
    	uint32 size = 1;
    	return Read(&c,size);
    	
    }

    // SYNCHRONISATION INTERFACE

    /** 
     * @brief Defines if operations could be blocking.
     * @return false. It means that a default stream has non blocking operations.
     *
     * Anyway since the method is virtual, it could be implemented differently in
     * the derived classes.
     *
       whether it can wait to complete operation 
       or in absence of data or buffer space the 
       operation terminates immediately       
       msecTimeout is used to limit the blocking time 
       when blocking is active. It is not used otherwise - unless
       complete is set in which case the operation is tried multiple 
       times each after fixed interval (set as timeout/10 or 1ms min) 
       This implies that class implementations of Read and Write 
       might have to involve a select call or something similar
       for classes light String blocking has no meaning.        
    */
    virtual bool        CanBlock(){ return false; };
    
    /** 
     * @brief Sets or unsets the blocking mode.
     * @param flag is a flag.
     * @return false. It means that by default a stream cannot set blocking mode.
     *
     * Anyway since this method is virtual it could be implemented differently
     * in the derived classes. */
    virtual bool        SetBlocking(bool flag){ return false; };

    // RANDOM ACCESS INTERFACE

    /**
     * @brief Pure virtual method. The size of the stream.
     * @return the size of the stream depending on derived classes implementation.  */
    virtual int64       Size() = 0;

    /** 
     * @brief Pure virtual method. Moves within the stream to an absolute location.
     * @param pos is the desired absolute position.
     * @return return value depends on derived classes implementation. */
    virtual bool        Seek(int64 pos) = 0;
    
    /** 
     * @brief Pure virtual method. Moves within the file relative to current location.
     * @param deltaPos is the gap from the current position.
     * @return return value depends on derived classes implementation. */
    virtual bool        RelativeSeek(int32 deltaPos)=0;
    
    /**
     * @brief Pure virtual method. Returns current position.
     * @return the current position in the stream. */
    virtual int64       Position()  = 0 ;

    /**
     * @brief Pure virtual method. Clip the stream size to the desired value.
     * @param size is the desired size.
     * @return return value depends on the derived classes implementation. */
    virtual bool        SetSize(int64 size) = 0;

    /** 
     * @brief Pure virtual method. Defines if seek operations can be performed on the stream.
     * @return return value depends on the derived classes implementation. */
    virtual bool        CanSeek()const =0;

    // MULTISTREAM INTERFACE

    /**
     * @brief Pure virtual method. Returns the number of avaiable streams.
     * @return the number of avaiable streams. */
    virtual uint32      NOfStreams() =0;

    /**
     * @brief Pure virtual method. Select the stream by number.
     * @param n is the number of the desired stream.
     * @return depends on the derived classes implementation. */
    virtual bool        Switch(uint32 n)=0;

    /** 
     * @brief Pure virtual method. Select the stream by name.
     * @param name is the name of the desired stream.
     * @return depends on the derived classes implementation. */
    virtual bool        Switch(const char *name)=0;

    /** 
     * @brief Pure virtual method. Returns the number of the selected stream.
     * @return the number of the selected stream. */
    virtual uint32      SelectedStream()=0;

    /**
     * @brief Pure virtual method. Returns the name of one of the avaiables streams.
     * @param n is the number of the desired stream.
     * @param name is the name of the desired stream in return.
     * @param nameSize is the maximum size of the name buffer.
     * @return depends on the derived classes implementation. */
    virtual bool        StreamName(uint32 n,char *name,int nameSize)const =0;

    /** 
     * @brief Pure virtual method. Add a new stream.
     * @param name is the name to be assigned to the new stream.
     * @return depends on the derived classes implementation. */
    virtual bool        AddStream(const char *name)=0;

    /** 
     * @brief Pure virtual method. Remove an existing stream.
     * @param name is the name of the stream to be removed.
     * @return depends on the derived classes implementation. */
    virtual bool        RemoveStream(const char *name)=0;

    // Utility functions interface

    /**
     * @brief Pure virtual method. Get a token from the stream and write it on a char* buffer.
     * @param outputBuffer is the buffer where the token must be written.
     * @param terminator is a list of terminator characters.
     * @param outputBufferSize is the maximum size of the output buffer.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of character to skip.
     * @return depends on the derived classes implementation. 
     * 
	Extract a token from the stream into a string data until a terminator or 0 is found.
        Skips all skip characters and those that are also terminators at the beginning
        returns true if some data was read before any error or file termination. False only on error and no data available
        The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
        skipCharacters=NULL is equivalent to skipCharacters = terminator
        {BUFFERED}    */
    virtual bool        GetToken(
                            char *              outputBuffer,
                            const char *        terminator,
                            uint32              outputBufferSize,
                            char *              saveTerminator,
                            const char *        skipCharacters)=0;

    /** 
     * @brief Pure virtual method. Get a token from the stream and write it on another stream.
     * @param output is the output stream.
     * @param terminator is a list of terminato characters.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of characters to skip.
     * @return depends on the derived classes implementation.
     *
 	Extract a token from the stream into a string data until a terminator or 0 is found.
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
    virtual bool        GetToken(
    		                StreamInterface &  	output,
                            const char *        terminator,
                            char *              saveTerminator=NULL,
                            const char *        skipCharacters=NULL)=0;

    /**
     * @brief Pure virtual method. Skips a series of tokens delimited by terminators or 0.
     * @param count is the number of tokens to skip.
     * @param terminator is a list of terminator characters.
     * @return depends on the derived classes implementation. */
    virtual bool        SkipTokens(
                            uint32              count,
                            const char *        terminator)=0;
  
    /**
     * @brief Inline method which use pure virtual GetToken. Extract a line and write it on a char* buffer.
     * @param outputBuffer is the buffer where de line must be written.
     * @param outputBufferSize is the maximum size of the output buffer.
     * @param skipTerminators defines if the \r char should be skipped (true) or not (false).
     * @return depends on the derived classes implementation.  */
    inline bool 		GetLine(
    						char *				outputBuffer,
    						uint32 				outputBufferSize,
    						bool 				skipTerminators=True){
        const char *skipCharacters = "\r";
#if defined (_WIN32)
        if (!skipTerminators) skipCharacters = "\r";
#else
        if (!skipTerminators) skipCharacters = "";
#endif
        return GetToken(outputBuffer,"\n",outputBufferSize,NULL,skipCharacters);
    }

    /**
     * @brief Inline method which use pure virtual GetToken. Extract a line and write it on another stream.
     * @param outputBuffer is the output stream where de line must be written.
     * @param skipTerminators defines if the \r char should be skipped (true) or not (false).
     * @return depends on the derived classes implementation.  */
    inline bool 		GetLine(
    						StreamInterface &  	output,
    						bool 				skipTerminators=True){
        const char *skipCharacters = "\r";
#if defined (_WIN32)
        if (!skipTerminators) skipCharacters = "\r";
#else
        if (!skipTerminators) skipCharacters = "";
#endif
        return GetToken(output,"\n",NULL,skipCharacters);
    }    
    
    /**
     * @brief Pure virtual method. Prints a generic type in a specified format looking at the format descriptor.
     * @param par a generic type to print.
     * @param fd is the format descriptor for the desired print format.
     * @return true if successful, false otherwise.
    */
    virtual bool 		Print(const AnyType& par,FormatDescriptor fd=standardFormatDescriptor)=0;

    /**
     * @brief Pure virtual method. Prints a list of types in a specified format.
     * @param format is the printf like desired format.
     * @param pars is a list of types to be printed.
     * @return true if successful, false otherwise. 
    */
    virtual bool 		PrintFormatted(const char *format, const AnyType pars[])=0;

    /** 
     * @brief PrintFormatted with only one element to print.
     * @param format is the desired printf like format.
     * @param par1 is the element to be printed.
     * @return true if successfull, false otherwise.
    */
    inline bool 		Printf(const char *format, const AnyType& par1){
    	AnyType pars[2] = { par1,voidAnyType};
    	return PrintFormatted(format, pars);
    }
    
    /** 
     * @see Printf with two elements to print.
    */
    inline bool 		Printf(const char *format, const AnyType& par1, const AnyType& par2){
    	AnyType pars[3] = { par1,par2,voidAnyType}; 
    	return PrintFormatted(format, pars);
    }

    /** 
     * @see Printf with three elements to print.
    */
    inline bool 		Printf(const char *format, const AnyType& par1, const AnyType& par2, const AnyType& par3){
    	AnyType pars[4] = { par1,par2,par3,voidAnyType}; 
    	return PrintFormatted(format, pars);
    }

    /** 
     * @see Printf with four element to print.
    */
    inline bool 		Printf(const char *format, const AnyType& par1, const AnyType& par2, const AnyType& par3, const AnyType& par4){
    	AnyType pars[5] = { par1,par2,par3,par4,voidAnyType}; 
    	return PrintFormatted(format, pars);
    }
    
    /**
     * @brief Pure virtual method. Copies a const char* buffer into the stream from current position.
     * @param buffer is the buffer which must be copied on the stream.
     * @return depends on the derived classes implementation.
    */
    virtual bool 		Copy(const char *buffer) = 0;

    /**
     * @brief Pure virtual method. Copies on this stream another stream from its current Position to end.
     * @param stream is the stream which must be copied in this stream.
     * @return depends on the derived classes implementation. 
    */
    virtual bool 		Copy(StreamInterface &stream) = 0; 
    
};

#endif
