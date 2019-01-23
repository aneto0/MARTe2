/**
 * @file Memory.h
 * @brief Header file for class AnyType
 * @date 29 Aug 2018
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

 * @details This header file contains the declaration of the module Memory
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 * */

#ifndef L1_MARTE_MEMORY_H_
#define L1_MARTE_MEMORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                       Forward declarations                                */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace Memory{

/**
 * @brief Copies a block of memory into another.
 * @details A copy of size bytes from source location to destination position.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the size of the memory to be copied.
 * @return ErrorManagement::ParametersError in case of NULL parameter
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
inline ErrorManagement::ErrorType Copy(void * const destination,const void * const source, uint32 size);

/**
 * @brief Compares the first specified bytes of two blocks of memories.
 * @param[in] mem1 is the pointer to the first memory location.
 * @param[in] mem2 is the pointer to the second memory location.
 * @param[in] size is the number of byte to compare.
 * @return ErrorManagement::ParametersError in case of NULL parameter or
 * ErrorManagement::ComparisonFailure if areas are not the same or
 * no error if are the same
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
inline ErrorManagement::ErrorType Compare(const void * const mem1,const void * const mem2,uint32 size);

/**
 * @brief Compares the first specified bytes of two blocks of memories finding out who has the highest byte first...
 * @param[in] mem1 is the pointer to the first memory location.
 * @param[in] mem2 is the pointer to the second memory location.
 * @param[in] size is the number of byte to compare.
 * @return ErrorManagement::ParametersError in case of NULL parameter or a
 * non-error-code (ErrorType::GetNonErrorCode()) of 0 if mem1 == mem2, 1 if mem1 < mem2, 2 if mem1 > mem2.
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
inline ErrorManagement::ErrorType GetOrder(const void * const mem1,const void * const mem2,uint32 size);

/**
 * @brief Searches a character in the specified memory block.
 * @param[in] mem is the memory location.
 * @param[in] c is the character to find.
 * @param[in] size is the size of the memory area.
 * @return the pointer to the first occurrence of c in the memory. NULL if c is absent.
 * @pre the size parameter must be minor than the memory block size.
 */
inline const void *Search(const void * const mem,const char8 c,const uint32 size);

/**
 * @brief Copies a block of memory into another allowing overlapping.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the number of bytes to be copied.
 * @return ErrorManagement::ParametersError in case of NULL parameter
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
inline ErrorManagement::ErrorType Move(void * const destination,const void * const source,const uint32 size);

/**
 * @brief Sets a defined number bytes of the specified memory area equal to a specified character.
 * @param[in,out] mem is the pointer to the memory location.
 * @param[in] c is the character to store.
 * @param[in] size is the number of bytes where c will be written.
 * @return ErrorManagement::ParametersError in case of NULL parameter
 * @pre the size parameter must be minor than the memory block size.
 */
inline ErrorManagement::ErrorType Set(void * const mem,const char8 c, const uint32 size);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
}


inline ErrorManagement::ErrorType Memory::Copy(void* const destination, const void * const source, const uint32 size) {

	ErrorManagement::ErrorType ret;

	ret.parametersError = ((source == NULL) || (destination == NULL));

	REPORT_ERROR(ret,"Memory::Copy NULL parameters");

	if (ret){
        memcpy(destination, source, static_cast<size_t>(size));
	}
	return ret;
}

inline ErrorManagement::ErrorType Memory::Compare(const void * const mem1,const void * const mem2,const uint32 size) {

	ErrorManagement::ErrorType ret;

	ret.parametersError = ((mem1 == NULL) || (mem2 == NULL));

	REPORT_ERROR(ret,"Memory::Compare NULL parameters");

	if (ret){
        int32 temp = memcmp(mem1, mem2, static_cast<size_t>(size));

        ret.comparisonFailure = (temp != 0);
	}
    return ret;

}

inline ErrorManagement::ErrorType Memory::GetOrder(const void * const mem1,const void * const mem2,const uint32 size) {

	ErrorManagement::ErrorType ret;

	ret.parametersError = ((mem1 == NULL) || (mem2 == NULL));

	REPORT_ERROR(ret,"Memory::Compare NULL parameters");

	if (ret){
        int32 temp = memcmp(mem1, mem2, static_cast<size_t>(size));
        if (temp < 0) {
            ret.SetNonErrorCode(1); // 1 if mem1<mem2
        }
        if (temp > 0) {
        	ret.SetNonErrorCode(2); // 2 if mem1>mem2
        }
        if (temp == 0) {
        	ret.SetNonErrorCode(0); // 0 if mem1==mem2
        }
	}
    return ret;

}

inline const void* Memory::Search(const void * const mem,
                   const char8 c,
                   const uint32 size) {
    const void* ret = static_cast<const void*>(NULL);

    if (mem != NULL) {

        ret = memchr(mem, c, static_cast<size_t>(size));

    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Memory: Invalid input arguments");
    }

    return ret;
}

inline ErrorManagement::ErrorType Memory::Move(void * const destination, const void * const source,const uint32 size) {

	ErrorManagement::ErrorType ret;

	ret.parametersError = ((destination == NULL) || (source == NULL));

	REPORT_ERROR(ret,"Memory::Move NULL parameters");

    if (ret) {
        ret = memmove(destination, source, static_cast<size_t>(size)) != NULL;
    }

    return ret;
}

inline ErrorManagement::ErrorType Memory::Set(void * const mem,const char8 c,const uint32 size) {

	ErrorManagement::ErrorType ret;

	ret.parametersError = (mem == NULL);

	REPORT_ERROR(ret,"Memory::Set NULL parameters");

    if (ret) {

        memset(mem, c, static_cast<size_t>(size));
    }

    return ret;
}


} // MARTe



#endif /* MEMORY_H_ */
