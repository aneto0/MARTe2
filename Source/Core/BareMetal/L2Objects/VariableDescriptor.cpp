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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

// just a reasonable number.
const uint32 MaxZTALayerSize = 1024;

void VariableDescriptor::AddModifiersLayer(char8 modifier, uint64 size){
	if (modifier != '\0'){
		modifiers.Append(modifier);
		if (size > 0) modifiers.AppendNum(size);
	}
}

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
//	MoveCodeToModifiers();
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
static inline bool GetLayerInfo(CCString &modifierString,char8 &modifier,uint64 &size ){
	bool ret = true;
	modifier = modifierString[0];
	if (modifier == '\0'){
		ret = false;
		size = 0;
	} else {
		modifierString++;
		size = readNumber(modifierString);
	}
	return ret;
}

#if 0
bool VariableDescriptor::GetTopTypeDescriptor(TypeDescriptor &td, uint32 depth) const {
    CCString modifiersCopy = modifiers;
    char8 modifier;
    uint64 size;

    GetLayerInfo(modifiersCopy,modifier,size );
    while ((depth > 0) && (modifier != '\0')){
    	GetLayerInfo(modifiersCopy,modifier,size );
    	depth--;
    }

    td = DelegatedType;

    if (modifier != '\0'){
    	//printf("[%c]",token);
   		CombinedArrayType cat = reverseLookUpArrayTypeGen(modifier, &APLookupTable1[0]);
   		if (cat != ArrayUnknown){
   			td.arraySize = size;
   			td.arrayProperty = cat;
		} else {
			cat = reverseLookUpArrayTypeGen(modifier, &APLookupTable2[0]);
   			td.combinedArrayType = cat;
		}
    } else {
    	if (depth == 0){
    		td = typeDescriptor;
    		modifier = ' '; // force true
    	}
    	//return  GetFullTypeDescriptor();
    }
	return (modifier != '\0');
}

bool VariableDescriptor::GetModifiersLayer(char8 &modifier,uint64 &size,bool remove){
	bool ret = false;

    CCString modifiersCopy  = modifiers;
    ret = GetLayerInfo(modifiersCopy,modifier,size );
    if (ret && remove){
    	uint32 step = (uint32)(modifiersCopy.GetList() - modifiers.GetList());
    	ret = modifiers.Remove(step);
    }

    return ret;
}
#endif

ErrorManagement::ErrorType VariableDescriptor::FullLayerInfo(
		CCString &modifierString,const uint8 *pointer,
		uint64 &numberOfElements,uint32 &elementSize,
		uint32 &arrayStringSize,uint32 &numberOfTermElements,
		char8 &modifier) const{

	ErrorManagement::ErrorType ret;
    char8 c = '\0';
 	uint64 n = 0;
 	numberOfElements = 1;
 	// to allow recovering size of array section
 	arrayStringSize = 0;
 	numberOfTermElements = 0;

 	char8 * savePtr = modifierString.GetList();

 	const CCString variableLayers = "zZdDsS";

 	GetLayerInfo(modifierString,c,n);

 	// only first layer may be a variable size one
 	// between variable size there must be a pointer of sort
	if (variableLayers.In(c)) {
		ret.internalSetupError = (pointer == NULL);

		uint64 layerSize  = 0;
		if (ret){
	 		uint32 ass = (savePtr - modifierString.GetList());
	 		uint32 temp;
	 		// pointer is set to NULL to communicate the fact that a variable layer
			ret = FullLayerInfo(modifierString,NULL,numberOfElements,elementSize,arrayStringSize,temp,modifier);
			ass += arrayStringSize;
			numberOfTermElements = numberOfElements;
			layerSize = numberOfElements*elementSize;
			ret.unsupportedFeature = (layerSize > MaxZTALayerSize);
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
	ret.internalSetupError = (numberOfTermElements > 0) && (layerMultiplier > 1);
	/// TODO add Error message

	if (ret){
		// if we can delve deep and there is deep....
		if ((maxDepth > 0) && (modifier != '\0')){
			maxDepth--;

			if (isVariableSize(modifierString)){

				switch(modifier){
				case 'p':
				case 'P':{
					// cannot do this conversion using c++ style casts
					const uint8 **pp = (const uint8 **)(pointer);
					storageSize = numberOfElementsIncludingTerm * elementSize;
					dataSize = 0;
					uint64 index;
					for (index = 0; (index < numberOfElements) && ret; index++){
						const uint8 *p = pp[index];
						ret.exception = !MemoryCheck::Check(p);
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
				} break;
				case 'V':
				case 'v':{
					const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);
					storageSize = numberOfElementsIncludingTerm * elementSize;
					dataSize = 0;
					uint64 index;
					for (index = 0; (index < numberOfElements) && ret; index++){
						const uint8 *p = pv[index].GetDataPointer();
						uint32 numberOfArrayElements = pv[index].GetNumberOfElements();
						ret.exception = !MemoryCheck::Check(p);
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
				} break;
				case 'M':
				case 'm':{
					const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);
					storageSize = numberOfElementsIncludingTerm * elementSize;
					dataSize = 0;
					uint64 index;
					for (index = 0; (index < numberOfElements) && ret; index++){
						const uint8 *p = pm[index].GetDataPointer();
						uint32 numberOfArrayElements = pm[index].GetNumberOfRows() * pm[index].GetNumberOfColumns();
						ret.exception = !MemoryCheck::Check(p);
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
				} break;
				default:{
					// Aa, Zz, Ss, Dd are not terminators of a layer .
					// they shall not be encountered here
					ret.fatalError = true;
					// TODO produce error message
				}
				}



			} else { // fixed size
				uint64 dataSize2;
				uint64 storageSize2;
				const uint8 **pp = (const uint8 **)(pointer);
				ret.exception = !MemoryCheck::Check(pp[0]);
				ret = GetDeepSize(modifierString, pp[0],dataSize2,storageSize2,maxDepth);

				if (ret){
					storageSize = storageSize2 + numberOfElementsIncludingTerm * elementSize;
					dataSize = numberOfElementsIncludingTerm * dataSize2;
				}
			}
		} else {  // '\0' or maxdepth reached
			storageSize = 0;
			dataSize = numberOfElementsIncludingTerm * elementSize;
		}
	}
	return ret;
}



#if 0
// returns size of layer, and number of elements
// does not Redirect
 void VariableDescriptor::ExamineLayer(
		CCString &modifierString,
		uint64 &nOfElements,
		uint64 &elementSize,
		const uint8 *pointer
		) const {
	char8 code  = '\0';
	uint64 number;

	// this consumes  modifierString
	// just reads the letter and the number
	// cannot simply calculate get the layersize
	// as I potentially need to compute the size of a sublayer for each element of the array
	GetLayerInfo(modifierString,code,number);

	if (code == '\0') {
		elementSize = typeDescriptor.Size();
		nOfElements = 0;
	} else
	if ((code == 'P') || (code == 'p')){
		elementSize = sizeof(void *);
		nOfElements = 0;
	} else
	if ((code == 'V') || (code == 'v')){
		elementSize = sizeof(Vector<char8>);
		nOfElements = 0;
	} else
	if ((code == 'M') || (code == 'm')){
		elementSize = sizeof(Matrix<char8>);
		nOfElements = 0;
	} else
	if ((code == 'D') || (code == 'd') ||
	    (code == 'S') || (code == 's') ||
	    (code == 'Z') || (code == 'z')){
		elementSize = sizeof(ZeroTerminatedArray<char8>);
		nOfElements = 0;
	} else
	if (code == 'A') {
		nOfElements = number;
		elementSize = FullLayerSize(modifierString,pointer);
	}
}


ErrorManagement::ErrorType VariableDescriptor::ExamineAndRedirect(
		CCString &modifierString,
		const uint8 *&pointer,
		uint64 &nOfElements,
		uint64 &storageSize,
		uint64 &size
		) const {
	ErrorManagement::ErrorType ret;

	char8 code  = '\0';
	uint64 number;

	// this consumes  modifierString
	// just reads the letter and the number
	// cannot simply calculate get the layersize
	// as I potentially need to compute the size of a sublayer for each element of the array
	GetLayerInfo(modifierString,code,number);

	if (code == '\0') {
		switch (typeDescriptor.all){
		case StaticCharString_number:
		case DynamicCharString_number:
		case ConstCharString_number:
		case CharString_number: {
			const CCString string = * (reinterpret_cast<CCString const *>(pointer));
			ret.exception = !MemoryCheck::Check(string.GetList());
			if (ret){
				pointer = reinterpret_cast<const uint8 *>(string.GetList());
//				size = 1;
//				nOfElements = string.GetSize() + 1;
				size = string.GetSize() + 1;
				nOfElements = 0;  // need to terminate here
			}
			storageSize = sizeof(CCString);
		} break;//case
		default:{
			size = typeDescriptor.Size();
			nOfElements = 0;
			storageSize = 0;
		}

		}// switch


	} else
	if ((code == 'P') || (code == 'p')){

		// allows browsing past pointers assuming one element only
		storageSize = sizeof(void *);
		nOfElements = 1;
		size = FullLayerSize(modifierString);

		// cannot do this conversion using c++ style casts
		const uint8 **pp = (const uint8 **)(pointer);
		ret.exception = !MemoryCheck::Check(pp);
		if (ret){
			pointer = *pp;
		}

	} else
	if ((code == 'V') || (code == 'v')){

		size = FullLayerSize(modifierString);
		storageSize = sizeof (Vector<char8>);
		const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(pointer);

		ret.exception = !MemoryCheck::Check(pv);
		if (ret){
			pointer = static_cast<uint8 *>(pv->GetDataPointer());
			nOfElements = pv->GetNumberOfElements();
		}

	} else
	if ((code == 'M') || (code == 'm')){
		size = FullLayerSize(modifierString);
		storageSize = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(pointer);

		ret.exception = !MemoryCheck::Check(pm);
		if (ret){
			pointer = static_cast<uint8 *>(pm->GetDataPointer());
			nOfElements = pm->GetNumberOfColumns()*pm->GetNumberOfRows();
		}

	} else
	if ((code == 'D') || (code == 'd') ||
	    (code == 'S') || (code == 's') ||
	    (code == 'Z') || (code == 'z')){
		size = FullLayerSize(modifierString);
		storageSize = sizeof (ZeroTerminatedArray<char8>);

		ret.exception = !MemoryCheck::Check(pointer);
		if (ret){
			nOfElements = ZeroTerminatedArrayGetSize(pointer,size);
			pointer = *((uint8 **)pointer);
		}
	} else
	if (code == 'A') {
		storageSize = 0;
		size = FullLayerSize(modifierString);
		nOfElements = number;
	}

	return ret;
}

ErrorManagement::ErrorType VariableDescriptor::GetDeepSize(CCString modifierString, const uint8 *pointer,
		                               uint64 &dataSize, uint64 &storageSize,uint8 maxDepth) const {

	ErrorManagement::ErrorType ret;
	uint64 nOfElements = 0;
	uint64 size = 0;
	uint64 storageSz = 0;
//printf ("{mod=%s}",modifierString);

	if (maxDepth > 0){
		// will modify pointer
		// will return nOfElements > 0 in case of arrays or of pointers
		// size is the nextLayerSize in case of arrays/pointers
		// size is the elementSize in all other cases
		// storageSize2 is not zero in case of indirection
		ret = ExamineAndRedirect(modifierString,pointer,nOfElements,storageSz,size);

		if (storageSz > 0){
			maxDepth--;
		}
	} else {
		// will not modify pointer
		// will return nOfElements > 0 only in case of arrays
		// size is the nextLayerSize in case of arrays
		// size is the elementSize in all other cases
		ExamineLayer(modifierString,nOfElements,size);
	}
//printf ("{ne=%Li sz=%Li ssz=%Li mod=%s}",nOfElements,size,storageSz,modifierString);
	if (ret){
		if (nOfElements > 0){
//printf("->");

			if (isVariableSize(modifierString)){
				dataSize = 0;
				const uint8 *ptr = pointer;
				for (uint64 i = 0;i<nOfElements;i++){
//printf("(%Li)",i);
					uint64 dataSize2;
					uint64 storageSize2;
					GetDeepSize(modifierString, ptr,dataSize2,storageSize2,maxDepth) ;
					dataSize += dataSize2;
					storageSz += storageSize2;
					ptr += size;
				}
				storageSz += size; // the terminator

			} else {
				uint64 dataSize2;
				uint64 storageSize2;
				GetDeepSize(modifierString, pointer,dataSize2,storageSize2,maxDepth) ;
				dataSize = nOfElements * dataSize2;
				storageSz = storageSz + (nOfElements * storageSize2);
				storageSz += size; // the terminator
			}
			storageSize = storageSz;
		} else {
			dataSize = size;
			storageSize = storageSz;
		}


	}
	return ret;
}
#endif

#if 0
/**
 * @brief removes one indirection layer and update variable pointer
 * @param[in out] pointer, the pointer to the variable
 * @param[in] index the offset
 * @return true if all ok or the error
 */
ErrorManagement::ErrorType VariableDescriptor::Redirect(const uint8 *&pointer,uint32 index){
	ErrorManagement::ErrorType ret;
	uint64 nOfElements = 0;
	uint64 size = 0;
	uint64 storageSz = 0;
	CCString modifierString = modifiers;
	// will modify pointer
	// will return nOfElements > 0 in case of arrays or of pointers
	// size is the nextLayerSize in case of arrays/pointers
	// size is the elementSize in all other cases
	// storageSize2 is not zero in case of indirection
	ret = ExamineAndRedirect(modifierString,pointer,nOfElements,storageSz,size);

	if (ret){
		ret.outOfRange = (index >= nOfElements);
	}

	if (ret){
		pointer = pointer + index * size;
		uint32 step = (modifierString.GetList() - modifiers.GetList());
		modifiers.Remove(step);
	}
	return ret;
}
#endif


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
 	GetLayerInfo(modifierString,modifier,size);


 	switch (modifier){
 	case 'p':
 	case 'P':{
		const uint8 **pp = (const uint8 **)(pointer);
		const uint8 *p = pp[index];
		ret.exception = !MemoryCheck::Check(p);

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
				if (index == 0){
					pointer = p;
				} else {
					ret.outOfRange = true;
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
		ret.unsupportedFeature = (layerSize > MaxZTALayerSize);
		if (ret){
			uint32 maxIndex  = ZeroTerminatedArrayGetSize(pointer, layerSize);
			ret.outOfRange = (index > maxIndex);
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
		const uint8 *p = pv[0].GetDataPointer();
		uint32 numberOfArrayElements = pv[0].GetNumberOfElements();
		ret.exception = !MemoryCheck::Check(p);
		ret.outOfRange = (index >  numberOfArrayElements);

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
		const uint8 *p = pm[0].GetDataPointer();
		ret.exception = !MemoryCheck::Check(p);
		ret.outOfRange = (index > pm[0].GetNumberOfRows());
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


 	default:{
 		ret.internalSetupError = true;
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
		const uint8 *sourcePtr,uint8 *&destFreePtr,uint64 &destFreeSize,
		VariableDescriptor &destVd,uint8 maxDepth,CCString modifierString,
		bool dereferenceCompound,uint8 *destPtr) const {
	ErrorManagement::ErrorType ret;
// examine layer ending at a pointer or at final


	char8 modifier  = '\0';
	uint64 number;
	// number of elements in this layer
    uint64 numberOfElements =1;
    uint32 elementSize = 0;
    CCString modifierStringSave = modifierString;
    // returns number of characters used to encode array layers
    uint32 appendSize = FullLayerInfo(modifierString,numberOfElements,elementSize,modifier);
    destVd.modifiers.AppendN(modifierStringSave,appendSize);

    bool nextIsVariableLayer = isVariableSize(modifierString);

	// proceed recursively
	uint64 toCopy = 0;
	uint64 toReserve = 0;

	switch(modifier){
	case 'P':{
		if (maxDepth > 0){
			maxDepth--;

			if (nextIsVariableLayer){
				// reserve pointer space


				//loop through pointers
				// copy
				// update pointer space


			} else {
				//skip this layer


			}


			// skip this layer. just move pointer
			// cannot do this conversion using c++ style casts
			const uint8 **pp = (const uint8 **)(sourcePtr);
			ret.exception = !MemoryCheck::Check(pp);
			if (ret){
				sourcePtr = *pp;
			}
		} else {
			// copy the pointer and end here
			toCopy = sizeof(void *) * numberOfElements;
		}

	}break;
	case 'm':
	case 'M':{
		modifier = 'm';
		if (!destVd.InsertModifiersLayer(modifier,0)){
			ret.fatalError = true;
		}

		// reserve space for matrices

		//loop through pointers
		// copy
		// update pointer space






		// does not consume modifierString
		nextLayerSize = FullLayerSize(modifierString);
//			storageSize = sizeof (Matrix<char8>);
		const Matrix<char8> *psm = reinterpret_cast<const Matrix<char8> *>(sourcePtr);

		ret.exception = !MemoryCheck::Check(pm);
		if (ret){
			sourcePtr = static_cast<uint8 *>(psm->GetDataPointer());
			nOfElements = psm->GetNumberOfColumns()*psm->GetNumberOfRows();

			Matrix<char> *pdm = reinterpret_cast<const Matrix<char8> *>(destPtr);
			pdm->Set()

		}
		destPtr +


	}break;
	case 'v':
	case 'V':{
		modifier = 'v';
	}break;
	case 's':
	case 'd':
	case 'z':
	case 'S':
	case 'D':
	case 'Z':{
		modifier = 'z';
		size = 0;
	}break;
	}


		// copy toCopy  bytes









	return ret;

}

#if 0
/**
 * @brief copies the variable layer by layer. The copied layer is implemented in contiguous memory
 * @param[in] sourcePtr, the pointer to the variable to be copied
 * @param[in,out] destPtr, pointer to the memory area to copy the variable to. Returns the pointer to the unused area
 * @param[in,out] destSize, length of the memory area to copy to. Returns the unused area size
 * @param[out] destVd, the variable descriptor of the used area. Note that all varieties of ZeroTermarrays become ZeroTermArray<const T>
 * @param[in] maxDepth, the max number of pointer redirection to include in the copy
 * @return true if all ok or the error
 */
ErrorManagement::ErrorType VariableDescriptor::Copy(
		const uint8 *sourcePtr,uint8 *&destPtr,uint64 &destSize,VariableDescriptor *destVd,uint8 maxDepth) const {

	ErrorManagement::ErrorType ret;

	//check destPtr an destSize

	ret.parametersError = (destPtr == NULL_PTR(uint8 *));
	if (ret){
		ret.parametersError = (destSize == 0);
	}

	//prepare destVd only if destVd not null
	if (ret && (destVd != NULL_PTR(VariableDescriptor))){
		destVd->modifiers.Truncate(0);
		destVd->typeDescriptor = typeDescriptor;

		CCString modifierString = modifiers;
		char8 modifier;
		uint64 size;
		while(GetLayerInfo(modifierString,modifier,size )){
			switch(modifier){
			case 'P':{
			}break;
			case 'm':
			case 'M':{
				modifier = 'm';
			}break;
			case 'v':
			case 'V':{
				modifier = 'v';
			}break;
			case 's':
			case 'd':
			case 'z':
			case 'S':
			case 'D':
			case 'Z':{
				modifier = 'z';
				size = 0;
			}break;
			}
			if (!destVd->InsertModifiersLayer(modifier,size)){
				ret.fatalError = true;
			}
		}
	}

	// proceed recursively
	if (ret){
		CCString modifierString = modifiers;
		char8 modifier;
		uint64 size;
		uint64 nextLayerSize;

		// this consumes  modifierString
		// just reads the letter and the number
		// cannot simply calculate get the layersize
		// as I potentially need to compute the size of a sublayer for each element of the array
		GetLayerInfo(modifierString,modifier,size);

		uint64 toCopy = 0;

		switch(modifier){
		case 'P':{
			if (maxDepth > 0){
				maxDepth--;
				// skip this layer. just move pointer
				// cannot do this conversion using c++ style casts
				const uint8 **pp = (const uint8 **)(sourcePtr);
				ret.exception = !MemoryCheck::Check(pp);
				if (ret){
					sourcePtr = *pp;
				}
			} else {
				// copy the pointer and end here
				toCopy = sizeof(void *);
			}

		}break;
		case 'm':
		case 'M':{
			modifier = 'm';

			// does not consume modifierString
			nextLayerSize = FullLayerSize(modifierString);
//			storageSize = sizeof (Matrix<char8>);
			const Matrix<char8> *psm = reinterpret_cast<const Matrix<char8> *>(sourcePtr);

			ret.exception = !MemoryCheck::Check(pm);
			if (ret){
				sourcePtr = static_cast<uint8 *>(psm->GetDataPointer());
				nOfElements = psm->GetNumberOfColumns()*psm->GetNumberOfRows();

				Matrix<char> *pdm = reinterpret_cast<const Matrix<char8> *>(destPtr);
				pdm->Set()

			}
			destPtr +


		}break;
		case 'v':
		case 'V':{
			modifier = 'v';
		}break;
		case 's':
		case 'd':
		case 'z':
		case 'S':
		case 'D':
		case 'Z':{
			modifier = 'z';
			size = 0;
		}break;
		}


		// copy toCopy  bytes





	}





return ret;
}
#endif

#if 0
ErrorManagement::ErrorType VariableDescriptor::Copy(
		const uint8 *sourcePtr,
		VariableDescriptor &destination,
		uint8 *destPtr,
		uint64 destSize) const {

	ErrorManagement::ErrorType ret;

	//check destPtr an destSize

	ret.parametersError = (destPtr == NULL_PTR(uint8 *));
	if (ret){
		ret.parametersError = (destSize == 0);
	}


	if (ret){
		//prepare destination
		destination.modifiers.Truncate(0);
		destination.typeDescriptor = typeDescriptor;

		CCString modifierString = modifiers;
		char8 modifier;
		uint64 size;
		while(GetLayerInfo(modifierString,modifier,size )){
			switch(modifier){
			case 'P':{
			}break;
			case 'm':
			case 'M':{
				if (!destination.InsertModifiersLayer('z',0)){
					ret.fatalError = true;
				}
				modifier = 'z';
			}break;
			case 'v':
			case 's':
			case 'd':
			case 'z':
			case 'V':
			case 'S':
			case 'D':
			case 'Z':{
			}break;
				modifier = 'z';
				size = 0;
			}
			if (!destination.InsertModifiersLayer(modifier,size)){
				ret.fatalError = true;
			}
		}
	}

	// recursively copy data and prepare pointers
	if (ret){
		CCString modifierString = modifiers;
		uint64 nOfElements = 0;
		uint64 size = 0;
		uint64 storageSz = 0;
		const uint8 *pointer = sourcePtr;
		// will modify pointer
		// will return nOfElements > 0 in case of arrays or of pointers
		// size is the nextLayerSize in case of arrays/pointers
		// size is the elementSize in all other cases
		// storageSize2 is not zero in case of indirection
		char8 token = modifierString[0];
		ret = ExamineAndRedirect(modifierString,pointer,nOfElements,storageSz,size);

		if (ret){
			//  there is a next level
			if (nOfElements > 0){
				// allocate space for this layer


				// and I need to copy block by block
				if (isVariableSize(modifierString)){
					const uint8 *ptr = pointer;
					for (uint64 i = 0;i<nOfElements;i++){
	//printf("(%Li)",i);
						uint64 dataSize2;
						uint64 storageSize2;
						GetDeepSize(modifierString, ptr,dataSize2,storageSize2,maxDepth) ;
						dataSize += dataSize2;
						storageSz += storageSize2;
						ptr += size;
					}
					storageSz += size; // the terminator

				} else {
					uint64 dataSize2;
					uint64 storageSize2;
					GetDeepSize(modifierString, pointer,dataSize2,storageSize2,maxDepth) ;
					dataSize = nOfElements * dataSize2;
					storageSz = storageSz + (nOfElements * storageSize2);
					storageSz += size; // the terminator
				}
				storageSize = storageSz;
			} else {
				dataSize = size;
				storageSize = storageSz;
			}


		}





	}



	return ret;
}
#endif




bool VariableDescriptor::BrowseModifiersLayer(char8 &modifier,uint64 &size,uint32 layerNo)const{
	bool ret = true;
    char8 token ;

    CCString modifiersCopy = modifiers;
    layerNo++;
	while(ret && (layerNo > 0)){
		ret = GetLayerInfo(modifiersCopy,modifier,size);
		layerNo--;
	}
    return ret;
}


static bool BrowseModifiersLayerR( char8 &modifier,uint64 &size,CCString modifiers,int32 targetLayer,int32 &reverseDepth) {
	bool ret = true;
	bool isArray = false;
	ret = GetLayerInfo(modifiers,modifier,size);

//printf("{%c|%i}%i",modifier,size,targetLayer);

	if (((modifier == 'A') || (modifier == 'a')) && ret){
		isArray = true;
		// remember the target at the beginning of a sequence of A/a
		if (reverseDepth < 0){
			reverseDepth = targetLayer;
		}
	} else {
		// end of sequence of array and not yet found target
		if (targetLayer >= 0){
			reverseDepth = -1;
		}
	}

		if ((isArray) || (targetLayer > 0)){
			char8 modifier2;
			uint64 size2;
			targetLayer--;
//printf("@");
			bool ret2 = BrowseModifiersLayerR( modifier2,size2,modifiers,targetLayer,reverseDepth);
//if (ret2)printf("!");

			// skip results until reverse layers consumed
			if (reverseDepth >= 0){
//printf(">");
				reverseDepth--;
			} else {
//printf("<");
				modifier 	= modifier2;
				size 		= size2;
				ret 		= ret2;
				reverseDepth = -1;
//printf("(%c|%i)%i",modifier,size,targetLayer);
			}
	}

	return ret;
}

// TODO fix ==> uint32 (* )[32] written as  uint32[32] *
bool VariableDescriptor::ToString(DynamicCString &string)const{
	bool ret=true;
	int32 maxLayer =0;
	char8 modifier;
	uint64 size;
	int32 rd = -1;
	// first loop add all prefix modifiers that encapsulate: Vector<  Matrix<
	while ( BrowseModifiersLayerR(modifier,size,modifiers.GetList(),maxLayer,rd) && ret){
//printf("[%c-%x|%i]",modifier,modifier,size);
		const APLookUp *apl = reverseLookUpCode(modifier);
		if (apl != NULL){
			string.AppendN(apl->cExpansionPre);
		} else {
			ret = false;
		}
		maxLayer++;
		rd = -1;
	}
	maxLayer--;

	if (ret) {
		ret = typeDescriptor.ToString(string);
	}

	// second loop close encapsulating modifiers and add postfix ones> * [N]
	while ((maxLayer>=0)  && ret){
		rd = -1;
		ret = BrowseModifiersLayerR(modifier,size,modifiers.GetList(),maxLayer,rd);

		if (ret){
			const APLookUp *apl = reverseLookUpCode(modifier);
			if (apl != NULL){
				if (apl->cExpansionPost.GetList() != NULL ){
					string.AppendN(apl->cExpansionPost);
					string.AppendNum(size);
				}
				string.AppendN(apl->cExpansionEnd);
			} else {
				ret = false;
			}
		}
		maxLayer--;
	}

	return ret;

}




}


	
