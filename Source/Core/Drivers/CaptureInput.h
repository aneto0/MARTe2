/**
 * @file CaptureInput.h
 * @brief Header file for class CaptureInput
 * @date 26/08/2015
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

 * @details This header file contains the declaration of the class CaptureInput
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CAPTUREINPUT_H_
#define CAPTUREINPUT_H_

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
#define SET_CAPTURE_INPUT_CALLBACK_FUNCTION(n, channel, function, timHandlePtr) \
    extern "C"{ \
        void TIM##n##_IRQHandler(void) { \
            __HAL_TIM_CLEAR_IT(timHandlePtr, TIM_IT_CC##channel); \
            timHandlePtr->Channel = HAL_TIM_ACTIVE_CHANNEL_##channel; \
            uint32 TIM_CCMRArr[]={0, 0x0003, 0x0300, 0x0003, 0x0300}; \
            uint32 CCMRArr[]={0, timHandlePtr->Instance->CCMR1,timHandlePtr->Instance->CCMR1, timHandlePtr->Instance->CCMR2,timHandlePtr->Instance->CCMR2}; \
            if((CCMRArr[channel] & TIM_CCMRArr[channel]) != 0x00) \
               function(); \
            timHandlePtr->Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED; \
        } \
     }

class CaptureInput {
public:

    bool Init(uint8 instance, bool mode, uint8 channelMask, bool initialize=true);

    bool Start(uint8 channelMask);

    bool Stop(uint8 channelMask);

    void DeInit();

    CapInHandle *GetHandlePtr();

private:
    CapInHandle handle;
    uint8 configChannelMask;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CAPTUREINPUT_H_ */

