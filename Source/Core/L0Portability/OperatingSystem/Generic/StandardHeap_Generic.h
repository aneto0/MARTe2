/**
 * @file StandardHeap_Generic.h
 * @brief Header file for class StandardHeap_Generic
 * @date Aug 13, 2015
 * @author fsartori
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

 * @details This header file contains the declaration of the class StandardHeap_Generic
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L0PORTABILITY_OPERATINGSYSTEM_GENERIC_STANDARDHEAP_GENERIC_H_
#define SOURCE_CORE_L0PORTABILITY_OPERATINGSYSTEM_GENERIC_STANDARDHEAP_GENERIC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace HeapManager{
/**
 * @brief TBD
 */
class StandardHeap: public HeapI {

    /**
     * @brief minimum of address returned by malloc.
     */
    uintp firstAddress;
    /**
     * @brief maximum of address of last byte of memory returned by malloc
     */
    uintp lastAddress;
public:

    /**
     * @brief constructor
     */
    StandardHeap();    /**
     * @brief constructor
     */

    virtual ~StandardHeap();


    /**
     * @brief Allocates a portion of memory on the heap.
     * @param[in] size The size in byte of the memory to allocate.
     * @return The pointer to the allocated memory. NULL if allocation failed.
     */
    virtual void *Malloc(const uint32 size);

    /**
     * @brief Releases a memory area and sets its pointer to NULL.
     * @param[in,out] data The memory area to be freed.
     * @return true if the memory is freed, false in case of invalid pointer.
     * @post data = NULL
     */
    virtual void Free(void *&data);

    /**
     * @brief Reallocates a memory portion possibly contiguously with the specified already existent memory area.
     * @details If there is no space available for the new size specified, the system could allocate the new portion
     * in a different location and in this case the pointer changes its value.
     * @param[in,out] data The pointer to the new memory block.
     * @param[in] newSize The size of the new memory block.
     * @return The pointer to the new data block. NULL if reallocation failed.
     */
    virtual void *Realloc(void *&data,const uint32 newSize);

    /**
     * @brief Duplicates a memory section into a new area from the specified heap.
     * @param[in] data The pointer to the memory which must be copied.
     * @param[in] size size of memory to allocate. if size = 0 then memory is copied until a zero is found
     * @return The pointer to the new allocated memory which contains a copy of s.
     */
    virtual void *Duplicate(const void * const data, uint32 size=0U);

    /**
     * @brief start of range of memory addresses served by this heap.
     * @return first memory address
     */
    virtual uintp FirstAddress()const;

    /**
     * @brief end (inclusive) of range of memory addresses served by this heap.
     * @return last memory address
     */
    virtual uintp LastAddress()const;

    /**
     * @brief Returns the name of the heap
     * @return name of the heap
     */
    virtual const char8 *Name()const;

} ;


/*****************************************************************************/
/*                          CONDITIONAL SECTION                              */
/*****************************************************************************/

#if defined (GENERIC_STANDARD_HEAP_IMPLEMENTATION)


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief constructor
 */
StandardHeap::StandardHeap(){

    /** initialise memory addresses to NULL as we have no way to obtain this information until malloc is called */
    firstAddress = 0U;
    lastAddress  = 0U;
}
/**
 * @brief destructor
 */
StandardHeap::~StandardHeap(){
    lastAddress  = 0U;
    firstAddress = 0U;
};

/**
 * @brief allocates size bytes of data in the heap. Maximum allocated size is 4Gbytes
 * @return a pointer to the allocated memory or NULL if the allocation fails.
 */
void *StandardHeap::Malloc(const uint32 &size){
    //void *pointer = malloc(size);
    void *pointer = new char8[size];

    /*lint -e{9091} -e{923} the casting from pointer type to integer type is required
     * in order to be able to update the range of addresses provided by this heap
     * uintp is an integer type that has by design the same span as a pointer in all systems*/
    uintp address = reinterpret_cast<uintp>(pointer);
    if ((firstAddress > address ) || (firstAddress == 0U)){
        firstAddress = address;
    }
    address += size;
    if ((lastAddress  < address ) || (lastAddress  == 0U)) {
        lastAddress = address;
    }

    return pointer;

}

/**
 * @brief free the pointer data and its associated memory.
 * @param data the data to be freed.
 */
void StandardHeap::Free(void *&data){

    delete[] (reinterpret_cast<char8 *>(data));
    data = NULL_PTR(void *);
//    free(data);
}

/**
 * @brief start of range of memory addresses served by this heap.
 * @return first memory address
 */
uintp StandardHeap::FirstAddress()const {
    return firstAddress;
}

/**
 * @brief end (inclusive) of range of memory addresses served by this heap.
 * @return last memory address
 */
uintp StandardHeap::LastAddress()const {
    return lastAddress;
}

/**
 * @brief Returns the name of the heap
 * @return name of the heap
 */
const char8 *StandardHeap::Name()const {
    return "StandardHeap";
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#endif

}

#endif /* SOURCE_CORE_L0PORTABILITY_OPERATINGSYSTEM_GENERIC_STANDARDHEAP_GENERIC_H_ */
	
