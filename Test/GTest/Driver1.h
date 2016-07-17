/**
 * @file Driver1.h
 * @brief Header file for class Driver1
 * @date Jun 8, 2016
 * @author aneto
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

 * @details This header file contains the declaration of the class Driver1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_GTEST_DRIVER1_H_
#define 		TEST_GTEST_DRIVER1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "MemoryMapBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Driver1: public DataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    Driver1();

    virtual ~Driver1();

    virtual bool Initialise(StructuredDataI & data);

    virtual uint32 GetCurrentBufferIndex();

    virtual uint32 GetNumberOfMemoryBuffers();

    virtual bool GetSignalMemoryBuffer(uint32 functionIdx,
                                       uint32 functionSignalIdx,
                                       uint32 bufferIdx,
                                       void *&signalAddress);

    virtual bool GetSignalMemoryBuffer(uint32 signalIdx,
                                       uint32 bufferIdx,
                                       void *&signalAddress);

    virtual bool AllocateMemory();

    virtual ReferenceT<BrokerI> GetInputReader(const char8 * const functionName);

    virtual ReferenceT<BrokerI> GetOutputWriter(const char8 * const functionName);

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_GTEST_Driver1_H_ */

