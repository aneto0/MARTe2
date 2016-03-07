/**
 * @file RealTimeDataSourceDefContainer.h
 * @brief Header file for class RealTimeDataSourceDefContainer
 * @date 01/03/2016
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

 * @details This header file contains the declaration of the class RealTimeDataSourceDefContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEDEFCONTAINER_H_
#define REALTIMEDATASOURCEDEFCONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataDefI.h"
#include "GAM.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class RealTimeDataSourceDefContainer: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    RealTimeDataSourceDefContainer();

    virtual bool Initialise(StructuredDataI & data);

    bool AddDataDefinition(ReferenceT<GAM> gam);

    bool Verify();

private:

    bool AddSingleDataDefinition(ReferenceT<RealTimeDataDefI> definition,
                                 ReferenceT<GAM> gam,
                                 bool isProducer,
                                 bool isConsumer,
                                 StreamString defaultPath = "");

    uint32 numberOfInitialDDBs;

    bool final;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEDEFCONTAINER_H_ */

