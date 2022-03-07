/**
 * @file ProgressiveTypeCreator.h
 * @brief Header file for class AnyType
 * @date 5 Jan 2018
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

#ifndef PROGRESSIVETYPECREATOR_H_
#define PROGRESSIVETYPECREATOR_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompositeErrorManagement.h"
#include "TypeDescriptor.h"
#include "TypeConversionOperatorI.h"
#include "StaticListHolder.h"
#include "TypeCharacteristics.h"
#include "ReferenceT.h"
#include "Private/MemoryPageFile.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief A tool to create AnyObject that contains scalar/vectors/arrays of fixed size objects
 * It handles arrays or trees up to two dimensions, content can be fixed size or strings
 * Each row can either have the same size (matrix) or not (Vector<Vector>)
 * It produces a memory structure that can
 */
class DLL_API ProgressiveTypeCreator{

public:
	/**
	 * @brief Creates the object selecting the default PageSize
	 * Pagesize will grow if not sufficient to hold one row
	 * Multiple rows will be fitted on a PageSize if possible
	 * Rows will not span different Pages
	 */
	ProgressiveTypeCreator(uint32 pageSizeIn );

	/**
	 * @deletes the object and any memory allocated in the pages
	 */
	virtual ~ProgressiveTypeCreator();

	/**
	 * @brief first call - sets the output type
	 * From this the type of data storage is determined: fixed size or strings
	 */
	ErrorManagement::ErrorType Start(TypeDescriptor typeIn);

	/**
	 * @brief frees memory and readies the system for Start(). works on any state
	 * After this only Start is accepted
	 */
	void Clean();

	/**
	 * @adds an element. It will convert the string to the specified type. Callable after Start()
	 */
	ErrorManagement::ErrorType AddElement(CCString typeStringRepresentation);

	/**
	 * @brief Marks the end of a row of elements. Callable after Start()
	 * Signal the system that a second layer is possible (matrix or Vector<Vector>>
	 */
	ErrorManagement::ErrorType EndVector();

	/**
	 * @brief Marks the end of the object construction. Callable after Start()
	 * Enables last stage: GetReference()
	 */
	ErrorManagement::ErrorType End();

	/**
	 * @brief Allows retrieving the Object that has been built.
	 * Can only be done after End has been called. Callable after End()
	 * status is brought back to initial value, equivalent to Clean()
	 * Memory used to hold the object is severed from this object and linked to the reference x
	 * x references an AnyObject or a MemoryPageObject depending on complexity
	 * memory used during the creation is either transferred to the MemoryPageObject or recycled
	 */
	ErrorManagement::ErrorType GetReference(Reference &x);

	/**
	 * @brief returns Actual page size after growth etc...
	 */
	inline uint32 DefaultPageSize();

	/**
	 *
	 */
	 enum PTCState {
		// no data yet
		started 			= 0x0000,
		// one data
		scalar 				= 0x0001,
		// more than one
		vector 				= 0x0002,
		// end of first vector reached
		vectorEnd 			= 0x0003,
		// nth vector data in. all vectors same size so far
		matrixRow 	    	= 0x0004,
		// end of nth vector. all vectors same size
		matrixRowEnd    	= 0x0005,
		// nth vector data in. at least one vector has different size
		sparseMatrixRow		= 0x0006,
		// end of nth vector
		sparseMatrixRE 		= 0x0007,
		// finished as a scalar
		finishedS 			= 0x00E0,
		// finished as a vector
		finishedV 			= 0x00E1,
		// finished as a matrix
		finishedM 			= 0x00E2,
		// finished as a sparse matrix
		finishedSM 			= 0x00E3,
		//
		notStarted          = 0x00F0,
		// some error encountered
		error 				= 0x00FF,
		//
		statusMask		    = 0x00F0
	};


private:

	/**
	 * @brief a growable container of uint32
	 */
	class SizeStack: protected StaticListHolder{

	public:
		/**
		 *
		 */
		inline SizeStack();
		/**
		 *
		 */
		inline bool Push(uint32 size);
		/**
		 *
		 */
		inline uint32 NumberOfElements();
		/**
		 *
		 */
		inline uint32 operator[](uint32 index);

		/**
		 *
		 */
		inline void Clean();

	private:

		void operator=(const SizeStack&){}

	};


	/**
	 *
	 */
	ErrorManagement::ErrorType CompleteFixedSizeEl(uint8 *&dataPtr,uint32 &auxSize,uint8 *&auxPtr);

	/**
	 *
	 */
	ErrorManagement::ErrorType CompleteStringEl(uint8 *&dataPtr,uint32 &auxSize,uint8 *&auxPtr);

	/**
	 * @brief Core of implementation of GetObject
	 * called by GetObject
	 * if data is fragmented into multiple pages, it assumes that the start of data is in the first cell of the top page
	 * Therefore requires memory flipping.
	 * Would not support fragmented table of string pointers. (not generated currently)
	 */
	ErrorManagement::ErrorType GetReferencePrivate(Reference &x, uint8 *dataPtr, uint8 *auxPtr,uint32 auxSize);

	/**
	 * status within the progressive creation
	 */
	PTCState status;

	/**
	 * Any of the finished states
	 */
	inline bool Finished();

	/**
	 * Any of the started states
	 */
	inline bool Started();

	/**
	 * The size of the first vector and of all other vectors if status = matrix
	 */
	uint32 vectorSize;

	/**
	 * Number of rows in matrix
	 */
	uint32 matrixRowSize;

	/**
	 * Size of vector currently being scanned
	 */
	uint32 currentVectorSize;

	/**
	 * vectorSize*matrixRowSize or something less for sparse matrices
	 */
	uint32 numberOfElements;

	/**
	 * The object containing the memory
	 */
	MemoryPageFile pageFile;

	/**
	 * Size of one object in bytes
	 */
	uint32 	objectSize;
	/**
	 * converts from string to type
	 */
	const TypeConversionOperatorI *converter;
	/**
	 * holds the size of each vectors
	 */
	SizeStack sizeStack;
	/**
	 * the type of the data;
	 */
	TypeDescriptor type;

	/**
	 *
	 */
	bool isString;

	/**
	 * As set by the user. Different from defaultPageSize that has potentially grown
	 */
	uint32 originalPageSize;

private:

	void operator=(const ProgressiveTypeCreator&){}

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

//ProgressiveTypeCreator::PTCState x;
/**
 * Any of the finished states or error
 */
bool ProgressiveTypeCreator::Finished(){
	const uint32 mask 			= static_cast<uint32>(statusMask);
	const uint32 finishedMask 	= static_cast<uint32>(finishedS);
	uint32 statusAsInt = static_cast<uint32>(status);
	return ((statusAsInt & mask) == finishedMask);
}

/**
 * Any of the start,
 */
bool ProgressiveTypeCreator::Started(){
	const uint32 mask 			= static_cast<uint32>(statusMask);
	const uint32 startedMask 	= static_cast<uint32>(started);
	uint32 statusAsInt = static_cast<uint32>(status);
	return ((statusAsInt & mask) == startedMask);
}

inline uint32 ProgressiveTypeCreator::DefaultPageSize(){
	return pageFile.GetDefaultPageSize();
}


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PROGRESSIVETYPECREATOR_H_ */
