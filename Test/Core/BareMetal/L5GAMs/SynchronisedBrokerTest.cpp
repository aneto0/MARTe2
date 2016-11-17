/**
 * @file SynchronisedBrokerTest.cpp
 * @brief Source file for class SynchronisedBrokerTest
 * @date Nov 17, 2016 TODO Verify the value and format of the date
 * @author aneto TODO Verify the name and format of the author
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

 * @details This source file contains the definition of all the methods for
 * the class SynchronisedBrokerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "SynchronisedBrokerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class SynchronisedBrokerDataSourceITestHelper: public MARTe::DataSourceI {
public:

    SynchronisedBrokerDataSourceITestHelper() {
        synchroniseCalled = false;
    }

    virtual ~SynchronisedBrokerDataSourceITestHelper();

    virtual bool AllocateMemory() {
        return true;
    }

    virtual MARTe::uint32 GetNumberOfMemoryBuffers() {
        return 0u;
    }

    virtual bool GetSignalMemoryBuffer(const MARTe::uint32 signalIdx,
                                       const MARTe::uint32 bufferIdx,
                                       void *&signalAddress) {
        return 0;
    }

    virtual const MARTe::char8 *GetBrokerName(MARTe::StructuredDataI &data,
                                              const MARTe::SignalDirection direction) {
        return NULL;
    }

    virtual bool PrepareNextState(const MARTe::char8 * const currentStateName,
                                  const MARTe::char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(MARTe::ReferenceContainer &inputBrokers,
                                 const MARTe::char8* const functionName,
                                 void * const gamMemPtr) {
        return true;
    }

    virtual bool GetOutputBrokers(MARTe::ReferenceContainer &outputBrokers,
                                  const MARTe::char8* const functionName,
                                  void * const gamMemPtr) {
        return true;
    }

    virtual bool GetPreBrokers(MARTe::ReferenceContainer &brokers,
                               const MARTe::char8* const functionName,
                               void * const gamMemPtr) {
        return true;
    }

    virtual bool GetPostBrokers(MARTe::ReferenceContainer &brokers,
                                const MARTe::char8* const functionName,
                                void * const gamMemPtr) {
        return true;
    }

    virtual bool Synchronise() {
        synchroniseCalled = true;
    }

    bool synchroniseCalled;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SynchronisedBrokerTest::TestConstructor() {
    using namespace MARTe;
    SynchronisedBroker test;
    return true;
}

bool SynchronisedBrokerTest::TestInit() {
    using namespace MARTe;
    SynchronisedBroker test;
    SynchronisedBrokerDataSourceITestHelper dataSource;
    return test.Init(OutputSignals, dataSource, NULL, NULL);
}

bool SynchronisedBrokerTest::TestExecute() {
    using namespace MARTe;
    SynchronisedBroker test;
    SynchronisedBrokerDataSourceITestHelper dataSource;
    bool ok = test.Init(OutputSignals, dataSource, NULL, NULL);
    if(ok) {
        ok = test.Execute();
    }
    if(ok) {
        ok = dataSource.synchroniseCalled;
    }
    return ok;
}

