#include <stdio.h>

#include "CircleHAL.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class PiBridgeInitializationSingleton {
    private:      
        PiBridgeInitializationSingleton() : PiBridge("MARTePiBridge") {
        }

    public:
        static PiBridgeInitializationSingleton& GetInstance() {
            
            static PiBridgeInitializationSingleton instance;
            return instance;
        }

        CStdlibAppStdio PiBridge;
};

typedef enum martepibridge_address_type {
    MPiAddressTypeIP,
    MPiAddressTypeNetmask,
    MPiAddressTypeGateway,
    MPiAddressTypeDNS
}MARTePiBridgeAddressType;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CNetSubSystem *networkSubSystem;

void CircleHALPrint(const char* stringToPrint) {
    PiBridgeInitializationSingleton::GetInstance().PiBridge.Printf(stringToPrint);
}

bool CircleHALInit() {
    return PiBridgeInitializationSingleton::GetInstance().PiBridge.Initialize();
}

void CircleHALIPDottedQuadStringToQuadU8(const char* inputDottedQuad, uint8_t* outputQuadU8) {
    uint32_t tempU32[4] = { 0 };
    sscanf(inputDottedQuad, "%d.%d.%d.%d", &tempU32[0], &tempU32[1], &tempU32[2], &tempU32[3]);
    outputQuadU8[0] = (uint8_t)tempU32[0];
    outputQuadU8[1] = (uint8_t)tempU32[1];
    outputQuadU8[2] = (uint8_t)tempU32[2];
    outputQuadU8[3] = (uint8_t)tempU32[3];
}

bool CircleHALInitNetworking(CircleHALIPConfiguration *ipConfiguration) {
    // bool retVal = (piBridge != NULL);

    // if(retVal) {
    //     if(ipConfiguration->dhcpEnabled) {
    //         /* The dummy IP 0.0.0.0 means DHCP enabled */
    //         uint8_t dummyQuadU8[4] = { 0 };
            
    //         piBridge->SetNetworkAddress(MPiAddressTypeIP, &dummyQuadU8[0]);
    //         piBridge->SetNetworkAddress(MPiAddressTypeNetmask, &dummyQuadU8[0]);
    //         piBridge->SetNetworkAddress(MPiAddressTypeGateway, &dummyQuadU8[0]);
    //     }
    //     else {
    //         piBridge->SetNetworkAddress(MPiAddressTypeIP, ipConfiguration->ipAddressQuadU8);
    //         piBridge->SetNetworkAddress(MPiAddressTypeNetmask, ipConfiguration->netmaskQuadU8);
    //         piBridge->SetNetworkAddress(MPiAddressTypeGateway, ipConfiguration->gatewayQuadU8);
    //     }
    //     retVal = piBridge->InitialiseNetwork();

    //     if(retVal) {
    //         networkSubSystem = piBridge->GetNetworkSubSystem();
    //     }
    // }

    // return retVal;

    return false;
}

void CircleHALYield() {
    
}
