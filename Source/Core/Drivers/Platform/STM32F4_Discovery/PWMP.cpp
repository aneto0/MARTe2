/**
 * @file PWMP.c
 * @brief Source file for class PWMP
 * @date 17/08/2015
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
 * the class PWMP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PWM.h"
#include "Clock.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define ENABLE_TIM_CLOCK(instance)           __HAL_RCC_TIM##instance##_CLK_ENABLE()

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/* Timers-Pin table
 ******PP1  PP2  PP3  PP1  PP2  PP3  PP1  PP2  PP3  PP1  PP2  PP3
 TIM1  PA8  PE9       PA9  PE10      PA10 PE13      PA11 PE14
 TIM2  PA0  PA5  PA15 PA1  PB3       PA2  PB10      PA3  PB11
 TIM3  PA6  PB4  PC6  PA7  PB5  PC7  PB0  PC8       PB1  PC9
 TIM4  PB6  PD12      PB7  PD13      PB8  PD14      PB9  PD15
 TIM5  PA0  PH10      PA1  PH11      PA2  PH12      PA3  PI0
 TIM8  PC6  PI5       PC7  PI6       PC8  PI7       PC9  PI2
 TIM9  PA2  PE5       PA3  PE6
 TIM10 PB8  PF6
 TIM11 PB9  PF7
 TIM12 PB14 PH6       PB15 PH9
 TIM13 PA6  PF8
 TIM14 PA7  PF9
 */

bool PWM::Init(uint8 instance,
               uint16 frequency,
               uint8 dutyCycle,
               uint8 mode,
               uint8 channelMask) {

    /* Configure the system clock to 168 MHz */

    TIM_TypeDef* TIMInstance[] = { TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };

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

    // Note: The max period is 65535 (16 bit).
    // So if you want a little PWM frequency choose also a bigger prescaler value.

    /* Time base configuration */
    // prescaler=(sysclock/2)/(tick_frequency)-1
    //Cannot have a frequency > clock_frequency/10000

    //choose a standard tick period
    tickPeriod = 1999;

    //tickFrequency=(tickPeriod+1)*frequency
    prescalerValue = (uint16)(((Clock::GetClockFrequency()) / (2*(2000*frequency))) - 1);
    // period=tick_frequency/pwm_frequency -1

    /* Compute the prescaler value */
    /* Time base configuration */
    handle.Instance = TIMInstance[instance];

    //choose the period accordingly with the desired PWM frequency
    handle.Init.Period = tickPeriod;    //pwm frequency = 1!
    handle.Init.Prescaler = prescalerValue;
    handle.Init.ClockDivision = 0;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    if (HAL_TIM_PWM_Init(&handle) != HAL_OK) {
        return false;
    }

    uint16 pulse = (tickPeriod * dutyCycle) / 100;

    /* TIM PWM1 Mode configuration: Channel */
    /* Output Compare Timing Mode configuration: Channel1 */

    TIM_OC_InitTypeDef config;

    //clear on compare match
    if (mode == 1) {
        config.OCMode = TIM_OCMODE_PWM1;
        config.OCIdleState = TIM_CCx_ENABLE;
        config.Pulse = pulse;
        config.OCPolarity = TIM_OCPOLARITY_HIGH;
    }
    //set on compare match
    if (mode == 2) {
        config.OCMode = TIM_OCMODE_PWM2;
        config.Pulse = pulse;
        config.OCPolarity = TIM_OCPOLARITY_LOW;
    }

    /* Output Compare PWM Mode configuration: Channel1 */

    configChannelMask = channelMask;

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        if (HAL_TIM_PWM_ConfigChannel(&handle, &config, TIM_CHANNEL_1) != HAL_OK) {
            return false;
        }
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        if (HAL_TIM_PWM_ConfigChannel(&handle, &config, TIM_CHANNEL_2) != HAL_OK) {
            return false;
        }
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        if (HAL_TIM_PWM_ConfigChannel(&handle, &config, TIM_CHANNEL_3) != HAL_OK) {
            return false;
        }
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        if (HAL_TIM_PWM_ConfigChannel(&handle, &config, TIM_CHANNEL_4) != HAL_OK) {
            return false;
        }
    }
    return true;
}

bool PWM::Start(uint8 channelMask) {
    /*   if ((channelMask & configChannelMask) != channelMask) {
     return false;
     }*/

    if ((channelMask & configChannelMask) != channelMask) {
        return false;
    }

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        HAL_TIM_PWM_Start(&handle, TIM_CHANNEL_1);
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        HAL_TIM_PWM_Start(&handle, TIM_CHANNEL_2);
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        HAL_TIM_PWM_Start(&handle, TIM_CHANNEL_3);
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        HAL_TIM_PWM_Start(&handle, TIM_CHANNEL_4);
    }

    return true;
}

bool PWM::Stop(uint8 channelMask) {
    if ((channelMask & configChannelMask) != channelMask) {
        return false;
    }

    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        HAL_TIM_PWM_Stop(&handle, TIM_CHANNEL_1);
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        HAL_TIM_PWM_Stop(&handle, TIM_CHANNEL_2);
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        HAL_TIM_PWM_Stop(&handle, TIM_CHANNEL_3);
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        HAL_TIM_PWM_Stop(&handle, TIM_CHANNEL_4);
    }

    return true;
}

bool PWM::SetDutyCycle(uint8 dutyCycle,
                       uint8 channelMask) {

    if ((channelMask & configChannelMask) != channelMask) {
        return false;
    }

    uint16 pulse = (tickPeriod * dutyCycle) / 100;
    if ((channelMask & 0x1) == 0x1) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_1)) {
            return false;
        }
        //choose a compare set to a fraction of the period for the duty value
        __HAL_TIM_SET_COMPARE(&handle, TIM_CHANNEL_1, pulse);
    }

    /* Output Compare PWM Mode configuration: Channel2 */
    if ((channelMask & 0x2) == 0x2) {
        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_2)) {
            return false;
        }
        //choose a compare set to a fraction of the period for the duty value
        __HAL_TIM_SET_COMPARE(&handle, TIM_CHANNEL_2, pulse);
    }

    /* Output Compare PWM Mode configuration: Channel3 */
    if ((channelMask & 0x4) == 0x4) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_3)) {
            return false;
        }
        //choose a compare set to a fraction of the period for the duty value
        __HAL_TIM_SET_COMPARE(&handle, TIM_CHANNEL_3, pulse);
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if ((channelMask & 0x8) == 0x8) {

        if (!IS_TIM_CCX_INSTANCE(handle.Instance, TIM_CHANNEL_4)) {
            return false;
        }
        //choose a compare set to a fraction of the period for the duty value
        __HAL_TIM_SET_COMPARE(&handle, TIM_CHANNEL_4, pulse);
    }

    return true;
}

void PWM::SetFrequency(uint16 frequency) {

    uint16 tickFrequency = (Clock::GetClockFrequency()) / (2 * (prescalerValue + 1));

    // period=tick_frequency/pwm_frequency -1
    tickPeriod = (tickFrequency / frequency) - 1;

    /*to set another period*/
    __HAL_TIM_SET_AUTORELOAD(&handle, tickPeriod);

}

//to set a new value in the rim register at runtime.
//__HAL_TIM_SET_COUNTER(&handle, (TIM_ARR - MaxAcceleration) / 2);

