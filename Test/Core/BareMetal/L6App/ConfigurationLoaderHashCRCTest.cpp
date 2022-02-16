/**
 * @file ConfigurationLoaderHashCRCTest.cpp
 * @brief Source file for class ConfigurationLoaderHashCRCTest
 * @date 16/02/2022
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
 * the class ConfigurationLoaderHashCRCTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "ConfigurationLoaderHashCRC.h"
#include "ConfigurationLoaderHashCRCTest.h"
#include "CRC.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ConfigurationLoaderHashCRCTest::TestConstructor() {
    using namespace MARTe;
    ConfigurationLoaderHashCRC test;
    return true;
}

bool ConfigurationLoaderHashCRCTest::TestGetSeed() {
    using namespace MARTe;
    ConfigurationLoaderHashCRC test;
    return (test.GetSeed() > 0u);
}

bool ConfigurationLoaderHashCRCTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationLoaderHashCRC test;
    ConfigurationDatabase cdb;
    cdb.Write("Key", 0x1);
    bool ok = test.Initialise(cdb);

    return ok;
}

bool ConfigurationLoaderHashCRCTest::TestInitialise_NoKey() {
    using namespace MARTe;
    ConfigurationLoaderHashCRC test;
    ConfigurationDatabase cdb;
    bool ok = !test.Initialise(cdb);
    return ok;
}

bool ConfigurationLoaderHashCRCTest::TestComputeHash() {
    using namespace MARTe;
    ConfigurationLoaderHashCRC test;
    ConfigurationDatabase cdb;
    CRC<uint32> crc;
    uint32 key = 0x1;
    crc.ComputeTable(key);
    cdb.Write("Key", key);
    bool ok = test.Initialise(cdb);
    if (ok) {
        uint32 lastSeed = test.GetSeed();
        uint32 hashMem[2] = {lastSeed, 0x12345678};
        uint32 computedHash = crc.Compute(reinterpret_cast<uint8 *>(&hashMem[0]), 2 * sizeof(uint32), 0u, false);
        //Do not include the seed
        ok = (computedHash == test.ComputeHash(reinterpret_cast<char8 *>(&hashMem[1]), sizeof(uint32)));
    }
    return ok;
}



