/**
 * @file CircularStaticList.h
 * @brief Header file for class CircularStaticList
 * @date 18/04/2017
 * @author Bertrand Bauvir
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class CircularStaticList
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CIRCULAR_STATIC_LIST_H_
#define CIRCULAR_STATIC_LIST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticList.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Circular buffer implementation of the StaticList.
 */

/*lint -e{1712} the implementation does not provide default constructor*/
/*lint -esym(9107, MARTe::CircularStaticList*) [MISRA C++ Rule 3-1-1] required for template implementation*/
template<typename Type> class CircularStaticList: public StaticList<Type> {
public:

    /**
     * @brief Constructor. Allocates memory from the standard heap in order to create a StaticList with bufferSize elements.
     * @param[in] bufferSize the number of elements of the circular buffer.
     * @post
     *   index = 0u
     */
    CircularStaticList(const uint32 bufferSize);

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~CircularStaticList();

    /**
     * @brief Inserts data in the next position of the circular buffer.
     * @param[in] data the data to be inserted.
     * @return true if the data can be successfully inserted (see StaticList::Set).
     */
    bool PushData(Type& data);

    /**
     * @brief Replaces the data in the current buffer position and returns the removed element.
     * @param[in] dataIn the data to be inserted.
     * @param[out] dataOut the data that was replaced.
     * @return true if the data can be successfully inserted (see StaticList::Set).
     */
    bool PushData(Type& dataIn, Type& dataOut);

    /**
     * @brief Returns the last added element.
     * @param[out] dataOut the last added element.
     * @return true if the element can be successfully retrieved.
     */
    bool GetLast(Type& dataOut);

    /**
     * @brief Gets the current circular buffer index value.
     * @return the current circular buffer index value.
     */
    uint32 GetIndex();

private:

    /**
     * Circular buffer index.
     */
    uint32 index;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename Type> CircularStaticList<Type>::CircularStaticList(const uint32 bufferSize) :
        StaticList<Type>() {
    index = 0u;
    uint32 i;
    for (i = 0u; i < bufferSize; i++) {
        Type temp;
        (void) StaticList<Type>::Add(temp);
    }
}

template<typename Type> CircularStaticList<Type>::~CircularStaticList() {
    /* Nothing further to do than the default base class destructor */
}

template<typename Type> bool CircularStaticList<Type>::PushData(Type& data) {

    bool ok = StaticList<Type>::Set(index, data);

    if (ok) {
        index++;
    }

    if (index == StaticList<Type>::GetSize()) {
        index = 0u;
    }

    return ok;
}

template<typename Type> bool CircularStaticList<Type>::PushData(Type& dataIn, Type& dataOut) {
    bool ok = StaticList<Type>::Peek(index, dataOut);

    if (ok) {
        ok = PushData(dataIn);
    }

    return ok;
}

template<typename Type> bool CircularStaticList<Type>::GetLast(Type& dataOut) {
    bool ok = true;
    if (index > 0u) {
        ok = StaticList<Type>::Peek(index - 1, dataOut);
    }
    else {
        ok = StaticList<Type>::Peek(StaticList<Type>::GetSize() - 1, dataOut);
    }

    return ok;
}

template<typename Type> uint32 CircularStaticList<Type>::GetIndex() {
    return index;
}


} /* namespace MARTe */

#endif /* CIRCULAR_STATIC_LIST_H_ */

