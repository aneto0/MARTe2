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
template <typename T>
class SaturatedInteger{
public:
	/**
	 * @brief constructor.
	 * @param special if 'I' the number is loaded with infinite, if 'i' with -infinite, with '\0' with zero, otherwise undeterminate
	 */
	SaturatedInteger(char8 special='\0');

	/**
	 * constructor
	 */
	template <typename T2>
	SaturatedInteger(const T2 & in);

	/**
	 * constructor
	 */
	template <typename T2>
	SaturatedInteger(const SaturatedInteger<T2> & in);

	/**
	 * @brief true if the number is larger than maxint
	 */
	inline bool isPositiveInf() const;

	/**
	 * @brief true if the number is smaller than -maxint
	 */
	inline bool isNegativeInf() const;

	/**
	 * @brief true if the number is not defined
	 */
	inline bool isIndeterminate() const;

	/**
	 * @brief true if the number does not have a defined value: either inf,-inf,indet
	 */
	inline bool notANormalNumber() const;

	/**
	 * @brief true if the number is negative
	 */
	inline bool isNegative() const;

	/**
	 * @brief copy operator
	 */
	inline SaturatedInteger<T> operator= (const SaturatedInteger<T> &x);

	/**
	 * @brief compare operator
	 */
	inline bool operator== (const SaturatedInteger<T> &x) const;

	/**
	 * @brief different operator
	 */
	inline bool operator!= (const SaturatedInteger<T> &x) const;

	/**
	 * @brief saturation safe product operator
	 */
	inline SaturatedInteger<T> operator*(const SaturatedInteger<T> &x) const;

	/**
	 * @brief saturation safe product operator
	 */
	inline SaturatedInteger<T> operator+(const SaturatedInteger<T> &x) const;

	/**
	 * @brief saturation safe product operator
	 */
	inline SaturatedInteger<T> operator-(const SaturatedInteger<T> &x) const;

	/**
	 * @brief allows copying the saturated value. Returns error if the value is +/-inf or it does not fit in the T2 type
	 */
	template <typename T2>
	inline ErrorManagement::ErrorType ToNumber(T2& output) const;

	/**
	 * @brief allows access to the internal value
	 */
	inline T GetData() const;

	/**
	 * @brief generates a +infinite
	 */
	static SaturatedInteger<T> PlusInfinite();

	/**
	 * @brief generates a -infinite
	 */
	static SaturatedInteger<T> MinusInfinite();

	/**
	 * @brief generates a not determinate
	 */
	static SaturatedInteger<T> Indeterminate();

	/**
	 * @brief generates a zero
	 */
	static SaturatedInteger<T> Zero();
private:
	/**
	 * The maximum value allowed for this number
	 */
	const T hiLimitValue = TypeCharacteristics<T>::MaxValue()-3;
	/**
	 * The minimum value allowed for this number
	 */
	const T loLimitValue = TypeCharacteristics<T>::MinValue();
	/**
	 * The maximum value allowed for this number
	 */
	const T positiveInf = hiLimitValue+3;
	/**
	 * The maximum value allowed for this number
	 */
	const T negativeInf = hiLimitValue+2;
	/**
	 * The maximum value allowed for this number
	 */
	const T indeterminate = hiLimitValue+1;

	/**
	 *  the actual data
	 */
	T data;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename T>
SaturatedInteger<T>::SaturatedInteger(char8 special='\0'){
	if (special == '\0'){
		data = 0;
	}
	if (special == 'I'){
		data = positiveInf;
	}
	if (special == 'i'){
		data = negativeInf;
	} else {
		data = indeterminate;
	}
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::PlusInfinite(){
	return SaturatedInteger<T>('I');
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::MinusInfinite(){
	return SaturatedInteger<T>('i');
}

template <typename T>
SaturatedInteger<T>	SaturatedInteger<T>::Indeterminate(){
	return SaturatedInteger<T>('x');
}

template <typename T>
SaturatedInteger<T>	SaturatedInteger<T>::Zero(){
	return SaturatedInteger<T>('\0');
}

template <typename T>
template <typename T2>
SaturatedInteger<T>::SaturatedInteger(const T2 & x){
	if (x > hiLimitValue){
		data = positiveInf;
	} else
	if (x < loLimitValue){
		data = negativeInf;
	} else {
		data = x;
	}
}


template <typename T>
template <typename T2>
SaturatedInteger<T>::SaturatedInteger(const SaturatedInteger<T2> & in){
	if (in.isPositiveInf()) {
		data = positiveInf;
	} else
	if (in.isNegativeInf()) {
		data = negativeInf;
	} else
	if (in.isIndeterminate()) {
		data = indeterminate;
	} else
	if (in.data > hiLimitValue){
		data = positiveInf;
	} else
	if (in.data < loLimitValue){
		data = negativeInf;
	} else {
		data = in.data;
	}
}

template <typename T>
bool SaturatedInteger<T>::isPositiveInf() const{
	return (data == positiveInf);
}

template <typename T>
bool SaturatedInteger<T>::isNegativeInf() const{
	return (data == negativeInf);
}

template <typename T>
bool SaturatedInteger<T>::isIndeterminate() const{
	return (data == indeterminate);
}

template <typename T>
bool SaturatedInteger<T>::notANormalNumber() const{
	return (data > hiLimitValue);
}

template <typename T>
bool SaturatedInteger<T>::isNegative() const{
	return (isNegativeInf() || (data < 0));
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::operator= (const SaturatedInteger &x){
	data = x.data;
	return *this;
}

template <typename T>
bool SaturatedInteger<T>::operator== (const SaturatedInteger &x) const{
	return (data == x.data);
}

template <typename T>
bool SaturatedInteger<T>::operator!= (const SaturatedInteger &x) const{
	return (data != x.data);
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::operator*(const SaturatedInteger<T> &x) const{
	SaturatedInteger<T> ret;
	if (x.isIndeterminate() || isIndeterminate()){
		ret.data = indeterminate;
	} else
	if (isPositiveInf() ){
		if (x.isNegative()){
			ret.data = negativeInf;
		} else {
			ret.data = positiveInf;
		}
	} else
	if (isNegativeInf()){
		if (x.isNegative()) {
			ret.data = positiveInf;
		} else {
			ret.data = negativeInf;
		}
	} else {
		double test = (double)data * (double)x.data;
		if (test > hiLimitValue){
			ret.data = positiveInf;
		} else
		if (test < loLimitValue){
			ret.data = negativeInf;
		} else {
			ret.data = data * x.data;
		}
	}
	return ret;
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::operator+(const SaturatedInteger<T> &x) const{
	SaturatedInteger<T> ret;
	if (x.isIndeterminate() || isIndeterminate()){
		ret.data = indeterminate;
	} else
	if (isPositiveInf()){
		if (x.isNegativeInf()){
			ret.data = indeterminate;
		} else {
			ret.data = positiveInf;
		}
	} else
	if (isNegativeInf()){
		if (x.isPositiveInf()) {
			ret.data = indeterminate;
		} else {
			ret.data = negativeInf;
		}
	} else {
		double test = (double)data + (double)x.data;
		if (test > hiLimitValue){
			ret.data = positiveInf;
		} else
		if (test < loLimitValue){
			ret.data = negativeInf;
		} else {
			ret.data = data + x.data;
		}
	}
	return ret;
}

template <typename T>
SaturatedInteger<T> SaturatedInteger<T>::operator-(const SaturatedInteger<T> &x) const{
	SaturatedInteger<T> ret;
	if (x.isIndeterminate() || isIndeterminate()){
		ret.data = indeterminate;
	} else
	if (isPositiveInf()){
		if (x.isNegativeInf()){
			ret.data = indeterminate;
		} else {
			ret.data = positiveInf;
		}
	} else
	if (isNegativeInf()){
		if (x.isPositiveInf()) {
			ret.data = indeterminate;
		} else {
			ret.data = negativeInf;
		}
	} else {
		double test = (double)data - (double)x.data;
		if (test > hiLimitValue){
			ret.data = positiveInf;
		} else
		if (test < loLimitValue){
			ret.data = negativeInf;
		} else {
			ret.data = data - x.data;
		}
	}
	return ret;
}

template <typename T>
template <typename T2>
ErrorManagement::ErrorType SaturatedInteger<T>::ToNumber(T2& output) const{
	ErrorManagement::ErrorType  ret;
	if (notANormalNumber()){
		ret.outOfRange = true;
	} else
	if (data > TypeCharacteristics<T2>::MaxValue()){
		ret.outOfRange = true;
	} else
	if (data < TypeCharacteristics<T2>::MinValue()){
		ret.outOfRange = true;
	} else {
		output = data;
	}
	return ret;
}

template <typename T>
T SaturatedInteger<T>::GetData() const{
	return data;
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGER_H_ */
