/**
 * @file Driver1.cpp
 * @brief Source file for class Driver1
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

 * @details This source file contains the definition of all the methods for
 * the class Driver1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Driver1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Driver1::Driver1() :
        DataSourceI() {

}

Driver1::~Driver1() {

}

bool Driver1::Initialise(StructuredDataI & data) {
    return DataSourceI::Initialise(data);
}

uint32 Driver1::GetCurrentBufferIndex() {
    return 0u;
}

uint32 Driver1::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool Driver1::GetSignalMemoryBuffer(uint32 functionIdx,
                                    uint32 functionSignalIdx,
                                    uint32 bufferIdx,
                                    void *&signalAddress) {
    return true;
}

bool Driver1::AllocateMemory() {
    return false;
}

bool Driver1::GetSignalMemoryBuffer(uint32 signalIdx,
                                    uint32 bufferIdx,
                                    void *&signalAddress) {
    return true;
}

ReferenceT<BrokerI> Driver1::GetInputReader(const char8 * const functionName) {
    ReferenceT<MemoryMapBroker> broker("MemoryMapBroker");
    return broker;
}

ReferenceT<BrokerI> Driver1::GetOutputWriter(const char8 * const functionName) {
    ReferenceT<MemoryMapBroker> broker("MemoryMapBroker");
    return broker;
}

bool Driver1::PrepareNextState(const RealTimeStateInfo &status) {
    return true;
}

bool Driver1::ChangeState() {
    return true;
}

CLASS_REGISTER(Driver1, "1.0");
}
