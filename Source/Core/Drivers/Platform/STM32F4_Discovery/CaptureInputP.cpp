/**
 * @file CaptureInputP.cpp
 * @brief Source file for class CaptureInputP
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

 * @details This source file contains the definition of all the methods for
 * the class CaptureInputP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CaptureInput.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define ENABLE_TIM_CLOCK(instance)           __HAL_RCC_TIM##instance##_CLK_ENABLE()

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool CaptureInput::Init(uint8 instance,
                        bool mode,
                        uint8 channelMask,
                        bool initialize) {

    TIM_TypeDef* TIMInstance[] = { TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };

    // instance 1 --> TIM1
    instance--;
    if (instance > 13) {
        instance = 13;
    }
    /* Time base configuration */
    handle.Instance = TIMInstance[instance];

    if (initialize) {
        switch (instance) {
        case 0:
            ENABLE_TIM_CLOCK(1);
            break;
        case 1:
            ENABLE_TIM_CLOCK(2);
            break;
        case 2:
            ENABLE_TIM_CLOCK(3);
            break;
        case 3:
            ENABLE_TIM_CLOCK(4);
            break;
        case 4:
            ENABLE_TIM_CLOCK(5);
            break;
        case 5:
            ENABLE_TIM_CLOCK(6);
            break;
        case 6:
            ENABLE_TIM_CLOCK(7);
            break;
        case 7:
            ENABLE_TIM_CLOCK(8);
            break;
        case 8:
            ENABLE_TIM_CLOCK(9);
            break;
        case 9:
            ENABLE_TIM_CLOCK(10);
            break;
        case 10:
            ENABLE_TIM_CLOCK(11);
            break;
        case 11:
            ENABLE_TIM_CLOCK(12);
            break;
        case 12:
            ENABLE_TIM_CLOCK(13);
            break;
        case 13:
            ENABLE_TIM_CLOCK(14);
            break;

        }

        handle.Init.Period = 0xFFFF;
        handle.Init.Prescaler = 0;
        handle.Init.ClockDivision = 0;
        handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        if (HAL_TIM_IC_Init(&handle) != HAL_OK) {
            return false;
        }
    }

    TIM_IC_InitTypeDef config;

    if (mode) {
        // interrup on rising
        config.ICPolarity = TIM_ICPOLARITY_RISING;
        config.ICSelection = TIM_ICSELECTION_DIRECTTI;
    }
    else {

        //interrupt on falling
        config.ICPolarity = TIM_ICPOLARITY_FALLING;
        config.ICSelection = TIM_ICSELECTION_INDIRECTTI;
    }

    config.ICPrescaler = 0;
    config.ICFilter = 0;

    configChannelMask = channelMask;

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        if (HAL_TIM_IC_ConfigChannel(&handle, &config, TIM_CHANNEL_1) != HAL_OK) {
            return false;
        }
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        if (HAL_TIM_IC_ConfigChannel(&handle, &config, TIM_CHANNEL_2) != HAL_OK) {
            return false;
        }
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        if (HAL_TIM_IC_ConfigChannel(&handle, &config, TIM_CHANNEL_3) != HAL_OK) {
            return false;
        }
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        if (HAL_TIM_IC_ConfigChannel(&handle, &config, TIM_CHANNEL_4) != HAL_OK) {
            return false;
        }
    }
    return true;

}

bool CaptureInput::Start(uint8 channelMask) {

    if ((channelMask & configChannelMask) != channelMask) {
        return false;
    }

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        HAL_TIM_IC_Start_IT(&handle, TIM_CHANNEL_1);
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        HAL_TIM_IC_Start_IT(&handle, TIM_CHANNEL_2);
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        HAL_TIM_IC_Start_IT(&handle, TIM_CHANNEL_3);
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        HAL_TIM_IC_Start_IT(&handle, TIM_CHANNEL_4);
    }

    return true;
}

bool CaptureInput::Stop(uint8 channelMask) {
    if ((channelMask & configChannelMask) != channelMask) {
        return false;
    }

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        HAL_TIM_IC_Stop_IT(&handle, TIM_CHANNEL_1);
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        HAL_TIM_IC_Stop_IT(&handle, TIM_CHANNEL_2);
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        HAL_TIM_IC_Stop_IT(&handle, TIM_CHANNEL_3);
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        HAL_TIM_IC_Stop_IT(&handle, TIM_CHANNEL_4);
    }

    return true;
}

void CaptureInput::DeInit() {
    HAL_TIM_IC_DeInit(&handle);
}

CapInHandle *CaptureInput::GetHandlePtr() {
    return &handle;
}

