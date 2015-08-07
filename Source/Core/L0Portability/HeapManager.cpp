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
#include "Heap.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class HeapDataBase {
/**
     * @brief Lists all heaps
     * all unused heaps have a NULL pointer
     */
    Heap *heaps[MaximumNumberOfHeaps];

    /**
     * reference to Heap
     * */
    Heap *defaultHeap;

public:

    /**
     * true --> Pointer is inside current address range
     * */
   Heap *GetHeap(int index)const{
        Heap *returnValue = NULL;
        if ((index >= 0) && (index < MaximumNumberOfHeaps)){
            returnValue = heaps[index];
        }
        return returnValue;
    }


   Heap *GetDefaultHeap()const {
       return defaultHeap;
   }

    /**
     * true --> Pointer is inside current address range
     * */
    HeapDataBase(){
        defaultHeap = NULL;

        int i=0;
        for(i=0;i<MaximumNumberOfHeaps;i++){
            heaps[i] = NULL;
        }
    }

} heapDataBase;


Heap *HeapManager::FindHeap(void * address){
    const int notFound = -1;
     /**
     * 0 to MaximumNumberOfHeaps-1 means found
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


    Heap *heap = NULL;

   int i=0;
   for(i=0;(i<MaximumNumberOfHeaps) && ok ;i++){

        /** retrieve heap information in current slot */
        heap = heapDataBase.GetHeap(i);

        /** if slot used */
        if (heap != NULL){

            /* check address compatibility */
            if ( (heap->FirstAddress() >= address) &&
                 (heap->LastAddress()  <= address)){

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
        } /* end i loop */

        heap = heapDataBase.GetDefaultHeap();

        if (foundIndex != notFound){
            heap = heapDataBase.GetHeap(foundIndex);
        }

        return heap;
    }


    return NULL;
}
	
