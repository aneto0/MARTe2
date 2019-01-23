/**
 * @file ClassRegistryIndex.cpp
 * @brief Source file for class ClassRegistryIndex
 * @date 03/05/2017
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

 * @details This source file contains the definition of all the methods for
 * the class GlobalObjectsDatabase (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "ClassRegistryIndex.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


ClassRegistryIndex::ClassRegistryIndex(){}

ClassRegistryIndex::~ClassRegistryIndex(){}


ClassRegistryIndex *ClassRegistryIndex::Instance() {
    static ClassRegistryIndex *instance = NULL_PTR(ClassRegistryIndex *);
    if (instance == NULL_PTR(ClassRegistryIndex *)) {
        instance = new(HeapManager::singletonHeapId) ClassRegistryIndex();
    }
    return instance;
}


ClassRegistryItemI * ClassRegistryIndex::operator[] (uint32 classRegistrationNo){
	uint32 ix1 = classRegistrationNo >> IndexBits;
	uint32 ix2 = classRegistrationNo &  IndexCellMask;

	bool ret = (ix1 < index.NumberOfUsedElements());
	ClassRegistryIndexCell *table = NULL_PTR(ClassRegistryIndexCell *);
	if (ret){
		table = index[ix1];
		ret = (table != NULL);
	}
	if (ret){
		ret = (ix2 < table->NumberOfUsedElements());
	}
	ClassRegistryItemI * cri = NULL_PTR(ClassRegistryItemI *);
	if (ret){
		cri = (*table)[ix2];
	}

	return cri;
}

uint32 ClassRegistryIndex::GetClassSize (uint32 classRegistrationNo){
	ClassRegistryItemI *crii =  (*this)[classRegistrationNo];
	uint32 size = 0;
	if (crii != NULL){
		size = crii->GetSizeOfClass();
	}
	return size;
}

TypeDescriptor ClassRegistryIndex::GetTypeDescriptor(CCString name){
	TypeDescriptor td = InvalidType(0);
	for (uint32 i = 0u; (i < NumberOfRegisteredClasses()) && (td.all == InvalidType(0).all); i++){
		ClassRegistryItemI *crii =  (*this)[i];
		if (crii != NULL){
			CCString className = crii->GetClassName();
			if (name.isSameAs(className.GetList())){
				td = crii->GetTypeDescriptor();
			}
		}
	}
	return td;
}


uint32 ClassRegistryIndex::NumberOfRegisteredClasses(){
	uint32 no = 0;
	if (index.NumberOfUsedElements() > 0){
		no += (index.NumberOfUsedElements()-1) << IndexBits;
		ClassRegistryIndexCell *cric = index[index.NumberOfUsedElements() - 1];
		if (cric != NULL_PTR(ClassRegistryIndexCell *)){
			no += (cric->NumberOfUsedElements()-1);
		} else {
	        REPORT_ERROR(ErrorManagement::InternalSetupError, "ClassRegistryIndex: database error ");
		}
	}
	return no;
}

uint32 ClassRegistryIndex::AllocateFreeSlots(){
	uint32 value = 0;
	if (index.NumberOfUsedElements() > 0){
		ClassRegistryIndexCell *table = index[index.NumberOfUsedElements() - 1];
		if (table != NULL_PTR(ClassRegistryIndexCell *)){
			value = table->NumberOfFreeElements();
		}
	}

	if (value == 0){
		if (index.NumberOfFreeElements()>0){
			ClassRegistryIndexCell *cric = new ClassRegistryIndexCell;
			bool ret = (cric != NULL);
			if (ret){
				ret = index.Add(cric);
			}

			if (ret){
				value = cric->NumberOfFreeElements();
			}
		}
	}

	return value;
}

uint32 ClassRegistryIndex::Add(ClassRegistryItemI *criIn,uint32 size){
	uint32 no = 0xFFFFFFFF;
	uint32 availableSize = AllocateFreeSlots();
	if (availableSize > 0){
		ClassRegistryIndexCell *cric = index[index.NumberOfUsedElements() - 1];
		if (cric != NULL){
			cric->Add(criIn);
			no = (index.NumberOfUsedElements()-1) << IndexBits;
			no += (cric->NumberOfUsedElements() - 1);

		}
	}
	return no;
}



}


