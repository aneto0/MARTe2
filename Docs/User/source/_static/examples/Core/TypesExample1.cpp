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
namespace MARTe2Tutorial {
/**
 * @brief Print the input variable value, together with its characteristics.
 * @input[in] input the variable to print.
 */
void PrintType(const MARTe::AnyType &input) {
    using namespace MARTe;
    //Get the TypeDescriptor
    TypeDescriptor td = input.GetTypeDescriptor();
    bool isConstant = td.isConstant;
    uint32 numberOfBits = td.numberOfBits;
    //Automatically retrieve the name
    const char8 *const typeName = TypeDescriptor::GetTypeNameFromTypeDescriptor(td);
    //The logging mechanism will automatically print the value (note the %!) based on the TypeDescriptor.
    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Type: %s; NumberOfBits: %d; Constant? :%d, "
            "Value: %!", typeName, numberOfBits, isConstant, input);
}

/**
 * @brief The instances of AnyType do not hold a copy of the actual value, but a pointer to it, so the value
 *  must be accessible during the life of the AnyType instance!
 */
MARTe::AnyType DoNotDoThisEver() {
    MARTe::uint16 aValue = 3;
    return aValue;
}

}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    uint8 anUInt8 = 18;
    uint16 anUInt16 = 116;
    //Note that the AnyType is automatically constructed.
    PrintType(anUInt8);
    PrintType(anUInt16);
    PrintType(32.0);
    bool aBoolean = true;
    PrintType(aBoolean);
    //An AnyType can also be manually constructed (but this is not the main use case)
    AnyType at(UnsignedInteger16Bit, 0u, &anUInt16);
    PrintType(at);

    (void) DoNotDoThisEver();
    return 0;
}

