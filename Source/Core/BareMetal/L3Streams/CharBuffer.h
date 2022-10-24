/**
 * @file AnyType.h
 * @brief Header file for class AnyType
 * @date 01/10/2015
 * @author Filippo Sartori
 * @author Andre' Neto
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

#ifndef CHAR_BUFFER_H
#define CHAR_BUFFER_H

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BitBoolean.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Char buffer wrapper.
 *
 * @details This class implements a generic character buffer. It wraps a char8 * buffer
 * which can either be dynamically allocated by this class on the heap, or can be linked
 * to an existing memory reference.
 *
 * @invariant 1) CanWrite() => Buffer() == BufferReference()
 * @invariant 2) not CanWrite() => Buffer() != BufferReference() && BufferReference() == NULL
 * @invariant 3) (Buffer() != NULL && Size() > 0) || (Buffer() == NULL && Size() == 0)
 */
class DLL_API CharBuffer {

public:

    /** 
     * @brief Default constructor.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   Size() == 0 &&
     *   AllocationGranularity() == 1u &&
     *   not CanWrite() &&
     *   not IsAllocated()
     */
    CharBuffer();

    /**
     * @brief Constructor.
     * @details If the input allocationGranularity is not a power of two, the granularity will be the immediate power of two greater than
     * the input (i.e input=10 ==> granularity=16).
     * @param[in] allocationGranularity specifies the minimum size which can be allocated.
     * The memory allocated will be always a multiple of this number.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   Size() == 0 &&
     *   AllocationGranularity() == "allocationGranularity rounded at the first minor 2^x value" &&
     *   not CanWrite() &&
     *   not IsAllocated()
     */
    CharBuffer(const uint32 allocationGranularity);

    /** 
     * @brief Destructor.
     */
    ~CharBuffer();

    /**
     * @brief Allocates or reallocate memory to the desired size on the heap.
     * @details Allocates \a desiredSize bytes in the heap. If Buffer() != NULL the
     * buffer is resized (Realloc) for \a desiredSize bytes.
     * @param[in] desiredSize the desired size of the buffer.
     * @return false if precondition fails or if the (re)allocation of the desiredSize in the heap fails.
     * @pre true
     * @post
     *    Buffer() != NULL &&
     *    Size() == (((desiredSize + AllocationGranularity()) - 1u) & ~(AllocationGranularity() - 1u)) &&
     *    CanWrite()
     */
    bool SetBufferSize(const uint32 desiredSize);

    /**
     * @brief Memory assignment of a preallocated buffer in read and write mode.
     * @param[in] buff a pointer to the writable buffer.
     * @param[in] buffSize the size of the buffer.
     * @pre
     *    buff != NULL && buffSize > 0
     * @post
     *    Buffer() == buff &&
     *    Size() == buffSize &&
     *    CanWrite()
     * @warning buff is an input parameter, because this method only sets the
     * pointer, but once assigned the pointer could be accessed for read and
     * write.
     */
    void SetBufferReference(char8 * const buff,
                            const uint32 buffSize);

    /**
     * @brief Memory assignment of a preallocated buffer in read-only mode.
     * @param[in] buff is a pointer to the buffer without write access.
     * @param[in] buffSize is the size of the buffer.
     * @pre
     *    buff != NULL && buffSize > 0
     * @post
     *    Buffer() == buff &&
     *    Size() == buffSize &&
     *    not CanWrite()
     */
    void SetBufferReference(const char8 * const buff,
                            const uint32 buffSize);

    /** 
     * @brief Gets a read-only pointer to the internal buffer.
     * @return a pointer to the buffer.
     */
    inline const char8 *Buffer() const;

    /**
     * @brief Gets a read/Write pointer to the internal buffer
     * @return a pointer to the buffer.
     */
    inline char8 *BufferReference();

    /**
     * @brief Gets the size of the memory associated to the buffer.
     * @return the size of the memory associated to the buffer.
     */
    inline uint32 Size() const;

    /**
     * @brief Queries if the buffer is writable.
     * @return true if the buffer is writable.
     */
    bool CanWrite() const;

    /**
     * @brief Queries if the current buffer is allocated
     */
    bool IsAllocated() const;

    /**
     * @brief Gets the allocation granularity
     */
    uint32 AllocationGranularity() const;

private:

    /**
     * A mask of bits which represents the allocation granularity
     * as a power of two.
     */
    uint32 allocationGranularityMask;

    /**
     * true if memory was allocated by this class
     * false if this class only holds a reference to a an existing buffer
     */
    bool allocated;

    /**
     * true if it is read-only memory
     */
    bool readOnly;

    /**
     * The size of the allocated memory block
     */
    uint32 bufferSize;

    /**
     * The wrapper char8 * buffer
     */
    /*lint -sem(MARTe::CharBuffer::Reset,cleanup)*/
    char8 *buffer;

    /**
     * @brief Resets the buffer, releasing any memory previously allocated in
     * the heap if it was allocated by the class itself.
     * @pre true
     * @post
     *   Buffer() == NULL &&
     *   Size() == 0 &&
     *   not CanWrite() &&
     *   not IsAllocated()
     */
    void Reset();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline const char8 *CharBuffer::Buffer() const {
    return buffer;
}
inline char8 *CharBuffer::BufferReference() {
    /*if (readOnly) {
       REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "The buffer is Read Only");
    }*/
    return buffer;
}

inline uint32 CharBuffer::Size() const {
    return bufferSize;
}

inline bool CharBuffer::CanWrite() const {
    return !readOnly;
}

inline bool CharBuffer::IsAllocated() const {
    return allocated;
}

inline uint32 CharBuffer::AllocationGranularity() const {
    return ((~allocationGranularityMask) + 1u);
}

}

#endif /* CHAR_BUFFER_H */
