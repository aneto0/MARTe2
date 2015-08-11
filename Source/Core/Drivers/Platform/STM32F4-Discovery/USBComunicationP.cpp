/**
 * @file USBComunicationP.cpp
 * @brief Source file for class USBComunicationP
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
 * the class USBComunicationP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "USBComunication.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "stm32f4xx_hal.h"
#include "usbd_core.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
USBHandle USBComunication::handle = { 0 };
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

extern PCD_HandleTypeDef hpcd;

/**
 * @brief  This function handles USB-On-The-Go FS global interrupt request.
 * @param  None
 * @retval None
 */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
                       {
    HAL_PCD_IRQHandler(&hpcd);
}

//// FILE FROM usb_desc.c
/**
 * @brief  Initializes the PCD MSP.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_MspInit(PCD_HandleTypeDef *hpcd) {

    //Configure the USB-OTG pins!! A10 OTG_FS_ID and A11 OTG_FS_DM
    GPIO_InitTypeDef GPIO_InitStruct;

    if (hpcd->Instance == USB_OTG_FS) {
        /* Configure USB FS GPIOs */
        __GPIOA_CLK_ENABLE();

        /* Configure DM DP Pins */
        GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure VBUS Pin */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Configure ID pin */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* Enable USB FS Clocks */
        __USB_OTG_FS_CLK_ENABLE();

        /* Set USBFS Interrupt priority to 6 */
        HAL_NVIC_SetPriority(OTG_FS_IRQn, 6, 0);

        /* Enable USBFS Interrupt */
        HAL_NVIC_EnableIRQ (OTG_FS_IRQn);
    }

    else if (hpcd->Instance == USB_OTG_HS) {
#ifdef USE_USB_HS_IN_FS

        //Configure HS pins B12 B13 and B15

        /* Configure USB FS GPIOs */
        __GPIOB_CLK_ENABLE();

        /*Configure GPIO for HS on FS mode*/
        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_14 |GPIO_PIN_15;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF12_OTG_HS_FS;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Configure VBUS Pin */
        GPIO_InitStruct.Pin = GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        GPIO_InitStruct.Pull = GPIO_NOPULL;

#else

        /* Configure USB HS GPIOs */
        __GPIOA_CLK_ENABLE();
        __GPIOB_CLK_ENABLE();
        __GPIOC_CLK_ENABLE();
        __GPIOH_CLK_ENABLE();
        __GPIOI_CLK_ENABLE();

        /* CLK */
        GPIO_InitStruct.Pin = GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* D0 */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* D1 D2 D3 D4 D5 D6 D7 */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 |\
 GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* STP */
        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* NXT */
        GPIO_InitStruct.Pin = GPIO_PIN_4;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

        /* DIR */
        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Alternate = GPIO_AF10_OTG_HS;
        HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

        /* Enable embedded ULPI */
        __USB_OTG_HS_ULPI_CLK_ENABLE();
#endif /* USE_USB_HS_IN_FS */

        /* Enable USB HS Clocks */
        __USB_OTG_HS_CLK_ENABLE();

        /* Set USBHS Interrupt priority to 6 */
        HAL_NVIC_SetPriority(OTG_HS_IRQn, 6, 0);

        /* Enable USBHS Interrupt */
        HAL_NVIC_EnableIRQ (OTG_HS_IRQn);
    }
}

/**
 * @brief  DeInitializes the PCD MSP.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_MspDeInit(PCD_HandleTypeDef *hpcd) {
    if (hpcd->Instance == USB_OTG_FS) {
        /* Disable USB FS Clocks */
        __USB_OTG_FS_CLK_DISABLE();
        __SYSCFG_CLK_DISABLE();
    }
    else if (hpcd->Instance == USB_OTG_HS) {
        /* Disable USB FS Clocks */
        __USB_OTG_HS_CLK_DISABLE();
        __SYSCFG_CLK_DISABLE();
    }
}

/*******************************************************************************
 LL Driver Callbacks (PCD -> USB Device Library)
 *******************************************************************************/

/**
 * @brief  Setup stage callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_SetupStage((USBD_HandleTypeDef*) hpcd->pData, (uint8_t *) hpcd->Setup);
}

/**
 * @brief  Data Out stage callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd,
                                  uint8_t epnum) {
    USBD_LL_DataOutStage((USBD_HandleTypeDef*) hpcd->pData, epnum, hpcd->OUT_ep[epnum].xfer_buff);
}

/**
 * @brief  Data In stage callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd,
                                 uint8_t epnum) {
    USBD_LL_DataInStage((USBD_HandleTypeDef*) hpcd->pData, epnum, hpcd->IN_ep[epnum].xfer_buff);
}

/**
 * @brief  SOF callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_SOF((USBD_HandleTypeDef*) hpcd->pData);
}

/**
 * @brief  Reset callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd) {
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /* Set USB Current Speed */
    switch (hpcd->Init.speed) {
    case PCD_SPEED_HIGH:
        speed = USBD_SPEED_HIGH;
        break;

    case PCD_SPEED_FULL:
        speed = USBD_SPEED_FULL;
        break;

    default:
        speed = USBD_SPEED_FULL;
        break;
    }
    USBD_LL_SetSpeed((USBD_HandleTypeDef*) hpcd->pData, speed);

    /* Reset Device */
    USBD_LL_Reset((USBD_HandleTypeDef*) hpcd->pData);
}

/**
 * @brief  Suspend callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_Suspend((USBD_HandleTypeDef*) hpcd->pData);
}

/**
 * @brief  Resume callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_Resume((USBD_HandleTypeDef*) hpcd->pData);
}

/**
 * @brief  ISOC Out Incomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd,
                                      uint8_t epnum) {
    USBD_LL_IsoOUTIncomplete((USBD_HandleTypeDef*) hpcd->pData, epnum);
}

/**
 * @brief  ISOC In Incomplete callback.
 * @param  hpcd: PCD handle
 * @param  epnum: Endpoint Number
 * @retval None
 */
void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd,
                                     uint8_t epnum) {
    USBD_LL_IsoINIncomplete((USBD_HandleTypeDef*) hpcd->pData, epnum);
}

/**
 * @brief  Connect callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_DevConnected((USBD_HandleTypeDef*) hpcd->pData);
}

/**
 * @brief  Disconnect callback.
 * @param  hpcd: PCD handle
 * @retval None
 */
void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd) {
    USBD_LL_DevDisconnected((USBD_HandleTypeDef*) hpcd->pData);
}

/*******************************************************************************
 LL Driver Interface (USB Device Library --> PCD)
 *******************************************************************************/
/**
 * @brief  Initializes the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Init(USBD_HandleTypeDef *pdev) {
    /* Change Systick prioity */
    NVIC_SetPriority(SysTick_IRQn, 0);

#ifdef USE_USB_FS
    /*Set LL Driver parameters */
    hpcd.Instance = USB_OTG_FS;
    hpcd.Init.dev_endpoints = 4;
    hpcd.Init.use_dedicated_ep1 = 0;
    hpcd.Init.ep0_mps = 0x40;
    hpcd.Init.dma_enable = 0;
    hpcd.Init.low_power_enable = 0;
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.Sof_enable = 0;
    hpcd.Init.speed = PCD_SPEED_FULL;
    hpcd.Init.vbus_sensing_enable = DISABLE;
    /* Link The driver to the stack */
    hpcd.pData = pdev;
    pdev->pData = &hpcd;
    /*Initialize LL Driver */
    HAL_PCD_Init(&hpcd);

    HAL_PCD_SetRxFiFo(&hpcd, 0x80);
    HAL_PCD_SetTxFiFo(&hpcd, 0, 0x40);
    HAL_PCD_SetTxFiFo(&hpcd, 1, 0x80);

#endif
#ifdef USE_USB_HS
    /*Set LL Driver parameters */
    hpcd.Instance = USB_OTG_HS;
    hpcd.Init.dev_endpoints = 6;
    hpcd.Init.use_dedicated_ep1 = 0;
    hpcd.Init.ep0_mps = 0x40;

    /* Be aware that enabling USB-DMA mode will result in data being sent only by
     multiple of 4 packet sizes. This is due to the fact that USB DMA does
     not allow sending data from non word-aligned addresses.
     For this specific application, it is advised to not enable this option
     unless required. */
    hpcd.Init.dma_enable = 0;

    hpcd.Init.low_power_enable = 0;

#ifdef USE_USB_HS_IN_FS
    hpcd.Init.phy_itface = PCD_PHY_EMBEDDED;
    hpcd.Init.speed = PCD_SPEED_HIGH_IN_FULL;
#else
    hpcd.Init.phy_itface = PCD_PHY_ULPI;
    hpcd.Init.speed = PCD_SPEED_HIGH;
#endif
    hpcd.Init.Sof_enable = 0;
    hpcd.Init.vbus_sensing_enable = DISABLE;
    /* Link The driver to the stack */
    hpcd.pData = pdev;
    pdev->pData = &hpcd;
    /*Initialize LL Driver */
    HAL_PCD_Init(&hpcd);

    HAL_PCD_SetRxFiFo(&hpcd, 0x200);
    HAL_PCD_SetTxFiFo(&hpcd, 0, 0x80);
    HAL_PCD_SetTxFiFo(&hpcd, 1, 0x174);

#endif
    return USBD_OK;
}

/**
 * @brief  De-Initializes the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev) {
    HAL_PCD_DeInit((PCD_HandleTypeDef*) pdev->pData);
    return USBD_OK;
}

/**
 * @brief  Starts the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev) {
    HAL_PCD_Start((PCD_HandleTypeDef*) pdev->pData);
    return USBD_OK;
}

/**
 * @brief  Stops the Low Level portion of the Device driver.
 * @param  pdev: Device handle
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev) {
    HAL_PCD_Stop((PCD_HandleTypeDef*) pdev->pData);
    return USBD_OK;
}

/**
 * @brief  Opens an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  ep_type: Endpoint Type
 * @param  ep_mps: Endpoint Max Packet Size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev,
                                  uint8_t ep_addr,
                                  uint8_t ep_type,
                                  uint16_t ep_mps) {
    HAL_PCD_EP_Open((PCD_HandleTypeDef*) pdev->pData, ep_addr, ep_mps, ep_type);
    return USBD_OK;
}

/**
 * @brief  Closes an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev,
                                   uint8_t ep_addr) {
    HAL_PCD_EP_Close((PCD_HandleTypeDef*) pdev->pData, ep_addr);
    return USBD_OK;
}

/**
 * @brief  Flushes an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev,
                                   uint8_t ep_addr) {
    HAL_PCD_EP_Flush((PCD_HandleTypeDef*) pdev->pData, ep_addr);
    return USBD_OK;
}

/**
 * @brief  Sets a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev,
                                   uint8_t ep_addr) {
    HAL_PCD_EP_SetStall((PCD_HandleTypeDef*) pdev->pData, ep_addr);
    return USBD_OK;
}

/**
 * @brief  Clears a Stall condition on an endpoint of the Low Level Driver.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev,
                                        uint8_t ep_addr) {
    HAL_PCD_EP_ClrStall((PCD_HandleTypeDef*) pdev->pData, ep_addr);
    return USBD_OK;
}

/**
 * @brief  Returns Stall condition.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval Stall (1: yes, 0: No)
 */
uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev,
                          uint8_t ep_addr) {
    PCD_HandleTypeDef *hpcd = (PCD_HandleTypeDef*) pdev->pData;

    if ((ep_addr & 0x80) == 0x80) {
        return hpcd->IN_ep[ep_addr & 0x7F].is_stall;
    }
    else {
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall;
    }
}

/**
 * @brief  Assigns an USB address to the device
 * @param  pdev: Device handle
 * @param  dev_addr: USB address
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev,
                                         uint8_t dev_addr) {
    HAL_PCD_SetAddress((PCD_HandleTypeDef*) pdev->pData, dev_addr);
    return USBD_OK;
}

/**
 * @brief  Transmits data over an endpoint
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  pbuf: Pointer to data to be sent
 * @param  size: Data size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev,
                                    uint8_t ep_addr,
                                    uint8_t *pbuf,
                                    uint16_t size) {
    HAL_PCD_EP_Transmit((PCD_HandleTypeDef*) pdev->pData, ep_addr, pbuf, size);
    return USBD_OK;
}

/**
 * @brief  Prepares an endpoint for reception
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @param  pbuf:pointer to data to be received
 * @param  size: data size
 * @retval USBD Status
 */
USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev,
                                          uint8_t ep_addr,
                                          uint8_t *pbuf,
                                          uint16_t size) {
    HAL_PCD_EP_Receive((PCD_HandleTypeDef*) pdev->pData, ep_addr, pbuf, size);
    return USBD_OK;
}

/**
 * @brief  Returns the last transfered packet size.
 * @param  pdev: Device handle
 * @param  ep_addr: Endpoint Number
 * @retval Recived Data Size
 */
uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev,
                               uint8_t ep_addr) {
    return HAL_PCD_EP_GetRxCount((PCD_HandleTypeDef*) pdev->pData, ep_addr);
}

/**
 * @brief  Delay routine for the USB Device Library
 * @param  Delay: Delay in ms
 * @retval None
 */
void USBD_LL_Delay(uint32_t Delay) {
    HAL_Delay(Delay);
}

/////// FILE FROM usbd_cdc_interface.c

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
    USBD_CDC_SetRxBuffer(&(USBComunication::handle), s_RxBuffer.Buffer);
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

////////////////////////////////////////////////////////////////////////////////////

USBComunication::USBComunication() {

}

bool USBComunication::Init() {
    // USB CDC initialization
    USBD_Init(&handle, &VCP_Desc, 0);

    USBD_RegisterClass(&handle, &USBD_CDC);
    USBD_CDC_RegisterInterface(&handle, &USBD_CDC_Template_fops);
    return true;

}

bool USBComunication::Start() {
    USBD_Start(&handle);

    return true;
}

bool USBComunication::Stop() {
    USBD_Stop(&handle);
    return true;

}

int32 USBComunication::Send(const char8* const txBuffer,
                            int32 size) {
    if (size > CDC_DATA_HS_OUT_PACKET_SIZE) {
        int offset;
        for (offset = 0; offset < size; offset++) {
            int todo = MIN(CDC_DATA_HS_OUT_PACKET_SIZE, size - offset);
            int done = Send(((char *) txBuffer) + offset, todo);
            if (done != todo)
                return offset + done;
        }

        return size;
    }

    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef *) handle.pClassData;
    while (pCDC->TxState) {
    } //Wait for previous transfer

    USBD_CDC_SetTxBuffer(&handle, (uint8_t *) txBuffer, size);
    if (USBD_CDC_TransmitPacket(&handle) != USBD_OK)
        return 0;

    while (pCDC->TxState) {
    } //Wait until transfer is done
    return size;

}

int32 USBComunication::Receive(char8* const rxBuffer,
                               int32 size) {
    if (!s_RxBuffer.ReadDone)
        return 0;

    int remaining = s_RxBuffer.Size - s_RxBuffer.Position;
    int todo = MIN(remaining, size);
    if (todo <= 0)
        return 0;

    memcpy(rxBuffer, s_RxBuffer.Buffer + s_RxBuffer.Position, todo);
    s_RxBuffer.Position += todo;
    if (s_RxBuffer.Position >= s_RxBuffer.Size) {
        s_RxBuffer.ReadDone = 0;
        USBD_CDC_ReceivePacket(&handle);
    }

    return todo;
}

USBHandle handle;

