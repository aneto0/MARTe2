/**
 * @file AnyObject.cpp
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

#include "AnyObject.h"
#include "AnyObjectT.h"
#include "AnyObjectM.h"
#include "HeapManager.h"

#define CHECKANDMAKE(size)									\
		if (sizeToCopy <= size){							\
			ReferenceT<AnyObjectT<size>> ao(HeapManager::standardHeapId);\
			if (ao.IsValid()){								\
				ao->Setup(sizeToCopy,pointer,descriptor);	\
				reference = ao;								\
			}												\
		} else												\


namespace MARTe{

Reference AnyObject::Clone(uint32 sizeToCopy,const void *pointer,const VariableDescriptor &descriptor){
	Reference reference;

	CHECKANDMAKE(4)
	CHECKANDMAKE(8)
	CHECKANDMAKE(16)
	CHECKANDMAKE(24)
	CHECKANDMAKE(32)
	CHECKANDMAKE(40)
	CHECKANDMAKE(48)
	CHECKANDMAKE(56)
	CHECKANDMAKE(64)
	{
		ReferenceT<AnyObjectM> ao(HeapManager::standardHeapId);
		if (ao.IsValid()){
			ao->Setup(sizeToCopy,pointer,descriptor);
			reference = ao;
		}
	}

	return reference;
}



} //MARTe
