/**
 * @file Bootstrap.cpp
 * @brief Source file for class Bootstrap
 * @date 04/04/2018
 * @author Andre Neto
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
 * the class Bootstrap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "BasicFile.h"
#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "BasicUDPSocket.h"
#include "CircleHAL.h"
#include "ConfigurationDatabase.h"
#include "File.h"
#include "StandardParser.h"

namespace MARTe {

    /**
     * @brief Input configuration file pointer
     */
    BasicFile *inputConfigurationFile;

    //Forward declaration
    void CircleNewLibProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                    const MARTe::char8 * const errorDescription);

    Bootstrap::Bootstrap() {
        ErrorManagement::SetErrorProcessFunction(CircleNewLibProcessFunction);
    }

    Bootstrap::~Bootstrap() {

    }

    ErrorManagement::ErrorType Bootstrap::Run() {

        // CircleHALIPConfiguration ipConfig;
        // ipConfig.dhcpEnabled = false;
        
        // ipConfig.ipAddressQuadU8[0] = 192;
        // ipConfig.ipAddressQuadU8[1] = 168;
        // ipConfig.ipAddressQuadU8[2] = 1;
        // ipConfig.ipAddressQuadU8[3] = 10;

        // ipConfig.netmaskQuadU8[0] = 255;
        // ipConfig.netmaskQuadU8[1] = 255;
        // ipConfig.netmaskQuadU8[2] = 255;
        // ipConfig.netmaskQuadU8[3] = 0;

        // ipConfig.gatewayQuadU8[0] = 192;
        // ipConfig.gatewayQuadU8[1] = 168;
        // ipConfig.gatewayQuadU8[2] = 1;
        // ipConfig.gatewayQuadU8[3] = 1;

        // CircleHALInitNetworking(&ipConfig);

        // BasicFile f;
        // bool ok = f.Open("bftest.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
        // StreamString str = "This is a test message to write on a file\r\n";
        // uint32 writeSize = str.Size();
        // uint32 totalWriteSize = 0u;

        // for(int i = 0; i < 1000; i++) {
        //     writeSize = str.Size();
        //     f.Write(str.Buffer(), writeSize);
        //     totalWriteSize += writeSize;
        // }
        
        // f.Close();

        // StreamString msg;
        // msg.Printf("Written %d of %d", totalWriteSize, 1000 * str.Size());
        // REPORT_ERROR_STATIC(ErrorManagement::Information, msg.Buffer());
        
        // char8* entireFile = malloc(totalWriteSize + 1);
        // memset(entireFile, '@', totalWriteSize + 1);

        // BasicFile fr;
        // fr.Open("bftest.txt", BasicFile::ACCESS_MODE_R);
        // fr.Read(entireFile, totalWriteSize);
        // fr.Close();

        //REPORT_ERROR_STATIC_0(ErrorManagement::Information, entireFile);
        // char8* tempReceivedBytes = malloc(128);
        // uint32 tcpReadSize = 100u;
        // uint32 tcpEffRead = 0u;

        // BasicTCPSocket tcpSocket;
        // tcpSocket.Open();
        // tcpSocket.Listen(5001);

        // BasicTCPSocket *clientSocket = tcpSocket.WaitConnection();
        // REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "TCP Client accepted");
        
        // MARTe::float64 countsDelta = 1.0 * MARTe::HighResolutionTimer::Frequency();
        
        // while(true){
        //     MARTe::uint64 countStart = MARTe::HighResolutionTimer::Counter();
        //     MARTe::uint64 countEnd = countStart + static_cast<MARTe::uint64>(countsDelta);

        //     while(MARTe::HighResolutionTimer::Counter() < countEnd) {    
        //     }

        //     for(int i = 0; i < 128; i++) {
        //         tempReceivedBytes[i] = 0;
        //     }
            
        //     tcpEffRead = clientSocket->Read(tempReceivedBytes, tcpReadSize);
        //     REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %s = %d", tempReceivedBytes, tcpEffRead);
        //     uint32 sendSize = 21u;
        //     clientSocket->Write("MARTe2 sending data\r\n", sendSize);
        // }
        
        /*
        

        BasicTCPSocket tcpSocket;
        tcpSocket.Open();

        tcpSocket.Connect("192.168.1.1", 5001);
        uint32 tcpWriteSize = 20u;
        uint32 tcpReadSize = 127;
        while(true) {
            tcpSocket.Write("Testing TCP Socket\r\n", tcpWriteSize);
            
            for(int i = 0; i < 128; i++) {
                tempReceivedBytes[i] = 0;
            };

            tcpSocket.Read(tempReceivedBytes, tcpReadSize);
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "TCP Read %s", tempReceivedBytes);
        }
        */

        // BasicUDPSocket udpSocket;
        // uint32 udpWriteSize = 20u;
        // uint32 udpReadSize = 0u;

        // REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "UDP Socket Open %d", udpSocket.Open());
        // REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "UDP Socket Connect %d", udpSocket.Connect("192.168.1.1", 5000));

        

        // countsDelta = 1.0 * MARTe::HighResolutionTimer::Frequency();
        // while(true) {
        //     MARTe::uint64 countStart = MARTe::HighResolutionTimer::Counter();
        //     MARTe::uint64 countEnd = countStart + static_cast<MARTe::uint64>(countsDelta);

        //     REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Led is ON");
            
        //     udpWriteSize = 20u;
        //     udpSocket.Write("TESTING UDP SOCKET\r\n", udpWriteSize);
        //     REPORT_ERROR_STATIC(ErrorManagement::Information, "UDP Written %d", udpWriteSize);

        //     for(int i = 0; i < 128; i++) {
        //         tempReceivedBytes[i] = 0;
        //     }

        //     while(MARTe::HighResolutionTimer::Counter() < countEnd) {    
        //     }

        //     udpReadSize = 127u;
        //     udpSocket.Read(tempReceivedBytes, udpReadSize);
        //     REPORT_ERROR_STATIC(ErrorManagement::Information, "UDP Read %s %d", tempReceivedBytes, udpReadSize);

        //     countStart =MARTe:: HighResolutionTimer::Counter();
        //     countEnd = countStart + static_cast<MARTe::uint64>(countsDelta);

        //     REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Led is OFF");
        //     while(MARTe::HighResolutionTimer::Counter() < countEnd) {
        //     }
        // }

        return ErrorManagement::NoError;
    }
    
    void MARTe2ToCircleIPConfigurationBridge(ConfigurationDatabase &inputData, CircleHALIPConfiguration *outputData) {
        bool retVal = true;

        StreamString ipAddress;
        StreamString subnetMask;
        StreamString defaultGateway;
        uint8_t dhcpEnabled;
        retVal = inputData.MoveToRoot();

        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Cannot move to root in Raspberry specific configuration file");
        }

        if(retVal) {
            retVal = inputData.MoveAbsolute("RPiConfig");
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Missing RPiConfig section in Raspberry specific configuration file");
            }
        }

        if(retVal) {
            bool tempRead = inputData.Read("DHCP", dhcpEnabled);
            if(!tempRead) {
                dhcpEnabled = 0;
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Missing DHCP configuration parameter, defaulting to false");
            }
        }

        if(retVal) {
            bool tempRead = inputData.Read("IPAddress", ipAddress);
            if(!tempRead) {
                ipAddress = "0.0.0.0";
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Missing IP Address configuration parameter, defaulting to 0.0.0.0 (DHCP)");
            }
        }

        if(retVal) {
            bool tempRead = inputData.Read("SubnetMask", subnetMask);
            if(!tempRead) {
                subnetMask = "0.0.0.0";
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Missing Subnet Mask configuration parameter, defaulting to 0.0.0.0 (DHCP)");
            }
        }

        if(retVal) {
            bool tempRead = inputData.Read("DefaultGateway", defaultGateway);
            if(!tempRead) {
                defaultGateway = "0.0.0.0";
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Missing Default Gateway parameter, defaulting to 0.0.0.0 (DHCP)");
            }
        }

        outputData->dhcpEnabled = dhcpEnabled;
        REPORT_ERROR_STATIC(ErrorManagement::Information, "DHCP is %s", ((dhcpEnabled == 1)?"enabled":"disabled"));

        if(!dhcpEnabled) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "IP Address is %s", ipAddress.Buffer());
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Subnet mask is %s", subnetMask.Buffer());
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Default Gateway is %s", defaultGateway.Buffer());
            
            CircleHALIPDottedQuadStringToQuadU8(ipAddress.Buffer(), outputData->ipAddressQuadU8);
            CircleHALIPDottedQuadStringToQuadU8(subnetMask.Buffer(), outputData->netmaskQuadU8);
            CircleHALIPDottedQuadStringToQuadU8(defaultGateway.Buffer(), outputData->gatewayQuadU8);
        }
        else {
            memset(outputData->ipAddressQuadU8, 0, 4);
            memset(outputData->netmaskQuadU8, 0, 4);
            memset(outputData->gatewayQuadU8, 0, 4);
        }

    }

    ErrorManagement::ErrorType Bootstrap::GetConfigurationStream(StructuredDataI &loaderParameters, StreamI *&configurationStream) {
        ErrorManagement::ErrorType retVal = ErrorManagement::NoError;
        
        /* Raspberry Pi related configuration section */
        bool raspiCfgRetVal = true;
        File *rpiConfigurationFile = new File();
        raspiCfgRetVal = (rpiConfigurationFile != NULL_PTR(File*));

        if(raspiCfgRetVal) {    
            
            raspiCfgRetVal = rpiConfigurationFile->Open("rpiconfig.cfg", BasicFile::ACCESS_MODE_R);
            if(!raspiCfgRetVal) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Bootstrap::GetConfigurationStream() Failure getting Raspberry Pi configuration file. Default settings will be applied.");
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::GetConfigurationStream() Failure allocating file structure for RPi configuration file.");
            /* If we cannot allocate before, a deeper problem is occurring. We stop the whole method call */
            retVal.OSError = true;
        }

        if(raspiCfgRetVal) {
            raspiCfgRetVal = rpiConfigurationFile->Seek(0LLU);
            if(!raspiCfgRetVal) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Bootstrap::GetConfigurationStream() Failure seeking RPi configuration file. Default settings will be applied.");
            }
        }

        ConfigurationDatabase rpiConfigurationDB;
        StreamString rpiParserError;

        if(raspiCfgRetVal) {
            StandardParser rpiCfgParser(*rpiConfigurationFile, rpiConfigurationDB, &rpiParserError);
            raspiCfgRetVal = rpiCfgParser.Parse();

            if(!raspiCfgRetVal) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Bootstrap::GetConfigurationStream() Failure parsing RPi configuration file. Default settings will be applied.");
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "Bootstrap::GetConfigurationStream() StandardParser reports %s", rpiParserError.Buffer());
            }
        }

        bool test = rpiConfigurationDB.MoveAbsolute("RPiConfig");

        if(raspiCfgRetVal) {
            CircleHALIPConfiguration ipConfig;
            MARTe2ToCircleIPConfigurationBridge(rpiConfigurationDB, &ipConfig);
            CircleHALInitNetworking(&ipConfig);
        }

        /* End of Raspberry Pi related configuration section */
        
        inputConfigurationFile = new File();
        
        retVal.OSError = !(inputConfigurationFile != NULL_PTR(File*));

        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::GetConfigurationStream() Failure allocating MARTe2 configuration file reader structure");
        }

        if(retVal) {
            retVal.OSError = !inputConfigurationFile->Open("marte2config.cfg", BasicFile::ACCESS_MODE_R);
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::GetConfigurationStream() Failure reading MARTe2 configuration file (marte2config.cfg)");
            }
        }

        if(retVal) {
            retVal.OSError = !inputConfigurationFile->Seek(0LLU);
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::GetConfigurationStream() Failure seeking configuration file start");
            }
        }
        
        if(retVal) {
            configurationStream = inputConfigurationFile;
        }

        return retVal;
    }

    ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters) {
        ErrorManagement::ErrorType retVal;

        //Writing defaults in loader parameters
        retVal.parametersError = !loaderParameters.Write("Parser", "cdb");
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure writing default parser (cdb)");
        }

        if(retVal) {
            retVal.parametersError = !loaderParameters.Write("Loader", "RealTimeLoader");
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure writing default loader (RealTimeLoader)");
            }
        }

        if(retVal) {
            retVal.parametersError = !loaderParameters.Write("FirstState", "State1");
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure setting startup state (State1)");
            }
        }

        return retVal;
    }

    void CircleNewLibProcessFunction(const ErrorManagement::ErrorInformation &errorInfo,
                                    const char8 * const errorDescription) {
        CircleHALPrint(errorDescription);
    }

    void Bootstrap::Printf(const char8 * const msg) {
        CircleHALPrint(msg);
    }

    ErrorManagement::ErrorType Bootstrap::InitHAL(int32 argc, char8 **argv) {
        CircleHALInit();
        return ErrorManagement::NoError;
    }

}
