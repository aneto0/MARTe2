/**
 * @file USBConsoleP.cpp
 * @brief Source file for class USBConosoleP
 * @date 09/08/2015
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
 * the class USBConsoleP (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "USBConsole.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "stm32f4xx_hal.h"
#include "usbd_core.h"
#include "stm32f4_discovery.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
USBHandle USBConsole::handle = { 0 };
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

static int8_t TEMPLATE_Init(void);
static int8_t TEMPLATE_DeInit(void);
static int8_t TEMPLATE_Control(uint8_t cmd,
                               uint8_t* pbuf,
                               uint16_t length);
static int8_t TEMPLATE_Receive(uint8_t* pbuf,
                               uint32_t *Len);

USBD_CDC_ItfTypeDef USBD_CDC_Template_fops = { TEMPLATE_Init, TEMPLATE_DeInit, TEMPLATE_Control, TEMPLATE_Receive };

USBD_CDC_LineCodingTypeDef linecoding = { 115200, /* baud rate*/
0x00, /* stop bits-1*/
0x00, /* parity - none*/
0x08 /* nb. of bits 8*/
};

//extern USBD_HandleTypeDef USBD_Device;

static struct {
    uint8_t Buffer[CDC_DATA_FS_OUT_PACKET_SIZE];
    int Position;
    int Size;
    char ReadDone;
} s_RxBuffer;

char g_VCPInitialized;

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  TEMPLATE_Init
 *         Initializes the CDC media low layer
 * @param  None
 * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t TEMPLATE_Init(void) {
    for(uint32 i=0; i<CDC_DATA_FS_OUT_PACKET_SIZE; i++){
        s_RxBuffer.Buffer[i]=0;
    }
    USBD_CDC_SetRxBuffer(&(USBConsole::handle), s_RxBuffer.Buffer);
    g_VCPInitialized = 1;
    return (0);
}

/**
 * @brief  TEMPLATE_DeInit
 *         DeInitializes the CDC media low layer
 * @param  None
 * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t TEMPLATE_DeInit(void) {
    /*
     Add your deinitialization code here
     */
    return (0);
}

/**
 * @brief  TEMPLATE_Control
 *         Manage the CDC class requests
 * @param  Cmd: Command code
 * @param  Buf: Buffer containing command data (request parameters)
 * @param  Len: Number of data to be sent (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t TEMPLATE_Control(uint8_t cmd,
                               uint8_t* pbuf,
                               uint16_t length) {
    switch (cmd) {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        /* Add your code here */
        break;

    case CDC_GET_ENCAPSULATED_RESPONSE:
        /* Add your code here */
        break;

    case CDC_SET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_GET_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_CLEAR_COMM_FEATURE:
        /* Add your code here */
        break;

    case CDC_SET_LINE_CODING:
        linecoding.bitrate = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
 (pbuf[2] << 16) | (pbuf[3] << 24));
        linecoding.format = pbuf[4];
        linecoding.paritytype = pbuf[5];
        linecoding.datatype = pbuf[6];

        /* Add your code here */
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = (uint8_t)(linecoding.bitrate);
        pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
        pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
        pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
        pbuf[4] = linecoding.format;
        pbuf[5] = linecoding.paritytype;
        pbuf[6] = linecoding.datatype;

        /* Add your code here */
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        /* Add your code here */
        break;

    case CDC_SEND_BREAK:
        /* Add your code here */
        break;

    default:
        break;
    }

    return (0);
}

/**
 * @brief  TEMPLATE_DataRx
 *         Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will block any OUT packet reception on USB endpoint
 *         untill exiting this function. If you exit this function before transfer
 *         is complete on CDC interface (ie. using DMA controller) it will result
 *         in receiving more data while previous ones are still not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
 */
static int8_t TEMPLATE_Receive(uint8_t* Buf,
                               uint32_t *Len) {
    s_RxBuffer.Position = 0;
    s_RxBuffer.Size = *Len;
    s_RxBuffer.ReadDone = 1;
    return (0);
}

USBConsole::USBConsole() {

}

ErrorType USBConsole::Open(const FlagsType &mode) {
    // USB CDC initialization
    USBD_Init(&handle, &VCP_Desc, 0);

    USBD_RegisterClass(&handle, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handle, &USBD_CDC_Template_fops);
    USBD_Start(&handle);

    return NoError;
}

ErrorType USBConsole::Close() {
    USBD_Stop(&handle);
    return NoError;

}

ErrorType USBConsole::Write(const char8* const txBuffer,
                            uint32 &size,
                            const TimeoutType &timeout) {
    if (size > CDC_DATA_HS_OUT_PACKET_SIZE) {
        uint32 offset;
        for (offset = 0; offset < size; offset += CDC_DATA_HS_OUT_PACKET_SIZE) {
            uint32 todo = (uint32) (MIN(CDC_DATA_HS_OUT_PACKET_SIZE, (int32) (size - offset)));
            ErrorType ret = Write(((char *) txBuffer) + offset, todo, timeout);
            if (ret != NoError) {
                size = offset + todo;
                return FatalError;
            }
        }

        return NoError;
    }

    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *) handle.pClassData;
    while (pCDC->TxState) {
    } //Wait for previous transfer

    USBD_CDC_SetTxBuffer(&handle, (uint8_t *) txBuffer, size);
    if (USBD_CDC_TransmitPacket(&handle) != USBD_OK) {
        return FatalError;
    }

    while (pCDC->TxState) {
    } //Wait until transfer is done
    return NoError;

}

uint32 USBConsole::privateRead(char8* const rxBuffer,
                               uint32 size) {

    if (!s_RxBuffer.ReadDone) {
        return 0;
    }

    int remaining = s_RxBuffer.Size - s_RxBuffer.Position;
    int todo = MIN(remaining, (int) size);
    if (todo <= 0) {
        return 0;
    }

    memcpy(rxBuffer, s_RxBuffer.Buffer + s_RxBuffer.Position, todo);
    s_RxBuffer.Position += todo;
    if (s_RxBuffer.Position >= s_RxBuffer.Size) {
        s_RxBuffer.ReadDone = 0;
        USBD_CDC_ReceivePacket(&handle);
    }

    return todo;
}

ErrorType USBConsole::Read(char8* const rxBuffer,
                           uint32 &size,
                           const TimeoutType &timeout) {

    uint32 cont = 0;
    while (cont < size) {
        cont += privateRead(rxBuffer + cont, size - cont);
    }

    return NoError;

}

int32 USBConsole::size() {
    return s_RxBuffer.Size;
}

int32 USBConsole::position() {
    return s_RxBuffer.Position;
}

