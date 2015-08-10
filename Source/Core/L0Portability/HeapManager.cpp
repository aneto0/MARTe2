/**
 * @file HeapManager.cpp
 * @brief Source file for class HeapManager
 * @date Aug 7, 2015
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

 * @details This source file contains the definition of all the methods for
 * the class HeapManager (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManager.h"

#include "HeapInterface.h"

#include "StringPortable.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static class HeapDataBase {
    /**
     * @brief Lists all heaps
     * all unused heaps have a NULL pointer
     */
    HeapInterface *heaps[MaximumNumberOfHeaps];


public:

    /**
     * true --> Pointer is inside current address range
     * */
    HeapInterface *GetHeap(int index)const{
        HeapInterface *returnValue = NULL;
        if ((index >= 0) && (index < MaximumNumberOfHeaps)){
            returnValue = heaps[index];
        }
        return returnValue;
    }

    /**
     * true --> Pointer is inside current address range; slot is free and heap is not NULL
     * sets slot to heap
     * */
    bool SetHeap(int index, const HeapInterface *heap){
        bool ok = false;
        if ((index >= 0) && (index < MaximumNumberOfHeaps)){
            if ((heaps[index] == NULL) && (heap != NULL)){
                heaps[index] = heap;
                ok = true;
            }
        }
        return ok;
    }

    /**
     * true --> Pointer is inside current address range; slot is same as heap
     * sets slot to NULL
     * */
    bool UnsetHeap(int index, const HeapInterface *heap){
        bool ok = false;
        if ((index >= 0) && (index < MaximumNumberOfHeaps)){
            if (heaps[index] = heap ){
                heaps[index] = NULL;
                ok = true;
            }
        }
        return ok;
    }

    /**
     * true --> Pointer is inside current address range
     * */
    HeapDataBase(){

        int i=0;
        for(i=0;i<MaximumNumberOfHeaps;i++){
            heaps[i] = NULL;
        }
    }

} heapDataBase;


static class StandardHeap: public HeapInterface {

    /**
     * @brief minimum of address returned by malloc.
     */
    uint8 *firstAddress;
    /**
     * @brief maximum of address of last byte of memory returned by malloc
     */
    uint8 *lastAddress;
public:

    /**
     * @brief No operation.
     */
    StandardHeap(){

        /** initialise memory addresses to NULL as we have no way to obtain this information until malloc is called */
        firstAddress = static_cast<uint8 *>  (malloc(1));
        lastAddress  = firstAddress+1;
    }
    /**
     * @brief No operation.
     */
    virtual ~StandardHeap(){
        free (static_cast<void *>  (firstAddress));
        firstAddress = static_cast<uint8 *>  (NULL);
        lastAddress  = static_cast<uint8 *>  (NULL);
    };

    /**
     * @brief allocates size bytes of data in the heap. Maximum allocated size is 4Gbytes
     * @return a pointer to the allocated memory or NULL if the allocation fails.
     */
    virtual void *Malloc(const uint32 &size){
        return malloc(size);
    }

    /**
     * @brief free the pointer data and its associated memory.
     * @param data the data to be freed.
     */
    virtual void Free(void *&data){
        free(data);
    }

    /**
     * @brief start of range of memory addresses served by this heap.
     * @return first memory address
     */
    virtual uint8* FirstAddress()const {
        return firstAddress;
    }

    /**
     * @brief end (inclusive) of range of memory addresses served by this heap.
     * @return last memory address
     */
    virtual uint8* LastAddress()const {
        return lastAddress;
    }

    /**
     * @brief Returns the name of the heap
     * @return name of the heap
     */
    virtual const char *Name()const {
        return "StandardHeap";
    }

} standardHeap;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HeapInterface *HeapManager::GetStandardHeap(){

    return &standardHeap;
}


HeapInterface *HeapManager::FindHeap(const void * address){

    /**
     * used to mark that the heap was not found
     */
    const int notFound = -1;
    /**
     * where the heap was found
     * */
    int foundIndex = notFound;
    /**
     address range of currently found heap
     */
    int64 foundSpan  = 0;

    /**
     * no error encountered
     */
    bool ok = true;

    /**
     * the search will set this pointer to point to the heap found
     * by default return the standard heap
     */
    HeapInterface *heap = NULL;

    int i=0;
    for(i=0;(i<MaximumNumberOfHeaps) && ok ;i++){

        /** retrieve heap information in current slot */
        heap = heapDataBase.GetHeap(i);

        /** if slot used */
        if (heap != NULL){

            /* check address compatibility */
            if ( heap->Owns(address)){

                /** check if first occurrence or */
                if (foundIndex == notFound){

                    /* save position of found heap */
                    foundIndex = i;

                    /* size of memory space */
                    foundSpan = ((char *)heap->LastAddress()-(char *)heap->FirstAddress());

                } else { /* further occurrences */

                    /* size of memory space */
                    int newFoundSpan = ((char *)heap->LastAddress()-(char *)heap->FirstAddress());

                    /**
                     * smaller memory span and intersecting memory address space
                     * it can only mean that this heap is a sub-heap of the previously found heap
                     */
                    if (newFoundSpan < foundSpan){

                        /* save size of memory space */
                        foundSpan = newFoundSpan;

                        /* save position of found heap */
                        foundIndex = i;

                    } // end if (newFoundSpan < foundSpan)

                } /* end of check first occurrence */

            } /* end check address compatibility */

        } /* end if heap != NULL */

    } /* end i loop */

    /* assign to heap the found heap or the default one */
    if (foundIndex == notFound) {

        /** try default heap */
        heap = GetStandardHeap();

        /* check ownership of default heap */
        if (! heap->Owns(address) ){
            heap = NULL;
        }

    } else {
        heap = heapDataBase.GetHeap(foundIndex);
    }

    return heap;
}

HeapInterface *HeapManager::FindHeap(const char *name){

    bool ok = (name != NULL);

    /**
     * found heap
     */
    bool found = false;

    /**
     * the search will set this pointer to point to the heap found
     */
    HeapInterface *foundHeap = NULL;

    if (ok){
        int i=0;
        for(i=0;(i<MaximumNumberOfHeaps) && !found ;i++){

            /** retrieve heap information in current slot */
            HeapInterface *heap = heapDataBase.GetHeap(i);

            /** if slot used */
            if (heap != NULL){

                /* check address compatibility */
                if( StringPortable::Compare (heap->Name(),name) == 0 ){

                    found = true;

                    foundHeap = heap;


                } /* end check name */

            } /* end if heap != NULL */

        } /* end i loop */
    }

    return foundHeap;
}


bool HeapManager::Free(void *&data){
    HeapInterface *heap = FindHeap(data);

    bool ok = false;

    /** Does not belong to any heap?*/
    if (heap != NULL){

        heap->Free(data);

        ok = true;

    } else {


        // error message here

    }

    return ok;
}

void *HeapManager::Malloc(uint32 size, const char *name){

    void *address = NULL;

    if (name == NULL){
        address = standardHeap.Malloc(size);
    } else {

        // find by name !

    }


    return address;
}

bool HeapManager::AddHeap(HeapInterface *newHeap){

    bool ok = true;

    /* check value of heap not to be NULL */
    if (newHeap == NULL){
        ok = false;
    }


    int i = 0;
    /* check if not registered already */
    for(i=0;(i<MaximumNumberOfHeaps) && ok ;i++){
        /** retrieve heap information in current slot */
        HeapInterface *heap = heapDataBase.GetHeap(i);

        /** already found */
        if (heap == newHeap) {
            ok = false;

            /* error message here */
        }
    }

    /* check if space available and if so register it  */
    if (ok) {

        bool found = false;
        /* for each slot  */
        for(i=0;(i<MaximumNumberOfHeaps) && !found ;i++){
            /** try to set each slot */
            found = heapDataBase.SetHeap(i, newHeap);
        }

        /** no more space */
        if (!found){
            ok = false;
            // error message here
        }

    }

    return ok;
}

bool HeapManager::RemoveHeap(HeapInterface *heap){

    bool found = false;

    int i = 0;
    /* check if not registered already */
    for(i=0;(i<MaximumNumberOfHeaps) && !found ;i++){
        /** retrieve heap information in current slot */
        found = heapDataBase.UnsetHeap(i,heap);
    }

    return found;
}

