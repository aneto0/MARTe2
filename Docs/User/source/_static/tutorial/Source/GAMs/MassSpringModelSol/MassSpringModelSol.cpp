/**                                                                             
 * @file MassSpringModelSol.cpp                                                                
 * @brief Source file for class MassSpringModelSol                                        
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
 * the class MassSpringModelSol (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MassSpringModelSol.h"
#include "StructuredDataIHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

MassSpringModelSol::MassSpringModelSol() :
        GAM() {
    using namespace MARTe;
    ///AUTO-GENERATED: CTOR PARAMETERS. DO NOT EDIT!

    mass = 0.0f;
    springConstant = 0.0f;
    dampingCoefficient = 0.0f;
    initialPosition = 0.0f;
    initialVelocity = 0.0f;
    ///AUTO-GENERATED: END OF CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR SIGNALS. DO NOT EDIT!

    time = NULL_PTR(uint32 *);
    force = NULL_PTR(float64 *);
    position = NULL_PTR(float64 *);
    velocity = NULL_PTR(float64 *);
    ///AUTO-GENERATED: END OF CTOR SIGNALS. DO NOT EDIT!
    lastTime = 0u;
}

MassSpringModelSol::~MassSpringModelSol() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF DTOR PARAMETERS. DO NOT EDIT!
}

bool MassSpringModelSol::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = GAM::Initialise(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: INITIALISE PARAMETERS. DO NOT EDIT!

    if (ok) {
        ok = helper.ReadValidated("Mass", mass, "(Mass > (float64)(0))");
    }
    if (ok) {
        ok = helper.Read("SpringConstant", springConstant, 10.0);
    }
    if (ok) {
        ok = helper.Read("DampingCoefficient", dampingCoefficient, 0.5);
    }
    if (ok) {
        ok = helper.Read("InitialPosition", initialPosition, 0.0);
    }
    if (ok) {
        ok = helper.Read("InitialVelocity", initialVelocity, 0.0);
    }
    ///AUTO-GENERATED: END OF INITIALISE PARAMETERS. DO NOT EDIT!

    return ok;
}

bool MassSpringModelSol::Setup() {
    using namespace MARTe;
    bool ok = true;
    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!

    if (ok) {
        ok = (GetNumberOfInputSignals() == 2);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of input signals. Expected 0 and read %d", 2, GetNumberOfInputSignals());
        }
    }
    if (ok) {
        TypeDescriptor signalTypeTime = GetSignalType(InputSignals, 0);
        ok = (signalTypeTime == UnsignedInteger32Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for InputSignals at index 0");
        }
    }
    if (ok) {
        time = reinterpret_cast<uint32 *>(GetInputSignalMemory(0));
    }
    if (ok) {
        TypeDescriptor signalTypeForce = GetSignalType(InputSignals, 1);
        ok = (signalTypeForce == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for InputSignals at index 1");
        }
    }
    if (ok) {
        force = reinterpret_cast<float64 *>(GetInputSignalMemory(1));
    }
    if (ok) {
        ok = (GetNumberOfOutputSignals() == 2);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of output signals. Expected 0 and read %d", 2, GetNumberOfOutputSignals());
        }
    }
    if (ok) {
        TypeDescriptor signalTypePosition = GetSignalType(OutputSignals, 0);
        ok = (signalTypePosition == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for OutputSignals at index 0");
        }
    }
    if (ok) {
        position = reinterpret_cast<float64 *>(GetOutputSignalMemory(0));
    }
    if (ok) {
        TypeDescriptor signalTypeVelocity = GetSignalType(OutputSignals, 1);
        ok = (signalTypeVelocity == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type for OutputSignals at index 1");
        }
    }
    if (ok) {
        velocity = reinterpret_cast<float64 *>(GetOutputSignalMemory(1));
    }
    ///AUTO-GENERATED: END OF SETUP SIGNALS. DO NOT EDIT!
    //Normalise the springConstant and the dampingCoefficient
    if (ok) {
        springConstant /= mass;
        dampingCoefficient /= mass;
    }
    return ok;
}

bool MassSpringModelSol::Execute() {
    using namespace MARTe;
    bool ok = true;
    if (lastTime == 0u) {
        *position = initialPosition;
        *velocity = initialVelocity;
    }
    else {
        float64 dt = static_cast<uint32>(*time - lastTime);
        dt /= 1e6;
        *velocity += dt * ((-springConstant * (*position)) - (dampingCoefficient * (*velocity)) + (*force / mass));
        *position += dt * (*velocity); //Use the already updated velocity even if it is not exact Forward Euler
    }
    lastTime = *time;

    return ok;
}

CLASS_REGISTER(MassSpringModelSol, "1.0")
} 

