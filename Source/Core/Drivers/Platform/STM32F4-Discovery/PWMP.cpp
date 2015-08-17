/**
 * @file PWMP.c
 * @brief Source file for class PWMP
 * @date 17/ago/2015
 * @author pc
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/* Timers-Pin table
 PP1  PP2  PP3  PP1  PP2  PP3  PP1  PP2  PP3  PP1  PP2  PP3
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

bool PWM::Init() {

    // Note: The max period is 65535 (16 bit).
    // So if you want a little PWM frequency choose also a bigger prescaler value.

    // 2000 is the timer tick frequency
    // frequency=(sysclock/2)/(tick_frequency)+1

    uint16 PrescalerValue = ((SystemCoreClock / 2) / 2000) - 1;

    /* Time base configuration */
    handle.Instance = TIM4;

    //choose the period accordingly with the desired PWM frequency
    // period=tick_frequency/pwm_frequency -1
    handle.Init.Period = 1999;    //pwm frequency = 1!
    handle.Init.Prescaler = PrescalerValue;
    handle.Init.ClockDivision = 0;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    if (HAL_TIM_PWM_Init(&handle) != HAL_OK) {
        return false;
    }

    /* TIM PWM1 Mode configuration: Channel */
    /* Output Compare Timing Mode configuration: Channel1 */
    sConfigTim4.OCMode = TIM_OCMODE_PWM1;
    sConfigTim4.OCIdleState = TIM_CCx_ENABLE;
    sConfigTim4.Pulse = TIM_CCR;
    sConfigTim4.OCPolarity = TIM_OCPOLARITY_HIGH;

    /* Output Compare PWM1 Mode configuration: Channel1 */
    if (HAL_TIM_PWM_ConfigChannel(&handle, &sConfigTim4, TIM_CHANNEL_1) != HAL_OK) {
        return false;
    }

    /* Output Compare PWM1 Mode configuration: Channel2 */
    if (HAL_TIM_PWM_ConfigChannel(&handle, &sConfigTim4, TIM_CHANNEL_2) != HAL_OK) {
        return false;
    }

    /* Output Compare PWM1 Mode configuration: Channel3 */
    if (HAL_TIM_PWM_ConfigChannel(&handle, &sConfigTim4, TIM_CHANNEL_3) != HAL_OK) {
        return false;
    }
    /* Output Compare PWM1 Mode configuration: Channel4 */
    if (HAL_TIM_PWM_ConfigChannel(&handle, &sConfigTim4, TIM_CHANNEL_4) != HAL_OK) {
        return false;
    }

    //to set a new value in the rim register at runtime.
    __HAL_TIM_SET_COUNTER(&htim4,(TIM_ARR-MaxAcceleration)/2);

    //choose a compare set to a fraction of the period for the duty value
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, period / 2);

    /*to set another period*/
    __HAL_TIM_SET_AUTORELOAD(&htim4, period);

    /* Enable TIM4 Capture Compare Channel1 */
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

    return true;
}

