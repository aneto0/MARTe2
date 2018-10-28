/**
 * @file CompilerTypes.h
 * @brief Header file for CompilerTypes
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the CompilerTypes
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COMPILERTYPES_H_
#define COMPILERTYPES_H_
/**
 * @brief Quotes an expression after expanding it.
 */
/*lint -save -e9026, function-like macro defined */
#define QUOTE(x) QUOTE_1(x)
/*lint -restore */

/**
 * @brief Quotes an expression without expanding it.
 */
/*lint -save -e9026 -e9024, function-like macro defined, '#/##' operators used in macro */
#define QUOTE_1(x) #x
/*lint -restore */

/**
 * @brief Builds an include filename based on actual architecture.
 */
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#define INCLUDE_FILE_ARCHITECTURE(x,y) QUOTE(Architecture/x/y)
/*lint -restore */


/**
 * @brief Builds an include filename based on actual environment.
 */
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#define INCLUDE_FILE_ENVIRONMENT(x,y) QUOTE(Environment/x/y)
/*lint -restore */

#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,CompilerTypes.h)

/**
 * @brief Casts a 0 value to the target pointer type.
 * @details The aim is to reduce the clutter in the code. This avoids
 * replacing ptr = static<MyObject *>(NULL) with ptr = NULL_PTR(MyObject *)
 */
/*lint -save -e9026, function-like macro defined. The aim is to reduce the clutter in the code
 * This avoids replacing ptr = static<MyObject *>(NULL) with ptr = NULL_PTR(MyObject *)*/
#define NULL_PTR(ptr) static_cast<ptr>(0)
/*lint -restore */

/**
 * @brief Makes NULL a synonym of NULL_PTR(void *).
 */
#ifndef NULL
#define NULL NULL_PTR(void *)
#endif

/***
 * Defined in WinDef.h
 * Conflicts with the methods min() and max() of <limits>
 */
#undef max
#undef min

namespace MARTe {

#if 0 // use uintp
    /** Large enough to store a pointer*/
#ifdef __LP64__
    typedef unsigned long intptr;
#elif defined __ILP64__
    typedef unsigned long intptr;
#elif defined __LLP64__
    typedef unsigned long long intptr;
#else
    typedef unsigned long intptr;
#endif
#endif

/**
 * @brief Defines the symbols' direction in DLL as import
 */
#if !defined (dll_import)
#define dll_import
#endif

/**
 * @brief Defines the symbols' direction in DLL as export
 */
#if !defined (dll_import)
#define dll_export
#endif

/**
* @brief Polymorphic macro for defining the actual direction of the
* symbols when using Windows DLLs (import or export)
*/
#if !defined (DLL_API)
#define DLL_API dll_import
#else
#undef DLL_API
#define DLL_API dll_export
#endif

#if 0
    use "TypeCharacteristics.h"

    /**
     * Maximum value for integers of 64 bits
     */
    static const int64 MAX_INT64 = 0x7FFFFFFFFFFFFFFF;

    /**
     * Maximum value for unsigned integers of 64 bits
     */
    static const uint64 MAX_UINT64 = 0xFFFFFFFFFFFFFFFF;

    /**
     * Minimum value for integers of 64 bits
     */
    static const int64 MIN_INT64 = 0x8000000000000000;

    /**
     * Minimum value for unsigned integers of 64 bits
     */
    static const uint64 MIN_UINT64 = 0;

    /**
     * Maximum value for integers of 32 bits
     */
    static const int32 MAX_INT32 = 0x7FFFFFFF;

    /**
     * Maximum value for unsigned integers of 32 bits
     */
    static const uint32 MAX_UINT32 = 0xFFFFFFFF;

    /**
     * Minimum value for integers of 32 bits
     */
    static const int32 MIN_INT32 = 0x80000000;

    /**
     * Minimum value for unsigned integers of 32 bits
     */
    static const uint32 MIN_UINT32 = 0;

    /**
     * Maximum value for integers of 16 bits
     */
    static const int16 MAX_INT16 = 0x7FFF;

    /**
     * Maximum value for unsigned integers of 16 bits
     */
    static const uint16 MAX_UINT16 = 0xFFFF;

    /**
     * Minimum value for integers of 16 bits
     */
    static const int16 MIN_INT16 = 0x8000;

    /**
     * Minimum value for unsigned integers of 16 bits
     */
    static const uint16 MIN_UINT16 = 0;

    /**
     * Maximum value for integers of 8 bits
     */
    static const int8 MAX_INT8 = 0x7F;

    /**
     * Maximum value for unsigned integers of 8 bits
     */
    static const uint8 MAX_UINT8 = 0xFF;

    /**
     * Minimum value for integers of 8 bits
     */
    static const int8 MIN_INT8 = 0x80;

    /**
     * Minimum value for unsigned integers of 8 bits
     */
    static const uint8 MIN_UINT8 = 0;
    static const uint32 MIN_FLOAT32 = 0xFF7FFFFFu;
    static const uint32 MAX_FLOAT32 = 0x7F7FFFFFu;
    static const uint32 EPSILON_FLOAT32 = 0x34000000u;
    static const uint64 EPSILON_FLOAT64 = 0x3CB0000000000000ull;

    /**
     * float64 limits
     */
//    static const float64 MAX_FLOAT64 = 0x1.FFFFFFFFFFFFFp1023;
#endif

    /** A tool to find indexes of structures fields.
     1024 has been used to avoid alignment problems. */
    #define indexof(type,field) ((MARTe::uintp)&(((type *)1024)->field) - 1024)

    /** A tool to find indexes of structures fields.
     1024 has been used to avoid alignment problems. */
    #define ancestorIndexof(type,ancestor) ((MARTe::uintp)( (ancestor *)((type *)1024)  ) - 1024)

    /** A tool to find the size of structures fields.
     1024 has been used to avoid alignment problems. */
    #define msizeof(type,field) sizeof(((type *)1024)->field)

    /** A tool to find the type of class/structures fields.
     1024 has been used to avoid alignment problems. */
    #define memberOf(type,field) ((type *)1024)->field

    extern const uint8 zCTable[256];
    extern const uint8 pETable[256];

    /**
     * @brief templated function to discover the position of the first 0 bit .
     * @tparam indexType the integer type used by the parameter ix. MUST be unsigned
     * @param ix the number to be analyzed
     * @return the number of consecutive non zero bits, starting from the LSB
     */
    template <typename indexType>
    uint8 firstZeroBit(indexType ix){
    	uint8 result = 0;
    	uint8 fb = pETable[ix & 0xFF];
    	while (fb >= 8){
    		ix= ix >> 8;
    		result += 8;
    		fb = pETable[ix & 0xFF];
    	}
    	return result + fb;
    }

    /**
     * @brief templated function to discover the number of zeroes in a number
     * @tparam indexType the integer type used by the parameter ix. MUST be unsigned
     * @param ix the number to be analyzed
     * @return the number of zeroes in the number. The result depends and is limited by sizeof(indexType)
     */
    template <typename indexType>
    uint8 nOfZeroes(indexType ix){
    	uint8 result = 0;
    	int i = 0;
    	for (i = 0 ; i < sizeof(indexType); i++){
    		result += zCTable[ix & 0xFF];
    		ix = ix >> 8;
    	}
    	return result ;
    }

    /**
     * @brief namespace for definitions that are not meant for the user to know or use
     */
    namespace Private{
    	typedef char (&yes)[1];
    	typedef char (&no)[2];

        /**
         * if D derives from Bit contains value constant true
        */
    	template <typename B, typename D>
    	struct Host
		{
    		operator B*() const;
    		operator D*();
		};

    	template <typename B, typename D>
    	struct is_base_of
		{
    		template <typename T>
    		static yes check(D*, T);
    		static no check(B*, int);

    		static const bool value = sizeof(check(Host<B,D>(), int())) == sizeof(yes);

		};


        /**
         * if T and U are the same it contains value constant true
        */
    	template<typename T, typename U>
    	struct is_same
    	{
    	    static const bool value = false;
    	};

    	template<typename T>
    	struct is_same<T,T>  //specialization
    	{
    	   static const bool value = true;
    	};
    }

    /**
     *	@return true if derived is derived from base.
     */
    #define isBaseOf(base,derived)  Private::is_base_of<base,derived>::value

    /**
     *	@return true if T1 and T2 are the same class.
     */
    #define isSame(T1,T2)  Private::is_same<T1,T2>::value

    /**
     *	@return true if derived is derived from base or is base
     */
    #define isSameOrBaseOf(base,derived)  (Private::is_base_of<base,derived>::value || Private::is_same<base,derived>::value)

    /**
     *	@brief A class that inherits from this will inherit from T only if bool is True.
     */
    template <bool, typename T = void>
    struct enable_if
    {};

    template <typename T>
    struct enable_if<true, T> {
      typedef T type;
    };

    /**
     * @brief return the maximum between two values
     */
    template <typename T>
    inline T max(const T &v1, const T &v2){
    	T ret = v1;
    	if (ret < v2) {
    		ret = v2;
    	}
    	return ret;
    }

    /**
     * @brief return the minimum between two values
     */
    template <typename T>
    inline T min(const T &v1, const T &v2){
    	T ret = v1;
    	if (ret > v2) {
    		ret = v2;
    	}
    	return ret;
    }

}

#endif /* COMPILERTYPES */

