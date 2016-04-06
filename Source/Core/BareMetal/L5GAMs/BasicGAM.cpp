/**
 * @file BasicGAM.cpp
 * @brief Source file for class BasicGAM
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
 * the class BasicGAM (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicGAM.h"
#include "RealTimeDataDefContainer.h"
#include "RealTimeDataSourceContainer.h"
#include "RealTimeDataDefI.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicGAM::BasicGAM() :
        GAMI() {
}

/*void BasicGAM::SetUp() {
 // initialises the local status
 }*/

/*
 virtual void Execute(){
 // execution routine
 */

bool BasicGAM::ConfigureDataSourceLinks() {
    // it is virtual... can be overriden if the data are static

    inputReader = ReferenceT<BasicRealTimeDataSourceInputReader>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (inputReader.IsValid()) {
        inputReader->SetName("inputReader");
    }
    outputWriter = ReferenceT<BasicRealTimeDataSourceOutputWriter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (outputWriter.IsValid()) {
        outputWriter->SetName("outputWriter");
    }

    bool isReaderValid = inputReader.IsValid();
    bool isWriterValid = outputWriter.IsValid();
    bool ret = (isReaderValid && isWriterValid);

    if (ret) {
        if (inputReader.IsValid()) {
            inputReader->SetApplication(*application);
        }
        if (outputWriter.IsValid()) {
            outputWriter->SetApplication(*application);
        }
    }

    uint32 numberOfElements = Size();
    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        ReferenceT<RealTimeDataDefContainer> defContainer = Get(i);
        ret=defContainer.IsValid();
        if (ret) {
            uint32 numberOfDefs = defContainer->Size();
            for (uint32 j = 0u; (j < numberOfDefs) && (ret); j++) {
                ReferenceT<RealTimeDataDefI> def = defContainer->Get(j);
                ret=def.IsValid();
                if (ret) {
                    if (defContainer->IsInput()) {
                        ret = inputReader->AddVariable(def);
                    }
                    if (defContainer->IsOutput()) {
                        ret = outputWriter->AddVariable(def);
                    }
                }
                else{
                    REPORT_ERROR(ErrorManagement::FatalError,"The RealTimeDefContainer must contain RealTimeDataDefI objects");
                }
            }
        }
        else{
            REPORT_ERROR(ErrorManagement::FatalError,"The BasicGAM must contain RealTimeDataDefContainer objects");
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

Reference BasicGAM::GetInputReader() const {
    return inputReader;
}

Reference BasicGAM::GetOutputWriter() const {
    return outputWriter;
}

bool BasicGAM::IsSync() {
    bool ret = false;

    if (inputReader.IsValid()) {
        ret = inputReader->IsSync();
    }
    return ret;
}

}
