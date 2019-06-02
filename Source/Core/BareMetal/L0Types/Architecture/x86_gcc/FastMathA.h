/**
 * @file FastMathA.h
 * @brief Header file for module FastMathA
 * @date 26/06/2015
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

 * @details This header file contains the declaration of the module FastMathA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATHA_H_
#define FASTMATHA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace FastMath {

typedef unsigned char uint8 ;
typedef unsigned short uint16 ;
typedef unsigned long uint32 ;
typedef unsigned long long uint64 ;
typedef char int8 ;
typedef short int16 ;
typedef long int32 ;
typedef long long int64 ;
typedef float float32 ;

/** 128 Bit unsigned integer. */
typedef unsigned __int128 uint128;
/** 128 Bit signed integer. */
typedef __int128 int128;

inline float Sin(const float angle) {
    volatile float output;
    __asm__ __volatile__(
            "fsin"
            : "=t" (output) : "0" (angle)
    );
    return output;
}

inline float Cos(const float angle) {
    volatile float output;
    __asm__ __volatile__(
            "fcos;"
            : "=t" (output) : "0" (angle)
    );
    return output;
}

inline int32 FloatToInt(const float input) {
    volatile int32 output;
    __asm__ __volatile__(
            "fld   %1;\n"
            "fistpl %0;"
            : "=m" (output) : "m" (input)
    );
    return output;
}

template<typename T1,typename T2> T1 UMulT (T1 x1,T1 x2,T1 &high){
	const uint8 nBits = sizeof(T1)*8;
	const T2 mask = static_cast<T2>(~((T1)0));
	T2 ret = (T2)x1 * (T2)x2;
	high = (T1)(ret >> nBits);
	return (T1)(ret & mask);
}


inline uint8  Mul(uint8  x1,uint8  x2,uint8  &high){	return UMulT<uint8,uint16>(x1,x2,high);    }
inline uint16 Mul(uint16 x1,uint16 x2,uint16 &high){	return UMulT<uint16,uint32>(x1,x2,high);   }
inline uint32 Mul(uint32 x1,uint32 x2,uint32 &high){	return UMulT<uint32,uint64>(x1,x2,high);   }
inline uint64 Mul(uint64 x1,uint64 x2,uint64 &high){   return UMulT<uint64,uint128>(x1,x2,high);  }

inline int8  Mul(int8  x1,int8  x2,int8  &high){	return UMulT<int8,int16>(x1,x2,high);    }
inline int16 Mul(int16 x1,int16 x2,int16 &high){	return UMulT<int16,int32>(x1,x2,high);   }
inline int32 Mul(int32 x1,int32 x2,int32 &high){	return UMulT<int32,int64>(x1,x2,high);   }
inline int64 Mul(int64 x1,int64 x2,int64 &high){    return UMulT<int64,int128>(x1,x2,high);   }



}

}
#endif /* FASTMATHA_H_ */
