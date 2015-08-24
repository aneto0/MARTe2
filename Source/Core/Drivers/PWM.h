/**
 * @file PWM.h
 * @brief Header file for class PWM
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

 * @details This header file contains the declaration of the class PWM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PWM_H_
#define PWM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_PLATFORM(PLATFORM,DriversDefinitionsP.h)
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class PWM{
public:

    bool Init(uint8 instance,
              uint16 frequency,
              uint8 dutyCycle,
              uint8 mode,
              uint8 channelMask);

    bool Start(uint8 channelMask);

    bool Stop(uint8 channelMask);

    void SetFrequency(uint16 frequency);

    bool SetDutyCycle(uint8 dutyCycle, uint8 channelMask);



private:

    PWMHandle handle;


    TIM_OC_InitTypeDef sConfigTim4;

    uint16 tickPeriod;

    uint8 configChannelMask;

    uint16 prescalerValue;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PWM_H_ */

