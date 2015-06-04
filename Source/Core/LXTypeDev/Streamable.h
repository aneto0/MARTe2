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


#if !defined STREAMABLE
#define STREAMABLE

//#include "TypeConversion.h"
#include "TimeoutType.h"
#include "StreamInterface.h"
#include "AnyType.h"
/**
 * @file Streamable.h
 * @brief Common father class for all streamable implementations.
 *
 * This functions implements the most useful and powerful functions like Printf and GetToken leaving
 * other functions implementation to the derived classes.  
 *
 * Since StreamInterface performs just an interface, this class is the father of the 
 * more specific streams like for example BufferedStream (file, socket, ecc.) StreamString (advanced strings manager with 
 * heap memory allocation), StreamMemoryReference (stack memory).
 *
 * StreamInterface functions for the multi stream are overloaded protected because are not used by all descendents.
 */ 




/** 
 * @brief Abstract super class for all streamables.
 *
 * It's the common point of implementation between Streamable descendent 
 * and MemoryMappedStreams.
 * Provide common abstract buffering scheme.
 */
class Streamable: public StreamInterface {
       
protected: // mode switch methods

    /// default constructor
    Streamable(){
    }

    /// default destructor
    virtual ~Streamable();

    
    // MULTISTREAM INTERFACE
    // DEFAULT IMPLEMENTATION = NO MULTISTREAM
    
    /**
     * @brief Returns how many streams are available.
     * @return 0 in this implementation. */
    virtual uint32      NOfStreams();

    /**
     * @brief Select the stream to read from. Switching may reset the stream to the start.
     * @param n is the number of the desired stream.
     * @return false in this implementation.  */
    virtual bool        Switch(uint32 n);

    /**
     * @brief Select the stream to read from. Switching may reset the stream to the start.
     * @param name is the name of the desired stream.
     * @return false in this implementation. */
    virtual bool        Switch(const char *name);

    /** 
     * @brief Returns the number of the selected stream.
     * @return 0 in this implementation.
     */
    virtual uint32      SelectedStream();

    /**
     * @brief The name of a stream.
     * @param n is the number of the desired stream.
     * @param name is the output buffer which will contains the stream name.
     * @param nameSize is the size of the buffer.
     * @return false in this implementation.
     */
    virtual bool        StreamName(uint32 n,char *name,int nameSize) const ;

    /**
     * @brief Add a new stream to write to.
     * @param name is the name to be assigned to the new stream.
     * @return false in this implementation. */
    virtual bool        AddStream(const char *name);

    /**  
     * @brief Remove an existing stream.
     * @param name is the name of the stream to remove.
     * @return false in this implementation */
    virtual bool        RemoveStream(const char *name);
    
    
public:  // auxiliary functions based on buffering

    /**
     * @brief Automatic cast to AnyType for a generic stream Printf. */
    operator AnyType(){
    	AnyType at;
    	at.dataPointer = (void *)this;
    	at.bitAddress  = 0;
    	at.dataDescriptor.type = TypeDescriptor::StreamInterface;
    	at.dataDescriptor.isConstant = false;
    	at.dataDescriptor.isStructuredData=false;
    	return at;
    }   
    
    /**
     * @brief Reads a token from the stream and writes it on the char* buffer provided.
     * @param terminator is a list of terminator characters.
     * @param outputBufferSize is the maximum size of the output buffer.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of characters to be skipped.
     * @return false if no data read, true otherwise.
     *
     * This function is performed for buffered streams, namely streams with an IOBuffer type as read buffer.
     * If the function GetInputBuffer returns NULL a StreamWrapperIOBuffer is created at the moment on the stack
     * with a dimension of 64 bytes and it substitutes the absent IOBuffer.
     *
        Extract a token from the stream into a string data until a terminator or 0 is found.
        Skips all skip characters, if you want to skip also terminator characters at the begging add them to skip characters.
        Returns true if some data was read before any error or file termination. False only on error and no data available
        The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
        skipCharacters=NULL is equivalent to skipCharacters = terminator.
        */
    virtual bool        GetToken(
                            char *              outputBuffer,
                            const char *        terminator,
                            uint32              outputBufferSize,
                            char *              saveTerminator,
                            const char *        skipCharacters);

    /**
     * @brief Reads a token from the stream and writes it on another stream.
     * @param terminator is a list of terminator characters.
     * @param saveTerminator is the found terminator in return.
     * @param skipCharacters is a list of characters to be skipped.
     * @return false if no data read, true otherwise.
     *
     * This function is performed for buffered streams, namely this stream should have an IOBuffer type as read buffer
     * and the output stream an IOBuffer as write buffer.
     * If the function GetInputBuffer returns NULL for this stream a StreamWrapperIOBuffer is created at the moment on the stack
     * with a dimension of 64 bytes and it substitutes the absent IOBuffer for read operations.
     * If the function GetOutputBuffer returns NULL for the output stream a StreamWrapperIOBuffer is created at the moment
     * on the stack with a dimension of 64 bytes and it substitutes the absent IOBuffer for write operations. 
     *
     ** extract a token from the stream into a string data until a terminator or 0 is found.
        Skips all skip characters, if you want to skip also terminator characters at the beginning add them to the skip characters.
        returns true if some data was read before any error or file termination. False only on error and no data available
        The terminator (just the first encountered) is consumed in the process and saved in saveTerminator if provided
        skipCharacters=NULL is equivalent to skipCharacters = terminator
        A character can be found in the terminator or in the skipCharacters list  in both or in none
        0) none                 the character is copied
        1) terminator           the character is not copied the string is terminated
        2) skip                 the character is not copied
        3) skip + terminator    the character is not copied, the string is terminated only if not empty
    */
    virtual bool        GetToken(
    						StreamInterface &   output,
                            const char *        terminator,
                            char *              saveTerminator=NULL,
                            const char *        skipCharacters=NULL);
   
    
    /**
     * @brief Skips a series of tokens delimited by terminators or 0.
     * @param count is the number of tokens to be skipped.
     * @param terminator is a list of terminator characters.
     * @return false if no data read, true otherwise.
     */
    virtual bool        SkipTokens(
                            uint32              count,
                            const char *        terminator);

      
    //  Methods to convert and print numbers and other objects 
    
    /**
     * @brief Very powerful function to handle data conversion into a stream of characters.
     * @param par is the element to be printed.
     * @param fd is the desired printf like format.
     * @return depends by par type and on the functions called for its conversion to printable characters.
     *
     * The class AnyType provides a void* pointer and a descriptor of the element type. Besides the descriptor
     * (integer, float, const char string, ecc), the right function is called passing the stream and the element
     * and it prints the element on the stream converting it in characters.
     * For more informations on AnyType class @see AnyType.h, for more informations on the format descriptor
     * @see FormatDescriptor.h
    */
    virtual bool 		Print(const AnyType& par,FormatDescriptor fd=standardFormatDescriptor);

    /** 
     * @brief Prints a list of elements looking to a specified format.
     * @param format is a printf like string format.
     * @param pars is a list of elements to be printed.
     * @see Print.
     *
         Pars is a vector terminated by voidAnyType value.
         Format follows the TypeDescriptor::InitialiseFromString.
         Prints all data pointed to by pars.
    */
    virtual bool 		PrintFormatted(const char *format, const AnyType pars[]);

    /**
     * @brief Copies a const char* into this stream from current position.
     * @param buffer is the buffer to be copied on the stream.
     * @return the result of the Write operation which depends on derived classes implementation.
    */
    virtual bool 		Copy(const char *buffer);

    /**
     * @brief Copies from stream current Position to end.
     * @param stream is the stream to be copied on this stream.
     * @param return false if the results of Read and Write streams operations fails.
    */
    virtual bool 		Copy(StreamInterface &stream); 
    
    
};


#endif
