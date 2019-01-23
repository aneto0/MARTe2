/**
 * @file ZeroTerminatedArrayToolT.h
 * @brief Header file for class AnyType
 * @date 12 Dec 2018
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

 * @details This header file contains the declaration of the class ZeroTerminatedArrayToolT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ZEROTERMINATEDARRAYAPPENDTOOLT_H_
#define ZEROTERMINATEDARRAYAPPENDTOOLT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ZeroTerminatedArray.h"
#include "ZeroTerminatedArrayTool.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 *  forward declaration
 */
template <class T> class ZeroTerminatedArray;

/**
 *
 */
template<typename T>
class ZeroTerminatedArrayToolT: public ZeroTerminatedArrayTool{
public:

	/**
	 *
	 */
	ZeroTerminatedArrayToolT(T **dataPointerAddressIn,T *dataPointerIn,uint32 sizeOfBufferIn);

	/**
	 *
	 */
	ZeroTerminatedArrayToolT<T> &Append(const T &data);

	/**
	 *
	 */
	ZeroTerminatedArrayToolT<T> &AppendN(const ZeroTerminatedArray<const T> & data,uint32 maxAppendSize);

	/**
	 *
	 */
	ZeroTerminatedArrayToolT<T> &AppendN(const ZeroTerminatedArray<T> & data,uint32 maxAppendSize);

	/**
	 *
	 */
	ZeroTerminatedArrayToolT<T> &Truncate(uint32 newIndex);

	/**
	 *
	 */
	ZeroTerminatedArrayToolT<T> &Remove(uint32 elements);

	/**
	 * if assigned to an error type it copies its error info
	 */
	operator ErrorManagement::ErrorType();
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
ZeroTerminatedArrayToolT<T>::ZeroTerminatedArrayToolT(T **dataPointerAddressIn,T *dataPointerIn,uint32 sizeOfBufferIn):
ZeroTerminatedArrayTool(reinterpret_cast<uint8**>(dataPointerAddressIn),reinterpret_cast<uint8*>(dataPointerIn),sizeOfBufferIn,sizeof(T))
{
}

template<typename T>
ZeroTerminatedArrayToolT<T>::operator ErrorManagement::ErrorType(){
	return ret;
}


template<typename T>
ZeroTerminatedArrayToolT<T> &ZeroTerminatedArrayToolT<T>::Append(const T &data){
	ZeroTerminatedArrayTool::Append(reinterpret_cast<const uint8*>(&data),1);
	return *this;
}

template<typename T>
ZeroTerminatedArrayToolT<T> &ZeroTerminatedArrayToolT<T>::AppendN(const ZeroTerminatedArray<const T> & data,uint32 maxAppendSize){
	uint32 dataSize = data.GetSize();
	if (dataSize > maxAppendSize){
		dataSize = maxAppendSize;
	}
	if (data.GetList() != NULL_PTR(T*)){
		ZeroTerminatedArrayTool::Append(reinterpret_cast<const uint8*>(data.GetList()),dataSize);
	}
	return *this;
}

template<typename T>
ZeroTerminatedArrayToolT<T> &ZeroTerminatedArrayToolT<T>::AppendN(const ZeroTerminatedArray<T> & data,uint32 maxAppendSize){
	uint32 dataSize = data.GetSize();
	if (dataSize > maxAppendSize){
		dataSize = maxAppendSize;
	}
	if (data.GetList() != NULL_PTR(const T*)){
		ZeroTerminatedArrayTool::Append(reinterpret_cast<const uint8*>(data.GetList()),dataSize);
	}
	return *this;
}

template<typename T>
ZeroTerminatedArrayToolT<T> &ZeroTerminatedArrayToolT<T>::Truncate(uint32 newIndex){
	ZeroTerminatedArrayTool::Truncate(newIndex);
	return *this;
}

template<typename T>
ZeroTerminatedArrayToolT<T> &ZeroTerminatedArrayToolT<T>::Remove(uint32 elements){
	ZeroTerminatedArrayTool::Remove(elements);
	return *this;
}


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ZEROTERMINATEDARRAYAPPENDTOOLT_H_ */
