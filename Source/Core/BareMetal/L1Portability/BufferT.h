/**
 * @file BufferT.h
 * @brief Header file for class BufferT
 * @date 05/04/2017
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class BufferT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BUFFERT_H_
#define BUFFERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Buffer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Template implementation of Buffer.
 */
template<typename Type>
class BufferT: protected Buffer {

public:

    /**
     * @brief Default constructor.
     * @param[in] size of the buffer to allocate memory
     */
    BufferT(const uint32 bufferSize);

    /**
     * @brief Destructor. Frees allocated memory.
     */
    virtual ~BufferT();

    /**
     * @brief @see Buffer::Initialise()
     */
    virtual bool Initialise(const Type& data);

    /**
     * @brief @see Buffer::GetSize()
     */
    virtual uint32 GetSize() const;

    /**
     * @brief @see Buffer::GetData()
     */
    virtual bool GetData(Type*& data,
                 const uint32 index) const;

    /**
     * @brief @see Buffer::PutData()
     */
    virtual bool PutData(const Type& data,
                 const uint32 index);

};

template<typename Type>
BufferT<Type>::BufferT(const uint32 bufferSize) : Buffer(bufferSize, static_cast<uint32>(sizeof(Type))) {
}

template<typename Type>
BufferT<Type>::~BufferT() {
}

template<typename Type>
bool BufferT<Type>::Initialise(const Type& data) {
    return (Buffer::Initialise(reinterpret_cast<const char8 *>(&data)));
}

template<typename Type>
uint32 BufferT<Type>::GetSize() const {
    return (Buffer::GetSize());
}

template<typename Type>
bool BufferT<Type>::GetData(Type*& data,
                            const uint32 index) const {
    return (Buffer::GetData(reinterpret_cast<char8 *&>(data), index));
}

template<typename Type>
bool BufferT<Type>::PutData(const Type& data,
                            const uint32 index) {
    return (Buffer::PutData(reinterpret_cast<const char8 *>(&data), index));
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BUFFERT_H_ */

