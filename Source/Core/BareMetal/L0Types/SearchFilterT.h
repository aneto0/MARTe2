/**
 * @file SearchFilterT.h
 * @brief Header file for class SearchFilterT
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

 * @details This header file contains the declaration of the class SearchFilterT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEARCHFILTERT_H_
#define SEARCHFILTERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "SearchFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Template version of the SearchFilter interface.
 * @details This interface is the template version of the interface functor
 * SearchFilter, parameterising the type of the object that the Test method
 * will check for compliance, with respect to a given searching criteria.
 * @tparam T the type of the object which will be checked by the Test method.
 * @warning It is expected that T be descendant of LinkedListable.
 */
template<typename T>
class DLL_API SearchFilterT: public SearchFilter {
public:

    /**
     * @brief Destructor.
     */
    virtual ~SearchFilterT();

    /**
     * @brief LinkedListable searching callback function.
     * @details This function is called for every element in the list being searched (i.e. traversed).
     * @param[in] data the current LinkedListable element to be tested.
     * @return true if \a data meets the search criteria.
     */
    virtual bool Test(T *data)=0;

    virtual bool Test(LinkedListable *data);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
SearchFilterT<T>::~SearchFilterT() {
}

template<typename T>
bool SearchFilterT<T>::Test(LinkedListable *data) {
	bool ret;
	T* target = dynamic_cast<T*>(data);
	if (target == NULL_PTR(T*)) {
		ret = false;
	}
	if (ret) {
		ret = SearchFilterT<T>::Test(target);
	}
	return ret;
}

}

#endif /* SEARCHFILTERT_H_ */

