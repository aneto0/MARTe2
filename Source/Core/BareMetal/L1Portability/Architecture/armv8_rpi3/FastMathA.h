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

//0-100
//accuracy 100 points per pi/2
static float sinLookupTable[] = { 0, 0.0157, 0.0314, 0.0471, 0.0628, 0.0785,
		0.0941, 0.1097, 0.1253, 0.1409, 0.1564, 0.1719, 0.1874, 0.2028, 0.2181,
		0.2334, 0.2487, 0.2639, 0.2790, 0.2940, 0.3090, 0.3239, 0.3387, 0.3535,
		0.3681, 0.3827, 0.3971, 0.4115, 0.4258, 0.4399, 0.4540, 0.4679, 0.4818,
		0.4955, 0.5090, 0.5225, 0.5358, 0.5490, 0.5621, 0.5750, 0.5878, 0.6004,
		0.6129, 0.6252, 0.6374, 0.6494, 0.6613, 0.6730, 0.6845, 0.6959, 0.7071,
		0.7181, 0.7290, 0.7396, 0.7501, 0.7604, 0.7705, 0.7804, 0.7902, 0.7997,
		0.8090, 0.8181, 0.8271, 0.8358, 0.8443, 0.8526, 0.8607, 0.8686, 0.8763,
		0.8838, 0.8910, 0.8980, 0.9048, 0.9114, 0.9178, 0.9239, 0.9298, 0.9354,
		0.9409, 0.9461, 0.9511, 0.9558, 0.9603, 0.9646, 0.9686, 0.9724, 0.9759,
		0.9792, 0.9823, 0.9851, 0.9877, 0.9900, 0.9921, 0.9940, 0.9956, 0.9969,
		0.9980, 0.9989, 0.9995, 0.9999, 1.0 };

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
