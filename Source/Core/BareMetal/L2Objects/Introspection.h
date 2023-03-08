/**
 * @file Introspection.h
 * @brief Header file for class Introspection
 * @date 05/08/2015
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
 *
 * @details This header file contains the declaration of the class Introspection
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTION_H_
#define INTROSPECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntry.h"
#include "ZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Groups the information about each member of a class or a structure.
 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
class DLL_API Introspection {

public:

    /**
     * @brief Default constructor
     * @post
     *   GetClassSize()==0u
     */
    Introspection();

    /**
     * @brief Constructor.
     * @param[in] introspectionListIn contains a list of IntrospectionEntry pointers, one for each class member.
     * @param[in] classSizeIn is the class size.
     * @pre
     *   introspectionListIn must be a zero-terminated array.
     * @post
     *   GetClassSize()==classSizeIn
     */
    Introspection(const IntrospectionEntry ** const introspectionListIn,
                  const uint32 classSizeIn);

    /**
     * @brief Retrieves the information about a specific member.
     * @param[in] index is the member number.
     * @return a pointer to the IntrospectionEntry structure containing all the
     * information about the \a index-th class member.
     */
    const IntrospectionEntry operator[](const uint32 index) const;

    /**
     * @brief Retrieves the number of IntrospectionEntries in the internal array.
     * @return the number of IntrospectionEntries in the internal array.
     */
    uint32 GetNumberOfMembers() const;

    /**
     * @brief Retrieves the class Size.
     * @return the class size.
     */
    uint32 GetClassSize() const;

    /**
     * @brief Gets the Introspection fields.
     * @return the Introspection fields.
     */
    const IntrospectionEntry **GetFields();

    /**
     * @brief The IntrospectionEntry members include pointers to (string) memory allocated elsewhere.
     * This helper method allows to store the location of such memory, so that it can be recovered later (e.g. to delete it).
     * @param[in] mem pointer to the memory location where the IntrospectionEntry strings are stored.
     */
    void SetIntrospectionEntryMemory(void *mem);

    /**
     * @brief See SetIntrospectionEntryMemory.
     * @return pointer to the memory location where the IntrospectionEntry strings are stored.
     */
    void *GetIntrospectionEntryMemory();
private:

    /**
     * An array of pointer to the class member's descriptors.
     * The array must be zero-terminated.
     */
    ZeroTerminatedArray<const IntrospectionEntry *> fields;

    /**
     * The class size.
     */
    uint32 classSize;

    /**
     * IntrospectionEntry members memory.
     */
    void *introMemLocation;

};

}

#endif /* INTROSPECTION_H_ */

