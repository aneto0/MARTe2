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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {



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
		size = crb->sizeOfClass;
	}
	return size;
}

ClassRegistryItem *  ClassRegistryIndex::GetClassRegistryItem (uint32 classRegistrationNo){
	ClassRegistryBrief *crb =  (*this)[classRegistrationNo];
	ClassRegistryItem *cri = NULL_PTR(ClassRegistryItem *);
	if (crb != NULL){
		cri = crb->cri;
	}
	return cri;

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


uint32 ClassRegistryIndex::Add(ClassRegistryItem *criIn,uint32 size){
	uint32 no = 0xFFFFFFFF;
	uint32 availableSize = AllocateFreeSlots();
	if (availableSize > 0){
		ClassRegistryIndexCell *cric = index[index.NumberOfUsedElements() - 1];
		if (cric != NULL){
			ClassRegistryBrief * crb = &((*cric)[cric->NumberOfUsedElements() - 1]);
			if (crb != NULL){
				crb->sizeOfClass = size;
				crb->cri = criIn;
				no = index.NumberOfUsedElements() << IndexBits + cric->NumberOfUsedElements() - 1;
			}
		}
	}
	return no;
}



}


