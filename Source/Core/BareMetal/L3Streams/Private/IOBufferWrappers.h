/**
 * @file IOBufferWrappers.h
 * @brief Header file for class AnyType
 * @date Jul 9, 2020
 * @author Filippo Sartori
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef IOBUFFERWRAPPERS_H_
#define IOBUFFERWRAPPERS_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
//#include "TypeConversionFactoryI.h"
//#include "TypeConversionManager.h"
#include "IOBuffer.h"
//#include "IOBufferPrivate.h"
#include "StreamString.h"
#include "CompositeErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * @brief provides a generic interface between IOBuffer and arrays of character streams
 */
class IOBufferWrapper;
/**
 * @brief connects IOBuffer to a single Stream
 */
class IOBufferWrapperStream;
/**
 * @brief connects IOBuffer to a SStrings
 */
class IOBufferWrapperSString;


/**
 * @brief provides a generic interface between IOBuffer and arrays of character streams
 */
class IOBufferWrapper: public IOBuffer{

public:
    /**
     * @brief interface to this stream
     */
    virtual void Wrap(void *ptr)=0;

    /**
     * @brief flush the IOBuffer
     */
    ErrorManagement::ErrorType  Flush();

    /**
     * @brief constructor
     */
    IOBufferWrapper();

    /**
     * @brief switch to next stream
     */
    virtual ErrorManagement::ErrorType  Next();

    /**
     * @brief check for any error
     */
    virtual ErrorManagement::ErrorType  Check(){
        ErrorManagement::ErrorType  ret;
        return ret;
    }


private:
    /**
     * @brief buffer for the IOBuffer
     */
    char buffer[32];

};

/**
 * @brief connects IOBuffer to a single Stream
 */
class IOBufferWrapperStream: public IOBufferWrapper{
public:
    /**
     * @brief constructor
     */
    IOBufferWrapperStream(uint32 sizeIn);

    /**
     * @brief interfaces to the Stream
     */
    virtual void Wrap(void *ptr);

    /**
     * @brief switch to next stream
     */
    virtual ErrorManagement::ErrorType Next();

protected:
    /** pointer to the current output */
    uint8 *pointer;

    /** the stream */
    StreamI *stream;

    /** the size of the object StreamI derived */
    uint32 size;

    /**
     * @brief dumps the IOBuffer to the Stream
     * */
    virtual bool NoMoreSpaceToWrite() ;
};

/**
 * @brief connects IOBuffer to a SStrings
 */
class IOBufferWrapperSString: public IOBufferWrapperStream{
public:

    /**
     * @brief constructor
     */
    IOBufferWrapperSString();

    /**
     * @brief interfaces to the Stream
     */
    virtual void Wrap(void *ptr);

    /**
     * @brief switch to next stream
     */
    virtual ErrorManagement::ErrorType  Next();

protected:
    /**
     *
     */
    StreamString *ss;
};

/**
 * @brief connects IOBuffer to a DynamicCString
 */
class IOBufferDynStringWrapper: public IOBufferWrapper{
public:

    /**
     * @brief constructor
     */
    IOBufferDynStringWrapper();

    /**
     * @brief interfaces to the Stream
     */
    virtual void Wrap(void *ptr);

    /**
     * @brief switch to next stream
     */
    virtual ErrorManagement::ErrorType  Next();

protected:

    /**
     * @brief dumps the IOBuffer to the Stream
     * */
    virtual bool NoMoreSpaceToWrite();

private:
    /**
     * @brief pointer to array of DynamicCString
     */
    DynamicCString *string;
};

/**
 *
 */
class IOBufferCStringCompareWrapper: public IOBufferWrapper{
public:

    /**
     * @brief constructor
     */
    IOBufferCStringCompareWrapper();

    /**
     * @brief interfaces to the Stream
     */
    virtual void Wrap(void *ptr);

    /**
     * @brief switch to next stream
     */
    virtual ErrorManagement::ErrorType  Next();

    /**
     * @brief check for any error
     */
    virtual ErrorManagement::ErrorType  Check();
protected:

    /**
     * @brief dumps the IOBuffer to the Stream
     * */
    virtual bool NoMoreSpaceToWrite();

private:
    /**
     * @brief pointer to array of CCString
     */
    CCString *string;

    /**
     * @brief pointer to current comparison point
     */
    CCString currentString;

    /**
     * the result
     */
    bool IsSame;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_PRIVATE_IOBUFFERWRAPPERS_H_ */
