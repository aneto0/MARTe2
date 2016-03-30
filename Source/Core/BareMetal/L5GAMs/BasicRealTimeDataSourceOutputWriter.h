/**
 * @file BasicRealTimeDataSourceOutputWriter.h
 * @brief Header file for class BasicRealTimeDataSourceOutputWriter
 * @date 21/mar/2016
 * @author pc
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

 * @details This header file contains the declaration of the class BasicRealTimeDataSourceOutputWriter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICREALTIMEDATASOURCEOUTPUTWRITER_H_
#define BASICREALTIMEDATASOURCEOUTPUTWRITER_H_

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
 * @brief Writes data to the RealTimeDataSource.
 */
class DLL_API BasicRealTimeDataSourceOutputWriter: public RealTimeDataSourceBroker {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    BasicRealTimeDataSourceOutputWriter();

    /**
     * @brief Writes data to the RealTimeDataSource.
     * @details After the configuration of the interface between GAM and RealTimeDataSource
     * (see RealTimeDataSourceBroker), copies data the GAM variables to the RealTimeDataSource.
     * @param[in] activeDataSourceBuffer is the buffer index to be used. This parameter must change
     * from 0 to 1 on each state switch.
     * @return false in case of errors, true otherwise.
     */
    bool Write(const uint8 activeDataSourceBuffer) const;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICREALTIMEDATASOURCEOUTPUTWRITER_H_ */

