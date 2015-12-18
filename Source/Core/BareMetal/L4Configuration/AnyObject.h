/**
 * @file AnyObject.h
 * @brief Header file for class AnyObject
 * @date 27/10/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class AnyObject
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYOBJECT_H_
#define ANYOBJECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An helper class to serialise the contents of an AnyType (i.e. of the data pointed by an AnyType.GetDataPointer()).
 * @details This class allows to associate a name and a reference to an AnyType.
 * It holds and manages a memory space to store a copy of the data of the serialised AnyType.
 */
class DLL_API AnyObject: public Object {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     * @post
     *   GetType().GetDataPointer() == NULL
     */
    AnyObject();

    /**
     * @brief Default destructor.
     * @details Frees any memory allocated to store the AnyType.
     */
    virtual ~AnyObject();

    /**
     * @brief Frees the memory of the internal serialised AnyType to be ready for a new serialisation.
     */
    void CleanUp();

    /**
     * @brief Serialises an AnyType.
     * @details The input AnyType \a typeIn will be copied and its contents
     * (provided by typeIn.GetDataPointer()) copied into a memory space managed by this
     * AnyObject. This memory will be freed by the destructor of the AnyType.
     * @param[in] typeIn the AnyType to be serialised.
     * @return true if all the memory allocation and copy operations are successful.
     * @pre
     *   GetType().GetDataPointer() == NULL
     * @post
     *   GetType() == typeIn
     */
    bool Serialise(const AnyType &typeIn);

    /**
     * @brief Gets the serialised AnyType.
     * @return the serialised AnyType.
     */
    AnyType GetType() const;

private:

    /**
     * The serialised AnyType
     */
    AnyType type;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ANYOBJECT_H_ */
