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
#include "Memory.h"
#include "DynamicCString.h"
#include "MemoryCheck.h"
#include "TypeConversionManager.h"
#include "VariableDescriptor.h"
#include "MemoryPageFile.h"
#include "ReferenceT.h"
#include "AnyObject.h"
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

/**
 * TODO: Change this to fit the specific needs of the platform
 */
const uint32 defaultAllocationSize = 256*256;

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

		if (modifier == 'A'){
			uint32 sz = modifiers.GetSize();
			if (( sz > 0 ) && (modifiers.GetList()[sz-1] == 'P')){
				modifiers.GetList()[sz-1] = 'F';
			} else
			if (( sz > 0 ) && (modifiers.GetList()[sz-1] == 'p')){
				modifiers.GetList()[sz-1] = 'f';
			} else {
				modifiers.Append(modifier);
			}
		} else {
			modifiers.Append(modifier);
		}

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
//#if 1
TypeDescriptor VariableDescriptor::GetVariableDimensions(const uint8 *ptr,uint32 &depth,uint32 *sizes) const {
    uint32 maxDepth = depth;
    depth = 0;
	CCString modifiersCopy = modifiers;
    char8 modifier  = '\0';
    uint32 size = 0;
    TypeDescriptor td;

    bool finished = false;

    while ((depth < maxDepth) && (!finished)){
        GetLayerInfo(modifiersCopy,modifier,size );

    	switch (modifier){
    	case '\0':{
    		finished = true;
    	}break;
    	case 'P':
    	case 'p':{
   			finished = true;
   			td = GenericPointer;
    	}break;
    	case 'M':
    	case 'm':{

    		// only at first level we read the size from memory.
    		// this is because the size is unique only at this level
			if (depth == 0){
				const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
				sizes[depth++] = pm->GetNumberOfRows();
	    		if (depth < maxDepth){
    	    		sizes[depth++] = pm->GetNumberOfColumns();
	    		}
			} else {
	    		sizes[depth++] = 0;
	    		if (depth < maxDepth){
		    		// size = 0 means undertermined
    	    		sizes[depth++] = 0;
	    		}
			}
    	}break;
    	case 'V':
    	case 'v':{
    		// only at first level we read the size from memory.
    		// this is because the size is unique only at this level
			if (depth == 0){
				const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
				sizes[depth++] = pv->GetNumberOfElements();
			} else {
	    		// size = 0 means undertermined
	    		sizes[depth++] = 0;
			}
    	}break;
    	case 'A':{
       		sizes[depth++] = size;
    	}break;
    	case 'd':
    	case 'D':
    	case 'z':
    	case 'Z':
    	case 's':
    	case 'S':{
    		// only at first level we read the size from memory.
    		// this is because the size is unique only at this level
			if (depth == 0){
				ErrorManagement::ErrorType ret = RedirectP(ptr);
				REPORT_ERROR(ret," redirection failed");

				// TODO get size from next layer
				//sizes[depth++] = ZeroTerminatedArrayGetSize(ptr, n);

				// get the size
			} else {
	    		// size = 0 means undertermined
	    		sizes[depth++] = 0;
			}
    	}break;

    	default:{
    		td = InvalidType(0);
    		COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError, "Incorrect modifier: ",modifier);
    	}
    	}
    }

    return td;
}

/*
 * Array --> type + size
 * Vector --> type + size
 * pointer to array --> type + size
 * ZTA --> type + size
 *
 */
// used externally 4 times
TypeDescriptor VariableDescriptor::GetSummaryTypeDescriptor(uint32 *dimensionSize) const {
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
    			// TODO Do we really need to distinguish this?
    			td = PointerType;
    		}

    		// TODO get dimensions for PA PZ PD etc...
    	}break;
    	case 'V':
    	case 'v':
    	case 'M':
    	case 'm':{
    		td = GenericPointer;
    		// TODO get dimensions
    	}break;
    	case 'A':{
    		td = GenericArray;
    		if (dimensionSize != NULL_PTR(uint32 *)){
    			*dimensionSize = size;
    		}
    	}break;

    	default:{
    		td = InvalidType(0);
            REPORT_ERROR(ErrorManagement::FatalError, "Incorrect modifier: ZDS not prepended by P ");
    	}
    	}
    }

	return td;
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
	 * @brief the type that ends this layer. A layer is made of a sequence of As AAP  AAZ etc...
	 */
	char8  endType;

	/**
	 * @brief number of elements in this dimension
	 * NonDet for pointers, Vectors, Arrays,ZTAs etc..
	 */
	DimensionSize numberOfElements;

	/**
	 * @brief product of all the underlying dimensions up to a terminal dimension.
	 * include size of terminating type (void *), vector<>,matrix<> or sizeof(T)
	 * exclude the actual multiplier of the Vector/Matrix/ZTA
	 */
	DimensionSize elementSize;

	/**
	 * @brief constructor
	 */
	DimensionInfoElement(char8 typeIn,const DimensionSize &numberOfElementsIn){
		type = typeIn;
		endType = typeIn;
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
	 *
	 * stopAtFirstTerminal stops parsing at the first non A modifier
	 */
	DimensionHandler(CCString modifiers,TypeDescriptor tdIn,bool stopAtFirstTerminal=false);

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

// supports both expanded and compressed representation
// pA and PA are converted in f and F and at the same time f and F are handled correctly
static const CCString terminals = "VMvmZDSzds";
static const CCString ATerminals = "AFf";

DimensionHandler::DimensionHandler(CCString modifiers,TypeDescriptor tdIn,bool stopAtFirstTerminal){
	td = tdIn;
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	bool finished = false;
	while (!finished){
		if (modifier == '\0'){
			this->Add(DimensionInfoElement('\0',1));
			finished = true;
		} else
		if (ATerminals.In(modifier)){
			this->Add(DimensionInfoElement(modifier,size));
		}
		{
			// !!! Actual pointers are not processed here! --> transform into a modified Td
			if ((modifier == 'P') || (modifier == 'p')){
				td = PointerType;
				if (!IsUpperCase(modifier)){
					td.dataIsConstant = true;
				}
				this->Add(DimensionInfoElement('\0',1));
				finished = true;
			} else
			if (terminals.In(modifier)){

				this->Add(DimensionInfoElement(modifier,SaturatedInteger<uint32>::Indeterminate()));
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

static ErrorManagement::ErrorType HasSameDimensionsAs(const DimensionHandler &first,const DimensionHandler &second){
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

	ret.internalSetupError = (!HasSameDimensionsAs(destHandler,sourceHandler));
	REPORT_ERROR(ret, "Dimension mismatch");

	if (ret){
		tco = TypeConversionManager::Instance().GetOperator(destHandler.GetTypeDescriptor(),sourceHandler.GetTypeDescriptor(),isCompare);
    	ret.unsupportedFeature = ( tco == NULL_PTR(TypeConversionOperatorI *));
    	REPORT_ERROR(ret, "Conversion Operator not found");
	}

    if (ret){
    	ret = CopyToRecursive(0,sourceHandler,sourcePtr,destHandler,destPtr,*tco);
    	REPORT_ERROR(ret, "CopyToRecursive failed");
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
	REPORT_ERROR(ret,"UpdatePointerAndSize failed");

	if (ret){
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

ErrorManagement::ErrorType VariableDescriptor::GetSize(const uint8 *pointer,uint64 &dataSize, uint64 *overHeadSize) const{
	ErrorManagement::ErrorType ret;
	uint64 overHeadSz = 0;

	DimensionHandler handler(this->modifiers,this->typeDescriptor);

	ret = GetSizeRecursive(0,handler,pointer,dataSize,overHeadSz);
	REPORT_ERROR(ret,"GetSizeRecursive failed");

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
	//VDCloner(CCString modifiers,TypeDescriptor tdIn);
	VDCloner(DimensionHandler &handlerIn,MemoryPageFile &pageFileIn);

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
	DimensionHandler &              handler;

	/**
	 * used to store data
	 */
	MemoryPageFile &				pageFile;

	/**
	 * Data is finally encoded as zero term strings
	 */
	bool							isString;
};



VDCloner::VDCloner(DimensionHandler &handlerIn,MemoryPageFile &pageFileIn): handler(handlerIn), pageFile(pageFileIn){
//	handler(modifiers,tdIn),pageFile(1024*1024)
//	handler = handlerIn;
//	pageFile = pageFileIn;
	isString = false;
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
        	REPORT_ERROR(ret, "Overflow");

        	if (ret){
    			// reserve space either for the constant size data or for the string pointers
    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
    			REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
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
					        REPORT_ERROR(ret, "pageFile.WriteReserve failed");

					        if (ret){
								pointerArray[i] = ptr;
								Memory::Copy(ptr,string.GetList(),stringSize);
					        }
						}
					}

        		} else { // not a string
					Memory::Copy(addressOfOutput,inputPointer,totalSize);
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
				REPORT_ERROR(ret, "DoIndexR failed");
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
	        	REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				Vector<uint8 *> *vp = reinterpret_cast<Vector<uint8 *> *>(addressOfOutput);

				switch (inputType){
				case 'v':
				case 'V':{
					const Vector<uint8 *> *vip = reinterpret_cast<const Vector<uint8 *> *>(inputPointer);
					// loop through the collapsed layer
					for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){

						if (vip->GetDataPointer() == NULL_PTR(uint8 *)){

							ret.internalSetupError = (vip->GetNumberOfElements() != 0);
			    			REPORT_ERROR(ret, "Vector with size > 0 and NULL ptr");
							if (ret){
								vp->InitVector(NULL_PTR(uint8**),0);
							}

						} else {
							uint8 *newAddressOfOutput;
							const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(vip->GetDataPointer());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(vip->GetNumberOfElements()));
				    			REPORT_ERROR(ret, "DoCreateR failed");
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
		    			REPORT_ERROR(ret, "RedirectP failed");
		    			if (newInputPointer != NULL){
							uint32 numberOfElements= ZeroTerminatedArrayGetSize(/* *zip*/newInputPointer, handler[level+1].elementSize.GetData());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(numberOfElements));
				    			REPORT_ERROR(ret, "DoCreateR failed");
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
	    			REPORT_ERROR(ret, "unexpected case");
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
	        	REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				Matrix<uint8 > *mp = reinterpret_cast<Matrix<uint8 > *>(addressOfOutput);

				switch (inputType){
				case 'm':
				case 'M':{
					const Matrix<uint8 > *mip = reinterpret_cast<const Matrix<uint8 > *>(inputPointer);
					// loop through the collapsed layer
					for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){
						if (mip->GetDataPointer() == NULL_PTR(uint8 *)){

							ret.internalSetupError = (mip->GetNumberOfElements() != 0);
			    			REPORT_ERROR(ret, "Matrix with size > 0 and NULL ptr");
			    			if (ret){
								mp->InitMatrix(NULL_PTR(uint8*),0,0);
			    			}

						} else {
							uint8 *newAddressOfOutput;
							const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(mip->GetDataPointer());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,DimensionSize(mip->GetNumberOfElements()));
				    			REPORT_ERROR(ret, "DoCreateR failed");
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
	    			REPORT_ERROR(ret, "unexpected case");
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
	        	REPORT_ERROR(ret, "Overflow");

	        	if (ret){
	    			// reserve space either for the constant size data or for the string pointers
	    			ret = pageFile.WriteReserveAtomic(addressOfOutput, totalSize);
	    			REPORT_ERROR(ret, "pageFile.ReserveAtomic failed");
	        	}
				uint8 **fp = reinterpret_cast<uint8 **>(addressOfOutput);
				const uint8 * const *fip = reinterpret_cast<const uint8 * const *>(inputPointer);
				// loop through the collapsed layer
				for (uint32 i = 0; (i < numberOfElementsD.GetData()) && ret; i++){
					uint8 *newAddressOfOutput;
					const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(fip);
					ret = RedirectP(newInputPointer,true);
	    			REPORT_ERROR(ret, "RedirectP failed");
	    			if (newInputPointer != NULL){
						if (ret){
							ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,handler[level].elementSize);
			    			REPORT_ERROR(ret, "DoCreateR failed");
						}
						if (ret){
							*fp = newAddressOfOutput;
						}
	    			} else {
						*fp = NULL_PTR(uint8*);
	    			}
					fp++;
					fip++;
				}
			} else {
				ret.fatalError = true;
    			REPORT_ERROR(ret, "Unmapped outputType");
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
			dc.Append("f");
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

	// analyse VariableDescriptor
	// if it can be copied directly without pointers...

	DimensionHandler handler(modifiers,typeDescriptor);

	bool isString = handler.GetTypeDescriptor().IsCharString();
	bool isFixedData = (handler.GetTypeDescriptor().IsStructuredData() || handler.GetTypeDescriptor().IsBasicType());
	bool hasOneLayer = (handler[0].endType == '\0');

	if (!isString && !isFixedData){
		ret.unsupportedFeature = true;
		REPORT_ERROR(ret, "Unsupported type");
	}

	if (ret){
		if ((hasOneLayer) && (!isString)){
			uint32 sizeToCopy = 0;
			DimensionSize ds = handler[0].elementSize;
			ds = ds * handler[0].numberOfElements;

			ret = ds.ToNumber(sizeToCopy);
			REPORT_ERROR(ret, "Size of object > uint32 or undefined");

			if (ret){
				reference = AnyObject::Clone(sizeToCopy,reinterpret_cast<const void *>(pointer),*this);
			}

		} else {
			MemoryPageFile pageFile(defaultAllocationSize);
			VDCloner cloner(handler,pageFile);

			// force allocation of first page
			ret = pageFile.CheckAndNewPage();
			REPORT_ERROR(ret, "CheckAndNewPage(1) failed");


			// copy and create indexes
			if (ret){
				// move it down one level
				uint8 *addressOfOutput = NULL;
				DimensionSize ds(1);
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
				TypeDescriptor type = cloner.handler.GetTypeDescriptor();

				ReferenceT<MemoryPageObject> mpor;
				mpor = ReferenceT<MemoryPageObject> (buildNow);
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


static const CCString zeroTerMods="zZsSdD";
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


	// go ahead and process in reverse pointers and arrays
	if ((modifier=='A') || (modifier=='P') || (modifier=='p') || (modifier=='F') || (modifier=='f')){
		ret = ToStringPrivate(modifiers,typeDescriptor,string,modifierString,false,priority);

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


/***************************************************************************************************************/
/*               Redirect                                                                                      */
/***************************************************************************************************************/

static DimensionSize LayerSize(CCString modifiers,TypeDescriptor td){
	char8 type;
	uint32 size;

	DimensionSize ds(1);
	GetLayerInfo(modifiers,type,size);
	while (type == 'A'){
		ds = ds * size;
		GetLayerInfo(modifiers,type,size);
	}

	/**
	 * note that PS PZ .... have the same size as P so we do not need to distinguish them from P
	 */
	ds = ds * Type2Size(type,td);
	return ds;
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
	ret.parametersError = (pointer == NULL_PTR(const uint8 *));
    REPORT_ERROR(ret, "pointer = NULL");

	if (ret){
		GetLayerInfo(modifierString,modifier,size);

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

				DimensionSize layerSize = LayerSize(modifierString,typeDescriptor);
				DimensionSize step = layerSize * index;

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = pointer + step32;
					modifiers.Remove(modifierString.GetList()-modifiers.GetList());
				}
	 		}
	 	}break;
	 	case 'p':
	 	case 'P':{
 			ret.outOfRange = (index != 0);
	        REPORT_ERROR(ret, "index!=0 for pointer to var");

			if (ret){
				modifiers.Remove(modifierString.GetList()-modifiers.GetList());
			}
	 	}break; // end Pp case
	 	case 'A':{
			ret.outOfRange = (index >= size);
	        REPORT_ERROR(ret, "index >= size");

	 		// need fulllayerSize of the remaining full layer
	 		// note that next layer cannot be ZzDdSs but only A or a terminator like PpVvMm and 0
	 		if (ret){

				DimensionSize layerSize = LayerSize(modifierString,typeDescriptor);
				DimensionSize step = layerSize * index;

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = pointer + step32;
					modifiers.Remove(modifierString.GetList()-modifiers.GetList());
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

			DimensionSize layerSize = LayerSize(modifierString,typeDescriptor);
			uint32 layerSize32;
			ret = layerSize.ToNumber(layerSize32);
	        REPORT_ERROR(ret, "layerSize >= 32bit");

	        uint32 maxIndex  = 0;
	        if (ret){
	        	maxIndex  = ZeroTerminatedArrayGetSize(pointer, layerSize32);
	        	ret.outOfRange = (index >= maxIndex);
	        	REPORT_ERROR(ret, "index >= maxIndex");
	 		}

			if (ret){
				DimensionSize step = layerSize * index;
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");
//REPORT_ERROR(ErrorManagement::Debug, "LayerSize = ",layerSize.GetData());
//REPORT_ERROR(ErrorManagement::Debug, "Step = ",step.GetData(),"Step32 = ",step32,"Index = ",index);

		        if (ret){
			        pointer = pointer + step32;
					modifiers.Remove(modifierString.GetList()-modifiers.GetList());
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

				DimensionSize layerSize = LayerSize(modifierString,typeDescriptor);
//REPORT_ERROR(ErrorManagement::Debug, "LayerSize = ",layerSize.GetData());
				DimensionSize step = layerSize * index;
//REPORT_ERROR(ErrorManagement::Debug, "Step = ",step.GetData(),"Index = ",index);

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = p;
					pointer = pointer + step32;
					modifiers.Remove(modifierString.GetList()-modifiers.GetList());
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
				DimensionSize layerSize = LayerSize(modifierString,typeDescriptor);
				DimensionSize step = layerSize * index;
				step = step * pm[0].GetNumberOfColumns();

				// check saturation
				uint32 step32;
				ret = step.ToNumber(step32);
		        REPORT_ERROR(ret, "step >= 32bit");

				if (ret){
					pointer = p;
					pointer = pointer + step32;
					modifiers.Remove(modifierString.GetList()-modifiers.GetList());
				}

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
