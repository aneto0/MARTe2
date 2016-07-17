/**
 * @file AndreGAM1.cpp
 * @brief Source file for class AndreGAM1
 * @date Jun 8, 2016
 * @author aneto
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
 * the class AndreGAM1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GAM1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

AndreGAM1::AndreGAM1() :
        GAM() {

}

AndreGAM1::~AndreGAM1() {

}

bool AndreGAM1::Initialise(StructuredDataI & data) {
    return GAM::Initialise(data);
}
#include <stdio.h>
static int aaa = 1;
bool AndreGAM1::Execute() {
    Read();
    const char8 *name = GetName();
    printf("%s:\n", name);
    uint32 numberOfInputSignals = GetNumberOfInputSignals();
    uint32 numberOfOutputSignals = GetNumberOfOutputSignals();
    printf("Inputs\n");
    uint32 n;
    for (n = 0u; n < numberOfInputSignals; n++) {
        StreamString signalName;
        GetSignalName(InputSignals, n, signalName);
        uint32 signalValue = *((uint32 *) GetInputSignalAddress(n));
        printf("  %s = %u\n", signalName.Buffer(), signalValue);
    }
    for (n = 0u; n < numberOfOutputSignals; n++) {
        *((uint32 *) GetOutputSignalAddress(n)) = aaa * (n + 1) * 100;
    }
    printf("Outputs\n");
    Write();
    for (n = 0u; n < numberOfOutputSignals; n++) {
        StreamString signalName;
        GetSignalName(OutputSignals, n, signalName);
        uint32 signalValue = *((uint32 *) GetOutputSignalAddress(n));
        printf("  %s = %u\n", signalName.Buffer(), signalValue);
    }
    aaa++;
    return true;
}

CLASS_REGISTER(AndreGAM1, "1.0");
}
