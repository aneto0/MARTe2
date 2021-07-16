#ifndef CIRCLE_HAL_H
#define CIRCLE_HAL_H

#include <circle/actled.h>
#include <circle/string.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/nulldevice.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/usbhcidevice.h>
#include <SDCard/emmc.h>
#include <circle/input/console.h>
#include <circle/sched/scheduler.h>
#include <circle/net/netsubsystem.h>
#include <wlan/bcm4343.h>
#include <wlan/hostap/wpa_supplicant/wpasupplicant.h>

#include <circle_glue.h>
#include <string.h>

/**
 * Basic Circle Stdlib application that supports GPIO access.
 */
class CStdlibApp
{
    public:
        enum TShutdownMode
        {
                ShutdownNone,
                ShutdownHalt,
                ShutdownReboot
        };

        CStdlibApp (const char *kernel) : FromKernel (kernel) {
            consoleReady = false;
        }

        virtual ~CStdlibApp (void) {
        }

        virtual bool Initialize (void) {
            return mInterrupt.Initialize ();
        }

        virtual void Cleanup (void) {
        }

        virtual void Printf(const char* message) = 0;

        const char *GetKernelName(void) const {
            return FromKernel;
        }

    protected:
        CActLED            mActLED;
        CKernelOptions     mOptions;
        CDeviceNameService mDeviceNameService;
        CNullDevice        mNullDevice;
        CExceptionHandler  mExceptionHandler;
        CInterruptSystem   mInterrupt;
        bool               consoleReady;

    private:
        char const *FromKernel;
};

/**
 * Stdlib application that adds screen support
 * to the basic CStdlibApp features.
 */
class CStdlibAppScreen : public CStdlibApp
{
    public:
        CStdlibAppScreen(const char *kernel)
            : CStdlibApp (kernel),
              mScreen (mOptions.GetWidth (), mOptions.GetHeight ()),
              mTimer (&mInterrupt),
              mLogger (mOptions.GetLogLevel (), &mTimer) {
        }

        virtual bool Initialize (void) {
            bool retVal = CStdlibApp::Initialize();

            if(retVal) {
                retVal = mScreen.Initialize();
            }

            if(retVal) {
                retVal = mSerial.Initialize(115200);
            }

            if(retVal) {
                CDevice *pTarget = mDeviceNameService.GetDevice (mOptions.GetLogDevice (), false);
                if (pTarget == 0) {
                    pTarget = &mScreen;
                }
                retVal = mLogger.Initialize(pTarget);
            }

            if(retVal) {
                retVal = mTimer.Initialize();
            }
            
            if(retVal) {
                consoleReady = true;
            }

            return retVal;
        }
        
        virtual void Printf(const char* message) {
            if(consoleReady) {
                mLogger.Write(GetKernelName (), LogNotice, message);
            }
        }
    protected:
        CScreenDevice   mScreen;
        CSerialDevice   mSerial;
        CTimer          mTimer;
        CLogger         mLogger;
};

/**
 * Stdlib application that adds stdio support
 * to the CStdlibAppScreen functionality.
 */
class CStdlibAppStdio: public CStdlibAppScreen
{
    private:
        char const *mpPartitionName;

    public:
        #define CSTDLIBAPP_LEGACY_DEFAULT_PARTITION "emmc1-1"
        #define CSTDLIBAPP_DEFAULT_PARTITION "SD:"

        CStdlibAppStdio (const char *kernel,
                         const char *pPartitionName = CSTDLIBAPP_DEFAULT_PARTITION)
            : CStdlibAppScreen (kernel),
              mpPartitionName (pPartitionName),
              mUSBHCI (&mInterrupt, &mTimer, TRUE),
              mEMMC (&mInterrupt, &mTimer, &mActLED),
            #if !defined(__aarch64__) || !defined(LEAVE_QEMU_ON_HALT)
              mConsole (0, TRUE)
            #else
              //Use alternatively mSerial to redirect stdout to serial or mScreen to redirect stdout to framebuffer
              mConsole(&mSerial) 
              //mConsole (&mScreen)
            #endif
        {
        }

        virtual bool Initialize (void) {
            bool retVal = CStdlibAppScreen::Initialize ();

            if(retVal) {
                retVal = mEMMC.Initialize();
            }

            if(retVal) {
                char const *partitionName = mpPartitionName;
                // Recognize the old default partion name
                if (strcmp(partitionName, CSTDLIBAPP_LEGACY_DEFAULT_PARTITION) == 0) {
                    partitionName = CSTDLIBAPP_DEFAULT_PARTITION;
                }

                if (f_mount (&mFileSystem, partitionName, 1) != FR_OK) {
                    mLogger.Write (GetKernelName (), LogError, "Cannot mount partition: %s", partitionName);
                    retVal = false;
                }
            }

            if(retVal) {
                #if !defined(__aarch64__) || !defined(LEAVE_QEMU_ON_HALT)
                        // The USB driver is not supported under 64-bit QEMU, so
                        // the initialization must be skipped in this case, or an
                        // exit happens here under 64-bit QEMU.
                    if (!mUSBHCI.Initialize ()) {
                        return false;
                    }
                #endif
            }

            if(retVal) {
                retVal = mConsole.Initialize();
            }

            if(retVal) {
                // Initialize newlib stdio with a reference to Circle's file system and console
                CGlueStdioInit (mFileSystem, mConsole);
                mLogger.Write (GetKernelName (), LogNotice, "Compile time: " __DATE__ " " __TIME__);
            }

            return retVal;
        }

        virtual void Cleanup (void)
        {
            f_mount(0, "", 0);
            CStdlibAppScreen::Cleanup ();
        }

    protected:
        CUSBHCIDevice   mUSBHCI;
        CEMMCDevice     mEMMC;
        FATFS           mFileSystem;
        CConsole        mConsole;
};

/**
 * Stdlib application that adds network functionality
 * to the CStdlibAppStdio features.
 */
class CStdlibAppNetwork: public CStdlibAppStdio
{
public:
        #define CSTDLIBAPP_WLAN_FIRMWARE_PATH   CSTDLIBAPP_DEFAULT_PARTITION "/firmware/"
        #define CSTDLIBAPP_WLAN_CONFIG_FILE     CSTDLIBAPP_DEFAULT_PARTITION "/wpa_supplicant.conf"

        CStdlibAppNetwork (const char *kernel,
                   const char *pPartitionName = CSTDLIBAPP_DEFAULT_PARTITION,
                   const u8 *pIPAddress      = 0,       // use DHCP if pIPAddress == 0
                   const u8 *pNetMask        = 0,
                   const u8 *pDefaultGateway = 0,
                   const u8 *pDNSServer      = 0,
                   TNetDeviceType DeviceType = NetDeviceTypeEthernet)
          : CStdlibAppStdio(kernel, pPartitionName),
            mDeviceType (DeviceType),
            mWLAN (CSTDLIBAPP_WLAN_FIRMWARE_PATH),
            mNet(pIPAddress, pNetMask, pDefaultGateway, pDNSServer, DEFAULT_HOSTNAME, DeviceType),
            mWPASupplicant (CSTDLIBAPP_WLAN_CONFIG_FILE)
        {
        }

        virtual bool Initialize (bool const bWaitForActivate = true)
        {
            bool retVal = CStdlibAppStdio::Initialize();
            
            if(retVal) {
                if (mDeviceType == NetDeviceTypeWLAN)
                {
                    retVal = mWLAN.Initialize();
                }
            }

            if(retVal) {
                retVal = mNet.Initialize(false);
            }

            if(retVal) {
                if (mDeviceType == NetDeviceTypeWLAN)
                {
                    retVal = mWPASupplicant.Initialize();
                }
            }

            if(retVal) {
                while (bWaitForActivate && !mNet.IsRunning ())
                {
                    mScheduler.Yield ();
                }
            }

            return retVal;
        }

    protected:
        CScheduler      mScheduler;
        TNetDeviceType  mDeviceType;
        CBcm4343Device  mWLAN;
        CNetSubSystem   mNet;
        CWPASupplicant  mWPASupplicant;
};

typedef enum __circle_hal_netdevice {
    CircleHALWiredNetwork,
    CircleHALWirelessNetwork
}CircleHALNetworkDevice;

typedef struct __circle_hal_ipconfig {
    bool                    dhcpEnabled;
    CircleHALNetworkDevice  networkDevice;
    uint8_t                 ipAddressQuadU8[4];
    uint8_t                 netmaskQuadU8[4];
    uint8_t                 gatewayQuadU8[4];
    uint8_t                 dnsQuadU8[4];

    __circle_hal_ipconfig() {
        dhcpEnabled = true;
        networkDevice = CircleHALWiredNetwork;
        ipAddressQuadU8[0] = ipAddressQuadU8[1] = ipAddressQuadU8[2] = ipAddressQuadU8[3] = 0;
        netmaskQuadU8[0] = netmaskQuadU8[1] = netmaskQuadU8[2] = netmaskQuadU8[3] = 0;
        gatewayQuadU8[0] = gatewayQuadU8[1] = gatewayQuadU8[2] = gatewayQuadU8[3] = 0;
        dnsQuadU8[0] = dnsQuadU8[1] = dnsQuadU8[2] = dnsQuadU8[3] = 0;
    }
}CircleHALIPConfiguration;

bool CircleHALInit();

bool CircleHALInitNetworking(CircleHALIPConfiguration *ipConfiguration);

void CircleHALPrint(const char* stringToPrint);

void CircleHALYield();

void CircleHALIPDottedQuadStringToQuadU8(const char* inputDottedQuad, uint8_t* outputQuadU8);

#endif