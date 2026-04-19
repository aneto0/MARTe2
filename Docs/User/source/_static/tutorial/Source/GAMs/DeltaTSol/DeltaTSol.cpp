/**                                                                             
 * @file DeltaTSol.cpp                                                                
 * @brief Source file for class DeltaTSol                                        
 * @date 17/04/2026                                                                
 * @author cabrian                                                            
 *                                                                              
 * @copyright Copyright 2026 F4E | European Joint Undertaking for ITER and      
 * the Development of Fusion Energy ('Fusion for Energy').                      
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved      
 * by the European Commission - subsequent versions of the EUPL (the 'Licence') 
 * You may not use this work except in compliance with the Licence.             
 * You may obtain a copy of the Licence at: https://eupl.eu/                    
 *                                                                              
 * @warning Unless required by applicable law or agreed to in writing,          
 * software distributed under the Licence is distributed on an 'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express          
 * or implied. See the Licence permissions and limitations under the Licence.   

 * @details This source file contains the definition of all the methods for     
 * the class DeltaTSol (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DeltaTSol.h"
#include "StructuredDataIHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

DeltaTSol::DeltaTSol() :
        GAM() {
    using namespace MARTe;
    ///AUTO-GENERATED: CTOR PARAMETERS. DO NOT EDIT!

    deltaTDiv = 0;
    ///AUTO-GENERATED: END OF CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR SIGNALS. DO NOT EDIT!

    absoluteTime = NULL_PTR(uint64 *);
    deltaT = NULL_PTR(uint32 *);
    ///AUTO-GENERATED: END OF CTOR SIGNALS. DO NOT EDIT!
    lastAbsoluteTime = 0;
}

DeltaTSol::~DeltaTSol() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF DTOR PARAMETERS. DO NOT EDIT!
}

bool DeltaTSol::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: INITIALISE PARAMETERS. DO NOT EDIT!

    if (ok) {
        ok = helper.ReadValidated("DeltaTDiv", deltaTDiv, "(DeltaTDiv > (uint32)(0))");
    }
    ///AUTO-GENERATED: END OF INITIALISE PARAMETERS. DO NOT EDIT!

    return ok;
}

bool DeltaTSol::Setup() {
    using namespace MARTe;
    bool ok = true;
    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!

    if (ok) {
        ok = (GetNumberOfInputSignals() == 1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of input signals. Expected 1 and read %d", GetNumberOfInputSignals());
        }
    }
    if (ok) {
        TypeDescriptor signalTypeAbsoluteTime = GetSignalType(InputSignals, 0);
        ok = (signalTypeAbsoluteTime == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for InputSignals at index 0");
        }
    }
    if (ok) {
        absoluteTime = reinterpret_cast<uint64 *>(GetInputSignalMemory(0));
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 1);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of output signals. Expected 1 and read %d", GetNumberOfOutputSignals());
        }
    }
    if (ok) {
        TypeDescriptor signalTypeDeltaT = GetSignalType(OutputSignals, 0);
        ok = (signalTypeDeltaT == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for OutputSignals at index 0");
        }
    }
    if (ok) {
        deltaT = reinterpret_cast<uint32 *>(GetOutputSignalMemory(0));
    }
    ///AUTO-GENERATED: END OF SETUP SIGNALS. DO NOT EDIT!
    return ok;
}

bool DeltaTSol::Execute() {
    using namespace MARTe;
    bool ok = true;
    if (lastAbsoluteTime > 0) {
        uint64 timeDiff = (*absoluteTime - lastAbsoluteTime);
        timeDiff /= static_cast<uint64>(deltaTDiv);
        *deltaT += static_cast<uint32>(timeDiff);
    }
    else {
        *deltaT = 0;
    }
    lastAbsoluteTime = *absoluteTime;
    return ok;
}

CLASS_REGISTER(DeltaTSol, "1.0")
} 

