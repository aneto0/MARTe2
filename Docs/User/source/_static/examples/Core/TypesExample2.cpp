/**
 * @file TypesExample1.cpp
 * @brief Source file for class TypesExample1
 * @date 14/03/2018
 * @author Andre' Neto
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
 * the class TypesExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "Matrix.h"
#include "Object.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StreamString.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    using namespace MARTe;

    printf("Character8Bit: [%d]\n", static_cast<uint16>(Character8Bit.all));
    printf("Float32Bit: [%d]\n", static_cast<uint16>(Float32Bit.all));
    printf("Float64Bit: [%d]\n", static_cast<uint16>(Float64Bit.all));
    printf("Float128Bit: [%d]\n", static_cast<uint16>(Float128Bit.all));
    printf("VoidType: [%d]\n", static_cast<uint16>(VoidType.all));
    printf("SignedInteger8Bit: [%d]\n", static_cast<uint16>(SignedInteger8Bit.all));
    printf("UnsignedInteger8Bit: [%d]\n", static_cast<uint16>(UnsignedInteger8Bit.all));
    printf("SignedInteger16Bit: [%d]\n", static_cast<uint16>(SignedInteger16Bit.all));
    printf("UnsignedInteger16Bit: [%d]\n", static_cast<uint16>(UnsignedInteger16Bit.all));
    printf("SignedInteger32Bit: [%d]\n", static_cast<uint16>(SignedInteger32Bit.all));
    printf("UnsignedInteger32Bit: [%d]\n", static_cast<uint16>(UnsignedInteger32Bit.all));
    printf("SignedInteger64Bit: [%d]\n", static_cast<uint16>(SignedInteger64Bit.all));
    printf("UnsignedInteger64Bit: [%d]\n", static_cast<uint16>(UnsignedInteger64Bit.all));
    printf("ConstCharString: [%d]\n", static_cast<uint16>(ConstCharString.all));
    printf("CharString: [%d]\n", static_cast<uint16>(CharString.all));
    printf("StructuredDataInterfaceType: [%d]\n", static_cast<uint16>(StructuredDataInterfaceType.all));
    printf("PointerType: [%d]\n", static_cast<uint16>(PointerType.all));
    printf("InvalidType: [%d]\n", static_cast<uint16>(InvalidType.all));

    return 0;
}

