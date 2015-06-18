/**
 * @file SemCore.h
 * @brief Header file for class SemCore
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class SemCore
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEMCORE_H_
#define 		SEMCORE_H_
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 @brief This class provides basic facilities to use semaphores.
 */
class SemCore {

public:

    /**
     * @brief Copy constructor.
     * @param[in] s is the semaphore handle to be copied in this.
     */
    inline SemCore(HANDLE s);

    /**
     * @brief Constructor.
     */
    inline SemCore();

    /** @brief Set the semaphore handle to 0. */
    inline void Init();

    /**
     * @brief Set the semaphore.
     * @param[in] s is a pointer to the semaphore structure.
     */
    inline void Init(HANDLE s);

    /**
     * @brief Copy operator.
     * @param[in] s is another SemCore object and its value will be copied in this.
     */
    inline void operator=(const SemCore &s);

    /**
     * @brief return number associated to the semaphore.
     * @return the current value associated to the semaphore.
     */
    inline HANDLE Handle() const;

protected:
    /** The handle associated to the semaphore. */
    HANDLE semH;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

SemCore::SemCore(HANDLE s) {
    Init(s);
}

SemCore::SemCore() {
    Init();
}

void SemCore::Init() {
    semH = (HANDLE) 0;
}

void SemCore::Init(HANDLE s) {
    semH = s;
}

void SemCore::operator=(const SemCore &s) {
    semH = s.semH;
}

HANDLE SemCore::Handle() const {
    return semH;
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEMCORE_H_ */

