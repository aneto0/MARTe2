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

#include "TypeCharacteristics.h"
#include "Units.h"
#include "SaturatedInteger.h"

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
	 *  @return the raw integer value of time. only valid if GetTime.IsValid()
	 */
	inline baseType 	GetTimeRaw() const ;

	/**
	 *  @return  the raw value of time. Actual value depends on the units
	 */
	inline SaturatedInteger<baseType> 	GetTime() const ;

	/**
	 *  @return the units used in this type
	 */
	inline unitType 	GetUnits() const ;

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
    inline TimeType<baseType,unitType> &operator-=(const TimeType<baseType,unitType> &tt);

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
    inline TimeType<baseType,unitType> operator-(const TimeType<baseType,unitType> &tt) const;

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
    inline TimeType<baseType,unitType> &operator+=(const TimeType<baseType,unitType> &tt);

    /**
     * @brief Subtracts the specified time to the current value.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return this object.
     */
    inline TimeType<baseType,unitType> operator+(const TimeType<baseType,unitType> &tt) const;

    /**
     * @brief scales a time type using a number. Product among TimeTypes has no meaning
     * @param[in] scale is the value to be multiplied
     * @return this object.
     */
    template <typename scaleType>
    inline TimeType<baseType,unitType> operator*(const scaleType &scale) const;

    /**
     * @brief Compares the values.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return true if this value is bigger than tt.
     */
    inline bool operator>(const TimeType<baseType,unitType> &tt) const;

    /**
     * @brief Compares the values.
     * @param[in] tt is the value which will be subtracted to the timeout
     * @return true if this value is smaller than tt.
     */
    inline bool operator<(const TimeType<baseType,unitType> &tt) const;

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
	 * indeterminate
	 */
	bool IsIndeterminate() const;

	/**
	 * Valid or positive infinite
	 */
	bool IsPositive() const;

	/**
	 * constant infinite
	 */
	static const  TimeType Infinite;


private:

	/**
	 */
	SaturatedInteger<baseType> 		time;

	/**
	 *
	 */
	unitType        units;

	/**
	 * construct from saturated integer
	 */
	inline TimeType(SaturatedInteger<baseType> si);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



template<typename baseType,class unitType>
baseType TimeType<baseType,unitType>::GetTimeRaw() const {
	return time.GetData();
}

template<typename baseType,class unitType>
SaturatedInteger<baseType> TimeType<baseType,unitType>::GetTime() const {
	return time.GetData();
}

template<typename baseType,class unitType>
unitType TimeType<baseType,unitType>::GetUnits() const {
	return units;
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsValid() const{
	return (time.IsValid()) ;
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsIndeterminate() const{
	return (time.IsIndeterminate());
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsInfinite() const{
	return (time.IsInfinite());
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::IsPositive() const{
	return (time.IsInfinite() || time.IsValid());
}


template<typename baseType,class unitType>
template<typename bT, class uT>
TimeType<baseType,unitType>::TimeType(bT time,const uT &units){
	if (units.GetScale() == 0.0){
		this->time = SaturatedInteger<baseType>(time);
	} else {
	    double temp = 0.5 + static_cast<double>(time) * (units.GetScale() * this->units.GetScaleInv());
		this->time = SaturatedInteger<baseType>(temp);
	}
}

template<typename baseType,class unitType>
template<typename bT, class uT>
    TimeType<baseType,unitType>::TimeType(const TimeType<bT,uT> &tt){
	if (tt.IsValid()){
		*this = TimeType<baseType,unitType>(tt.GetTimeRaw(),tt.GetUnits());
	} else {
		// if it is invalid then the calibration is irrelevant
		time = SaturatedInteger<baseType>(tt.GetTime());
	}
}

template<typename baseType,class unitType>
TimeType<baseType,unitType>::TimeType(){
	time = 0;
}

template<typename baseType,class unitType>
TimeType<baseType,unitType> &TimeType<baseType,unitType>::operator+=(const TimeType<baseType,unitType> &tt){
	time += tt.GetTime();
	return *this;
}

template<typename baseType,class unitType>
TimeType<baseType,unitType> &TimeType<baseType,unitType>::operator-=(const TimeType<baseType,unitType> &tt){
	time -= tt.GetTime();
	return *this;
}

template<typename baseType,class unitType>
TimeType<baseType,unitType> TimeType<baseType,unitType>::operator+(const TimeType<baseType,unitType> &tt) const{
	TimeType<baseType,unitType> res(*this);
	res += tt;
	return res;
}

template<typename baseType,class unitType>
TimeType<baseType,unitType> TimeType<baseType,unitType>::operator-(const TimeType<baseType,unitType> &tt) const{
	TimeType<baseType,unitType> res(*this);
	res -= tt;
	return res;
}

template<typename baseType,class unitType>
template <typename scaleType>
TimeType<baseType,unitType> TimeType<baseType,unitType>::operator*(const scaleType &scale) const{
	TimeType<baseType,unitType> res(*this);
	res.time.Scale(scale);
	return res;
}


template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::operator>(const TimeType<baseType,unitType> &tt) const{
	return time > tt.time;
}

template<typename baseType,class unitType>
bool TimeType<baseType,unitType>::operator<(const TimeType<baseType,unitType> &tt) const{
	return time < tt.time;
}


template<typename baseType,class unitType>
template<typename bT, class uT>
bool TimeType<baseType,unitType>::operator==(const TimeType<bT,uT> &tt) const{
	TimeType<baseType,unitType> t(tt);
	return (time==t.time)/* && t.IsValid()*/;
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

template<typename baseType,class unitType>
TimeType<baseType,unitType>::TimeType(SaturatedInteger<baseType> si){
	time = si;
}

template<typename baseType,class unitType>
const TimeType<baseType,unitType> TimeType<baseType,unitType>::Infinite(SaturatedInteger<baseType>('I'));


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_TIMETYPE_H_ */
