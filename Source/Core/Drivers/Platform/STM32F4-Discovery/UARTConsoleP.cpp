/**
 * @file UARTConsoleP.cpp
 * @brief Source file for class UARTConsoleP
 * @date 13/08/2015
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
 * the class UARTConsoleP (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UARTConsole.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF7_USART1

/* Size of Transmission buffer */
#define TXBUFFERSIZE                     (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                     TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))

UARTConsole::UARTConsole() {

}

ErrorType UARTConsole::Open(const FlagsType &mode) {
    HAL_UART_MspInit (&handle);
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
        return OSError;
    }
    return NoError;
}

ErrorType UARTConsole::Write(const char8 * const buffer,
                             uint32 &size,
                             const TimeoutType &timeout) {

    HAL_StatusTypeDef retVal;

    if (timeout.IsFinite()) {
        if ((retVal = HAL_UART_Transmit(&handle, (uint8_t*) buffer, size, timeout.GetTimeoutMSec())) != HAL_OK) {
            return (retVal == HAL_TIMEOUT) ? Timeout : FatalError;
        }
    }
    else {
        if (HAL_UART_Transmit(&handle, (uint8_t*) buffer, size, HAL_MAX_DELAY) != HAL_OK) {
            return FatalError;
        }
    }
    return NoError;

}

ErrorType UARTConsole::Read(char8* const buffer,
                           uint32 &size,
                           const TimeoutType &timeout) {

    HAL_StatusTypeDef retVal;

    if (timeout.IsFinite()) {
        if ((retVal = HAL_UART_Receive(&handle, (uint8_t*) buffer, size, timeout.GetTimeoutMSec())) != HAL_OK) {
            return (retVal == HAL_TIMEOUT) ? Timeout : FatalError;
        }
    }
    else {
        if (HAL_UART_Receive(&handle, (uint8_t*) buffer, size, HAL_MAX_DELAY) != HAL_OK) {
            return FatalError;
        }
    }
    return NoError;

}

