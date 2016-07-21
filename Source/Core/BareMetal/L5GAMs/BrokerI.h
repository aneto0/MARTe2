/**
 * @file BrokerI.h
 * @brief Header file for class eBrokerI
 * @date 11/04/2016
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

 * @details This header file contains the declaration of the class DataSourceBrokerI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BROKERI_H_
#define BROKERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceSignal.h"
#include "GAMSignalI.h"
#include "RealTimeApplication.h"
#include "ReferenceContainer.h"
#include "ExecutableI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Defines the interface between DataSourceSignalI and GAMSignalI signals.
 * @details A class that implements this interface is capable of connecting signals from
 *  DataSourceSignalI components with signals from GAMSignalI components.
 */
class BrokerI: public ExecutableI {

public:

    BrokerI();

    virtual ~BrokerI();

    virtual bool Init(SignalDirection direction,
                      DataSourceI *dataSourceIn,
                      const char8 * const functionName) = 0;

    uint32 GetNumberOfCopies();

    uint32 GetCopyByteSize(uint32 copyIdx);

    uint32 GetCopyOffset(uint32 copyIdx);

    void *GetFunctionPointer(uint32 copyIdx);

protected:
    bool InitFunctionPointers(SignalDirection direction,
                              ReferenceT<DataSourceI> dataSource,
                              const char8 * const functionName);

    bool InitFunctionPointers2(SignalDirection direction,
                               DataSourceI &dataSource,
                               const char8 * const functionName,
                               void *gamMemoryAddress);

    uint32 numberOfCopies;

private:

    uint32 *copyByteSize;

    uint32 *copyOffset;

    void **functionSignalPointers;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEBROKERI_H_ */

