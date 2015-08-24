/**
 * @file GeneralPurposeIOP.cpp
 * @brief Source file for class GeneralPurposeIOP
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
 * the class GeneralPurposeIOP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GeneralPurposeIO.h"

#define ENABLE_GPIO_CLOCK(port)      __HAL_RCC_GPIO##port##_CLK_ENABLE();



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//pinMask is the mask of 16 pins.
//#define  GPIO_MODE_INPUT                        ((uint32_t)0x00000000)   /*!< Input Floating Mode                   */
//#define  GPIO_MODE_OUTPUT_PP                    ((uint32_t)0x00000001)   /*!< Output Push Pull Mode                 */
//#define  GPIO_MODE_OUTPUT_OD                    ((uint32_t)0x00000011)   /*!< Output Open Drain Mode                */
//#define  GPIO_MODE_AF_PP                        ((uint32_t)0x00000002)   /*!< Alternate Function Push Pull Mode     */
//#define  GPIO_MODE_AF_OD                        ((uint32_t)0x00000012)   /*!< Alternate Function Open Drain Mode    */
//#define  GPIO_MODE_ANALOG                       ((uint32_t)0x00000003)   /*!< Analog Mode  */
//#define  GPIO_MODE_IT_RISING                    ((uint32_t)0x10110000)   /*!< External Interrupt Mode with Rising edge trigger detection          */
//#define  GPIO_MODE_IT_FALLING                   ((uint32_t)0x10210000)   /*!< External Interrupt Mode with Falling edge trigger detection         */
//#define  GPIO_MODE_IT_RISING_FALLING            ((uint32_t)0x10310000)   /*!< External Interrupt Mode with Rising/Falling edge trigger detection  */
//#define  GPIO_MODE_EVT_RISING                   ((uint32_t)0x10120000)   /*!< External Event Mode with Rising edge trigger detection               */
//#define  GPIO_MODE_EVT_FALLING                  ((uint32_t)0x10220000)   /*!< External Event Mode with Falling edge trigger detection              */
//#define  GPIO_MODE_EVT_RISING_FALLING           ((uint32_t)0x10320000)   /*!< External Event Mode with Rising/Falling edge trigger detection       */
//#define  GPIO_SPEED_LOW         ((uint32_t)0x00000000)  /*!< Low speed     */
//#define  GPIO_SPEED_MEDIUM      ((uint32_t)0x00000001)  /*!< Medium speed  */
//#define  GPIO_SPEED_FAST        ((uint32_t)0x00000002)  /*!< Fast speed    */
//#define  GPIO_SPEED_HIGH        ((uint32_t)0x00000003)  /*!< High speed    */
//#define  GPIO_NOPULL        ((uint32_t)0x00000000)   /*!< No Pull-up or Pull-down activation  */
//#define  GPIO_PULLUP        ((uint32_t)0x00000001)   /*!< Pull-up activation                  */
//#define  GPIO_PULLDOWN      ((uint32_t)0x00000002)   /*!< Pull-down activation
void GeneralPurposeIO::Init(uint16 pinMask,
                            uint8 modeNumber,
                            uint32 pullMask,
                            uint32 speed,
                            uint8 alternateFunction,
                            uint8 portNumber) {
    uint32 GPIOMode[] = { 0x00000000u, 0x00000001u, 0x00000011u, 0x00000002u, 0x00000012u, 0x00000003, 0x10110000u, 0x10210000u, 0x10310000u, 0x10120000u,
            0x10220000u, 0x10320000u };

    GPIO_TypeDef * Ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE };

    switch (portNumber) {
    case 0:
        ENABLE_GPIO_CLOCK (A);
        break;
    case 1:
        ENABLE_GPIO_CLOCK (B);
        break;
    case 2:
        ENABLE_GPIO_CLOCK (C);
        break;
    case 3:
        ENABLE_GPIO_CLOCK (D);
        break;
    case 4:
        ENABLE_GPIO_CLOCK (E);
        break;
    }

    GPIO_InitTypeDef initStructure;

    initStructure.Pin = pinMask;

    if (modeNumber > 11) {
        modeNumber = 11;
    }
    initStructure.Mode = GPIOMode[modeNumber];

    // could be 0,1,2.
    initStructure.Pull = pullMask & 0x3;

    // could be [0-3]
    initStructure.Speed = speed & 0x3;
    initStructure.Alternate = alternateFunction & 0xf;

    if (portNumber > 4) {
        portNumber = 4;
    }

    handle = Ports[portNumber];
    HAL_GPIO_Init(handle, &initStructure);

}

void GeneralPurposeIO::DeInit(uint16 pinMask) {

    HAL_GPIO_DeInit(handle, pinMask);
}

bool GeneralPurposeIO::ReadPin(uint16 pinMask) {
    return (HAL_GPIO_ReadPin(handle, pinMask) == GPIO_PIN_SET) ? true : false;
}

void GeneralPurposeIO::WritePin(uint16 pinMask,
                                bool value) {
    value ? HAL_GPIO_WritePin(handle, pinMask, GPIO_PIN_SET) : HAL_GPIO_WritePin(handle, pinMask, GPIO_PIN_RESET);
}

void GeneralPurposeIO::TogglePin(uint16 pinMask) {
    HAL_GPIO_TogglePin(handle, pinMask);
}

