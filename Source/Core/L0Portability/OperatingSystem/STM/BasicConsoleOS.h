/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Basic console implementation in linux
 */

//see Driver/STM..HAL/ stm..uart...c


#ifndef BASIC_CONSOLE_OS_H
#define BASIC_CONSOLE_OS_H


#include "../../GeneralDefinitions.h"

#define STDOUT 1
#define STDIN  0
/**
 * Number of rows that will be cleared when BasicConsoleOSClear is called
 */
#define BASIC_CONSOLE_LINUX_CLEAR_ROWS 40


//global definition ç_ç
static UART_HandleTypeDef UartHandle;


/** Template functions which uses macro defined in GeneralDefinitions.h to init UART */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  /* Enable USART2 clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
    
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
}




/*
  HAL_Init();
  
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
  BSP_LED_Init(LED6);

  SystemClock_Config();

  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
#ifdef TRANSMITTER_BOARD
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);
	
  while (BSP_PB_GetState(BUTTON_KEY) == RESET)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(40);		
  }
  while (BSP_PB_GetState(BUTTON_KEY) == SET)
  {
  }
  
  BSP_LED_Off(LED3);
  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
  {
    Error_Handler();   
  }
    
  BSP_LED_On(LED6);
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
  {
    Error_Handler();  
  }
    
  BSP_LED_On(LED4);
  
#else
  
  if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
  {
    Error_Handler();    
  }
  BSP_LED_On(LED4);
  
  if(HAL_UART_Transmit(&UartHandle, (uint8_t*)aTxBuffer, TXBUFFERSIZE, 5000)!= HAL_OK)
  {
    Error_Handler();
  }
  BSP_LED_On(LED6);
  
  
  if(Buffercmp((uint8_t*)aTxBuffer,(uint8_t*)aRxBuffer,RXBUFFERSIZE))
  {
    Error_Handler();  
  }
   
  while (1)
  {    
  }

*/

/** Inizialize the UART instance */
bool UartInizialisation(BasicConsole &con){
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();
  
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  UartHandle.Instance          = USARTx;
  
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    return False;
  }

	//save the handle
	con.inputConsoleHandle = (void *)&UartHandle;

	return True;

}


/**
 * @see BasicConsoleOpen
 * @brief Open a console.
 * @param con is the console.
 * @param numberOfColumns is the desired number of columns.
 * @param numberOfRows is the desired number of rows.
 * @return true if the console is opened correctly.
 */
bool BasicConsoleOSOpen(BasicConsole &con, int32 numberOfColumns,
                        int32 numberOfRows) {

	return UartInizialisation(con);

}

/**
 * @see BasicConsoleClose
 * @brief Close a console.
 * @param con is the console to close.
 * @return true if the console is closed correctly.
 */
bool BasicConsoleOSClose(BasicConsole &con) {
	UART_HandleTypeDef *thisHandle = (UART_HandleTypeDef *)(con.inputConsoleHandle);
	return HAL_UART_DeInit(thisHandle) != HAL_OK;
}

/**
 * @see BasicConsole::Show
 * @brief Not Implemented.
 * @param con is the console.
 * @return true.
 */
bool BasicConsoleOSShow(BasicConsole &con) {
    return True;
}

/**
 * @see BasicConsole::Write
 * @brief Write on the console.
 * @param con is the console.
 * @param buffer is a pointer to the memory to write.
 * @param size is the number of bytes to write on the console.
 * @return true if at least one byte is written.
 */
bool BasicConsoleOSWrite(BasicConsole &con, const void* buffer, uint32 &size) {

	//5000 should not be a limit for the timeout.
	return HAL_UART_Transmit(&UartHandle, (unsigned char *)buffer, size, 5000) == HAL_OK;   
    
}

/**
 * @see BasicConsole::Read.
 * @brief Read from the console.
 * @param con is the console.
 * @param buffer is the location of memory where store read bytes.
 * @param size is the number of bytes to read.
 * @return true if at least one byte is read.
 */
bool BasicConsoleOSRead(BasicConsole &con, void* buffer, uint32 &size,
                        TimeoutType msecTimeout) {
	//5000 should not be a limit for the timeout.
	return HAL_UART_Receive(&UartHandle, (unsigned char*) buffer, size, 5000) == HAL_OK;

}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param title is the title.
 * @return false.
 */
bool BasicConsoleOSSetTitleBar(BasicConsole &con, const char *title) {
    return False;
}

/**
 * @see BasicConsole::SetWindowSize.
 * @brief Set the number of rows and columns.
 * @param con is the console.
 * @param numberOfColumns is the desired number of columns.
 * @param numberOfRows is the desired number of rows.
 * @return true.
 */
bool BasicConsoleOSSetWindowSize(BasicConsole &con, int numberOfColumns,
                                 int numberOfRows) {
    con.numberOfColumns = numberOfColumns;
    con.numberOfRows = numberOfRows;
    return True;
}

/**
 * @see BasicConsole::GetWindowSize.
 * @brief Get the number of columns and rows.
 * @param con is the console.
 * @param numberOfColumns is the numberOfColumns in return.
 * @param numberOfRows is the numberOfRows in return.
 * @return true.
 */
bool BasicConsoleOSGetWindowSize(BasicConsole &con, int &numberOfColumns,
                                 int &numberOfRows) {
    numberOfColumns = con.numberOfColumns;
    numberOfRows = con.numberOfRows;
    return True;
}

/**
 * @see BasicConsoleOSSetWindowSize BasicConsole::SetWindowSize
 */
bool BasicConsoleOSSetSize(BasicConsole &con, int numberOfColumns,
                           int numberOfRows) {
    con.numberOfColumns = numberOfColumns;
    con.numberOfRows = numberOfRows;
    return True;
}

/**
 * @see BasicConsoleOSGetWindowSize BasicConsole::GetWindowSize
 */
bool BasicConsoleOSGetSize(BasicConsole &con, int &numberOfColumns,
                           int &numberOfRows) {
    numberOfColumns = con.numberOfColumns;
    numberOfRows = con.numberOfRows;
    return True;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param column is the desired x position.
 * @param row is the desired y position.
 * @return false.
 */
bool BasicConsoleOSSetCursorPosition(BasicConsole &con, int column, int row) {
    return False;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param column is the cursor x position.
 * @param row is the cursor y position.
 * @return false.
 */
bool BasicConsoleOSGetCursorPosition(BasicConsole &con, int &column, int &row) {
    return False;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param foreGroundColour is the desired foreground color.
 * @param backGroundColour is the desired background color.
 * @return false.
 */
bool BasicConsoleOSSetColour(BasicConsole &con, Colours foreGroundColour,
                             Colours backGroundColour) {
    return False;
}

/**
 * @see BasicConsole::Clear
 * @brief Clean the console.
 * @param con is the console.
 * @return true.
 */
bool BasicConsoleOSClear(BasicConsole &con) {
   
    return True;
}

/**
 * @brief Not implemented.
 * @param con is the console.
 * @param c is the char to write.
 * @param foreGroundColour is the desired foreground color.
 * @param backGroundColour is the desired background color.
 * @param column is the desired x position.
 * @param row is the desired y position.
 * @return false.
 */
bool BasicConsoleOSPlotChar(BasicConsole &con, char c, Colours foreGroundColour,
                            Colours backGroundColour, int column, int row) {
    return False;

}

#endif
