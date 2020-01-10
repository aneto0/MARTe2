/**
 * @file ReferenceT.h
 * @brief Header file for class ReferenceT
 * @date 07/08/2015
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
 *
 * @details This header file contains the declaration of the class ReferenceT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCET_H_
#define REFERENCET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Reference to a constant T::Object.
 */
#define ConstReferenceT(x) ReferenceT<const x>

/**
 * @brief Template version of the shared pointer implementation (see Reference).
 * @param T the base type of the objects referenced by instances of this class
 */
template<typename T>
class DLL_API ReferenceT: public Reference {
public:

    /**
     * @brief Creates an empty reference (referenced object is set to NULL).
     */
    ReferenceT();

    /**
     * @brief Creates an empty reference or a reference to base type T.
     * @param[in] heap the heap responsible for allocating the object.
     */
    ReferenceT(HeapI * const heap);

    /**
     * @brief Creates a reference to an object that inherits from base type T.
     * @details The recommended usage is ReferenceT<T>(new MyClass()). This operation might fail (if T
     * does not inherit from Object) in which case an empty reference is created and IsValid will return false.
     * @param[in] p pointer to the object to be referenced.
     */
    ReferenceT(T * const p);

    /**
     * @brief Creates a ReferenceT from an existing ReferenceT.
     * @details This ReferenceT will own the same object referenced by sourceReference.
     * @param[in] sourceReference the Reference to the object to be shared.
     */
    ReferenceT(const Reference& sourceReference);

    /**
     * @brief Creates a ReferenceT from an existing ReferenceT.
     * @param[in] sourceReference the reference to the object to be shared.
     */
    ReferenceT(const ReferenceT<T>& sourceReference);

    /**
     * @brief Instantiates a new object of type \a typeName and links a reference to it.
     * @details If the operation succeeds a reference to the new object is created,
     * otherwise an empty reference is created and a subsequent call to IsValid will return false.
     * @param[in] typeName the type (i.e. class name) of the object to be instantiated.
     * @param[in] heap the heap responsible for allocating the object.
     */
    ReferenceT(const char8* const typeName,
               HeapI* const heap = static_cast<HeapI *>(NULL));

    /**
     * @brief Removes the reference to the underlying object (see RemoveReference).
     */
    virtual ~ReferenceT();

    /**
     * @brief Removes the reference to the underlying object.
     * @details If the number of references to the underlying object is zero, the object is deleted.
     */
    virtual void RemoveReference();

    /**
     * @brief Verifies if the reference to the underlying object is valid.
     * @details A valid reference is one where the referenced object is not NULL.
     * @return true if the referenced object is not NULL.
     */
    virtual bool IsValid() const;

    /**
     * @brief Assignment operator.
     * @details This reference will be referencing the same object as the sourceReference.
     * @param[in] sourceReference the source reference to be assigned to this reference.
     * @return a reference to the object referenced by \a sourceReference.
     */
    ReferenceT<T>& operator=(const ReferenceT<T>& sourceReference);

    /**
     * @brief Assignment operator.
     * @details This reference will be referencing the same object as the \a sourceReference.
     * @param[in] sourceReference the source reference to be assigned to this reference.
     * @return a reference to the object referenced by \a sourceReference.
     */
    virtual ReferenceT<T>& operator=(const Reference& sourceReference);

    /**
     * @brief Verifies if this Reference links to the same object of \a sourceReference.
     * @param[in] sourceReference reference to be compared.
     * @return true if the \a sourceReference links to the same object as this Reference.
     */
    /*lint -e{1511} Reference::operator== hidden on purpose.*/
    bool operator==(const ReferenceT<T>& sourceReference) const;

    /**
     * @brief Provides access to the object referenced by this Reference.
     * @return a pointer to the object referenced by this Reference.
     */
    virtual T* operator->() const;

    /**
     * @brief Creates an object from a structured list of elements.
     * @param[in] data the data to initialise the underlying object.
     * @param[in] initOnly if true the object is supposed to be already created.
     * @return true if the object was successfully created and initialized.
     */
    virtual bool Initialise(StructuredDataI &data,
                            const bool &initOnly);

private:

    /**
     * @brief Prevents the copying of a reference by taking its address.
     * @return a copy of this reference.
     */
    virtual ReferenceT<T>* operator &();

    /**
     * @brief Set thes internal pointers to NULL.
     */
    void Init();

    /**
     * The pointer to the referenced object.
     */
    T* typeTObjectPointer;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
void ReferenceT<T>::Init() {
    objectPointer = static_cast<Object *>(NULL);
    typeTObjectPointer = static_cast<T *>(NULL);
}

/*lint -e{1566} Init function initializes members */
template<typename T>
ReferenceT<T>::ReferenceT(const Reference& sourceReference) :
        Reference(sourceReference) {
    //use operator =
    (*this) = sourceReference;
}

/*lint -e{1566} Init function initializes members */
template<typename T>
ReferenceT<T>::ReferenceT() :
        Reference() {
    Init();
}

/*lint -e{1566} Init function initializes members */
/*lint -e{929} -e{925} the current implementation of the LinkedListable requires pointer to pointer casting
 * i.e. downcasting is necessary.*/
template<typename T>
ReferenceT<T>::ReferenceT(HeapI* const heap) :
        Reference() {
    Init();
    T *p = new (heap) T;
    if (p != NULL) {
        Object *obj;
        obj = dynamic_cast<Object *>(p);
        if (obj != NULL) {
            Reference::operator=(obj);
            typeTObjectPointer = p;
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ReferenceT: Dynamic cast failed.");
        }
    }
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
T* ReferenceT<T>::operator->() const {
    return typeTObjectPointer;
}

template<typename T>
ReferenceT<T>* ReferenceT<T>::operator&() {
    return this;
}

template<typename T>
bool ReferenceT<T>::Initialise(StructuredDataI &data,
                               const bool &initOnly) {
    bool ok = true;
    if (Reference::Initialise(data, initOnly)) {
        typeTObjectPointer = dynamic_cast<T*>(objectPointer);
        if (typeTObjectPointer == NULL) {
            Reference::RemoveReference();
            typeTObjectPointer = static_cast<T *>(NULL);
        }
    }
    else {
        REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ReferenceT: Failed Reference::Initialise()");
        ok = false;
    }
    return ok;
}

/*lint -e{1566} Init function initializes members */
/*lint -e{929} -e{925} the current implementation of the LinkedListable requires pointer to pointer casting
 * i.e. downcasting is necessary.*/
template<typename T>
ReferenceT<T>::ReferenceT(T * const p) :
        Reference() {
    Init();
    if (p != NULL) {
        Object *obj = dynamic_cast<Object *>(p);
        if (obj != NULL) {
            Reference::operator=(obj);
            typeTObjectPointer = p;
        }
    }
}

template<typename T>
ReferenceT<T>::ReferenceT(const ReferenceT<T>& sourceReference) :
        Reference(sourceReference) {
    //use operator =
    (*this) = sourceReference;
}

/*lint -e{929} -e{925} the current implementation of the LinkedListable requires pointer to pointer casting
 * i.e. downcasting is necessary.*/
template<typename T>
ReferenceT<T>::ReferenceT(const char8* const typeName,
                          HeapI* const heap) :
        Reference(typeName, heap) {
    typeTObjectPointer = static_cast<T *>(NULL);
    if (Reference::IsValid()) {
        typeTObjectPointer = dynamic_cast<T*>(objectPointer);
        if (typeTObjectPointer == NULL) {
            Reference::RemoveReference();
            typeTObjectPointer = static_cast<T *>(NULL);
            REPORT_ERROR_STATIC_0(ErrorManagement::FatalError, "ReferenceT: Dynamic cast failed.");
        }
    }
}

template<typename T>
ReferenceT<T>& ReferenceT<T>::operator=(const ReferenceT<T>& sourceReference) {
    if (this != &sourceReference) {
        RemoveReference();
        if (sourceReference.IsValid()) {
            Reference::operator=(sourceReference);
            typeTObjectPointer = sourceReference.typeTObjectPointer;
        }
    }
    return *this;
}

template<typename T>
bool ReferenceT<T>::operator==(const ReferenceT<T>& sourceReference) const {
    return (typeTObjectPointer == sourceReference.typeTObjectPointer);
}

}
#endif /* REFERENCET_H_ */
