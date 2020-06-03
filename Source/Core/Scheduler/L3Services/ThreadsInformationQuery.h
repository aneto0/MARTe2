/**
 * @file ThreadsInformationQuery.h
 * @brief Header file for class ThreadsInformationQuery
 * @date 30/03/2019
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

 * @details This header file contains the declaration of the class ThreadsInformationQuery
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_SCHEDULER_L3SERVICES_THREADSINFORMATIONQUERY_H_
#define SOURCE_CORE_SCHEDULER_L3SERVICES_THREADSINFORMATIONQUERY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief Lists all the known threading information.
 */
class ThreadsInformationQuery: public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    ThreadsInformationQuery ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ThreadsInformationQuery ();

    /**
     * @brief See Object::ExportData. Lists all the information known about all the currently spawned threads.
     * @param[out] data a new entry will be added for every thread, listing the following properties: Name, Affinity, PriorityClass, State and PriorityLevel.
     * @return true if all the thread properties were successfully written into \a data.
     */
    virtual bool ExportData(StructuredDataI & data);
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_SCHEDULER_L3SERVICES_THREADSINFORMATIONQUERY_H_ */

