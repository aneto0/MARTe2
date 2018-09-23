/**
 * @file Ticks.h
 * @brief Header file for class AnyType
 * @date 17 Sep 2018
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

#ifndef TICKS_H_
#define TICKS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "OSInitializer.h"
#include "TimeType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * A collection of classes used to specify and characterize units
 * They all have the same interface but are not virtual as they are used in templates
 */
namespace Units{

	/**
	 * The CPU tick
	 */
    static class ticksClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return OSInitializer::period; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return OSInitializer::frequency; }
    } ticks;

};

/**
 * Definition of the ticks time type.
 * Usable range depends on CPU clock
 * Resolution  depends on CPU clock
 */
typedef TimeType<uint64,Units::ticksClass> Ticks;

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_TICKS_H_ */
