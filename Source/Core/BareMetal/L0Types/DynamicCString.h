/**
 * @file DynamicCString.h
 * @brief Header file for class DynamicCString
 * @date Dec 16, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class DynamicCString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_DYNAMICCSTRING_H_
#define L0TYPES_DYNAMICCSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "DynamicZeroTerminatedArray.h"
#include "CCString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
* @brief Wrapper for writable char buffers
* */
class DynamicCString: public DynamicZeroTerminatedArray<char8,16>{
public:
    /**
     * @brief constructor
     */
    inline DynamicCString ();

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (DynamicCString const &s);

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (char8 const * const &s);

    /**
     * @brief returns a CCString containing the same pointer.
     * Note that after any operation affecting the size of this
     * string the pointer may change
     */
    inline operator CCString() const;

    /**
     * @Brief compare content
     */
    inline bool operator==(const CCString &s) const;

    /**
     * @Brief copies one string to another.
     * Necessary to avoid default copy operator use!
     */
    inline void  operator=(const DynamicCString &s) ;


    /**
     * @brief append a number at the end of the string
     */
    inline bool AppendHex(uint64 num);

    /**
     * @brief append a char at the end of the string
     */
    inline bool Append(const char8 c);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const char8 *s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const CCString &s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a string at the end of the string
     */
    inline bool Append(const DynamicCString &s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const uint8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const int8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const double num);
    /**
     * @brief append a number at the end of the string
     */
    inline bool Append(const float num);

private:
    bool AppendN(const ZeroTerminatedArray<const char8> & data,uint32 maxAppendSize){ return false;}

    template <typename T>
    inline bool AppendT(T num);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

DynamicCString::DynamicCString (){
}

DynamicCString::DynamicCString(DynamicCString const &s):DynamicZeroTerminatedArray<char8,16>(s){
}

DynamicCString::DynamicCString (char8 const * const &s):DynamicZeroTerminatedArray<char8,16>(CCString(s)){
}

DynamicCString::operator CCString() const{
    return CCString(DynamicZeroTerminatedArray<char8,16>::GetList());
}

bool DynamicCString::operator==(const CCString &s) const{
	return DynamicZeroTerminatedArray<char8,16>::isSameAs(s.GetList());
}

void DynamicCString::operator=(const DynamicCString &s) {
	Truncate(0U);
	DynamicZeroTerminatedArray<char8,16>::AppendN(s);
}

bool DynamicCString::Append(const uint64 num){
	return AppendT(num);
}

bool DynamicCString::Append(const uint32 num){
	return AppendT(num);
}

bool DynamicCString::Append(const uint16 num){
	return AppendT(num);
}

bool DynamicCString::Append(const uint8 num){
	return AppendT(num);
}

bool DynamicCString::Append(const int64 num){
	return AppendT(num);
}

bool DynamicCString::Append(const int32 num){
	return AppendT(num);
}

bool DynamicCString::Append(const int16 num){
	return AppendT(num);
}

bool DynamicCString::Append(const int8 num){
	return AppendT(num);
}

bool DynamicCString::Append(const float num){
	int64 numI = num;
	return AppendT(numI);
}

bool DynamicCString::Append(const double num){
	int64 numI = num;
	return AppendT(numI);
}

template <typename T>
bool DynamicCString::AppendT(T num){
	bool ret = true;
	if (num < 0){
		ret = ret && Append('-');
		num = -num;
	}

	T tester = 1;
	T tested = num/10;
	while (tester <= tested){
		tester *= 10;
	}
	while (tester > 0){
		uint8 digit = num/tester;
		ret = ret && Append(static_cast<char8>(static_cast<uint8>('0')+digit));
		num     = num%tester;
		tester /= 10;
	}
	return ret;
}

#if 0
bool DynamicCString::Append(uint64 num,int32 fill0){
	bool ret = true;
	if (num > 9){
		uint64 numH = num/10u;
		num = num - numH * 10u;
		ret = Append(numH);
	}
	ret = ret && Append (static_cast<char8>(static_cast<uint8>('0')+num));
	return ret;
}
#endif

bool DynamicCString::AppendHex(uint64 num){
	bool ret = true;
	for (int i = 60;(i>=0) && ret;i-=4){
		uint8 n = (num >> i) & 0xF;
		if (n >= 10) ret = ret && Append(static_cast<char8>(static_cast<uint8>('A')+n-10));
		else         ret = ret && Append(static_cast<char8>(static_cast<uint8>('0')+n));
	}
	return ret;
}

bool DynamicCString::Append(const char8 c){
	return DynamicZeroTerminatedArray<char8,16>::Append(c);
}

bool DynamicCString::Append(const char8 *s,uint32 maxAppendSize){
	return DynamicZeroTerminatedArray<char8,16>::AppendN(s,maxAppendSize);
}
bool DynamicCString::Append(const DynamicCString &s,uint32 maxAppendSize){
	return DynamicZeroTerminatedArray<char8,16>::AppendN(s.GetList(),maxAppendSize);
}

bool DynamicCString::Append(const CCString &s,uint32 maxAppendSize){
	return DynamicZeroTerminatedArray<char8,16>::AppendN(s.GetList(),maxAppendSize);
}

}
#endif /* L0TYPES_DYNAMICCSTRING_H_ */
	
