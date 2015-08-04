/**
 * @file ClassRegistryItem.h
 * @brief Header file for class ClassRegistryItem
 * @date 04/08/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ClassRegistryItem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_
#define SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "Heap.h"
#include "LinkedListable.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO
 * @details TODO
 */
typedef Object *(ObjectBuildFn)(Heap &);

class ClassRegistryItem: public LinkedListable {
public:
    /**
     * @brief Assigns the input variables to the class members.
     * @param clName the class name to register.
     * @param clVersion the class version to register.
     * @param objBuildFn the object build function to register.
     */
    ClassRegistryItem(const char8* clName, const char8* clVersion, ObjectBuildFn *objBuildFn);
    /**
     * @brief Returns the class name associated to this item.
     * @return the class name associated to this item.
     */
    const char8* GetClassName() const;

    /**
     * @brief Returns the class version associated to this item.
     * @details The version of the class is the version against which the code was compiled.
     * @return the class version associated to this item.
     */
    const char8* GetClassVersion() const;

    /**
     * @brief Returns a pointer to function that allows to instantiate a new object from this class.
     * @return a pointer to function that allows to instantiate a new object from this class.
     */
    ObjectBuildFn *GetObjectBuildFunction() const;

    /**
     * @brief Increments the number of instantiated objects belonging to the class type represented by this registry item.
     */
    void IncrementNumberOfInstances();

    /**
     * @brief Decrements the number of instantiated objects belonging to the class type represented by this registry item.
     */
    void DecrementNumberOfInstances();

    /**
     * @brief Returns the number of instantiated objects belonging to the class type represented by this registry item.
     * @return the number of instantiated objects belonging to the class type represented by this registry item.
     */
    uint32 GetNumberOfInstances();

    /**
     * @brief Returns the heap that was selected to allocate objects belonging to the class type represented by this registry item.
     * @return the heap that was selected to allocate objects belonging to the class type represented by this registry item.
     */
    const Heap &GetHeap() const;

    /**
     * @brief Sets the heap to allocate objects belonging to the class type represented by this registry item.
     * @param h the heap to allocate objects belonging to the class type represented by this registry item.
     */
    void SetHeap(const Heap& h);

private:
    /**
     * The name of the class
     */
    const char8 *className;

    /**
     * The version of the class against which the code was compiled.
     */
    const char8 *classVersion;

    /**
     * The object instantiation function.
     */
    ObjectBuildFn *objectBuildFn;

    /**
     * The number of instantiated objects belonging to the class type represented by this registry item.
     */
    uint32 numberOfInstances;

    /**
     * The heap that is being used to instantiate objects of the class type represented by this registry item.
     */
    Heap heap;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_CLASSREGISTRYITEM_H_ */

