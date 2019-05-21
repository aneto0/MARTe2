/**
 * @file VariableCloner.cpp
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

#include "VariableDescriptorLib.h"
#include "VariableCloner.h"
#include "MemoryOperators.h"
#include "MemoryPageFile.h"
#include "Vector.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace VariableDescriptorLib{

static CCString variableVectors("vVzZdDsS");
static CCString variableMatrices("mM");
static CCString conditionalArrays("fF");


VariableCloner::VariableCloner(VariableDescriptorLib::Variable &handlerIn,MemoryPageFile &pageFileIn): handler(handlerIn), pageFile(pageFileIn){
	isString = false;
}

ErrorManagement::ErrorType VariableCloner::DoCreateR(
			uint32 							level,
			const uint8* 					inputPointer,
			uint8 *&						addressOfOutput,
			DimensionSize 					numberOfElementsD
	){
		ErrorManagement::ErrorType ret;


		char8 inputType = handler[level].TypeChar();

		// handle the simple scalar type
		// consider the numberOfElements to be copied
		if (inputType == 'O'){
			// calculate total Size needed to store final layer
			// multiply the numberOfElements by each size
			VariableDescriptorLib::DimensionSize totalSizeD = numberOfElementsD * handler.GetTypeDescriptor().StorageSize();
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
				uint32 numberOfColumns = 1;
				uint32 numberOfRows = 1;

				if (ret){
					ret = handler[level].UpdatePointerAndSizeEx(inputPointer,numberOfColumns,numberOfRows);
				    REPORT_ERROR(ret, "pointer handling failed");
				}

				// increase multiplier
				numberOfElementsD = numberOfElementsD * numberOfColumns;
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
							uint8 *newAddressOfOutput = NULL_PTR(uint8*);
							const uint8 *newInputPointer = reinterpret_cast<const uint8 *>(vip->GetDataPointer());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,VariableDescriptorLib::DimensionSize(vip->GetNumberOfElements()));
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
//		    				uint32 nextLayerSize;

//		    				ret = handler[level].GetNextLayerElementSize(nextLayerSize);
//			    			REPORT_ERROR(ret, "GetNextLayerElementSize failed");
//		    				uint32 numberOfElements= ZeroTerminatedArrayGetSize(newInputPointer, nextLayerSize);

		    				uint32 numberOfElements= ZeroTerminatedArrayStaticTools::ZTAGetSize(/* *zip*/newInputPointer, handler[level+1].GetLayerCombinedElementSize().GetData());
//							uint32 numberOfElements= ZeroTerminatedArrayStaticTools::ZTAGetSize(/* *zip*/newInputPointer, handler[level+1].elementSize.GetData());
							if (ret){
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,VariableDescriptorLib::DimensionSize(numberOfElements));
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
				VariableDescriptorLib::DimensionSize totalSizeD = numberOfElementsD * sizeof(Matrix<uint8 >);
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
								ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,VariableDescriptorLib::DimensionSize(mip->GetNumberOfElements()));
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
				VariableDescriptorLib::DimensionSize totalSizeD = numberOfElementsD * sizeof(uint8 *);
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
							ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,handler[level].GetLayerCombinedElementSize());
//							ret = DoCreateR(level+1,newInputPointer,newAddressOfOutput,handler[level].elementSize);
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
void VariableCloner::GetOutputModifiers(DynamicCString &dc) const {
	ErrorManagement::ErrorType ret;

	for (uint32 i = 0; i < handler.NDimensions();i++){
		char8 type = handler[i].TypeChar();
		switch (type){
		case 'O':{
		} break;
		case 'A':{
			uint32 size;
			ret = handler[i].GetNumberOfElements(size);
			dc().Append('A').Append(size);
		}break;
		case 'F':
		case 'f':{
			uint32 size;
			ret = handler[i].GetNumberOfElements(size);
			dc().Append("f").Append(size);
		}break;
		default:{
			if (variableVectors.In(type)){
				type = 'v';
			} else
			if (variableMatrices.In(type)){
				type = 'm';
			}
			dc().Append(type);
		}
		}
	}
}



}
} //MARTe
