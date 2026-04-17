/**                                                                             
 * @file SystemMonitorSol.cpp                                                                
 * @brief Source file for class SystemMonitorSol                                        
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
 * the class SystemMonitorSol (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SystemMonitorSol.h"
#include "StructuredDataIHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

SystemMonitorSol::SystemMonitorSol() :
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
    cpuLoad = NULL_PTR(float64 *);
    nOfElementsCPULoad = 0;
    ///AUTO-GENERATED: END OF CTOR SIGNALS. DO NOT EDIT!

    lastTotalIdle = NULL_PTR(uint64 *);
    lastTotal = NULL_PTR(uint64 *);
}

SystemMonitorSol::~SystemMonitorSol() {
    using namespace MARTe;
    ///AUTO-GENERATED: DTOR PARAMETERS. DO NOT EDIT!

    ///AUTO-GENERATED: END OF DTOR PARAMETERS. DO NOT EDIT!
    ///AUTO-GENERATED: DTOR SIGNALS. DO NOT EDIT!

    if (cpuLoad != NULL_PTR(float64 *)) {
        delete[] cpuLoad;
    }
    ///AUTO-GENERATED: END OF DTOR SIGNALS. DO NOT EDIT!
    (void) txPacketsFile.Close();
    (void) rxPacketsFile.Close();
    (void) txDroppedFile.Close();
    (void) rxDroppedFile.Close();
    (void) statFile.Close();

    if (lastTotalIdle != NULL_PTR(uint64 *)) {
        delete [] lastTotalIdle;
    }
    if (lastTotal != NULL_PTR(uint64 *)) {
        delete [] lastTotal;
    }

}

bool SystemMonitorSol::Initialise(MARTe::StructuredDataI &data) {
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
    if (ok) {
        ok = statFile.Open("/proc/stat", (File::ACCESS_MODE_R));
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Failed to Open file /proc/stat");
        }
    }

    return ok;
}

bool SystemMonitorSol::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = DataSourceI::SetConfiguredDatabase(data);
    StructuredDataIHelper helper(data, this);

    ///AUTO-GENERATED: SETUP SIGNALS. DO NOT EDIT!

    if (ok) {
        ok = (GetNumberOfSignals() == 5);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of signals. Expected 5 and read %d", GetNumberOfSignals());
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
    if (ok) {
        TypeDescriptor signalTypeCPULoad = GetSignalType(4);
        ok = (signalTypeCPULoad == Float64Bit);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected signal type at index 4");
        }
    }
    if (ok) {
        nOfElementsCPULoad = 0;
        ok = GetSignalNumberOfElements(4, nOfElementsCPULoad);
    }
    ///AUTO-GENERATED: END OF SETUP SIGNALS. DO NOT EDIT!
     
    if (ok) {
        uint32 numberOfOnlineCPUs = sysconf(_SC_NPROCESSORS_ONLN);
        ok = (numberOfOnlineCPUs == nOfElementsCPULoad);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unexpected number of CPUs. Available %d and configured %d", numberOfOnlineCPUs, nOfElementsCPULoad);
        }
    }
    if (ok) {
        lastTotalIdle = new uint64[nOfElementsCPULoad];
        lastTotal = new uint64[nOfElementsCPULoad];
    }
    return ok;
}

bool SystemMonitorSol::Synchronise() {
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

    //Skip the first line
    if (ok) {
        StreamString line;
        ok = statFile.GetLine(line);
    }
    for (uint32 c=0; (c<nOfElementsCPULoad) && (ok); c++) {
        StreamString line;
        ok = statFile.GetLine(line);
        if (ok) {
            ok = line.Seek(0LLU);
        }
        if (ok) {
            cpuLoad[c] = ParseCPULoad(line, c);
        }
    }
    if (ok) {
        ok = statFile.BasicFile::Seek(0LLU);
    }
    return ok;
}

bool SystemMonitorSol::AllocateMemory() {
    using namespace MARTe;
    bool ok = true;

    ///AUTO-GENERATED: ALLOCATE MEMORY SIGNALS. DO NOT EDIT!

    cpuLoad = new float64[8];
    ///AUTO-GENERATED: END OF ALLOCATE MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

bool SystemMonitorSol::GetSignalMemoryBuffer(const MARTe::uint32 signalIdx, const MARTe::uint32 bufferIdx, void *&signalAddress) {
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
    else if (signalIdx == 4) {
        signalAddress = &cpuLoad[0];
    }
    ///AUTO-GENERATED: END OF GET MEMORY SIGNALS. DO NOT EDIT!
    return ok;
}

const MARTe::char8 *SystemMonitorSol::GetBrokerName(MARTe::StructuredDataI &data, const MARTe::SignalDirection direction) {
    using namespace MARTe;
    const char8 *brokerName = "";
    ///AUTO-GENERATED: GET BROKER NAME. DO NOT EDIT!
    brokerName = "MemoryMapSynchronisedInputBroker";
    ///AUTO-GENERATED: END OF GET BROKER NAME. DO NOT EDIT!
    return brokerName;
}

bool SystemMonitorSol::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

MARTe::float64 SystemMonitorSol::ParseCPULoad(MARTe::StreamString &line, MARTe::uint32 cpuIdx) {
    using namespace MARTe;
    //cpu0 11967760 10418 3650720 720076644 1657 1953761 669881 0 0 0
    char8 term;
    StreamString token;
    (void) line.GetToken(token, " ", term);//skip the cpu id
    uint64 user = ParseNextCPUToken(line);
    uint64 nice = ParseNextCPUToken(line); 
    uint64 system = ParseNextCPUToken(line); 
    uint64 idle = ParseNextCPUToken(line); 
    uint64 iowait = ParseNextCPUToken(line); 
    uint64 irq = ParseNextCPUToken(line); 
    uint64 softirq = ParseNextCPUToken(line);

    uint64 totalIdle = idle + iowait;
    uint64 total = user + nice + system + idle + iowait + irq + softirq;

    float64 deltaTotalIdle = static_cast<float64>(totalIdle - lastTotalIdle[cpuIdx]);
    float64 deltaTotal = static_cast<float64>(total - lastTotal[cpuIdx]);
    
    float64 cpuUsage = 0.0f;
    if (deltaTotal > 0) {
        cpuUsage = 1.0 - (deltaTotalIdle / deltaTotal);
    }

    lastTotalIdle[cpuIdx] = totalIdle;
    lastTotal[cpuIdx] = total;

    return cpuUsage;
}


MARTe::uint64 SystemMonitorSol::ParseNextCPUToken(MARTe::StreamString &line) {
    using namespace MARTe;
    uint64 tokenU64 = 0LLU;
    StreamString token;
    char8 term;
    (void) line.GetToken(token, " ", term);
    TypeConvert(tokenU64, token.Buffer());
    return tokenU64;
}

CLASS_REGISTER(SystemMonitorSol, "1.0")
}
