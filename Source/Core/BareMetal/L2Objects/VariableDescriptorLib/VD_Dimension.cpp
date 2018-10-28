/**
 * @file VD_Dimension.cpp
 * @brief Header file for class AnyType
 * @date 20 Oct 2018
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

#include "VD_Dimension.h"
#include "MemoryCheck.h"
#include "Vector.h"
#include "Matrix.h"
#include "Memory.h"
#include "CompositeErrorManagement.h"
#include "VariableDescriptorLib.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe{
namespace VariableDescriptorLib{


Dimension::Dimension(){
	flags = 0;
	next = NULL;
	endStack = NULL;
}

Dimension::~Dimension(){
	if (next != NULL){
		delete next;
		next = NULL;
	}
}


void Dimension::Add(Dimension *newDimension){
	if (next == NULL_PTR(Dimension *)){
		next = newDimension;
	} else {
		next->Add(newDimension);
		if (newDimension->IsBreak() && (endStack==NULL_PTR(Dimension *))){
			endStack = newDimension;
		}
	}
}

void Dimension::AddToEnd(Dimension *&rootPtr,Dimension *newDimension){
	if (newDimension != NULL_PTR(Dimension *)){
		if (rootPtr == NULL_PTR(Dimension *)){
			rootPtr = newDimension;
		} else {
			rootPtr->Add(newDimension);
		}
	}
}

bool Dimension::IsFinalLayer() const{
	bool ret = (next == NULL);
	if (ret == false){
		ret= (next->next == NULL);
	}
	return ret;
}

uint32 Dimension::GetOverHead() const {
	return TypeSize();
}

TypeDescriptor Dimension::GetTypeDescriptor() const{
	return GenericArray;
}

ErrorManagement::ErrorType Dimension::GetNumberOfElements(uint32 &numberOfElements) const{
	return ErrorManagement::IllegalOperation;
}

ErrorManagement::ErrorType Dimension::ReSize(
			uint8 *&		ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const{
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = true;

	return ret;
}

static ErrorManagement::ErrorType SetPtrToZero(DimensionSize numberOfElements, uint8 *ptr){
	ErrorManagement::ErrorType ret;
	uint32 n;
	numberOfElements = numberOfElements * sizeof(void *);
	ret = numberOfElements.ToNumber(n);
	REPORT_ERROR(ret,"number of elements overflow");

	if (ret){
		ret = Memory::Set(ptr,0,n);
		REPORT_ERROR(ret,"Memory::Set error");
	}
	return ret;
}

















DataDimension::DataDimension(const TypeDescriptor &td): Dimension(){
	endStack = this;
	this->td = td;
	flags = isBreak | isFinal;
	if (td.dataIsConstant){
		flags |= isConstant;
	}
}

DataDimension::~DataDimension(){
}

uint32 DataDimension::TypeSize() const {
	return td.StorageSize();
};

TypeDescriptor DataDimension::GetTypeDescriptor() const{
	return td;
}

DimensionSize DataDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(td.StorageSize());
}

ErrorManagement::ErrorType DataDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {

	ErrorManagement::ErrorType ok;
	numberOfRows = 1;
	numberOfColumns = 1;
	return ok;
}

ErrorManagement::ErrorType DataDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {

	ErrorManagement::ErrorType ok;

	if (td.SameAs(DynamicCharString)){
		ok = SetPtrToZero(numberOfElements, ptr);
		REPORT_ERROR(ok,"Failed setting memory to 0");
	} else
	{
//TODO handle somehow the copy constructor problem.
// this implementation will fail with structures requiring non-trivial copy constructors
		ok.unsupportedFeature = (!td.IsBasicType() && !td.IsStructuredData());
		REPORT_ERROR(ok,"cannot handle the type");
	}

	return ok;

}

char8 DataDimension::TypeChar() const {
	return 'O';
}












ArrayDimension::ArrayDimension(uint32 numberOfElementsIn): Dimension(){
	numberOfElements = numberOfElementsIn;
}

ArrayDimension::~ArrayDimension(){
}

uint32 ArrayDimension::TypeSize() const {
	return 0;
};

char8 ArrayDimension::TypeChar() const {
	return 'A';
}

DimensionSize ArrayDimension::GetLayerCombinedElementSize() const{
	DimensionSize ret(numberOfElements);
	if (next != NULL){
		ret = ret * next->GetLayerCombinedElementSize();
	} else{
		REPORT_ERROR(ErrorManagement::FatalError,"unterminated stack");
	}

return ret;
}

ErrorManagement::ErrorType ArrayDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {

	ErrorManagement::ErrorType ok;

	numberOfRows = 1;
	numberOfColumns = numberOfElements;
	return ok;
}

ErrorManagement::ErrorType ArrayDimension::GetNumberOfElements(uint32 &numberOfElements) const {
	ErrorManagement::ErrorType ok;
	numberOfElements = this->numberOfElements;
	return ok;
}

ErrorManagement::ErrorType ArrayDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {

	return InitStack(ptr,numberOfElements * this->numberOfElements);
}













PointerToArrayDimension::PointerToArrayDimension(uint32 numberOfElementsIn,bool constant): ArrayDimension(numberOfElementsIn){
	if (constant){
		flags |= isConstant;
	}
	flags |= isBreak;
}

PointerToArrayDimension::~PointerToArrayDimension(){}


DimensionSize PointerToArrayDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(sizeof(void *));
}

uint32 PointerToArrayDimension::TypeSize() const {
	return sizeof(void *);
};

char8 PointerToArrayDimension::TypeChar() const {
	char8 ret = 'F';
	if ((flags & isConstant)!=0){
		ret = 'f';
	}
	return ret;
}

ErrorManagement::ErrorType PointerToArrayDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	numberOfRows = 1;
	numberOfColumns = numberOfElements;

	// all need to redirect
	// it works as vector is descendant of Pointer class
	ok = VariableDescriptorLib::RedirectP(ptr,true);
	COMPOSITE_REPORT_ERROR(ok, "pointer to array type redirection failed ");

	// handle case of null pointers
	if (ptr == NULL) {
		numberOfColumns = 0;
	}

	return ok;
}

ErrorManagement::ErrorType PointerToArrayDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	return SetPtrToZero(numberOfElements, ptr);
}











VectorDimension::VectorDimension(bool constant): Dimension(){
	if (constant){
		flags |= isConstant;
	}else {
		flags |= isDynamic;
	}
	flags |= isBreak;
}

VectorDimension::~VectorDimension(){
}

DimensionSize VectorDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(TypeSize());
}

uint32 VectorDimension::TypeSize() const {
	return sizeof(Vector<char8>);
}

char8 VectorDimension::TypeChar() const {
	char8 ret = 'V';
	if ((flags & isConstant)!=0){
		ret = 'v';
	}
	return ret;
}

ErrorManagement::ErrorType VectorDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	bool allowNULL = false;
	// get size from structure
	const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
	numberOfRows = 1;
	numberOfColumns = pv->GetNumberOfElements();
	if (numberOfColumns == 0U){
		// if 0 element the Vector will probably have NULL PTR
		allowNULL = true;
	}

	// all need to redirect
	// it works as vector is descendant of Pointer class
	ok = VariableDescriptorLib::RedirectP(ptr,allowNULL);
	COMPOSITE_REPORT_ERROR(ok, "Vector type redirection failed ");

    // handle case of null pointers
	if (ptr == NULL) {
		numberOfColumns = 0;
	}

	return ok;
}

ErrorManagement::ErrorType VectorDimension::ReSize(
			uint8 *&		ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows)const {
	ErrorManagement::ErrorType ret;

	DimensionSize totalSizeD ;
	ret = GetNextLayerElementSize(totalSizeD);
    REPORT_ERROR(ret, "GetNextLayerElementSize failed");

    uint32 totalAllocationSize;
    if (ret){
    	totalSizeD = totalSizeD * numberOfColumns;
		totalSizeD = totalSizeD * numberOfRows;

    	ret = totalSizeD.ToNumber(totalAllocationSize);
        COMPOSITE_REPORT_ERROR(ret, "total size overflow: NCol = ",numberOfColumns,"NRows = ",numberOfRows);
    }

    uint8 *newMemory;
	if (ret){
		// allocate memory
		newMemory = reinterpret_cast<uint8 *>(HeapManager::Malloc(totalAllocationSize));
		ret.outOfMemory = (newMemory == NULL_PTR(uint8 *));
		COMPOSITE_REPORT_ERROR(ret, "malloc of ", totalAllocationSize ," failed" );
	}

	Vector<uint8> *pVec = reinterpret_cast<Vector<uint8> *>(ptr);
	if (ret){
		// resize vector
		pVec->SetSize(0);
		pVec->InitVector(newMemory,numberOfColumns,true);// true means allocated

		ret.internalSetupError = (next == NULL_PTR(Dimension *));
		REPORT_ERROR(ret,"next layer to a Vector is NULL");

		if (ret){
			ret = next->InitStack(newMemory,numberOfColumns);
			REPORT_ERROR(ret,"memory init failed");
		}

		if (ret){
			ptr = newMemory;
		}
	}

	return ret;
}

ErrorManagement::ErrorType VectorDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	uint32 n;
	ret = numberOfElements.ToNumber(n);
	REPORT_ERROR(ret,"number of elements overflow");

	if (ret){
		Vector<char8 >* vptr = reinterpret_cast<Vector<char8 >* >(ptr);
		for (int i = 0; i< n;i++){
			vptr->InitVector(NULL,0,false);
			vptr++;
		}
	}

	return ret;
}












MatrixDimension::MatrixDimension(bool constant): Dimension(){
	if (constant){
		flags |= isConstant;
	}else {
		flags |= isDynamic;
	}
	flags |= isBreak;
	flags |= is2D;
}

MatrixDimension::~MatrixDimension(){
}

DimensionSize MatrixDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(TypeSize());
}

uint32 MatrixDimension::TypeSize() const {
	return sizeof(Matrix<char8>);
}

char8 MatrixDimension::TypeChar() const {
	char8 ret = 'M';
	if ((flags & isConstant)!=0){
		ret = 'm';
	}
	return ret;
}

ErrorManagement::ErrorType MatrixDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	bool allowNULL = false;
	// get size from structure
	const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);

	numberOfRows = pm->GetNumberOfRows();
	numberOfColumns = pm->GetNumberOfColumns();
	if (pm->GetNumberOfElements() == 0U){
		allowNULL = true;
	}

	// all need to redirect
	// it works as vector is descendant of Pointer class
	ok = VariableDescriptorLib::RedirectP(ptr,allowNULL);
	COMPOSITE_REPORT_ERROR(ok, "Matrix type redirection failed ");

    // handle case of null pointers
	if (ptr == NULL) {
		numberOfColumns = 0;
		numberOfRows = 0;
	}
	return ok;
}

uint8 MatrixDimension::NOfDimensions() const{
	return 2;
}

ErrorManagement::ErrorType MatrixDimension::ReSize(
			uint8 *&		ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const{
	ErrorManagement::ErrorType ret;

	DimensionSize totalSizeD ;
	ret = GetNextLayerElementSize(totalSizeD);
    REPORT_ERROR(ret, "GetNextLayerElementSize failed");

    uint32 totalAllocationSize;
    if (ret){
    	totalSizeD = totalSizeD * numberOfColumns;
    	totalSizeD = totalSizeD * numberOfRows;

    	ret = totalSizeD.ToNumber(totalAllocationSize);
        COMPOSITE_REPORT_ERROR(ret, "total size overflow: NCol = ",numberOfColumns,"NRows = ",numberOfRows);
    }

    uint8 *newMemory;
	if (ret){
		// allocate memory
		newMemory = reinterpret_cast<uint8 *>(HeapManager::Malloc(totalAllocationSize));
		ret.outOfMemory = (newMemory == NULL_PTR(uint8 *));
		COMPOSITE_REPORT_ERROR(ret, "malloc of ", totalAllocationSize ," failed" );
	}

	Matrix<uint8> *pMat = reinterpret_cast<Matrix<uint8> *>(ptr);
	if (ret){
		// resize vector
		pMat->SetSize(0,0);
		pMat->InitMatrix(newMemory,numberOfRows,numberOfColumns,true);// true means allocated

		ret.internalSetupError = (next == NULL_PTR(Dimension *));
		REPORT_ERROR(ret,"next layer to a Matrix is NULL");

		if (ret){
			ret = next->InitStack(newMemory,numberOfColumns*numberOfRows);
			REPORT_ERROR(ret,"memory init failed");
		}

		if (ret){
			ptr = newMemory;
		}

	}

	return ret;
}

ErrorManagement::ErrorType MatrixDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	uint32 n;
	ret = numberOfElements.ToNumber(n);
	REPORT_ERROR(ret,"number of elements overflow");

	if (ret){
		Matrix<char8 >* mptr = reinterpret_cast<Matrix<char8 >* >(ptr);
		for (int i = 0; i< n;i++){
			mptr->InitMatrix(NULL,0,0,false);
			mptr++;
		}
	}

	return ret;
}









ZTADimension::ZTADimension(bool dynamic,bool constant): Dimension(){
	if (constant){
		flags |= isConstant;
	} else {
		if (dynamic){
			flags |= isDynamic;
		}
	}
	flags |= isBreak;
}

ZTADimension::~ZTADimension(){
}

DimensionSize ZTADimension::GetLayerCombinedElementSize() const {
	return DimensionSize(TypeSize());
}

uint32 ZTADimension::TypeSize() const {
	return sizeof(void *);
}

char8 ZTADimension::TypeChar() const {
	char8 ret = 'Z';
	if ((flags & isConstant)!=0){
		if ((flags & isDynamic)!=0){
			ret = 'd';
		} else {
			ret = 'z';
		}
	} else {
		if ((flags & isDynamic)!=0){
			ret = 'D';
		}
	}
	return ret;
}

uint32 ZTADimension::GetOverHead() const {
	uint32 overHead = TypeSize();
	if (next){
		overHead += next->GetLayerCombinedElementSize().GetData();
	}
	return overHead;
}

ErrorManagement::ErrorType ZTADimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	numberOfColumns = 0;
	numberOfRows = 1;
	ok.fatalError = (next == NULL);
	REPORT_ERROR(ok,"next element of a ZTA is missing");

	if (ok){
		// all need to redirect
		// it works as vector is descendant of Pointer class
		ok = VariableDescriptorLib::RedirectP(ptr,true);
		REPORT_ERROR(ok, "ZTA type  redirection failed ");
	}

	uint32 nextElementSize;
	if (ok){
		ok = next->GetLayerCombinedElementSize().ToNumber(nextElementSize);
	    REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}

	if (ok){
	// note ZTA do not allow null so NULL is trapped by RedirectP
	// calculate actual size of each ZTA
	    REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfColumns = ZeroTerminatedArrayGetSize(ptr, nextElementSize);
		}
	}

	return ok;
}

ErrorManagement::ErrorType ZTADimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	return SetPtrToZero(numberOfElements, ptr);
}



















#if 0


Dimension::operator TypeDescriptor() const{
	return Type2TypeDescriptor(type,TypeDescriptor(mux.tdAsInt));
}

uint32 Dimension::TypeSize() const{
	const CCString matrixS = "Mm";
	const CCString vectorS = "Vv";
	const CCString pointerS = "FfSDZPsdzp";
	uint32 size = 0;
	if (pointerS.In(type)){
		size = sizeof(void *);
	} else
	if (matrixS.In(type)){
		size = sizeof(Matrix<char8>);
	} else
	if (vectorS.In(type)){
		size = sizeof(Vector<char8>);
	} else {
		size = TypeDescriptor(mux.tdAsInt).StorageSize();
	}
	return size;
}


ErrorManagement::ErrorType Dimension::UpdatePointerAndSize(
			const uint8 *&	ptr,
			uint32 &		numberOfElementsIO,
			uint32 &		nextElementSize,
			uint32 &		overHead)  const {
	ErrorManagement::ErrorType ok;

	uint32 numberOfRows = 1;
	ok = UpdatePointerAndSizeEx(ptr,numberOfElementsIO,numberOfRows,nextElementSize,overHead);

	numberOfElementsIO *= numberOfRows;


	return ok;
}

ErrorManagement::ErrorType Dimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows,
			uint32 &		nextElementSize,
			uint32 &		overHead)  const {
	ErrorManagement::ErrorType ok;

	uint8 typeT = type;
	bool isZta = false;
	bool allowNULL = false;
	overHead = 0;

	numberOfRows = 1;

	VariableDescriptorLib::DimensionSize nextElementSizeD(0);

	if (next != NULL){
		nextElementSizeD = next->stackElementSize;
	}
	// size of the element in next level

	switch (type){

	case 'O':{
		// next level does not exist
		numberOfColumns = 1;
		// mimic A case / a single number is like A1
		typeT = 'A';
		nextElementSizeD = stackElementSize;
	} break;
//	case 'x': // F or f with NULLs - created at this level
	case 'f':
	case 'F':{
		overHead = sizeof (void *);
		numberOfColumns = mux.numberOfElements;
		allowNULL = true;
	} break;
	case 'A':{
		numberOfColumns = mux.numberOfElements;
	}break;
	case 'd':
	case 's':
	case 'z':
	case 'D':
	case 'S':
	case 'Z':{
		isZta = true;
		// overhead is the pointer and the terminator
		// next is valid as the last element is the 0
		overHead = sizeof (void *) + next->stackElementSize.GetData();
	}break;
	case 'v':
	case 'V':{
		// overhead is the structure itself
		overHead = sizeof (Vector<char8>);
		// get size from structure
		const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
		numberOfColumns = pv->GetNumberOfElements();
		if (numberOfColumns == 0U){
			// if 0 element the Vector will probably have NULL PTR
			allowNULL = true;
		}
	}break;
	case 'm':
	case 'M':{
		overHead = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);

		numberOfRows = pm->GetNumberOfRows();
		numberOfColumns = pm->GetNumberOfColumns();
		if (pm->GetNumberOfElements() == 0U){
			allowNULL = true;
		}
	}break;
	default:{
		ok.fatalError = true;
        REPORT_ERROR(ok, "Default case reached");
	}break;

	}

	// handle all but array and terminals
	if ((typeT != 'A') && ok){

		// all need to redirect
		// it works as vector is descendant of Pointer class
		ok = VariableDescriptorLib::RedirectP(ptr,allowNULL);
		COMPOSITE_REPORT_ERROR(ok, "type ",type," redirection failed ");

        // handle case of null pointers
		if (ptr == NULL) {
			numberOfColumns = 0;
		}
	}

	// note ZTA do not allow null so NULL is trapped by RedirectP
	// calculate actual size of each ZTA
	if (isZta && ok){
		uint32 n;
		ok = next->stackElementSize.ToNumber(n);
	    REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfColumns = ZeroTerminatedArrayGetSize(ptr, n);
		}
	}

	if (ok){
		ok = nextElementSizeD.ToNumber(nextElementSize);
	    REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}

	return ok;
}
#endif

}
} //MARTe
