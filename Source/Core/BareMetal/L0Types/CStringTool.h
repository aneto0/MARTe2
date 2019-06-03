/**
 * @file CStringAppendTool.h
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef CSTRINGTOOL_H_
#define CSTRINGTOOL_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "VoidStream.h"
#include "ZeroTerminatedArrayTool.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * allows operating on CStrings, StaticCString and DynamicCString
 */
class DLL_API CStringTool: public ZeroTerminatedArrayTool,public VoidStream{

public:

	/**
	 * @allows constructing the tool
	 */
	inline CStringTool(char8 **dataPointerAddressIn,char8 *dataPointerIn,uint32 sizeOfBufferIn);

	/**
	 * @brief allow using the result of a tool operation, which is the tool itself as the error code produced by the operation
	 */
	inline operator ErrorManagement::ErrorType();

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &AppendHex(uint64 num);

    /**
     * @brief append a char at the end of the string
     */
    inline CStringTool &Append(const char8 c);

    /**
     * @brief append a string at the end of the string
     */
    inline CStringTool &Append(const CCString &s,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const uint64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const uint32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const uint16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const uint8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(int64 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(int32 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(int16 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(int8 num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const double num);

    /**
     * @brief append a number at the end of the string
     */
    inline CStringTool &Append(const float num);

	/**
	 * @brief force string length to a smaller value
	 */
    inline CStringTool &Truncate(uint32 newIndex);

	/**
	 * @brief remove characters from top of string
	 */
    inline CStringTool &Remove(uint32 elements);

    /**
     * @brief how long is the string now?
     */
    inline uint32 GetSize();

    /**
     * @see StreamI
     */
    virtual bool CanWrite() const;


    /**
     * @see StreamI
     */
    virtual bool Write(const char8 * const input, uint32 & size);


    /**
     * @see StreamI
     */
    virtual bool Write(const char8 * const input,
            uint32 & size,
            const MilliSeconds &timeout);

protected:
    /**
     *
     */
	inline void Terminate(uint32 index);

	/**
	 *
	 */
    template <typename T>
    inline CStringTool &AppendT(T positiveNum);

private:
    /**
     * inaccessible
     */
    inline CStringTool &operator=(const CStringTool &in){ return *this; }

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CStringTool::CStringTool(
		char8 **dataPointerAddressIn,
		char8 *dataPointerIn,
		uint32 sizeOfBufferIn
		): ZeroTerminatedArrayTool(reinterpret_cast<uint8**>(dataPointerAddressIn),reinterpret_cast<uint8*>(dataPointerIn),sizeOfBufferIn,sizeof(char8)){}

CStringTool::operator ErrorManagement::ErrorType(){
	return ZeroTerminatedArrayTool::ret;
}

void CStringTool::Terminate(uint32 index){
	ZeroTerminatedArrayStaticTools::ZTAZero(dataPointer+index*elementSize,elementSize);
}

uint32 CStringTool::GetSize(){
	return index;
}

CStringTool &CStringTool::Append(const uint64 num){
	return AppendT(num);
}

CStringTool &CStringTool::Append(const uint32 num){
	return AppendT(num);
}

CStringTool &CStringTool::Append(const uint16 num){
	return AppendT(num);
}

CStringTool &CStringTool::Append(const uint8 num){
	return AppendT(num);
}

CStringTool &CStringTool::Append(int64 num){
	if (num < 0) {
		Append('-');
		num = -num;
	}
	return AppendT(static_cast<uint64>(num));
}

CStringTool &CStringTool::Append(int32 num){
	if (num < 0) {
		Append('-');
		num = -num;
	}
	return AppendT(static_cast<uint32>(num));
}

CStringTool &CStringTool::Append(int16 num){
	if (num < 0) {
		Append('-');
		num = -num;
	}
	return AppendT(static_cast<uint16>(num));
}

CStringTool &CStringTool::Append(int8 num){
	if (num < 0) {
		Append('-');
		num = -num;
	}
	return AppendT(static_cast<uint8>(num));
}

CStringTool &CStringTool::Append(const float num){
	int64 numI = static_cast<int64>(num);
	return AppendT(numI);
}

CStringTool &CStringTool::Append(const double num){
	int64 numI = static_cast<int64>(num);
	return AppendT(numI);
}

CStringTool &CStringTool::Append(const char8 c){
	ZeroTerminatedArrayTool::Append(reinterpret_cast<const uint8 *>(&c),1);
	return *this;
}

template <typename T>
CStringTool &CStringTool::AppendT(T positiveNum){


	T tester = 1u;
	T tested = static_cast<T>(positiveNum/10);

	while (tester <= tested){
		tester *= 10u;
	}
	while (tester > 0u){
		uint8 digit = static_cast<uint8>(positiveNum/tester);
		Append(static_cast<char8>(static_cast<uint8>('0')+digit));
		positiveNum     = static_cast<T>(positiveNum % tester);
		tester /= 10u;
	}
	return *this;
}

CStringTool &CStringTool::AppendHex(uint64 num){
	for (int i = 60;(i>=0) && ret;i-=4){
		uint8 n = (num >> i) & 0xF;
		if (n >= 10) Append(static_cast<char8>(static_cast<uint8>('A')+n-10));
		else         Append(static_cast<char8>(static_cast<uint8>('0')+n));
	}
	return *this;
}

CStringTool &CStringTool::Append(const CCString &s,uint32 maxAppendSize){
	uint32 dataSize = s.GetSize();
	if (dataSize > maxAppendSize){
		dataSize = maxAppendSize;
	}
	ZeroTerminatedArrayTool::Append(reinterpret_cast<const uint8 *>(s.GetList()),dataSize);
	return *this;
}

CStringTool &CStringTool::Truncate(uint32 newIndex){
	ZeroTerminatedArrayTool::Truncate(newIndex);
	return *this;
}

CStringTool &CStringTool::Remove(uint32 elements){
	ZeroTerminatedArrayTool::Remove(elements);
	return *this;
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_CSTRINGAPPENDTOOL_H_ */
