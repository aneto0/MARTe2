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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 * @file 
 * @brief Common functionalities shared by all semaphore types
 */
#ifndef SEM_CORE_H
#define SEM_CORE_H

#include "GeneralDefinitions.h"
/**
 @brief This class provides basic facilities to use semaphores.
 */
class SemCore {
protected:
    /** A number associated to the semaphore. */
    HANDLE semH;
public:
    /** @brief Set the semaphore handle to 0. */
    void Init() {
        semH = (HANDLE) 0;
    }
    /** @brief Set the semaphore.
      * @param s is a pointer to the semaphore structure. */
    void Init(HANDLE s) {
        semH = s;
    }

    /** @brief Constructor.
      * @param s is a pointer to the semaphore structure. */
    SemCore(HANDLE s) {
        Init(s);
    }

    /** @brief Constructor. */
    SemCore() {
        Init();
    }

    /** @brief Assign operator.
      * @param s is another SemCore object and its value will be copied in this.*/
    void operator=(SemCore &s) {
        semH = s.semH;
    }

    /** @brief return number associated to the semaphore.
      * @return the current value associated to the semaphore. */
    inline HANDLE Handle() {
        return semH;
    }
};
#endif

