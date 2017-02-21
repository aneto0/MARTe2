/**
 * @file BasicType.cpp
 * @brief Source file for class BasicType
 * @date Dec 15, 2016
 * @author fsartori
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

 * @details This source file contains the definition of all the methods for
 * the class BasicType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicType.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{




BasicObjectSize BasicObjectSizeFromBits(uint32 bits){
    BasicObjectSize  bos = SizeUnknown;
    if (bits > 32){
        if (bits > 128){
            if (bits == 256) bos = Size256bit;
            if (bits == 512) bos = Size512bit;
        } else {
            if (bits == 128) bos = Size128bit;
            if (bits == 64)  bos = Size64bit;
        }
    } else {
        if (bits == 32)      bos = Size32bit;
        if (bits == 16)      bos = Size16bit;
        if (bits == 8)       bos = Size8bit;
    }

    return bos;
}


uint32 BitsFromBasicObjectSize(BasicObjectSize bos){
    uint32 ret = 0;
    if (bos != SizeUnknown){
        ret = 4 << bos;
    }
    return ret;
}

#if 0
CCString basicTypeNames[]{
		(char8 *)"int",
		(char8 *)"uint",
		(char8 *)"float",
		(char8 *)"char",
		(char8 *)"void*",
		(char8 *)"int",
		(char8 *)"uint",
		(char8 *)"sstring",
		(char8 *)"undef1",
		(char8 *)"undef2",
		(char8 *)"undef3",
		(char8 *)"undef4",
		(char8 *)"Stream",
		(char8 *)"struct",
		(char8 *)"void",
		(char8 *)"invalid"
};


CCString BasicTypeName(uint32 bt){
	CCString ret = "??";
	if (bt < 16){
		ret = basicTypeNames[bt];
	}
	return ret;
}

#else

#define BTN_CASE(btName) case btName: { ret = #btName ; } break;
#define BTN_CASE_FULL(btName,alias) case btName: { ret = alias;} break;


CCString BasicTypeName(uint32 bt){
    CCString ret = "??";

    switch(bt){

    BTN_CASE_FULL(SignedInteger,"int")
    BTN_CASE_FULL(UnsignedInteger,"uint")
    BTN_CASE_FULL(Float,"float")
    BTN_CASE_FULL(Char,"char")
    BTN_CASE_FULL(SignedBitInteger,"int")
    BTN_CASE_FULL(UnsignedBitInteger,"uint")
    BTN_CASE(SString)
    BTN_CASE(Stream)
    BTN_CASE_FULL(StructuredDataInterface,"{struct}")
    BTN_CASE_FULL(Void,"void")
    BTN_CASE(Invalid)


    default:{

    }

    };

    return ret;
}


#endif


}

	
