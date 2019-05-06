/**
 * @file Kernel.cpp
 * @brief Source file for class Kernel
 * @date 05 mag 2019
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
 * the class Kernel (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Kernel.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#define CLOCK_PIN 4

#define CLOCK_DIVIDER 4000// 500 MHz / 4000 = 125 KHz

static const u8 IPAddress[] = { 192, 168, 1, 10 };
static const u8 NetMask[] = { 255, 255, 255, 0 };
static const u8 DefaultGateway[] = { 192, 168, 1, 1 };
static const u8 DNSServer[] = { 192, 168, 1, 1 };


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

Kernel::Kernel(void) :
        m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
        m_Timer(&m_Interrupt),
        m_Logger(m_Options.GetLogLevel(), &m_Timer),
        m_Clock0(GPIOClock0, GPIOClockSourcePLLD), // clock source PLLD, 500 MHz
        m_ClockPin(CLOCK_PIN, GPIOModeAlternateFunction0),
        m_DWHCI(&m_Interrupt, &m_Timer),
        m_multiCore(&m_Memory),
        myNet(IPAddress, NetMask, DefaultGateway, DNSServer)
         {
    m_ActLED.Blink(5); // show we are alive
    m_Clock0.Start(CLOCK_DIVIDER);
}

Kernel::~Kernel(void) {
}

bool Kernel::Initialize(void) {
    bool bOK = true;

    if (bOK) {
        bOK = m_Screen.Initialize();
    }

    if (bOK) {
        bOK = m_Serial.Initialize(115200);
    }

    if (bOK) {
        CDevice *pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
        if (pTarget == 0) {
            pTarget = &m_Screen;
        }

        bOK = m_Logger.Initialize(pTarget);
    }

    if (bOK) {
        bOK = m_Interrupt.Initialize();
    }

    if (bOK) {
        bOK = m_Timer.Initialize();
    }

    if (bOK) {
        bOK = m_DWHCI.Initialize();
    }
    if (bOK) {
        myNet.Initialize();
    }
    if (bOK) {
        m_multiCore.Initialize();
    }

    return bOK;
}
