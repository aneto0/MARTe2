/**
 * @file BasicRealTimeDataSourceInputReader.h
 * @brief Header file for class BasicRealTimeDataSourceInputReader
 * @date 21/03/2016
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceInputReader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEINPUTREADER_H_
#define BASICREALTIMEDATASOURCEINPUTREADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataSourceBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Reads data from the RealTimeDataSource.
 */
class BasicRealTimeDataSourceInputReader: public RealTimeDataSourceBroker {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    BasicRealTimeDataSourceInputReader();

    /**
     * @brief Reads data from the RealTimeDataSource.
     * @details After the configuration of the interface between GAM and RealTimeDataSource
     * (see RealTimeDataSourceBroker), copies data from the RealTimeDataSource into the GAM variables.
     * @param[in] activeDataSourceBuffer is the buffer index to be used. This parameter must change
     * from 0 to 1 on each state switch.
     * @return false in case of errors, true otherwise.
     */
    bool Read(const uint8 activeDataSourceBuffer) const;

    /**
     * @brief Performs an advanced synchronising read operation.
     * @details If this reader is synchronised (namely is linked to a data source which provides a spin-lock semaphore)
     * this function waits that the spin-lock event semaphore is posted by the data source before performing the read
     * operation. It is possible make a sub-sampling (with loss of data) passing the number of read operations (\a numberOfReads)
     * that this function has to achieve (if it is synchronized the function will wait on the spin-lock before each read operation)
     * or impose that the function cannot terminate before a specified amount of time (\a sampleTime).
     * @param[in] activeDataSourceBuffer  is the buffer index to be used. This parameter must change
     * from 0 to 1 on each state switch.
     * @param[in] sampleTime is the time such that this function cannot exit before.
     * @param[in] numberOfReads is the number of consecutive reads that this function performs. If this reader is
     * synchronised, each read operation will be sinchronising.
     * @param[in] timeout is the maximum time to wait on the spin-lock semaphore.
     * @param[in] sleepTime denotes how much time the cpu will be released during the spin-lock waiting loop and the
     * eventual wait on \a sampleTime.
     * @return false if the read operation fails or the spin-lock wait fails because of the timeout, true otherwise.
     */
    bool SynchroniseOnSpinLockSem(const uint8 activeDataSourceBuffer,
                                  float64 sampleTime = 0.0,
                                  uint32 numberOfReads = 1u,
                                  TimeoutType timeout = TTInfiniteWait,
                                  float64 sleepTime = 0.0);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEINPUTREADER_H_ */

