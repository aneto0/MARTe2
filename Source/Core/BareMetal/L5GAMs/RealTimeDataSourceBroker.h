/**
 * @file RealTimeDataSourceBroker.h
 * @brief Header file for class RealTimeDataSourceBroker
 * @date 22/mar/2016
 * @author pc
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

 * @details This header file contains the declaration of the class RealTimeDataSourceBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L5GAMS_REALTIMEDATASOURCEBROKER_H_
#define 		SOURCE_CORE_BAREMETAL_L5GAMS_REALTIMEDATASOURCEBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "MemoryArea.h"
#include "ReferenceT.h"
#include "RealTimeApplication.h"
#include "RealTimeDataDefI.h"
#include "BasicRealTimeDataSourceDef.h"
#include "FastPollingEventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The interface between GAMs and RealTimeDataSource.
 */
class DLL_API RealTimeDataSourceBroker: public ReferenceContainer {

public:

    /**
     * @brief Constructor.
     * @post
     *   GetData(*) == NULL &&
     *   GetMemoryPointer(*) == NULL
     */
    RealTimeDataSourceBroker();

    /**
     * @brief Sets the application where this object is involved in.
     * @param[in] rtApp is a Reference to the application to be set.
     */
    void SetApplication(RealTimeApplication &rtApp);

    /**
     * @brief Links a GAM variable with the RealTimeDataSource.
     * @details If \a ptr is null, this function allocates memory for the variable and stores the pointer
     * in a vector. The pointer to the respective variable memory, allocated in the RealTimeDataSource, will be
     * stored into another vector. If \a ptr is not NULL, this means that the GAM provides the memory of that
     * variable by itself, then the allocation is not performed.
     * @param[in] def is the variable definition.
     * @param[in] ptr is the pointer to the variable memory area.
     * @return false in case of errors, true otherwise.
     */
    bool AddVariable(ReferenceT<RealTimeDataDefI> def,
                     void * const ptr = NULL_PTR(void*));

    /**
     * @brief Retrieves the pointer to the \a i-th variable.
     * @param[in] i is the variable index.
     * @return the pointer to the \a i-th variable. NULL if this object is not finalised (see Finalise()).
     */
    void *GetData(const uint32 i) const;

    /**
     * @brief Retrieves the n-th pointer.
     * @details This function is different from GetData(*) when variables are structures.
     * Actually this function can provide a direct access to the variable internal members.
     * @return the n-th pointer. NULL if the object is not finalised (see Finalise()).
     */
    void *GetMemoryPointer(const uint32 n) const;

    /**
     * @brief Finalises the object.
     * @details This function has to be called after all the variables are added.
     * @return false in case of errors, true otherwise.
     */
    virtual bool Finalise();


    bool IsSync() const;

protected:

    /**
     * The RealTimeApplication where this object is
     * involved in.
     */
    RealTimeApplication *application;

    /**
     * The memory area where variables will be allocated
     * on.
     */
    MemoryArea memory;

    /**
     * Stores the indexes of the allocations
     * in memory area.
     */
    StaticList<uint32> GAMOffsets;

    /**
     * Stores the pointers to the begin
     * of memory areas
     */
    StaticList<void *> beginPointers;

    /**
     * Stores the final pointers to the
     * GAM variables.
     */
    StaticList<void *> GAMPointers;

    /**
     * Stores the pointers to the RealTimeDataSource
     * variables.
     */
    StaticList<void **> DSPointers[2];


    StaticList<BasicRealTimeDataSourceDef *> dataSources;

    /**
     * Stores the sizes of the variables.
     */
    StaticList<uint32> sizes;

    /**
     * Stores the indexes of each variable
     * into the GAMPointers.
     */
    StaticList<uint32> chunkIndex;


    bool synchronized;


    FastPollingEventSem *pollingSem;


    /**
     * Specifies if Finalise() is called or not.
     */
    bool finalised;
private:

    /**
     * @brief Links a GAM variable with the RealTimeDataSource.
     * @details This function can call itself recursively if the variable to be allocated is a structure.
     * @param[in] def is the variable definition.
     * @param[in] ptr is the pointer to be added in the \a GAMPointers array. If NULL, the memory
     * will be allocated.
     * @param[in] memStart is the begin of the variable memory area.
     * @return false in case of errors, true otherwise.
     */
    bool AddVariablePrivate(ReferenceT<RealTimeDataDefI> def,
                            void * ptr,
                            void * memStart);

    /**
     * @brief Retrieves the pointer at the \a n-th position before the Finalise().
     * @param[in] n is the index of the pointer to be returned.
     * @return the pointer at the \a n-th position.
     */
    void *GetMemoryPointerPrivate(const uint32 n) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_REALTIMEDATASOURCEBROKER_H_ */

