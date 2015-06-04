/*
 * Main_TestAll.cpp
 *
 *  Created on: Jan 8, 2015
 *      Author: root
 */

#include <limits.h>
#include "gtest/gtest.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

