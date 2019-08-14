/*
 * @file SimpleStaticListT.h
 * @brief Header file for class SimpleStaticListT
 * @date 03/05/2017
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

 * @details This header file contains the declaration of the class StaticList
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SIMPLESTATICLISTT_H_
#define SIMPLESTATICLISTT_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief This template class is a ADT (Abstract Data Type) for fixed size
 * lists which allows managed progressive usage by adding at the end .
 *
 * @details This class implements fixed size array with an index pointing at the next free object
 *
 * @tparam allocationSize is the size of this array as type T
 * @tparam T is any object. Note that T must have a zero parameters constructor and have a working copy operator
 *
 */
template <class T,uint32 allocationSize>
class SimpleStaticListT{
public:
	/**
	 *@brief Initialises the list
	 */
	SimpleStaticListT();

	/**
	 *@brief access elements by const reference
	 *@details acess by index is not protected
	 *@param index needs to be limited by user to within [0-listSize)
	 */
	inline const T &Get(uint32 index) const;

	/**
	 *@brief access elements by reference
	 *@details acess by index is not protected
	 *@param index needs to be limited by user to within [0-listSize)
	 */
	inline T &operator[](uint32 index) ;

	/**
	 *@brief adds one element by copying data to next free slot
	 *@param data will be copied (operator=) to the next free slot
	 *@return false if no space left
	 */
	bool Add(const T &data );

	/**
	 *@return how many used slots
	 */
	inline uint32 NumberOfUsedElements() const;

	/**
	 *@brief how much space left unused
	 */
	inline uint32 NumberOfFreeElements() const ;

	/**
	 *@brief how much space was allocated
	 */
	inline uint32 NumberOfElements() const;

private:
	/**
	 * the number of used elements
	 */
	uint32 listSize;
	/**
	 * the array
	 */
	T array[allocationSize];

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <class T,uint32 allocationSize>
SimpleStaticListT<T,allocationSize>::SimpleStaticListT(){
	listSize = 0;
}

template <class T,uint32 allocationSize>
bool SimpleStaticListT<T,allocationSize>::Add(const T &data )
{
	bool ret = (listSize < allocationSize);
	if (ret){
		array[listSize++] = data;
	}
	return ret;
}

template <class T,uint32 allocationSize>
uint32 SimpleStaticListT<T,allocationSize>::NumberOfFreeElements() const{
	return allocationSize - listSize;
}

template <class T,uint32 allocationSize>
T &SimpleStaticListT<T,allocationSize>::operator[](uint32 index) {
	return array[index];
}

template <class T,uint32 allocationSize>
const T &SimpleStaticListT<T,allocationSize>::Get(uint32 index) const{
	return array[index];
}

template <class T,uint32 allocationSize>
uint32 SimpleStaticListT<T,allocationSize>::NumberOfUsedElements() const{
	return listSize;
}

template <class T,uint32 allocationSize>
uint32 SimpleStaticListT<T,allocationSize>::NumberOfElements() const{
	return allocationSize;
}


}

#endif /* SIMPLESTATICLISTT_H_ */
