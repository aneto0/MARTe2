/**
 * @file ListNode.cpp
 * @brief Source file for class ListNode
 * @date 05/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class ListNode (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/* -------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ListNode.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



namespace MARTe {

ListNode::~ListNode()
{
	next = NULL_PTR(ListNode *);
}


ErrorManagement::ErrorType ListNode::InsertAfter(ListNode *node, uint32 skipNo){
	ErrorManagement::ErrorType ret;

	if (node == NULL){
		ret.parametersError = true;
	} else
	if (node->next != NULL){
		ret.illegalOperation = true;
	}
	ListNode *location = this;

	while ((skipNo > 0) && (ret)){
		location = location->next;
		skipNo--;
		if (location == NULL){
			ret.outOfRange = true;
		}
	}

	if  (ret){
		node->next = location->next;
		location->next = node;
	}

	return ret;
}

ErrorManagement::ErrorType ListNode::RemoveAfter(ListNode *&node, uint32 skipNo){
	ErrorManagement::ErrorType ret;

	ListNode *location = this;

	while ((skipNo > 0) && (ret)){
		location = location->next;
		skipNo--;
		ret.outOfRange = (location == NULL);
	}

	if (ret){
		node = location->next;
		ret.outOfRange = (location->next == NULL);
	}

	if (ret){
		location->next = node->next;
	}

	return ret;
}

ErrorManagement::ErrorType ListNode::Peek(ListNode *&node, uint32 skipNo) const{
	ErrorManagement::ErrorType ret;

	const ListNode *location = this;

	while ((skipNo > 0) && (ret)){
		location = location->next;
		skipNo--;
		ret.outOfRange = (location == NULL);
	}

	if (ret){
		node = location->next;
		ret.outOfRange = (location->next == NULL);
	}

	return ret;
}



}
