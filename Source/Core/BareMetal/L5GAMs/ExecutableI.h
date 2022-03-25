/**
 * @file ExecutableI.h
 * @brief Header file for class ExecutableI
 * @date 19/07/2016
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

 * @details This header file contains the declaration of the class ExecutableI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXECUTORI_H_
#define EXECUTORI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief Classes that implement this interface are schedulable and can be
 *  executed by a GAMSchedulerI.
 */
class DLL_API ExecutableI {

public:
    /**
     * @brief Constructor. NOOP.
     */
    ExecutableI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ExecutableI();

    /**
     * \deprecated 
     * @brief Method called by a GAMSchedulerI to trigger the execution of the component.
     * @details Deprecated in favor of ErrorManagement::ErrorType Process.
     * @return true if the component is successfully executed.
     */
    virtual bool Execute();

    /**
     * @brief Method called by a GAMSchedulerI to trigger the execution of the component.
     * @return ErrorManagement::NoError if the component is successfully executed; ErrorManagement::Completed if the component
     *  wants to inform the scheduler that it should terminate its execution cycle; ErrorManagement::FatalError otherwise.
     */
    virtual ErrorManagement::ErrorType Process();

    /**
     * @brief Sets the address in memory where the signal which contains the last execution time of this component is stored.
     * @details The last execution time units are micro-seconds and are measured w.r.t. to the start of a cycle.
     * @param[in] timingSignalAddressIn the address of the timing signal.
     */
    void SetTimingSignalAddress(uint32 * const timingSignalAddressIn);

    /**
     * @brief Gets the address in memory where the signal which contains the last execution time is stored.
     * @details The last execution time units are micro-seconds and are measured w.r.t. to the start of a cycle.
     * @return the address in memory where the signal which contains the last execution time is stored.
     */
    inline uint32 *GetTimingSignalAddress();

private:

    uint32 * timingSignalAddress;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
uint32 * ExecutableI::GetTimingSignalAddress() {
    return timingSignalAddress;
}

}
#endif /* EXECUTORI_H_ */
	
