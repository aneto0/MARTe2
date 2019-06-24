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
namespace ObjectRegistryDatabase {

	/**
     * @brief Retrieves the Reference of the root database.
	 */
	ReferenceT<ReferenceContainer> Access();

    /**
     * @brief Retrieves the Reference at the given address.
     * @param[in] path is the address of the Reference within the database. The syntax is
     * "A.B.C..." where A, B and C are the name of the container object nodes.
     * @param[in] current is the search start point. In this case we admit the syntax "::A.B.C"
     * where the ':' symbol set the search start point to the previous domain with respect to \a current.
     * If no ':' is found at the beginning of the path, the start point is the root.
     * @return the reference found at the provided \a path or an invalid reference in case of failure.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2].*/
    Reference Find(const CCString path,const Reference current = emptyReference);

}
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTRYDATABASE_H_ */

