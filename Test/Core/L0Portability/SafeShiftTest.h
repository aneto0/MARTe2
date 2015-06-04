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
 * $Id$
 *
 **/
/**
 * @file SafeShiftTest.h
 * @brief Tests the Shift functions.*/ 

#ifndef SAFE_SHIFT_TEST_H
#define SAFE_SHIFT_TEST_H

#include "GeneralDefinitions.h"
#include "Shift.h"
class SafeShiftTest{

public:
  

    /** @brief Tests the logical right shift.
      * @return true if the shifts give the expected results. */
    bool TestLogicalRightShift() {
	
	int8 sbit8=(int8)0xf0;
	
	//shift greater than size
	if(Shift::LogicalSafeShift(sbit8, -8) != 0){
		return False;
	}

	//logical shift, no sign extension
	if(Shift::LogicalSafeShift(sbit8, -4) !=(int16) 0xf){
		return False;
	}
	
	

	uint8 ubit8=0xf0;

	//shift greater than size
	if(Shift::LogicalSafeShift(ubit8, -100) != 0){
		return False;
	}

	if(Shift::LogicalSafeShift(ubit8, -7) !=(int16) 0x1){
		return False;
	}

	return True;

    }


    /** @brief Tests the logical left shift.
      * @return true if the shifts give the expected results. */
    bool TestLogicalLeftShift(){
	int16 sbit16=(int16)0xf;

	if(Shift::LogicalSafeShift(sbit16, 16) != 0){
		return False;
	}

	if(Shift::LogicalSafeShift(sbit16, 13) !=(int16) 0xe000){
		return False;
	}



	uint16 ubit16=0xf;
	
	if(Shift::LogicalSafeShift(ubit16, 12) != 0xf000){
		return False;
	}

	if(Shift::LogicalSafeShift(ubit16, 18) != 0){
		return False;
	}

	return True;

    }


    /** @brief Tests the mathematic right shift. 
      * @return true if the shifts give the expected results.*/
    bool TestMathematicRightShift() {
	int32 sbit32=(int32)0xf0000000;
	
	if(Shift::MathematicSafeShift(sbit32, -32) != 0){
		return False;
	}

	//sign extension for negative number
	if(Shift::MathematicSafeShift(sbit32, -31) != -1){
		return False;
	}

	uint32 ubit32=0xf0000000;
	
	if(Shift::MathematicSafeShift(ubit32, -33) != 0){
		return False;
	}


	if(Shift::MathematicSafeShift(ubit32, -4) != 0xf000000){
		return False;
	}

	return True;
    }


    /**
     * @brief Tests the mathematic left shift.
     * @return true if the shifts give the expected results. */
    bool TestMathematicLeftShift() {
	

	int64 sbit64=(int64)0xf;
	
	if(Shift::MathematicSafeShift(sbit64, 64) != 0){
		return False;
	}


	if(Shift::MathematicSafeShift(sbit64, 60) != (int64)0xf000000000000000){
		return False;
	}

	uint64 ubit64=0xf;
	
	if(Shift::MathematicSafeShift(ubit64, 64) != 0){
		return False;
	}


	if(Shift::MathematicSafeShift(ubit64, 61) != 0xe000000000000000){
		return False;
	}



	return True;
    }
   

};

#endif
