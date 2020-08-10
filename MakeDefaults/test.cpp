template<typename T1,typename T2> T1 UMulT (T1 x1,T1 x2,T1 &high){
	const unsigned short nBits = sizeof(T1)*8;
	const T2 mask = static_cast<T2>(~((T1)0));
	T2 ret = (T2)x1 * (T2)x2;
	high = (T1)(ret >> nBits);
	return (T1)(ret & mask);
}

typedef __int128 int128;
typedef unsigned __int128 uint128;
typedef long long int64;
typedef unsigned long long uint64;
typedef int int32;
typedef unsigned int uint32;

/** 16 Bit unsigned integer. */
typedef unsigned short uint16;
/** 16 Bit signed integer. */
typedef signed short int16;
/** 8 Bit unsigned integer. */
typedef unsigned char uint8;
/** 8 Bit signed integer. */
typedef signed char int8;


inline uint8  Mul(uint8  x1,uint8  x2,uint8  &high){	return UMulT<uint8,uint16>(x1,x2,high);    }
inline uint16 Mul(uint16 x1,uint16 x2,uint16 &high){	return UMulT<uint16,uint32>(x1,x2,high);   }
inline uint32 Mul(uint32 x1,uint32 x2,uint32 &high){	return UMulT<uint32,uint64>(x1,x2,high);   }
inline uint64 Mul(uint64 x1,uint64 x2,uint64 &high){   return UMulT<uint64,uint128>(x1,x2,high);  }

inline int8  Mul(int8  x1,int8  x2,int8  &high){	return UMulT<int8,int16>(x1,x2,high);    }
inline int16 Mul(int16 x1,int16 x2,int16 &high){	return UMulT<int16,int32>(x1,x2,high);   }
inline int32 Mul(int32 x1,int32 x2,int32 &high){	return UMulT<int32,int64>(x1,x2,high);   }
inline int64 Mul(int64 x1,int64 x2,int64 &high){    return UMulT<int64,int128>(x1,x2,high);   }
