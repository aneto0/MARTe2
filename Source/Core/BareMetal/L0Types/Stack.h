/**
 * @file Stack.h
 * @brief Header file for class List
 * @date May 14, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef STACK_H
#define STACK_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "List.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Simplified implementation of a list.
 * Does not expose internals. user interacts only with payload type loadClass.
 * Implementation strongly hinges on iterator.
 * To implement complex functionalities simply implement appropriate iterators
 * Support GenericIterator which is agnostic of internal List structure.
 */
template <class loadClass>
class  Stack: protected List<loadClass>{
public:

	/**
	 * TODO
	 */
	inline Stack();

	/**
	 * TODO
	 */
	virtual  ~Stack();

	/**
	 * @brief Inserts an element at the top of the stack.
	 * @param[in] data the reference to the data to be saved. its type must be compatible with loadClass
	 * i.e. I can create a loadClass() from a compatible
	 * */
	template <class compatible>
 	inline ErrorManagement::ErrorType Push( const compatible &data);

	/**
	 * @brief Removes and deletes the first element of the Stack. saves the payload to data.
	 * @param[in] data the reference to where to save the data retrieved.
	 * */
	template <class compatible>
	inline ErrorManagement::ErrorType Pop( compatible &data);

	/**
	 * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
	 * @param[in] index the position of the requested element (0 means the first element).
	 * @return a pointer to the element at index position.
	 */
	template <class compatible>
	inline ErrorManagement::ErrorType Peek( compatible &data, const uint32 index) const;

	/**
	 * @brief Returns the number of elements in the list.
	 * @return the number of the elements in the list.
	 */
	inline uint32 Size() const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <class loadClass>
Stack<loadClass>::Stack():List<loadClass>(){
}

template <class loadClass>
Stack<loadClass>::~Stack(){
}

template <class loadClass>
uint32 Stack<loadClass>::Size() const{
	return List<loadClass>::Size();
}

template <class loadClass>
template <class compatible>
ErrorManagement::ErrorType Stack<loadClass>::Push(const compatible &data){
	return List<loadClass>::Insert(data,0);
}

template <class loadClass>
template <class compatible>
ErrorManagement::ErrorType Stack<loadClass>::Pop( compatible &data){
	return List<loadClass>::Remove(data,0);
}

template <class loadClass>
template <class compatible>
ErrorManagement::ErrorType Stack<loadClass>::Peek(compatible &data, const uint32 index) const{
	ErrorManagement::ErrorType ret;
	loadClass *dataP = List<loadClass>::operator [](index);

	ret.outOfRange = (dataP == NULL_PTR(loadClass *));

	if (ret){
		data = *dataP;
	}
	return ret;
}

} // Marte



#endif /* LIST_H_ */
