/**
 * @file GAM.cpp
 * @brief Source file for class GAM
 * @date 18/02/2016
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
 * the class GAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAM.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataDefI.h"
#include "RealTimeDataSourceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAM::GAM():
        GAMI() {
    localData = NULL_PTR(StructuredDataI*);
    numberOfSupportedStates = 0u;
    supportedStates = NULL_PTR(StreamString *);
    group = NULL_PTR(GAMGroup*);
    application = NULL_PTR(RealTimeApplication *);
    inputReader = ReferenceT<RealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (inputReader.IsValid()) {
        inputReader->SetName("inputReader");
    }
    outputWriter = ReferenceT<RealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (outputWriter.IsValid()) {
        outputWriter->SetName("outputWriter");
    }
}


bool GAM::ConfigureDataSourceLinks() {
    // it is virtual... can be overriden if the data are static

    bool isReaderValid = inputReader.IsValid();
    bool isWriterValid = outputWriter.IsValid();
    bool ret = (isReaderValid && isWriterValid);
    uint32 numberOfElements = Size();
    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        ReferenceT<RealTimeDataDefContainer> defContainer = Get(i);
        if (defContainer.IsValid()) {
            uint32 numberOfDefs = defContainer->Size();
            for (uint32 j = 0u; (j < numberOfDefs) && (ret); j++) {
                ReferenceT<RealTimeDataDefI> def = defContainer->Get(j);
                if (def.IsValid()) {
                    if (defContainer->IsInput()) {
                        ret = inputReader->AddVariable(def);
                    }
                    if (defContainer->IsOutput()) {
                        ret = outputWriter->AddVariable(def);
                    }
                }
            }
        }
    }
    if (ret) {
        ret = inputReader->Finalise();
    }
    if (ret) {
        ret = outputWriter->Finalise();
    }

    return ret;
}


Reference GAM::GetInputReader() const{
    return inputReader;
}

Reference GAM::GetOutputWriter() const{
    return outputWriter;
}

bool GAM::IsSync() {
    bool ret = false;
    bool isReaderValid = inputReader.IsValid();
    bool isWriterValid = outputWriter.IsValid();

    if (isReaderValid && isWriterValid) {
        ret = (inputReader->IsSync() || outputWriter->IsSync());
    }
    return ret;
}

}
