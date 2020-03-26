/**
 * @file VariableDescriptorLib.cpp
 * @brief Header file for class AnyType
 * @date 3 Oct 2018
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

#include "CompositeErrorManagement.h"
#include "Private/VariableDescriptorLib.h"
#include "MemoryCheck.h"
#include "TypeConversionOperatorI.h"
#include "Matrix.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe{
namespace VariableDescriptorLib{

static inline bool IsNumber(char8 c);
static inline uint32 ToNumber(char8 c);
static inline bool IsUpperCase(char8 c);
static inline uint32 ReadNumber(CCString &buffer);

bool IsNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

uint32 ToNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

bool IsUpperCase(char8 c){
	bool ret = true;
	if ((c >= 'A')  && (c <= 'Z')) {
		ret = false;
	}
	return ret;
}
uint32 ReadNumber(CCString &buffer){
	uint32 result = 0;
	while (IsNumber(buffer[0])){
		result = result * 10u;
		result += ToNumber(buffer[0]);
		buffer++;
	}
	return result;
}

void GetLayerInfo(CCString &modifierString,char8 &modifier,uint32 &size ){
	if (modifierString.IsNullPtr()){
		modifier = 'O';
		size = 0;
	} else {
		modifier = modifierString[0];
		if (modifier == '\0'){
			size = 0;
			modifier = 'O';
		} else {
			modifierString++;
			size = ReadNumber(modifierString);
		}
	}
}


TypeDescriptor Type2TypeDescriptor(char8 c, const TypeDescriptor &tdIn){
	TypeDescriptor td = GenericArray;

   	switch (c){
	case 'O':{
		td = tdIn;
	}break;
	case 'P':
	case 'p':{
		td = GenericPointer;
	}break;
	case 'M':
	case 'm':
	case 'V':
	case 'v':
	case 'f':
	case 'F':
	case 'A':
	case 'd':
	case 'D':
	case 'z':
	case 'Z':
	case 's':
	case 'S':{
	}break;

	default:{
		td = InvalidType(0);
	}
	}
   	return td;
}

uint32 Type2Size(char8 c,const TypeDescriptor &tdIn) {
	const CCString matrixS = "Mm";
	const CCString vectorS = "Vv";
	const CCString pointerS = "FfSDZPsdzp";
	uint32 size = 0;
	if (pointerS.In(c)){
		size = sizeof(void *);
	} else
	if (matrixS.In(c)){
		size = sizeof(Matrix<char8>);
	} else
	if (vectorS.In(c)){
		size = sizeof(Vector<char8>);
	} else {
		size = tdIn.StorageSize();
	}
	return size;
}


VariableDescriptorLib::DimensionSize LayerSize(CCString modifierString,TypeDescriptor td){
	char8 type;
	uint32 size;

	VariableDescriptorLib::DimensionSize ds(1);
	VariableDescriptorLib::GetLayerInfo(modifierString,type,size);
	while (type == 'A'){
		ds = ds * size;
		VariableDescriptorLib::GetLayerInfo(modifierString,type,size);
	}

	/**
	 * note that PS PZ .... have the same size as P so we do not need to distinguish them from P
	 */
	ds = ds * VariableDescriptorLib::Type2Size(type,td);
	return ds;
}

ErrorManagement::ErrorType RedirectP(const uint8* &ptr,bool allowNULL){
	ErrorManagement::ErrorType ret;
	const uint8 **pp = (const uint8 **)(ptr);
	const uint8 *p = *pp;
	if ((p == NULL) && (allowNULL)){
		ptr = p;
	} else
	if ((p == NULL) || (!MemoryCheck::Check(p))){
		ret.exception = true;
		DynamicCString errM;
		errM().Append("bad pointer (").AppendHex(reinterpret_cast<uint64>(p)).Append(") at (").AppendHex(reinterpret_cast<uint64>(pp)).Append(')');
        COMPOSITE_REPORT_ERROR(ret, errM.GetList());
	} else {
		ptr = p;
	}
	return ret;
}



static ErrorManagement::ErrorType DimensionJoin(
							const Dimension *&		dim,
							const uint8 *&			ptr,
							uint32 &				numberOfColumns){
	ErrorManagement::ErrorType ret;

	const Dimension *joinedDimension = dim->Next();
	ret.unsupportedFeature = (joinedDimension == NULL_PTR(Dimension *));
	REPORT_ERROR(ret, "no more destinations available to join");

	if (ret){
		ret.unsupportedFeature = (joinedDimension->IsBreak() || joinedDimension->NOfDimensions()>1);
		COMPOSITE_REPORT_ERROR(ret, "Cannot join a dimension of type ",joinedDimension->TypeChar());
	}

	uint32 numberOfRows;
	if (ret){
		ret = joinedDimension->UpdatePointerAndSizeEx((const uint8 *&)ptr,numberOfColumns,numberOfRows);
		REPORT_ERROR(ret, "destination pointer handling failed");
	}
	if (ret){
		dim = joinedDimension;
	}
	return ret;
}

ErrorManagement::ErrorType VectorCopy(
		Dimension *						sourceDimensions,
		const uint8* 					sourcePtr,
		Dimension *						destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		){
	ErrorManagement::ErrorType ret = sourceDimensions->NOfDimensions()==1;


	// must redirect pointer	uint32 sourceNumberOfColumns = 1;
	uint32 sourceNumberOfRows = 1;
	uint32 sourceNumberOfColumns = 1;

	if (ret){
		ret = sourceDimensions->UpdatePointerAndSizeEx(sourcePtr,sourceNumberOfColumns,sourceNumberOfRows);
	    REPORT_ERROR(ret, "source pointer handling failed");
	}

	if (ret){
		destDimensions->ReSize(destPtr,sourceNumberOfRows,sourceNumberOfColumns);
	}

	if (ret){
		ret = destDimensions->UpdatePointerAndSizeEx((const uint8 *&)destPtr,sourceNumberOfColumns,sourceNumberOfRows);
	    REPORT_ERROR(ret, "source pointer handling failed");
	}

	if (ret){
		ret = op.Convert(destPtr,sourcePtr,sourceNumberOfColumns);
	    REPORT_ERROR(ret, "convert failed");
	}

	return ret;
}

ErrorManagement::ErrorType MatrixCopy(
		uint32 							level,
		const Dimension *				sourceDimensions,
		const uint8* 					sourcePtr,
		const Dimension *				destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		){
	ErrorManagement::ErrorType ret;

	uint32 sourceNumberOfColumns = 1;
	uint32 sourceNumberOfRows = 1;

	if (ret){
		ret = sourceDimensions->UpdatePointerAndSizeEx(sourcePtr,sourceNumberOfColumns,sourceNumberOfRows);
	    REPORT_ERROR(ret, "source pointer handling failed");
	}

	if (ret && sourceDimensions->NOfDimensions()<2){
		sourceNumberOfRows = sourceNumberOfColumns;

		ret = DimensionJoin(sourceDimensions,sourcePtr,sourceNumberOfColumns);
	    REPORT_ERROR(ret, "Dimension Join failed");
	}

	if (ret){
		destDimensions->ReSize(destPtr,sourceNumberOfRows,sourceNumberOfColumns);
	}

	if (ret){
		ret = destDimensions->UpdatePointerAndSizeEx((const uint8 *&)destPtr,sourceNumberOfColumns,sourceNumberOfRows);
	    REPORT_ERROR(ret, "source pointer handling failed");
	}

	if (ret){
		ret = op.Convert(destPtr,sourcePtr,sourceNumberOfColumns*sourceNumberOfRows);
	    REPORT_ERROR(ret, "convert failed");
	}

	return ret;
}

ErrorManagement::ErrorType CopyToRecursive(
		const Dimension *				 sourceDimensions,
		const uint8* 					 sourcePtr,
		const Dimension *				 destDimensions,
		uint8* 							 destPtr,
		const TypeConversionOperatorI &	 op,
		bool 							 isCopy
		){
	ErrorManagement::ErrorType ret;

	if (ret){
		ret.fatalError = (sourceDimensions == NULL_PTR(Dimension *));
		REPORT_ERROR(ret, "sourceDimensions = NULL");
	}

	if (ret){
		ret.fatalError = (destDimensions == NULL_PTR(Dimension *));
		REPORT_ERROR(ret, "destDimensions = NULL");
	}

	uint32 slnd = 0;
	uint32 dlnd = 0;
	if (ret){
		slnd = sourceDimensions->NOfDimensions();
		dlnd = destDimensions->NOfDimensions();
	}

	// unused here
//	uint32 overHead;

/**
 * PROCESS SOURCE DIMENSIONS
 */

	uint32 sourceNumberOfColumns = 1;
	uint32 sourceNumberOfRows = 1;

	if (ret){
		ret = sourceDimensions->UpdatePointerAndSizeEx(sourcePtr,sourceNumberOfColumns,sourceNumberOfRows);
	    REPORT_ERROR(ret, "source pointer handling failed");
	}

	/* will only succeed if next dimension is 'A' */
	if (ret && (dlnd > slnd)){
//		sourceNumberOfRows = sourceNumberOfColumns;
		ret =  DimensionJoin(sourceDimensions,sourcePtr,sourceNumberOfRows);
	    REPORT_ERROR(ret, "Dimension Join failed");
	}

	/** total n of elements */
	uint32 numberOfElements = sourceNumberOfColumns * sourceNumberOfRows;

/**
* * PROCESS DESTINATION DIMENSIONS
*/

	uint32 destNumberOfColumns = 1;
	uint32 destNumberOfRows = 1;
	uint8 *redirectedDestPtr = destPtr;
	if (ret){
		ret = destDimensions->UpdatePointerAndSizeEx((const uint8 *&)redirectedDestPtr,destNumberOfColumns,destNumberOfRows);
		REPORT_ERROR(ret, "destination pointer handling failed");
	}

	/* will only succeed if next dimension is 'A' */
	if (ret && (slnd > dlnd)){
//		destNumberOfRows = destNumberOfColumns;
		ret =  DimensionJoin(destDimensions,(const uint8 *&)redirectedDestPtr,destNumberOfRows);
	    REPORT_ERROR(ret, "Dimension Join failed");
	}

/**
* * CHECK COMPATIBILITY
*/
	if (ret){
		bool sizeMismatch = (destNumberOfColumns != sourceNumberOfColumns)  || (destNumberOfRows != sourceNumberOfRows);

		/*
		 * in case of copy, if size is mismatched, and if destination is monolithic, dynamic and not constant
		 * try to resize destination
		 */
		if (sizeMismatch && isCopy && (dlnd >= slnd) && destDimensions->IsDynamic()){
			redirectedDestPtr = destPtr;
			ret = destDimensions->ReSize(redirectedDestPtr,sourceNumberOfColumns,sourceNumberOfRows);
		    REPORT_ERROR(ret, "Destination Dimension Resize failed");

		    if (ret){
		    	sizeMismatch = false;
		    }
		}

		ret.unsupportedFeature = sizeMismatch;
		COMPOSITE_REPORT_ERROR(ret, "mismatch in dimensions sizes D(",destNumberOfColumns,",",destNumberOfRows,")!=S(",sourceNumberOfColumns,",",sourceNumberOfRows,")");
	}



/**
* * FINISH = COPY or LOOP RECURSIVELY
*/

	if (ret){

		// the last dimension is always the scalar typed
		if (destDimensions->IsFinalLayer() && sourceDimensions->IsFinalLayer()){
			ret = op.Convert(redirectedDestPtr,sourcePtr,numberOfElements);
	        REPORT_ERROR(ret, "conversion failed");

		} else {
			uint32 sourceElementSize;
			ret = sourceDimensions->GetNextLayerElementSize(sourceElementSize);
	        REPORT_ERROR(ret, "source dimensions overflow");

			uint32 destElementSize = 0U;
	        if (ret){
	        	ret = destDimensions->GetNextLayerElementSize(destElementSize);
		        REPORT_ERROR(ret, "dest dimensions overflow");
	        }

			// skip forward
			uint32 ix = 0;
			for (ix = 0; (ix < numberOfElements) && ret; ix++){
				ret = CopyToRecursive(sourceDimensions->Next(),sourcePtr,destDimensions->Next(),redirectedDestPtr,op,isCopy);

				sourcePtr+= sourceElementSize;
				redirectedDestPtr += destElementSize;
				COMPOSITE_REPORT_ERROR(ret,"Failed at row (",ix,")");
			}
		}
	}
	return ret;
}



/**
 * Operates recursively
 */
ErrorManagement::ErrorType GetSizeRecursive(
		const Dimension					*dimension,
		const uint8* 					pointer,
		uint64 							&dataSize,
		uint64 							&auxSize
		){
	ErrorManagement::ErrorType ret;

	DimensionSize auxSizeD(0);
	DimensionSize dataSizeD(0);
	uint32 multiplier = 1;
	DimensionSize multiplierD(1);

	if (!dimension->IsFinal()){
		uint32 numberOfColumns = 1;
		uint32 numberOfRows = 1;

		ret = dimension->UpdatePointerAndSizeEx(pointer,numberOfRows,numberOfColumns);
		REPORT_ERROR(ret,"UpdatePointerAndSize failed");

		if (ret){
			multiplierD = multiplierD * numberOfRows;
			multiplierD = multiplierD * numberOfColumns;

			auxSizeD = auxSizeD + dimension->GetOverHead();
			dataSizeD = dataSizeD + dimension->GetOverHead();

			const Dimension	*nextDimension = dimension->Next();
			ret.fatalError = (nextDimension == NULL_PTR(Dimension *));
			COMPOSITE_REPORT_ERROR(ret, "next dimension to a non Break dimension: ",dimension->TypeChar()," is NULL");

			if (ret){
				dimension = nextDimension;

				// Join all non-break dimensions (Ann) they have no overhead
				while ((!dimension->IsBreak())&& (ret)){

					ret = dimension->UpdatePointerAndSizeEx(pointer,numberOfRows,numberOfColumns);
					REPORT_ERROR(ret,"UpdatePointerAndSize failed");

					if (ret){
						multiplierD = multiplierD * numberOfRows;
						multiplierD = multiplierD * numberOfColumns;

						nextDimension = dimension->Next();
						ret.fatalError = (nextDimension == NULL_PTR(Dimension *));
						REPORT_ERROR(ret, "next dimension to a non Break dimension is NULL");
					}
					dimension = nextDimension;

				}

			}
		}

		if (ret){
			ret = multiplierD.ToNumber(multiplier);
			REPORT_ERROR(ret,"too many elements Multiplier Overflow!");
		}
	}


	if (ret){
		uint32 storageSize = dimension->TypeSize();
		// 'O'
		if (dimension->IsFinal()){
			TypeDescriptor td = dimension->GetTypeDescriptor();

			// calculate the size of the pointer storage for this type
			DimensionSize  fullStorageSizeD(storageSize);
			fullStorageSizeD = fullStorageSizeD * multiplierD;
			uint32 fullStorageSize;
			ret = fullStorageSizeD.ToNumber(fullStorageSize);
			REPORT_ERROR(ret,"too many elements StorageSize Overflow!");

			if (ret){
				bool hasVariableSize = td.IsCharStreamType();
				if (hasVariableSize){

					auxSizeD = auxSizeD + fullStorageSizeD;

					// now calculate the size of the actual payload
					for (uint32 i = 0;i < multiplier;i++){
						dataSizeD = dataSizeD + td.FullSize(pointer);
						pointer += storageSize;
					}

				} else {
					dataSizeD = dataSizeD + fullStorageSizeD;
				}
			}
		} else
			//non final break dimension
		{
//			auxSizeD = auxSizeD + dimension->GetOverHead() * multiplier;
//			dataSizeD = dataSizeD + dimension->GetOverHead() * multiplier;

			for (uint64 i = 0;(i < multiplier) && ret;i++){
				uint64 dataSize2;
				uint64 auxSize2;
				ret = GetSizeRecursive(dimension,pointer,dataSize2,auxSize2);
				dataSizeD = dataSizeD + dataSize2;
				auxSizeD  = auxSizeD + auxSize2;
				pointer 			+= storageSize;
			}
		}

		if (ret){
			ret = dataSizeD.ToNumber(dataSize);
			REPORT_ERROR(ret,"too many elements DataSize Overflow!");
		}

		if (ret){
			ret = auxSizeD.ToNumber(auxSize);
			REPORT_ERROR(ret,"too many elements AuxSize Overflow!");
		}

	}
	return ret;
}

/**
 * This function does a double recursion in order to process the pre-pending(*) and post-pending([]) modifiers
 */
ErrorManagement::ErrorType ToString(CCString modifiers,const TypeDescriptor &typeDescriptor,CStringTool &string,CCString modifierString,bool start,int8 &priority){
	ErrorManagement::ErrorType ret;

	if (modifierString.IsNullPtr()){
		modifierString = modifiers;
	}
	char8 modifier;
	uint32 size;
	VariableDescriptorLib::GetLayerInfo(modifierString,modifier,size );


	// go ahead and process in reverse pointers and arrays
	if ((modifier=='A') || (modifier=='P') || (modifier=='p') || (modifier=='F') || (modifier=='f')){
		ret = ToString(modifiers,typeDescriptor,string,modifierString,false,priority);

		// process A and P in reverse
		if (ret){
			if (modifier == 'P'){
				string.Append(" *");
			} else
			if (modifier == 'F'){
				string.Append("( *");
			} else
			if (modifier == 'f'){
				string.Append("( * const");
			}

			// if this was the start of the sequence now do the forward section to add the vectors[]
			if (start){
				priority = 1;

				bool end = false;
				while (!end  && ret){

					switch (modifier){
					case 'f':
					case 'F':{
						string.Append(')').Append('[').Append(size).Append(']');
					}break;
					case 'A':{
						string.Append('[').Append(size).Append(']');
					}break;
					case 'p':
					case 'P':{
					}break;
					case 'O':
					case 'm':
					case 'M':
					case 'v':
					case 'V':
					case 'd':
					case 'D':
					case 's':
					case 'S':
					case 'z':
					case 'Z':{
						end = true;
					}break;
					default:{
						ret.fatalError = true;
					}
					}
					if (!end && ret){
						VariableDescriptorLib::GetLayerInfo(modifierString,modifier,size );
					}
				}
			}
		}
	} else {
		// when encountering 0 or a template MVZSDmvzsd start new sequence for goes inside the template
		if (modifier == 'O'){
			typeDescriptor.ToString(string);
		} else {
			CCString templateName;
			bool hasSize=false;
			switch(modifier){
			case 'M':{
				templateName = "Matrix<";
			} break;
			case 'V':{
				templateName = "Vector<";
			} break;
			case 'v':{
				templateName = "const Vector<";
			} break;
			case 'm':{
				templateName = "const Matrix<";
			} break;
			case 'Z':{
				templateName = "ZeroTerminatedArray<";
			} break;
			case 'z':{
				templateName = "const ZeroTerminatedArray<";
			} break;
			case 'D':{
				templateName = "DynamicZeroTerminatedArray<";
			} break;
			case 'd':{
				templateName = "const DynamicZeroTerminatedArray<";
			} break;
			case 'S':{
				templateName = "StaticZeroTerminatedArray<";
				hasSize = true;
			} break;
			case 's':{
				templateName = "const StaticZeroTerminatedArray<";
				hasSize = true;
			} break;
			default:{
				ret.unsupportedFeature=true;
			}
			}

			if (ret){
				string.Append(templateName);
				// insert the type of what follows
				int8 localPriority=0;
				ret = ToString(modifiers,typeDescriptor,string,modifierString,true,localPriority);
				if (hasSize){
					string.Append(',').Append(size);
				}
				// close the template
				string.Append('>');
			}
		}
	} // end of reverse action

	return ret;
}



} //VariableDescriptorLib
} //MARTe
