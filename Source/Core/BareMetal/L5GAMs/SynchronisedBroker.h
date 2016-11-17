/**
 * @file SynchronisedBroker.h
 * @brief Header file for class SynchronisedBroker
 * @date 17/11/2016
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

 * @details This header file contains the declaration of the class SynchronisedBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L5GAMS_SYNCHRONISEDBROKER_H_
#define SOURCE_CORE_BAREMETAL_L5GAMS_SYNCHRONISEDBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BrokerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Synchronous BrokerI implementation.
 * @details This class calls Synchronise on the DataSourceI every-time it is executed..
 */
class DLL_API SynchronisedBroker: public BrokerI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Default constructor. NOOP.
     */
    SynchronisedBroker();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~SynchronisedBroker();

    /**
     * @see BrokerI::Init.
     * @detail After this function is executed the Execute will call Synchronise on the \a dataSourceIn.
     */
    virtual bool Init(SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void *gamMemoryAddress);

    /**
     * @brief Calls Synchronise on the DataSourceI.
     * @return true if the synchronisation call is successful.
     */
    virtual bool Execute();
private:
    /**
     * The DataSourceI where to call Synchronise
     */
    DataSourceI *dataSource;
};

}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_SYNCHRONISEDBROKER_H_ */
	
