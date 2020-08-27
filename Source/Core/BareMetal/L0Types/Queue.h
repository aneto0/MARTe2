/**
 * @file Queue.h
 * @brief Header file for class Queue
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

#ifndef QUEUE_H_
#define QUEUE_H_

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
 * @brief Simplified implementation of a queue.
 * Does not expose internals. user interacts only with payload type loadClass.
 * Implementation strongly hinges on iterator.
 * To implement complex functionalities simply implement appropriate iterators
 * Support GenericIterator which is agnostic of internal List structure.
 */
template <class loadClass>
class  Queue: protected List<loadClass>{
public:

	/**
	 * TODO
	 */
	inline Queue();

	/**
	 * TODO
	 */
	virtual  ~Queue();

	/**
	 * @brief Inserts an element at the start of the Q (end of the list).
	 * @param[in] data the reference to the data to be saved.
	 * */
 	inline ErrorManagement::ErrorType Insert( loadClass &data);

	/**
	 * @brief Removes and deletes the last element added to the queue (top of the list). saves the payload to data.
	 * @param[in] data the reference to where to save the data retrieved.
	 * */
	inline ErrorManagement::ErrorType Remove( loadClass &data);

	/**
	 * @brief Browses the queue from the element that was first added (0) to that that was added last
	 * @param[in] index the position of the requested element (0 means the first added element).
	 * @return a pointer to the element at index position.
	 */
	inline ErrorManagement::ErrorType Peek( loadClass &data, const uint32 index) const;

	/**
	 * @brief Returns the number of elements in the list.
	 * @return the number of the elements in the list.
	 */
	inline uint32 Size() const;

private:
	/**
	 * @ to improve efficiency in adding to the queue
	 */
	ListNode *lastAdded;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template <class loadClass>
Queue<loadClass>::Queue():List<loadClass>(){
	lastAdded = this;
}

template <class loadClass>
Queue<loadClass>::~Queue(){
}

template <class loadClass>
uint32 Queue<loadClass>::Size() const{
	return List<loadClass>::Size();
}

template <class loadClass>
ErrorManagement::ErrorType Queue<loadClass>::Insert( loadClass &data){
	ErrorManagement::ErrorType ret;
	ListNodeT<loadClass> *newNode = new ListNodeT<loadClass>(data);

	ret = lastAdded->InsertAfter(newNode,0);

	if (ret){
		lastAdded = newNode;
		List<loadClass>::size++;
	}

	return ret;
}

template <class loadClass>
ErrorManagement::ErrorType Queue<loadClass>::Remove( loadClass &data){
	return List<loadClass>::Remove(data,0);
}

template <class loadClass>
ErrorManagement::ErrorType Queue<loadClass>::Peek(loadClass &data, const uint32 index) const{
	ErrorManagement::ErrorType ret;
	loadClass *dataP = List<loadClass>::operator [](index);

	ret.outOfRange = (dataP == NULL_PTR(loadClass *));

	if (ret){
		data = *dataP;
	}
	return ret;
}

} // MARTe

#endif /* QUEUE_H_ */
