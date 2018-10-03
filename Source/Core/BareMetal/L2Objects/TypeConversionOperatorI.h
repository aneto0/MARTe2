/**
 * @file TypeConversionOperatorI.h
 * @brief Header file for class AnyType
 * @date 11 Nov 2017
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

#ifndef TYPECONVERSIONOPERATORI_H_
#define TYPECONVERSIONOPERATORI_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TypeDescriptor.h"
#include "FormatDescriptor.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
/**
 * @brief data conversion functors
 */
class DLL_API TypeConversionOperatorI{
public:
	/**
	 * @brief data conversion-copy/comparison method
	 * @param[in] dest   pointer to the area to be copied/compared to
	 * @param[in] source pointer to the area to be read an converted
	 * @param[in] numberOfElements number of elements to be processed
	 * @details converts each element pointed by source and copy/compare the conversion to dest
	 * @return true if no error and if the comparison was ok. comparisonFailure is returned in case of different content
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const =0;

	/**
	 * @brief data conversion-copy/comparison method
	 * @param[in] dest   pointer to the area to be copied/compared to
	 * @param[in] source pointer to the area to be read an converted
	 * @param[in] numberOfElements number of elements to be processed
	 * @param[in] fd alternative formatDescriptor for the conversion. Only used to convert to string/stream
	 * @details converts each element pointed by source and copy/compare the conversion to dest
	 * @return true if no error and if the comparison was ok. comparisonFailure is returned in case of different content
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements,const FormatDescriptor &fd){
		return Convert(dest, source,numberOfElements);
	};

	/**
	 * @brief destructor
	 */
	virtual ~TypeConversionOperatorI(){}

	/*
	 * @brief sets the compare bit
	 */
	TypeConversionOperatorI(){

	}

	/*
	 * @brief default format
	 */
	static const FormatDescriptor format;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* TYPECONVERSIONOPERATORI_H_ */
