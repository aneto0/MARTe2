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
    objectPointer = NULL;
}

Reference::Reference(const Reference& object) {
    objectPointer = NULL;
    (*this) = object;
}

Reference::Reference(Object * pointer) {
    objectPointer = NULL;
    *this = pointer;
}

Reference::Reference(const char8* typeName,
                     Heap &heap) {
    objectPointer = NULL;
    Object *objPtr = ClassRegistryDatabase::Instance().CreateByName(typeName, heap);
    if (objPtr != NULL) {
        objectPointer = dynamic_cast<Object *>(objPtr);
    }

    if (objectPointer == NULL) {
        delete objPtr;
        objectPointer = NULL;
        return;
    }
    else {
        objectPointer->IncrementReferences();
    }
}

Reference& Reference::operator=(Object * pointer) {
    RemoveReference();
    objectPointer = pointer;
    if (objectPointer != NULL) {
        objectPointer->IncrementReferences();
    }
    return *this;
}

Reference::~Reference() {
    RemoveReference();
}

bool Reference::Initialise(const StructuredData &data,
                           bool createOnly) {
//TODO
    return true;
}

void Reference::RemoveReference() {
    if (objectPointer != NULL) {
        uint32 numberOfReferences = objectPointer->DecrementReferences();

        if (numberOfReferences == 0) {
            delete objectPointer;  //Polimorphism used here it need to destroy derived object.
        }
    }
    objectPointer = NULL;
}

bool Reference::IsValid() const {
    return (objectPointer != NULL);
}

Reference& Reference::operator=(const Reference& reference) {
    RemoveReference();
    if (reference.IsValid()) {
        reference.objectPointer->IncrementReferences();
        objectPointer = reference.objectPointer;
    }
    return *this;
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
    bool ok = false;
    if (reference.IsValid()) {
        Object * tmp = reference->Clone();
        if (tmp != NULL) {
            RemoveReference();
            objectPointer = tmp;
            objectPointer->IncrementReferences();
            ok = true;
        }
    }
    return ok;
}
