/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/

/**
 * @file
 * @brief Logical and mathematic shift operations.
 */
#ifndef SHIFT_H
#define SHIFT_H

#include "GeneralDefinitions.h"


/** @brief A collector of functions that implements logical and mathematic shift in safe mode. 
 *
 * These methods implements shift operations in the right way, returning zero when the shift is 
 * greater than the size of the number. Furthermore they implements the logical shift which
 * does not extend the sign for negative numbers and it is very useful in many functionalities.
 */
class Shift{
	


public:

	/** @brief Logical right shift for uint8 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline uint8 LogicalRightShift(uint8 number, uint8 shift){
		return number>>shift;
	}

	/** @brief Logical right shift for uint16 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline uint16 LogicalRightShift(uint16 number, uint8 shift){
		return number>>shift;
	}

	/** @brief Logical right shift for uint32 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline uint32 LogicalRightShift(uint32 number, uint8 shift){
		return number>>shift;
	}

	/** @brief Logical right shift for uint64 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline uint64 LogicalRightShift(uint64 number, uint8 shift){
		return number>>shift;
	}

	/** @brief Logical right shift for int8 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline int8 LogicalRightShift(int8 number, uint8 shift){
		return ((uint8)number)>>shift;
	}
	
	/** @brief Logical right shift for int16 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline int16 LogicalRightShift(int16 number, uint8 shift){
		return ((uint16)number)>>shift;
	}

	/** @brief Logical right shift for int32 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline int32 LogicalRightShift(int32 number, uint8 shift){
		return ((uint32)number)>>shift;
	}

	/** @brief Logical right shift for int64 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number right shifted. */
	static inline int64 LogicalRightShift(int64 number, uint8 shift){
		return ((uint64)number)>>shift;
	}


	/** @brief Logical left shift for uint8 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline uint8 LogicalLeftShift(uint8 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for uint16 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline uint16 LogicalLeftShift(uint16 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for uint32 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline uint32 LogicalLeftShift(uint32 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for uint64 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline uint64 LogicalLeftShift(uint64 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for int8 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline int8 LogicalLeftShift(int8 number, uint8 shift){
		return number<<shift;
	}
	
	/** @brief Logical left shift for int16 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline int16 LogicalLeftShift(int16 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for int32 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline int32 LogicalLeftShift(int32 number, uint8 shift){
		return number<<shift;
	}

	/** @brief Logical left shift for int64 numbers.
          * @param number is the number to shift.
          * @param shift is the shift in bits.
          * @return the number left shifted. */
	static inline int64 LogicalLeftShift(int64 number, uint8 shift){
		return number<<shift;
	}




public:

	/** @brief Implementation of a template function for safe logical shifts.
          * @param number is the number to be shifted.
          * @param shift is the shift in bits. 
          * @return the number shifted. */
	template <typename T>
	static inline T LogicalSafeShift(T number, int8 shift){

		int bound = sizeof(T)*8;

		//if |shift| is greater than the size of the number return 0.		
		if(shift <= -bound || shift >= bound) return (T)0;

		//if shift is greater than zero do the left shift.
		if(shift > 0)
			return number<<shift;
		//if shift is minor than 0 do the right shift.		
		else
			return LogicalRightShift(number,(uint8)(-shift));
	}

	/** @brief Implementation of a template function for safe mathematic shifts.
          * @param number is the number to be shifted.
          * @param shift is the shift in bits. 
          * @return the number shifted. */
	template <typename T>
	static inline T MathematicSafeShift(T number, int8 shift){ 
		
		int bound=sizeof(T)*8;
		
		//if |shift| is greater than the number size return 0.
		if(shift >= bound || shift <= -bound){
			return (T)0;
		}

		//if shift is greater than 0 d the left shift.
		if(shift > 0)
			return number<<shift;
		//if the shift is minor than zero do the right shift.
		else
			return number>>(-shift);
	}
	


};
			
#endif




