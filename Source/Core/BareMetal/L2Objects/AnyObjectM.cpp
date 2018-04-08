/**
 * @file AnyObjectM.cpp
 * @brief Header file for class AnyType
 * @date 8 Apr 2018
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

#include "AnyObjectM.h"
#include "GlobalObjectsDatabase.h"


namespace MARTe{

AnyObjectM::AnyObjectM():vd(InvalidType(0),""){
	data = NULL;
};

AnyObjectM::~AnyObjectM(){
	GlobalObjectsDatabase::Instance().GetStandardHeap().Free(data);
};

void AnyObjectM::ToAnyType(AnyType &at){
	at = AnyType(vd,data);
}

void AnyObjectM::Setup(uint32 sizeToCopy,const void *pointer,const VariableDescriptor &descriptor){
	vd = descriptor;
	data = GlobalObjectsDatabase::Instance().GetStandardHeap().Malloc(sizeToCopy);
	if (data == NULL){
		REPORT_ERROR(ErrorManagement::OutOfMemory,"Malloc failed");
		vd = VariableDescriptor();
	} else {
		MemoryOperationsHelper::Copy(data,pointer,sizeToCopy);
	}
}

CLASS_REGISTER(AnyObjectM,"1.0")



} //MARTe
