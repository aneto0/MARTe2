/**
 * @file AnyType.cpp
 * @brief Source file for class AnyType
 * @date Nov 12, 2015
 * @author aneto
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
 * the class AnyType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Object.h"
#include "VariableDescriptor.h"
#include "MemoryCheck.h"
#include "ClassMember.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//#include "Windows.h"

namespace MARTe {


ErrorManagement::ErrorType AnyType::Dereference (uint32 index){

	const uint8* pointer = reinterpret_cast<const uint8*> (pointer2Variable);
	ErrorManagement::ErrorType ret = variableDescriptor.Redirect(pointer,index) ;
	pointer2Variable = reinterpret_cast<const void *> (pointer);

	return ret;

#if 0
	char8 modifier;
	uint64 max;
	uint64 layerSize;
	variableDescriptor.GetSize(pointer2Variable,layerSize,NULL,0);
	ErrorManagement::ErrorType ret;

	ret.exception = !MemoryCheck::Check(pointer2Variable);

	if (ret){
		// no modifiers
		// check for potential compact types (only strings supported)
		if (!variableDescriptor.RemoveModifiersLayer(modifier,max)){
			switch (variableDescriptor.GetFullTypeDescriptor().all){
			case StaticCharString_number:
			case DynamicCharString_number:
			case ConstCharString_number:
			case CharString_number: {
				const CCString string = * (reinterpret_cast<CCString const *>(pointer2Variable));
				uint32 size = string.GetSize();

				if (index < size){
					pointer2Variable = string.GetList() + index;

					TypeDescriptor &td = variableDescriptor.AccessTypeDescriptor();

					td.arrayProperty = SizedCArray_AP;
					td.arraySize     = 1;

				} else {
					ret.outOfRange = true;
				}
			}break;

			default : {
				ret.invalidOperation = true;
			}

			}
		} else {
			if (ret){
				switch(modifier){
				case 'P':
				case 'p':{
					char8 const  *ptr = * (reinterpret_cast<char8 const * const *>(pointer2Variable));

					uint64 step = layerSize * index;

					pointer2Variable = (ptr + step);
				}break;
				case 'A':
				case 'a':{
					char8 const *ptr = (reinterpret_cast<char8 const *>(pointer2Variable));

					if (index >= max){
						index = max -1;
					}
					uint64 step = layerSize * index;

					ptr = ptr + step;

					pointer2Variable = static_cast<const void *>(ptr);

				}break;
				case 'D':
				case 'd':
				case 'S':
				case 's':
				case 'Z':
				case 'z':{
					char8 const  *ptr = * (reinterpret_cast<char8 const * const *>(pointer2Variable));
					uint64 step = index;

					while (step > 0 ){
						uint32 i;
						for (i = 0; ((i<layerSize) && ptr[i] == '\0'); i++ );
						if (i != layerSize){
							ptr+= layerSize;
						} else {
							step = 0;
						}
					}

					pointer2Variable = ptr ;

				}break;
				case 'M':
				case 'm':{
					Matrix<const char8> const * matrix = (reinterpret_cast< Matrix<const char8> const *>(pointer2Variable));
					max = matrix->GetNumberOfRows();
					if (index >= max){
						index = max -1;
					}

					char8 const * ptr = static_cast< char8 const *> (matrix->GetDataPointer());

					if (matrix->IsStaticDeclared()){
						uint64 step = layerSize * index;
						ptr += step;

					} else {
						if (!MemoryCheck::Check(pointer2Variable)){
							char8 const * const * pptr = reinterpret_cast< char8 const * const *> (ptr);
							ptr = pptr[index];
						} else {
							ret.exception = true;
						}
					}

					pointer2Variable = ptr;
					// need to add to the variableDescriptor the fact that we have now a Array layer left
					variableDescriptor.InsertModifiersLayer('a',matrix->GetNumberOfColumns());


				}break;
				case 'V':
				case 'v':{
					Vector<const char8> const *vector = (reinterpret_cast<Vector<const char8> const *>(pointer2Variable));
					char8 const * ptr = static_cast<char8 const *>(vector->GetDataPointer());
					max = vector->GetNumberOfElements();

					if (index >= max){
						index = max -1;
					}
					uint64 step = layerSize * index;

					ptr = ptr + step;

					pointer2Variable = ptr;

				}break;
				default:{
					ret.internalSetupError = true;
				}

				}
			}

		}

	}
	return ret;
#endif
}


ErrorManagement::ErrorType  AnyType::Dereference (CCString field){

	char8 modifier;
	uint64 max;
	ErrorManagement::ErrorType ret;

	// check field
	if (field.GetSize()==0){
		ret.invalidOperation = true;
	}

	// there must be no modifiers
	if (variableDescriptor.GetModifiersLayer(modifier,max)){
		ret.invalidOperation = true;
	}

	// check pointer to object
	if (ret){
		ret.exception = !MemoryCheck::Check(pointer2Variable);
	}

	// check if type is a structure
	const TypeDescriptor &td = variableDescriptor.GetFullTypeDescriptor();
	if (ret){
		ret.invalidOperation = !td.isStructuredData;
	}

	// find structure documentation
	ClassRegistryDatabase *crd= NULL_PTR(ClassRegistryDatabase *);
	if (ret){
		crd = ClassRegistryDatabase::Instance();
		ret.internalSetupError = (crd == NULL_PTR(ClassRegistryDatabase *));
	}
	ClassRegistryItem *cri = NULL_PTR(ClassRegistryItem *);
	if (ret){
		cri = crd->Find(td);
		ret.unsupportedFeature = (cri == NULL_PTR(ClassRegistryItem *));
	}

	ClassMember const *cm = NULL_PTR(ClassMember const *);
	if (ret){
		cm = cri->FindMember(field);
		ret.unsupportedFeature = (cm == NULL_PTR(ClassMember const *));
	}

	if (ret){
		//bool isConst = td.dataIsConstant;
		const char8 *newPointer2Variable = static_cast<const char8 *>(pointer2Variable);
		newPointer2Variable += cm->GetOffset();
		pointer2Variable = newPointer2Variable;

		variableDescriptor = cm->GetDescriptor();

	}

	return ret;
}

static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

ErrorManagement::ErrorType  AnyType::MultipleDereference (CCString CExpresssion){
	const CCString dels[6] = {CCString("."),CCString("*"),CCString("->"),CCString("["),CCString("]"),CCString()};
	const ZeroTerminatedArray<const CCString> delimiters = &dels[0];
	enum DerefStatus {
		Normal,
		Member,
		Matrix,
		MatrixDone
	}  status = Normal;

	CCString deref = CExpresssion;
	ErrorManagement::ErrorType ok;


	while ((deref.GetSize()>0) && (ok)){
		DynamicCString token;
		int32 term  =-1;
    	deref = StringHelper::Tokenize(deref,token,term, delimiters,CCString(". "));

    	// process token
    	if (token.GetSize() > 0){
        	if (isNumber(token[0])){
        		if (status == Matrix ){
            		ok = Dereference(atoi(token.GetList()));
            		if (ok) {
            			status = MatrixDone;
            		}
        		}
        	} else {
        		if (status == Member){
        			ok = Dereference(token);
            		if (ok) {
            			status = Normal;
            		}
        		} else {
        			ok.syntaxError = true;
        		}
        	}
    	}
    	// process term
    	if (ok){
        	switch(term){
        	case 0:{ // .
        		if (status == Normal){
        			status = Member;
        		} else ok.syntaxError = true;
        	} break;
        	case 1:{ // *
        		if (status == Normal){
        			ok = Dereference(0);
        		} else ok.syntaxError = true;
        	} break;
        	case 2:{ // ->
        		if (status == Normal){
        			ok = Dereference(0);
        			if (ok){
        				status = Member;
        			}
        		} else ok.syntaxError = true;
        	} break;
        	case 3:{ // [
        		if (status == Normal){
        			status = Matrix;
        		} else ok.syntaxError = true;
        	} break;
        	case 4:{ // ]
        		if (status == MatrixDone){
        			status = Normal;
        		} else ok.syntaxError = true;
        	} break;
        	default:{

        	}
        	}
    	}
	}
	return ok;
}



}
