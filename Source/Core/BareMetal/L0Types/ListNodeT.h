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

#ifndef LISTNODET_H_
#define LISTNODET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ListNode.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template <class loadClass>
class  ListNodeT:public ListNode{
public:

	/**
	 *
	 */
	ListNodeT(const loadClass & loadIn): ListNode(),load(loadIn){

	}

	/**
	 *
	 */
	virtual ~ListNodeT(){

	}

#if 0
	/**
	 *
	 */
	inline ListNodeT<loadClass> &Next(){
		ListNodeT<loadClass> *lnt = reinterpret_cast<ListNodeT<loadClass> *>(next);
		return *lnt;
	}
#endif
public:

	/**
	 * the payload
	 */
	loadClass load;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}
#endif /* LISTNODET_H_ */

