/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: CStream.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/
/**
 * @file 
 * @brief Constructs integer of double size. Limited operator support.
*/

#if !defined DOUBLE_INTEGER
#define DOUBLE_INTEGER

#include "GeneralDefinitions.h"
#include <math.h>


/** @brief Implementation of an integer which duplicates the size of the template passed in argument.
  * 
  * Implementation is endianity dependant.
  * Implements an integer number of double the size of T.
  * Uses operators of T to implement operators.
 */
template<typename T>
class DoubleInteger{

private:

	/** @brief Checks if the template is a signed type.
	  * @return true if the template is a signed type, false otherwise.*/
	static inline bool IsSigned(){
		return ((T)-1)<0;
	}

public:
	/** Least significative. */
	T lower;
	/** Most significative. */
	T upper;
public:
	/** @brief Default constructor. */
	DoubleInteger(){
		lower = 0;
		upper = 0;
	}

	/** @brief Copy constructor. */
	DoubleInteger(const DoubleInteger<T> &n){
		lower = n.lower;
		upper = n.upper;
	}
	/** @brief Smart copy constructor. */
	template<typename T2>
	DoubleInteger(T2 n){
		//if n=0 initialize to zero.
		if (n==0){
			lower = 0;
			upper = 0;
		} else
		if (n>0){
			//cast the lower bits
			lower = (T)n;
			upper = 0;
			if (sizeof (T2) > sizeof(T)){
				uint8 shift = (sizeof(T)*8); 
				//put in upper the most significative bits remained.
				n >>= shift;
				upper = (T)n;
			} 
		} else {
			//cast the lower bits
			lower = (T)n;
			upper = -1;
			if (sizeof (T2) > sizeof(T)){
				uint8 shift = (sizeof(T)*8); 
				//put in upper the most significative bits remained.
				n >>= shift;
				upper = (T)n;
			} 
			
		}
	}


	/*	
	DoubleInteger(T *memory){
		lower = *memory++;
		upper = *memory;
	}
	*/



	/** @brief Implementation of mathematic left shift.
	  * @param shift is the desired shift. */
	void operator<<=(int16 shift){
		// shift of sizeof(T)*8 is treated as shift 0
		// for this reason exit here to avoid this pitfall
		if (shift <= 0) return;
		// calculates n of bits of T
		int bitSize = (sizeof(T)*8);
		// shift within one half
		if (shift < bitSize){
			// shift upper first
			upper = upper << shift;
			// add overflow from lower
			// this would fail if shift is 0
			upper |= (lower >> (bitSize - shift));
			// complete the lower
			lower = lower << shift;
		} else { // more than half!			
			// remove half
			shift -= bitSize;
			// swap lower -> upper and shift with the remainder
			upper = lower << shift;
			// lower is 0
			lower = 0;
		}
	}	

	/** @brief Implementation of mathematic right shift.
	  * @param shift is the desired shift. */
	void operator>>=(int16 shift){
		// shift of sizeof(T)*8 is treated as shift 0
		// for this reason exit here to avoid this pitfall
		if (shift <=0) return;
		// calculates n of bits of T
		int bitSize = (sizeof(T)*8);
		// shift within one half
		if (shift < bitSize){
			// shift lower first
			lower = lower >> shift;		
			// add overflow from upper
			// this would fail if shift is 0
			lower |= (upper << (bitSize-shift));
			// complete the upper
			upper = upper >> shift;
		} else { // more than half!			
			// remove half
			shift -= bitSize;
			// swap upper -> lower and shift with the remainder
			lower = upper >> shift;
			// upper is 0
			if(upper < 0) upper = -1;
			else upper = 0;
		}
	}	

	/** @brief Implementation of mathematic right shift.
	  * @param shift is the desired shift.
          * @return this right shifted of shift. */
	DoubleInteger<T>  operator>>(uint8 shift)const{
		DoubleInteger<T>output(*this);
		// uses unary op
		output >>= shift;		
		return output;
	}

	/** @brief Implementation of mathematic left shift.
	  * @param shift is the desired shift.
          * @return this left shifted of shift. */
	DoubleInteger<T>  operator<<(uint8 shift)const{
		DoubleInteger<T>output(*this);
		// uses unary op
		output <<= shift;		
		return output;
	}
	/** @brief Bitwise and operator.
          * @param n is the DoubleInteger argument. */
	void operator&=(const DoubleInteger<T> &n){
		lower &= n.lower;		
		upper &= n.upper;
	}	

	/** @brief Bitwise or operator.
          * @param n is the DoubleInteger argument. */
	void operator|=(const DoubleInteger<T> &n){
		lower |= n.lower;		
		upper |= n.upper;
	}	

	/** @brief Is Different operator.
	  * @param n is the DoubleInteger argument for comparison.
          * @return false if this and n are equal, true otherwise. */
	bool operator!=(const DoubleInteger<T> &n)const{
		return ((upper != n.upper) || (lower != n.lower)); 
	}


	/** @brief Is Equal operator.
	  * @param n is the DoubleInteger argument for comparison.
          * @return true if this and n are equal, false otherwise. */
	bool operator==(const DoubleInteger<T> &n)const{
		return !((*this)!=n);
	}

	/** @brief Bitwise invert. */
	DoubleInteger<T>  operator~()const{
		DoubleInteger<T>output;
		output.upper = ~upper;
		output.lower = ~lower;
		return output;
	}
	/** @brief Bitwise and. 
          * @param n is the DoubleInteger argument.
          * @return the result of the bitwise and between this and n.*/ 
	DoubleInteger<T>  operator&(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output;
		output.upper = upper & n.upper;
		output.lower = lower & n.lower;
		return output;
	}
	/** @brief Bitwise or.
          * @param n is the DoubleInteger argument.
          * @return the result of the bitwise or between this and n.*/
	DoubleInteger<T>  operator|(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output;
		output.upper = upper | n.upper;
		output.lower = lower | n.lower;
		return output;
	}

	/** @brief Math subtraction.
	  * @param n is the number to subtract.
          * @return the result of the subtraction. */ 
	DoubleInteger<T>  operator-(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output;
		// subtract upper
		output.upper = upper - n.upper;
		output.lower = lower - n.lower;

		//if the template is signed we must consider negative number
		//as unsigned number then we must decrement upper when:
		//lowers both negative or both positive, if n.lower > lower.
		//lower positive and n.lower negative. 
		if(IsSigned()){
			if(lower>=0 && n.lower<0){
				output.upper--;
				return output;
			}
			if(lower<0 && n.lower>=0) return output;  
		}

		// if there will be an underflow borrow from upper
		if (n.lower > lower) output.upper--;
		// subtract lower
	
		return output;
	}



	/** @brief Math sum.
	  * @param n is the number to add.
          * @return the sum. */
	DoubleInteger<T>  operator+(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output;

		// add upper
		output.upper = upper + n.upper;
		// add lower
		output.lower = lower + n.lower;

		//if the template is signed and the the two lowers are negative increment upper
		//because we need to consider them as unsigned values.
		if(IsSigned()){
			if(lower<0 && n.lower<0){
			        output.upper++;
				return output;
			}
		}

		// handle overflow
		//case are both positive or one negative one positive.
		//if result is negative, no overflow happens.
		//the result has to be be minor than the positive otherwise is overflow.
        if (((output.lower < lower) || (output.lower < n.lower)) && (output.lower >= 0)) output.upper++;  			
		return output;
	}
	
	/** @brief Logical >.
          * @param n is the DoubleInteger for the comparison.
    	  * @return true if this is greater than n. */
	bool operator>(const DoubleInteger<T> &n)const{
		//check the greater upper 
		if(upper > n.upper) return true;
		if(upper < n.upper) return false;
		
		//in case of same upper	
		if(IsSigned()){
			//consider the lower as unsigned.
			if(lower < 0 && n.lower>=0) return true;
			if(lower >=0 && n.lower<0) return false;
		}
		
		return (lower > n.lower); 
	}

	
	/** @brief Logical <.
          * @param n is the DoubleInteger for the comparison.
    	  * @return true if this is minor than n. */
	bool operator<(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output=(*this);
		return !(output>n);	
	//
	}

	/** @brief Logical >=.
          * @param n is the DoubleInteger for the comparison.
    	  * @return true if this is greater or equal than n. */
	bool operator>=(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output=(*this);
		return (output>n || (upper==0 && lower==0));
	}
	
	/** @brief Logical <=.
          * @param n is the DoubleInteger for the comparison.
    	  * @return true if this is minor than n. */
	bool operator<=(const DoubleInteger<T> &n)const{
		DoubleInteger<T>output=(*this);
		return (output<n || (upper==0 && lower==0));
	}



	/** @brief Math subtraction.
	  * @param n is the number to subtract.*/
	void  operator-=(const DoubleInteger<T> &n){
		DoubleInteger<T>output=(*this);
		(*this)=output-n;
	}

	/** @brief Math sum.
	  * @param n is the number to add.*/
	void  operator+=(const DoubleInteger<T> &n){
		DoubleInteger<T>output=(*this);
		(*this)=output+n;
	}


	
};

#endif
