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

/*lint -e{9141} [MISRA C++ Rule 7-3-1]. Justification: 'SpeedTable' declared "
    "as global.*/
struct SpeedTable {
    MARTe::int32 code;
    MARTe::uint32 speed;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{1401} [MISRA C++ Rule 8-5-1]. Justification: FD_ZERO arguments
        correctly initialised.*/
BasicUART::BasicUART() {
}

BasicUART::~BasicUART() {
}

bool BasicUART::SetSpeed(const uint32 speed) {
    return false;
}
/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'name'
                kept as non const.*/
bool BasicUART::Open(const char8* name) {
    return false;
}

void BasicUART::Close() {
    return false;
}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'buffer' kept
        as non const.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Member function :Read()
        kept as non const.*/
bool BasicUART::Read(char8* buffer, uint32 &size) {
    return false;
}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'buffer' kept
        as non const.*/
bool BasicUART::Read(char8* buffer, uint32 &size, const uint32 timeoutUsec) {
    return false;
}

/*lint -e{952} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'size' kept as
        non const.*/
/*lint -e{1762} [MISRA C++ Rule 9-3-3]. Justification: Member function :Read()
        kept as non const.*/
/*lint -e{818} [MISRA C++ Rule 7-1-1]. Justification: Parameter 'size' kept as
        non const.*/
bool BasicUART::Write(char8 *buffer, uint32 size) {
    return false;
}

bool BasicUART::WaitRead(const uint32 timeoutUsec) {
    return false;
}

bool BasicUART::WaitWrite(const uint32 timeoutUsec) {
    return false;
}

}
