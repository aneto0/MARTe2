/**
 * @file GAMDataSource.h
 * @brief Header file for class GAMDataSource
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

 * @details This header file contains the declaration of the class DataSourceSignal
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMDATASOURCE_H_
#define GAMDATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "DataSourceI.h"
#include "MemoryArea.h"
#include "RealTimeApplication.h"
#include "RealTimeStateInfo.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief DataSourceSignalI implementation for the exchange of signals between GAM components.
 *
 * @details This implementation of the DataSourceSignalI interface is intend for the interchange
 *  of signals in real-time between GAM components. The DataSourceSignal instances are managed by a
 *  single DataSource which offers the memory back-end to store the signal data. It is implemented
 *  with a dual buffering mechanism which allows to load a default value when changing to a new state
 *  (if this signal was not being used in the current state).
 *
 * @details The syntax in the input configuration stream (see Initialise) has to be:
 *
 * +DataSourceSignal_Name= {\n
 *    Class = DataSourceSignal
 *    Type = "the variable type" (default "")\n
 *    Default = "the variable default value" (default "")\n
 *    Dimensions = "the variable dimensions" (default "")\n
 *    Samples = "how many samples will be produced for each cycle" (default 1)\n
 *     ...\n
 * }\n
 *
 * and it has to be contained inside a [DataSource] declaration.
 */
class DLL_API GAMDataSource: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
GAMDataSource    ();

    /**
     * @brief Destructor
     */
    virtual ~GAMDataSource();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx, uint32 bufferIdx, void *&signalAddress);

    virtual bool AllocateMemory();

    virtual const char8 *GetBrokerName(StructuredDataI &data, SignalDirection direction);

    virtual bool PrepareNextState(const RealTimeStateInfo &status);

    virtual bool GetInputBrokers(
            ReferenceContainer &inputBrokers,
            const char8 * functionName,
            void* gamMemPtr);

    virtual bool GetOutputBrokers(
            ReferenceContainer &outputBrokers,
            const char8 * functionName,
            void* gamMemPtr);

protected:
    void **signalMemory[2];

    void **signalMemoryIndex[2];

    HeapI *heap;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMDATASOURCE_H_ */

