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
#include "MemoryOperationsHelper.h"
#include "DynamicCString.h"
#include "MemoryCheck.h"
#include "MemoryOperationsHelper.h"
#include "TypeConversionManager.h"
#include "VariableDescriptor.h"
#include "VDPrivate.h"
#include "MemoryPageFile.h"
#include "ReferenceT.h"
#include "AnyObjectT.h"
#include "MemoryPageObject.h"
#include "ZeroTerminatedArray.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

// just a reasonable number. max size of an object in ZeroTermArray
const uint32 MaxZTALayerSize = 1024;

// max copiable size
const uint64 MaxCopySize = 1024*1024*128;

// max size of an intermediate layer
const uint64 MaxLayerSize = 1024*1024;


void VariableDescriptor::AddModifiersLayerConst(char8 modifier, uint64 size){
	if (modifier != '\0'){
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
		modifiers.Append(modifier);
		if (size > 0) modifiers.Append(size);
	}
}


VariableDescriptor::VariableDescriptor(){
    typeDescriptor = VoidType;
}

VariableDescriptor::~VariableDescriptor(){
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td, CCString modifierString){
    typeDescriptor = td;
    modifiers.Append(modifierString.GetList());
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.Append(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.Append(x.modifiers.GetList());
}

VariableDescriptor &VariableDescriptor::operator=(const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.Truncate(0);
    modifiers.Append(x.modifiers.GetList());
    return *this;
}



static inline bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static inline uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

static inline char8 toUpper(char8 c){
	if ((c >='a') && (c <= 'z')){
		c = (c - 'a') + 'A';
	}
	return c;
}

/** string to integer */
static inline uint32 readNumber(CCString &buffer){
	uint32 result = 0;
	while (isNumber(buffer[0])){
		result = result * 10u;
		result += toNumber(buffer[0]);
		buffer++;
	}
	return result;
}

/**
 * extracts information about a layer
 * updates the layer string pointer
 * used 8 times
 */
static inline void GetLayerInfo(CCString &modifierString,char8 &modifier,uint32 &size ){
	if (modifierString.IsNullPtr()){
		modifier = '\0';
		size = 0;
	} else {
		modifier = modifierString[0];
		if (modifier == '\0'){
			size = 0;
		} else {
			modifierString++;
			size = readNumber(modifierString);
		}
	}
}

// used externally 4 times
TypeDescriptor VariableDescriptor::GetSummaryTypeDescriptor() const {
    CCString modifiersCopy = modifiers;
    char8 firstModifier  = '\0';
    char8 nextModifier  = '\0';
    uint32 size = 0;
    GetLayerInfo(modifiersCopy,firstModifier,size );
    nextModifier= modifiersCopy[0];

    TypeDescriptor td;

    if (firstModifier == '\0'){
    	td = typeDescriptor;
    } else {
    	switch (firstModifier){
    	case 'P':
    	case 'p':{
    		td = GenericPointer;

    		if (nextModifier == 0){
    			td = PointerType;
    		}
    	}break;
    	case 'V':
    	case 'v':
    	case 'M':
    	case 'm':{
    		td = GenericPointer;
    	}break;
    	case 'A':{
    		td = GenericArray;
    	}break;

    	default:{
    		td = InvalidType(0);
            REPORT_ERROR(ErrorManagement::FatalError, "Incorrect modifier: ZDS not prepended by P ");
    	}
    	}
    }

	return td;
}

// used 4 times
ErrorManagement::ErrorType VariableDescriptor::FullLayerInfo(
		CCString &modifierString,
		const uint8 *pointer,
		uint64 &numberOfElements,
		uint32 &elementSize,
		uint32 &arrayStringSize,
		uint32 &numberOfTermElements,
		char8  &modifier) const{

	ErrorManagement::ErrorType ret;
    char8 c = '\0';
 	uint32 n = 0;
 	numberOfElements = 1;
 	// to allow recovering size of array section
 	arrayStringSize = 0;
 	numberOfTermElements = 0;

 	const char8 * savePtr = modifierString.GetList();

 	const CCString variableLayers = "zZdDsS";

 	GetLayerInfo(modifierString,c,n);

 	// only first layer may be a variable size one
 	// between variable size there must be a pointer of sort
	if (variableLayers.In(c)) {
		if (pointer == NULL){
			ret.internalSetupError = true;
	        REPORT_ERROR(ret, "pointer=NULL and a ZTA is found. sequence of two ZTA var layers not possible");
		}

		uint64 layerSize  = 0;
		if (ret){
	 		uint32 ass = (savePtr - modifierString.GetList());
	 		uint32 temp;
	 		// pointer is set to NULL to communicate the fact that this is part of a variable layer
			ret = FullLayerInfo(modifierString,NULL,numberOfElements,elementSize,arrayStringSize,temp,modifier);
			ass += arrayStringSize;
			numberOfTermElements = numberOfElements;
			layerSize = numberOfElements*elementSize;
			ret.unsupportedFeature = (layerSize > MaxZTALayerSize);
			if (layerSize > MaxZTALayerSize){
				ret.unsupportedFeature = true;
		        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "Size of ZTA element too large ");
			}
		}
		if (ret){
			uint32 ztaSize = ZeroTerminatedArrayGetSize(pointer, (uint32)layerSize);
			numberOfElements = numberOfElements * ztaSize;
		}
	} else { // not variable layer
 		while (c == 'A') {
			numberOfElements = numberOfElements * n;
	 		arrayStringSize = (savePtr - modifierString.GetList());
	 		GetLayerInfo(modifierString,c,n);
 		}
		if (c== '\0'){
			elementSize = typeDescriptor.StorageSize();

		} else
		if ((c=='p') || (c=='P')){
			elementSize = sizeof(void *);
		} else
		if ((c=='v') || (c=='V')){
			elementSize = sizeof(Vector<int>);
		} else
		if ((c=='m') || (c=='M')){
			elementSize = sizeof(Matrix<int>);
		} else {
			ret.unsupportedFeature = true;
	        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "Unsupported modifier ");
		}
	 	modifier = c;
	}

 	return ret;
 }

// used 4 times
uint64 VariableDescriptor::FullLayerSize(CCString modifierString,const uint8 *pointer) const{
	 uint64 numberOfElements = 0;
	 uint32 elementSize = 0;
	 uint32 numberOfTermElements = 0;
	 uint32 temp;
	 char8 modifier = '\0';
	 CCString modS = modifierString;

 	 FullLayerInfo(modS,pointer,numberOfElements,elementSize,temp,numberOfTermElements,modifier);
 	 return (numberOfElements + numberOfTermElements)* elementSize ;
}

// used by copyTo twice.
TypeDescriptor VariableDescriptor::GetDimensionsInformation(DynamicZeroTerminatedArray<DimensionInfo,4> &dimensions) const{
	const CCString pointerModifiers = "Pp";
	const CCString immediateModifiers = "AVM";
	const CCString prefixedModifiers = "ZDS";
	TypeDescriptor td = typeDescriptor;

	dimensions.Truncate(0);

    CCString modifierString = modifiers;
    CCString previousModifierString = modifiers;
    CCString savedModifierString = modifiers;
    DimensionInfo di;

    // stores
    char8 lastPointer = '\0';

    di.type = '?';
    while (di.type != 0){
    	previousModifierString = modifierString;
        GetLayerInfo(modifierString,di.type,di.numberOfElements);
        char8 c = toUpper(di.type);
        // not a type descriptor yet
        if (c != '\0'){
        	// Array Vector or Matrix
        	if (immediateModifiers.In(c)){
        		// PA == F  others: errors
        		if (lastPointer != '\0'){
        			if (c == 'A'){
    					di.type = 'f';
        				if (lastPointer == 'P'){
        					di.type = 'F';
        	        		dimensions.Append(di);
        				}
        			} else { // C != A
            			// error PV PM etc...
//        				td = InvalidType(0);
        				// treat as a pointer
            			// stop and exit
            			di.type = '\0';
        				td = PointerType;
            			modifierString = savedModifierString;
        			}
        			// consume last pointer
        			lastPointer = '\0';
        		} else { // lastPointer == 0
            		dimensions.Append(di);
        		}
        	} else // NOT Array Vector or Matrix
        	// Zero Terminated Arrays
        	if (prefixedModifiers.In(c)){
        		// there must be a P before!
        		if (lastPointer != '\0'){
            		dimensions.Append(di);
            		lastPointer = '\0';
        		} else {
        			//error ZSD without prefix
        			// stop and exit
        			di.type = '\0';
    				td = InvalidType(0);
        			// rewind to this layer
        			modifierString = previousModifierString;
        		}
        	} else
        	// found a pointer.
        	if (c == 'P'){
        		// check for a PP sequence
        		if (lastPointer != '\0'){
        			//PP!
        			// stop and exit
        			di.type = '\0';
    				td = PointerType;
        			modifierString = savedModifierString;
        		} else {
        			//
        			lastPointer = di.type;
        			savedModifierString = previousModifierString;
        		}
        	}
        // handle the final case
        } else { // c == 0

        	// handle the P0 case as PA1 case
    		if (lastPointer != '\0'){
    			di.type = 'F';
    			di.numberOfElements = 1;
        		dimensions.Append(di);
    			di.type = '\0';

    		} else {
    			di.type = 'A';
    			di.numberOfElements = 1;
        		dimensions.Append(di);
    			di.type = '\0';
    		}
        }
    }

	return td;
}

// used internally 4 time
ErrorManagement::ErrorType VariableDescriptor::GetDeepSize(CCString modifierString, const uint8 *pointer,
		                               uint64 &dataSize, uint64 &overHeadSz,uint8 maxDepth,uint32 layerMultiplier) const {

	ErrorManagement::ErrorType ret;

	uint64 numberOfElements = 0;
	uint32 elementSize = 0;
	uint32 overheadSize = 0;
	uint32 temp;
    uint32 numberOfTermElements = 0;
	char8 modifier = '\0';

	ret = FullLayerInfo(modifierString,pointer,numberOfElements,elementSize,temp,numberOfTermElements,modifier);
	// account for extra array layer (vector and matrix support)
	numberOfElements = numberOfElements * layerMultiplier;
	/// include terminators
	uint64 numberOfElementsIncludingTerm = numberOfElements  + numberOfTermElements;

	/// if numberOfTermElements > 0  && layerMultiplier > 1 error
	/// means that a sequence VZ is encountered instead of the correct VPZ
	if ((numberOfTermElements > 0) && (layerMultiplier > 1)){
		ret.internalSetupError = true;
        REPORT_ERROR(ret, "Incorrect sequence VZ or MZ encountered ");
	}

	if (ret){
		// if we can delve deep and there is deep....
		if ((maxDepth > 0) && (modifier != '\0')){
			maxDepth--;

			switch(modifier){
			case 'p':
			case 'P':{
				// cannot do this conversion using c++ style casts
				const uint8 **pp = (const uint8 **)(pointer);
				//if (true /*isVariableSize(modifierString)*/){
				overHeadSz = numberOfElementsIncludingTerm * elementSize;
				dataSize = overHeadSz;
				uint64 index;
				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = pp[index];
					if (p != NULL){
						if (!MemoryCheck::Check(p)){
							ret.exception = true;
							DynamicCString errM;
							errM.Append("Pointer[");
							errM.Append(index);
							errM.Append("]=");
							errM.AppendHex(reinterpret_cast<uint64>(p));
							errM.Append(" is Invalid");
					        REPORT_ERROR(ret,errM.GetList());
						}

						if (ret){
							uint64 dataSize2;
							uint64 overHeadSz2;
							ret = GetDeepSize(modifierString, p,dataSize2,overHeadSz2,maxDepth);

							if (ret){
								overHeadSz = overHeadSz + overHeadSz2;
								dataSize = dataSize + dataSize2 ;
							}
						}
					}
				}
			} break;
			case 'V':
			case 'v':{
				const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);
				overHeadSz = numberOfElementsIncludingTerm * elementSize;
				dataSize = overHeadSz;
				uint64 index;

				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = static_cast<const uint8 *>(pv[index].GetDataPointer());
					uint32 numberOfArrayElements = pv[index].GetNumberOfElements();
					if (numberOfArrayElements != 0){
						if (!MemoryCheck::Check(p)){
							ret.exception = true;
					        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
						}
						if (ret){
							uint64 dataSize2;
							uint64 storageSize2;
							ret = GetDeepSize(modifierString, p,dataSize2,storageSize2,maxDepth,numberOfArrayElements);

							if (ret){
								overHeadSz = overHeadSz + storageSize2;
								dataSize = dataSize + dataSize2 ;
							}
						}
					}
				}
			} break;
			case 'M':
			case 'm':{
				const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);
				overHeadSz = numberOfElementsIncludingTerm * elementSize;
				dataSize = overHeadSz;
				uint64 index;
				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = static_cast<const uint8 *>(pm[index].GetDataPointer());
					uint32 numberOfArrayElements = pm[index].GetNumberOfRows() * pm[index].GetNumberOfColumns();
					if (numberOfArrayElements > 0){
						if (!MemoryCheck::Check(p)){
							ret.exception = true;
					        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
						}
						if (ret){
							uint64 dataSize2;
							uint64 storageSize2;
							ret = GetDeepSize(modifierString, p,dataSize2,storageSize2,maxDepth,numberOfArrayElements);

							if (ret){
								overHeadSz = overHeadSz + storageSize2;
								dataSize = dataSize + dataSize2 ;
							}
						}
					}
				}
			} break;
			default:{
				// Aa, Zz, Ss, Dd are not terminators of a layer .
				// they shall not be encountered here
				ret.fatalError = true;
		        REPORT_ERROR(ErrorManagement::FatalError, "AZSD at a layer termination");
			}
			}


		} else {  // '\0' or maxdepth reached
			overHeadSz = numberOfElementsIncludingTerm * elementSize;
			if (typeDescriptor.IsBasicType() || typeDescriptor.IsStructuredData()){
				dataSize = numberOfElements * elementSize;
				overHeadSz -= dataSize;
			} else
			if (typeDescriptor.IsCharStreamType()){
					dataSize = 0;
					uint64 index;
					for (index = 0; (index < numberOfElements) && ret; index++){
						dataSize += typeDescriptor.FullSize(pointer);
						pointer +=  elementSize;
					}
					// add the terminator!
					if (index < numberOfElementsIncludingTerm){
						dataSize += elementSize;
					}
			} else {
				dataSize = overHeadSz;
			}
		}
	}
	return ret;
}


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
	if (pointer == NULL_PTR(const uint8 *)){
		ret.parametersError = true;
	}

	if (ret){
		GetLayerInfo(modifierString,modifier,size);

	 	switch (modifier){
	 	case 'p':
	 	case 'P':{
			const uint8 **pp = (const uint8 **)(pointer);
			const uint8 *p = *pp;
			if (!MemoryCheck::Check(p)){
				ret.exception = true;
		        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
			}
			if (ret){
				pointer = p;
	 			char8 nextModifier = modifierString[0];
	 			switch (nextModifier){
	 			case 'A':
			 	case 's':
	 			case 'S':
		 		case 'd':
	 			case 'D':
	 			case 'z':
		 		case 'Z':{
		        	ret = Redirect(pointer,index,modifierString);
	 			}break;
		 		default:{
					if (index != 0){
						ret.outOfRange = true;
				        REPORT_ERROR(ErrorManagement::OutOfRange, "index!=0 for pointer to var");
					}

					if (ret){
						modifiers.Remove(modifierString.GetList()-modifiers.GetList());
					}
	 			}
	 			}// end switch
			} // ret is not false
	 	}break; // end Pp case
	 	case 'A':{
	 		// need fulllayerSize of the remaining full layer
	 		// note that next layer cannot be ZzDdSs but only A or a terminator like PpVvMm and 0
			if (index < size){
				uint64 layerSize = FullLayerSize(modifierString,pointer);
				uint64 step = layerSize * index;
				pointer = pointer + step;
				modifiers.Remove(modifierString.GetList()-modifiers.GetList());
			} else {
				ret.outOfRange = true;
		        REPORT_ERROR(ErrorManagement::OutOfRange, "index >= size");
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
			uint64 layerSize = FullLayerSize(modifierString,pointer);
			if (layerSize > MaxZTALayerSize){
				ret.unsupportedFeature = true;
		        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "ZTA element too large");
			}
			if (ret){
				uint32 maxIndex  = ZeroTerminatedArrayGetSize(pointer, layerSize);
				if (index >= maxIndex){
					ret.outOfRange = true;
			        REPORT_ERROR(ret, "index >= maxIndex");
				}
			}
			if (ret){
				uint64 step = layerSize * index;
				pointer = pointer + step;
				modifiers.Remove(modifierString.GetList()-modifiers.GetList());
			}
	 	}break;
	 	case 'v':
	 	case 'V':{
			const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);
			const uint8 *p = static_cast<const uint8 *>(pv[0].GetDataPointer());
			uint32 numberOfArrayElements = pv[0].GetNumberOfElements();
			if (!MemoryCheck::Check(p)){
				ret.exception = true;
		        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
			}
			if (index >=  numberOfArrayElements){
				ret.outOfRange = true;
		        REPORT_ERROR(ErrorManagement::OutOfRange, "index >= numberOfArrayElements");
			}

			if (ret){
				uint64 layerSize = FullLayerSize(modifierString,pointer);
				uint64 step = layerSize * index;
				pointer = p + step;
				modifiers.Remove(modifierString.GetList()-modifiers.GetList());
			}
	 	}break;
	 	case 'm':
	 	case 'M':{
			const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);
			const uint8 *p = static_cast<const uint8 *>(pm[0].GetDataPointer());
			if (!MemoryCheck::Check(p)){
				ret.exception = true;
		        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
			}
			if (index >=  pm[0].GetNumberOfRows()){
				ret.outOfRange = true;
		        REPORT_ERROR(ErrorManagement::OutOfRange, "index >= pm[0].GetNumberOfRows()");
			}
			if (ret){
				uint64 layerSize = FullLayerSize(modifierString,pointer);
				uint64 step = layerSize * index * pm[0].GetNumberOfColumns();
				pointer = p + step;

				DynamicCString modifiersTemp;
				modifiersTemp.Append(modifierString);
				modifiers.Truncate(0);
				modifiers.Append('A');
				modifiers.Append(pm[0].GetNumberOfColumns());
				modifiers.Append(modifiersTemp);
			}

	 	}break;
	 	case '\0':{
	 		if (typeDescriptor.IsCharStreamType()){
	 			TD_FullType fullType = typeDescriptor.fullType;
	 			switch (fullType){
	 			case TDF_DynamicCString:
	 			case TDF_CString:
	 			case TDF_CCString:{
	 				CCString *string = reinterpret_cast<CCString *>(const_cast<uint8 *>(pointer));
	 				uint32 size = string->GetSize();
	 				if (size <= index){
	 					ret.outOfRange = true;
	 			        REPORT_ERROR(ErrorManagement::OutOfRange, "index >= pm[0].GetNumberOfRows()");
	 				}
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
			        REPORT_ERROR(ErrorManagement::IllegalOperation, "can only redirect Zero Term Char streams");
	 			} break;
	 			}
	 		} else {
		 		ret.illegalOperation = true;
		        REPORT_ERROR(ErrorManagement::IllegalOperation, "cannot redirect a basic type");
	 		}
	 	} break;
	 	default:{
	 		ret.internalSetupError = true;
	        REPORT_ERROR(ErrorManagement::InternalSetupError, "unmapped modifier");
	 	}
	 	}

	}

	return ret;
}


//used by GetNextElementSize
static inline  uint32 GetDimensionSize(char8 type){
	char8 c = toUpper(type);
	uint32 size = 0;
	switch(c){
	case 'M':{
		size = sizeof (Matrix<char8>);
	}break;
	case 'V':{
		size = sizeof (Vector<char8>);
	}break;
	case 'S':
	case 'D':
	case 'Z':
	case 'F':{
		size = sizeof (void *);
	}break;
	default:{}
	}
	return size;
}

// used by HandlePointer
static uint32 GetNextElementSize(
		const ZeroTerminatedArray<DimensionInfo> &dimensions,
		TypeDescriptor &td
		){

	uint32 size = 1;
	uint32 ix = 1;
	while (dimensions[ix].type == 'A'){
		size *= dimensions[ix].numberOfElements;
		ix++;
	}
	if (dimensions[ix].type == '\0'){
// TODO support variable size terminals
		size *= td.StorageSize();
	} else {
		size *= GetDimensionSize(dimensions[ix].type);
	}
	return size;
}

static inline ErrorManagement::ErrorType RedirectP(const uint8* &ptr){
	ErrorManagement::ErrorType ret;
	const uint8 **pp = (const uint8 **)(ptr);
	const uint8 *p = *pp;
	if (!MemoryCheck::Check(p)){
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

//used by LayerOperate
static inline ErrorManagement::ErrorType HandlePointer(
		const ZeroTerminatedArray<DimensionInfo> &dimensions,
		const uint8* &ptr,
		TypeDescriptor td,
		uint32 &numberOfElements,
		uint32 &sourceElementSize
		){
	ErrorManagement::ErrorType ret;

	sourceElementSize = GetNextElementSize(dimensions,td);
	numberOfElements = 1;

	if (dimensions.GetSize() > 0){
		switch(toupper(dimensions[0].type)){
		case 'A':{
			numberOfElements = dimensions[0].numberOfElements;
		}break;
		case 'F':{
			numberOfElements = dimensions[0].numberOfElements;
			ret = RedirectP(ptr);
			if (!ret){
		        REPORT_ERROR(ret, "pointer to array redirection failed");
			}
		}break;
		case 'D':
		case 'S':
		case 'Z':{
			ret = RedirectP(ptr);
			if (!ret){
		        REPORT_ERROR(ret, "pointer to Zero Term array redir failed");
			}
			if (ret){
				numberOfElements = ZeroTerminatedArrayGetSize(ptr, sourceElementSize);
			}
		}break;
		case 'V':{
			const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
			numberOfElements = pv->GetNumberOfElements();

			// it works as vector is descendant of Pointer class
			ret = RedirectP(ptr);
			if (!ret){
		        REPORT_ERROR(ret, "pointer to Vector Data redir failed");
			}
		}break;
		case 'M':{
			const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
			numberOfElements = pm->GetNumberOfElements();

			// it works as vector is descendant of Pointer class
			ret = RedirectP(ptr);
			if (!ret){
		        REPORT_ERROR(ret, "pointer to Matrix Data redir failed");
			}
		}break;
		default:{
			ret.fatalError = true;
	        REPORT_ERROR(ret, "Default case reached");
		}break;

		};
	}

	return ret;
}

// used by CopyTo
static ErrorManagement::ErrorType LayerOperate(
		ZeroTerminatedArray<DimensionInfo> sourceDimensions,
		const uint8* sourcePtr,
		TypeDescriptor &sourceTd,
		ZeroTerminatedArray<DimensionInfo> destDimensions,
		uint8* destPtr,
		TypeDescriptor &destTd,
		const TypeConversionOperatorI &op
		){
	ErrorManagement::ErrorType ret;

///printf("layer start %p %p t=%c\n",sourcePtr,destPtr,sourceDimensions[0].type);

	uint32 sourceNumberOfElements = 1;
	uint32 sourceElementSize = 1;
	uint32 destNumberOfElements = 1;
	uint32 destElementSize = 1;

	// handle source pointer;
	if (ret){
		ret = HandlePointer(sourceDimensions,sourcePtr,sourceTd,sourceNumberOfElements,sourceElementSize);
		if (!ret){
	        REPORT_ERROR(ret, "source pointer handling failed");
		}
	}

	// handle dest pointer;
	if (ret){
		ret = HandlePointer(destDimensions,(const uint8 *&)destPtr,destTd,destNumberOfElements,destElementSize);
		if (!ret){
	        REPORT_ERROR(ret, "destination pointer handling failed");
		}
	}

//printf("layer end  %p %p N=%i ND=%i\n",sourcePtr,destPtr,sourceNumberOfElements,destDimensions.GetSize());

	if (ret){
		if (sourceNumberOfElements != destNumberOfElements){
			ret.unsupportedFeature = true;
	        REPORT_ERROR(ret, "mismatch in dimensions");
		}
	}

	if (ret){
		// the last dimension is always the scalar typed
		if (destDimensions.GetSize() <= 2){
			ret = op.Convert(destPtr,sourcePtr,destNumberOfElements);
			if (!ret){
		        REPORT_ERROR(ret, "conversion failed");
			}
		} else {
			sourceDimensions++;
			destDimensions++;
			// skip forward
			uint32 ix = 0;
			for (ix = 0; (ix < sourceNumberOfElements) && ret; ix++){
				ret = LayerOperate(sourceDimensions,sourcePtr,sourceTd,destDimensions,destPtr,destTd,op);
				sourcePtr+= sourceElementSize;
				destPtr+= destElementSize;

				if (!ret){
					DynamicCString errM;
					errM.Append("Failed at row (");
					errM.Append(ix);
					errM.Append(')');
			        REPORT_ERROR(ret, errM.GetList());
				}
			}
		}
	}

	return ret;
}


// used by CopyTo
static ErrorManagement::ErrorType CopyToRecursive(
		uint32 							level,
		DimensionHandler 				&sourceDimensions,
		const uint8* 					sourcePtr,
		DimensionHandler 				&destDimensions,
		uint8* 							destPtr,
		const TypeConversionOperatorI &	op
		){
	ErrorManagement::ErrorType ret;
/*
char c1 = 	sourceDimensions[0].type;
char c2 = 	sourceDimensions[1].type;
printf ("%i %c %i\n",sourceDimensions.NDimensions(),c1,c2);
c1 = 	destDimensions[0].type;
c2 = 	destDimensions[1].type;
printf ("%i %c %i\n",destDimensions.NDimensions(),c1,c2);
*/
	uint32 sourceNumberOfElements = 1;
	uint32 sourceElementSize = 1;
	uint32 destNumberOfElements = 1;
	uint32 destElementSize = 1;
	uint32 overHead;

	ret = sourceDimensions.UpdatePointerAndSize(level,sourcePtr,sourceNumberOfElements,sourceElementSize,overHead);
    CONDITIONAL_REPORT_ERROR(ret, "source pointer handling failed");

	if (ret){
		ret = destDimensions.UpdatePointerAndSize(level,(const uint8 *&)destPtr,destNumberOfElements,destElementSize,overHead);
		CONDITIONAL_REPORT_ERROR(ret, "destination pointer handling failed");
	}

	if (ret){
		if (sourceNumberOfElements != destNumberOfElements){
			ret.unsupportedFeature = true;
	        REPORT_ERROR(ret, "mismatch in dimensions");
		}
	}
	if (ret){
		// the last dimension is always the scalar typed
		if (destDimensions.NDimensions() <= (level+2)){
//printf("OP[%i]\n",destNumberOfElements);
			ret = op.Convert(destPtr,sourcePtr,destNumberOfElements);
	        CONDITIONAL_REPORT_ERROR(ret, "conversion failed");
		} else {
			// skip forward
			uint32 ix = 0;
			for (ix = 0; (ix < sourceNumberOfElements) && ret; ix++){
				ret = CopyToRecursive(level+1,sourceDimensions,sourcePtr,destDimensions,destPtr,op);
				sourcePtr+= sourceElementSize;
				destPtr+= destElementSize;

				if (!ret){
					DynamicCString errM;
					errM.Append("Failed at row (");
					errM.Append(ix);
					errM.Append(')');
			        REPORT_ERROR(ret, errM.GetList());
				}
			}
		}
	}
	return ret;
}

ErrorManagement::ErrorType VariableDescriptor::CopyTo(
		const uint8 *sourcePtr,
			  uint8 *destPtr,
			  const VariableDescriptor &destVd,
			  bool isCompare
		) const {

//printf("2 sp=%p dp=%p\n",sourcePtr,destPtr);

	ErrorManagement::ErrorType ret;
	const TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

	DimensionHandler sourceHandler(this->modifiers,this->typeDescriptor);
	DimensionHandler destHandler(destVd.modifiers,destVd.typeDescriptor);
/*
char c1 = 	sourceHandler[0].type;
char c2 = 	sourceHandler[1].type;
printf ("%i %c %i\n",sourceHandler.NDimensions(),c1,c2);
c1 = 	destHandler[0].type;
c2 = 	destHandler[1].type;
printf ("%i %c %i\n",destHandler.NDimensions(),c1,c2);
*/
	if (!destHandler.HasSameDimensionsAs(sourceHandler)){
		ret.internalSetupError = true;
        REPORT_ERROR(ret, "Dimension mismatch");
	}

	if (ret){
		tco = TypeConversionManager::Instance().GetOperator(destHandler.GetTypeDescriptor(),sourceHandler.GetTypeDescriptor(),isCompare);
	    if ( tco == NULL_PTR(TypeConversionOperatorI *)){
	    	ret.unsupportedFeature = true;
	        REPORT_ERROR(ret, "Conversion Operator not found");
	    }
	}

    if (ret){
    	ret = CopyToRecursive(0,sourceHandler,sourcePtr,destHandler,destPtr,*tco);
    }

    if (tco != NULL_PTR(TypeConversionOperatorI *)){
    	delete tco;
    }

    return ret;
}

static ErrorManagement::ErrorType GetSizeRecursive(
		uint32 							level,
		DimensionHandler 				&handler,
		const uint8* 					pointer,
		uint64 							&dataSize,
		uint64 							&auxSize
		){
	ErrorManagement::ErrorType ret;

	uint32 nextElSize;
	uint32 overHead;
	uint32 multiplier;
	ret = handler.UpdatePointerAndSize(level, pointer,multiplier,nextElSize,overHead);
 	dataSize 			= overHead;
	auxSize 			= overHead;

//printf("mul=%i sz=%i\n",multiplier,nextElSize); //TODO

	// not the last level therefore could be 'A's
	if (handler.NDimensions() > (level+1)){
		DimensionSize multiplierD(multiplier);
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

//printf("mul=%i sz=%i\n",multiplier,nextElSize); //TODO

	// either was the last level or we are looking at one more levels below
	// either way if we find a '0' here we can act accordingly
	if (handler[level].type == '\0'){
		TypeDescriptor td = handler.GetTypeDescriptor();
//		uint32 storageSize = handler[level].elementSize;
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
		for (uint64 i = 0;i < multiplier;i++){
			uint64 dataSize2;
			uint64 auxSize2;
			GetSizeRecursive(level,handler,pointer,dataSize2,auxSize2);
			dataSize 			+= dataSize2;
			auxSize 			+= auxSize2;
			pointer 			+= nextElSize;
		}
	}

	return ret;
}

ErrorManagement::ErrorType VariableDescriptor::GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *overHeadSize) const{
	ErrorManagement::ErrorType ret;
	uint64 overHeadSz = 0;

	DimensionHandler handler(this->modifiers,this->typeDescriptor);

	ret = GetSizeRecursive(0,handler,pointer,dataSize,overHeadSz);

	if (overHeadSize != NULL){
		*overHeadSize = overHeadSz;
	}
	return ret;
}

#if 0

/**
 * @brief copies the variable layer by layer. The copied layer is implemented in contiguous memory
 * @param[in] sourcePtr, the pointer to the variable to be copied
 * @param[in,out] destFreePtr, pointer to the memory area to copy the variable to. Returns the pointer to the unused area
 * @param[in,out] destFreeSize, length of the memory area to copy to. Returns the unused area size
 * @param[out] destVd, the variable descriptor of the used area. Note that all varieties of ZeroTermarrays become ZeroTermArray<const T>
 * @param[in] maxDepth, the max number of pointer redirection to include in the copy
 * @param[in] destPtr, pointer to the area reserved for next layer. use if provided or take from destFreePtr
 * @param[in] modifierString, at start points at full modifiers.After each recursion it is progressively consumed.
 * @return true if all ok or the error
 */
ErrorManagement::ErrorType VariableDescriptor::Copy(
		const uint8 *sourcePtr,
		uint8 *&destFreePtr,
		uint64 &destFreeSize,
		VariableDescriptor &destVd,
		uint8 maxDepth,
		CCString modifierString) const {

	ErrorManagement::ErrorType ret;

    if (modifierString.IsNullPtr()){
    	modifierString = modifiers;
    }
    CCString modifierStringSave = modifierString;

	// number of elements in this layer
	uint64 numberOfElements = 1;
	// size of each element
    uint32 elementSize = 0;
//    uint32 overheadSize = 0;
    uint32 numberOfTermElements = 0;
	char8 modifier  = '\0';
	uint32 arrayStringSize=0;
	uint64 totalLayerSize=0;

	ret = FullLayerInfo(modifierString,sourcePtr,numberOfElements,elementSize,arrayStringSize,numberOfTermElements,modifier);

    if (ret){
    	totalLayerSize = elementSize * (numberOfElements + numberOfTermElements);
		if (totalLayerSize > destFreeSize){
			ret.outOfMemory = true;
	        REPORT_ERROR(ErrorManagement::OutOfMemory, "totalLayerSize > destFreeSize");
		}
		if (totalLayerSize > MaxCopySize){
			ret.unsupportedFeature = true;
	        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "totalLayerSize > MaxCopySize");
		}
    }

    if (ret){
    	if ( (modifier != '\0') && !/*(*/(modifier == 'P') /*&& nextIsVariableLayer)*/){
    		if (totalLayerSize > MaxLayerSize){
    			ret.unsupportedFeature = true;
    	        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "totalLayerSize > MaxLayerSize");
    		}
    	}
    }

    if (ret){
    	// copy all multiplying modifiers A,Z,D,S,z,d,s
	    destVd.modifiers.Append(modifierStringSave,arrayStringSize);

		if (((maxDepth == 0) || (modifier == '\0')) /* && !((modifier == 'P') && (nextIsVariableLayer))*/ ){

			// just copy all as is
			MemoryOperationsHelper::Copy(destFreePtr,sourcePtr,totalLayerSize);

		} else {
			maxDepth--;

			switch(modifier){
			case 'P':{
				// cannot do this conversion using c++ style casts
				const uint8 **spp = (const uint8 **)(sourcePtr);
				uint8 **dpp = (uint8 **)(destFreePtr);

			    destVd.modifiers.Append(modifier);
				// reserve pointer space
				destFreePtr  += totalLayerSize;
				destFreeSize -= totalLayerSize;

				uint32 index;
				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = spp[index];
					dpp[index] = destFreePtr;
					if (!MemoryCheck::Check(p)){
						ret.exception = true;
				        REPORT_ERROR(ret, "bad pointer");
					}
					if (ret){
						if (index == 0){
							VariableDescriptor &dvd = destVd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						} else {
							VariableDescriptor dvd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						}
					}
				}

				MemoryOperationsHelper::Set(&dpp[numberOfElements],0,numberOfTermElements*elementSize);
			}break;

			case 'm':
			case 'M':{
				const Matrix<uint8> *psm = reinterpret_cast<const Matrix<uint8> *>(sourcePtr);
			    destVd.modifiers.Append(modifier);
    				// reserve pointer space
				Matrix<uint8> *dpm = reinterpret_cast< Matrix<uint8> *>(destFreePtr);
				destFreePtr  += totalLayerSize;
				destFreeSize -= totalLayerSize;

				uint32 index;
				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = static_cast<const uint8 *>(psm[index].GetDataPointer());
					dpm[index].InitMatrix(destFreePtr,psm[index].GetNumberOfRows(),psm[index].GetNumberOfColumns()) ;
					if (!MemoryCheck::Check(p)){
						ret.exception = true;
				        REPORT_ERROR(ret, "bad pointer");
					}
					if (ret){
						if (index == 0){
							VariableDescriptor &dvd = destVd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						} else {
							VariableDescriptor dvd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						}
					}
				}
				//zero the terminator layer
				MemoryOperationsHelper::Set(&dpm[numberOfElements],0,numberOfTermElements*elementSize);

			}break;
			case 'v':
			case 'V':{
				const Vector<uint8> *psm = reinterpret_cast<const Vector<uint8> *>(sourcePtr);
			    destVd.modifiers.Append(modifier);
    				// reserve pointer space
			    Vector<uint8> *dpv = reinterpret_cast<Vector<uint8> *>(destFreePtr);
				destFreePtr  += totalLayerSize;
				destFreeSize -= totalLayerSize;

				uint32 index;
				for (index = 0; (index < numberOfElements) && ret; index++){
					const uint8 *p = static_cast<const uint8 *>(psm[index].GetDataPointer());
					dpv[index].InitVector(destFreePtr,psm[index].GetNumberOfElements()) ;
					if (!MemoryCheck::Check(p)){
						ret.exception = true;
				        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
					}
					if (ret){
						if (index == 0){
							VariableDescriptor &dvd = destVd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						} else {
							VariableDescriptor dvd;
							ret = Copy(p,destFreePtr,destFreeSize,dvd,maxDepth,modifierString);
						}
					}
				}

				//  zero the terminator layer
				MemoryOperationsHelper::Set(&dpv[numberOfElements],0,numberOfTermElements*elementSize);

			}break;
			default:{
			}
		    } // end switch
		}
	}// if ret

	return ret;
}
#endif

ErrorManagement::ErrorType VariableDescriptor::ToString(DynamicCString &string,bool rawFormat) const{
	ErrorManagement::ErrorType  ret;
	if (rawFormat){
		bool retbool = true;
		retbool = string.Append(modifiers);
		string.Append(' ');
		retbool = retbool && typeDescriptor.ToString(string);
		ret.fatalError = !retbool;
	} else {
		int8 priority=0;
	    ret = ToStringPrivate(string,emptyString,true,priority) ;
	}
    return ret;
}


ErrorManagement::ErrorType VariableDescriptor::ToStringPrivate(DynamicCString &string,CCString modifierString,bool start,int8 &priority)const{
	ErrorManagement::ErrorType ret;

	if (modifierString.IsNullPtr()){
		modifierString = modifiers;
	}
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifierString,modifier,size );

	if ((modifier == 'P') || (modifier == 'p')){
		// skip P/p layer if followed by one of ZDSzds
		const CCString zeroTerMods="zZsSdD";
		char8 nextModifier = modifierString[0];
		if ( zeroTerMods.In(nextModifier)){
			GetLayerInfo(modifierString,modifier,size );
			if (modifierString[0] == '\0'){
				if ((modifier == 'Z')||(modifier == 'z')){
					if (typeDescriptor.SameAs(Character8Bit)){
						if (modifier == 'z'){
							string.Append("const ");
						}
						string.Append("CString");
						return ret;
					} else
					if (typeDescriptor.SameAs(ConstCharacter8Bit)){
						if (modifier == 'z'){
							string.Append("const ");
						}
						string.Append("CCString");
						return ret;
					}
				}
				if ((modifier == 'D')||(modifier == 'd')){
					if (typeDescriptor.SameAs(Character8Bit) ){
						if (modifier == 'd'){
							string.Append("const ");
						}
						string.Append("DynamicCString");
						return ret;
					}
				}
				if ((modifier == 'S')||(modifier == 's')){
					if (typeDescriptor.SameAs(Character8Bit)){
						if (modifier == 'd'){
							string.Append("const ");
						}
						string.Append("StaticCString<");
						string.Append(size);
						string.Append('>');
						return ret;
					}
				}
			}
		}
	}

	// go ahead and process in reverse pointers and arrays
	if ((modifier=='A') || (modifier=='P')|| (modifier=='p')){
		ret = ToStringPrivate(string,modifierString,false,priority);

		// process A and P in reverse
		if (ret){
			if (modifier == 'A'){
				priority = 1;
			} else
			if (modifier == 'P'){
				if (priority == 1){
					priority = 0;
					string.Append('(');
				}
				string.Append(" *");
			} else
			if (modifier == 'p'){
				if (priority == 1){
					priority = 0;
					string.Append('(');
				}
				string.Append(" * const");
			}

			// if this was the start of the sequence now do the forward section to add the vectors[]
			if (start){
				priority = 1;

				bool end = false;
				while (!end  && ret){

					switch (modifier){
					case 'A':{
						if (priority == 0){
							priority = 1;
							string.Append(')');
						}
						string.Append('[');
						string.Append(size);
						string.Append(']');
					}break;
					case 'p':
					case 'P':{
						priority = 0;
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
						GetLayerInfo(modifierString,modifier,size );
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
				ret = ToStringPrivate(string,modifierString,true,localPriority);
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


class VDCloner{
public:
	VDCloner(CCString modifiers,TypeDescriptor tdIn): handler(modifiers,tdIn),pageFile(1024*1024)/*,indexPageFile(1024*1024)*/{
//		finalElementSize = 0;
		isString = false;
	}

	ErrorManagement::ErrorType Init(){
		ErrorManagement::ErrorType ret;

		// force allocation of first page
		ret = pageFile.CheckAndNewPage();
		CONDITIONAL_REPORT_ERROR(ret, "CheckAndNewPage(1) failed");

		isString = handler.GetTypeDescriptor().IsCharString();
		bool isFixedData = (handler.GetTypeDescriptor().IsStructuredData() || handler.GetTypeDescriptor().IsBasicType());

		if (!isString && !isFixedData){
			ret.unsupportedFeature = true;
			CONDITIONAL_REPORT_ERROR(ret, "Unsupported type");
		}

		if (ret){
			ret = DecideSimpleIndexing();
			CONDITIONAL_REPORT_ERROR(ret, "DecideSimpleIndexing failed");
		}

		return ret;
	}

	ErrorManagement::ErrorType DecideSimpleIndexing(){
		ErrorManagement::ErrorType ret;

		CCString variableVectors("vVzZdDsS");
		CCString variableMatrices("mM");
		CCString conditionalArrays("fF");

		char layerEnd = '\0';
		// loop through levels and create the description of the output type
		// skip '/0' level
		for (int32 level=handler.NDimensions()-1;level>=0;level-- ){
			DimensionInfoElement &die = handler[level];

			// an aggregator level
			if (die.type != '\0'){
				if (conditionalArrays.In(die.type)){
					die.outputType = 'f';
					layerEnd = 'f';
				} else
				if (variableVectors.In(die.type)){
					die.outputType = 'v';
					layerEnd = 'v';
				} else
//TODO correctly implement matrices
				if (variableMatrices.In(die.type)){
					die.outputType = 'v';
					layerEnd = 'v';
				} else {
					die.outputType = 'A';
				}
			}
			die.layerEnd = layerEnd;
		}

		return ret;
	}

/**
 * Recursive function
 * @param level is the current level of variable level indirection
 * @param inputPointer is the pointer to the data to be copied
 * @param startOfOutput
 * @param addressOfOutput address of the block created in the current level
 * @param sizeOfOutput  --> number of elements
 */
ErrorManagement::ErrorType DoCreateR(
			uint32 							level,
			const uint8* 					inputPointer,
			uint8 *&						addressOfOutput,
			DimensionSize 					numberOfElementsD
	){
		ErrorManagement::ErrorType ret;

		char8 outputType = handler[level].outputType;
		char8 inputType = handler[level].type;


		// handle the simple scalar type
		// consider the numberOfElements to be copied
		if (outputType == '\0'){
			// calculate total Size needed to store final layer
			// multiply the numberOfElements by each size
			DimensionSize totalSizeD = numberOfElementsD * handler.GetTypeDescriptor().StorageSize();
			uint32 totalSize;
			ret = totalSizeD.ToNumber(totalSize);
        	CONDITIONAL_REPORT_ERROR(ret, "Overflow");

        	if (ret){
    			// reserve space either for the constant size data or for the string pointers
    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
    			CONDITIONAL_REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
        	}

        	if (ret){
    			// a string need to allocate space for each string and save the pointers
        		if (isString){
        			// I do not need to break the page here and open a new one
        			// as this page will never be reallocated if a string cannot fit in it
        			// each string is instead potentially allocated in its own page
					uint8 **pointerArray = reinterpret_cast<uint8**>(addressOfOutput);
					const CCString *inputs = reinterpret_cast<const CCString *>(inputPointer);

					// numberOfElementsD is good otherwise we would have had an overflow when calculating totalSize
					for (uint32 i = 0;(i < numberOfElementsD.GetData()) && ret;i++){
						if (inputs[i].IsNullPtr()) {
							pointerArray[i] = NULL_PTR(uint8 *);
						} else {
							const CCString &string =  inputs[i];
							uint32 stringSize = string.GetSize() + 1;
							uint8 *ptr;
							// reserve page for a string - potentially create new page
							ret = pageFile.WriteReserveAtomic(ptr,stringSize);
					        CONDITIONAL_REPORT_ERROR(ret, "pageFile.WriteReserve failed");

					        if (ret){
								pointerArray[i] = ptr;
								MemoryOperationsHelper::Copy(ptr,string.GetList(),stringSize);
					        }
						}
					}

        		} else { // not a string
					MemoryOperationsHelper::Copy(addressOfOutput,inputPointer,totalSize);
		        }
			}
        // not the final layer
        // maybe an array or an array pointer or a vector or a zero terminated pointer...
		} else {
			/**
			 * Handling of static arrays
			 * treat simply as a multiplier to next layer
			 * no pointer table allocated here
			 */
			if (outputType == 'A'){
				// increase multiplier
				numberOfElementsD = numberOfElementsD * handler[level].numberOfElements;//multiplier;
				// navigate one side of the tree
				// at the end (case above) consume the whole size of this subtree
				ret = DoCreateR(level+1,inputPointer,addressOfOutput,numberOfElementsD);
				CONDITIONAL_REPORT_ERROR(ret, "DoIndexR failed");
			} else

			/**
			 * Handling of vectors
			 * This is a pointer (or array of) to data, associated with the size
			 */
			// note that ZzDdMmV becomes v in the indexing creation
			if (outputType == 'v'){
				// first time we reach this subtree node
				// calculate total Size needed to store this layer
				// multiply the numberOfElements by each size
				DimensionSize totalSizeD = numberOfElementsD * sizeof(Vector<uint8 *>);
				uint32 totalSize;
				ret = totalSizeD.ToNumber(totalSize);
	        	CONDITIONAL_REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			CONDITIONAL_REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				Vector<uint8 *> *vp = reinterpret_cast<Vector<uint8 *> *>(addressOfOutput);

				switch (inputType){
				case 'v':
				case 'V':{
					const Vector<uint8 *> *vip = reinterpret_cast<const Vector<uint8 *> *>(inputPointer);
					// loop through the collapsed layer
					for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){

						if (vip->GetDataPointer() == NULL_PTR(uint8 *)){
							if (vip->GetNumberOfElements() != 0){
								ret.internalSetupError = true;
				    			REPORT_ERROR(ret, "Vector with size > 0 and NULL ptr");
							}
						} else {
							uint8 *newAddressOfOutput;
							const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(vip->GetDataPointer());
//							ret = RedirectP(newInputPointer,false);
//			    			CONDITIONAL_REPORT_ERROR(ret, "RedirectP failed");
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(vip->GetNumberOfElements()));
				    			CONDITIONAL_REPORT_ERROR(ret, "DoCreateR failed");
							}
							if (ret){
								vp->InitVector(reinterpret_cast<uint8**>(newAddressOfOutput),vip->GetNumberOfElements());
							}
						}
						vp++;
						vip++;
					}
				}break;
				case 'm':
				case 'M':{
					const Matrix<uint8 *> *mip = reinterpret_cast<const Matrix<uint8 *> *>(inputPointer);
					// loop through the collapsed layer
					for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){
						if (mip->GetDataPointer() == NULL_PTR(uint8 *)){
							if (mip->GetNumberOfElements() != 0){
								ret.internalSetupError = true;
				    			REPORT_ERROR(ret, "Matrix with size > 0 and NULL ptr");
							}
						} else {
							uint8 *newAddressOfOutput;
							const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(mip->GetDataPointer());
//							ret = RedirectP(newInputPointer,false);
//			    			CONDITIONAL_REPORT_ERROR(ret, "RedirectP failed");
							if (ret){
								uint32 numberOfElements = mip->GetNumberOfElements();
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(numberOfElements));
				    			CONDITIONAL_REPORT_ERROR(ret, "DoCreateR failed");
								if (ret){
									vp->InitVector(reinterpret_cast<uint8**>(newAddressOfOutput),numberOfElements);
								}
							}
						}
						vp++;
						mip++;
					}
				}break;
				case 'd':
				case 'D':
				case 's':
				case 'S':
				case 'z':
				case 'Z':{
					const uint8 * const *zip = reinterpret_cast<const uint8 * const *>(inputPointer);
					// loop through the collapsed layer
					for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){
						uint8 *newAddressOfOutput;
						const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(zip);
						ret = RedirectP(newInputPointer,true);
		    			CONDITIONAL_REPORT_ERROR(ret, "RedirectP failed");
		    			if (newInputPointer != NULL){
							uint32 numberOfElements= ZeroTerminatedArrayGetSize(/* *zip*/newInputPointer, handler[level+1].elementSize.GetData());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(numberOfElements));
				    			CONDITIONAL_REPORT_ERROR(ret, "DoCreateR failed");
							}
							if (ret){
								vp->InitVector(reinterpret_cast<uint8**>(newAddressOfOutput),numberOfElements);
							}
		    			} else {
							vp->InitVector(reinterpret_cast<uint8**>(NULL),0);
		    			}
						vp++;
						zip++;
					}
				}break;
				default:{
					ret.internalSetupError=true;
	    			CONDITIONAL_REPORT_ERROR(ret, "unexpected case");
				}
				}
			} else


			if (outputType == 'f'){
				// first time we reach this subtree node
				// calculate total Size needed to store this layer
				// multiply the numberOfElements by each size
				DimensionSize totalSizeD = numberOfElementsD * sizeof(uint8 *);
				uint32 totalSize;
				ret = totalSizeD.ToNumber(totalSize);
	        	CONDITIONAL_REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			CONDITIONAL_REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				uint8 **fp = reinterpret_cast<uint8 **>(addressOfOutput);
				const uint8 * const *fip = reinterpret_cast<const uint8 * const *>(inputPointer);
				// loop through the collapsed layer
				for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){
					uint8 *newAddressOfOutput;
					const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(fip);
					ret = RedirectP(newInputPointer,true);
	    			CONDITIONAL_REPORT_ERROR(ret, "RedirectP failed");
	    			if (newInputPointer != NULL){
						if (ret){
							ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,handler[level].elementSize);
			    			CONDITIONAL_REPORT_ERROR(ret, "DoCreateR failed");
						}
						if (ret){
							*fp = newAddressOfOutput;
						}
	    			}
					fp++;
					fip++;
				}
			} else {
				ret.fatalError = true;
    			CONDITIONAL_REPORT_ERROR(ret, "Unmapped outputType");
			}
		}



		return ret;
	}

public:
	/**
	 *
	 */
	DimensionHandler 				handler;

	/**
	 * used to store data
	 */
	MemoryPageFile					pageFile;

	/**
	 * Data is finally encoded as zero term strings
	 */
	bool							isString;


};


// simple algorithm
// without optimisation
ErrorManagement::ErrorType VariableDescriptor::Clone(
		const uint8 *				pointer,
		Reference &					reference
		) const{
	ErrorManagement::ErrorType ret;

	VDCloner cloner(this->modifiers,this->typeDescriptor);

	ret = cloner.Init();
	CONDITIONAL_REPORT_ERROR(ret,"cloner.Init() failed ");

	// copy and create indexes
	if (ret){
		// move it down one level
		uint8 *addressOfOutput = NULL;
		DimensionSize ds(1);
		ret = cloner.DoCreateR(0,pointer,addressOfOutput,ds);
		CONDITIONAL_REPORT_ERROR(ret,"cloner.DoCreateR() failed ");
	}

	// free unused memory
	if (ret){
		ret = cloner.pageFile.CheckAndTrimPage(0);
		CONDITIONAL_REPORT_ERROR(ret,"cloner.pageFile.CheckAndTrimPage() failed ");
	}

	// get start pointer
	uint8 *dataPtr = NULL;
	if (ret){
		dataPtr = cloner.pageFile.Address(0);
	}

	VariableDescriptor vd;
	if (ret){
		DynamicCString mods;
		cloner.handler.GetOutputModifiers(mods);
		TypeDescriptor type = cloner.handler.GetTypeDescriptor();

		if (ret){
			uint32 pageSize = cloner.pageFile.CurrentPageSize();
//printf("pages = %i size = %i\n",page.NumberOfPages(),pageSize);
			if ((cloner.pageFile.NumberOfPages()==1) && (pageSize <= 64) && (false)){
				if (pageSize <= 4){
					ReferenceT<AnyObjectT<4>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						reference = ao;
					}
				} else
				if (pageSize <= 8){
					ReferenceT<AnyObjectT<8>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						reference = ao;
					}
				} else
				if (pageSize <= 16){
					ReferenceT<AnyObjectT<16>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						reference = ao;
					}
				} else
				if (pageSize <= 32){
					ReferenceT<AnyObjectT<32>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						reference = ao;
					}
				} else  {
					ReferenceT<AnyObjectT<64>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						reference = ao;
					}
				}
			} else {
				ReferenceT<MemoryPageObject> mpor;

				mpor = ReferenceT<MemoryPageObject> (buildNow);
				if (!mpor.IsValid()){
					ret.outOfMemory = true;
					REPORT_ERROR(ret,"MemoryPageObject construction failed");
				}

				if (ret){
					mpor->Setup(type,mods,dataPtr,cloner.pageFile);
					reference = mpor;

				}
			}
		}
	}

	return ret;
}


}


	
