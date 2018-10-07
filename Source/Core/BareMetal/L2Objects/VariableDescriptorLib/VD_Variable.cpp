/**
 * @file VD_Variable.cpp
 * @brief Header file for class AnyType
 * @date 7 Oct 2018
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#define DLL_API

#include "VD_Variable.h"
#include "VariableDescriptorLib.h"
#include "MemoryCheck.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe{
namespace VariableDescriptorLib{

static inline bool IsUpperCase(char8 c);


bool IsUpperCase(char8 c){
	bool ret = true;
	if ((c >= 'A')  && (c <= 'Z')) {
		ret = false;
	}
	return ret;
}

// supports both expanded and compressed representation
// pA and PA are converted in f and F and at the same time f and F are handled correctly
static const CCString terminals = "VMvmZDSzds";
static const CCString ATerminals = "AFf";

Variable::Variable(CCString modifiers,TypeDescriptor tdIn,bool stopAtFirstTerminal){
	td = tdIn;
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	bool finished = false;
	while (!finished){
		if (modifier == '\0'){
			this->Add(Dimension('\0',1));
			finished = true;
		} else
		if (ATerminals.In(modifier)){
			this->Add(Dimension(modifier,size));
		}
		{
			// !!! Actual pointers are not processed here! --> transform into a modified Td
			if ((modifier == 'P') || (modifier == 'p')){
				td = PointerType;
				if (!IsUpperCase(modifier)){
					td.dataIsConstant = true;
				}
				this->Add(Dimension('\0',1));
				finished = true;
			} else
			if (terminals.In(modifier)){

				this->Add(Dimension(modifier,SaturatedInteger<uint32>::Indeterminate()));
			}
		}
		if (!finished){
			GetLayerInfo(modifiers,modifier,size);
		}
	}

	char8 endType = '0';
	DimensionSize elementSize = 1;
	for (int i = (NDimensions()-1); i >= 0; i--){

		char8 type = (*this)[i].type;
		if (type != 'A'){
			elementSize = Type2Size((*this)[i].type,td);
			endType = type;
		} else {
			elementSize = elementSize * (*this)[i].numberOfElements ;
		}
		this->Access(i).endType = endType;
		this->Access(i).elementSize = elementSize;
	}
}

}
} //MARTe
