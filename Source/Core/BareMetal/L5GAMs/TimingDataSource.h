/**
 * @file TimingDataSource.h
 * @brief Header file for class TimingDataSource
 * @date 01/08/2016
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class TimingDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TimingDataSource_H_
#define TimingDataSource_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAMDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief GAMDataSource implementation that is used to store the executions timing of the MARTe components.
 * @details For every Thread in the application it will generate a signal named STATE_NAME.THREAD_NAME_CycleTime,
 * STATE_NAME is the object name of the RealTimeState instance and THREAD_NAME is the object name of the RealTimeThread instance.
 * The type of this signal is uint32 and it holds the thread cycle time.
 * For every GAM in the application it will generate three signals named GAM_NAME_ReadTime, GAM_NAME_WriteTime,
 * GAM_NAME_ExecTime where GAM_NAME is the object name of the GAM instance. The type of these signals is uint32.
 * The GAM_NAME_ReadTime holds the time elapsed from the beginning of the cycle until all the input brokers
 *  for this GAM_NAME have been executed. The GAM_NAME_WriteTime holds the time elapsed from the beginning of the cycle
 *  until all the output brokers for this GAM_NAME have been executed. The GAM_NAME_ExecTime holds the time elapsed
 *  from the beginning of the cycle until this GAM_NAME has finished its execution.
 */
class DLL_API TimingDataSource: public GAMDataSource {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    TimingDataSource();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~TimingDataSource();

    /**
     * @brief see GAMDataSource::Initialise.
     * @details Verifies that there are no producers assigned to this DataSourceI. The timing data will be produced by
     * the GAMSchedulerI and not by GAM instances.
     * @param[in] data see GAMDataSource::Initialise.
     * @return true if GAMDataSource::Initialise returns true and if there are no producers assigned to this DataSourceI.
     */
    virtual bool SetConfiguredDatabase(StructuredDataI & data);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TimingDataSource_H_ */

