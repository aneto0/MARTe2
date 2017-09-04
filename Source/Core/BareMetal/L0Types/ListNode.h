/**
 * @file ListNode.h
 * @brief Header file for class ListNode
 * @date 02/09/2017
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

 * @details This header file contains the declaration of the class ListNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LISTNODE_H_
#define LISTNODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class is a singly linked node which is intended to be chained
 * with other nodes, creating an unbounded singly linked list.
 *
 * @details This class is the internal implementation of List
 */
class   ListNode{
public:
	/**
	 *
	 */
	inline ListNode();
	/**
	 *
	 */
	virtual ~ListNode();

	/**
	 * move ahead @param skipNo nodes
	 * insert node by linking it to the next variable of the last node.
	 * @return ellegalOperation if node->next is not NULL!
	 * @return parametersError if node = NULL
	 */
	ErrorManagement::ErrorType InsertAfter(ListNode *node, uint32 skipNo);

public:
	/**
	 *
	 */
	ListNode  *next;


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline ListNode::ListNode():next(NULL){
}


}
#endif /* LINKEDLISTABLE_H_ */
