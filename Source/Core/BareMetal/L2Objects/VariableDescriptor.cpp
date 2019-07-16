/**
 * @file VariableDescriptor.cpp
 * @brief Source file for class VariableDescriptor
 * @date Nov 29, 2016
 * @author fsartori
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

 * @details This source file contains the definition of all the methods for
 * the class VariableDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringHelper.h"
#include "MemoryOperators.h"
#include "DynamicCString.h"
#include "MemoryCheck.h"
#include "TypeConversionManager.h"
#include "VariableDescriptor.h"
#include "Private/VariableDescriptorLib.h"
#include "Private/VariableCloner.h"
#include "ReferenceT.h"
#include "AnyObject.h"
#include "Private/MemoryPageObject.h"
#include "Private/MemoryPageFile.h"
#include "ZeroTerminatedArray.h"
#include "StaticList.h"
#include "SaturatedInteger.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * TODO: Change this to fit the specific needs of the platform
 */
const uint32 defaultAllocationSize = 256*256;

void VariableDescriptor::AddModifiersLayerConst(char8 modifier, uint64 size){
	if (modifier != 'O'){
		switch (modifier){
		case 'V':{
			if (typeDescriptor.dataIsConstant){
				modifier = 'v';
			}
			typeDescriptor.dataIsConstant = false;
		}break;
		case 'M':{
			if (typeDescriptor.dataIsConstant){
				modifier = 'm';
			}
			typeDescriptor.dataIsConstant = false;
		}break;
		case 'P':{
			if (typeDescriptor.dataIsConstant){
				modifier = 'p';
			}
			typeDescriptor.dataIsConstant = false;
		}break;
		default:{

		}
		}

		if (modifier == 'A'){
			uint32 sz = modifiers.GetSize();
			if (( sz > 0 ) && (modifiers.GetList()[sz-1] == 'P')){
				modifiers.GetList()[sz-1] = 'F';
			} else
			if (( sz > 0 ) && (modifiers.GetList()[sz-1] == 'p')){
				modifiers.GetList()[sz-1] = 'f';
			} else {
				modifiers().Append(modifier);
			}
		} else {
			modifiers().Append(modifier);
		}

		if (size > 0) modifiers().Append(size);
	}
}


VariableDescriptor::VariableDescriptor(){
    typeDescriptor = VoidType;
}

VariableDescriptor::~VariableDescriptor(){
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td, CCString modifierString){
    typeDescriptor = td;
    modifiers().Append(modifierString.GetList());
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers().Append(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers().Append(x.modifiers.GetList());
}

VariableDescriptor &VariableDescriptor::operator=(const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers().Truncate(0);
    modifiers().Append(x.modifiers.GetList());
    return *this;
}

ErrorManagement::ErrorType VariableDescriptor::GetVariableDimensions(
		const void *		varPtr,
		TypeDescriptor &	td,
		uint32 &			nOfDimensions,
		uint32 *			dimensionSizes) const {

	uint32 		maxDepth = nOfDimensions;
	CCString 	modifiersCopy = modifiers;

    ErrorManagement::ErrorType ret;
    // error if varPtr
//    ret.parametersError = (varPtr == NULL) && (nOfDimensions > 0);
//	REPORT_ERROR(ret,"varPtr is a NULL pointer");
//	if (ret){
//	    ret.parametersError = (dimensionSizes == NULL) && (nOfDimensions > 0);
//		REPORT_ERROR(ret,"sizes is a NULL pointer");
//	}

	// at least it is a simple variable...
	nOfDimensions = 0U;
	// unless it is not and will be updated
    td = GenericArray;
//TODO - assign type using VariableDescriptorLib::Type2TypeDescriptor()

    bool finished = false;
    while ((nOfDimensions <= maxDepth)&& (!finished) && ret){
    	uint32 		size = 0;
    	char8 		modifier  = 'O';
    	VariableDescriptorLib::GetLayerInfo(modifiersCopy,modifier,size );

   	   	switch (modifier){
    	case 'O':{
    		finished = true;
    		//depth = 0;
    		//sizes[] unmodified
    		td = this->typeDescriptor;
    	}break;
    	case 'P':
    	case 'p':{
   			finished = true;
   			td = GenericPointer;
    		//depth = 0;
    		//sizes[] unmodified
    	}break;
    	case 'M':
    	case 'm':{
    		//depth > 0;
    		//sizes[0:1] set to actual value
    		//sizes[>0] set to 0

    		// calculate depth only if this is not the last level
    		if ((nOfDimensions < maxDepth) && (dimensionSizes != NULL)) {
        		// only at first level we read the size from memory.
        		// this is because the size is unique only at this level
    			if ((nOfDimensions == 0U) && (varPtr != NULL)){
    				const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(varPtr);
    				dimensionSizes[nOfDimensions] = pm->GetNumberOfRows();
    	    		if (nOfDimensions < maxDepth){
    	    			nOfDimensions++;
    	    			dimensionSizes[nOfDimensions] = pm->GetNumberOfColumns();
    	    		}
    			} else {
    				dimensionSizes[nOfDimensions] = 0;
    	    		if (nOfDimensions < maxDepth){
    	    			nOfDimensions++;
    		    		// size = 0 means undetermined
    	    			dimensionSizes[nOfDimensions] = 0;
    	    		}
    			}
    		}
    	}break;
    	case 'V':
    	case 'v':{
    		//depth > 0;
    		//sizes[0] set to actual value
    		//sizes[>0] set to 0

    		// calculate depth only if this is not the last level
    		if ((nOfDimensions < maxDepth) && (dimensionSizes != NULL)) {
        		// only at first level we read the size from memory.
        		// this is because the size is unique only at this level
    			if ((nOfDimensions == 0) && (varPtr != NULL)){
    				const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(varPtr);
    				dimensionSizes[nOfDimensions] = pv->GetNumberOfElements();
    			} else {
    	    		// size = 0 means undetermined
    				dimensionSizes[nOfDimensions] = 0;
    			}
    		}
    	}break;
    	case 'f':
    	case 'F':
    	case 'A':{
    		//depth > 0;
    		//sizes[*] set to actual value
    		// calculate depth only if this is not the last level
    		if ((nOfDimensions < maxDepth) && (dimensionSizes != NULL)) {
    			dimensionSizes[nOfDimensions] = size;
    		}
    	}break;
    	case 'd':
    	case 'D':
    	case 'z':
    	case 'Z':
    	case 's':
    	case 'S':{
    		//depth > 0;
    		//sizes[0] set to actual value
    		//sizes[>0] set to 0

    		// calculate depth only if this is not the last level
    		if ((nOfDimensions < maxDepth) && (dimensionSizes != NULL)) {
    			if ((nOfDimensions == 0) && (varPtr != NULL)){
            		// only at first level we read the size from memory.
            		// this is because the size is unique only at this level
        			const uint8 *ptr = static_cast<const uint8 *>(varPtr);
    				ret = VariableDescriptorLib::RedirectP(ptr);
    				REPORT_ERROR(ret," redirection failed");

    				uint32 layerSize = 0;
    				if (ret){
    					VariableDescriptorLib::DimensionSize ds = VariableDescriptorLib::LayerSize(modifiersCopy,td);
    					ret.internalStateError = ds.ToNumber(layerSize);
    					REPORT_ERROR(ret,"layer size error");
    				}

    				if (ret){
    					dimensionSizes[nOfDimensions] = ZeroTerminatedArrayStaticTools::ZTAGetSize(ptr, layerSize);
    				}
    			} else {
    	    		// size = 0 means undetermined
    				dimensionSizes[nOfDimensions] = 0;
    			}
			}
    	}break;

    	default:{
    		td = InvalidType(0);
    		ret = ErrorManagement::FatalError;
    		COMPOSITE_REPORT_ERROR(ret, "Incorrect modifier: ",modifier);
    	}
    	}
   	   	nOfDimensions++;
    } // while

    // decrement as nOfDimensions will be incremented once too often
    nOfDimensions--;


    return ret;
}



/**************************************************************************************************************************/
/*                   CopyTo 																							  */
/*                   Uses VariableDescriptorLib::Variable                                             					  */
/*  			     uses UpdatePointerAndSize                                                                            */
/*  			     uses HasSameDimensionsAs                                                                             */
/**************************************************************************************************************************/


ErrorManagement::ErrorType VariableDescriptor::CopyTo(
		const uint8 *sourcePtr,
			  uint8 *destPtr,
			  const VariableDescriptor &destVd,
			  bool isCompare
		) const {

	ErrorManagement::ErrorType ret;

	const TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);
	VariableDescriptorLib::Variable sourceHandler(this->modifiers,this->typeDescriptor);
	VariableDescriptorLib::Variable destHandler(destVd.modifiers,destVd.typeDescriptor);
//	uint32 nDim = sourceHandler.NDimensions();

	ret.parametersError = ((destPtr == NULL) || (sourcePtr == NULL));
	REPORT_ERROR(ret, "NULL inputs");

	if (ret){
		tco = TypeConversionManager::GetOperator(destVd.typeDescriptor,this->typeDescriptor,isCompare);
    	ret.unsupportedFeature = ( tco == NULL_PTR(TypeConversionOperatorI *));
    	REPORT_ERROR(ret, "Conversion Operator not found");
	}

	if (ret){

    	ret = VariableDescriptorLib::CopyToRecursive(&sourceHandler[0],sourcePtr,&destHandler[0],destPtr,*tco,!isCompare);
    	REPORT_ERROR(ret, "CopyToRecursive failed");
	    if (tco != NULL_PTR(TypeConversionOperatorI *)){
	    	delete tco;
	    }

	}



    return ret;
}

/*****************************************************************************************************************************/
/*  			GetSize                                                                                                      */
/*  			uses VariableDescriptorLib::Variable                                                                         */
/*  			uses UpdatePointerAndSize                                                                                    */
/*****************************************************************************************************************************/


ErrorManagement::ErrorType VariableDescriptor::GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *overHeadSize) const{
	ErrorManagement::ErrorType ret;
	uint64 overHeadSz = 0;

	VariableDescriptorLib::Variable handler(this->modifiers,this->typeDescriptor);

	const VariableDescriptorLib::Dimension *start = &handler[0];
	ret = VariableDescriptorLib::GetSizeRecursive(start,pointer,dataSize,overHeadSz);
	REPORT_ERROR(ret,"GetSizeRecursive failed");

	if (overHeadSize != NULL){
		*overHeadSize = overHeadSz;
	}
	return ret;
}

/******************************************************************************************************/
/*          Clone algorithm                                                                           */
/*          Uses VariableDescriptorLib::Variable                                                      */
/******************************************************************************************************/

// simple algorithm
// without optimisation
ErrorManagement::ErrorType VariableDescriptor::Clone(
		const uint8 *				pointer,
		Reference &					reference
		) const{
	ErrorManagement::ErrorType ret;

	// analyse VariableDescriptor
	// if it can be copied directly without pointers...
	VariableDescriptorLib::Variable variables(modifiers,typeDescriptor);
	const VariableDescriptorLib::Dimension *dimensions = &variables[0];

	// a composition of strings
	bool isString = typeDescriptor.IsCharString();

	// means that the data at the end has a fixed size
	bool isFixedData = (typeDescriptor.IsStructuredData() || typeDescriptor.IsBasicType());

	// means that the variable data can be accessed directly without redirection
	// this means simple arrays.
//	bool hasOneLayer = (handler[0].endType == 'O');
	bool hasOneStack = dimensions->GetNextBreak()->IsFinal();

	if (!isString && !isFixedData){
		ret.unsupportedFeature = true;
		REPORT_ERROR(ret, "Unsupported type");
	}

	if (ret){
		// simple multidimensional array of fixed size stuff
		if ((hasOneStack) && (!isString)){
			uint32 sizeToCopy = 0;
			VariableDescriptorLib::DimensionSize ds = dimensions->GetLayerCombinedElementSize();

			ret = ds.ToNumber(sizeToCopy);
			REPORT_ERROR(ret, "Size of object > uint32 or undefined");
			if (ret){
				reference = AnyObject::Clone(sizeToCopy,reinterpret_cast<const void *>(pointer),*this);
			}

		// variable size - memory will be allocated progressively
		} else {
			MemoryPageFile pageFile(defaultAllocationSize);

			// prepare the worker
			VariableDescriptorLib::VariableCloner cloner(variables,pageFile);

			// force allocation of first page
			ret = pageFile.CheckAndNewPage();
			REPORT_ERROR(ret, "CheckAndNewPage(1) failed");

			// copy and create indexes
			if (ret){
				// move it down one level
				uint8 *addressOfOutput = NULL;
				VariableDescriptorLib::DimensionSize ds(1);

				// all the work is done here / recursive function
				ret = cloner.DoCreateR(0,pointer,addressOfOutput,ds);
				REPORT_ERROR(ret,"cloner.DoCreateR() failed ");
			}

			// free unused memory
			if (ret){
				ret = cloner.pageFile.CheckAndTrimPage(0);
				REPORT_ERROR(ret,"cloner.pageFile.CheckAndTrimPage() failed ");
			}

			// get start pointer
			uint8 *dataPtr = NULL;
			if (ret){
				dataPtr = cloner.pageFile.Address(0);
			}

			if (ret){
				DynamicCString mods;

				cloner.GetOutputModifiers(mods);
//				TypeDescriptor type = cloner.handler.GetTypeDescriptor();
				TypeDescriptor type = typeDescriptor;

				ReferenceT<MemoryPageObject> mpor;
				mpor = ReferenceT<MemoryPageObject> (HeapManager::standardHeapId);
				ret.outOfMemory = (!mpor.IsValid());
				REPORT_ERROR(ret,"MemoryPageObject construction failed");

				if (ret){
					mpor->Setup(type,mods,dataPtr,cloner.pageFile);
					reference = mpor;
				}
			}
		}
	}

	return ret;
}


/**************************************************************************************************/
/*      ToString                                                                                  */
/*      uses GetLayerInfo                                                                         */
/**************************************************************************************************/


ErrorManagement::ErrorType VariableDescriptor::ToString(CStringTool &string,bool rawFormat) const{
	ErrorManagement::ErrorType  ret;
	if (rawFormat){
		bool retbool = true;
		ErrorManagement::ErrorType ret = string.Append(modifiers).Append(' ');
		retbool = ret.ErrorsCleared();
		retbool = retbool && typeDescriptor.ToString(string);
		ret.fatalError = !retbool;
	} else {
		int8 priority=0;
	    ret = VariableDescriptorLib::ToString(modifiers,typeDescriptor,string,emptyString,true,priority) ;
	}
    return ret;
}


/***************************************************************************************************************/
/*               Redirect                                                                                      */
/***************************************************************************************************************/


/**
 * @brief removes one indirection layer and update variable pointer
 * @param[in out] pointer, the pointer to the variable
 * @param[in] index the offset
 * @return true if all ok or the error
 */
ErrorManagement::ErrorType VariableDescriptor::Redirect(const uint8 *&pointer,uint32 index,CCString modifierString){
	ErrorManagement::ErrorType ret;
	char8 modifier;
	uint32 size;
	if (modifierString.IsNullPtr()){
		modifierString = modifiers;
	}
	ret.parametersError = (pointer == NULL_PTR(const uint8 *));
    REPORT_ERROR(ret, "pointer = NULL");

	if (ret){
		VariableDescriptorLib::GetLayerInfo(modifierString,modifier,size);

		const CCString redirSet = "pPsSzZdDfF";
		if (redirSet.In(modifier)){
			const uint8 **pp = (const uint8 **)(pointer);
			const uint8 *p = *pp;
			ret.exception = !MemoryCheck::Check(p);
		    REPORT_ERROR(ret, "bad pointer");
		    if (ret){
				pointer = p;
		    }
		}
		if (ret){
	 	switch (modifier){
		case 'f':
	 	case 'F':{
			ret.outOfRange = (index >= size);
	        REPORT_ERROR(ret, "index >= size");

	 		// need fulllayerSize of the remaining full layer
	 		// note that next layer cannot be ZzDdSs but only A or a terminator like PpVvMm and 0
	 		if (ret){

				VariableDescriptorLib::DimensionSize layerSize = VariableDescriptorLib::LayerSize(modifierString,typeDescriptor);
				VariableDescriptorLib::DimensionSize step = layerSize * index;

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = pointer + step32;
					modifiers().Remove(static_cast<uint32>(static_cast<uint32>(modifierString.GetList()-modifiers.GetList())));
				}
	 		}
	 	}break;
	 	case 'p':
	 	case 'P':{
 			ret.outOfRange = (index != 0);
	        REPORT_ERROR(ret, "index!=0 for pointer to var");

			if (ret){
				modifiers().Remove(static_cast<uint32>(static_cast<uint32>(modifierString.GetList()-modifiers.GetList())));
			}
	 	}break; // end Pp case
	 	case 'A':{
			ret.outOfRange = (index >= size);
	        REPORT_ERROR(ret, "index >= size");

	 		// need fulllayerSize of the remaining full layer
	 		// note that next layer cannot be ZzDdSs but only A or a terminator like PpVvMm and 0
	 		if (ret){

				VariableDescriptorLib::DimensionSize layerSize = VariableDescriptorLib::LayerSize(modifierString,typeDescriptor);
				VariableDescriptorLib::DimensionSize step = layerSize * index;

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = pointer + step32;
					modifiers().Remove(static_cast<uint32>(modifierString.GetList()-modifiers.GetList()));
				}
			}
	 	}break;
	 	case 's':
	 	case 'S':
	 	case 'd':
	 	case 'D':
	 	case 'z':
	 	case 'Z':{
	 		// this is not the full ZeroTermArray but just the zero term memory referenced to.
	 		// it was preceded by a P that has been skipped.

			VariableDescriptorLib::DimensionSize layerSize = VariableDescriptorLib::LayerSize(modifierString,typeDescriptor);
			uint32 layerSize32;
			ret = layerSize.ToNumber(layerSize32);
	        REPORT_ERROR(ret, "layerSize >= 32bit");

	        uint32 maxIndex  = 0;
	        if (ret){
	        	maxIndex  = ZeroTerminatedArrayTool::ZTAGetSize(pointer, layerSize32);
	        	ret.outOfRange = (index >= maxIndex);
	        	REPORT_ERROR(ret, "index >= maxIndex");
	 		}

			if (ret){
				VariableDescriptorLib::DimensionSize step = layerSize * index;
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");
//REPORT_ERROR(ErrorManagement::Debug, "LayerSize = ",layerSize.GetData());
//REPORT_ERROR(ErrorManagement::Debug, "Step = ",step.GetData(),"Step32 = ",step32,"Index = ",index);

		        if (ret){
			        pointer = pointer + step32;
					modifiers().Remove(static_cast<uint32>(static_cast<uint32>(modifierString.GetList()-modifiers.GetList())));
		        }
			}

	 	}break;
	 	case 'v':
	 	case 'V':{
			const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);
			const uint8 *p = static_cast<const uint8 *>(pv[0].GetDataPointer());
			uint32 numberOfArrayElements = pv[0].GetNumberOfElements();

			ret.exception = !MemoryCheck::Check(p);
		    REPORT_ERROR(ret, "bad pointer");

			if (ret){
				ret.outOfRange = (index >=  numberOfArrayElements);
				REPORT_ERROR(ret, "index >= numberOfArrayElements");
			}

			if (ret){

				VariableDescriptorLib::DimensionSize layerSize = VariableDescriptorLib::LayerSize(modifierString,typeDescriptor);
//REPORT_ERROR(ErrorManagement::Debug, "LayerSize = ",layerSize.GetData());
				VariableDescriptorLib::DimensionSize step = layerSize * index;
//REPORT_ERROR(ErrorManagement::Debug, "Step = ",step.GetData(),"Index = ",index);

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = p;
					pointer = pointer + step32;
					modifiers().Remove(static_cast<uint32>(modifierString.GetList()-modifiers.GetList()));
				}
			}
	 	}break;
	 	case 'm':
	 	case 'M':{
			const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);
			const uint8 *p = static_cast<const uint8 *>(pm[0].GetDataPointer());
			ret.exception = !MemoryCheck::Check(p);
			REPORT_ERROR(ret, "bad pointer");

			if (ret){
				ret.outOfRange = (index >=  pm[0].GetNumberOfRows());
				REPORT_ERROR(ret, "index >= pm[0].GetNumberOfRows()");
			}

			if (ret){
				VariableDescriptorLib::DimensionSize layerSize = VariableDescriptorLib::LayerSize(modifierString,typeDescriptor);
				VariableDescriptorLib::DimensionSize step = layerSize * index;
				step = step * pm[0].GetNumberOfColumns();

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = p;
					pointer = pointer + step32;
					modifiers().Remove(static_cast<uint32>(modifierString.GetList()-modifiers.GetList()));
				}

				// add missing dimension
				DynamicCString modifiersTemp;
				modifiersTemp().Append(modifierString);
				modifiers().Truncate(0);
				modifiers().Append('A');
				modifiers().Append(pm[0].GetNumberOfColumns());
				modifiers().Append(modifiersTemp);
			}

	 	}break;
	 	case 'O':{
	 		if (typeDescriptor.IsCharStreamType()){
	 			TD_FullType fullType = static_cast<TD_FullType>(typeDescriptor.fullType);
	 			switch (fullType){
	 			case TDF_DynamicCString:
	 			case TDF_CString:
	 			case TDF_CCString:{
	 				CCString *string = reinterpret_cast<CCString *>(const_cast<uint8 *>(pointer));
	 				uint32 size = string->GetSize();
	 				ret.outOfRange = (size <= index);
 			        REPORT_ERROR(ret, "index >= string size");
	 				if (ret){
	 					pointer = reinterpret_cast<const uint8 *>(string->GetList()+index);
		 				if (fullType == TDF_CCString){
			 				typeDescriptor = ConstCharacter8Bit;
		 				} else {
			 				typeDescriptor = Character8Bit;
		 				}
	 				}

	 			} break;
	 			default :{
			 		ret.illegalOperation = true;
			        REPORT_ERROR(ret, "can only redirect Zero Term Char streams");
	 			} break;
	 			}
	 		} else {
		 		ret.illegalOperation = true;
		        REPORT_ERROR(ret, "cannot redirect a basic type");
	 		}
	 	} break;
	 	default:{
	 		ret.internalSetupError = true;
	        REPORT_ERROR(ret, "unmapped modifier");
	 	}
	 	}// end case
		}// end if
	}

	return ret;
}

}
