/**
 * @file SemCore.h
 * @brief Header file for class SemCore
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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
#define SEMCORE_H_
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
 * @brief This class provides basic facilities to use semaphores.
 */
class SemCore {

public:

    /**
     * @brief Copy constructor.
     * @param[in] s is the semaphore handle to be copied in this.
     */
    SemCore(const HANDLE const s);

    /**
     * @brief Constructor.
     */
    SemCore();

    /**
     * @brief Destructor that can be overridden by inherited classes
     */
    virtual ~SemCore();

    /**
     * @brief Set the semaphore handle to 0.
     */
    void Init();

    /**
     * @brief Set the semaphore.
     * @param[in] s is a pointer to the semaphore structure.
     */
    void Init(const HANDLE const s);

    /**
     * @brief return number associated to the semaphore.
     * @return the current value associated to the semaphore.
     */
    const HANDLE Handle() const;

private:
    /** The handle associated to the semaphore. */
    const HANDLE semH;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEMCORE_H_ */

