/**
 * @file RealTimeDataSource.h
 * @brief Header file for class RealTimeDataSource
 * @date 29/02/2016
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

#include "Object.h"
#include "MemoryArea.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class RealTimeDataSource: public ReferenceContainer {

public:

    virtual bool AddDataDefinition(RealTimeDataDefContainer &definitionContainer);


    /**
     * @brief retrieves the variable address in the RealTimeDataSource.
     * @return the variable address in the RealTimeDataSource.
     */
    const char8 *GetName();

    void SetName(const char8* pathName);

    /**
     * @brief Retrieves the variable type.
     * @return the variable type.
     */
    const char8 *GetType();

    /**
     * @brief Initialises the container and reads the variable address and type from the StructuredData
     * in input.
     */
    virtual bool Initialise(StructuredDataI &data);

    /*
     RealTimeDataSource();

     bool AddDefinitionInterface(RealTimeDataDefContainer &dataDefinition,
     uint32 userId,
     bool isConsumer,
     bool isProducer);
     */
private:

    bool AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                 const char8 * userName,
                                 const char8 * threadName,
                                 const char8 * stateName,
                                 bool isProducer,
                                 bool isConsumer);

    /*
     bool AddSingleData(ReferenceT<RealTimeDataDefI> singleDefinition,
     uint32 userId,
     bool isConsumer,
     bool isProducer);

     MemoryArea memory;

     StaticList<RealTimeDataSourceRecord> records;*/

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCE_H_ */

