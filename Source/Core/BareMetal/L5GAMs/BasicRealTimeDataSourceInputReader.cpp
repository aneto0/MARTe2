/**
 * @file BasicRealTimeDataSourceInputReader.cpp
 * @brief Source file for class BasicRealTimeDataSourceInputReader
 * @date 21/03/2016
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

 * @details This source file contains the definition of all the methods for
 * the class BasicRealTimeDataSourceInputReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceInputReader.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

BasicRealTimeDataSourceInputReader::BasicRealTimeDataSourceInputReader() :
        BasicRealTimeDataSourceBroker() {

}

bool BasicRealTimeDataSourceInputReader::Read(const uint8 activeDataSourceBuffer) const {

    bool ret = finalised;
    for (uint32 i = 0u; (i < GAMOffsets.GetSize()) && (ret); i++) {
        void ** DSPointer = NULL_PTR(void **);
        ret = DSPointers[activeDataSourceBuffer].Peek(i, DSPointer);
        if (ret) {
            ret = (DSPointer != NULL);
        }
        void * GAMPointer = NULL_PTR(void *);
        if (ret) {
            ret = GAMPointers.Peek(i, GAMPointer);
        }
        if (ret) {
            ret = (GAMPointer != NULL);
        }
        uint32 size = 0u;
        if (ret) {
            ret = sizes.Peek(i, size);
        }
        BasicRealTimeDataSourceDef *dsDef = NULL_PTR(BasicRealTimeDataSourceDef *);
        if (ret) {
            ret = dataSources.Peek(i, dsDef);
        }
        if (ret) {
            dsDef->ReadStart();
            ret = MemoryOperationsHelper::Copy(GAMPointer, *DSPointer, size);
            dsDef->ReadEnd();
        }
    }

    return ret;
}
CLASS_REGISTER(BasicRealTimeDataSourceInputReader, "1.0")

}
