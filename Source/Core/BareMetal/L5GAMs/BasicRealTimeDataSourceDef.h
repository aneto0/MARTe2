/**
 * @file BasicRealTimeDataSourceDef.h
 * @brief Header file for class BasicRealTimeDataSourceDef
 * @date 21/03/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEDEF_H_
#define BASICREALTIMEDATASOURCEDEF_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "ReferenceT.h"
#include "RealTimeStateInfo.h"
#include "GAMI.h"
#include "MemoryArea.h"
#include "FastPollingEventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The definition of a real-time variable shared between GAMs.
 */
class BasicRealTimeDataSourceDef: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    BasicRealTimeDataSourceDef();

    /**
     * @brief Adds a GAM as a consumer.
     * @param[in] stateIn the state name where \a gam is involved as a consumer of this variable.
     * @param[in] gam is the gam which will consume this variable in the state \a stateIn.
     * @return false in case of errors, false otherwise.
     */
    bool AddConsumer(const char8 * const stateIn,
                     ReferenceT<GAMI> gam);

    /**
     * @brief Adds a GAM as a producer.
     * @param[in] stateIn the state name where \a gam is involved as a producer of this variable.
     * @param[in] gam is the gam which will produce this variable in the state \a stateIn.
     * @return false in case of errors, false otherwise.
     */
    bool AddProducer(const char8 * const stateIn,
                     ReferenceT<GAMI> gam);

    /**
     * @brief Sets the default value of this variable.
     *
     * @details If the default value is already set (by another GAM) a warning will be generated
     * but the default value will be refreshed to \a defaultIn.
     *
     * @details If the variable is a scalar basic type the variable will be initialised
     * with the value result of the conversion from \a defaultIn (string) to the specific type.\n
     * If the variable is a multi-dimensional basic type (vector or matrix) \a defaultIn must be in the form:
     *   - { element1, element2, ... } for vectors \n
     *   - { { element11, element12, ... } { element21, element22, ... } } for matrices \n
     * If the variable is a structure, the default value must define all the members:\n
     *   - member1_scalar_basic = element\n
     *     member2_scalar_vector = { element ...}\n
     *     member3_scalar_matrix = { { element ...} ...}\n
     *     member4_structure = {\n
     *         ...\n
     *     }\n
     *
     * @param[in] defaultIn the default value.
     */
    void SetDefaultValue(const char8 * const defaultIn);

    /**
     * @brief Retrieves the default value.
     * @return the default value.
     */
    const char8 *GetDefaultValue();

    /**
     * @brief Retrieves the number of consumer GAMs of this variable in the specified state.
     * @param[in] statIn is the name of the state to be checked.
     * @return the number of consumer GAMs of this variable in \a stateIn.
     */
    uint32 GetNumberOfConsumers(const char8 * const stateIn);

    /**
     * @brief Retrieves the number of producer GAMs of this variable in the specified state.
     * @param[in] statIn is the name of the state to be checked.
     * @return the number of producer GAMs of this variable in \a stateIn.
     */
    uint32 GetNumberOfProducers(const char8 * const stateIn);

    /**
     * @brief Checks that for each state, at most one producer GAM is defined.
     * @return true if there is no more than one producer GAM for each state.
     */
    bool Verify();

    /**
     * @brief Set the variable type.
     * @param[in] typeName is the name of the type. See TypeDescriptor for more documentation about the basic
     * types.
     * @return false if the type is already set differently from \a typeName, true otherwise.
     */
    bool SetType(const char8 * const typeName);

    /**
     * @brief Retrieves the variable type name.
     * @return the variable type name.
     */
    const char8 *GetType();

    /**
     * @brief Retrieves the pointer to the pointer of the buffer to be used with the
     * specified index.
     * @details After the allocation of a double buffer memory for this variable, two
     * attributes of this object can store the pointer to one buffer or the other. If no
     * need to reset the variable in PrepareNextState, the pointer to the current buffer
     * will be stored in the attribute related to the next index in order to use again the
     * same buffer in the next state. Otherwise the attribute related to the next index
     * will store the pointer of the currently unused buffer.
     * @param[in] bufferIndex is the index of the store attributes.
     * @pre
     *   bufferIndex == 0 ||
     *   bufferIndex == 1;
     */
    void **GetDataSourcePointer(uint8 bufferIndex);

    /**
     * @brief Prepares the environment for the next state.
     * @details If the variable is not used in the current state but will be used in the next, its value
     * will be reset to the default.
     * @param[in] status contains informations about the current, the next state and the current
     * active buffer.
     * @return true if the default value set is compatible with the variable type, false otherwise.
     */
    bool PrepareNextState(const RealTimeStateInfo &status);

    /**
     * @brief Allocates the memory for this variable.
     * @param[in] dsMemory is the memory where this variable must be allocated.
     * @return false if the type is not recognized or in case of errors, true otherwise.
     */
    bool Allocate(MemoryArea &dsMemory);

    /**
     * @brief The routine to be executed at the begin of a write operation.
     */
    virtual void WriteStart();

    /**
     * @brief The routine to be executed at the begin of a read operation.
     */
    virtual void ReadStart();

    /**
     * @brief The routine to be executed at the end of a write operation
     */
    virtual void WriteEnd();

    /**
     * @brief The routine to be executed at the end of a read operation
     */
    virtual void ReadEnd();

    /**
     * @brief Retrieves the pointer to the eventual spin-lock semaphore
     * @return the pointer to the spin-lock semaphore, NULL if it is this data source
     * is not synchronized by spin-locks.
     */
    FastPollingEventSem * GetSpinLockSemaphore() const;

    /**
     * @brief Sets the number of elements for a specific dimension.
     * @param[in] dimension is the dimension where to set \nElements
     * @param[in] nElements is the number of elements in the dimension \a dimension.
     */
    void SetNumberOfElements(uint32 dimension,
                             const uint32 nElements);

    /**
     * @brief Sets the number of dimensions.
     * @param[in] nDimensions is the number of dimensions of this variable (0=scalar, 1=vector, 2=matrix).
     */
    void SetNumberOfDimensions(uint8 nDimensions);

    /**
     * @brief Retrieves the number of dimensions of this type definition (0=scalar, 1=vector, 2=matrix)
     * @return the number of dimensions of this type definition.
     */
    uint8 GetNumberOfDimensions() const;

    /**
     * @brief Retrieves the number of elements belonging to a specific dimension.
     * @return the number of elements belonging to a specific dimension.
     */
    uint32 GetNumberOfElements(uint32 dimension) const;

    /**
     * @brief Converts this object in a StructuredData.
     * @param[in] data contains the configuration data.
     * @return false in case of errors, true otherwise.
     */
    virtual bool ToStructuredData(StructuredDataI & data);
private:

    /**
     * The variable type
     */
    StreamString type;

    /**
     * The variable default value
     */
    StreamString defaultValue;

    /**
     * The positions of the buffers related to this variable in the
     * global memory area
     */
    uint32 bufferPtrOffset[2];

    /**
     * Pointers to the buffer to be used.
     */
    void *usedBuffer[2];

    /**
     * The variable number of dimensions.
     */
    uint8 numberOfDimensions;

    /**
     * The variable number of elements for
     * each dimension.
     */
    uint32 numberOfElements[3];

    /**
     * A pointer to the global memory area.
     */
    MemoryArea *memory;

    /**
     * The spin-lock semaphore
     */
    FastPollingEventSem * spinLockSem;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEDEF_H_ */

