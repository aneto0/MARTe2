/**
 * @file Units.h
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

#ifndef UNITS_H_
#define UNITS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "TypeCharacteristics.h"

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
	 * The seconds
	 */
	static const class sClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return 1.0; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return 1.0; }
    } s;

	/**
	 * The milli seconds
	 */
    static const class msClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return 1.0E-3; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return 1.0E3; }
    } ms;

	/**
	 * The micro seconds
	 */
    static const class usClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return 1.0E-6; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return 1.0E6; }
    } us;

	/**
	 * The nano seconds
	 */
    static const class nsClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return 1.0E-9; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return 1.0E9; }
    } ns;

    /**
     * The raw value
     */
    static const class rawClass{
    	public:
    	/**
    	 * The value of one unit in seconds
    	 */
    	inline double GetScale() const{ return 0.0; }
    	/**
    	 * How many units to make a second
    	 */
    	inline double GetScaleInv() const{ return 0.0; }
    } raw;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_UNITS_H_ */
