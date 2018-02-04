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

#include "ClassRegistryIndex.h"
#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassRegistryIndex::ClassRegistryIndex(){

}

ClassRegistryIndex::~ClassRegistryIndex(){

}

ClassRegistryIndex *ClassRegistryIndex::Instance() {
    static ClassRegistryIndex *instance = NULL_PTR(ClassRegistryIndex *);
    if (instance == NULL_PTR(ClassRegistryIndex *)) {
        instance = new ClassRegistryIndex();
        uint32 order = NUMBER_OF_GLOBAL_OBJECTS - 4u;
        GlobalObjectsDatabase::Instance().Add(instance, order);
    }
    return instance;
}

CCString ClassRegistryIndex::GetClassName() const{
	return "ClassRegistryIndex";

}


ClassRegistryBrief * ClassRegistryIndex::operator[] (uint32 classRegistrationNo){
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
	ClassRegistryBrief * crb = NULL_PTR(ClassRegistryBrief *);
	if (ret){
		crb = &(*table)[ix2];
	}

	return crb;
}

uint32 ClassRegistryIndex::GetClassSize (uint32 classRegistrationNo){
	ClassRegistryBrief *crb =  (*this)[classRegistrationNo];
	uint32 size = 0;
	if (crb != NULL){
		size = crb->GetSizeOfClass();
	}
	return size;
}

TypeDescriptor ClassRegistryIndex::GetTypeDescriptor(CCString name){
	TypeDescriptor td = InvalidType(0);
	for (int i = 0; (i < NumberOfRegisteredClasses()) && (td.all == InvalidType(0).all); i++){
		ClassRegistryBrief *crb =  (*this)[i];
		if (crb != NULL){
			if (crb->crii != NULL){
				CCString className = crb->crii->GetClassName();
				if (name.isSameAs(className.GetList())){
					td = crb->crii->GetTypeDescriptor();
				}
			}
		}
	}
	return td;
}

ClassRegistryItem *  ClassRegistryIndex::GetClassRegistryItem (uint32 classRegistrationNo){
	ClassRegistryBrief *crb =  (*this)[classRegistrationNo];
	ClassRegistryItem *cri = NULL_PTR(ClassRegistryItem *);
	if (crb != NULL){
//		// works only if ClassRegistryItemI is the sole ancestor of ClassRegistryItem
//		cri = reinterpret_cast<ClassRegistryItem *>(crb->crii);
		if (crb->crii != NULL){
			cri = crb->crii->GetBasePtr();
		}
	}
	return cri;

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

#include <stdio.h>
uint32 ClassRegistryIndex::Add(ClassRegistryItemI *criIn,uint32 size){
	uint32 no = 0xFFFFFFFF;
	uint32 availableSize = AllocateFreeSlots();
	if (availableSize > 0){
		ClassRegistryIndexCell *cric = index[index.NumberOfUsedElements() - 1];
		if (cric != NULL){
			ClassRegistryBrief crb;
//			crb.sizeOfClass = size;
			crb.crii = criIn;
			cric->Add(crb);
			no = (index.NumberOfUsedElements()-1) << IndexBits;
			no += (cric->NumberOfUsedElements() - 1);

		}
	}
	return no;
}



}


