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

#include "GeneralDefinitions.h"

#include "HeapManager.h"

#include "HeapI.h"

#include "StringHelper.h"

#include "FastPollingMutexSem.h"


#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,StandardHeap.h)


/*---------------------------------------------------------------------------*/
/*                           Local Class declaration                         */
/*---------------------------------------------------------------------------*/

namespace HeapManager /*Internals*/{

/**
 * @brief TODO
 */
static const int32 MaximumNumberOfHeaps = 16;

/**
 * @brief TODO
 */
class HeapDataBase {
    /**
     * @brief Lists all heaps
     * all unused heaps have a NULL pointer
     */
    HeapI *     heaps[MaximumNumberOfHeaps];

    /**
     * @brief TODO
     */
    FastPollingMutexSem mux;

public:

    /**
     * @brief gets the HeapI contained in a given slot of the database
     * @param index indicates the slots of the database
     * @return NULL index out of range or if empty slot
     * */
    HeapI *GetHeap(int32 index)const;

    /**
     * @brief sets the HeapI in a given slot of the database
     * @param index indicates the slots of the database
     * @param heap  is the desired heap to store
     * @return true if index is within range; specified slot is free; and heap is not NULL
     * */
    bool SetHeap(int32 index, HeapI *const heap);

    /**
     * @brief sets to NULL a given slot of the database
     * @param index indicates the slots of the database
     * @param heap must contain the same value as in the database
     * @return true if index is within range; specified slot contains heapl heap is not NULL
     * */
    bool UnsetHeap(int32 index, const HeapI *heap);
    /**
     * @brief constructor
     * */
    HeapDataBase();
    /**
     * @brief locks access to database
     * @return true if locking successful
     * */
    bool Lock();

    /**
     * @brief unlocks access to database
     * */
    void UnLock();

};


/**
 * @brief TODO
 */

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/**
 * @brief TODO
 */
static HeapDataBase heapDataBase;

/**
 * @brief TODO
 */
static StandardHeap standardHeap;



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


/**
 * @brief gets the HeapI contained in a given slot of the database
 * @param index indicates the slots of the database
 * @return NULL index out of range or if empty slot
 * */
HeapI *HeapDataBase::GetHeap(const int32 index)const{
    HeapI *returnValue = NULL_PTR(HeapI *);
    if ((index >= 0) && (index < MaximumNumberOfHeaps)){
        returnValue = heaps[index];
    }
    return returnValue;
}

/**
 * @brief sets the HeapI in a given slot of the database
 * @param index indicates the slots of the database
 * @param heap  is the desired heap to store
 * @return true if index is within range; specified slot is free; and heap is not NULL
 * */
bool HeapDataBase::SetHeap(const int32 index, HeapI * const heap  ){
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
 * @brief sets to NULL a given slot of the database
 * @param index indicates the slots of the database
 * @param heap must contain the same value as in the database
 * @return true if index is within range; specified slot contains heapl heap is not NULL
 * */
bool HeapDataBase::UnsetHeap(const int32 index, const HeapI * const heap){
    bool ok = false;
    if ((index >= 0) && (index < MaximumNumberOfHeaps)){
        if (heaps[index] == heap ){
            heaps[index] = NULL_PTR(HeapI *);
            ok = true;
        }
    }
    return ok;
}

/**
 * @brief constructor
 * */
HeapDataBase::HeapDataBase(){

    int32 i;
    for(i=0;i<MaximumNumberOfHeaps;i++){
        heaps[i] = NULL_PTR(HeapI *);
    }
}

/**
 * @brief locks access to database
 * @return true if locking successful
 * */
bool HeapDataBase::Lock(){
    return (mux. FastLock()==NoError);
}

/**
 * @brief unlocks access to database
 * */
void HeapDataBase::UnLock(){
    mux.FastUnLock();
}

HeapI *GetStandardHeap(){
    return  &standardHeap;
}

/**
 * @brief Finds the Heap that manages the specified memory location
 * @param address is a memory address that the target heap should manage
 * returns NULL if not found
 */
HeapI *FindHeap(const void * const address){

    /**
     address range of currently found heap
     */
    uintp foundSpan  = 0U;

    /**
     * the search will set this pointer to point to the heap found
     * by default return the standard heap
     */
    HeapI *foundHeap = NULL_PTR(HeapI *);

    /* controls access to database */
    if (heapDataBase.Lock()){
        // TODO add error message here

        int32 i;
        for(i=0;(i<MaximumNumberOfHeaps)  ;i++){

            /** retrieve heap information in current slot */
            HeapI *heap = heapDataBase.GetHeap(i);

            /** if slot used */
            if (heap != NULL_PTR(HeapI *)){

                /* check address compatibility */
                if ( heap->Owns(address)){

                    /** check if first occurrence or */
                    if (foundHeap == NULL_PTR(HeapI *)){

                        /* size of memory space */
                        foundSpan = heap->LastAddress() - heap->FirstAddress();

                        /* save heap */
                        foundHeap = heap;

                    } else { /* further occurrences */

                        /* size of memory space */
                        uintp newFoundSpan = heap->LastAddress() - heap->FirstAddress();

                        /**
                         * smaller memory span and intersecting memory address space
                         * it can only mean that this heap is a sub-heap of the previously found heap
                         */
                        if (newFoundSpan < foundSpan){

                            /* save size of memory space */
                            foundSpan = newFoundSpan;

                            /* save heap */
                            foundHeap = heap;
                        } // end if (newFoundSpan < foundSpan)

                    } /* end of check first occurrence */

                } /* end check address compatibility */

            } /* end if heap != NULL */

        } /* end i loop */


        heapDataBase.UnLock();
    } else {
        // TODO add error message here
    }

    /* assign to heap the found heap or the default one */
    if (foundHeap == NULL_PTR(HeapI *)) {

        /** try default heap */
        foundHeap = &standardHeap;

        /* check ownership of default heap */
        if (! foundHeap->Owns(address) ){
            foundHeap = NULL_PTR(HeapI *);
        }

    }

    return foundHeap;
}

HeapI *FindHeap(const char8 * const name){

    bool ok = (name != NULL);

    /**
     * found heap
     */
    bool found = false;

    /**
     * the search will set this pointer to point to the heap found
     */
    HeapI *foundHeap = NULL_PTR(HeapI *);

    if (ok){

        /* controls access to database */
        if (heapDataBase.Lock()){
            // TODO add error message here
            int32 i;
            for(i=0;(i<MaximumNumberOfHeaps) && (!found) ;i++){

                /** retrieve heap information in current slot */
                HeapI *heap = heapDataBase.GetHeap(i);

                /** if slot used */
                if (heap != NULL_PTR(HeapI *)){

                    /* check address compatibility */
                    if( StringHelper::Compare (heap->Name(),name) == 0 ){

                        found = true;

                        foundHeap = heap;


                    } /* end check name */

                } /* end if heap != NULL */

            } /* end i loop */
            heapDataBase.UnLock();
        }
    }

    return foundHeap;
}


bool Free(void *&data){
    HeapI *heap = FindHeap(data);

    bool ok = false;

    /** Does not belong to any heap?*/
    if (heap != NULL_PTR(HeapI *)){

        heap->Free(data);

        ok = true;

    } else {


        // TODO error message here

    }

    return ok;
}

void *Malloc(uint32 const size, const char8 * const name){

    void *address = NULL_PTR(void *);

    if (name == NULL){
        address = standardHeap.Malloc(size);
    } else {
        // TODO comment
        HeapI *heap = FindHeap(name);

        if (heap != NULL){
            address =  heap->Malloc(size);
        }

    }

    return address;
}

void *Realloc(void *&data,const uint32 newSize){
    void *newAddress = NULL_PTR(void *);

    HeapI *chosenHeap = FindHeap(data);

    if (chosenHeap != NULL){
        newAddress = chosenHeap->Realloc(data,newSize);
    }
    //if the heap is not found (the data is null) allocates it on the standard heap.
    else{
        standardHeap.Realloc(data,newSize);
    }

    return newAddress;
}

void *Duplicate(const void * const data, uint32 size, const char8 *heapName){
    void *newAddress = NULL_PTR(void *);

    HeapI *chosenHeap = NULL_PTR(HeapI *);

    if (heapName != NULL){
        chosenHeap = FindHeap(heapName);

    }

    if (chosenHeap == NULL){
        chosenHeap = FindHeap(data);
    }

    if (chosenHeap != NULL){
        newAddress = chosenHeap->Duplicate(data,size);
    }
    // to duplicate static memory!
    else{
        newAddress = standardHeap.Duplicate(data,size);
    }
    return newAddress;

}


bool AddHeap(HeapI *const newHeap){

    bool ok = true;
    /* check value of heap not to be NULL */
    if (newHeap == NULL_PTR(HeapI *)){
        ok = false;
    }


    /* controls access to database */
    if (heapDataBase.Lock()){
        int32 i;
        /* check if not registered already */
        for(i=0;(i<MaximumNumberOfHeaps) && ok ;i++){
            /** retrieve heap information in current slot */
            HeapI *heap = heapDataBase.GetHeap(i);

            /** already found */
            if (heap == newHeap) {
                ok = false;

                /* TODO error message here */
            }
        }

        /* check if space available and if so register it  */
        if (ok) {

            bool found = false;
            /* for each slot  */
            for(i=0;(i<MaximumNumberOfHeaps) && (!found) ;i++){
                /** try to set each slot */
                found = heapDataBase.SetHeap(i, newHeap);
            }

            /** no more space */
            if (!found){
                ok = false;
                // TODO error message here
            }

        }
        /* controls access to database */
        heapDataBase.UnLock();
    } else {
        // TODO error message
        ok = false;
    }

    return ok;
}

bool RemoveHeap(const HeapI *const heap){

    bool found = false;

    /* controls access to database */
    if (heapDataBase.Lock()){
        int32 i;
        /* check if not registered already */
        for(i=0;(i<MaximumNumberOfHeaps) && (!found) ;i++){
            /** retrieve heap information in current slot */
            found = heapDataBase.UnsetHeap(i,heap);
        }
        /* controls access to database */
        heapDataBase.UnLock();
    }

    return found;
}

}

