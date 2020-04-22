/**
 * @file StringReaders.h
 * @brief Header file for class AnyType
 * @date Apr 22, 2020
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

#ifndef PRIVATE_STRINGREADERS_H_
#define PRIVATE_STRINGREADERS_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/



#include "CCString.h"
#include "DynamicCString.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief interface definition
 */
class StringReader{

public:
	/**
	 * @brief to obtain the string to be parsed
	 * and increases pointer to the next object
	 * @return the buffer where the string is written to
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index)=0;

	/**
	 * @brief virtual destructor
	 */
	virtual ~StringReader(){}

};

/**
 * extract tokens Streams
 * manages only one Stream as size of actual class is not known
 */
class StreamReader: public StringReader{
public:

	/**
	 * @param [in]  streamSizeIn the size of the actual Stream derived object
	 */
	StreamReader(uint32 streamSizeIn);

	/**
	 *
	 */
	virtual ~StreamReader();

	/**
	 * @brief to be able to use the desired source
	 * @details directly parses the stream here. will not seek to start nor leave position untouched
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index);

private:
	/**
	 * the actual size of the stream derived class
	 */
	const uint32 streamSize;
	/**
	 *
	 */
	DynamicCString buffer;
};

/**
 * @reads from SString to String
 */
class SStringReader: public StringReader{
public:

	/**
	 * @brief to be able to use the desired source
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index);
};

class CCStringReader: public StringReader{
public:

	/**
	 * @brief to be able to use the desired source
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index);
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_PRIVATE_STRINGREADERS_H_ */
