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
}


ErrorManagement::ErrorType  AnyType::Dereference (CCString field){


	ErrorManagement::ErrorType ret;

	// check field
	if (field.GetSize()==0){
		ret.invalidOperation = true;
	}

	// check pointer to object
	if (ret){
		ret.exception = !MemoryCheck::Check(pointer2Variable);
	}

	// check if type is a structure
	TypeDescriptor td = variableDescriptor.GetSummaryTypeDescriptor();
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


ErrorManagement::ErrorType  AnyType::MultipleDereference (CCString CExpression){
	const CCString dels[6] = {CCString("."),CCString("*"),CCString("->"),CCString("["),CCString("]"),CCString()};
	const ZeroTerminatedArray<const CCString> delimiters = &dels[0];
	enum DerefStatus {
		Normal,
		Member,
		Matrix,
		MatrixDone
	}  status = Normal;

	CCString deref = CExpression;
	ErrorManagement::ErrorType ok;


	while ((deref.GetSize()>0) && (ok)){
		DynamicCString token;
		int32 term  =-1;
    	deref = StringHelper::Tokenize(deref,token,term, delimiters,CCString(". "));

    	// process token
    	if (token.GetSize() > 0){
        	if (isNumber(token[0])){
        		if (status == Matrix ){
                        CCString tok = token;
                        uint32 number = readNumber(tok);
            		ok = Dereference(number);
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

ErrorManagement::ErrorType AnyType::CopyTo(AnyType destination){
	const uint8 *sourcePtr =  reinterpret_cast<const uint8 *>(pointer2Variable);
	uint8 *destPtr         =  reinterpret_cast<uint8 *>(const_cast<void *>(destination.pointer2Variable));
	return variableDescriptor.CopyTo(sourcePtr,destPtr,destination.variableDescriptor);
}



}
