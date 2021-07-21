/**
 * @file FastMathA.h
 * @brief Header file for module FastMathA
 * @date 26/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the module FastMathA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATHA_H_
#define FASTMATHA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <math.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace FastMath {

inline float Sin(const float angle) {

    return __builtin_sin(angle);
    /*


	float result = 0.;
	float angleX = angle;
	while (angleX < 0.f) {
		angleX += (2 * M_PI);
	}
	uint32 rounds = angleX / (2. * M_PI);
	angleX -= rounds * (2. * M_PI);

	uint32 quadrant = angleX / (M_PI / 2.);

	if (quadrant == 0u) {
		float remain = angleX;
		uint32 index = (uint32)((remain / (M_PI/2.)) * 100.);
		result = sinLookupTable[index];
	} else if (quadrant == 1u) {
		float remain = angleX - (M_PI/2.);
		uint32 index = (uint32)((remain / (M_PI/2.)) * 100.);
		result = sinLookupTable[100 - index];
	} else if (quadrant == 2u) {
		float remain = angleX - M_PI;
		uint32 index = (uint32)((remain / (M_PI/2.)) * 100.);
		result = -sinLookupTable[index];
	} else if (quadrant == 3u) {
		float remain = angleX - (1.5 * M_PI);
		uint32 index = (uint32)((remain / (M_PI/2.)) * 100.);
		result = -sinLookupTable[100 - index];
	}
	return result;*/
}

inline float Cos(const float angle) {
    return __builtin_cos(angle);
    /*
	return Sin(angle + (M_PI / 2));*/
}

inline int32 FloatToInt(const float input) {
	return (int32) input;
}

}

}
#endif /* FASTMATHA_H_ */
