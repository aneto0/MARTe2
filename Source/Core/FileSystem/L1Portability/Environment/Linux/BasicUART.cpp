/**
 * @file BasicUART.cpp
 * @brief Source file for class BasicUART.
 * @date 07/08/2018
 * @author Andre Neto
 * @author Pedro Lourenco
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
 *
 * @details This source file contains the definition of all the methods for
 * the class BasicUART (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BasicUART.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e628 []. Justification: Argument 'errno' of 'strerror()' provided
        via <errno.h>.*/
/*lint -e10 []. Justification: '}' present.*/

struct SpeedTable {
    MARTe::int32 code;
    MARTe::uint32 speed;
};

const SpeedTable speedTable[] = {
                                { B0, 0u }, { B50, 50u },
                                { B75, 75u }, { B110, 110u },
                                { B134, 134u }, { B150, 150u },
                                { B200, 200u }, { B300, 300u },
                                { B600, 600u }, { B1200, 1200u },
                                { B1800, 1800u }, { B2400, 2400u },
                                { B4800, 4800u }, { B9600, 9600u },
                                { B19200, 19200u }, { B38400, 38400u },
                                { B57600, 57600u }, { B115200, 115200u },
                                { B230400, 230400u }, { B460800, 460800u },
                                { B500000, 500000u }, { B576000, 576000u },
                                { B921600, 921600u }, { B1000000, 1000000u },
                                { B1152000, 1152000u }, { B1500000, 1500000u },
                                { B2000000, 2000000u }, { B2500000, 2500000u },
                                { B3000000, 3000000u }, { B3500000, 3500000u },
                                { B4000000, 4000000u }
                                };

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{1401} [MISRA C++ Rule 8-5-1]. Justification: FD_ZERO arguments
        correctly initialised.*/
BasicUART::BasicUART() {
    
    fileDescriptor = -1;
    speedCode = B19200;
    FD_ZERO(&readFDS);
    FD_ZERO(&readFDS_done);
    FD_ZERO(&writeFDS);
    FD_ZERO(&writeFDS_done);

}

BasicUART::~BasicUART() {
    /*lint -e{1551} []. Justification: thrown exception non critical.*/
    Close();
}

bool BasicUART::SetSpeed(const uint32 speed) {

    bool ok = (fileDescriptor == -1);
    if (ok) {
        int32 ix = 0;
        while ( (speedTable[ix].code != __MAX_BAUD) &&
                (speed > speedTable[ix].speed)) {
            ix++;
        }
        speedCode = speedTable[ix].code;
        ok = (speed == speedTable[ix].speed);
    }

    return ok;
}
/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'name'
                kept as non const.*/
bool BasicUART::Open(const char8* name) {

    // Info from termios.h:
    // c_iflag - input modes;
    // c_oflag - output modes;
    // c_cflag - control modes;
    // c_lflag - local modes;
    // c_cc[NCCS] - special characters.

    bool ok = (fileDescriptor == -1);
    if (ok) {
        ok = (name != NULL);
    }
    if (ok) {
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operation over signed type.*/
        fileDescriptor = open(name, O_RDWR | O_NOCTTY | O_NONBLOCK);
        if (fileDescriptor == -1) {
            std::stringstream stream;
            /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known
                    viable set of function and operator.*/
            stream << "BasicUART::Open - cannot open serial device " << name
                   << ".";
            std::string msg = stream.str();
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, msg.c_str());
            ok = false;
        }
        FD_SET(fileDescriptor, &readFDS);
        FD_SET(fileDescriptor, &writeFDS);
    }
    if (ok) {
        struct termios newtio;
        // Clean all settings
        cfmakeraw(&newtio); 
        // Eight Data bits
        // CSIZE - Character size mask. Values are CS5, CS6, CS7, or CS8.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        /*lint -e{737} []. Justification: Known loss of signedness.*/
        newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS8 | speedCode;
        // CLOCAL - Ignore modem control lines.
        // CREAD - Enable receiver.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        newtio.c_cflag |= (CLOCAL | CREAD);
        // No parity:
        // PARENB - Enable parity generation on output and parity checking for
        // input.
        // PARODD - If set, then parity for input and output is odd; otherwise
        // even parity is used.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        newtio.c_cflag &= ~(PARENB | PARODD);
        // No hardware handshake:
        // CRTSCTS - Enable RTS/CTS (hardware) flow control.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9105} [MISRA C++ Rule 2-13-3]. Justification: Known U suffix
                missing.*/
        newtio.c_cflag &= ~CRTSCTS;
        // One stopbit:
        // CSTOPB - Set two stop bits, rather than one.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        newtio.c_cflag &= ~CSTOPB; 
        // IGNBRK - Ignore BREAK condition on input.
        //newtio.c_iflag = IGNBRK;
        // Software handshake:
        // IXON - Enable XON/XOFF flow control on output.
        // IXANY - Typing any character will restart stopped output.
        // IXOFF - Enable XON/XOFF flow control on input.
        //newtio.c_iflag |= (IXON | IXOFF | IXANY);
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
        // Non cannonical mode
        // ICANON - Enable canonical mode.
        // ECHO - Echo input characters.
        // ECHOE - If ICANON is also set, the ERASE character erases the
        // preceding input character, and WERASE erases the preceding word.
        // ISIG - When any of the characters INTR, QUIT, SUSP, or DSUSP are
        // received, generate the corresponding signal.
        /*lint -e{9130} [MISRA C++ Rule 5-0-21]. Justification: Known bitwise
                operations over signed types.*/
        /*lint -e{9117} [MISRA C++ Rule 5-0-4]. Justification: Known signedness
                change due to implicit conversion.*/
        newtio.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        newtio.c_lflag = 0u;
        newtio.c_oflag = 0u;
        // VMIN - Minimum number of characters for noncanonical read (MIN).
        newtio.c_cc[VMIN] = 1u;
        // VTIME - Timeout in deciseconds for noncanonical read (TIME).
        newtio.c_cc[VTIME] = 5u;

        ok = (cfsetspeed(&newtio, static_cast<uint32>(speedCode)) == 0);
        ErrorManagement::ErrorType errorCode = ok ?
                                    ErrorManagement::Information :
                                    ErrorManagement::OSError;
        std::string modifyMessage = ok ? "successfully set" :
                                    "could not set";
        std::stringstream stream;
        /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known
                viable set of function and operator.*/
        stream << "BasicUART::Open - " << modifyMessage <<" serial device "
               << name << " speed to " << speedCode << ".";
        std::string msg = stream.str();
        REPORT_ERROR_STATIC_0(errorCode, msg.c_str());

        if (ok) {
            ok = (tcsetattr(fileDescriptor, TCSANOW, &newtio) != -1);
            errorCode = ok ? ErrorManagement::Information :
                        ErrorManagement::OSError;
            modifyMessage.clear();
            modifyMessage = ok ? "successfully set" : "could not set";
            stream.str("");
            msg.clear();
            /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known
                    viable set of function and operator.*/
            stream << "BasicUART::Open - " << modifyMessage
                   << " serial device parameters.";
            msg = stream.str();
            REPORT_ERROR_STATIC_0(errorCode, msg.c_str());
        }
    }

    return ok;
}

void BasicUART::Close() {

    if (fileDescriptor != -1) {
        int32 err = close(fileDescriptor);
        bool ok = (err == 0);
        ErrorManagement::ErrorType errorCode = ok ?
                                            ErrorManagement::Information :
                                            ErrorManagement::OSError;
        std::string msg = ok ? "BasicUART::Close - closed serial interface." :
                        "BasicUART::Close - failed to close serial interface.";
        REPORT_ERROR_STATIC_0(errorCode, msg.c_str());
        fileDescriptor = -1;
    }

}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'buffer' kept
        as non const.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Member function :Read()
        kept as non const.*/
bool BasicUART::Read(char8* buffer, uint32 &size) {

    size_t readSize = static_cast<size_t>(size);
    ssize_t readBytes = read(fileDescriptor, buffer, readSize);
    size = static_cast<uint32>(readBytes);
    bool ok = (readBytes == static_cast<ssize_t>(size));

    return ok;
}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'buffer' kept
        as non const.*/
bool BasicUART::Read(char8* buffer, uint32 &size, const uint32 timeoutUsec) {
    
    bool ok = true;
    size_t leftToRead = static_cast<size_t>(size);
    void *rbuffer = &buffer[0u];
    while ((leftToRead > 0u) && (ok)) {
        ok = WaitRead(timeoutUsec);
        if (ok) {
            ssize_t readBytes = read(fileDescriptor, rbuffer, leftToRead);
            if (readBytes > 0) {
                leftToRead -= static_cast<size_t>(readBytes);
                uint32 idx = size - static_cast<uint32>(leftToRead);
                rbuffer = &buffer[idx];
                // std::stringstream stream;
                // /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known
                //         viable set of function and operator.*/
                // stream << "BasicUART::Read - read " << readBytes << " bytes "
                //        "from serial device.";
                // std::string msg = stream.str();
                // REPORT_ERROR_STATIC_0(ErrorManagement::Debug, msg.c_str());
            }
            else {
                std::stringstream stream;
                /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known
                        viable set of function and operator.*/
                /*lint -e{1055} [MISRA C++ Rule 14-8-2]. Justification: Symbol
                        'strerror' declared.*/
                /*lint -e{746} []. Justification: Prototype not present in 
                        MARTe::strerror().*/
                stream << "BasicUART::Read - Failed to read from serial "
                        "device with error " << std::strerror(errno) << ".";
                std::string msg = stream.str();
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, msg.c_str());
                ok = false;
            }
        }
    }
    size = (size - static_cast<uint32>(leftToRead));

    return ok;
}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'size' kept as
        non const.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Member function :Read()
        kept as non const.*/
/*lint -e{818} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'size' kept as
        non const.*/
bool BasicUART::Write(char8 *buffer, uint32 size) {
    
    size_t writeSize = static_cast<size_t>(size);
    ssize_t writtenBytes = write(fileDescriptor, buffer, writeSize);
    // std::stringstream stream;
    // /*lint -e{9153} [MISRA C++ Rule 14-8-2]. Justification: Known viable set
    //         of function and operator.*/
    // stream << "BasicUART::Write - wrote " << writtenBytes << " bytes "
    //        "to serial device.";
    // std::string msg = stream.str();
    // REPORT_ERROR_STATIC_0(ErrorManagement::Debug, msg.c_str());
    bool ok = (writtenBytes == static_cast<ssize_t>(writeSize));

    return ok;
}

bool BasicUART::WaitRead(const uint32 timeoutUsec) {

    struct timeval timeWait;
    uint32 tmp = timeoutUsec / 1000000u;
    timeWait.tv_sec = static_cast<time_t>(tmp);
    tmp = timeoutUsec % 1000000u;
    timeWait.tv_usec = static_cast<suseconds_t>(tmp);
    readFDS_done = readFDS;
    int32 readyCount = select(fileDescriptor + 1, &readFDS_done,
                                NULL_PTR(fd_set *), NULL_PTR(fd_set *),
                                &timeWait);

    return (readyCount > 0);
}

bool BasicUART::WaitWrite(const uint32 timeoutUsec) {

    struct timeval timeWait;
    uint32 tmp = timeoutUsec / 1000000u;
    timeWait.tv_sec = static_cast<time_t>(tmp);
    tmp = timeoutUsec % 1000000u;
    timeWait.tv_usec = static_cast<suseconds_t>(tmp);
    writeFDS_done = writeFDS;
    int32 readyCount = select(fileDescriptor + 1, NULL_PTR(fd_set *),
                            &writeFDS_done, NULL_PTR(fd_set *),
                            &timeWait);

    return (readyCount > 0);
}

}
