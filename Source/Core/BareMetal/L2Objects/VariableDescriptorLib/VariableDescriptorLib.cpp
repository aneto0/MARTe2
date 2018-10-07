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
#include "VariableDescriptorLib.h"
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
		modifier = '\0';
		size = 0;
	} else {
		modifier = modifierString[0];
		if (modifier == '\0'){
			size = 0;
		} else {
			modifierString++;
			size = ReadNumber(modifierString);
		}
	}
}

uint32 VariableDescriptorLib::Type2Size(char8 c,const TypeDescriptor &tdIn) {
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
		errM.Append("bad pointer (");
		errM.AppendHex(reinterpret_cast<uint64>(p));
		errM.Append(") at (");
		errM.AppendHex(reinterpret_cast<uint64>(pp));
		errM.Append(')');
        REPORT_ERROR(ret, errM.GetList());
	} else {
		ptr = p;
	}
	return ret;
}

static ErrorManagement::ErrorType UpdatePointerAndSize(
			VariableDescriptorLib::Variable &dh,
			uint32 			layerIndex,
			const uint8 *&	ptr,
			uint32 &		numberOfElementsIO,
			uint32 &		nextElementSize,
			uint32 &		overHead)  {
	ErrorManagement::ErrorType ok;

	const VariableDescriptorLib::Dimension &diNext = dh[layerIndex+1];
	VariableDescriptorLib::Dimension &di = dh.Access(layerIndex);

	uint8 type = di.type;
	bool isZta = false;
	bool allowNULL = false;
	bool updateNumberOfElements = false;
	overHead = 0;

	VariableDescriptorLib::DimensionSize numberOfElements(0);
	VariableDescriptorLib::DimensionSize nextElementSizeD(0);

	nextElementSizeD = diNext.elementSize;

	switch (type){

	case '\0':{
		numberOfElements = 1;
		type = 'A';
		nextElementSizeD = di.elementSize;
	} break;
	case 'x': // F or f with NULLs - created at this level
	case 'f':
	case 'F':{
		overHead = sizeof (void *);
		numberOfElements = di.numberOfElements;
		allowNULL = true;
	} break;
	case 'A':{
		numberOfElements = di.numberOfElements;
	}break;
	case 'd':
	case 's':
	case 'z':
	case 'D':
	case 'S':
	case 'Z':{
		isZta = true;
		updateNumberOfElements = true;
		overHead = sizeof (void *) + diNext.elementSize.GetData();
	}break;
	case 'v':
	case 'V':{
		overHead = sizeof (Vector<char8>);
		const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
		numberOfElements = pv->GetNumberOfElements();
		if (numberOfElements == VariableDescriptorLib::DimensionSize(0U)){
			allowNULL = true;
		}
		updateNumberOfElements = true;
	}break;
	case 'm':
	case 'M':{
		overHead = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
		numberOfElements = pm->GetNumberOfElements();
		if (numberOfElements == VariableDescriptorLib::DimensionSize(0U)){
			allowNULL = true;
		}
		updateNumberOfElements = true;
	}break;
	default:{
		ok.fatalError = true;
        REPORT_ERROR(ok, "Default case reached");
	}break;

	}

	if ((type != 'A') && ok){
		// it works as vector is descendant of Pointer class
		ok = VariableDescriptorLib::RedirectP(ptr,allowNULL);
		COMPOSITE_REPORT_ERROR(ok, "type ",type," redirection failed ");

        // handle case of null pointers
		// change F to X type
		if (ptr == NULL) {
			numberOfElements = 0;
			if ((type == 'f') || (type == 'F')){
				di.type = 'x';
			}
		}
	}

	// calculate actual size of each ZTA
	if (isZta && ok){
		uint32 n;
		ok = diNext.elementSize.ToNumber(n);
	    REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfElements = ZeroTerminatedArrayGetSize(ptr, n);
		}
	}

	if (ok){
		ok = numberOfElements.ToNumber(numberOfElementsIO);
	    REPORT_ERROR(ok, "numberOfElements is infinite/indefinite");
	}

	if (ok){
		ok = nextElementSizeD.ToNumber(nextElementSize);
	    REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}

	return ok;
}

ErrorManagement::ErrorType HasSameDimensionsAs(const VariableDescriptorLib::Variable &first,const VariableDescriptorLib::Variable &second){
	ErrorManagement::ErrorType ok = true;
	uint32 nDim = first.NDimensions();
	uint32 nDimO = second.NDimensions();
	ok.internalSetupError=(nDim == 0);
	REPORT_ERROR(ok, "NDimensions == 0");

	if (ok){
		ok.invalidOperation=(nDim != nDimO);

		if (!ok){
			DynamicCString errM;
			errM.Append("N of dim[");
			for (int i = 0;i < nDim;i++){
				char8 type = first[i].type;
				if (type != '\0') errM.Append(type);
				else errM.Append('0');
			}
			errM.Append( "]= ");
			errM.Append(nDimO);
			errM.Append(" != [");
			for (int i = 0;i < nDim;i++){
				char8 type = second[i].type;
				if (type != '\0') errM.Append(type);
				else errM.Append('0');
			}
			errM.Append(']');
			errM.Append(nDimO);
	        REPORT_ERROR(ok, errM.GetList());
		}
	}

	for (int i = 0;ok && (i < nDim); i++){
		VariableDescriptorLib::DimensionSize d1 = first[i].numberOfElements;
		VariableDescriptorLib::DimensionSize d2 = second[i].numberOfElements;

		if (((d1 != d2) && ( !d1.isIndeterminate()) && ( !d2.isIndeterminate())) || ( d1.isPositiveInf())){
			ok.invalidOperation=true;
			COMPOSITE_REPORT_ERROR(ok,"dimension[",i,"] d1= ",d1.GetData()," d2= ",d2.GetData())
		}
	}

	return ok;
}

/**
 * Recursive Operation
 */
ErrorManagement::ErrorType CopyToRecursive(
		uint32 							level,
		VariableDescriptorLib::Variable &sourceDimensions,
		const uint8* 					sourcePtr,
		VariableDescriptorLib::Variable &destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		){
	ErrorManagement::ErrorType ret;

	uint32 sourceNumberOfElements = 1;
	uint32 sourceElementSize = 1;
	uint32 destNumberOfElements = 1;
	uint32 destElementSize = 1;
	uint32 overHead;

	ret = UpdatePointerAndSize(sourceDimensions,level,sourcePtr,sourceNumberOfElements,sourceElementSize,overHead);
    REPORT_ERROR(ret, "source pointer handling failed");

	if (ret){
		ret = UpdatePointerAndSize(destDimensions,level,(const uint8 *&)destPtr,destNumberOfElements,destElementSize,overHead);
		REPORT_ERROR(ret, "destination pointer handling failed");
	}

	if (ret){
		ret.unsupportedFeature = (sourceNumberOfElements != destNumberOfElements);
		REPORT_ERROR(ret, "mismatch in dimensions");
	}

	if (ret){
		// the last dimension is always the scalar typed
		if (destDimensions.NDimensions() <= (level+2)){
			ret = op.Convert(destPtr,sourcePtr,destNumberOfElements);
	        REPORT_ERROR(ret, "conversion failed");
		} else {
			// skip forward
			uint32 ix = 0;
			for (ix = 0; (ix < sourceNumberOfElements) && ret; ix++){
				ret = CopyToRecursive(level+1,sourceDimensions,sourcePtr,destDimensions,destPtr,op);
				sourcePtr+= sourceElementSize;
				destPtr+= destElementSize;
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
		uint32 							level,
		Variable 						&handler,
		const uint8* 					pointer,
		uint64 							&dataSize,
		uint64 							&auxSize
		){
	ErrorManagement::ErrorType ret;

	uint32 nextElSize;
	uint32 overHead;
	uint32 multiplier;
	ret = UpdatePointerAndSize(handler,level, pointer,multiplier,nextElSize,overHead);
	REPORT_ERROR(ret,"UpdatePointerAndSize failed");

	if (ret){
	 	dataSize 			= overHead;
		auxSize 			= overHead;

		// not the last level therefore could be 'A's
		if (handler.NDimensions() > (level+1)){
			VariableDescriptorLib::DimensionSize multiplierD(multiplier);
			level++;
			char8 type = handler[level].type;
			while ((type=='A')&& ret){
				multiplierD 	= multiplierD * handler[level].numberOfElements;
				level++;
				ret = handler[level].elementSize.ToNumber(nextElSize);
				type 		= handler[level].type;
			}
			ret = multiplierD.ToNumber(multiplier);
		}

		// either was the last level or we are looking at one more levels below
		// either way if we find a '0' here we can act accordingly
		if (handler[level].type == '\0'){
			TypeDescriptor td = handler.GetTypeDescriptor();
			uint32 storageSize = nextElSize;
			bool hasVariableSize = td.IsCharStreamType();
			if (hasVariableSize){
				auxSize += multiplier * storageSize;
				for (uint32 i = 0;i < multiplier;i++){
					dataSize += td.FullSize(pointer);
					pointer += storageSize;
				}
			} else {
				dataSize += multiplier * storageSize;
			}
		// not a '0' so not last level so we are at least one level below
		} else {
			for (uint64 i = 0;(i < multiplier) && ret;i++){
				uint64 dataSize2;
				uint64 auxSize2;
				ret = GetSizeRecursive(level,handler,pointer,dataSize2,auxSize2);
				dataSize 			+= dataSize2;
				auxSize 			+= auxSize2;
				pointer 			+= nextElSize;
			}
		}
	}

	return ret;
}

/**
 * This function does a double recursion in order to process the pre-pending(*) and post-pending([]) modifiers
 */
ErrorManagement::ErrorType ToString(CCString modifiers,const TypeDescriptor &typeDescriptor,DynamicCString &string,CCString modifierString,bool start,int8 &priority){
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
						string.Append(')');
						string.Append('[');
						string.Append(size);
						string.Append(']');
					}break;
					case 'A':{
						string.Append('[');
						string.Append(size);
						string.Append(']');
					}break;
					case 'p':
					case 'P':{
					}break;
					case '\0':
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
		if (modifier == '\0'){
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
					string.Append(',');
					string.Append(size);
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
