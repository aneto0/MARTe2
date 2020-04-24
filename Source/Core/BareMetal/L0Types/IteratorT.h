/**
 * @file IteratorT.h
 * @brief Header file for class IteratorT
 * @date 06/08/2015
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

 * @details This header file contains the declaration of the class IteratorT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ITERATORT_H_
#define ITERATORT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Iterator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Template version of the Iterator interface.
 * @details This interface is the template version of the interface functor
 * Iterator, parameterising the type of the object on which the Do method
 * will apply an specific action.
 * @tparam T the type of the object on which an action will be applied by
 * the Do method.
 */
template<typename T>
class IteratorT: public Iterator {
public:
    /**
     * @brief Destructor.
     */
    virtual ~IteratorT() {

    }
    /**
     * @brief The function performing the desired specific action on the input parameter.
     * @param[in] data is a generic template type.
     */
    virtual void Do(T *data)=0;

    void Do(LinkedListable *data);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
void IteratorT<T>::Do(LinkedListable *data){
    T *Tdata = dynamic_cast<T*>(data);
    Do(Tdata);
}

}

#endif /* ITERATORT_H_ */

