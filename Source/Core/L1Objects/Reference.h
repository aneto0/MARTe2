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

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCE_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief TODO
 */
class Reference {

public:

    /**
     * @brief Creates an empty reference.
     */
    Reference();

    /**
     * @brief Creates a new reference from an existing one.
     * @param object the source reference.
     */
    Reference(const Reference& object);

    /**
     * @brief Creates a new object of type @param typeName and builds a reference to it.
     * @param the class type.
     */
    Reference(const char* typeName);

    /**
     * @brief Creates a new reference from an object pointer.
     * @param pointer the source pointer to be referenced.
     */
    Reference(Reference * pointer) {
        objectPointer = NULL;
        *this = pointer;
    }

    /**
     * @brief Create an object from a structured list of elements.
     * @param createOnly if true the object Initialise method is not called.
     * @return true if the object was successfully created and initialised.
     */
    virtual bool Initialise(const StructuredData &data,
                            bool createOnly = false);

    /**
     * @brief Removes a reference to the underlying object.
     */
    virtual void RemoveReference();

    /**
     * @brief Assignment operator.
     * @param reference source reference to assign. It will increment the number of references pointing to the
     * underlying object.
     * @return a reference to the underlying object.
     */
    Reference& operator=(const Reference& reference);

    /**
     * @brief Assignment operator.
     * @param pointer source object to assign. It will increment the number of references pointing to the
     * underlying object.
     * @return a reference to the underlying object.
     */
    Reference& operator=(Object * pointer);

    /**
     * Destructor. Removes the reference to the underlying object.
     */
    virtual ~Reference();

    /**
     * @brief Verifies if the reference is pointing to a valid object.
     * @return true if the reference is pointing to a valid object.
     */
    virtual bool IsValid() const;

    /**
     * @brief Returns the number of references pointing to the underlying object.
     * @return the number of references pointing to the underlying object.
     */
    inline uint32 NumberOfReferences() const;

    /**
     * @brief Verifies if the reference points to the same object.
     * @param[in] reference reference to be compared.
     * @return true if the reference point to the same object.
     */
    inline bool operator==(const Reference& reference) const;

    /**
     * @brief Verifies if the reference does not point to the same object.
     * @param[in] reference reference to be compared.
     * @return true if the reference do not point to the same object.
     */
    inline bool operator!=(const GCReference& reference) const;

    /**
     * @brief Provides access to the underlying object pointed by this reference.
     * @return a pointer to the underlying object pointed by this reference.
     */
    inline Object* operator->() const;

    /**
     * @brief Creates a reference to a different object.
     * @param[in] reference the reference holding the source object.
     * @return true if the source reference and source object are valid.
     */
    inline bool Clone(const Reference &reference);

private:

    /**
     * @brief Prevents the copying of a reference by taking its address.
     * @return a copy of this reference.
     */
    Reference* operator&() {
        return this;
    }

    /**
     * The pointer to the referenced object.
     */
    Object* objectPointer;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool Reference::IsValid() const {
    return (objectPointer != NULL);
}

uint32 Reference::NumberOfReferences() const {
    if (!IsValid()) {
        return 0;
    }
    return objectPointer->NumberOfReferences();
}

bool Reference::operator==(const Reference& reference) const {
    return (objectPointer == reference.objectPointer);
}

bool Reference::operator!=(const Reference& reference) const {
    return (objectPointer != reference.objectPointer);
}

Object* Reference::operator->() const {
    return objectPointer;
}

bool Reference::Clone(const Reference &reference) {
    if (reference.IsValid()) {
        Object * tmp = reference->Clone();
        if (tmp != NULL) {
            RemoveReference();
            objectPointer = tmp;
            objectPointer->IncrementReferences();
        }
        // This is necessary, otherwise when
        // GCReference::Clone is called by
        // GCRTemplate, at this point the IsValid
        // function of GCRTemplate would be called,
        // returning false as the setup of GCRTemplate
        // Tobject is not yet done.
        return Reference::IsValid();
    }
}

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCE_H_ */

