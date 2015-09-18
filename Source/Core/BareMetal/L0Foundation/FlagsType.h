/**
 * @file FlagsType.h
 * @brief Header file for class FlagsType
 * @date 06/07/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class FlagsType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FLAGSTYPE_H_
#define FLAGSTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Utility class for the definition and storage of flag values.
 *
 * @details This offers a MISRA compliant alternative to doing bitwise operations
 * over enumerations. The class is implemented as a decorator over a uint32 mask.
 */
class FlagsType {
public:
    /**
     * @brief Sets the initial value of the mask to zero.
     */
    FlagsType() {
        mask = 0u;
    }

    /**
     * @brief Sets the initial value of the mask to the value of initialMask.
     * @param initialMask the value of the mask
     */
    FlagsType(const uint32 initialMask) : mask(initialMask) {
    }

    /**
     * Returns the value of the mask.
     * @return the current value of the mask.
     */
    inline uint32 GetMask() const {
        return mask;
    }

    /**
     * Returns the value of the mask directly as uint32.
     * @return the current value of the mask.
     */
    inline operator uint32() const {
        return mask;
    }

    /**
     * Assigns the current value of the mask to the value of newFlag.
     * @return a FlagType with the mask value of newFlag.
     */
    inline FlagsType &operator=(const FlagsType &newFlag) {
        if (this != &newFlag) {
            mask = newFlag.GetMask();
        }
        return *this;
    }

private:
    /**
     * The internal mask which is decorated by the class.
     */
    uint32 mask;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#endif /* FLAGSTYPE_H_ */

