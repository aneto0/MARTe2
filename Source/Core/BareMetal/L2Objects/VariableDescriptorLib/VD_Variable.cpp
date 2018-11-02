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
#include "CompositeErrorManagement.h"
#include "VD_Dimensions.h"

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

static DataDimension dummyDim(InvalidType(0));

const Dimension &Variable::operator[](uint32 index) const{
	const Dimension *p = firstDimension;
	while ((p != NULL) && (index > 0)){
		p = p->Next();
		index--;
	}

	if (p == NULL){
		REPORT_ERROR(ErrorManagement::FatalError,"access out of range");
		p = &dummyDim;
	}

	return *p;
}

Dimension &Variable::Access(uint32 index) {
	Dimension *p = firstDimension;
	while ((p != NULL) && (index > 0)){
		p = p->Next();
		index--;
	}

	if (p == NULL){
		REPORT_ERROR(ErrorManagement::FatalError,"access out of range");
		p = &dummyDim;
	}

	return *p;
}

void Variable::Add(Dimension *dimension){
	Dimension::AddToEnd(firstDimension,dimension);
}

// supports both expanded and compressed representation
// pA and PA are converted in f and F and at the same time f and F are handled correctly
//static const CCString terminals = "VMvmZDSzds";
//static const CCString ATerminals = "AFf";


static inline bool isLowerCase(char8 c){
	return (c >= 'a');
}

Variable::Variable(CCString modifiers,TypeDescriptor tdIn){
//printf("%s = ",modifiers.GetList());

	firstDimension = NULL;
	td = tdIn;
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	bool finished = false;
	while (!finished){
		bool isConstant = isLowerCase(modifier);
		switch(modifier){
		case 'O':{
			Add(new DataDimension(tdIn));
			finished = true;
		} break;
		case 'A':{
			Add(new ArrayDimension(size));
		}break;
		case 'f':
		case 'F':{
			Add(new PointerToArrayDimension(size,isConstant));
		}break;
		case 'v':
		case 'V':{
			Add(new VectorDimension(isConstant));
		}break;
		case 'm':
		case 'M':{
			Add(new MatrixDimension(isConstant));
		}break;
		case 's':
		case 'S':
		case 'z':
		case 'Z':{
			Add(new ZTADimension(false,isConstant));
		}break;
		case 'd':
		case 'D':{
			Add(new ZTADimension(true,isConstant));
		}break;
		case 'p':{
			Add(new DataDimension(ConstPointerType));
		}break;
		case 'P':{
			Add(new DataDimension(PointerType));
		}break;
		default:{
			COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"Unexpected type char ",modifier);
			finished = true;
		}break;
		}
		if (!finished){
			GetLayerInfo(modifiers,modifier,size);
		}
	}

}

}
} //MARTe
