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
#include "MemoryPageFile.h"
#include "ReferenceT.h"
#include "AnyObjectT.h"
#include "MemoryPageObject.h"
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

static inline uint32 Type2Size(char8 c,const TypeDescriptor &tdIn) {
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

static inline bool IsUpperCase(char8 c){
	bool ret = true;
	if ((c >= 'A')  && (c <= 'Z')) {
		ret = false;
	}
	return ret;
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

static inline ErrorManagement::ErrorType RedirectP(const uint8* &ptr,bool allowNULL= false){
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

/*****************************************************************************************************************************/
/*                  FullLayerInfo FullLayerSize                                                                              */
/*                  Used by Redirect --- to be obsoleted                                                                     */
/*****************************************************************************************************************************/

/**
 * @brief calculate properties of a full layer - n of array alements * elementsize
 * @details the layers start with a fixed/variable modifier and groups all following fixed modifiers
 * @details this does not calculate size. Only used to speedup operations
 * @param[in,out] modifierString, the string of variable modifiers
 * @param[out] numberOfElements is the number of elements in this layer
 * @param[out] elementSize is the storage space occupied by an element - not the data it contains.
 * @param[out] overheadSize is the size not used for data content
 * @param[out] arrayStringSize number of characters in modifierString consumed in arrays 'a' layers
 * @param[out] numberOfTermElements number of elements of size elementSize used as terminator in a ZTA
 * @param[out] modifier is the last code that terminated this scan (0 for the end of modifiers or one of PVNZSDpvnzsd
 */
static ErrorManagement::ErrorType FullLayerInfo(
		CCString &modifierString,
		const TypeDescriptor &td,
		const uint8 *pointer,
		uint64 &numberOfElements,
		uint32 &elementSize,
		uint32 &arrayStringSize,
		uint32 &numberOfTermElements,
		char8  &modifier) {

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
			ret = FullLayerInfo(modifierString,td,NULL,numberOfElements,elementSize,arrayStringSize,temp,modifier);
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
			elementSize = td.StorageSize();
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

/**
 * @brief calculate size of a full layer - n of array alements * elementsize
 * @param[in] modifierString, the string of variable modifiers
 * @return the full size of the memory necessary to store this layer
 */
static uint64 FullLayerSize(CCString modifierString,const TypeDescriptor &td,const uint8 *pointer) {
	 uint64 numberOfElements = 0;
	 uint32 elementSize = 0;
	 uint32 numberOfTermElements = 0;
	 uint32 temp;
	 char8 modifier = '\0';
	 CCString modS = modifierString;

 	 FullLayerInfo(modS,td,pointer,numberOfElements,elementSize,temp,numberOfTermElements,modifier);
 	 return (numberOfElements + numberOfTermElements)* elementSize ;
}


/***************************************************************************************************************/
/*               Redirect                                                                                      */
/*               Uses GetLayerInfo                                                                             */
/*               Uses FullLayerSize                                                                            */
/***************************************************************************************************************/
// TODO rewrite using DimensionInfo

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
				uint64 layerSize = FullLayerSize(modifierString,typeDescriptor,pointer);
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
			uint64 layerSize = FullLayerSize(modifierString,typeDescriptor,pointer);
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
				uint64 layerSize = FullLayerSize(modifierString,typeDescriptor,pointer);
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
				uint64 layerSize = FullLayerSize(modifierString,typeDescriptor,pointer);
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



/******************************************************************************************************************************/
/*                       DimensionInfo 																					      */
/*                       Structure to help process modifiers                                                                  */
/******************************************************************************************************************************/

/**
 * use a SaturatedInteger<uint32> to describes number of elements in the structures
 */
typedef  SaturatedInteger<uint32> DimensionSize;
/**
 * @brief description of a dimension.
 */
struct DimensionInfoElement{
	/**
	 * @brief A for arrays F/f for pointers to arrays, ZSDzsd for pointers to ZTA, MVmv for matrix & vectors /0 final
	 */
	char8  type;

	/**
	 * @brief number of elements in this dimension
	 * DynamicSize for variable size (ZSDMV)
	 */
	DimensionSize numberOfElements;

	/**
	 * @brief product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 */
	DimensionSize elementSize;

	/**
	 * @brief constructor
	 */
	DimensionInfoElement(char8 typeIn,const DimensionSize &numberOfElementsIn){
		type = typeIn;
		numberOfElements = numberOfElementsIn;
		elementSize = 0;
	}

};

/**
 * A more expanded and useful version of VariableDescriptor
 */
class DimensionHandler: protected StaticList<DimensionInfoElement>{
public:
	/**
	 * @brief prepare a table of DimensionInfoElement
	 * Stops modifier parsing when a pure pointer is encountered: P\0 or PP or PAAA\0 or PV or PM
	 */
	DimensionHandler(CCString modifiers,TypeDescriptor tdIn);

	/**
	 * Allow read only access to all DimensionInfoElement
	 */
	inline const DimensionInfoElement &operator[](uint32 index) const ;

	/**
	 * Allow access to all DimensionInfoElement
	 */
	inline DimensionInfoElement &Access(uint32 index) ;

	/**
	 * How many DimensionInfoElement
	 */
	inline uint32 				NDimensions() const ;

	/**
	 * access type
	 * This is either the original tdIn or a synthetic pointer type
	 */
	inline TypeDescriptor 		GetTypeDescriptor() const ;


private:

	/**
	 *	@brief constructor
	 */
	DimensionHandler(const DimensionHandler &){}

	/**
	 * @brief copy operator
	 */
	void operator=(const DimensionHandler &){}

	/**
	 * @Brief Normally a copy of the TdIn used in initialisation. It can become a Pointer in case a pointer modifier is encountered
	 *
	 */
	TypeDescriptor td;

};

TypeDescriptor DimensionHandler::GetTypeDescriptor() const{
	return td;
}

uint32 DimensionHandler::NDimensions() const {
	return GetSize();
}

inline const DimensionInfoElement &DimensionHandler::operator[](uint32 index) const{
	return StaticList<DimensionInfoElement>::operator[](index);
}

inline  DimensionInfoElement &DimensionHandler::Access(uint32 index) {
	return StaticList<DimensionInfoElement>::Access(index);
}

DimensionHandler::DimensionHandler(CCString modifiers,TypeDescriptor tdIn){
	td = tdIn;
	const CCString terminals = "VMvm";
	const CCString Zterminals = "ZDSzds";
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	char8 pointer = '\0';
	bool finished = false;
	while (!finished){
		if (modifier == '\0'){
			if (pointer != '\0') {
				td = PointerType;
				if (!IsUpperCase(pointer)){
					td.dataIsConstant = true;
				}
			}
			this->Add(DimensionInfoElement('\0',1));
			finished = true;
		} else
		if (modifier == 'A'){
			if (pointer == 'P'){
				this->Add(DimensionInfoElement('F',size));
			} else
			if (pointer == 'p'){
				this->Add(DimensionInfoElement('f',size));
			} else {
				this->Add(DimensionInfoElement('A',size));
			}
			pointer = '\0';
		} else {
			if ((modifier == 'P') || (modifier == 'p')){
				// PP double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					if (!IsUpperCase(pointer)){
						td.dataIsConstant = true;
					}
					this->Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
					pointer = modifier;
				}
			} else
			if (terminals.In(modifier)){
				// PV PM double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					this->Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
					this->Add(DimensionInfoElement(modifier,SaturatedInteger<uint32>::Indeterminate()));
				}
			} else
			if (Zterminals.In(modifier)){
				// only PZ PD PS are allowed
				if (pointer != '\0') {
					pointer = '\0';
					this->Add(DimensionInfoElement(modifier,SaturatedInteger<uint32>::Indeterminate()));
				} else {
					td = InvalidType(0);
				}
			}

		}
		if (!finished){
			GetLayerInfo(modifiers,modifier,size);
		}
	}

	DimensionSize elementSize = 1;
	for (int i = (NDimensions()-1); i >= 0; i--){
		if ((*this)[i].type != 'A'){
			elementSize = Type2Size((*this)[i].type,td);
		} else {
			elementSize = elementSize * (*this)[i].numberOfElements ;
		}
		this->Access(i).elementSize = elementSize;

	}
}

static ErrorManagement::ErrorType UpdatePointerAndSize(
			DimensionHandler &dh,
			uint32 			layerIndex,
			const uint8 *&	ptr,
			uint32 &		numberOfElementsIO,
			uint32 &		nextElementSize,
			uint32 &		overHead)  {
	ErrorManagement::ErrorType ok;

	const DimensionInfoElement &diNext = dh[layerIndex+1];
	DimensionInfoElement &di = dh.Access(layerIndex);

	uint8 type = di.type;
	bool isZta = false;
	bool allowNULL = false;
	bool updateNumberOfElements = false;
	overHead = 0;

	DimensionSize numberOfElements(0);
	DimensionSize nextElementSizeD(0);

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
		if (numberOfElements == DimensionSize(0U)){
			allowNULL = true;
		}
		updateNumberOfElements = true;
	}break;
	case 'm':
	case 'M':{
		overHead = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
		numberOfElements = pm->GetNumberOfElements();
		if (numberOfElements == DimensionSize(0U)){
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
		ok = RedirectP(ptr,allowNULL);
		if (!ok){
			DynamicCString errM;
			errM.Append("type ");
			errM.Append(type);
			errM.Append(" redirection failed ");
	        REPORT_ERROR(ok, errM.GetList());
		}
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
	    CONDITIONAL_REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfElements = ZeroTerminatedArrayGetSize(ptr, n);
		}
	}

	if (ok){
		ok = numberOfElements.ToNumber(numberOfElementsIO);
	    CONDITIONAL_REPORT_ERROR(ok, "numberOfElements is infinite/indefinite");
	}

	if (ok){
		ok = nextElementSizeD.ToNumber(nextElementSize);
	    CONDITIONAL_REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}

	return ok;
}

static ErrorManagement::ErrorType HasSameDimensionsAs(const DimensionHandler &first,const DimensionHandler &second){
	ErrorManagement::ErrorType ok = true;
	uint32 nDim = first.NDimensions();
	uint32 nDimO = second.NDimensions();
	if (nDim == 0){
		ok.internalSetupError=true;
        REPORT_ERROR(ok, "NDimensions == 0");
	}
	if (nDim != nDimO){
		ok.invalidOperation=true;

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

        COMPOSITE_REPORT_ERROR(ok,"different number of dimensions: ",errM.GetList())
	}

	for (int i = 0;ok && (i < nDim); i++){
		DimensionSize d1 = first[i].numberOfElements;
		DimensionSize d2 = second[i].numberOfElements;

		if (((d1 != d2) && ( !d1.isIndeterminate()) && ( !d2.isIndeterminate())) || ( d1.isPositiveInf())){
			ok.invalidOperation=true;
	        COMPOSITE_REPORT_ERROR(ok,"dimension[",i,"] d1= ",d1.GetData()," d2= ",d2.GetData())
		}
	}

	return ok;
}

/**************************************************************************************************************************/
/*                   CopyTo 																							  */
/*                   Uses DimensionHandler                                             									  */
/*  			     uses UpdatePointerAndSize                                                                            */
/*  			     uses HasSameDimensionsAs                                                                            */
/**************************************************************************************************************************/

/**
 * Recursive Operation
 */
static ErrorManagement::ErrorType CopyToRecursive(
		uint32 							level,
		DimensionHandler 				&sourceDimensions,
		const uint8* 					sourcePtr,
		DimensionHandler 				&destDimensions,
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
    CONDITIONAL_REPORT_ERROR(ret, "source pointer handling failed");

	if (ret){
		ret = UpdatePointerAndSize(destDimensions,level,(const uint8 *&)destPtr,destNumberOfElements,destElementSize,overHead);
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

	ErrorManagement::ErrorType ret;
	const TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

	DimensionHandler sourceHandler(this->modifiers,this->typeDescriptor);
	DimensionHandler destHandler(destVd.modifiers,destVd.typeDescriptor);

	if (!HasSameDimensionsAs(destHandler,sourceHandler)){
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

/*****************************************************************************************************************************/
/*  			GetSize                                                                                                      */
/*  			uses DimensionHandler                                                                                        */
/*  			uses UpdatePointerAndSize                                                                                    */
/*****************************************************************************************************************************/

/**
 * Operates recursively
 */
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
	ret = UpdatePointerAndSize(handler,level, pointer,multiplier,nextElSize,overHead);
 	dataSize 			= overHead;
	auxSize 			= overHead;

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


/******************************************************************************************************/
/*          Clone algorithm                                                                           */
/*          Uses DimensionHandler                                                                    */
/******************************************************************************************************/

static CCString variableVectors("vVzZdDsS");
static CCString variableMatrices("mM");
static CCString conditionalArrays("fF");

/**
 * Internal class to support Clone function
 */
class VDCloner{
public:
	/**
	 * @brief constructor: only initialises handler
	 */
	VDCloner(CCString modifiers,TypeDescriptor tdIn);

	/**
	 * @brief Separated from constructor to get access to error report
	 */
	ErrorManagement::ErrorType Init();

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
		);

	/**
	 * @brief Note that one has to create the modifier list from the Handler as some of the dimension might have been removed.
	 */
	void GetOutputModifiers(DynamicCString &dc) const ;

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



VDCloner::VDCloner(CCString modifiers,TypeDescriptor tdIn): handler(modifiers,tdIn),pageFile(1024*1024)/*,indexPageFile(1024*1024)*/{
	isString = false;
}

ErrorManagement::ErrorType VDCloner::Init(){
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

	return ret;
}

ErrorManagement::ErrorType VDCloner::DoCreateR(
			uint32 							level,
			const uint8* 					inputPointer,
			uint8 *&						addressOfOutput,
			DimensionSize 					numberOfElementsD
	){
		ErrorManagement::ErrorType ret;

		char8 inputType = handler[level].type;

		// handle the simple scalar type
		// consider the numberOfElements to be copied
		if (inputType == '\0'){
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
			if (inputType == 'A'){

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
			if (variableVectors.In(inputType)){

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

			/**
			 * Handling of matrices
			 * This is a pointer (or array of) to data, associated with the sizes
			 */
			// note that Mm becomes m in the indexing creation
			if (variableMatrices.In(inputType)){

				// first time we reach this subtree node
				// calculate total Size needed to store this layer
				// multiply the numberOfElements by each size
				DimensionSize totalSizeD = numberOfElementsD * sizeof(Matrix<uint8 >);
				uint32 totalSize;
				ret = totalSizeD.ToNumber(totalSize);
	        	CONDITIONAL_REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			CONDITIONAL_REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				Matrix<uint8 > *mp = reinterpret_cast<Matrix<uint8 > *>(addressOfOutput);

				switch (inputType){
				case 'm':
				case 'M':{
					const Matrix<uint8 > *mip = reinterpret_cast<const Matrix<uint8 > *>(inputPointer);
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
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(mip->GetNumberOfElements()));
				    			CONDITIONAL_REPORT_ERROR(ret, "DoCreateR failed");
								if (ret){
									mp->InitMatrix(newAddressOfOutput,mip->GetNumberOfRows(),mip->GetNumberOfColumns());
								}
							}
						}
						mp++;
						mip++;
					}
				}break;
				default:{
					ret.internalSetupError=true;
	    			CONDITIONAL_REPORT_ERROR(ret, "unexpected case");
				}
				}
			} else

			/**
			*
		    */
			if (conditionalArrays.In(inputType)){

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

/**
 * @brief Note that one has to create the modifier list from the Handler as some of the dimension might have been removed.
 */
void VDCloner::GetOutputModifiers(DynamicCString &dc) const {
	ErrorManagement::ErrorType ret;

	for (int i = 0; i < handler.NDimensions();i++){
		char8 type = handler[i].type;
		uint32 size = handler[i].numberOfElements.GetData();
		switch (type){
		case '\0':{
		} break;
		case 'A':{
			dc.Append('A');
			dc.Append(size);
		}break;
		case 'F':
		case 'f':{
			dc.Append("pA");
			dc.Append(size);
		}break;
		default:{
			if (variableVectors.In(type)){
				type = 'v';
			} else
			if (variableMatrices.In(type)){
				type = 'm';
			}
			dc.Append(type);
		}
		};
	}
}

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

		cloner.GetOutputModifiers(mods);

		TypeDescriptor type = cloner.handler.GetTypeDescriptor();

		if (ret){
			uint32 pageSize = cloner.pageFile.CurrentPageSize();
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


/**************************************************************************************************/
/*      ToString                                                                                  */
/*      uses GetLayerInfo                                                                         */
/**************************************************************************************************/

/**
 * This function does a double recursion in order to process the pre-pending(*) and post-pending([]) modifiers
 */
static ErrorManagement::ErrorType ToStringPrivate(CCString modifiers,const TypeDescriptor &typeDescriptor,DynamicCString &string,CCString modifierString,bool start,int8 &priority){
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
		ret = ToStringPrivate(modifiers,typeDescriptor,string,modifierString,false,priority);

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
				ret = ToStringPrivate(modifiers,typeDescriptor,string,modifierString,true,localPriority);
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
	    ret = ToStringPrivate(modifiers,typeDescriptor,string,emptyString,true,priority) ;
	}
    return ret;
}


}


	
