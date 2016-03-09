/**
 * @file RealTimeDataSourceBroker.h
 * @brief Header file for class RealTimeDataSourceBroker
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

 * @details This header file contains the declaration of the class RealTimeDataSourceBroker
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATASOURCEBROKER_H_
#define REALTIMEDATASOURCEBROKER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "MemoryArea.h"
#include "ReferenceT.h"
#include "RealTimeApplication.h"
#include "RealTimeDataDefI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class RealTimeDataSourceBroker: public ReferenceContainer {

public:
    RealTimeDataSourceBroker();


    void SetApplication(ReferenceT<RealTimeApplication> rtApp);

    bool AddVariable(ReferenceT<RealTimeDataDefI> def, void* ptr=NULL);

    void *GetData(uint32 i);

protected:


    ReferenceT<RealTimeApplication> application;

    MemoryArea memory;

    StaticList<void *> GAMPointers;

    StaticList<void **> DSPointers;

    StaticList<uint32> sizes;

    StaticList<uint32> chunkIndex;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATASOURCEBROKER_H_ */

