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
 * @file AnyType.h
 * @brief A pointer to each type of data.
 */
#ifndef ANY_TYPE_H
#define ANY_TYPE_H

#include "GeneralDefinitions.h"
#include "TypeDescriptor.h"

#include <stdio.h>

/**
 * @brief AnyType class.
 *
 * This class provide a smart mechanism for the manipulation of each type.
 * It is composed by a void* pointer to the element and by a descriptor
 * which gives information about the type (the size, if it is a signed or
 * unsigned number, if it is constant, if it is an object or a native element, ecc). 
 * For each type a constructor can automatically built the relative AnyType 
 * object and this is very useful for example in the Printf function. 
*/
class AnyType{
public:
	
    /** A pointer to the element. */
	void * 				dataPointer;

    /**
     * The descriptor of the element. It gives
     * all the necessary information about the specific element
     * type. See TypeDescriptor. */
    TypeDescriptor      dataDescriptor;
 
	/** 
	 * Used for BitSet typer.
	 * The maximum range is 255.
	 */
    uint8               bitAddress;	
	
public:
    /** 
     * @brief Default constructor.
     * 
     * It is used as the terminated element in a Printf list.
     * The data descriptor is void, the pointer null. */
    AnyType(void){
		dataPointer 		= NULL; 
		bitAddress  		= 0;
		dataDescriptor 		= VoidType;
	}   	
    
    /**
     * @brief Copy constructor.
     * @param x is the AnyType to be copied in this.
    */
    AnyType(const AnyType &x){
		dataPointer 		= x.dataPointer; 
		bitAddress  		= x.bitAddress;
		dataDescriptor 		= x.dataDescriptor;
	}   	
    
    /** 
     * @brief Checks if the AnyType is empty.
     * @return true if the data descriptor is VoidType. */
    bool IsVoid() const{
    	return (dataDescriptor == VoidType);
    }

    /**
     * @brief Constructor by float.
     * @param i is a pointer to the float number. */
    AnyType(float &i){
		dataPointer = (void *) &i; 
		bitAddress  = 0;
		dataDescriptor = Float32Bit;
	}   
    
    /**
     * @brief Constructor by const float.
     * @param i is a pointer to the const float number.
     * 
     * The flag isConstant is settes to true. */
    AnyType(const float &i){
		dataPointer = (void *) &i; 
		bitAddress  = 0;
		dataDescriptor = Float32Bit;
		dataDescriptor.isConstant = true;
	}   	

    /**
     * @brief Constructor from a double.
     * @param i is a pointer to the data number.
     */
    AnyType(double &i){
		dataPointer = (void *) &i; 
		bitAddress  = 0;
		dataDescriptor = Float64Bit;
	}   	

    /**
     * @brief Constructor from a const double.
     * @param i is a pointer to the const double number. */
    AnyType(const double &i){
		dataPointer = (void *) &i; 
		bitAddress  = 0;
		dataDescriptor = Float64Bit;
		dataDescriptor.isConstant = true;
	}   	

    /**
     * @brief Constructor from an 8 bit integer constant.
     * @param i is the const 8 bit integer (const char). */
    AnyType(const int8 &i){
		dataPointer = (void *) &i; 
		bitAddress  = 0;
		dataDescriptor = SignedInteger8Bit;
		dataDescriptor.isConstant = true;
	}   	

    /**
     * @brief Constructor from an integer 8 bit.
     * @param i is the 8 bit integer (char). */
    AnyType(int8 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger8Bit;
	}   	

    /** 
     * @brief Constructor from an unsigned integer 8 bit.
     * @param i is the unsigned 8 bit integer. */
    AnyType(uint8 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = UnsignedInteger8Bit;
	}   	

    /**
     * @brief Constructor from an integer 16 bit.
     * @param i is the 16 bit integer. */
    AnyType(int16 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger16Bit;
	}   	

    /** 
     * @brief Constructor from an unsigned integer 16 bit.
     * @param i is the 16 bit unsigned integer. */
    AnyType(uint16 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = UnsignedInteger16Bit;
	}   	

    /** 
     * @brief Constructor from an unsigned integer 32 bit.
     * @param i is the unsigned 32 bit integer. */
    AnyType(uint32 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = UnsignedInteger32Bit;
	}   

    /** 
     * @brief Constructor from an unsigned constant integer 32 bit.
     * @param i is the constant unsigned 32 bit integer. */
    AnyType(const uint32 &i){
		dataPointer = (void *)&i;
		bitAddress  = 0;
		dataDescriptor = UnsignedInteger32Bit;
		dataDescriptor.isConstant = true;
	}   
    
    /** 
     * @brief Constructor from an integer 32 bit.
     * @param i is the 32 bit integer. */
    AnyType(int32 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger32Bit;
	}   
    
    /** 
     * @brief Constructor from a constant integer 32 bit.
     * @param i is the constant 32 bit integer. */
    AnyType(const int32 &i){
		dataPointer = (void *)&i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger32Bit;
		dataDescriptor.isConstant = true;
	}   
    
    /** 
     * @brief Constructor from an unsigned integer 64 bit.
     * @param i is the unsigned 64 bit integer. */
    AnyType(uint64 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = UnsignedInteger64Bit;
	}   
    
    /**
     * @brief Constructor from an integer 64 bit.
     * @param i is the 64 bit integer. */
    AnyType(int64 &i){
		dataPointer = &i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger64Bit;
	}   
    
    /** 
     * @brief Constructor from a constant 64 bit integer.
     * @param i is the 64 bit constant integer. */
    AnyType(const int64 &i){
		dataPointer = (void *)&i;
		bitAddress  = 0;
		dataDescriptor = SignedInteger64Bit;
		dataDescriptor.isConstant = true;
	}

    /**
     * @brief Constructor from a const void* pointer.
     * @param i is the const void* pointer. */
    AnyType(const void * p){
		dataPointer = (void *)p;
		bitAddress  = 0;
		dataDescriptor.type = TypeDescriptor::Pointer;
		dataDescriptor.isStructuredData=false;
		dataDescriptor.isConstant = true;
		dataDescriptor.size = 8 * sizeof(void *);	
	} 
   
    /**
     * @brief Constructor from a void* pointer.
     * @param i is the const void* pointer. */
    AnyType(void * p){
		dataPointer = (void *)p;
		bitAddress  = 0;
		dataDescriptor.type = TypeDescriptor::Pointer;
		dataDescriptor.isStructuredData=false;
		dataDescriptor.isConstant = false;
		dataDescriptor.size = 8 * sizeof(void *);
	}
    
    /**
     * @brief Constructor from a const char* string.
     * @param i is the const char pointer. */
    AnyType(const char *p){
		dataPointer = (void *)p; // we will either print the variable or the string
		bitAddress  = 0;
		dataDescriptor.type = TypeDescriptor::CCString;
		dataDescriptor.isStructuredData=false;
		dataDescriptor.isConstant = true;
	} 
};

/** definition of the void AnyType (empty contructor). */
static const AnyType voidAnyType;

#endif

