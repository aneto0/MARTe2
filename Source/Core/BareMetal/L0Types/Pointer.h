/**
 * @file Pointer.h
 * @brief Header file for class AnyType
 * @date 29 Oct 2017
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

 * @details This header file contains the declaration of the class Pointer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef POINTER_H_
#define POINTER_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief A pointer to some data which may or not have been allocated by a descendant class.
 * @details Common part to Matrix and Vector. Does not manage memory allocation/destruction
 * as it would not be able to invoke constructor/destructors
 */
class Pointer {

public:

    /**
     * @brief Default constructor.
     * @post
     *   GetDataPointer() == NULL
     */
    inline Pointer();

    /**
     * @brief Associates the pointer to an existent memory area
     * @param[in] existingArray The pointer to the existing array
     * @post
     *   GetDataPointer() == existingArray
     */
    inline Pointer(void *existingArray);

    /**
     * @brief Associates the pointer to an existent memory area
     * @param[in] existingArray The pointer to the existing array
     * @post
     *   GetDataPointer() == existingArray
     */
    inline void Set(void *existingArray);

    /**
     * @brief Destructor.
     * @details clears the pointer
     */
    inline ~Pointer();

    /**
     * @brief Gets the data pointer associated to the raw matrix data.
     * @return the data pointer associated to the raw matrix data.
     */
    inline void *GetDataPointer() const;

protected:


    /**
     * The data pointer to the raw data.
     */
    void *dataPointer;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

Pointer::Pointer() {
    dataPointer = static_cast<void *>(NULL);
}


void Pointer::Set(void *existingArray) {
    dataPointer = reinterpret_cast<void *>(existingArray);
}

Pointer::Pointer(void *existingArray) {
    dataPointer = reinterpret_cast<void *>(existingArray);
}

Pointer::~Pointer() {
	dataPointer = NULL;
}

inline void* Pointer::GetDataPointer() const {
    return dataPointer;
}

} // MARTe

#endif /* POINTER_H_ */
