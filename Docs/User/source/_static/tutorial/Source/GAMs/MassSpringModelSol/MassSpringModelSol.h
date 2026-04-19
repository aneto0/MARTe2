/**                                                                              
 * @file MassSpringModelSol.h                                                                 
 * @brief Header file for class MassSpringModelSol                                         
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
 * software distributed under the Licence is distributed on an  'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express           
 * or implied. See the Licence permissions and limitations under the Licence.    

 * @details This header file contains the declaration of the class MassSpringModelSol      
 * with all of its public, protected and private members. It may also include    
 * definitions for inline methods which need to be visible to the compiler.      
 */                                                                              

#ifndef _TUTORIAL_MASSSPRINGMODELSOL_H_
#define _TUTORIAL_MASSSPRINGMODELSOL_H_

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
 * +MassSpringModelSolInstance = {
 *     Class = MassSpringModelSol
 *     Mass = 1.0 //Optional. The mass in kg
 *     SpringConstant = 10.0 //Optional. The spring constant in N/m
 *     DampingCoefficient = 0.5 //Optional. The damping coefficient in Ns/m
 *     InitialPosition = 0.0 //Optional. The initial position in m
 *     InitialVelocity = 0.0 //Optional. The initial velocity in m/s
 *     InputSignals = {
 *         Time = {//Input time in microseconds
 *             Type = uint32
 *             DataSource = "DDB1"
 *         }
 *         Force = {//Input force in N
 *             Type = float64
 *             DataSource = "DDB1"
 *         }
 *     }
 *     OutputSignals = {
 *         Position = {//Position in m
 *             Type = float64
 *             DataSource = "DDB1"
 *         }
 *         Velocity = {//Velocity in m/s
 *             Type = float64
 *             DataSource = "DDB1"
 *         }
 *     }
 * }
 */
///AUTO-GENERATED: END OF CLASS DOC. DO NOT EDIT!

class MassSpringModelSol : public MARTe::GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. 
     * @details TODO.
     */
    MassSpringModelSol ();

    /**
     * @brief Destructor. 
     * @details TODO.
     */
    virtual ~MassSpringModelSol ();

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
     * The mass in kg
     */
    MARTe::float64 mass;

    /**
     * The spring constant in N/m
     */
    MARTe::float64 springConstant;

    /**
     * The damping coefficient in Ns/m
     */
    MARTe::float64 dampingCoefficient;

    /**
     * The initial position in m
     */
    MARTe::float64 initialPosition;

    /**
     * The initial velocity in m/s
     */
    MARTe::float64 initialVelocity;
    ///AUTO-GENERATED: END OF PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: SIGNALS. DO NOT EDIT!

    /**
     * Input time in microseconds
     */
    MARTe::uint32 *time;

    /**
     * Input force in N
     */
    MARTe::float64 *force;

    /**
     * Position in m
     */
    MARTe::float64 *position;

    /**
     * Velocity in m/s
     */
    MARTe::float64 *velocity;
    ///AUTO-GENERATED: END OF SIGNALS. DO NOT EDIT!
   
    /**
     * Last time value.
     */ 
    MARTe::uint32 lastTime;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* _TUTORIAL_MASSSPRINGMODELSOL_H_*/
