/*
 * Main_TestAll.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: root
 */

#include <limits.h>
#include "gtest/gtest.h"
#include "ErrorManagement.h"

void MainGTestErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                   const char * const errorDescription) {
    printf("---->>%s\n", errorDescription);
}


int main(int argc, char **argv) {
    SetErrorProcessFunction(&MainGTestErrorProcessFunction);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

