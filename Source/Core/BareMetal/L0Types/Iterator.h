/**
 * @file Iterator.h
 * @brief Header file for class Iterator
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

 * @details This header file contains the declaration of the class Iterator
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ITERATOR_H_
#define ITERATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListable.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Interface which defines a functor responsible of doing an action on
 * a LinkedListable instance.
 * @details The classes that realise this interface will implement a specific
 * action assuming that the parameter will be an object of type LinkedListable.
 * @note In order to apply an action on all items of a list, the user will
 * have to call the Do method for each element of the list.
 */
class Iterator {
public:

    /**
     * @brief Destructor.
     */
    virtual ~Iterator() {

    }
    /**
     * @brief Iterator callback function.
     * @details This function is called for every element in the list being traversed.
     * @param[in] data the current LinkedListable element of the list being traversed.
     */
    virtual void Do(LinkedListable *data)=0;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ITERATOR_H_ */

