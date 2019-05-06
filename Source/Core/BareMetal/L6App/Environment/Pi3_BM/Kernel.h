/**
 * @file Kernel.h
 * @brief Header file for class Kernel
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

 * @details This header file contains the declaration of the class Kernel
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MARTE2_PLATFORMS_PI3_CIRCLE_DEMO1_KERNEL_H_
#define 		MARTE2_PLATFORMS_PI3_CIRCLE_DEMO1_KERNEL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <circle/actled.h>
#include <circle/logger.h>
#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/dwhcidevice.h>
#include <circle/sched/scheduler.h>
#include <circle/net/netsubsystem.h>
#include <circle/types.h>
#include <circle/multicore.h>
#include <circle/string.h>
#include <circle/startup.h>
#include <circle/gpioclock.h>
#include <circle/gpiopin.h>
#include <circle/pwmoutput.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CMultiCoreSupport1.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


class Kernel {
public:
    Kernel(void);
    ~Kernel(void);

    bool Initialize(void);

private:
// do not change this order
    CMemorySystem m_Memory;
    CActLED m_ActLED;
    CKernelOptions m_Options;
    CDeviceNameService m_DeviceNameService;
    CScreenDevice m_Screen;
    CSerialDevice m_Serial;
    CExceptionHandler m_ExceptionHandler;
    CInterruptSystem m_Interrupt;
    CTimer m_Timer;
    CLogger m_Logger;
    CGPIOClock m_Clock0;
    CGPIOPin m_ClockPin;
    CDWHCIDevice m_DWHCI;
    CScheduler m_Scheduler;
    CNetSubSystem myNet;
    CMultiCoreSupport1 m_multiCore;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MARTE2_PLATFORMS_PI3_CIRCLE_DEMO1_KERNEL_H_ */

