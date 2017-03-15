/**
 * @file LoggerConsumerI.cpp
 * @brief Source file for class LoggerConsumerI
 * @date 11/03/2017
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
 * the class LoggerConsumerI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoggerConsumerI.h"
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

LoggerConsumerI::LoggerConsumerI() {
    printKeys = true;
    formatPrefs.asUint16 = 0u;
}

LoggerConsumerI::~LoggerConsumerI() {

}

void LoggerConsumerI::PrintToStream(LoggerPage * const logPage, BufferedStreamI &err) const {
    StreamString errorCodeStr;
    ErrorManagement::ErrorInformation errorInfo = logPage->errorInfo;
    const char8 *key = "";
    if (formatPrefs.info.operator bool()) {
        if (printKeys) {
            key = "|E=";
        }
        else {
            key = "|";
        }
        ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
        (void) err.Printf("%s%s", key, errorCodeStr.Buffer());
    }
    if (formatPrefs.timeShort.operator bool()) {
        if (printKeys) {
            key = "|TM=";
        }
        else {
            key = "|";
        }
        (void) err.Printf("%s%d", key, errorInfo.hrtTime);
    }
    if (formatPrefs.timeFull.operator bool()) {
        if (printKeys) {
            key = "|TM=";
        }
        else {
            key = "|";
        }
        TimeStamp ts;
        if (ts.ConvertFromEpoch(static_cast<oslong>(errorInfo.timeSeconds))) {
            (void) err.Printf("%s%d:%d:%d (%d)", key, ts.GetHour(), ts.GetMinutes(), ts.GetSeconds(), errorInfo.hrtTime);
        }
    }
    if (formatPrefs.objectName.operator bool()) {
        if (printKeys) {
            key = "|o=";
        }
        else {
            key = "|";
        }
        if (errorInfo.objectPointer != NULL_PTR(const void *)) {
            (void) err.Printf("%s%s", key, errorInfo.objectName);
        }
        else {
            (void) err.Printf("%s", key);
        }
    }
    if (formatPrefs.objectPointer.operator bool()) {
        if (printKeys) {
            key = "|O=";
        }
        else {
            key = "|";
        }
        if (errorInfo.objectPointer != NULL_PTR(const void *)) {
            (void) err.Printf("%s%p", key, errorInfo.objectPointer);
        }
        else {
            (void) err.Printf("%s", key);
        }
    }
    if (formatPrefs.threadId.operator bool()) {
        if (printKeys) {
            key = "|T=";
        }
        else {
            key = "|";
        }
        //TODO
        (void) err.Printf("%s", key);
    }
    if (formatPrefs.functionName.operator bool()) {
        if (printKeys) {
            key = "|f=";
        }
        else {
            key = "|";
        }
        (void) err.Printf("%s%s", key, errorInfo.functionName);
    }
    if (formatPrefs.fileName.operator bool()) {
        if (printKeys) {
            key = "|F=";
        }
        else {
            key = "|";
        }
        (void) err.Printf("%s%s:%d", key, errorInfo.fileName, errorInfo.header.lineNumber);
    }
    if (formatPrefs.message.operator bool()) {
        if (printKeys) {
            key = "|D=";
        }
        else {
            key = "|";
        }
        (void) err.Printf("%s%s", key, logPage->errorStrBuffer);
    }
    if (formatPrefs.className.operator bool()) {
        if (printKeys) {
            key = "|C=";
        }
        else {
            key = "|";
        }
        (void) err.Printf("%s%s", key, errorInfo.className);
    }
}

bool LoggerConsumerI::LoadPrintPreferences(StructuredDataI &data) {
    StreamString format;
    bool ok = data.Read("Format", format);
    formatPrefs.asUint16 = 0u;
    uint64 formatSize = 0u;
    if (ok) {
        formatSize = format.Size();
        /*lint -e{1705} in order to access directly the static member the formatPrefs.unMapped.BitOffset() would have to be changed to BitRange<uint16, 6u, 10u>::BitOffset, but this would imply maintaining the offset here and in the header file...*/
        ok = (formatSize < (static_cast<uint64>(formatPrefs.unMapped.BitOffset()) + 1u));
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Format size is too large");
        }
    }
    if (ok) {
        uint32 n;
        for (n = 0u; (n < formatSize) && (ok); n++) {
            switch (format[n]) {
            case 'E':
                formatPrefs.info = true;
                break;
            case 'T':
                formatPrefs.timeShort = true;
                break;
            case 't':
                formatPrefs.timeFull = true;
                break;
            case 'O':
                formatPrefs.objectName = true;
                break;
            case 'o':
                formatPrefs.objectPointer = true;
                break;
            case 'F':
                formatPrefs.fileName = true;
                break;
            case 'f':
                formatPrefs.functionName = true;
                break;
            case 'm':
                formatPrefs.message = true;
                break;
            case 'R':
                formatPrefs.threadId = true;
                break;
            case 'C':
                formatPrefs.className = true;
                break;
            default:
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Unsupported Format");
                ok = false;
            }
        }
    }
    if (ok) {
        uint32 printKeysInt = 0u;
        if (data.Read("PrintKeys", printKeysInt)) {
            printKeys = (printKeysInt == 1u);
        }
    }

    return ok;
}

bool LoggerConsumerI::IsPrintKeys() const {
    return printKeys;
}

LoggerConsumerI::FormatPreferences LoggerConsumerI::GetFormatPreferences() const {
    return formatPrefs;
}

}
