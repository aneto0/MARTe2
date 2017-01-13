/**
 * @file HeapManager.cpp
 * @brief Source file for module HeapManager
 * @date 07/08/2015
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
 * the module HeapManager (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManager.h"
#include "FastPollingMutexSem.h"
#include "GeneralDefinitions.h"
#include "HeapI.h"
#include "StringHelper.h"
#include "GlobalObjectI.h"
#include "GlobalObjectsDatabase.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Local Module declaration                        */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace HeapManager /*Internals*/{

/**
 * @brief The heap database size.
 */
static const int32 MaximumNumberOfHeaps = 16;

/**
 * @brief Class to store HeapI pointers.
 * @details This class allows to store and recover pointers to HeapI objects,
 * which are addressable by a numeric index. It also offers methods for
 * locking the access to the database. It is intended to be created following
 * the singleton pattern by means of the Instance method.
 */
class HeapDatabase: public GlobalObjectI {

    /**
     * @brief Lists all heaps
     * all unused heaps have a NULL pointer.
     */
    HeapI * heaps[MaximumNumberOfHeaps];

    /**
     * @brief Internal mutex semaphore.
     */
    FastPollingMutexSem mux;

public:
    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     */
    static HeapDatabase *Instance();

    /**
     * @brief Destructor NOOP.
     */
    virtual ~HeapDatabase();

    /**
     * @brief gets the HeapI contained in a given slot of the database
     * @param[in] index indicates the slots of the database
     * @return NULL index out of range or if empty slot
     * */
    HeapI *GetHeap(int32 index) const;

    /**
     * @brief sets the HeapI in a given slot of the database
     * @param[in] index indicates the slots of the database
     * @param[in] heap  is the desired heap to store
     * @return true if index is within range; specified slot is free; and heap is not NULL
     * */
    bool SetHeap(int32 index,
                 HeapI * const heap);

    /**
     * @brief sets to NULL a given slot of the database
     * @param[in] index indicates the slots of the database
     * @param[in] heap must contain the same value as in the database
     * @return true if index is within range; specified slot contains heapl heap is not NULL
     * */
    bool UnsetHeap(int32 index,
                   const HeapI *heap);

    /**
     * @brief constructor
     * */
    HeapDatabase();

    /**
     * @brief locks access to database
     * @return true if locking successful
     * */
    bool Lock();

    /**
     * @brief unlocks access to database
     * */
    void UnLock();

    /**
     * @brief Returns "HeapDatabase".
     * @return "HeapDatabase".
     */
    virtual CCString const GetClassName() const;

};

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

HeapDatabase *HeapDatabase::Instance() {
    static HeapDatabase *instance = NULL_PTR(HeapDatabase *);
    if (instance == NULL_PTR(HeapDatabase *)) {
        instance = new HeapDatabase();
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 1u);
    }
    return instance;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HeapI *HeapDatabase::GetHeap(const int32 index) const {
    HeapI *returnValue = NULL_PTR(HeapI *);
    if ((index >= 0) && (index < MaximumNumberOfHeaps)) {
        returnValue = heaps[index];
    }

    return returnValue;
}

bool HeapDatabase::SetHeap(const int32 index,
                           HeapI * const heap) {
    bool ok = false;
    if ((index >= 0) && (index < MaximumNumberOfHeaps)) {

        if ((heaps[index] == NULL) && (heap != NULL)) {
            heaps[index] = heap;
            ok = true;
        }

    }
    return ok;
}

bool HeapDatabase::UnsetHeap(const int32 index,
                             const HeapI * const heap) {
    bool ok = false;
    if ((index >= 0) && (index < MaximumNumberOfHeaps)) {
        if (heaps[index] == heap) {
            heaps[index] = NULL_PTR(HeapI *);
            ok = true;
        }
    }

    return ok;
}

HeapDatabase::HeapDatabase() {
    int32 i;
    for (i = 0; i < MaximumNumberOfHeaps; i++) {
        heaps[i] = NULL_PTR(HeapI *);
    }
}

HeapDatabase::~HeapDatabase() {
}

bool HeapDatabase::Lock() {
    return (mux.FastLock() == ErrorManagement::NoError);
}

void HeapDatabase::UnLock() {
    mux.FastUnLock();
}

CCString const HeapDatabase::GetClassName() const {
    return "HeapDatabase";
}

HeapI *FindHeap(const void * const address) {

    /*
     address range of currently found heap
     */
    uintp foundSpan = 0U;

    /*
     * the search will set this pointer to point to the heap found
     * by default return the standard heap
     */
    HeapI *foundHeap = NULL_PTR(HeapI *);

    /* controls access to database */
    if (HeapDatabase::Instance()->Lock()) {

        int32 i;
        for (i = 0; (i < MaximumNumberOfHeaps); i++) {

            /* retrieve heap information in current slot */
            HeapI *heap = HeapDatabase::Instance()->GetHeap(i);
            /* if slot used */
            if (heap != NULL_PTR(HeapI *)) {
                /* check address compatibility */
                if (heap->Owns(address)) {
                    /* check if first occurrence or */
                    if (foundHeap == NULL_PTR(HeapI *)) {
                        /* size of memory space */
                        foundSpan = heap->LastAddress() - heap->FirstAddress();
                        /* save heap */
                        foundHeap = heap;
                    }
                    else { /* further occurrences */

                        /* size of memory space */
                        uintp newFoundSpan = heap->LastAddress() - heap->FirstAddress();

                        /*
                         * smaller memory span and intersecting memory address space
                         * it can only mean that this heap is a sub-heap of the previously found heap
                         */
                        if (newFoundSpan < foundSpan) {
                            /* save size of memory space */
                            foundSpan = newFoundSpan;
                            /* save heap */
                            foundHeap = heap;
                        } // end if (newFoundSpan < foundSpan)

                    } /* end of check first occurrence */

                } /* end check address compatibility */

            } /* end if heap != NULL */

        } /* end i loop */

        HeapDatabase::Instance()->UnLock();
    }

    /* assign to heap the found heap or the default one */
    if ((foundHeap == NULL_PTR(HeapI *))) {

        /* try default heap */
        foundHeap = GlobalObjectsDatabase::Instance()->GetStandardHeap();

        /* check ownership of default heap */
        if (!foundHeap->Owns(address)) {
            foundHeap = NULL_PTR(HeapI *);
        }

    }

    return foundHeap;
}

HeapI *FindHeap(const char8 * const name) {

    bool ok = (name != NULL);

    /*
     * found heap
     */
    bool found = false;

    /*
     * the search will set this pointer to point to the heap found
     */
    HeapI *foundHeap = NULL_PTR(HeapI *);

    if (ok) {

        /* controls access to database */
        if (HeapDatabase::Instance()->Lock()) {
            int32 i;
            for (i = 0; (i < MaximumNumberOfHeaps) && (!found); i++) {

                /* retrieve heap information in current slot */
                HeapI *heap = HeapDatabase::Instance()->GetHeap(i);

                /* if slot used */
                if (heap != NULL_PTR(HeapI *)) {

                    /* check address compatibility */
                    if (StringHelper::Compare(heap->Name(), name) == 0) {

                        found = true;

                        foundHeap = heap;

                    } /* end check name */

                } /* end if heap != NULL */

            } /* end i loop */
            HeapDatabase::Instance()->UnLock();
        }
    }

    return foundHeap;
}

bool Free(void *&data) {
    HeapI *heap = FindHeap(data);

    bool ok = false;
    /* Does not belong to any heap?*/
    if ((heap != NULL_PTR(HeapI *))) {
        heap->Free(data);
        ok = true;

    }
    else {

        REPORT_ERROR(ErrorManagement::FatalError, "Error: the pointer in input does not belong to any heap");

    }

    return ok;
}

void *Malloc(uint32 const size,
             const char8 * const heapName) {

    void *address = NULL_PTR(void *);

    /* Standard behavior */
    if (heapName == NULL) {
        address = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(size);
    }
    else {

        HeapI *heap = FindHeap(heapName);

        if (heap != NULL) {
            address = heap->Malloc(size);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "Error: no heaps with the specified name found");
        }

    }

    return address;
}

void *Realloc(void *&data,
              const uint32 newSize) {
    void *newAddress = NULL_PTR(void *);

    HeapI *chosenHeap = FindHeap(data);

    if (chosenHeap != NULL) {
        newAddress = chosenHeap->Realloc(data, newSize);
    }
    //if the heap is not found (the data is null) allocates it on the standard heap (C malloc).
    else {
        newAddress = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Realloc(data, newSize);
    }

    return newAddress;
}

void *Duplicate(const void * const data,
                const uint32 size,
                const char8 * const heapName) {
    void *newAddress = NULL_PTR(void *);

    HeapI *chosenHeap = NULL_PTR(HeapI *);

    //if the heapName is not null searches the heap by name
    if (heapName != NULL) {
        chosenHeap = FindHeap(heapName);
    }

    //if the heap with that name is not found calls the find by address
    if (chosenHeap == NULL) {
        chosenHeap = FindHeap(data);
    }

    // if found calls the correct heap duplicate
    if (chosenHeap != NULL) {
        newAddress = chosenHeap->Duplicate(data, size);
    }
    // if the address is not found considers the memory as a static
    else {
        //REPORT_ERROR(ErrorManagement::Warning, "ErrorManagement::Warning: the input address does not belong to any heap. It will be considered as a static memory address");
        newAddress = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Duplicate(data, size);
    }
    return newAddress;

}

bool AddHeap(HeapI * const newHeap) {

    bool ok = true;

    /* check value of heap not to be NULL */
    if (newHeap == NULL_PTR(HeapI *)) {
        ok = false;
    }

    /* controls access to database */
    if (HeapDatabase::Instance()->Lock()) {
        int32 i;
        /* check if not registered already */
        for (i = 0; (i < MaximumNumberOfHeaps) && ok; i++) {
            /* retrieve heap information in current slot */
            HeapI *heap = HeapDatabase::Instance()->GetHeap(i);

            /* already found */
            if (heap == newHeap) {
                ok = false;

                //Do not uncomment this line. The HeapManager cannot use the REPORT_ERROR since the HeapDatabase::Instance() is locked and
                // the user callback function could use the HeapDatabase, e.g. by using a StreamString
                //REPORT_ERROR(ErrorManagement::FatalError, "Error: heap already registered in the database");

            }
        }

        /* check if space available and if so register it  */
        if (ok) {

            bool found = false;
            /* for each slot  */
            for (i = 0; (i < MaximumNumberOfHeaps) && (!found); i++) {
                /* try to set each slot */
                found = HeapDatabase::Instance()->SetHeap(i, newHeap);
            }

            /* no more space */
            if (!found) {
                ok = false;
                //Do not uncomment this line. The HeapManager cannot use the REPORT_ERROR since the HeapDatabase::Instance() is locked and
                // the user callback function could use the HeapDatabase, e.g. by using a StreamString
                //REPORT_ERROR(ErrorManagement::FatalError, "Error: not enough space in the database for new records");

            }

        }
        /* controls access to database */
        HeapDatabase::Instance()->UnLock();
    }
    return ok;
}

bool RemoveHeap(const HeapI * const heap) {

    bool found = false;

    /* controls access to database */
    if (HeapDatabase::Instance()->Lock()) {
        int32 i;
        /* check if not registered already */
        for (i = 0; (i < MaximumNumberOfHeaps) && (!found); i++) {
            /* retrieve heap information in current slot */
            found = HeapDatabase::Instance()->UnsetHeap(i, heap);
        }
        /* controls access to database */
        HeapDatabase::Instance()->UnLock();
    }

    return found;
}

}

}
