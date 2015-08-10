/**
 * @file Reference.cpp
 * @brief Source file for class Reference
 * @date Aug 6, 2015
 * @author aneto
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

 * @details This source file contains the definition of all the methods for
 * the class Reference (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItem.h"
#include "Reference.h"
#include "ClassRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Reference::Reference() {
    objectPointer = NULL_PTR(Object*);
}

Reference::Reference(const Reference& sourceReference) {
    objectPointer = NULL_PTR(Object*);
    (*this) = sourceReference;
}

Reference::Reference(const char8* const typeName, const Heap &heap) {
    objectPointer = NULL_PTR(Object*);
    Object *objPtr = ClassRegistryDatabase::Instance().CreateByName(typeName, heap);
    if (objPtr != NULL_PTR(Object*)) {
        objectPointer = objPtr;
    }

    if (objectPointer == NULL_PTR(Object*)) {
        delete objPtr;
        objectPointer = NULL_PTR(Object*);
    }
    else {
        objectPointer->IncrementReferences();
    }
}

Reference::Reference(Object * const pointer) {
    Reference::RemoveReference();
    objectPointer = pointer;
    if (objectPointer != NULL_PTR(Object*)) {
        objectPointer->IncrementReferences();
    }
}

/*lint -e{1551} the only risk of throwing an exception would be for
 * RemoveReference() to decrement references in a NULL objectPointer
 * which is properly protected. The delete of objectPointer at least
 * for versions < C++11 do not through an exception. */
/*lint -sem(Reference::RemoveReference,cleanup)*/
Reference::~Reference() {
    Reference::RemoveReference();
}

/*lint -e{715} data and createOnly not referenced to be removed when the method is implemented in the future*/
bool Reference::Initialise(const StructuredData &data, const bool &createOnly) {
//TODO
    return true;
}

void Reference::RemoveReference() {
    if (objectPointer != NULL) {
        uint32 numberOfReferences = objectPointer->DecrementReferences();

        if (numberOfReferences == 0u) {
            delete objectPointer;  //Polimorphism used here it need to destroy derived object.
        }
    }
    objectPointer = NULL_PTR(Object*);
}

bool Reference::IsValid() const {
    return (objectPointer != NULL);
}

Reference& Reference::operator=(const Reference& sourceReference) {
    if (this != &sourceReference) {
        RemoveReference();
        if (sourceReference.IsValid()) {
            sourceReference.objectPointer->IncrementReferences();
            /*lint -e1555 direct copy of member in copy assignment is required given that this is member
             * that is to be shared by all the references owning the pointer*/
            objectPointer = sourceReference.objectPointer;
        }
    }
    return *this;
}

uint32 Reference::NumberOfReferences() const {
    uint32 nOfReferences = 0u;
    if (IsValid()) {
        /*lint -e613 IsValid() guarantees that objectPointer != NULL*/
        nOfReferences = objectPointer->NumberOfReferences();
    }
    return nOfReferences;
}

bool Reference::operator==(const Reference& sourceReference) const {
    return (objectPointer == sourceReference.objectPointer);
}

bool Reference::operator!=(const Reference& sourceReference) const {
    return (objectPointer != sourceReference.objectPointer);
}

Object* Reference::operator->() {
    return objectPointer;
}

bool Reference::Clone(Reference &sourceReference) {
    bool ok = sourceReference.IsValid();
    if (ok) {
        Object * tmp = sourceReference->Clone();
        if (tmp != NULL) {
            RemoveReference();
            objectPointer = tmp;
            objectPointer->IncrementReferences();
            // This is necessary, otherwise when
            // GCReference::Clone is called by
            // GCRTemplate, at this point the IsValid
            // function of GCRTemplate would be called,
            // returning false as the setup of GCRTemplate
            // Tobject is not yet done.
            ok = Reference::IsValid();
        }
        else {
            ok = false;
        }
    }
    return ok;
}
