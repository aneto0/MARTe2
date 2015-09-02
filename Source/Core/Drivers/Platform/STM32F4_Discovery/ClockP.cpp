/**
 * @file ClockP.cpp
 * @brief Source file for class ClockP
 * @date 08/08/2015
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
 * the class ClockP (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Clock.h"

//Contains all the register addresses.
#include "stm32f4xx.h"

//Includes all the needed driver (it is a list of header file)
#include "stm32f4xx_hal_conf.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

bool Clock::initialized = false;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#define VECT_TAB_OFFSET  0x00 /*!< Vector Table base offset field.
                                   This value must be a multiple of 0x200. */

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M      8
#define PLL_N      336

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P      2

/* USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ */
#define PLL_Q      7

//Definition of the clock maximum speed = 168MHz :)
//This variable is declared as extern in FreeRTOSConf.h
uint32_t SystemCoreClock = 168000000;

__I uint8_t AHBPrescTable[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9 };

void Clock::Reset(bool extRAMInit) {

    /* Reset the RCC clock configuration to the default reset state ------------*/
    /* Set HSION bit */
    RCC->CR |= (uint32_t) 0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t) 0xFEF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t) 0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    /* Vector Table Relocation in Internal FLASH */
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;

    initialized = true;

}

bool Clock::SysClockSet(uint32 clockType) {
    __IO uint32_t
    StartUpCounter = 0, HSEStatus = 0;
    bool retVal = false;

    if (initialized) {

        /* Enable HSE */
        RCC->CR |= ((uint32_t) RCC_CR_HSEON);

        /* Wait till HSE is ready and if Time out is reached exit */
        do {
            HSEStatus = RCC->CR & RCC_CR_HSERDY;
            StartUpCounter++;
        }
        while ((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

        if ((RCC->CR & RCC_CR_HSERDY) != RESET) {
            HSEStatus = (uint32_t) 0x01;
        }
        else {
            HSEStatus = (uint32_t) 0x00;
        }

        if (HSEStatus == (uint32_t) 0x01) {
            /* Enable high performance mode, System frequency up to 168 MHz */
            RCC->APB1ENR |= RCC_APB1ENR_PWREN;
            PWR->CR |= PWR_CR_PMODE;

            /* HCLK = SYSCLK / 1*/
            RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

            /* PCLK2 = HCLK / 2*/
            RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

            /* PCLK1 = HCLK / 4*/
            RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

            /* Configure the main PLL */
            RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) - 1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);

            /* Enable the main PLL */
            RCC->CR |= RCC_CR_PLLON;

            /* Wait till the main PLL is ready */
            while ((RCC->CR & RCC_CR_PLLRDY) == 0) {
            }

            /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
            FLASH->ACR = FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_5WS;

            /* Select the main PLL as system clock source */
            RCC->CFGR &= (uint32_t)((uint32_t) ~(RCC_CFGR_SW));
            RCC->CFGR |= RCC_CFGR_SW_PLL;

            /* Wait till the main PLL is used as system clock source */
            while ((RCC->CFGR & (uint32_t) RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
                ;
            {
            }
            retVal = true;
        }

    }
    return retVal;
}

bool Clock::SysClockUpdate() {
    uint32_t tmp = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp) {
    case 0x00: /* HSI used as system clock source */
        SystemCoreClock = HSI_VALUE;
        break;
    case 0x04: /* HSE used as system clock source */
        SystemCoreClock = HSE_VALUE;
        break;
    case 0x08: /* PLL used as system clock source */

        /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N
         SYSCLK = PLL_VCO / PLL_P
         */
        pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
        pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

        if (pllsource != 0) {
            /* HSE used as PLL clock source */
            pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
        }
        else {
            /* HSI used as PLL clock source */
            pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
        }

        pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1) * 2;
        SystemCoreClock = pllvco / pllp;
        break;
    default:
        SystemCoreClock = HSI_VALUE;
        break;
    }
    /* Compute HCLK frequency --------------------------------------------------*/
    /* Get HCLK prescaler */
    tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
    /* HCLK frequency */
    SystemCoreClock >>= tmp;

    HAL_Init();
    return true;
}

uint32 Clock::GetClockFrequency() {

    return HAL_RCC_GetHCLKFreq();
}
