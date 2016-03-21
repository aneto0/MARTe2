/**
 * @file RealTimeDataSource.h
 * @brief Header file for class RealTimeDataSource
 * @date 09/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCE_H_
#define REALTIMEDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryArea.h"
#include "StaticList.h"
#include "ReferenceContainer.h"
#include "BasicRealTimeDataSourceDef.h"
#include "RealTimeDataDefI.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * @brief The memory database shared between GAMs
 */
class RealTimeDataSource: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @details By default the definition is not final. This means that it is possible adds
     * more data source definitions than the ones defined in the configuration data.
     */
    RealTimeDataSource();

    /**
     * @brief Initialises from configuration data.
     * @details The following fields can be specified in \a data:
     *
     *   HeapMemory = the name of the heap memory
     *
     * By default (if not specified) the definition is not final. If final, it is not possible adding
     * more data source definitions than the ones specified in \a data.
     * @param[in] data contains the configuration data.
     * @return false in case of errors, true otherwise.
     */
    virtual bool Initialise(StructuredDataI & data);



    /**
     * @brief Allocates the memory for each data source defined.
     * @details Browses the tree and for each RealTimeDataSourceDef found allocates the memory for the specific type.
     * @return false in case of errors, true otherwise.
     */
    bool Allocate();

private:



    StreamString heapName;

    /**
     * The memory for all the data sources.
     */
    MemoryArea memory;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCE_H_ */

