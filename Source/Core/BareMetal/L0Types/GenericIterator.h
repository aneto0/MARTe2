/*
 * GenericIterator.h
 *
 *  Created on: 16 Jun 2017
 *      Author: sartofi
 */

#ifndef GENERICITERATOR_H_
#define GENERICITERATOR_H_

#include "ErrorType.h"

namespace MARTe{

/**
 *
 */
enum IA_ActionCodes{

	/** return this in case of no error and no action to do  */
	noAction          = 0x0,

	/** return this in case of no error and no action to do  */
	deleteNode        = 0x1,

	/** return this in case of no error and no action to do  */
	replaceNode       = 0x2,

	/** return this in case of no error and no action to do  */
	insertNodeAfter   = 0x3,

	/** return this in case of no error and no action to do  */
	insertNodeBefore  = 0x4,

	/** stop iterating, job is done	 */
	isCompleted       = 0x6,

	/**  it is an error interpret it accordingly */
	isAnError         = 0x7

};


/**
 *
 */
class IteratorAction: public ErrorManagement::ErrorType{
public:

	inline void SetActionCode(IA_ActionCodes code){
		format_as_integer = static_cast<ErrorManagement::ErrorIntegerFormat>(code)| ErrorManagement::NotAnErrorCode;
	}

	/**
	 * checks first that this is not an error!
	 */
	inline IA_ActionCodes ActionCode(){
		IA_ActionCodes ret = noAction;

		if (notAnErrorCode){
			ret = static_cast<IA_ActionCodes>(format_as_integer & 0x7);
		} else {
			ret = isAnError;
		}
		return ret;
	}


};



/**
 * @brief Interface which defines a functor responsible of doing an action on
 * a LinkedListable instance.
 * @details The classes that realise this interface will implement a specific
 * action assuming that the parameter will be an object of type LinkedListable.
 * @note In order to apply an action on all items of a list, the user will
 * have to call the Do method for each element of the list.
 * @note the Do method may return an action or an error. An error will terminate further iterations.
 * The function must be stateful and remember the error as it may be still be called afterwards (trees)
 * User must implement mechanism of error reset.
 * An error code Completed shall be used to indicate the need to stop iterating no because of a fault.
 * Any other error code will indicate that the termination is due to some fault.
 */
template < class loadClass>
class GenericIterator{

public:

    /**
     * @brief Destructor.
     */
    virtual ~GenericIterator() {

    }
    /**
     * @brief The function performing the desired specific action on the input parameter.
     * @param[in] data is a generic template type.
     * @return an action or an error. An error will terminate further iterations.
     * The function must be stateful and remember the error as it may be still be called afterwards
     * User must implement mechanism of error reset.
     * An error code Completed shall be used to indicate the need to stop iterating no because of a fault.
     * Any other error code will indicate that the termination is due to some fault.
     */
    virtual IteratorAction Do(loadClass &data,uint32 depth=0)=0;

    /**
     * to be used to insert nodes
     */
    virtual bool GetNodeToInsert(loadClass &data){
    	return false;
    };
};




}

#endif /* GENERICITERATOR_H_ */
