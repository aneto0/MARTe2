/**
 * @file ReferenceT.cpp
 * @brief Source file for class ReferenceT
 * @date 07/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class ReferenceT (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItem.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe{
/*lint -e{1566} Init function initializes members */
template<typename T>
ReferenceT<T>::ReferenceT(const Reference& sourceReference) :
        Reference(sourceReference) {
    //use operator =
    (*this) = sourceReference;
}

template<typename T>
ReferenceT<T>::~ReferenceT() {
    typeTObjectPointer = static_cast<T *>(NULL);
}

template<typename T>
void ReferenceT<T>::RemoveReference() {
    typeTObjectPointer = static_cast<T *>(NULL);

    Reference::RemoveReference();
}

template<typename T>
bool ReferenceT<T>::IsValid() const {

    return (Reference::IsValid()) ? (typeTObjectPointer != NULL) : false;
}

/*lint -e{929} -e{925} the current implementation of the LinkedListable requires pointer to pointer casting*/
template<typename T>
ReferenceT<T>& ReferenceT<T>::operator=(const Reference& sourceReference) {
    RemoveReference();

    if (sourceReference.IsValid()) {
        // do this first to allow access to objectPointer
        Reference::operator=(sourceReference);
        typeTObjectPointer = dynamic_cast<T*>(objectPointer);
        if (typeTObjectPointer == NULL) {
            RemoveReference();
        }
    }

    return *this;
}

template<typename T>
T* ReferenceT<T>::operator->() {
    return typeTObjectPointer;
}

template<typename T>
ReferenceT<T>* ReferenceT<T>::operator&() {
    return this;
}

template<typename T>
bool ReferenceT<T>::Initialise(const StructuredData &data,
                               const bool &createOnly) {
    Reference ref;
    bool ok = true;
    if (ref.Initialise(data, createOnly)) {
        *this = ref;
        ok = IsValid();
    }
    else {
        ok = false;
    }
    return ok;
}

}
