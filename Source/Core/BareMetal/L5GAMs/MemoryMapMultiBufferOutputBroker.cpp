/**
 * @file MemoryMapMultiBufferOutputBroker.cpp
 * @brief Source file for class MemoryMapMultiBufferOutputBroker
 * @date 12/04/2018
 * @author Giuseppe Ferro
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
 * the class MemoryMapMultiBufferOutputBroker (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MemoryMapMultiBufferOutputBroker.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MemoryMapMultiBufferOutputBroker::MemoryMapMultiBufferOutputBroker() :
        MemoryMapMultiBufferBroker() {

}

MemoryMapMultiBufferOutputBroker::~MemoryMapMultiBufferOutputBroker() {

}

/*lint -e{613} a valid Init of this class is a pre-condition for the Execute method and it assures dataSource to be != NULL*/
bool MemoryMapMultiBufferOutputBroker::Execute() {
    dataSource->PrepareOutputOffsets();
    bool ret = MemoryMapMultiBufferBroker::CopyOutputs();
    return ret;
}

CLASS_REGISTER(MemoryMapMultiBufferOutputBroker, "1.0")

}
