/**
 * @file List.h
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

#ifndef LIST_H_
#define LIST_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "GenericIterator.h"
#include "ErrorManagement.h"
#include "ListNode.h"
#include "ListNodeT.h"


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
 * Note that the first element of this node is always empty (no payload) as it simply provides
 */
template <class loadClass>
class  List: protected ListNode{
public:

	/**
	 * TODO
	 */
	inline List();

	/**
	 * TODO
	 */
	virtual  ~List();

	/**
	 * Allows iterating a GenericIterator functor on each element of the list
	 * Depending on the functor result the iteration may be suspended, an element may be deleted, replaced or inserted.
	 * Use this method to implement searches, multiple-searches, ordered insertions, sorting (by multiple calls) etc..
	 */
	inline ErrorManagement::ErrorType Iterate(GenericIterator<loadClass> &iterator);

	/**
	 * @brief Inserts an element in the specified position of the list.
	 * @details If the specified position is greater the the list size, the input is inserted at the end.
	 * @param[in] data the reference to the data to be saved.
	 * @param[in] index is the position in the list where \a data must be inserted.	position 0xFFFFFFFF means to the end */
	template <class compatible>
	inline ErrorManagement::ErrorType Insert( const compatible &data, uint32 position=0);

	/**
	 * @brief Removes an element from the list at the specified position.
	 * @details If the specified position is greater the the list size, returns error.
	 * @param[in] data the reference to the location where data retrieved shall be saved.
	 * @param[in] index is the position in the list where \a data must be removed.	 */
	template <class compatible>
	inline ErrorManagement::ErrorType Remove( compatible &data, uint32 position=0);

	/**
	 * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
	 * @param[in] index the position of the requested element (0 means the first element).
	 * @return a pointer to the element at index position.
	 */
	loadClass *operator[](const uint32 index);

	/**
	 * @brief Returns the number of elements in the list.
	 * @return the number of the elements in the list.
	 */
	inline uint32 Size() const;

private:
	/**
	 * TODO
	 */
	uint32 		size;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


/**
 *
 */
template <class loadClass>
List<loadClass>::List():ListNode(){
	size = 0;
}

/**
 *
 */
template <class loadClass>
List<loadClass>::~List(){
}

template <class loadClass>
inline uint32 List<loadClass>::Size() const{
	return size;
};


template <class loadClass>
template <class compatible>
inline ErrorManagement::ErrorType List<loadClass>::Insert(const compatible &data, uint32 position){
	ListNodeT<loadClass> *newNode = new ListNodeT<loadClass>(data);
	/// the handling of new allocation failure is performed by ListGeneric
	ErrorManagement::ErrorType ret;
	ret = InsertAfter(newNode,position);
	if (ret){
		size++;
	}
	return ret;
}

template <class loadClass>
template <class compatible>
ErrorManagement::ErrorType List<loadClass>::Remove( compatible &data, uint32 position){
	ListNode *savedNode = NULL_PTR(ListNode *);
	/// the handling of new allocation failure is performed by ListGeneric
	ErrorManagement::ErrorType ret;
	ret = RemoveAfter(savedNode,position);
	// ret ok means savedNode is ok
	if (ret){
		ListNodeT<loadClass> *ptr = reinterpret_cast<ListNodeT<loadClass> *>(savedNode);
		data = ptr->load;
		delete savedNode;
		size--;
	}
	return ret;
}

template <class loadClass>
loadClass *List<loadClass>::operator[](const uint32 index){
	ListNode *savedNode = NULL_PTR(ListNode *);
	/// the handling of new allocation failure is performed by ListGeneric
	ErrorManagement::ErrorType ret;
	loadClass *data = NULL;
	ret = ListNode::Peek(savedNode,index);
	// ret ok means savedNode is ok
	if (ret){
		ListNodeT<loadClass> *ptr = reinterpret_cast<ListNodeT<loadClass> *>(savedNode);
		data = &ptr->load;
	}
	return data;
}


template <class loadClass>
inline ErrorManagement::ErrorType List<loadClass>::Iterate(GenericIterator<loadClass> &iterator){
		ErrorManagement::ErrorType ret;
		IteratorAction ia;
		ListNode *current = this;
        bool completed = false;

//	no reason to be an error!	ret.illegalOperation = (current->next == NULL);
		while (ret && (current->next !=NULL) && !completed){

			// recover original ListNodeT type
			// can be done on the next! not on the first current as it is of different type
			if (ret){
				// whether to switch to next for next cycle
				// after a delete there is no need anymore
				//bool getNext=true;
				ListNodeT<loadClass> *target = static_cast<ListNodeT<loadClass>*>(current->next);

				ia = iterator.Do(target->load);
				if (ia.notAnErrorCode){
					switch(ia.ActionCode()){
					case isCompleted: {
						completed = true;
					} break;
					case noAction:{
						current = current->next;
					}break;
					case deleteNode:{
						ListNode *sav = current->next;
						current->next = sav->next;
						sav->next = NULL;
						delete sav;
						size--;
					}break;
					case replaceNode:{
						loadClass lc;
						if (!iterator.GetNodeToInsert(lc)){
							ret.format_as_integer = ErrorManagement::InvalidOperation;
						}
						ListNode *rep = NULL_PTR(ListNode *);
						if (ret){
							rep = new ListNodeT<loadClass>(lc);
							if (rep == NULL){
								ret.format_as_integer = ErrorManagement::FatalError;
							}
						}
						if (ret){
							ListNode *sav = current->next;
							current->next = rep;
							rep->next = sav->next;
							sav->next = NULL;
							delete sav;
							current = current->next;
						}
					}break;
					case insertNodeBefore:{
						loadClass lc;
						if (!iterator.GetNodeToInsert(lc)){
							ret.format_as_integer = ErrorManagement::InvalidOperation;
						}
						ListNode *rep = NULL_PTR(ListNode *);
						if (ret){
							rep = new ListNodeT<loadClass>(lc);
							if (rep == NULL){
								ret.format_as_integer = ErrorManagement::FatalError;
							}
						}
						if (ret){
							rep->next = current->next;
							current->next = rep;
							// skip to next to avoid iterating twice!
							current = current->next;
							size++;
							current = current->next;
						}

					}break;
					case insertNodeAfter:{
						loadClass lc;
						if (!iterator.GetNodeToInsert(lc)){
							ret.format_as_integer = ErrorManagement::InvalidOperation;
						}
						ListNode *rep = NULL_PTR(ListNode *);
						if (ret){
							rep = new ListNodeT<loadClass>(lc);
							if (rep == NULL){
								ret.format_as_integer = ErrorManagement::FatalError;
							}
						}
						if (ret){
							// skip to next and then insert
							current = current->next;
							rep->next = current->next;
							// this way the inserted will not be Iterated into
							current->next = rep;
							size++;
							current = current->next;
						}

					}break;
					default:{
						ret.unsupportedFeature = true;
					}
					} // end case

				} else { // it is an error code
					ret = ia;
					if (ret){
						current = current->next;
					}
				}
			}

		}
		return ret;
	}

} // Marte



#endif /* LIST_H_ */
