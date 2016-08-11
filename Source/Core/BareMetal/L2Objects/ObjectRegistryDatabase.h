/**
 * @file ObjectRegistryDatabase.h
 * @brief Header file for class ObjectRegistryDatabase
 * @date 18/02/2016
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
 *
 * @details This header file contains the declaration of the class ObjectRegistryDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTREGISTRYDATABASE_H_
#define OBJECTREGISTRYDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Singleton database of References to MARTe Objects.
 */
class DLL_API ObjectRegistryDatabase: public ReferenceContainer, public GlobalObjectI {

public:

    /**
     * @brief Singleton access to the database.
     * @return a pointer to the database.
     */
    static ObjectRegistryDatabase *Instance();

    /**
     * @brief Destructor.
     */
    virtual ~ObjectRegistryDatabase();

    /**
     * @brief Retrieves the Reference at the given address.
     * @param[in] path is the address of the Reference into the database. The syntax is
     * "A.B.C" where A, B and C must be replaced with the specific object names.
     * param[in] current is the research start point. In this case we admit the syntax "::A.B.C"
     * where the ':' symbol set the search start point to the previous domain with respect to \a current.
     * If no ':' is found at the beginning of the path, the start point is the root.
     * @return the reference found at the provided \a path or an invalid reference in case of failure.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2].*/
    Reference Find(const char8 * const path,
                   const Reference current = Reference());

    /**
     * @see Object::GetClassName
     * @return "ObjectRegistryDatabase".
     */
    virtual const char8 * const GetClassName() const;

private:
    /**
     * @brief Disallow the usage of new.
     * @param[in] size the size of the object.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The new operator must wrap GlobalObjectsI::new(*) */
    static void *operator new(const osulong size) throw ();

    /**
     * @brief Frees the memory area pointed by \a p previously allocated on the StandardHeap.
     * @param[in] p is the pointer to be freed.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The new operator must wrap GlobalObjectsI::delete(*) */
    static void operator delete(void * const p);

    /**
     * @brief Default constructor.
     */
    /*lint -e{1704} private constructor for singleton implementation*/
    ObjectRegistryDatabase();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTRYDATABASE_H_ */

