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

#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Local Class declaration                         */
/*---------------------------------------------------------------------------*/

namespace HeapManagerInternals{

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
    HeapInterface *     heaps[MaximumNumberOfHeaps];

    /**
     * @brief TODO
     */
    FastPollingMutexSem mux;

public:

    /**
     * @brief gets the HeapInterface contained in a given slot of the database
     * @param index indicates the slots of the database
     * @return NULL index out of range or if empty slot
     * */
    HeapInterface *GetHeap(int32 index)const;

    /**
     * @brief sets the HeapInterface in a given slot of the database
     * @param index indicates the slots of the database
     * @param heap  is the desired heap to store
     * @return true if index is within range; specified slot is free; and heap is not NULL
     * */
    bool SetHeap(int32 index, HeapInterface *const heap);

    /**
     * @brief sets to NULL a given slot of the database
     * @param index indicates the slots of the database
     * @param heap must contain the same value as in the database
     * @return true if index is within range; specified slot contains heapl heap is not NULL
     * */
    bool UnsetHeap(int32 index, const HeapInterface *heap);
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
 * @brief TBD
 */
class StandardHeap: public HeapInterface {

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
     * @brief allocates size bytes of data in the heap. Maximum allocated size is 4Gbytes
     * @return a pointer to the allocated memory or NULL if the allocation fails.
     */
    virtual void *Malloc(const uint32 &size);

    /**
     * @brief free the pointer data and its associated memory.
     * @param data the data to be freed.
     */
    virtual void Free(void *&data);

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
 * @brief gets the HeapInterface contained in a given slot of the database
 * @param index indicates the slots of the database
 * @return NULL index out of range or if empty slot
 * */
HeapInterface *HeapDataBase::GetHeap(const int32 index)const{
    HeapInterface *returnValue = NULL_PTR(HeapInterface *);
    if ((index >= 0) && (index < MaximumNumberOfHeaps)){
        returnValue = heaps[index];
    }
    return returnValue;
}

/**
 * @brief sets the HeapInterface in a given slot of the database
 * @param index indicates the slots of the database
 * @param heap  is the desired heap to store
 * @return true if index is within range; specified slot is free; and heap is not NULL
 * */
bool HeapDataBase::SetHeap(const int32 index,  HeapInterface * const heap  ){
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
bool HeapDataBase::UnsetHeap(const int32 index, const HeapInterface *heap){
    bool ok = false;
    if ((index >= 0) && (index < MaximumNumberOfHeaps)){
        if (heaps[index] == heap ){
            heaps[index] = NULL;
            ok = true;
        }
    }
    return ok;
}

/**
 * @brief constructor
 * */
HeapDataBase::HeapDataBase(){

    int32 i=0;
    for(i=0;i<MaximumNumberOfHeaps;i++){
        heaps[i] = NULL;
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
    free(data);
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
const char *StandardHeap::Name()const {
    return "StandardHeap";
}

}
/**
 * @brief Finds the Heap that manages the specified memory location
 * @param address is a memory address that the target heap should manage
 * returns NULL if not found
 */
HeapInterface *HeapManager::FindHeap(const void * address){

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
    HeapInterface *foundHeap = NULL;

    /* controls access to database */
    if (HeapManagerInternals::heapDataBase.Lock()){
        // TODO add error message here

        int i=0;
        for(i=0;(i<HeapManagerInternals::MaximumNumberOfHeaps) && ok ;i++){

            /** retrieve heap information in current slot */
            HeapInterface *heap = HeapManagerInternals::heapDataBase.GetHeap(i);

            /** if slot used */
            if (heap != NULL){

                /* check address compatibility */
                if ( heap->Owns(address)){

                    /** check if first occurrence or */
                    if (foundHeap == NULL){

                        /* size of memory space */
                        foundSpan = ((char *)heap->LastAddress()-(char *)heap->FirstAddress());

                        /* save heap */
                        foundHeap = heap;

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

                            /* save heap */
                            foundHeap = heap;
                        } // end if (newFoundSpan < foundSpan)

                    } /* end of check first occurrence */

                } /* end check address compatibility */

            } /* end if heap != NULL */

        } /* end i loop */


        HeapManagerInternals::heapDataBase.UnLock();
    } else {
        ok = false;
        // TODO add error message here
    }

    /* assign to heap the found heap or the default one */
    if (foundHeap == NULL) {

        /** try default heap */
        foundHeap = &HeapManagerInternals::standardHeap;

        /* check ownership of default heap */
        if (! foundHeap->Owns(address) ){
            foundHeap = NULL;
        }

    }

    return foundHeap;
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

        /* controls access to database */
        if (HeapManagerInternals::heapDataBase.Lock()){
            // TODO add error message here
            int i=0;
            for(i=0;(i<HeapManagerInternals::MaximumNumberOfHeaps) && !found ;i++){

                /** retrieve heap information in current slot */
                HeapInterface *heap = HeapManagerInternals::heapDataBase.GetHeap(i);

                /** if slot used */
                if (heap != NULL){

                    /* check address compatibility */
                    if( StringPortable::Compare (heap->Name(),name) == 0 ){

                        found = true;

                        foundHeap = heap;


                    } /* end check name */

                } /* end if heap != NULL */

            } /* end i loop */
            HeapManagerInternals::heapDataBase.UnLock();
        }
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


        // TODO error message here

    }

    return ok;
}

void *HeapManager::Malloc(uint32 size, const char *name){

    void *address = NULL;

    if (name == NULL){
        address = HeapManagerInternals::standardHeap.Malloc(size);
    } else {
        // TODO comment
        HeapInterface *heap = FindHeap(name);

        if (heap != NULL){
            return heap->Malloc(size);
        }

    }

    return address;
}

bool HeapManager::AddHeap(HeapInterface *newHeap){

    bool ok = true;

    /* check value of heap not to be NULL */
    if (newHeap == NULL){
        ok = false;
    }


    /* controls access to database */
    if (HeapManagerInternals::heapDataBase.Lock()){
        int i = 0;
        /* check if not registered already */
        for(i=0;(i<HeapManagerInternals::MaximumNumberOfHeaps) && ok ;i++){
            /** retrieve heap information in current slot */
            HeapInterface *heap = HeapManagerInternals::heapDataBase.GetHeap(i);

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
            for(i=0;(i<HeapManagerInternals::MaximumNumberOfHeaps) && !found ;i++){
                /** try to set each slot */
                found = HeapManagerInternals::heapDataBase.SetHeap(i, newHeap);
            }

            /** no more space */
            if (!found){
                ok = false;
                // TODO error message here
            }

        }
        /* controls access to database */
        HeapManagerInternals::heapDataBase.UnLock();
    } else {
        // TODO error message
        ok = false;
    }

    return ok;
}

bool HeapManager::RemoveHeap(HeapInterface *heap){

    bool found = false;

    /* controls access to database */
    if (HeapManagerInternals::heapDataBase.Lock()){
        int i = 0;
        /* check if not registered already */
        for(i=0;(i<HeapManagerInternals::MaximumNumberOfHeaps) && !found ;i++){
            /** retrieve heap information in current slot */
            found = HeapManagerInternals::heapDataBase.UnsetHeap(i,heap);
        }
        /* controls access to database */
        HeapManagerInternals::heapDataBase.UnLock();
    }

    return found;
}


