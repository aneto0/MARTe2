/**
 * @file APLookup.h
 * @brief Header file for class AnyType
 * @date 8 Oct 2017
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

#ifndef APLOOKUP_H_
#define APLOOKUP_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "BasicType.h"
#include "Vector.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 *
 */
struct APLookUp{
	//
	CombinedArrayType 	arrayCode;
	//
	char8 				keyCode;
	/**
	 * 0 means unknown size
	 */
	uint32 				layerSize;
	// true for simple arrays[] - they multiply the size of the layer below
	// false for pointers or objects with pointers inside - data access is indirect
	bool 				isMultiplyingLayer;
	//
	bool 				sizeFollows;
	//
	CCString			cExpansionPre;
	//
	CCString			cExpansionPost;
	//
	CCString			cExpansionEnd;
};

extern const CCString nullString;

extern const APLookUp APLookupTable1[];

extern const APLookUp APLookupTable2[];

const APLookUp *reverseLookUpCode(char8 code);

 CombinedArrayType reverseLookUpArrayTypeGen(char8 code, const APLookUp *apl);

/**
 * returns false if not found.
 * looks in both tables
 */
bool reverseLookUpArrayProperties(char8 code, const APLookUp *&apl);

char8 lookUpArrayTypeGen(CombinedArrayType arrayCode, const APLookUp *apl);

char8 lookUpArrayType(CombinedArrayType arrayCode);

char8 lookUpCompositeArrayCode(CombinedArrayType arrayCode);



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_APLOOKUP_H_ */
