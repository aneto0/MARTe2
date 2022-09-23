/**
 * @file JsonPrinter.h
 * @brief Header file for class JsonPrinter
 * @date 07/09/2018
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

 * @details This header file contains the declaration of the class JsonPrinter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef JSONPRINTER_H_
#define JSONPRINTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "PrinterI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief The Json language printer.
 * @see PrinterI
 */
class JsonPrinter: public PrinterI {
public:
    /**
     * @brief Constructor
     */
    JsonPrinter();

    /**
     * @brief Constructor. Sets a pointer to the stream where to print to.
     * @param[in] streamIn is the stream to write to.
     */
    JsonPrinter(BufferedStreamI & streamIn);

    /**
     * @brief Destructor
     */
    virtual ~JsonPrinter();

    /**
     * @see PrinterI::PrintOpenMatrix
     */
    virtual bool PrintOpenMatrix();

    /**
     * @see PrinterI::PrintCloseMatrix
     */
    virtual bool PrintCloseMatrix();

    /**
     * @see PrinterI::PrintScalarSeparator
     */
    virtual bool PrintScalarSeparator();

    /**
     * @see PrinterI::PrintVectorSeparator
     */
    virtual bool PrintVectorSeparator();

    /**
     * @see PrinterI::PrintVariableSeparator
     */
    virtual bool PrintVariableSeparator();

    /**
     * @see PrinterI::PrintBlockSeparator
     */
    virtual bool PrintBlockSeparator();

    /**
     * @see PrinterI::PrintOpenVector
     */
    virtual bool PrintOpenVector();

    /**
     * @see PrinterI::PrintCloseVector
     */
    virtual bool PrintCloseVector();

    /**
     * @see PrinterI::PrintOpenBlock
     */
    virtual bool PrintOpenBlock(const char8 *const blockName);

    /**
     * @see PrinterI::PrintCloseBlock
     */
    virtual bool PrintCloseBlock(const char8 *const blockName);

    /**
     * @see PrinterI::PrintOpenAssignment
     */
    virtual bool PrintOpenAssignment(const char8 *const varName);

    /**
     * @see PrinterI::PrintCloseAssignment
     */
    virtual bool PrintCloseAssignment(const char8 *const varName);

    /**
     * @see PrinterI::PrintVariable
     */
    virtual bool PrintVariable(const AnyType &var);

    /**
     * @brief Prints the { character.
     */
    virtual bool PrintBegin();

    /**
     * @brief Prints the } character.
     */
    virtual bool PrintEnd();

    /**
     * @brief Prints "blockName" : [ 
     */
    virtual bool PrintOpenArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief Prints the ] character
     */
    virtual bool PrintCloseArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief NOOP
     */
    virtual bool PrintOpenInsideArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief NOOP
     */
    virtual bool PrintCloseInsideOfArrayOfBlocks(const char8 * const blockName);

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_JSONPRINTER_H_ */

