/**
 * @file DriversDefinitionP.h
 * @brief Header file for class DriversDefinitionP
 * @date 11/ago/2015
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

 * @details This header file contains the declaration of the class DriversDefinitionP
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DRIVERSDEFINITIONP_H_
#define DRIVERSDEFINITIONP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "usbd_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#define CallbackFcn *(void) (void);


typedef USBD_HandleTypeDef USBHandle;

typedef UART_HandleTypeDef UARTHandle;

typedef TIM_HandleTypeDef PWMHandle;

typedef TIM_HandleTypeDef TimerHandle;

typedef GPIO_TypeDef* GPIOHandle;

#endif /* DRIVERSDEFINITIONP_H_ */

