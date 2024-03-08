#include "BasicUART.h"

namespace MARTe {

BasicUART::BasicUART() {
    properties.fileDescriptor = INVALID_HANDLE_VALUE;
    properties.speedCode = CBR_19200;  // Impostare la velocità desiderata
}

BasicUART::~BasicUART() {
    Close();
}

bool BasicUART::SetSpeed(const uint32 speed) {
    // Non necessario su Windows, la velocità è gestita nel DCB.
    return true;
}

bool BasicUART::Open(const char8* name) {
    properties.fileDescriptor = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (properties.fileDescriptor == INVALID_HANDLE_VALUE) {
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Open - Impossibile aprire la porta seriale %s.", name);
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(properties.fileDescriptor, &dcbSerialParams)) {
        CloseHandle(properties.fileDescriptor);
        properties.fileDescriptor = INVALID_HANDLE_VALUE;
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Open - Impossibile ottenere lo stato della porta seriale.");
        return false;
    }

    dcbSerialParams.BaudRate = properties.speedCode;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(properties.fileDescriptor, &dcbSerialParams)) {
        CloseHandle(properties.fileDescriptor);
        properties.fileDescriptor = INVALID_HANDLE_VALUE;
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Open - Impossibile impostare lo stato della porta seriale.");
        return false;
    }

    return true;
}

void BasicUART::Close() {
    if (properties.fileDescriptor != INVALID_HANDLE_VALUE) {
        CloseHandle(properties.fileDescriptor);
        properties.fileDescriptor = INVALID_HANDLE_VALUE;
    }
}

bool BasicUART::Read(char8* buffer, uint32& size) {
    DWORD bytesRead;
    if (ReadFile(properties.fileDescriptor, buffer, size, &bytesRead, NULL)) {
        size = static_cast<uint32>(bytesRead);
        return true;
    } else {
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Read - Errore nella lettura dalla porta seriale.");
        return false;
    }
}

bool BasicUART::Read(char8* buffer, uint32& size, const uint32 timeoutUsec) {
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = static_cast<DWORD>(timeoutUsec / 1000);
    timeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(properties.fileDescriptor, &timeouts)) {
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Read - Impossibile impostare i timeout della porta seriale.");
        return false;
    }

    return Read(buffer, size);
}

bool BasicUART::Write(char8* buffer, uint32 size) {
    DWORD bytesWritten;
    if (WriteFile(properties.fileDescriptor, buffer, size, &bytesWritten, NULL)) {
        return true;
    } else {
        REPORT_ERROR_STATIC(ErrorManagement::OSError, "BasicUART::Write - Errore nella scrittura sulla porta seriale.");
        return false;
    }
}

bool BasicUART::WaitRead(const uint32 timeoutUsec) {
    // Su Windows, la funzione ReadFile si occupa automaticamente dell'attesa.
    return true;
}

bool BasicUART::WaitWrite(const uint32 timeoutUsec) {
    // Su Windows, la funzione WriteFile si occupa automaticamente dell'attesa.
    return true;
}

}  // namespace MARTe
