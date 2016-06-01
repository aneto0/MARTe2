/**
 * @file Reference.h
 * @brief Header file for class Reference
 * @date 06/08/2015
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

 * @details This header file contains the declaration of the class Reference
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCE_H_
#define REFERENCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief Shared pointer implementation.
 * @details The Reference class is a smart pointer implementation where the shared
 * ownership of an underlying object is enabled. This guarantees that the life-cycle
 * of this object is safely managed and in particular that the object is destructed when
 * it is no longer referenced.
 *
 * The Reference might also own no object, in which case the function IsValid will return false.
 */
/*lint -e{9109} [MISRA C++ Rule 3-2-3] Justification: forward declaration of Reference in Object and StructureDataI required.*/
class DLL_API Reference {
public:

    /**
     * @brief Creates an empty Reference (referenced object is set to NULL).
     */
    Reference();

    /**
     * @brief Creates a Reference from an existing Reference.
     * @details This Reference will own the same object referenced by \a sourceReference.
     * @param[in] sourceReference the Reference to the object to be shared.
     */
    Reference(const Reference& sourceReference);

    /**
     * @brief Creates a new object of type \a typeName and links a reference to it.
     * @param[in] typeName the name of the class type.
     * @param[in] heap the heap responsible for allocating the object.
     */
    Reference(const char8* const typeName,
              HeapI* const heap = static_cast<HeapI *>(NULL));

              /**
               * @brief Creates a reference to an existing \a pointer.
               * @details Increments the number of references referencing the underlying object.
               * @param[in] pointer source object to assign.
               */
              Reference(Object * const pointer);

              /**
               * @brief Removes the reference to the underlying object.
               * @see RemoveReference.
               */
              virtual ~Reference();

              /**
               * @brief Creates an object from a structured list of elements.
               * @param[in] data the data to initialise the underlying object.
               * @param[in] initOnly if true the object is not created.
               * @return true if the object was successfully created and initialized, false otherwise.
               */
              virtual bool Initialise(StructuredDataI &data,
              const bool &initOnly);

              /**
               * @brief Removes the reference to the underlying object.
               * @details If the number of references to the underlying object is zero, the object is deleted.
               * IsValid function will return false after this operation.
               */
              virtual void RemoveReference();

              /**
               * @brief Verifies if the reference to the underlying object is valid.
               * @details A valid reference is one where the referenced object is not NULL.
               * @return true if the referenced object is not NULL, false otherwise.
               */
              virtual bool IsValid() const;

              /**
               * @brief Returns the number of references that are linked to the underlying object.
               * @return the number of references that are linked to the underlying object.
               */
              uint32 NumberOfReferences() const;

              /**
               * @brief Verifies if this Reference links to the same object of \a sourceReference.
               * @param[in] sourceReference reference to be compared.
               * @return true if the \a sourceReference links to the same object as this Reference.
               */
              bool operator==(const Reference& sourceReference) const;

              /**
               * @brief Verifies if this Reference owns the same object of \a sourceReference.
               * @param[in] sourceReference reference to be compared.
               * @return true if the \a sourceReference does not own the same object as this Reference.
               */
              virtual bool operator!=(const Reference& sourceReference) const;

              /**
               * @brief Assignment operator.
               * @details This reference will be referencing the same object as the \a  sourceReference.
               * @param[in] sourceReference the source reference to be assigned to this reference.
               * @return a reference to the object referenced by \a sourceReference.
               */
              virtual Reference& operator=(const Reference& sourceReference);

              /**
               * @brief Assignment operator.
               * @details It will increment the number of references referencing the underlying object.
               * @param[in] pointer source object to assign.
               * @return a reference to the underlying object.
               */
              virtual Reference& operator=(Object * const pointer);

              /**
               * @brief Provides access to the underlying object linked by this Reference.
               * @return a pointer to the underlying object linked by this Reference.
               */
              Object* operator->();

          protected:
              /**
               * The pointer to the referenced object.
               */
              /*lint -e9150 the member is not private so that ReferenceT can access it.*/
              /*lint -sem(MARTe::Reference::RemoveReference,cleanup)*/
              Object* objectPointer;

          private:
              /**
               * @brief Prevents the copying of a reference by taking its address.
               * @return a copy of this reference.
               */
              //LCOV_EXCL_START
                      /*lint -e9135 unity operator overloaded in order to implement access to the shared object.*/
                      virtual Reference* operator&() {
                          return this;
                      }
                      //LCOV_EXCL_STOP
                      /**
                       * @brief Instantiates a new object of type className in the specified heap.
                       * @param[in] className the name of the class.
                       * @param[in] heap the heap where the object is to be allocated.
                       * @return a new object of the specified class or NULL if the \a className does not exist.
                       */
                      Object *CreateByName(const char8 * const className,
                      HeapI* const heap) const;

                  };

              }
              /*---------------------------------------------------------------------------*/
              /*                        Inline method definitions                          */
              /*---------------------------------------------------------------------------*/

#endif /* REFERENCE_H_ */

