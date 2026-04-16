/**                                                                              
 * @file DeltaTSol.h                                                                 
 * @brief Header file for class DeltaTSol                                         
 * @date 16/04/2026                                                                 
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
 * software distributed under the Licence is distributed on an  'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express           
 * or implied. See the Licence permissions and limitations under the Licence.    

 * @details This header file contains the declaration of the class DeltaTSol      
 * with all of its public, protected and private members. It may also include    
 * definitions for inline methods which need to be visible to the compiler.      
 */                                                                              

#ifndef _TUTORIAL_DELTATSOL_H_
#define _TUTORIAL_DELTATSOL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

///AUTO-GENERATED: CLASS DOC. DO NOT EDIT!
/**
 * @details
 * +DeltaTSolInstance = {
 *     Class = DeltaTSol
 *     DeltaTDiv = 0 //Required. The time difference will be divided by this value before being sent to the output signal
 *     InputSignals = {
 *         AbsoluteTime = {//Absolute time in user defined units (e.g. microseconds) used to calculate the time difference. After division by DeltaTDiv, the output signal will be in micro-seconds.
 *             Type = uint64
 *             DataSource = "DDB1"
 *         }
 *     }
 *     OutputSignals = {
 *         DeltaT = {//Time difference between the current and previous AbsoluteTime input signal
 *             Type = uint32
 *             DataSource = "DDB1"
 *         }
 *     }
 * }
 */
///AUTO-GENERATED: END OF CLASS DOC. DO NOT EDIT!

class DeltaTSol : public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. 
     * @details TODO.
     */
    DeltaTSol ();

    /**
     * @brief Destructor. 
     * @details TODO.
     */
    virtual ~DeltaTSol ();

    /**
     * @brief Initialises the GAM. TODO
     * @details TODO. 
     * @param[in] data The GAM configuration parameters. TODO
     * @return TODO.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Perform post-initialise configurations. TODO 
     * @return TODO
     */
    virtual bool Setup();

    /**
     * @brief Executes the GAM logic. TODO
     * @return TODO.
     */
     virtual bool Execute();

private:
    ///AUTO-GENERATED: PARAMETERS. DO NOT EDIT!

    /**
     * The time difference will be divided by this value before being sent to the output signal
     */
    MARTe::uint32 deltaTDiv;
    ///AUTO-GENERATED: END OF PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: SIGNALS. DO NOT EDIT!

    /**
     * Absolute time in user defined units (e.g. microseconds) used to calculate the time difference. After division by DeltaTDiv, the output signal will be in micro-seconds.
     */
    MARTe::uint64 *absoluteTime;

    /**
     * Time difference between the current and previous AbsoluteTime input signal
     */
    MARTe::uint32 *deltaT;
    ///AUTO-GENERATED: END OF SIGNALS. DO NOT EDIT!

    /**
     * Last absolute time
     */
    MARTe::uint64 lastAbsoluteTime;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* _TUTORIAL_DELTATSOL_H_*/
