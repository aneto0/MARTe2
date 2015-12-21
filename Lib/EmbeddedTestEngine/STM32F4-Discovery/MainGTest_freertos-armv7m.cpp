/**
 * @file EmbeddedTestLauncher.cpp
 * @brief Source file for class EmbeddedTestLauncher
 * @date 16/12/2015
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
 * the class EmbeddedTestLauncher (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"
#include "init_clock.h"
#include "usbd_def.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "usbd_conf.h"
#include "stm32f4_discovery.h"
#include "stm32f4xx_hal.h"
#include "UARTConfig.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

//#include "BareMetal/L0Types/BasicTypeGTest.cpp"
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TestInfo TestFunctions[32] = { 0 };
volatile int numberOfTests = 0;



static void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);/* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    }

    /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
    if (HAL_GetREVID() == 0x1001) {
        /* Enable the Flash prefetch */
        __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    }
}

class UARTConsole {

public:
    UARTConsole() {

    }

    bool Open() {
        //HAL_UART_MspInit(&handle);TestInfo
        /*##-1- Configure the UART peripheral ######################################*/
        /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
        /* UART1 configured as follow:
         - Word Length = 8 Bits
         - Stop Bit = One Stop bit
         - Parity = None
         - BaudRate = 9600 baud
         - Hardware flow control disabled (RTS and CTS signals) */
        handle.Instance = USARTx;

        handle.Init.BaudRate = 9600;
        handle.Init.WordLength = UART_WORDLENGTH_8B;
        handle.Init.StopBits = UART_STOPBITS_1;
        handle.Init.Parity = UART_PARITY_NONE;
        handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
        handle.Init.Mode = UART_MODE_TX_RX;
        handle.Init.OverSampling = UART_OVERSAMPLING_16;

        if (HAL_UART_Init(&handle) != HAL_OK) {

            return false;
        }
        return true;
    }

    void Close() {
        HAL_UART_MspDeInit(&handle);
    }

    bool Write(const char * const buffer,
               unsigned int &size) {
        if (HAL_UART_Transmit(&handle, (uint8_t*) buffer, size, HAL_MAX_DELAY) != HAL_OK) {
            return false;
        }

        return true;
    }

    bool Read(char* const buffer,
              unsigned int &size) {

        if (HAL_UART_Receive(&handle, (uint8_t*) buffer, size, HAL_MAX_DELAY) != HAL_OK) {
            return false;
        }
        return true;

    }
private:
    UART_HandleTypeDef handle;

};

//#include "../../../Test/GTest/BareMetal/L0Types/BasicTypeGTest.cpp"


/*
 int u = 0;

 class boh {
 public:
 boh() {
 u++;
 }
 };

 static boh ahah;
*/
 UARTConsole console;


int main() {

    HAL_Init();
    BSP_LED_Init (LED6);
    BSP_LED_Init (LED5);
    SystemClock_Config();

    console.Open();
    char buffer[32];
    unsigned int sizec = 9;
    for (int i = 0; i < numberOfTests; i++) {
        if (TestFunctions[i].function()) {
            strcpy(buffer, "\n\rPassed");
            console.Write(buffer, sizec);
            BSP_LED_Toggle(LED6);
        }
        else {
            strcpy(buffer, "\n\rFailed");
            console.Write(buffer, sizec);
            BSP_LED_Toggle(LED5);
        }
    }

    return 0;
}
