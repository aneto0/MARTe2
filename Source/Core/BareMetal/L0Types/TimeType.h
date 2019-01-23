/**
 * @file TimeType.h
 * @brief Header file for class AnyType
 * @date 16 Sep 2018
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

#ifndef TIMETYPE_H_
#define TIMETYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//#include "../L1Portability/GeneralDefinitions.h"
#include "TypeCharacteristics.h"
#include "Units.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 *  @brief an unsigned integer number representing time.
 *  @details This template is to build the secTime msecTime and usecTime
 *  @tparam baseType the standard type which is used as a base for the new type - must be unsigned integer of any size
 *  @tparam unitType a member class of the Units namespace
 */
template<typename baseType,class unitType>
class TimeType{
public:

	/**
	 * Initializes the raw value
	 */
	inline TimeType();

	/**
	 *  @brief constructor from generic number and Unit::unit_constant
	 *  @tparam bT the type of time
	 *  @tparam uT the type of units
	 *  @param time is a number that specifies the time duration
	 *  @param units is a static member of Units. i.e. Units::s
	 */
	template<typename bT, class uT>
	inline TimeType(bT time,const uT &units);

	/**
	 *  @brief constructor from generic TimeType
	 *  @tparam bT the type of time
	 *  @tparam uT the type of units
	 *  @param tt the time to be copied and converted
	 */
	template<typename bT, class uT>
	inline TimeType(const TimeType<bT,uT> &tt);

	/**
	 *  @return  the raw value of time. Actual value depends on the units
	 */
	inline baseType 	GetTimeRaw() const ;

	/**
	 *  @return  the max value of the raw time (before special codes)
	 */
	static inline baseType 	GetMaxRawValue() ;

	/**
	 *  @return the units used in this type
	 */
	inline unitType 	GetUnits() const ;

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
	template<typename bT, class uT>
    inline TimeType<baseType,unitType> &operator-=(const TimeType<bT,uT> &tt);

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
	template<typename bT, class uT>
    inline TimeType<baseType,unitType> &operator+=(const TimeType<bT,uT> &tt);

	/**
     * @brief Compares two TimeTypes.
     * @param[in] tt is the timeout object to be compared with this.
     * @return true if the time value of both objects is the same when converted to this object time resolution.
     */
	template<typename bT, class uT>
    inline bool operator==(const TimeType<bT,uT> &tt) const;

    /**
     * @brief Compares two TimeTypes.
     * @param[in] tt is the timeout object to be compared with this.
     * @return true if the time value of both objects is the same when converted to this object time resolution.
     */
	template<typename bT, class uT>
    inline bool operator!=(const TimeType<bT,uT> &tt) const;

    /**
     * @brief Copy operator.
     * @param[in] tt is the TimeType to copy in this.
     * @return this object.
     */
	template<typename bT, class uT>
	inline TimeType<baseType,unitType> &operator=(const TimeType<bT,uT> &tt);

	/**
	 * 0 to Max-4
	 */
	bool IsValid() const;

	/**
	 *  + infinite or - infinite
	 */
	bool IsInfinite() const;

	/**
	 * -infinite or negative
	 */
	bool IsNegative() const;

	/**
	 *  +infinite or valid
	 */
	bool IsPositive() const;

	/**
	 * indeterminate
	 */
	bool IsIndeterminate() const;

	/**
	 * constant infinite
	 */
	static const TimeType<baseType,unitType> Infinite;

	/**
	 * constant negative
	 */
	static const TimeType<baseType,unitType> Negative;

	/**
	 * Returns 0 for a valid number or infiniteCodeOffset,negativeCodeOffset
	 */
	inline uint8 GetCode() const;

private:

	/**
	 * Contains the time information expressed in the units specified
	 * It is an unsigned integer number
	 * Valid values are 0 to maxValue-4
	 * maxValue indicates positive saturation
	 * maxValue-1 indicates negative saturation
	 * maxValue-2 spare
	 * maxValue-3 spare
	 */
	baseType 		time;

	/**
	 *
	 */
	unitType        units;

	/**
	 *
	 */
	static const uint8 spareCodes     	  = 4;
	static const uint8 infinite 		  = 4;
	static const uint8 negative           = 3;
	static const uint8 negativeInf 		  = 2;
	static const uint8 indeterminate 	  = 1;
	static const uint8 valid    		  = 0;

	static const uint8 TTSubtractionCodeMap[5][5];
	static const uint8 TTAdditionCodeMap[5][5];

	static const baseType maxRawValue;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType,class unitType>
const baseType TimeType<baseType,unitType>::maxRawValue = TypeCharacteristics<baseType>::MaxValue()-spareCodes;

template<typename baseType,class unitType>
const TimeType<baseType,unitType> TimeType<baseType,unitType>::Infinite(maxRawValue + infinite,Units::raw);

template<typename baseType,class unitType>
const TimeType<baseType,unitType> TimeType<baseType,unitType>::Negative(maxRawValue+ negative,Units::raw);


/*
 *  Maps Code combination to code
 */
template<typename baseType,class unitType>
const uint8 TimeType<baseType,unitType>::TTSubtractionCodeMap[5][5] = {
		{valid,    	    indeterminate, infinite,      indeterminate, negativeInf},   // valid
		{indeterminate, indeterminate, indeterminate, indeterminate, indeterminate}, // indeterminate
		{negativeInf,   indeterminate, indeterminate, negativeInf,   negativeInf},   // negative inf
		{negative,      indeterminate, infinite,      indeterminate, negativeInf},   // negative -
		{infinite,      indeterminate, infinite,      infinite, 	 indeterminate}  // infinite
};

/*
 *  Maps Code combination to code
 */
template<typename baseType,class unitType>
const uint8 TimeType<baseType,unitType>::TTAdditionCodeMap[5][5] = {
		{valid,    	    indeterminate, negativeInf,   indeterminate, infinite},      // valid
		{indeterminate, indeterminate, indeterminate, indeterminate, indeterminate}, // indeterminate
		{negativeInf,   indeterminate, negativeInf,   negativeInf,   indeterminate}, // negative inf
		{indeterminate, indeterminate, negativeInf,   negative,      infinite},      // negative -
		{infinite,      indeterminate, indeterminate, infinite, 	 infinite}       // infinite
};


template<typename baseType,class unitType>
baseType TimeType<baseType,unitType>::GetTimeRaw() const {
	return time;
}

template<typename baseType,class unitType>
uint8 TimeType<baseType,unitType>::GetCode() const {
	uint8 ret = valid;
	if (time >= maxRawValue){
		// valid because there are only a few possible values above maxRawValue
		ret = static_cast<uint8>(time - maxRawValue);
	}
	return ret;
}

template<typename baseType,class unitType>
baseType TimeType<baseType,unitType>::GetMaxRawValue()  {
	return maxRawValue;
}

template<typename baseType,class unitType>
unitType TimeType<baseType,unitType>::GetUnits() const {
	return units;
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsValid() const{
	return (GetCode() == valid) ;
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsIndeterminate() const{
	return (GetCode() == indeterminate);
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsInfinite() const{
	return (GetCode() == infinite) || (GetCode() == negativeInf);
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsNegative() const{
	return (GetCode() == negative) || (GetCode() == negativeInf);
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsPositive() const{
	return (GetCode() == infinite) || (GetCode() == valid);
}

template<typename baseType,class unitType>
template<typename bT, class uT>
TimeType<baseType,unitType>::TimeType(bT time,const uT &units){
	if (units.GetScale() == 0.0){
		this->time = static_cast<baseType>(time);
	} else {
	    double temp = 0.5 + static_cast<double>(time) * (units.GetScale() * this->units.GetScaleInv());
		if (temp < 0.0){
	        *this = Negative;
	    } else
		if (temp >= static_cast<double>(maxRawValue+1)){
	        *this = Infinite;
	    } else {
	        this->time = static_cast<baseType>(temp);
	    }
	}
}

template<typename baseType,class unitType>
template<typename bT, class uT>
    TimeType<baseType,unitType>::TimeType(const TimeType<bT,uT> &tt){
	if (tt.IsValid()){
		*this = TimeType<baseType,unitType>(tt.GetTimeRaw(),tt.GetUnits());
	} else {
		time = maxRawValue + tt.GetCode();
	}
}

template<typename baseType,class unitType>
TimeType<baseType,unitType>::TimeType(){
	time = 0;
}

template<typename baseType,class unitType>
template<typename bT, class uT>
TimeType<baseType,unitType> &TimeType<baseType,unitType>::operator+=(const TimeType<bT,uT> &tt){
	TimeType<baseType,unitType> t(tt);
	if (!IsValid() || !tt.IsValid()){
		time = maxRawValue +  TTAdditionCodeMap[GetCode()][tt.GetCode()];
	} else {
		baseType ts = time + t.time;
		if ((ts < time ) && (ts < t.time)){
			*this = Infinite;
		} else {
			*time = ts;
		}
	}
	return *this;
}

template<typename baseType,class unitType>
template<typename bT, class uT>
TimeType<baseType,unitType> &TimeType<baseType,unitType>::operator-=(const TimeType<bT,uT> &tt){
	TimeType<baseType,unitType> t(tt);
	if (!IsValid() || !tt.IsValid()){
		time = maxRawValue +  TTSubtractionCodeMap[GetCode()][tt.GetCode()];
	} else
		if (time >= t.time){
			time -= t.time;
		} else {
			*this = Negative();
		}
	return *this;
}


template<typename baseType,class unitType>
template<typename bT, class uT>
bool TimeType<baseType,unitType>::operator==(const TimeType<bT,uT> &tt) const{
	TimeType<baseType,unitType> t(tt);

	return (t==t.time) && t.IsValid();
}

template<typename baseType,class unitType>
template<typename bT, class uT>
bool TimeType<baseType,unitType>::operator!=(const TimeType<bT,uT> &tt) const{
	return !(*this == tt);
}

template<typename baseType,class unitType>
template<typename bT, class uT>
inline TimeType<baseType,unitType> &TimeType<baseType,unitType>::operator=(const TimeType<bT,uT> &tt){
	*this = TimeType<baseType,unitType>(tt);
	return *this;
}



} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_TIMETYPE_H_ */
