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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "VariableDescriptor.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "DynamicCString.h"
#include "MemoryCheck.h"
#include "APLookup.h"
#include "MemoryOperationsHelper.h"

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

#if 0
void VariableDescriptor::AddModifiersLayer(char8 modifier, uint64 size){
	if (modifier != '\0'){
		modifiers.Append(modifier);
		if (size > 0) modifiers.AppendNum(size);
	}
}
#endif

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
		if (size > 0) modifiers.AppendNum(size);
	}
}


void VariableDescriptor::FinaliseCode(TypeDescriptor td){

	bool isConst = typeDescriptor.dataIsConstant;
	typeDescriptor = td;
	if (isConst) typeDescriptor.dataIsConstant = true;
}



VariableDescriptor::VariableDescriptor(){
    typeDescriptor = VoidType;
}

VariableDescriptor::~VariableDescriptor(){
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td){
    typeDescriptor = td;
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor &VariableDescriptor::operator=(const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.Truncate(0);
    modifiers.AppendN(x.modifiers.GetList());
    return *this;
}

static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

static uint32 readNumber(CCString &buffer){
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
 */
static inline void GetLayerInfo(CCString &modifierString,char8 &modifier,uint64 &size ){
	modifier = modifierString[0];
	if (modifier == '\0'){
		size = 0;
	} else {
		modifierString++;
		size = readNumber(modifierString);
	}
}


ErrorManagement::ErrorType VariableDescriptor::FullLayerInfo(
		CCString &modifierString,
		const uint8 *pointer,
		uint64 &numberOfElements,
		uint32 &elementSize,
		uint32 &arrayStringSize,
		uint32 &numberOfTermElements,
		char8 &modifier) const{

	ErrorManagement::ErrorType ret;
    char8 c = '\0';
 	uint64 n = 0;
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
	        REPORT_ERROR(ErrorManagement::InternalSetupError, "pointer=NULL and a ZTA is found. sequence of two ZTA var layers not possible");
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
	} else {
 		while (c == 'A') {
			numberOfElements = numberOfElements * n;
	 		arrayStringSize = (savePtr - modifierString.GetList());
	 		GetLayerInfo(modifierString,c,n);
 		}
		if (c== '\0'){
	 		elementSize = typeDescriptor.Size();
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

uint64 VariableDescriptor::FullLayerSize(CCString modifierString,const uint8 *pointer) const{
	 uint64 numberOfElements;
	 uint32 elementSize;
	 uint32 numberOfTermElements;
	 uint32 temp;
	 char8 modifier;
	 CCString modS = modifierString;

 	 FullLayerInfo(modS,pointer,numberOfElements,elementSize,temp,numberOfTermElements,modifier);
 	 return (numberOfElements + numberOfTermElements)* elementSize ;
}

/**
 * true if the object has a size that depends on the content. (strings)
 */
bool isVariableSize(CCString modifierString){
	const CCString variableSizeModifiers = "VvMmZzDdSs";
	bool isVariable = false;

	if (!modifierString.IsNullPtr()){
		char8 c = modifierString[0];
		while (!isVariable && (c != '\0')){
			isVariable =  (variableSizeModifiers.In(c));
			modifierString++;
			c = modifierString[0];
		}
	}
	return isVariable;
}

TypeDescriptor VariableDescriptor::GetSummaryTypeDescriptor() const {
    CCString modifiersCopy = modifiers;
    char8 firstModifier  = '\0';
    char8 nextModifier  = '\0';
    uint64 size = 0;
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
    		CCString ZTAModifiers = "ZzDdSs";
    		if (ZTAModifiers.In(nextModifier)){
    		    GetLayerInfo(modifiersCopy,firstModifier,size );
    		    nextModifier= modifiersCopy[0];

    		    if ((nextModifier == 0) && ((typeDescriptor == Character8Bit) || (typeDescriptor == ConstCharacter8Bit))){
    		    	td = ConstCharString;
    		    }

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
    		td = InvalidType;
            REPORT_ERROR(ErrorManagement::FatalError, "Incorrect modifier: ZDS not prepended by P ");
    	}
    	}
    }

	return td;
}

ErrorManagement::ErrorType VariableDescriptor::GetDeepSize(CCString modifierString, const uint8 *pointer,
		                               uint64 &dataSize, uint64 &storageSize,uint8 maxDepth,uint32 layerMultiplier) const {

	ErrorManagement::ErrorType ret;

	uint64 numberOfElements;
	uint32 elementSize;
	uint32 temp;
    uint32 numberOfTermElements = 0;
	char8 modifier;

	ret = FullLayerInfo(modifierString,pointer,numberOfElements,elementSize,temp,numberOfTermElements,modifier);
	// account for extra array layer (vector and matrix support)
	numberOfElements = numberOfElements * layerMultiplier;
	/// include terminators
	uint64 numberOfElementsIncludingTerm = numberOfElements  + numberOfTermElements;

	/// if numberOfTermElements > 0  && layerMultiplier > 1 error
	/// means that a sequence VZ is encountered instead of the correct VPZ
	if ((numberOfTermElements > 0) && (layerMultiplier > 1)){
		ret.internalSetupError = true;
        REPORT_ERROR(ErrorManagement::InternalSetupError, "Incorrect sequence VZ or MZ encountered ");
	}

//printf("[%c,%i]",modifier,numberOfTermElements);

	if (ret){
		// if we can delve deep and there is deep....
		if ((maxDepth > 0) && (modifier != '\0')){
			maxDepth--;

			switch(modifier){
			case 'p':
			case 'P':{
				// cannot do this conversion using c++ style casts
				const uint8 **pp = (const uint8 **)(pointer);
				if (true /*isVariableSize(modifierString)*/){
					storageSize = numberOfElementsIncludingTerm * elementSize;
					dataSize = 0;
					uint64 index;
#if 0
					if (!MemoryCheck::Check(pp)){
						ret.parametersError = true;
				        REPORT_ERROR(ErrorManagement::Exception, "bad input pointer");
					}
#endif
					for (index = 0; (index < numberOfElements) && ret; index++){
						const uint8 *p = pp[index];
						if (p != NULL){
							if (!MemoryCheck::Check(p)){
								ret.exception = true;
						        REPORT_ERROR(ErrorManagement::Exception, "bad pointer");
							}

							if (ret){
								uint64 dataSize2;
								uint64 storageSize2;
								ret = GetDeepSize(modifierString, p,dataSize2,storageSize2,maxDepth);

								if (ret){
									storageSize = storageSize + storageSize2;
									dataSize = dataSize + dataSize2 ;
								}
							}
						}
					}
				} else {  // not variable size
					uint64 dataSize2;
					uint64 storageSize2;
					if (!MemoryCheck::Check(pp[0])){
						ret.exception = true;
						REPORT_ERROR(ErrorManagement::Exception, "bad dereferenced pointer");
					}
					ret = GetDeepSize(modifierString, pp[0],dataSize2,storageSize2,maxDepth);
					if (ret){
						storageSize = storageSize2 + numberOfElementsIncludingTerm * elementSize;
						dataSize = numberOfElementsIncludingTerm * dataSize2;
					}
				}
			} break;
			case 'V':
			case 'v':{
				const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);
				storageSize = numberOfElementsIncludingTerm * elementSize;
				dataSize = 0;
				uint64 index;
#if 0

				if (!MemoryCheck::Check(pv)){
					ret.parametersError = true;
			        REPORT_ERROR(ErrorManagement::Exception, "bad input pointer");
				}
#endif

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
								storageSize = storageSize + storageSize2;
								dataSize = dataSize + dataSize2 ;
							}
						}
					}
				}
			} break;
			case 'M':
			case 'm':{
//printf("{M}");
				const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);
				storageSize = numberOfElementsIncludingTerm * elementSize;
				dataSize = 0;
				uint64 index;
#if 0

				if (!MemoryCheck::Check(pm)){
					ret.parametersError = true;
			        REPORT_ERROR(ErrorManagement::Exception, "bad input pointer");
				}
#endif
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
								storageSize = storageSize + storageSize2;
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
			storageSize = 0;
			dataSize = numberOfElementsIncludingTerm * elementSize;
		}
	}
	return ret;
}



ErrorManagement::ErrorType VariableDescriptor::GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *storageSize,uint8  maxDepth) const{

	uint64 storageSz;
	ErrorManagement::ErrorType ret =  GetDeepSize(modifiers,pointer,dataSize,storageSz,maxDepth);

	dataSize += storageSz;
	if (storageSize != NULL){
		*storageSize = storageSz;
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
	uint64 size;
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
	//printf ("{%p->%p}",pointer,p);
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
				uint64 step = layerSize * size;
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
	//printf("{%p-%i-%i-%i}",pointer,index,maxIndex,layerSize);
					ret.outOfRange = true;
			        REPORT_ERROR(ErrorManagement::OutOfRange, "index >= maxIndex");
				}
			}
			if (ret){
				uint64 step = layerSize * size;
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
				uint64 step = layerSize * size;
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
				uint64 step = layerSize * size * pm[0].GetNumberOfColumns();
				pointer = p + step;

				DynamicCString modifiersTemp;
				modifiersTemp.AppendN(modifierString);
				modifiers.Truncate(0);
				modifiers.Append('A');
				modifiers.AppendNum(pm[0].GetNumberOfColumns());
				modifiers.AppendN(modifiersTemp);
			}

	 	}break;
	 	case '\0':{
	 		ret.illegalOperation = true;
	        REPORT_ERROR(ErrorManagement::IllegalOperation, "cannot redirect a basic type");
	 	} break;
	 	default:{
	 		ret.internalSetupError = true;
	        REPORT_ERROR(ErrorManagement::InternalSetupError, "unmapped modifier");
	 	}
	 	}

	}

	return ret;
}

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
    uint32 numberOfTermElements = 0;
	char8 modifier  = '\0';
	uint32 arrayStringSize=0;
	uint64 totalLayerSize=0;

	ret = FullLayerInfo(modifierString,sourcePtr,numberOfElements,elementSize,arrayStringSize,numberOfTermElements,modifier);
    bool nextIsVariableLayer = isVariableSize(modifierString);

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
    	if ( (modifier != '\0') && !((modifier == 'P') && nextIsVariableLayer)){
    		if (totalLayerSize > MaxLayerSize){
    			ret.unsupportedFeature = true;
    	        REPORT_ERROR(ErrorManagement::UnsupportedFeature, "totalLayerSize > MaxLayerSize");
    		}
    	}
    }

    if (ret){
    	// copy all multiplying modifiers A,Z,D,S,z,d,s
	    destVd.modifiers.AppendN(modifierStringSave,arrayStringSize);

		if (((maxDepth == 0) || (modifier == '\0')) && !((modifier == 'P') && (nextIsVariableLayer))){

			// just copy all as is
			MemoryOperationsHelper::Copy(destFreePtr,sourcePtr,totalLayerSize);

		} else {
			maxDepth--;

			switch(modifier){
			case 'P':{
				// cannot do this conversion using c++ style casts
				const uint8 **spp = (const uint8 **)(sourcePtr);
				uint8 **dpp = (uint8 **)(destFreePtr);
				if (nextIsVariableLayer){
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

					ret.fatalError=MemoryOperationsHelper::Set(&dpp[numberOfElements],0,numberOfTermElements*elementSize);

				} else { // next not variable

					uint32 index;
					for (index = 0; (index < numberOfElements) && ret; index++){
						const uint8 *p = spp[index];
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
				} // end if variable layer


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
					dpm[index].Set(destFreePtr,psm[index].GetNumberOfRows(),psm[index].GetNumberOfColumns()) ;
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
				//zero the terminator layer
				ret.fatalError=MemoryOperationsHelper::Set(&dpm[numberOfElements],0,numberOfTermElements*elementSize);

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
					dpv[index].Set(destFreePtr,psm[index].GetNumberOfElements()) ;
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
				ret.fatalError=MemoryOperationsHelper::Set(&dpv[numberOfElements],0,numberOfTermElements*elementSize);

			}break;
			default:{
			}
		    } // end switch
		}
	}// if ret

	return ret;
}

ErrorManagement::ErrorType VariableDescriptor::ToString(DynamicCString &string,bool rawFormat) const{
	ErrorManagement::ErrorType  ret;
	if (rawFormat){
		bool retbool = string.AppendN(modifiers);
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
	uint64 size;
	GetLayerInfo(modifierString,modifier,size );

	if ((modifier == 'P') || (modifier == 'p')){
		// skip P/p layer if followed by one of ZDSzds
		const CCString zeroTerMods="zZsSdD";
		char8 nextModifier = modifierString[0];
		if ( zeroTerMods.In(nextModifier)){
			GetLayerInfo(modifierString,modifier,size );
//printf("{%c}",modifierString[0]);
			if (modifierString[0] == '\0'){
				if ((modifier == 'Z')||(modifier == 'z')){
					if (typeDescriptor == Character8Bit){
						if (modifier == 'z'){
							string.AppendN("const ");
						}
						string.AppendN("CString");
						return ret;
					} else
					if (typeDescriptor == ConstCharacter8Bit){
						if (modifier == 'z'){
							string.AppendN("const ");
						}
						string.AppendN("CCString");
						return ret;
					}
				}
				if ((modifier == 'D')||(modifier == 'd')){
					if (typeDescriptor == Character8Bit){
						if (modifier == 'd'){
							string.AppendN("const ");
						}
						string.AppendN("DynamicCString");
						return ret;
					}
				}
				if ((modifier == 'S')||(modifier == 's')){
					if (typeDescriptor == Character8Bit){
						if (modifier == 'd'){
							string.AppendN("const ");
						}
						string.AppendN("StaticCString<");
						string.AppendNum(size);
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
				string.AppendN(" *");
			} else
			if (modifier == 'p'){
				if (priority == 1){
					priority = 0;
					string.Append('(');
				}
				string.AppendN(" * const");
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
						string.AppendNum(size);
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
				string.AppendN(templateName);
				// insert the type of what follows
				int8 localPriority=0;
				ret = ToStringPrivate(string,modifierString,true,localPriority);
				if (hasSize){
					string.Append(',');
					string.AppendNum(size);
				}
				// close the template
				string.Append('>');
			}
		}
	} // end of reverse action

	return ret;
}


}


	
