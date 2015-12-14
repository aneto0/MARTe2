/**
 * @file FractionalInteger.h
 * @brief Header file for class FractionalInteger
 * @date 28/08/2015
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

 * @details This header file contains the declaration of the class FractionalInteger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FRACTIONALINTEGER_H_
#define FRACTIONALINTEGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "TypeCharacteristics.h"
#include "TemplateParametersVerificator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An integer wrapper class parameterisable by bit size.
 *
 * @details This template class is an helper used to define integer types with non-standard bit sizes, such as uint3 or uint63.\n
 * The template requires two parameters: a baseType, i.e. the standard type which is used as a base for the new type; and numberOfBits, i.e.
 * the actual size of the new type.
 *
 * Values which are greater than the maximum (or smaller than the minimum) allowed in a numberOfBits size will be saturated, thus avoiding
 * any kind of rollover problem.
 *
 * The cast operator is overridden so that the class will behave as a basic type for mathematical operations.
 *
 * @pre numberOfBits must be less than the number of bits of baseType (e.g. less than 64 for a uint64)
 */
/*lint -e{1789} Template constructor cannot be a copy constructor. Justification: the constructor must not be used as a copy constructor
 * but as a constructor by integer number. */
/*lint -etemplate(1016, 948, 1790, 1942, 685, 944, 845) No code is truly generated. This strategy is used to guarantee that certain rules
 * about the template are guaranteed at compilation time.
 * Operator will always evaluate to true if the template is correctly used, otherwise it will not compile (which the objective)*/
template<typename baseType, uint8 numberOfBits>
class FractionalInteger: public TemplateParametersVerificator<((sizeof(baseType)*8u)>=numberOfBits) && (numberOfBits > 0u)> {

public:

    /**
     * @brief Returns the number of bits associated to this FractionalInteger.
     * @return the number of bits.
     */
    static inline const uint8 GetNumberOfBits();

    /**
     * @brief Default constructor.
     * @details Sets the integer value to zero.
     */
    FractionalInteger();

    /**
     * @brief Constructor by value.
     * @details The argument input could be another integer type.
     * If the input value is greater than the maximum allowed value, the value is set to the maximum value.
     * If the input value is lower than the minimum allowed value, the value is set to the minimum value.
     * @param[in] input is the desired integer value.
     */
    template<typename inputType> FractionalInteger(inputType input);

    /**
     * @brief Cast to integer.
     * @details Thanks to this operator this object can be treated as an integer.
     * @return the value of this integer.
     */
    inline operator baseType() const;

private:

    /**
     * The value of the integer.
     */
    baseType value;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename baseType, uint8 numberOfBits>
const uint8 FractionalInteger<baseType, numberOfBits>::GetNumberOfBits() {
    return numberOfBits;
}

template<typename baseType, uint8 numberOfBits>
FractionalInteger<baseType, numberOfBits>::FractionalInteger() {
    value = static_cast<baseType>(0);
}

template<typename baseType, uint8 numberOfBits>
template<typename inputType>
FractionalInteger<baseType, numberOfBits>::FractionalInteger(inputType input) {
    value = SaturateInteger<baseType, inputType, numberOfBits>(input);
}

template<typename baseType, uint8 numberOfBits>
FractionalInteger<baseType, numberOfBits>::operator baseType() const {
    return value;
}

/**
 * Unsigned 1 bit integer.
 */
typedef FractionalInteger<uint8, 1u> uint1;

/**
 * Unsigned 2 bit integer.
 */
typedef FractionalInteger<uint8, 2u> uint2;

/**
 * Unsigned 3 bit integer.
 */
typedef FractionalInteger<uint8, 3u> uint3;

/**
 * Unsigned 4 bit integer.
 */
typedef FractionalInteger<uint8, 4u> uint4;

/**
 * Unsigned 5 bit integer.
 */
typedef FractionalInteger<uint8, 5u> uint5;

/**
 * Unsigned 6 bit integer.
 */
typedef FractionalInteger<uint8, 6u> uint6;

/**
 * Unsigned 7 bit integer.
 */
typedef FractionalInteger<uint8, 7u> uint7;

/**
 * Unsigned 9 bit integer.
 */
typedef FractionalInteger<uint16, 9u> uint9;

/**
 * Unsigned 10 bit integer.
 */
typedef FractionalInteger<uint16, 10u> uint10;

/**
 * Unsigned 11 bit integer.
 */
typedef FractionalInteger<uint16, 11u> uint11;

/**
 * Unsigned 12 bit integer.
 */
typedef FractionalInteger<uint16, 12u> uint12;

/**
 * Unsigned 13 bit integer.
 */
typedef FractionalInteger<uint16, 13u> uint13;

/**
 * Unsigned 14 bit integer.
 */
typedef FractionalInteger<uint16, 14u> uint14;

/**
 * Unsigned 15 bit integer.
 */
typedef FractionalInteger<uint16, 15u> uint15;

/**
 * Unsigned 17 bit integer.
 */
typedef FractionalInteger<uint32, 17u> uint17;

/**
 * Unsigned 18 bit integer.
 */
typedef FractionalInteger<uint32, 18u> uint18;

/**
 * Unsigned 19 bit integer.
 */
typedef FractionalInteger<uint32, 19u> uint19;

/**
 * Unsigned 20 bit integer.
 */
typedef FractionalInteger<uint32, 20u> uint20;

/**
 * Unsigned 21 bit integer.
 */
typedef FractionalInteger<uint32, 21u> uint21;

/**
 * Unsigned 21 bit integer.
 */
typedef FractionalInteger<uint32, 22u> uint22;

/**
 * Unsigned 23 bit integer.
 */
typedef FractionalInteger<uint32, 23u> uint23;

/**
 * Unsigned 24 bit integer.
 */
typedef FractionalInteger<uint32, 24u> uint24;

/**
 * Unsigned 25 bit integer.
 */
typedef FractionalInteger<uint32, 25u> uint25;

/**
 * Unsigned 26 bit integer.
 */
typedef FractionalInteger<uint32, 26u> uint26;

/**
 * Unsigned 27 bit integer.
 */
typedef FractionalInteger<uint32, 27u> uint27;

/**
 * Unsigned 28 bit integer.
 */
typedef FractionalInteger<uint32, 28u> uint28;

/**
 * Unsigned 29 bit integer.
 */
typedef FractionalInteger<uint32, 29u> uint29;

/**
 * Unsigned 30 bit integer.
 */
typedef FractionalInteger<uint32, 30u> uint30;

/**
 * Unsigned 31 bit integer.
 */
typedef FractionalInteger<uint32, 31u> uint31;

/**
 * Unsigned 33 bit integer.
 */
typedef FractionalInteger<uint64, 33u> uint33;

/**
 * Unsigned 34 bit integer.
 */
typedef FractionalInteger<uint64, 34u> uint34;

/**
 * Unsigned 35 bit integer.
 */
typedef FractionalInteger<uint64, 35u> uint35;

/**
 * Unsigned 36 bit integer.
 */
typedef FractionalInteger<uint64, 36u> uint36;

/**
 * Unsigned 37 bit integer.
 */
typedef FractionalInteger<uint64, 37u> uint37;

/**
 * Unsigned 38 bit integer.
 */
typedef FractionalInteger<uint64, 38u> uint38;

/**
 * Unsigned 39 bit integer.
 */
typedef FractionalInteger<uint64, 39u> uint39;

/**
 * Unsigned 40 bit integer.
 */
typedef FractionalInteger<uint64, 40u> uint40;

/**
 * Unsigned 41 bit integer.
 */
typedef FractionalInteger<uint64, 41u> uint41;

/**
 * Unsigned 42 bit integer.
 */
typedef FractionalInteger<uint64, 42u> uint42;

/**
 * Unsigned 43 bit integer.
 */
typedef FractionalInteger<uint64, 43u> uint43;

/**
 * Unsigned 44 bit integer.
 */
typedef FractionalInteger<uint64, 44u> uint44;

/**
 * Unsigned 45 bit integer.
 */
typedef FractionalInteger<uint64, 45u> uint45;

/**
 * Unsigned 46 bit integer.
 */
typedef FractionalInteger<uint64, 46u> uint46;

/**
 * Unsigned 47 bit integer.
 */
typedef FractionalInteger<uint64, 47u> uint47;

/**
 * Unsigned 48 bit integer.
 */
typedef FractionalInteger<uint64, 48u> uint48;

/**
 * Unsigned 49 bit integer.
 */
typedef FractionalInteger<uint64, 49u> uint49;

/**
 * Unsigned 50 bit integer.
 */
typedef FractionalInteger<uint64, 50u> uint50;

/**
 * Unsigned 51 bit integer.
 */
typedef FractionalInteger<uint64, 51u> uint51;

/**
 * Unsigned 52 bit integer.
 */
typedef FractionalInteger<uint64, 52u> uint52;

/**
 * Unsigned 53 bit integer.
 */
typedef FractionalInteger<uint64, 53u> uint53;

/**
 * Unsigned 54 bit integer.
 */
typedef FractionalInteger<uint64, 54u> uint54;

/**
 * Unsigned 55 bit integer.
 */
typedef FractionalInteger<uint64, 55u> uint55;

/**
 * Unsigned 56 bit integer.
 */
typedef FractionalInteger<uint64, 56u> uint56;

/**
 * Unsigned 57 bit integer.
 */
typedef FractionalInteger<uint64, 57u> uint57;

/**
 * Unsigned 58 bit integer.
 */
typedef FractionalInteger<uint64, 58u> uint58;

/**
 * Unsigned 59 bit integer.
 */
typedef FractionalInteger<uint64, 59u> uint59;

/**
 * Unsigned 60 bit integer.
 */
typedef FractionalInteger<uint64, 60u> uint60;

/**
 * Unsigned 61 bit integer.
 */
typedef FractionalInteger<uint64, 61u> uint61;

/**
 * Unsigned 62 bit integer.
 */
typedef FractionalInteger<uint64, 62u> uint62;

/**
 * Unsigned 63 bit integer.
 */
typedef FractionalInteger<uint64, 63u> uint63;

/**
 * Signed 2 bit integer.
 */
typedef FractionalInteger<int8, 2u> int2;

/**
 * Signed 3 bit integer.
 */
typedef FractionalInteger<int8, 3u> int3;

/**
 * Signed 4 bit integer.
 */
typedef FractionalInteger<int8, 4u> int4;

/**
 * Signed 5 bit integer.
 */
typedef FractionalInteger<int8, 5u> int5;

/**
 * Signed 6 bit integer.
 */
typedef FractionalInteger<int8, 6u> int6;

/**
 * Signed 7 bit integer.
 */
typedef FractionalInteger<int8, 7u> int7;

/**
 * Signed 9 bit integer.
 */
typedef FractionalInteger<int16, 9u> int9;

/**
 * Signed 10 bit integer.
 */
typedef FractionalInteger<int16, 10u> int10;

/**
 * Signed 11 bit integer.
 */
typedef FractionalInteger<int16, 11u> int11;

/**
 * Signed 12 bit integer.
 */
typedef FractionalInteger<int16, 12u> int12;

/**
 * Signed 13 bit integer.
 */
typedef FractionalInteger<int16, 13u> int13;

/**
 * Signed 14 bit integer.
 */
typedef FractionalInteger<int16, 14u> int14;

/**
 * Signed 15 bit integer.
 */
typedef FractionalInteger<int16, 15u> int15;

/**
 * Signed 17 bit integer.
 */
typedef FractionalInteger<int32, 17u> int17;

/**
 * Signed 18 bit integer.
 */
typedef FractionalInteger<int32, 18u> int18;

/**
 * Signed 19 bit integer.
 */
typedef FractionalInteger<int32, 19u> int19;

/**
 * Signed 20 bit integer.
 */
typedef FractionalInteger<int32, 20u> int20;

/**
 * Signed 21 bit integer.
 */
typedef FractionalInteger<int32, 21u> int21;

/**
 * Signed 22 bit integer.
 */
typedef FractionalInteger<int32, 22u> int22;

/**
 * Signed 23 bit integer.
 */
typedef FractionalInteger<int32, 23u> int23;

/**
 * Signed 24 bit integer.
 */
typedef FractionalInteger<int32, 24u> int24;

/**
 * Signed 25 bit integer.
 */
typedef FractionalInteger<int32, 25u> int25;

/**
 * Signed 26 bit integer.
 */
typedef FractionalInteger<int32, 26u> int26;

/**
 * Signed 27 bit integer.
 */
typedef FractionalInteger<int32, 27u> int27;

/**
 * Signed 28 bit integer.
 */
typedef FractionalInteger<int32, 28u> int28;

/**
 * Signed 29 bit integer.
 */
typedef FractionalInteger<int32, 29u> int29;

/**
 * Signed 30 bit integer.
 */
typedef FractionalInteger<int32, 30u> int30;

/**
 * Signed 31 bit integer.
 */
typedef FractionalInteger<int32, 31u> int31;

/**
 * Signed 33 bit integer.
 */
typedef FractionalInteger<int64, 33u> int33;

/**
 * Signed 34 bit integer.
 */
typedef FractionalInteger<int64, 34u> int34;

/**
 * Signed 35 bit integer.
 */
typedef FractionalInteger<int64, 35u> int35;

/**
 * Signed 36 bit integer.
 */
typedef FractionalInteger<int64, 36u> int36;

/**
 * Signed 37 bit integer.
 */
typedef FractionalInteger<int64, 37u> int37;

/**
 * Signed 38 bit integer.
 */
typedef FractionalInteger<int64, 38u> int38;

/**
 * Signed 39 bit integer.
 */
typedef FractionalInteger<int64, 39u> int39;

/**
 * Signed 40 bit integer.
 */
typedef FractionalInteger<int64, 40u> int40;

/**
 * Signed 41 bit integer.
 */
typedef FractionalInteger<int64, 41u> int41;

/**
 * Signed 42 bit integer.
 */
typedef FractionalInteger<int64, 42u> int42;

/**
 * Signed 43 bit integer.
 */
typedef FractionalInteger<int64, 43u> int43;

/**
 * Signed 44 bit integer.
 */
typedef FractionalInteger<int64, 44u> int44;

/**
 * Signed 45 bit integer.
 */
typedef FractionalInteger<int64, 45u> int45;

/**
 * Signed 46 bit integer.
 */
typedef FractionalInteger<int64, 46u> int46;

/**
 * Signed 47 bit integer.
 */
typedef FractionalInteger<int64, 47u> int47;

/**
 * Signed 48 bit integer.
 */
typedef FractionalInteger<int64, 48u> int48;

/**
 * Signed 49 bit integer.
 */
typedef FractionalInteger<int64, 49u> int49;

/**
 * Signed 50 bit integer.
 */
typedef FractionalInteger<int64, 50u> int50;

/**
 * Signed 51 bit integer.
 */
typedef FractionalInteger<int64, 51u> int51;

/**
 * Signed 52 bit integer.
 */
typedef FractionalInteger<int64, 52u> int52;

/**
 * Signed 53 bit integer.
 */
typedef FractionalInteger<int64, 53u> int53;

/**
 * Signed 54 bit integer.
 */
typedef FractionalInteger<int64, 54u> int54;

/**
 * Signed 55 bit integer.
 */
typedef FractionalInteger<int64, 55u> int55;

/**
 * Signed 56 bit integer.
 */
typedef FractionalInteger<int64, 56u> int56;

/**
 * Signed 57 bit integer.
 */
typedef FractionalInteger<int64, 57u> int57;

/**
 * Signed 58 bit integer.
 */
typedef FractionalInteger<int64, 58u> int58;

/**
 * Signed 59 bit integer.
 */
typedef FractionalInteger<int64, 59u> int59;

/**
 * Signed 60 bit integer.
 */
typedef FractionalInteger<int64, 60u> int60;

/**
 * Signed 61 bit integer.
 */
typedef FractionalInteger<int64, 61u> int61;

/**
 * Signed 62 bit integer.
 */
typedef FractionalInteger<int64, 62u> int62;

/**
 * Signed 63 bit integer.
 */
typedef FractionalInteger<int64, 63u> int63;



/**
 * Class UUID for the Class Registry Database.
 */
typedef uint14 ClassUID;
}

#endif /* FRACTIONALINTEGER_H_ */
