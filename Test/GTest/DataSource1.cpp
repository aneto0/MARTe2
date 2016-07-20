/**
 * @file DataSource1.cpp
 * @brief Source file for class DataSource1
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
 * the class DataSource1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DataSource1.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

DataSource1::DataSource1() :
        DataSourceI() {

}

DataSource1::~DataSource1() {

}

bool DataSource1::Initialise(StructuredDataI & data) {
    return DataSourceI::Initialise(data);
}

uint32 DataSource1::GetCurrentBufferIndex() {
    return 0u;
}

uint32 DataSource1::GetNumberOfMemoryBuffers() {
    return 2u;
}

bool DataSource1::AllocateMemory() {
    return false;
}

bool DataSource1::GetSignalMemoryBuffer(uint32 signalIdx,
                                        uint32 bufferIdx,
                                        void *&signalAddress) {
    return true;
}

bool DataSource1::GetInputReaders(const char8 * const functionName,
                                            ReferenceContainer &output) {
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapInputBroker");
    ReferenceContainer brokers;
    return output.Insert(broker);
}


bool DataSource1::GetOutputWriters(const char8 * const functionName, ReferenceContainer &output){
    ReferenceT<MemoryMapInputBroker> broker("MemoryMapOutputBroker");
    ReferenceContainer brokers;
    return output.Insert(broker);
}

bool DataSource1::PrepareNextState(const RealTimeStateInfo &status) {
    return true;
}

bool DataSource1::ChangeState() {
    return true;
}

CLASS_REGISTER(DataSource1, "1.0");
}
