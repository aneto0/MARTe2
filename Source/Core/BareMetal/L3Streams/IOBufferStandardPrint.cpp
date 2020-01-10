/**
 * @file IOBufferStandardPrint.cpp
 * @brief Source file for class IOBufferStandardPrint
 * @date 06/09/2018
 * @author Giuseppe Ferro
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
 * the class IOBufferStandardPrint (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"
#include "IOBuffer.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Prints the Standard open matrix separator.
 * @param[in] iobuff the IOBuffer where to print to.
 */
bool PrintStandardOpenMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('{');
}

/**
 * @brief Prints the Standard close matrix separator.
 * @param[in] iobuff the IOBuffer where to print to.
 */
bool PrintStandardCloseMatrix(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

/**
 * @brief Prints the Standard open vector separator.
 * @param[in] iobuff the IOBuffer where to print to.
 */
bool PrintStandardOpenVector(IOBuffer &iobuff) {
    return iobuff.PutC('{');

}

/**
 * @brief Prints the Standard close vector separator.
 * @param[in] iobuff the IOBuffer where to print to.
 */
bool PrintStandardCloseVector(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

/**
 * @brief Prints the Standard open block separator.
 * @param[in] iobuff the IOBuffer where to print to.
 * @param[in] blockName the name of the block (structure) to output.
 */
bool PrintStandardOpenBlock(IOBuffer &iobuff, const char8 * const blockName) {
    AnyType at[] = { blockName, voidAnyType };
    return iobuff.PrintFormatted("%s = {", &at[0]);
}

/**
 * @brief Prints the Standard close block separator.
 * @param[in] iobuff the IOBuffer where to print to.
 */
bool PrintStandardCloseBlock(IOBuffer &iobuff) {
    return iobuff.PutC('}');
}

/**
 * @brief Prints the Standard open assignment separator.
 * @param[in] iobuff the IOBuffer where to print to.
 * @param[in] varName the variable name.
 */
bool PrintStandardOpenAssignment(IOBuffer &iobuff, const char8 * const varName) {
    uint32 size = StringHelper::Length(varName);
    bool ret = iobuff.WriteAll(varName, size);
    if (ret) {
        size = 2u;
        ret = iobuff.WriteAll(" =", size);
    }
    return ret;
}

}

