/**
 * @file SaturatedInteger.h
 * @brief Header file for class AnyType
 * @date 4 Mar 2018
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

#ifndef SATURATEDINTEGER_H_
#define SATURATEDINTEGER_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeCharacteristics.h"
#include "ErrorType.h"
#include "FastMath.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * Allows operations between integers with saturation check
 * @64 bit small saturations are not detected as it uses double operation to check for range
 */
template <typename baseType>
class SaturatedInteger{
public:
	/**
	 * @brief copy constructor
	 * @param special if 'I' the number is loaded with infinite, if 'i' with -infinite, with '\0' with zero, otherwise undeterminate
	 */
	inline SaturatedInteger(char8 special='\0');

	/**
	 * @brief copy constructor
	 */
	inline SaturatedInteger(const SaturatedInteger<baseType> & x);

	/**
	 * @brief conversion constructor
	 */
	template <typename inputType>
	inline SaturatedInteger(const SaturatedInteger<inputType> & x);

	/**
	 * @brief conversion constructor
	 */
	template <typename inputType>
	inline SaturatedInteger(const inputType & in);

	/**
	 * a valid and meaningful number
	 */
	inline bool IsValid() const;

	/**
	 *  means that it has overflown
	 */
	inline bool IsInfinite() const;

	/**
	 * means that it contains no information.
	 * operations on overflown numbers yield indeterminate
	 */
	inline bool IsIndeterminate() const;

	/**
	 * @brief true if the number is larger than maxint
	 */
	inline bool IsPositiveInf() const;

	/**
	 * @brief true if the number is smaller than -maxint
	 */
	inline bool IsNegativeInf() const;

	/**
	 * @brief compare operator
	 */
	inline bool operator== (const SaturatedInteger<baseType> &x) const;

	/**
	 * @brief compare operator
	 */
	inline bool operator> (const SaturatedInteger<baseType> &x) const;

	/**
	 * @brief compare operator
	 */
	inline bool operator< (const SaturatedInteger<baseType> &x) const;

	/**
	 * @brief different operator
	 */
	inline bool operator!= (const SaturatedInteger<baseType> &x) const;

	/**
     * @brief Copy operator.
     * @param[in] x is the value to copy in this.
     * @return this object.
     */
	inline SaturatedInteger<baseType> &operator=(const SaturatedInteger<baseType> &x);

    /**
     * @brief Subtracts the specified value to the current value.
     * @param[in] x is the value which will be subtracted to the value
     * @return this object.
     */
	inline SaturatedInteger<baseType> &operator-=(const SaturatedInteger<baseType> &x);

    /**
     * @brief adds the specified value to the current value.
     * @param[in] x is the value which will be addes to the value
     * @return this object.
     */
	inline SaturatedInteger<baseType> &operator+=(const SaturatedInteger<baseType> &x);

    /**
     * @brief multiplies the specified value to the current value.
     * @param[in] x is the value which will be multiplied to the value
     * @return this object.
     */
	inline SaturatedInteger<baseType> &operator*=(const SaturatedInteger<baseType> &x);

    /**
     * @brief multiplies the specified value to the current value.
     * @param[in] x is the value which will be multiplied to the value
     * @return this object.
     */
	inline SaturatedInteger<baseType> &Scale(const double &x);

	/**
	 * @brief saturation safe product operator
	 */
	template <typename mType>
	inline SaturatedInteger<baseType> operator*(const mType &x) const;

	/**
	 * @brief saturation safe product operator
	 */
	inline SaturatedInteger<baseType> operator+(const SaturatedInteger<baseType> &x) const;

	/**
	 * @brief saturation safe product operator
	 */
	inline SaturatedInteger<baseType> operator-(const SaturatedInteger<baseType> &x) const;

	/**
	 * @brief allows copying the saturated value. Returns error if the value is +/-inf or it does not fit in the T2 type
	 */
	template <typename outputType>
	inline ErrorManagement::ErrorType ToNumber(outputType& output) const;
#if 0
	template <typename outputType>
	inline operator outputType() const;
#endif

	/**
	 * @brief allows access to the internal value
	 */
	inline baseType GetData() const;

	/**
	 * Returns 0 for a valid number or infiniteCodeOffset,negativeCodeOffset
	 */
	inline uint8 GetCode() const;

	/**
	 * code is 0-3 value
	 */
	inline void SetCode(uint8 code) ;

private:

	/**
	 *  the actual data
	 */
	baseType data;

	/**
	 *
	 */
	static const baseType spareCodes;
	static const uint8 positiveInf        = 3U;
	static const uint8 negativeInf 		  = 2U;
	static const uint8 indeterminate 	  = 1U;
	static const uint8 valid    		  = 0U;

	static const uint8 TTSubtractionCodeMap[4][4];
	static const uint8 TTAdditionCodeMap[4][4];
	static const bool  TTCompareCodeMap[4][4];
	static const baseType maxRawValue;
};

#if 0 // removed as causes recursion
/**
 * defines the TypeCharacteristics for the SaturatedInteger<baseType> so that one can use the TypeCharacteristics functions
 */
template <typename baseType>
class TypeCharacteristics<SaturatedInteger<baseType>>: public TypeCharacteristics<baseType>{
public:

	/**
	 * @brief Returns the maximum possible value of the template integer/float type.
	 * @tparam T An integer/float type
	 * @return 0xffff...f if the type is unsigned, 0x7fff...f if it is signed.
	 */
	static const baseType MaxValue() {
		return SaturatedInteger<baseType>::maxRawValue-3;
	}
};

#endif

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


//template<typename baseType>
//const baseType SaturatedInteger<baseType>::maxRawValue = TypeCharacteristics<baseType>::MaxValue() - SaturatedInteger<baseType>::spareCodes;

template<typename baseType>
const baseType SaturatedInteger<baseType>::spareCodes = 3;

template<typename baseType>
const baseType SaturatedInteger<baseType>::maxRawValue = static_cast<baseType>(TypeCharacteristics<baseType>::MaxValue() - 3);

/*
 *  Maps Code combination to code for subtractions
 */
template<typename baseType>
const uint8 SaturatedInteger<baseType>::TTSubtractionCodeMap[4][4] = {
		{valid,    	    indeterminate, indeterminate, negativeInf},   // valid
		{indeterminate, indeterminate, indeterminate, indeterminate}, // indeterminate
		{negativeInf,   indeterminate, indeterminate, negativeInf},   // negative inf
		{positiveInf,   indeterminate, positiveInf,   indeterminate}  // infinite
};

/*
 *  Maps Code combination to code for addition
 */
template<typename baseType>
const uint8 SaturatedInteger<baseType>::TTAdditionCodeMap[4][4] = {
		{valid,    	    indeterminate, indeterminate, positiveInf},   // valid
		{indeterminate, indeterminate, indeterminate, indeterminate}, // indeterminate
		{negativeInf,   indeterminate, negativeInf,   indeterminate}, // negative inf
		{positiveInf,   indeterminate, positiveInf,   positiveInf}    // infinite
};

template<typename baseType>
const bool SaturatedInteger<baseType>::TTCompareCodeMap[4][4] = {
		{true,    	    true, 		   true,          false        },   // valid
		{false,         true,          true,          false        }, // indeterminate
		{false,         false,         true,          false        }, // negative inf
		{true,   		true,		   true,		  true         }    // infinite
};


template<typename baseType>
uint8 SaturatedInteger<baseType>::GetCode() const {
	static const baseType maxRawValue = static_cast<baseType>(TypeCharacteristics<baseType>::MaxValue() - 3);
	uint8 ret = valid;
	if (data > maxRawValue){
		// valid because there are only a few possible values above maxRawValue
		ret = static_cast<uint8>(data - maxRawValue);
	}
	return ret;
}

template<typename baseType>
void SaturatedInteger<baseType>::SetCode(uint8 code)  {
	static const baseType maxRawValue = static_cast<baseType>(TypeCharacteristics<baseType>::MaxValue() - 3);

	data = maxRawValue;
	data += (baseType)code;
}

//SaturatedInteger<baseType>::SaturatedInteger(char8 special='0'){

template <typename baseType>
SaturatedInteger<baseType>::SaturatedInteger(char8 special){
	if (special == '\0'){
		data = 0;
	} else
	if (special == 'I'){
		SetCode(positiveInf);
	} else
	if (special == 'i'){
		SetCode(negativeInf);
	} else {
		SetCode(indeterminate);
	}
}

template <typename baseType>
SaturatedInteger<baseType>::SaturatedInteger(const SaturatedInteger<baseType> & x){
	data = x.data;
}

template <typename baseType>
template <typename inputType>
SaturatedInteger<baseType>::SaturatedInteger(const SaturatedInteger<inputType> & x){
	if (!x.IsValid()){
		SetCode(x.GetCode());
	} else {
		*this = SaturatedInteger(x.GetData());
	}
}

template <typename baseType>
template <typename inputType>
SaturatedInteger<baseType>::SaturatedInteger(const inputType & x){

	// uses the TypeCharacteristics<SaturatedInteger<baseType>> so accounts for the limited range
	bool saturated = !SafeNumber2Number(x,data);

	if (saturated){
		// detect lower saturation
		if (data == TypeCharacteristics<baseType>::MinValue()){
			SetCode(negativeInf);
		} else {
			SetCode(positiveInf);
		}
	} else {
		if (data > maxRawValue){
			SetCode(positiveInf);
		}
	}
}

template<typename baseType>
bool SaturatedInteger<baseType>::IsValid() const{
	return (GetCode() == valid) ;
}

template<typename baseType>
bool SaturatedInteger<baseType>::IsIndeterminate() const{
	return (GetCode() == indeterminate);
}

template<typename baseType>
bool SaturatedInteger<baseType>::IsInfinite() const{
	return (GetCode() == positiveInf) || (GetCode() == negativeInf);
}

template <typename baseType>
bool SaturatedInteger<baseType>::IsPositiveInf() const{
	return (GetCode() == positiveInf);
}

template <typename baseType>
bool SaturatedInteger<baseType>::IsNegativeInf() const{
	return (GetCode() == negativeInf);
}

template <typename baseType>
bool SaturatedInteger<baseType>::operator== (const SaturatedInteger<baseType> &x) const{
	return (data == x.data);
}

template <typename baseType>
SaturatedInteger<baseType> &SaturatedInteger<baseType>::operator=(const SaturatedInteger<baseType> &x){
	data = x.data;
	return *this;
}

template <typename baseType>
bool SaturatedInteger<baseType>::operator> (const SaturatedInteger<baseType> &x) const{
	bool ret;
	if (!IsValid() || !x.IsValid()){
		ret = TTCompareCodeMap[GetCode()][x.GetCode()];
	} else {
		ret = (data > x.data);
	}
	return ret;
}

template <typename baseType>
bool SaturatedInteger<baseType>::operator< (const SaturatedInteger<baseType> &x) const{
	bool ret;
	if (!IsValid() || !x.IsValid()){
		ret = TTCompareCodeMap[x.GetCode()][GetCode()];
	} else {
		ret = (data < x.data);
	}
	return ret;
}

template <typename baseType>
bool SaturatedInteger<baseType>::operator!= (const SaturatedInteger<baseType> &x) const{
	return (data != x.data);
}

template<typename baseType>
SaturatedInteger<baseType> &SaturatedInteger<baseType>::operator+=(const SaturatedInteger<baseType> & x){
	if (!IsValid() || !x.IsValid()){
		SetCode(TTAdditionCodeMap[GetCode()][x.GetCode()]);
	} else {
		baseType temp = static_cast<baseType>(data + x.data);
		// check saturation
		if (TypeCharacteristics<baseType>::IsSigned()){
			// for signed numbers overflows happens on the sign bit:
			// this means only happens if signs were the same and if result has flipped sign
			const baseType signBit = static_cast<baseType>(static_cast<baseType>(1u) << (sizeof(baseType)*8u-1u));
			if ( (((data ^ x.data) & signBit) == 0u) && (((data ^ temp) & signBit) != 0u)){
				if ((data & signBit) == 0u){
					temp = maxRawValue;
					temp += (baseType)positiveInf;
				} else {
					temp = maxRawValue;
					temp += (baseType)negativeInf;
				}
			}
			// for unsigned the result is smaller than one of them
		} else {
			if ((temp < data ) && (temp < x.data)){
				temp = maxRawValue;
				temp += (baseType)positiveInf;
			}
		}
		data = temp;

	}
	return *this;
}

template<typename baseType>
SaturatedInteger<baseType> &SaturatedInteger<baseType>::operator-=(const SaturatedInteger<baseType> & x){
	if (!IsValid() || !x.IsValid()){
		SetCode(TTSubtractionCodeMap[GetCode()][x.GetCode()]);
	} else {
		baseType temp = static_cast<baseType>(data - x.data);
		// check saturation
		if (TypeCharacteristics<baseType>::IsSigned()){
			// for signed numbers overflows happens on the sign bit:
			// this means only happens if signs were different and if result has flipped sign compared to first operand
			const baseType signBit = (static_cast<baseType>(1u) << (sizeof(baseType)*8u-1u));
			if ( (((data ^ x.data) & signBit) != 0u) && (((data ^ temp) & signBit) != 0u)){
				if ((data & signBit) == 0u){
					SetCode(positiveInf);
				} else {
					SetCode(negativeInf);
				}
			} else {
				data = temp;
			}
			// for unsigned the result is smaller than one of them
		} else {
			if (x.data > data){
				SetCode(negativeInf);
			} else {
				data = temp;
			}
		}
	}
	return *this;
}

template<typename baseType>
SaturatedInteger<baseType> &SaturatedInteger<baseType>::operator*=(const SaturatedInteger<baseType> & x){
	if (!IsValid() || !x.IsValid()){
		SetCode(indeterminate);
	} else {
		baseType overflow;
		baseType result = FastMath::Mul(data,x.data,overflow);

		//check overflow for signed numbers
		if (TypeCharacteristics<baseType>::IsSigned()){
			if (overflow >= 0){
				if ((overflow == 0) && (result > 0)){
					data = result;
				} else {
					SetCode(positiveInf);
				}
			} else {
				baseType overFlowBI = ~overflow;
				if ((overFlowBI == 0) && (result < 0)){
					data = result;
				} else {
					SetCode(negativeInf);
				}
			}
		} else {
			//check overflow for unsigned numbers
			if (overflow == 0){
				data = result;
			} else {
				SetCode(positiveInf);
			}
		}
	}
	return *this;
}

template<typename baseType>
SaturatedInteger<baseType> &SaturatedInteger<baseType>::Scale(const double &x){
	if (IsValid()){
		double result = data * x;
		*this = SaturatedInteger<baseType>(result);
	} else {
		if (IsNegativeInf() && (data < 0)){
			this->SetCode(positiveInf);
		}
	}
	return *this;
}


template <typename baseType>
template <typename mType>
SaturatedInteger<baseType> SaturatedInteger<baseType>::operator*(const mType &x) const{
	SaturatedInteger<baseType> ret(*this);
	ret *= x;
	return ret;
}

template <typename baseType>
SaturatedInteger<baseType> SaturatedInteger<baseType>::operator+(const SaturatedInteger<baseType> &x) const{
	SaturatedInteger<baseType> ret(*this);
	ret += x;
	return ret;
}

template <typename baseType>
SaturatedInteger<baseType> SaturatedInteger<baseType>::operator-(const SaturatedInteger<baseType> &x) const{
	SaturatedInteger<baseType> ret(*this);
	ret -= x;
	return ret;
}

template <typename baseType>
template <typename outputType>
ErrorManagement::ErrorType SaturatedInteger<baseType>::ToNumber(outputType& output) const{
	ErrorManagement::ErrorType  ret;
	if (!IsValid()){
		ret.outOfRange = true;
	} else {
		ret.outOfRange = !SafeNumber2Number(data,output);
	}
	return ret;
}
#if 0
template <typename baseType>
template <typename outputType>
inline SaturatedInteger<baseType>::operator outputType() const{
	outputType ret;
	if (IsValid()){
		SafeNumber2Number(data,ret);
	}
	return ret;
}
#endif

template <typename baseType>
baseType SaturatedInteger<baseType>::GetData() const{
	return data;
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGER_H_ */
