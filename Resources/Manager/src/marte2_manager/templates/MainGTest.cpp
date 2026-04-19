${header_text}

#include <limits.h>
#include "gtest/gtest.h"
#include "ErrorManagement.h"
#include "Object.h"
#include "StreamString.h"

void MainGTestComponentsErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                   const char * const errorDescription) {
    MARTe::StreamString errorCodeStr;
    MARTe::ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
}

int main(int argc, char **argv) {
    SetErrorProcessFunction(&MainGTestComponentsErrorProcessFunction);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
