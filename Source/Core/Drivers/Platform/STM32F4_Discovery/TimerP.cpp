/**
 * @file TimerP.cpp
 * @brief Source file for class TimerP
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

 * @details This source file contains the definition of all the methods for
 * the class TimerP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Timer.h"
#include "Clock.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define ENABLE_TIM_CLOCK(instance)           __HAL_RCC_TIM##instance##_CLK_ENABLE()
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool Timer::Init(uint8 instance,
                 uint16 frequency) {

    TIM_TypeDef* TIMInstance[] = { TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8, TIM9, TIM10, TIM11, TIM12, TIM13, TIM14 };

    // instance 1 --> TIM1
    instance--;
    if (instance > 13) {
        instance = 13;
    }

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

    //choose a standard tick period
    uint16 tickPeriod = 1999;

    //tickFrequency=(tickPeriod+1)*frequency
    uint16 prescalerValue = (uint16) (((Clock::GetClockFrequency()) / (2 * ((tickPeriod + 1) * frequency))) - 1);
    // period=tick_frequency/pwm_frequency -1

    handle.Instance = TIMInstance[instance];
    handle.Init.Period = tickPeriod;
    handle.Init.Prescaler = prescalerValue;
    handle.Init.ClockDivision = 0;
    handle.Init.CounterMode = TIM_COUNTERMODE_UP;

    return HAL_TIM_Base_Init(&handle) == HAL_OK;

}

bool Timer::Start() {
    return HAL_TIM_Base_Start_IT(&handle) == HAL_OK;
}

void Timer::Stop() {
    HAL_TIM_Base_DeInit(&handle);
}


TimerHandle* Timer::GetHandlePtr(){
    return &handle;
}
