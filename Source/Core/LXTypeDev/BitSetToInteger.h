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
 * @brief Functions to convert range of bits among each other.
*/
#if !defined BITSET_TO_INTEGER
#define BITSET_TO_INTEGER

#include "GeneralDefinitions.h"
#include "FormatDescriptor.h"
#include <math.h>
#include "DoubleInteger.h"

/** @Brief BitSet to BitSet function for private use only.
  * @param destination is a pointer to the bitSet destination.
  * @param destinationBitShift is the desired shift in bit in destination variable.
  * @param destinationBitSize is the desired size in bit for the number to copy in destination.
  * @param destinationIsSigned specifies is the number in destination will be considered signed (true) or not (false).
  * @param source is a pointer to the bitSet source.
  * @param sourceBitShift defines the shift from which extract the number.
  * @param sourceBitSize is the desired size in bit for the number to extract from source.
  * @param sourceIsSigned specifies if the number to extract from source will be considered signed (true) or not (false).
  *
  * Source negative (then signed) and destination unsigned leads that zero is copied (saturation).
  * Source negative and destination signed with a greater dimension leads that will be copied the number with the sign extension.
  * If source is negative and destination is signed with a minor dimension, the function checks if source is minor than the minimum
  * supported by the destination dimension and in positive case copy the minimum possible (1000...). 
  * If source is positive and destination is signed the function operates a saturation observing the destination size (-1 in case of 
  * destination signed).
*/
template <typename T>
static inline void BSToBS(
    T *&                destination,
    uint8  &            destinationBitShift,
    uint8               destinationBitSize,
    bool                destinationIsSigned,
    T *&                source,
    uint8  &            sourceBitShift,
    uint8               sourceBitSize,
    bool                sourceIsSigned)
{
	// how many bits in T
	uint8 dataSize = sizeof(T)*8;
	
	// calculate mask covering range of input bits (starting from bit 0)
	T sourceMask           = 0 ;
	// all FFFFF....
	sourceMask 			   =~sourceMask;
	// shift FFFF.. to cover just the number bit size
	sourceMask 			   >>= (dataSize - sourceBitSize);
	
	// calculate mask covering range of output bits (starting from bit 0)
	T destinationMask      = 0;
	destinationMask 	   =~destinationMask;
	destinationMask 	   >>= (dataSize - destinationBitSize);

	// mask pinpointing sign bit (starting from bit 0)
	T sourceSignMask       = 1;
	sourceSignMask         <<= (sourceBitSize - 1);
	
	// mask pinpointing sign bit (starting from bit 0)
	T destinationSignMask  = 1;
	destinationSignMask    <<= (destinationBitSize - 1);
	
	// here we put the number while processing it
	T sourceCopy;
	
	// copy
	sourceCopy = *source;
	// shift number so LSB at bit 0
	// removes lower bits
	sourceCopy >>= sourceBitShift;	
	// mask out upper bits
	sourceCopy &= sourceMask;
	
	// extract sign bit
	T signBit = sourceCopy & sourceSignMask;
	
	// is negative if it has sign and last bit is 1
	bool sourceIsNegative = sourceIsSigned && (signBit != 0);

	// this means both that has sign and that the last bit is 1
	if (sourceIsNegative ){
		// if destination is not signed saturates to 0
		if (!destinationIsSigned){
			// saturate to 0
			sourceCopy = 0;
		} else {
			// if I need to squeeze a larger number into a smaller 
			if (sourceBitSize > destinationBitSize){
				// create a mask of bits covering the bits where source exceeds destination 
                                // the mask contains sourceBitSize-destinationBitSize+1 ones, and 
                                // if all of these bits are ones in the source the number can be written in the destination.
                                // ex. 1101 in 3 bits can be written (mask=1100), 1001 not.
				T mask = sourceMask - (destinationMask >> 1);

				// if any of these bits is not 1 then we have a larger negative number
				if ((sourceCopy & mask) != mask){
					// 0x8000.. is the maximum negative
					sourceCopy = destinationSignMask;
				}					
			} else { // smaller to larger sign extension needed
				// maks has all the bits that will be added
				T mask = destinationMask - sourceMask ;
				// it already had a sign bit, now we extend it to the full destination size
				sourceCopy |= mask;
				
			}
		}			
	} else {
		// 0xFFF.. is the max value for unsigned
		T maxPositiveValue = destinationMask; 
		if (destinationIsSigned){
			// 0x7FF.. is the max value for signed
			maxPositiveValue = (destinationMask >> 1);
		} 
		// saturate to max
		if (sourceCopy > maxPositiveValue){
			sourceCopy = maxPositiveValue;					
		}
	}
	// move the bits into output position
	sourceCopy <<= destinationBitShift;
	// shift mask as well
	destinationMask <<= destinationBitShift;
	// complementary mask- to erase all bits in destination range
	destinationMask = ~destinationMask;
	// use destinationMask to hold the current value at destination after masking 
	destinationMask &= *destination;
	// merge into sourceCopy
	sourceCopy |= destinationMask;
	// finally write result
	*destination = sourceCopy;  

}





/** @Brief BitSet to BitSet function for copy a bit range from a source to a destination.
  * @param destination is a pointer to the bitSet destination.
  * @param destinationBitShift is the desired shift in bit in destination variable.
  * @param destinationBitSize is the desired size in bit for the number to copy in destination.
  * @param destinationIsSigned specifies is the number in destination will be considered signed (true) or not (false).
  * @param source is a pointer to the bitSet source.
  * @param sourceBitShift defines the shift from which extract the number.
  * @param sourceBitSize is the desired size in bit for the number to extract from source.
  * @param sourceIsSigned specifies if the number to extract from source will be considered signed (true) or not (false).
  *
  * Converts an integer of bitSize sourceBitSize located at address source and bitAddress sourceBitShift
  * into an integer of bitSize destinationBitSize located at address destination and bitAddress destinationBitShift
  * destination and source must be of the same type T. 
  * T must be unsigned int of byte size power of 2 (8,16,32,64,128...) not 24 48 etc...
  * T determines the minimum number size used for the operations.
  * T=uint8 means that any number may be used.
  */
template <typename T>
static inline bool BitSetToBitSet(
        T *&                destination,
        uint8  &            destinationBitShift,
        uint8               destinationBitSize,
        bool                destinationIsSigned,
        T *&                source,
        uint8  &            sourceBitShift,
        uint8               sourceBitSize,
        bool                sourceIsSigned)
{
	
	// calculates granularity as bits
	int16 granularity      = sizeof(T) * 8;
	// mask to eliminate multiples of granularity - 
	// granularity must be a power of 2 
	int16 granularityMask  = granularity - 1;
	// exponent of the power of 2 that is granularity (log2 of the number of bits in T)
	int16 granularityShift = 3;
	int16 temp = sizeof(T);
	while (temp > 1){
		granularityShift++;
		temp >>=1;
	}
		
	// normalise sourceBitShift so that  0<sourceBitShift<granularity
	// adjusts source pointer adding sourceBitShift/numberOfBytes.
	if (sourceBitShift >= granularity){
		source += (sourceBitShift >> granularityShift);
		sourceBitShift &= granularityMask;
	} 
	// normalise destinationBitShift so that  0<destinationBitShift<granularity
	// adjusts destination pointer
	if (destinationBitShift >= granularity){
		destination += (destinationBitShift >> granularityShift);
		destinationBitShift &= granularityMask;
	}

	// work out range of bits involved 
	// for source  
	int sourceBitEnd      =  sourceBitShift     +sourceBitSize;
	// and for destination
	int destinationBitEnd =  destinationBitShift+destinationBitSize;
	
	// check number able to accomodate in full both input and output numbers 
	// and is big than granularity
	if ((sourceBitEnd <= 8) && (destinationBitEnd <= 8) && (granularity == 8)){
		// if 8 is fine then operate with 8 bit integers
		uint8 *destination8 = (uint8 *)destination;
		uint8 *source8      = (uint8 *)source;

		BSToBS(destination8,destinationBitShift,destinationBitSize,destinationIsSigned,source8,sourceBitShift,sourceBitSize,sourceIsSigned);	
		
	} else if ((sourceBitEnd <= 16)  && (destinationBitEnd <= 16) && (granularity <= 16)){
		// if 16 is fine then operate with 16 bit integers
		uint16 *destination16 = (uint16 *)destination;
		uint16 *source16      = (uint16 *)source;

		BSToBS(destination16,destinationBitShift,destinationBitSize,destinationIsSigned,source16,sourceBitShift,sourceBitSize,sourceIsSigned);
			
	} else if ((sourceBitEnd <= 32) &&  (destinationBitEnd <= 32) && (granularity <= 32)){
		// if 32 is fine then operate with 32 bit integers
		uint32 *destination32 = (uint32 *)destination;
		uint32 *source32      = (uint32 *)source;

		BSToBS(destination32,destinationBitShift,destinationBitSize,destinationIsSigned,source32,sourceBitShift,sourceBitSize,sourceIsSigned);
	
	} else if ((sourceBitEnd <= 64) &&  (destinationBitEnd <= 64) && (granularity <= 64)){
		// if 64 is fine then operate with 64 bit integers
		uint64 *destination64 = (uint64 *)destination;
		uint64 *source64      = (uint64 *)source;

		BSToBS(destination64,destinationBitShift,destinationBitSize,destinationIsSigned,source64,sourceBitShift,sourceBitSize,sourceIsSigned);
	}
    else if ((sourceBitEnd <= 128) && (destinationBitEnd <= 128) && (granularity <= 128)){
		// if 128 is fine then operate with double(uint64)
    	DoubleInteger<uint64> *destination128  = (DoubleInteger<uint64> *)destination;
    	DoubleInteger<uint64> *source128       = (DoubleInteger<uint64> *)source;
    	
    	BSToBS(destination128,destinationBitShift,destinationBitSize,destinationIsSigned,source128,sourceBitShift,sourceBitSize,sourceIsSigned);
	}
	
	sourceBitShift      += sourceBitSize;
	destinationBitShift += destinationBitSize;

	return true;
}

/** @Brief BitSet to Integer function.
  * @param dest is the location for the copy of the desired bit range.
  * @param source is a pointer to the bitSet source.
  * @param sourceBitShift defines the shift from which extract the number.
  * @param sourceBitSize is the desired size in bit for the number to extract from source.
  * @param sourceIsSigned specifies if the number to extract from source will be considered signed (true) or not (false).
  *
  *
  * Converts an integer of bitSize sourceBitSize located at address source and bitAddress sourceBitShift
  * into an integer of type T2.
  * T must be unsigned int of byte size power of 2 (8,16,32,64,128...) not 24 48 etc...
  * T determines the minimum number size used for the operations.
  * T=uint8 means that any number may be used
 */
template <typename T,typename T2>
static inline bool BitSetToInteger(
        T2 &                dest,
        T *&                source,
        uint8  &            sourceBitShift,
        uint8               sourceBitSize,
        bool                sourceIsSigned)
{

    // converts T2 into destination,destinationBitShift,destinationBitSize,destinationIsSigned
    T *destination = (T*) &dest;
    uint8 destinationBitShift = 0;
    uint8 destinationBitSize = sizeof(T2) *8;
    // detect if T2 has sign by seing if we can initialise a number negative
    bool destinationIsSigned = (((T2)-1) < 0); 

    return BitSetToBitSet(destination,destinationBitShift,destinationBitSize,destinationIsSigned,source,sourceBitShift,sourceBitSize,sourceIsSigned);
}

/** @Brief Integer to BitSet function.
  * @param destination is a pointer to the bitSet destination.
  * @param destinationBitShift is the desired shift in bit in destination variable.
  * @param destinationBitSize is the desired size in bit for the number to copy in destination.
  * @param destinationIsSigned specifies is the number in destination will be considered signed (true) or not (false).
  * @param source is the integer to copy.
  * 
  * Converts an integer of type T2 into an integer of bitSize destinationBitSize 
  * located at address destination and bitAddress destinationBitShift.
  * T must be unsigned int of byte size power of 2 (8,16,32,64,128...) not 24 48 etc...
  * T determines the minimum number size used for the operations.
  * T=uint8 means that any number may be used.
  */
template <typename T,typename T2>
static inline bool IntegerToBitSet(
        T *&                destination,
        uint8  &            destinationBitShift,
        uint8               destinationBitSize,
        bool                destinationIsSigned,
        T2 &               src)
{

    // converts T2 into source,sourceBitShift,sourceBitSize,sourceIsSigned
    T *source = (T*) &src;
    uint8 sourceBitShift = 0;
    uint8 sourceBitSize = sizeof(T2) *8;
    // detect if T2 has sign by seing if we can initialise a number negative
    bool sourceIsSigned = (((T2)-1) < 0); 

    return BitSetToBitSet(destination,destinationBitShift,destinationBitSize,destinationIsSigned,source,sourceBitShift,sourceBitSize,sourceIsSigned);
}


#endif
