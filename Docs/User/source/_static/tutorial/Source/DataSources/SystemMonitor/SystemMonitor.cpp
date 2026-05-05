/**                                                                             
 * @file SystemMonitor.cpp                                                                
 * @brief Source file for class SystemMonitor                                        
 * @date 17/04/2026                                                                
 * @author cabrian                                                            
 *                                                                              
 * @copyright Copyright 2026 F4E | European Joint Undertaking for ITER and      
 * the Development of Fusion Energy ('Fusion for Energy').                      
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved      
 * by the European Commission - subsequent versions of the EUPL (the 'Licence') 
 * You may not use this work except in compliance with the Licence.             
 * You may obtain a copy of the Licence at: https://eupl.eu/                    
 *                                                                              
 * @warning Unless required by applicable law or agreed to in writing,          
 * software distributed under the Licence is distributed on an 'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express          
 * or implied. See the Licence permissions and limitations under the Licence.   

 * @details This source file contains the definition of all the methods for     
 * the class SystemMonitor (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SystemMonitor.h"
#include "StructuredDataIHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

SystemMonitor::SystemMonitor() :
        MARTe::DataSourceI () {
    using namespace MARTe;
    ///AUTO-GENERATED: CTOR PARAMETERS. DO NOT EDIT!

    nicName = "";
    ///AUTO-GENERATED: END OF CTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: CTOR SIGNALS. DO NOT EDIT!

    rxPackets = 0;
    txPackets = 0;
    rxDropped = 0;
    txDropped = 0;
    ///AUTO-GENERATED: END OF CTOR SIGNALS. DO NOT EDIT!

}

SystemMonitor::~SystemMonitor() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF DTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: DTOR SIGNALS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF DTOR SIGNALS. DO NOT EDIT!
    
    (void) txPacketsFile.Close();
    (void) rxPacketsFile.Close();
    (void) txDroppedFile.Close();
    (void) rxDroppedFile.Close();
}

bool SystemMonitor::Initialise(MARTe::StructuredDataI &data) {
    using namespace MARTe;
    bool ok = DataSourceI::Initialise(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: INITIALISE PARAMETERS. DO NOT EDIT!

    if (ok) {
        ok = helper.Read("NICName", nicName);
    }
    ///AUTO-GENERATED: END OF INITIALISE PARAMETERS. DO NOT EDIT!
      
    File *files[] = {&txPacketsFile, &rxPacketsFile, &txDroppedFile, &rxDroppedFile};
    StreamString statName[] = {"tx_packets", "rx_packets", "tx_dropped", "rx_dropped"};
    uint32 numberOfFiles = sizeof(files) / sizeof(files[0]);
    for (uint32 i=0; (i<numberOfFiles) && (ok); i++) {
        StreamString filename;
        ok = filename.Printf("/sys/class/net/%s/statistics/%s", nicName.Buffer(), statName[i].Buffer());
        if (ok) {
            ok = files[i]->Open(filename.Buffer(), (File::ACCESS_MODE_R));
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "Failed to Open file %s", filename.Buffer());
            }
        }
    }


    return ok;
}

bool SystemMonitor::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!

    if (ok) {
        ok = (GetNumberOfSignals() == 4);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of signals. Expected 4 and read %d", GetNumberOfSignals());
        }
    }
    if (ok) {
        TypeDescriptor signalTypeRXPackets = GetSignalType(0);
        ok = (signalTypeRXPackets == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type at index 0");
        }
    }
    if (ok) {
        TypeDescriptor signalTypeTXPackets = GetSignalType(1);
        ok = (signalTypeTXPackets == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type at index 1");
        }
    }
    if (ok) {
        TypeDescriptor signalTypeRXDropped = GetSignalType(2);
        ok = (signalTypeRXDropped == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type at index 2");
        }
    }
    if (ok) {
        TypeDescriptor signalTypeTXDropped = GetSignalType(3);
        ok = (signalTypeTXDropped == UnsignedInteger64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type at index 3");
        }
    }
    ///AUTO-GENERATED: END OF SETUP SIGNALS. DO NOT EDIT!
    return ok;
}

bool SystemMonitor::Synchronise() {
    using namespace MARTe;
    bool ok = true;
    File *files[] = {&txPacketsFile, &rxPacketsFile, &txDroppedFile, &rxDroppedFile};
    uint64 *signals[] = {&txPackets, &rxPackets, &txDropped, &rxDropped};
    uint32 numberOfFiles = sizeof(files) / sizeof(files[0]);
    for (uint32 i=0; (i<numberOfFiles) && (ok); i++) {
        StreamString line;
        bool ok = files[i]->GetLine(line);
        if (ok) {
            ok = TypeConvert(*signals[i], line);
        }
        if (ok) {
            ok = files[i]->BasicFile::Seek(0LLU);
        }
    }
    return ok;
}

bool SystemMonitor::AllocateMemory() {
    using namespace MARTe;
    bool ok = true;

    ///AUTO-GENERATED: ALLOCATE MEMORY SIGNALS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF ALLOCATE MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

bool SystemMonitor::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
    using namespace MARTe;
    bool ok = true;
    ///AUTO-GENERATED: GET MEMORY SIGNALS. DO NOT EDIT!

    if (signalIdx == 0) {
        signalAddress = &rxPackets;
    }
    else if (signalIdx == 1) {
        signalAddress = &txPackets;
    }
    else if (signalIdx == 2) {
        signalAddress = &rxDropped;
    }
    else if (signalIdx == 3) {
        signalAddress = &txDropped;
    }
    ///AUTO-GENERATED: END OF GET MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

const MARTe::char8 *SystemMonitor::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    const char8 *brokerName = "";
    ///AUTO-GENERATED: GET BROKER NAME. DO NOT EDIT!
    brokerName = "MemoryMapSynchronisedInputBroker";
    ///AUTO-GENERATED: END OF GET BROKER NAME. DO NOT EDIT!
    return brokerName;
}

bool SystemMonitor::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

CLASS_REGISTER(SystemMonitor, "1.0")
}
