/*
 * BalancedTreeHolder.h
 *
 *  Created on: 13 Jun 2017
 *      Author: sartofi
 */

#ifndef LIST_H
#define LIST_H

#include "CompilerTypes.h"
#include "GenericIterator.h"
#include "ErrorManagement.h"
#include "ListNode.h"
#include "ListNodeT.h"

namespace MARTe {




/**
 * @brief Simplified implementation of a list.
 * Does not expose internals. user interacts only with payload type loadClass.
 * Implementation strongly hinges on iterator.
 * To implement complex functionalities simply implement appropriate iterators
 * Support GenericIterator which is agnostic of internal List structure.
 */
template <class loadClass>
class  List: protected ListNode{
public:

	/**
	 * TODO
	 */
	List();

	/**
	 * TODO
	 */
	virtual ~List();

	/**
	 * Allows iterating a GenericIterator functor on each element of the list
	 * Depending on the functor result the iteration may be suspended, an element may be deleted, replaced or inserted.
	 * Use this method to implement searches, multiple-searches, ordered insertions, sorting (by multiple calls) etc..
	 */
	inline ErrorManagement::ErrorType Iterate(GenericIterator<loadClass> &iterator);

	/**
	 * @brief Inserts a list in the specified position of the list.
	 * @details If the specified position is greater the the list size, the list input is inserted at the end.
	 * @param[in] data the reference to the data to be saved.
	 * @param[in] index is the position in the list where \a p must be inserted.	position 0xFFFFFFFF means to the end */
	inline ErrorManagement::ErrorType Insert( loadClass &data, uint32 position=0);

	/**
	 * @brief Browses the tree smallest to greatest. After an insertion the order will change as the insertion will be somewhere in the middle
	 * @param[in] index the position of the requested element (0 means the first element).
	 * @return a pointer to the element at index position.
	 */
	loadClass *operator[](const uint32 index) const;

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
inline ErrorManagement::ErrorType List<loadClass>::Insert( loadClass &data, uint32 position){
	ListNodeT<loadClass> *newNode = new ListNodeT<loadClass>(data);
	/// the handling of new allocation failure is performed by ListGeneric
	ErrorManagement::ErrorType ret=InsertAfter(newNode,position);
	if (ret){
		size++;
	}
	return ret;
}

template <class loadClass>
loadClass *List<loadClass>::operator[](const uint32 index) const{
        uint32 ix = index;
	loadClass *ret = NULL;
	if (ix < size){
		ListNodeT<loadClass> *ptr = reinterpret_cast<ListNodeT<loadClass> *>(next);
		while ((ix > 0) && (ptr!=NULL)){
			ix--;
			ptr = ptr->Next();
		}
		if ((ptr !=NULL) && (ix == 0)){
			ret = &ptr->load;
		}
	}
	return ret;
}


template <class loadClass>
inline ErrorManagement::ErrorType List<loadClass>::Iterate(GenericIterator<loadClass> &iterator){
		ErrorManagement::ErrorType ret;
		IteratorAction ia;
		ListNode *current = this;

//	no reason to be an error!	ret.illegalOperation = (current->next == NULL);
		while (ret && (current->next !=NULL)){

			// recover original ListNodeT type
			// can be done on the next! not on the first current as it is of different type
			if (ret){
				// whether to switch to next for next cycle
				// after a delete there is no need anymore
				//bool getNext=true;
//				ListNodeT<loadClass> *target = reinterpret_cast<ListNodeT<loadClass>*>(current->next);
				ListNodeT<loadClass> *target = static_cast<ListNodeT<loadClass>*>(current->next);

				ia = iterator.Do(target->load);
				if (ia.notAnErrorCode){
					switch(ia.ActionCode()){
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
