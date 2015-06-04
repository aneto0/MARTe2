
/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 * @file
 * @brief Basic memory management in Linux
 */
#ifndef MEMORY_OS_H
#define MEMORY_OS_H

#include "../../GeneralDefinitions.h"

/**
 * @see MemoryMalloc
 * @brief Allocate dinamically a portion of memory.
 * @param size is the number of bytes to allocate.
 * @param allocFlags 
 * @return a pointer to the allocated memory.
 */
void *MemoryOSMalloc(uint32 size, MemoryAllocationFlags allocFlags) {
	//don't use heap allocation for embedded systems.
    return NULL;
}

/** 
 * @see MemoryFree
 * @brief Free a portion of memory previously dinamically allocated.
 * @param data is the pointer to the memory to be freed.
 */
void MemoryOSFree(void *&data) {

}

/** 
 * @see MemoryRealloc
 * @brief Reallocate the memory.
 * @param data is the pointer to the memory which must be reallocated.
 * @param newSize is the desired new size for the memory portion.
 */
void *MemoryOSRealloc(void *&data, uint32 newSize) {
	return NULL;
}

/** 
 * @see MemoryStringDup
 * @brief Duplicate a string.
 * @param s is the string to duplicate.
 * @return a pointer to the string copy.
 */
char *MemoryOSStringDup(const char *s) {
    return NULL;
}

/** 
 * @see MemorySharedAlloc
 * @brief Allocate a shared memory.
 * @param key is the identifier of the shared memory.
 * @param size is the desired size of the shared memory.
 * @param permMask define the process permissions.
 * @return a pointer to the shared memory created.
 */
void *MemoryOSSharedAlloc(uint32 key, uint32 size, uint32 permMask) {
	return NULL; 
}

/** 
 * @see MemorySharedFree
 * @brief Free a shared memory portion.
 * @param address is a pointer to the shared memory.
 */
void MemoryOSSharedFree(void *address) {
}

/**
 * @see MemoryMemoryCheck
 * @brief Currently not implemented in Linux
 * @return true.
 */
bool MemoryOSCheck(void *address, MemoryTestAccessMode accessMode,
                   uint32 size) {
    return True;
}

/** @see MemoryCopy. 
  * @brief Copy source to destination.
  * @param destination is a pointer to the destination memory.
  * @param source is a pointer to the source memory.
  * @param is the number of bytes to copy.
  * @return true if source, destination and destination after the copy are not NULL. */
bool MemoryOsCopy(void* destination, const void* source, uint32 size) {
	char* byteDest=(char*) destination;
	const char* byteSource=(const char*) source;
	for(uint32 i=0; i<size; i++){
		if(byteSource==NULL || byteDest==NULL){
			return False;
		}
		*byteDest++=*byteSource++;
	}
	return True;
}

/** @see MemoryCompare.
  * @brief Compare two memory portions.
  * @param mem1 is a pointer to the first memory.
  * @param mem2 is a pointer to the second memory.
  * @param size is the size to compare.
  * @return (0 if mem1 < mem2), (1 if mem1 < mem2), (2 if mem1 > mem2), (-1 if one of memory arguments is NULL). */
int32 MemoryOsCompare(const void* mem1, const void* mem2, uint32 size) {

	const char* byteMem1=(const char*) mem1;
	const char* byteMem2=(const char*) mem2;

	for(uint32 i=0; i<size; i++, byteMem1++, byteMem2++){
		if(byteMem1==NULL || byteMem2==NULL){
			return -1;
		}
		if(*byteMem1 > *byteMem2){
			return 2;
		}
		if(*byteMem1 < *byteMem2){
			return 1;
		}
	}

	return 0;
}

/** @see MemorySearch.
  * @brief Search a byte in the memory.
  * @param mem is the memory.
  * @param c is the element to search.
  * @param size is the number of bytes to check.
  * @return a pointer to the first occurence of c in mem, NULL if is not found or in case of mem=NULL.  */
void* MemoryOsSearch(void* mem, int32 c, uint32 size) {

	char* byteMem=(char*)mem;
	for(uint32 i=0; i<size; i++){
		if(byteMem==NULL){
			return NULL;
		}
		if(*byteMem++==c){
			return byteMem-1;
		}
	}

	return NULL;
}

/** @see MemoryMove.
  * @brief Copy size bytes from source to destination.
  * @param destination is a pointer to the destination memory.
  * @param source is a pointer to source memory.
  * @param size is the number of bytes to copy.
  * @return true if source, destination and destination after the copy are not NULL. */
bool MemoryOsMove(void* destination, const void* source, uint32 size) {
	return MemoryOsCopy(destination, source, size);
}

/** @see MemorySet.
  * @brief Copy c in the first bytes of a memory.
  * @param mem is a pointer to the memory.
  * @param c is the byte to copy.
  * @param size is the number of bytes to set to c.
  * @return true if mem before and after the operation id not NULL. */
bool MemoryOsSet(void* mem, int32 c, uint32 size) {
	
	char* byteMem=(char*)mem;

	for(uint32 i=0; i<size; i++){
		if(byteMem==NULL){
			return False;
		}
		*byteMem++=c;
	}

	return True;

}

#endif

