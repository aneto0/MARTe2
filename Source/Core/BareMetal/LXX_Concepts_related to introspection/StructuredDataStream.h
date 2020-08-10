/**
 * @file StructuredDataStream.h
 * @brief Header file for class AnyType
 * @date May 15, 2020
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

#ifndef STRUCTUREDDATASTREAM_H_
#define STRUCTUREDDATASTREAM_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "BufferedStreamI.h"
#include "Stack.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * Adds a StructuredDataI interface to a stream that has seeking capabilities
 * Supports standard,json and xml formatting
 * Support only read and move operations
 */
class StructuredDataStream: StructuredDataI{

public:
	/**
	 * Adds a StructuredDataI interface to a stream
	 * format is the stream formatting
	 */
	StructuredDataStream(const BufferedStreamI &stream,CCString format);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Read(CCString path,const AnyType &value) = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Read(CCString path,Reference &object,bool borrow=true) = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType GetVariableInformation(
    		CCString 			path,
    		TypeDescriptor &	td,
			uint32 &			nOfDimensions,
			uint32 *			dimensionSizes) const = 0;

/**
 * require write access!
 */
    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(CCString name, const AnyType &value){
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(Reference object){
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Copy(StructuredDataI &destination) {
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * Affect the current position
     */

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToRoot() = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToAncestor(uint32 generations) = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveAbsolute(CCString path) = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveRelative(CCString path) = 0;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToChild(const uint32 childIdx) = 0;

    /**
     * Read path information
     */

    /**
     * @see StructuredDataI
     */
    virtual CCString GetName()=0;

    /**
     * @see StructuredDataI
     */
    virtual CCString GetChildName(const uint32 index)=0;

    /**
     * @see StructuredDataI
     */
    virtual uint32 GetNumberOfChildren()=0;


/**
 *  Affect path
 */

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType AddToCurrentNode(Reference node) {
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType CreateAbsolute(CCString path){
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType CreateRelative(CCString path){
    	return ErrorManagement::InvalidOperation;
    }

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Delete(CCString path){
    	return ErrorManagement::InvalidOperation;
    }

private:

	// start position of each of the substructures addressed by the current path
	Stack<uint64> positionHistory;

	//
	DynamicCString path;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_STRUCTUREDDATASTREAM_H_ */
