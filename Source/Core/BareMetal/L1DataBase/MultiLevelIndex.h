/**
 * @file IndexerI.h
 * @brief Header file for class AnyType
 * @date 21 Apr 2018
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

#ifndef MULTILEVEL_INDEX_H_
#define MULTILEVEL_INDEX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ErrorType.h"
#include "../L1Portability/HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class MultiLevelIndex;

/**
 * @brief common component for MultilevelIndex and MultilevelIndexT
 */
class MultiLevelIndexCore{
protected:
	/**
	 * @brief constructor to be used by dynamically constructed MultiLevelIndex
	 *  levelSizeBits is set by the MultiLevelIndex::new operator consistently with the memory allocated
	 */
	MultiLevelIndexCore(uint8 nLevels);

	/**
	 * @brief constructor to be used by statically constructed MultiLevelIndexT
	 */
	MultiLevelIndexCore(uint8 nLevels,uint8 levelSizeBitsIn);

public:

	/**
	 * @brief to be used by derived classes
	 */
	union SubtreeInfo {
		/**
		 * @brief used at leafs: points to the data
		 */
		void *data;

		/**
		 * @brief used by nodes: points to nodes or leafs
		 */
		MultiLevelIndex *nextLevel;
	};

	/**
	 * @brief the current node information
	 */
	class  ParametersInfo{
	public:
		/**
		 * @brief size of the level as bits
		 * it is not const as it is inizialised by the custom new operator
		 */
		 uint8 levelSizeBits;

		/**
		 * @brief How many elements this level
		 */
		uint16 levelSize;

		/**
		 * @brief allows limiting a number within [0..levelSize) by masking
		 * result is bound if levelSizeBits <=8
		 */
		const uint8 indexMask;

		/**
		 * @brief how many levels below this. 0 means leaf node
		 */
		const uint8 nLevels;

		/**
		 * @brief how many free slots. 1 bit for each slot
		 * 1 means subtree in use
		 * 0 means subtree absent
		 */
		uint8 freeSlotMask;

		/**
		 * @brief how many full subtrees. 1 bit for each slot
		 * 1 means subtree is full
		 * 0 means subtree has spaces
		 */
		uint8 fullSlotMask;

		/**
		 *	@brief constructor
		 */
		ParametersInfo(uint8 nLevelsIn,uint8 levelSizeBitsIn);

		/**
		 * @return true if the subtree hanging from this slot is absent
		 */
		inline bool isUnused(uint8 slot);

		/**
		 * no subtrees below this level?
		 */
		inline bool noSubTrees();

		/**
		 * no subtrees with free spaces ?
		 */
		inline bool isFull();

		/*
		 * set the attributes for a subtree
		 */
		inline void setSlot(uint8 slot,bool inUse, bool isFull);

		/**
		 * which slot has subtree with some spaces (or no subtree)? Starting from 0
		 */
		inline uint8 firstFreeSpaceSlot();

		/**
		 * which slot is unused ? Starting from 0
		 */
		inline uint8 firstEmptySlot();

		/**
		 * how many slots in use?
		 */
		inline uint8 usedSlots();

		/**
		 * how many slots in use?
		 */
		template <typename indexType>
		void *Read(indexType index,SubtreeInfo *sub);

		/**
		 * how many slots in use?
		 */
		template <typename indexType>
		ErrorManagement::ErrorType Store(void *dataIn,indexType &index,SubtreeInfo *sub);

		/**
		 * how many slots in use?
		 */
		template <typename indexType>
		ErrorManagement::ErrorType  FreeIndex(indexType index,SubtreeInfo *sub);

	} parameters;
};


/**
 * @brief
 */
class MultiLevelIndex: public MultiLevelIndexCore{
public:

	/**
	 * @brief
	 */
	static MultiLevelIndex * CreateInstance(uint8 nLevels,uint8 levelSizeBits);

	/**
	 * @brief
	 */
	template <typename indexType>
	void *operator[](indexType index);

	/**
	 * @brief
	 */
	template <typename indexType>
	ErrorManagement::ErrorType Store(void *dataIn,indexType &index);

	/**
	 * @brief
	 */
	template <typename indexType>
	ErrorManagement::ErrorType  FreeIndex(indexType index);

private:

	/**
	 * @brief
	 */
	MultiLevelIndex(uint8 nLevels);

	/**
	 * @brief
	 */
	void* operator new(size_t size, uint8 levelSizeBits=1);

private:
	SubtreeInfo subtrees[];
};


/**
 * @brief
 */
template<uint8 levelSizeBits>class MultiLevelIndexT: public MultiLevelIndexCore{

public:
	/**
	 * @brief
	 */
	MultiLevelIndexT(uint8 nLevels);

	/**
	 * @brief
	 */
	template <typename indexType>
	void *operator[](indexType index);

	/**
	 * @brief
	 */
	template <typename indexType>
	ErrorManagement::ErrorType Store(void *dataIn,indexType &index);

	/**
	 * @brief
	 */
	template <typename indexType>
	ErrorManagement::ErrorType  FreeIndex(indexType index);

private:

	/**
	 * @brief
	 */
	SubtreeInfo subtrees[1 << levelSizeBits];

};

/*---------------------------------------------------------------------------*/
/*                           Class implementation                            */
/*---------------------------------------------------------------------------*/

MultiLevelIndexCore::MultiLevelIndexCore(uint8 nLevels):parameters(nLevels,((ParametersInfo *)this)->levelSizeBits){
}

MultiLevelIndexCore::MultiLevelIndexCore(uint8 nLevels,uint8 levelSizeBitsIn):parameters(nLevels,levelSizeBitsIn){
}

MultiLevelIndexCore::ParametersInfo::ParametersInfo(uint8 nLevelsIn,uint8 levelSizeBitsIn):
	levelSizeBits(levelSizeBitsIn),
	levelSize(1 << levelSizeBits),
	indexMask(static_cast<uint8>(levelSize - 1)),
	nLevels(nLevels)
{
	freeSlotMask = 0;
	fullSlotMask = 0;
}

/**
 * is the subtree hanging from this slot absent
 */
bool MultiLevelIndexCore::ParametersInfo::isUnused(uint8 slot){
	return ((freeSlotMask & (1 << slot))==0);
}

/**
 * no subtrees below this level?
 */
bool MultiLevelIndexCore::ParametersInfo::noSubTrees(){
	return (freeSlotMask ==0);
}

/**
 * no subtrees with free spaces ?
 */
bool MultiLevelIndexCore::ParametersInfo::isFull(){
	return (fullSlotMask == indexMask);
}

/*
 * set the attributes for a subtree
 */
void MultiLevelIndexCore::ParametersInfo::setSlot(uint8 slot,bool inUse, bool isFull){
	uint8 ix = 1 << slot;
	if (inUse){
		freeSlotMask |= ix;
	} else {
		freeSlotMask &= ~ix;
	}
	if (isFull){
		fullSlotMask |= ix;
	} else {
		fullSlotMask &= ~ix;
	}
}

/**
 * which slot has subtree with some spaces (or no subtree)? Starting from 0
 */
uint8 MultiLevelIndexCore::ParametersInfo::firstFreeSpaceSlot(){
	return pETable[fullSlotMask];
}

/**
 * which slot is unused ? Starting from 0
 */
uint8 MultiLevelIndexCore::ParametersInfo::firstEmptySlot(){
	return pETable[freeSlotMask];
}

/**
 * how many slots in use?
 */
uint8 MultiLevelIndexCore::ParametersInfo::usedSlots(){
	return zCTable[~freeSlotMask];
}

template <typename indexType>
void *MultiLevelIndexCore::ParametersInfo::Read(indexType index,SubtreeInfo *sub){
	void *ret = NULL;
	if (nLevels > 0){
		uint8 slot =  index && indexMask;

		if (!isUnused(slot)){
			// to complete
			ret = sub[slot].nextLevel->operator[](index >> levelSizeBits);
		}
	} else {
		if ((index >= 0) && (index < levelSize)){
			ret = sub[index].data;
		}
	}

	return ret;
}

template <typename indexType>
ErrorManagement::ErrorType MultiLevelIndexCore::ParametersInfo::Store(void *dataIn,indexType &index,SubtreeInfo *sub){
	ErrorManagement::ErrorType ret;

	if (nLevels > 0){
		uint8 slot =  firstFreeSpaceSlot();
		ret.outOfRange = (slot >= levelSize);

		if (ret){
			if (sub[slot].nextLevel == NULL){
				sub[slot].nextLevel = MultiLevelIndex::CreateInstance(nLevels-1,levelSizeBits);
				ret.outOfMemory = (sub[slot].nextLevel == NULL);
			}

			if (ret){
				ret = sub[slot].nextLevel->Store(dataIn,index);

				if (ret){
					index = index << levelSizeBits;
					index = index + slot;

					setSlot(slot,true,sub[slot].nextLevel->parameters.isFull());
				}
			}
		}
	} else {
		uint8 slot =  firstEmptySlot();
		ret.outOfRange = (slot >= levelSize);

		if (ret){
			sub[slot].data = dataIn;
			index = slot;
			setSlot(slot,true,true);
		}
	}

	return ret;
}

template <typename indexType>
ErrorManagement::ErrorType  MultiLevelIndexCore::ParametersInfo::FreeIndex(indexType index,SubtreeInfo *sub){
	ErrorManagement::ErrorType ret;

	if (nLevels > 0){
		uint8 slot =  index && indexMask;

		ret.fatalError = isUnused(slot);

		if (ret){
			ret = sub[slot].nextLevel->FreeIndex(index >>levelSizeBits);
		}

		if (ret){
			if (sub[slot].nextLevel->parameters.noSubTrees()){
				delete sub[slot].nextLevel;
				sub[slot].nextLevel = NULL;
				// no subtree - certainly not full
				setSlot(slot,false,false);
			} else {
				// certainly not full anymore!
				setSlot(slot,true,false);
			}
		}
	} else {
		if ((index >= 0) && (index < levelSize)){
			setSlot(index,false,false);
		}
	}

	return ret;
}

MultiLevelIndex::MultiLevelIndex(uint8 nLevels):MultiLevelIndexCore(nLevels){
	//TODO init subTrees[]
}


template <typename indexType>
void *MultiLevelIndex::operator[](indexType index){
	return parameters.Read(index,subtrees);
}

template <typename indexType>
ErrorManagement::ErrorType MultiLevelIndex::Store(void *dataIn,indexType &index){
	return parameters.Store(dataIn,index,subtrees);
}

template <typename indexType>
ErrorManagement::ErrorType MultiLevelIndex::FreeIndex(indexType index){
	return parameters.FreeIndex(index,subtrees);
}


#include <stdio.h>


void* MultiLevelIndex::operator new(size_t size, uint8 levelSizeBits){
	size_t totalSize = size + (1 << levelSizeBits)*sizeof(SubtreeInfo);
//	printf ("[size=%i,bits=%i]==> total=%i ",size,levelSizeBits,totalSize);
	void *p = malloc(totalSize);
	if (p != NULL){
		((MultiLevelIndex *)p)->parameters.levelSizeBits = levelSizeBits;
	}
	return p;
}

MultiLevelIndex * MultiLevelIndex::CreateInstance(uint8 nLevels,uint8 levelSizeBits){
	return new(levelSizeBits) MultiLevelIndex(nLevels);
}

template<uint8 levelSizeBits>
MultiLevelIndexT<levelSizeBits>::MultiLevelIndexT(uint8 nLevels):MultiLevelIndexCore(nLevels,levelSizeBits){
}

template<uint8 levelSizeBits>
template <typename indexType>
void *MultiLevelIndexT<levelSizeBits>::operator[](indexType index){
	return parameters.Read(index,subtrees);
}

template<uint8 levelSizeBits>
template <typename indexType>
ErrorManagement::ErrorType MultiLevelIndexT<levelSizeBits>::Store(void *dataIn,indexType &index){
	return parameters.Store(dataIn,index,subtrees);
}

template<uint8 levelSizeBits>
template <typename indexType>
ErrorManagement::ErrorType MultiLevelIndexT<levelSizeBits>::FreeIndex(indexType index){
	return parameters.FreeIndex(index,subtrees);
}

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1DATABASE_INDEXERI_H_ */
