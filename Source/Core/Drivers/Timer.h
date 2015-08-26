/**
 * @file Timer.h
 * @brief Header file for class Timer
 * @date 25/08/2015
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

 * @details This header file contains the declaration of the class Timer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TIMER_H_
#define TIMER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_PLATFORM(PLATFORM,DriversDefinitionsP.h)
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

// Define your callback function, then use this macro in your code which
// automatically wraps your function with the default callback.
#define SET_TIMER_CALLBACK_FUNCTION(n, function, timHandlePtr) \
    extern "C"{\
        void TIM##n##_IRQHandler(void) {\
            __HAL_TIM_CLEAR_IT(timHandlePtr, TIM_IT_UPDATE); \
            function(); \
        }\
    }

class Timer {

public:

    bool Init(uint8 instance, uint16 frequency);

    bool Start();

    void Stop();

    TimerHandle* GetHandlePtr();
private:
    TimerHandle handle;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TIMER_H_ */

