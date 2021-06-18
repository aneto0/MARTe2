/**
 * @file GAMBareScheduler.h
 * @brief Header file for class GAMBareScheduler
 * @date 01/06/2021
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class GAMBareScheduler
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMBARESCHEDULER_H_
#define GAMBARESCHEDULER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "CompilerTypes.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "Reference.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    class GAMBareScheduler: public GAMSchedulerI {

        public:
            CLASS_REGISTER_DECLARATION()

            /**
             * @brief Construct a new GAMBareScheduler object
             */
            GAMBareScheduler();

            /**
             * @brief Destroy the GAMBareScheduler object
             */
            virtual ~GAMBareScheduler();

            /**
             * @brief Starts the single thread execution for the current state
             * @return ErrorManagement::NoError if the base Scheduler succeeds
             */
            virtual ErrorManagement::ErrorType StartNextStateExecution();

            /**
             * @brief Changes the flag which keeps the cycle execution running
             * @return ErrorManagement::NoError always 
             */
            virtual ErrorManagement::ErrorType StopCurrentStateExecution();

            /**
             * @brief Calls the parent class to generate the schedulable table and assigns
             *          the Realtime Application reference
             * @param realTimeAppIn The RealTime Application using the scheduler
             * @return true If the table can be successfully constructed and the RealTimeApplication reference is valid
             */
            virtual bool ConfigureScheduler(Reference realTimeAppIn);
            
            /**
             * @brief Does nothing, should contain custom routines to prepare the specific scheduler
             *        to the next state execution
             */
            virtual void CustomPrepareNextState();


        private:

            /**
             * @brief Flag used internally to control the continuation of the execution
             */
            bool isAlive;

            /**
             * @brief Reference to the RealTimeApplication
             */
            ReferenceT<RealTimeApplication> realTimeApplication;

            /**
             * @brief Pointer to the scheduled states
             */
            ScheduledState * const * scheduledStates;

            /**
             * @brief Executes a single cycle of the RealTimeApplication executables for the specified thread identifier
             * @param threadId Identifier of the thread 
             */
            void Cycle(uint32 threadId);
    };

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
}

#endif /* GAMBARESCHEDULER_H_ */
