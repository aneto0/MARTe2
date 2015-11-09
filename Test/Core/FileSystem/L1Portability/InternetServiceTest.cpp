/**
 * @file InternetServiceTest.cpp
 * @brief Source file for class InternetServiceTest
 * @date 29/10/2015
 * @author Giuseppe Ferrò
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
 * the class InternetServiceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "InternetServiceTest.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool InternetServiceTest::TestDefaultConstructor() {
    InternetService service;

    if (service.Port() != 0) {
        return false;
    }

    if (service.Name() != NULL) {
        return false;
    }

    return service.Protocol() == NULL;

}

bool InternetServiceTest::TestSearchByName(const InternetServiceTestTable* table) {

    InternetService service;

    uint32 i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByName(table[i].name, table[i].protocol);
        if (table[i].expected) {

            if (service.Port() != table[i].port) {
                return false;
            }
            if (StringHelper::Compare(service.Name(), table[i].name) != 0) {
                if (table[i].name != NULL) {
                    return false;
                }
            }
            if (StringHelper::Compare(service.Protocol(), table[i].expectedProtocol) != 0) {
                return false;
            }
        }
        if (ret != table[i].expected) {
            return false;

        }
        i++;
    }
    return true;
}

bool InternetServiceTest::TestSearchByPort(const InternetServiceTestTable* table) {
    InternetService service;

    uint32 i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByPort(table[i].port, table[i].protocol);
        if (table[i].expected) {

            if (service.Port() != table[i].port) {
                return false;
            }
            if (StringHelper::Compare(service.Name(), table[i].name) != 0) {
                if (table[i].name != NULL) {
                    return false;
                }
            }
            if (StringHelper::Compare(service.Protocol(), table[i].expectedProtocol) != 0) {
                return false;
            }
        }
        if (ret != table[i].expected) {
            return false;

        }
        i++;
    }
    return true;
}

bool InternetServiceTest::TestPort(const InternetServiceTestTable* table) {
    InternetService service;

    uint32 i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByName(table[i].name, table[i].protocol);
        if (table[i].expected) {

            if (service.Port() != table[i].port) {
                return false;
            }
        }
        if (ret != table[i].expected) {
            return false;

        }
        i++;
    }
    return true;
}

bool InternetServiceTest::TestName(const InternetServiceTestTable* table) {
    InternetService service;

    uint32 i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByPort(table[i].port, table[i].protocol);
        if (table[i].expected) {

            if (StringHelper::Compare(service.Name(), table[i].name) != 0) {
                if (table[i].name != NULL) {
                    return false;
                }
            }
        }
        if (ret != table[i].expected) {
            return false;

        }
        i++;
    }
    return true;
}

bool InternetServiceTest::TestProtocol(const InternetServiceTestTable* table) {
    InternetService service;

    uint32 i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByPort(table[i].port, table[i].protocol);
        if (table[i].expected) {

            if (StringHelper::Compare(service.Protocol(), table[i].expectedProtocol) != 0) {
                return false;
            }
        }
        if (ret != table[i].expected) {
            return false;
        }
        i++;
    }

    i = 0;
    while (table[i].name != NULL) {

        bool ret = service.SearchByName(table[i].name, table[i].protocol);
        if (table[i].expected) {

            if (StringHelper::Compare(service.Protocol(), table[i].expectedProtocol) != 0) {
                return false;
            }
        }
        if (ret != table[i].expected) {
            return false;
        }
        i++;
    }

    return true;
}


