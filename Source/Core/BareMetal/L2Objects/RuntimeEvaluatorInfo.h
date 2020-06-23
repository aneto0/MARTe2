/**
 * @file PCode_VariableInformation.h
 * @brief Header file for class AnyType
 * @date 8 Apr 2020
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

#ifndef RUNTIME_EVALUATOR_VARIABLE_INFORMATION_H_
#define RUNTIME_EVALUATOR_VARIABLE_INFORMATION_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"
#include "VariableDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace RuntimeEvaluatorInfo{

#if 0
/**
 * type used to store 1 PseudoCode element
 */
typedef uint16 CodeMemoryElement;

/**
 * type used to address 1 PseudoCode element
 */
typedef uint16 CodeMemoryAddress;

/**
 * type used to store 1 Data element in the stack and data area
 */
typedef uint32 DataMemoryElement;

/**
 * type used to address 1 Data element MUST be the same as a CodeMemoryElement
 */
typedef CodeMemoryElement DataMemoryAddress;

/**
 * used to mark an address to be invalid
 */
#define InvalidDataMemoryAddress TypeCharacteristics<RuntimeEvaluatorInfo::DataMemoryAddress>::MaxValue()

/**
 * used to mark an address to be invalid
 */
#define InvalidCodeMemoryElement TypeCharacteristics<RuntimeEvaluatorInfo::CodeMemoryElement>::MaxValue()


static inline DataMemoryAddress ByteSizeToDataMemorySize(uint32 byteSize){
    return static_cast<DataMemoryAddress>((byteSize + sizeof(DataMemoryElement) - 1U)/sizeof(DataMemoryElement));
}


/**
 *
 */
class VariablesMemory {

public:

	/**
	 * allocates the object T using the memory of this pool
	 * NOTE: T must be relocatable
	 * NOTE the destructor must be called explicitly
	 */
	template <class T>
	inline ErrorManagement::ErrorType       Allocate(DataMemoryAddress &location);

	/**
	 *
	 */
	inline ErrorManagement::ErrorType       AllocateBySize(uint32 byteSize, DataMemoryAddress &location);

	/**
	 * sets usedSize to 0
	 * allows reusing this object
	 */
	inline void                             Clean();

	/**
	 * Reads from Data Memory
	 */
	template<class T>
	inline  T &     		                Variable(DataMemoryAddress variableIndex);


	/**
	 * initialises it with minimum memory
	 */
	inline                                  VariablesMemory();

	/**
	 * frees all memory
	 * does not call destructors of objects created in this
	 */
    inline                                  ~VariablesMemory();

    /**
     *
     */
    inline DataMemoryAddress                GetUsedSize();

    /**
     * next address that will be used or the amount of DataMemoryElement used
     */
    inline DataMemoryAddress                NextAddress();
    /**
     * size expressed in RuntimeEvaluatorInfo::DataMemoryElement
     */
    inline ErrorManagement::ErrorType       RequestMemory(DataMemoryAddress size);

private:
    // how many RuntimeEvaluatorInfo::DataMemoryElement
    DataMemoryAddress 					allocatedSize;

    // how many RuntimeEvaluatorInfo::DataMemoryElement
    DataMemoryAddress 					usedSize;

	//
	DataMemoryElement *	                memory;

};

VariablesMemory::VariablesMemory(){
    allocatedSize   = 0;
    usedSize        = 0;
    memory = NULL_PTR(DataMemoryElement *);
}

VariablesMemory::~VariablesMemory(){
    if (memory != NULL_PTR(DataMemoryElement *)){
        HeapManager::Free((void *&)memory);
    }
}

DataMemoryAddress VariablesMemory::GetUsedSize(){
    return usedSize;
}

ErrorManagement::ErrorType VariablesMemory::RequestMemory(DataMemoryAddress size){
    ErrorManagement::ErrorType ret;
    if ((usedSize + size) >= allocatedSize){
        uint32 newAllocatedSize = allocatedSize + 64;
        memory = reinterpret_cast<DataMemoryElement *>(HeapManager::Realloc((void *&)memory, newAllocatedSize*sizeof(DataMemoryElement)));

        ret.outOfMemory = (memory == NULL_PTR(DataMemoryElement *));

        if (ret){
            allocatedSize = newAllocatedSize;
        } else {
            allocatedSize = 0;
        }
    }

    return ret;
}

template <class T>
ErrorManagement::ErrorType VariablesMemory::Allocate(DataMemoryAddress &location){
    ErrorManagement::ErrorType ret;
//    DataMemoryAddress ret = (DataMemoryAddress)-1;
    uint32 size = ByteSizeToDataMemorySize(sizeof(T));

    ret = RequestMemory(size);



    if (ret){

        T *pT = reinterpret_cast<T *>(new (reinterpret_cast<void *>(&memory[usedSize])) T);

        ret.outOfMemory = (pT == NULL_PTR(T*));
    }

    if (ret){
        location = usedSize;
        usedSize += size;
    }

    return ret;
}

ErrorManagement::ErrorType VariablesMemory::AllocateBySize(uint32 byteSize, DataMemoryAddress &location){
    ErrorManagement::ErrorType ret;
    uint32 size = ByteSizeToDataMemorySize(byteSize);

    ret = RequestMemory(size);

    if (ret){
        location = usedSize;
        usedSize += size;
    }

    return ret;
}


template<typename T>
T &VariablesMemory::Variable(DataMemoryAddress variableIndex){
    // note that variableIndex is an address to the memory with a granularity of sizeof(MemoryElement)
    return (T&)memory[variableIndex];
}

void VariablesMemory::Clean(){
    usedSize = 0;
}

DataMemoryAddress  VariablesMemory::NextAddress(){
    return usedSize;
}


/**
 * describes an external variable
 * used also in data stack when handling matrices
 */
struct ExternalVariableInformation{

	/**
	 * where to read the variable from
	 */
	void *				location;

	/**
	 * if the type is that of a matrix
		here are the 2 dimensions
	 */
	uint32 				dimensions[2];

};

/**
 *  Element for the list of variables
 */
struct VariableInformation {
	/**
	 * name of the variable.
	 */
	DynamicCString 				name;

	/**
	 * either a numeric type or a Matrix<float> or Matrix<double>
	 */
	VariableDescriptor 			type;

	/**
	 * location of the variable in the data area.
	 * if location is below the start of the variables then
	 * it is a constant
	 */
	DataMemoryAddress 			location;

	/**
	 * where to read the variable from
	 */
	ExternalVariableInformation external;

	/**
	 * set when during compilation to mark that this output variable has already been initialised : location is correct
	 */
//	bool 				        variableUsed;

	/**
	 * The memory pointed to by external has been allocated
	 * and needs to be freed
	 */
	mutable bool 				memoryAllocated;

	/**
        RuntimeEvaluatorFunction &fr = functionRecords[pCode];
	 * initialises data
	 */
	inline                              VariableInformation();

	/**
	 * frees memory
	 */
	inline                              ~VariableInformation();

	/**
	 * copies from and takes memory freeing responsibilities from in
	 */
	inline                              VariableInformation(const VariableInformation &in);

	/**
	 * allocates memory
	 */
	inline ErrorManagement::ErrorType   AllocateMatrixMemory();

	/*
	 * return
	 *
	 */
	inline SaturatedInteger<uint32>     CalculateMatrixStorageSize();

	/**
	 *
	 */
	inline bool                         isAllocated();

	/**
	 *
	 */
	inline bool                         isExternalMemoryAllocated();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

VariableInformation::VariableInformation(){
	type 					= 	VoidType;
	location 				= 	InvalidDataMemoryAddress;
	memoryAllocated 		= 	false;
	external.location		=	NULL;
	external.dimensions[0]	=	1;
	external.dimensions[1]	=	1;
}

VariableInformation::~VariableInformation(){
	if (memoryAllocated  && (external.location != NULL)){
		HeapManager::Free(external.location);
	}
}

bool VariableInformation::isAllocated(){
    return (location != InvalidDataMemoryAddress);
}


bool VariableInformation::isExternalMemoryAllocated(){
    return memoryAllocated;
}


SaturatedInteger<uint32> VariableInformation::CalculateMatrixStorageSize(){
    SaturatedInteger<uint32> storageSize = 0;

    storageSize = type.GetFinalTypeDescriptor().StorageSize();
    storageSize *= external.dimensions[0];
    storageSize *= external.dimensions[1];

    return storageSize;
}

inline ErrorManagement::ErrorType VariableInformation::AllocateMatrixMemory(){
	ErrorManagement::ErrorType ret;
	SaturatedInteger<uint32>  size;

	if (external.location != NULL){
		ret.invalidOperation = (memoryAllocated == false);

		if (ret){
			HeapManager::Free(external.location);
			memoryAllocated = false;
		}
	}

	size = CalculateMatrixStorageSize();

	uint32 actualSize;
	ret = size.ToNumber(actualSize);

	void * address;
	if (ret){
		address = HeapManager::Malloc(actualSize);

		ret.outOfMemory = (address == NULL);
	}

	if (ret){
		external.location = address;
		memoryAllocated = true;
	}


	return ret;
}


inline VariableInformation::VariableInformation(const VariableInformation &in){
	name 			= in.name;
	type 			= in.type;
	location 		= in.location;
	memoryAllocated = in.memoryAllocated;
	external		= in.external;
	in.memoryAllocated = false;
}
#endif

} // PseudoCode
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_PCODE_VARIABLEINFORMATION_H_ */
