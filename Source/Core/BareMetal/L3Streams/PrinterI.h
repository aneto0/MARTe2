/**
 * @file PrinterI.h
 * @brief Header file for class PrinterI
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

 * @details This header file contains the declaration of the class PrinterI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PRINTERI_H_
#define PRINTERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "BufferedStreamI.h"
#include "StreamString.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Interface to declare the prototype to write on a stream using
 * a specific language that depends on the implementation of the abstract methods.
 */
class PrinterI {
public:

    /**
     * @brief Default constructor
     */
    PrinterI();

    /**
     * @brief Constructor. Sets a pointer to the stream where to print to.
     * @param[in] streamIn is the stream to write to.
     */
    PrinterI(BufferedStreamI & streamIn);

    /**
     * @brief Destructor
     */
    virtual ~PrinterI();

    /**
     * @brief Sets the stream where to write on.
     * @param[in] streamIn the stream where to write on.
     */
    virtual void SetStream(BufferedStreamI & streamIn);

    /**
     * @brief Prints the open matrix separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenMatrix() = 0;

    /**
     * @brief Prints the close matrix separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseMatrix() = 0;

    /**
     * @brief Prints the scalar separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintScalarSeparator() = 0;

    /**
     * @brief Prints the vector separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintVectorSeparator() = 0;

    /**
     * @brief Prints the variable separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintVariableSeparator() = 0;

    /**
     * @brief Prints the block separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintBlockSeparator() = 0;

    /**
     * @brief Prints the open vector separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenVector() = 0;

    /**
     * @brief Prints the close vector separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseVector() = 0;

    /**
     * @brief Prints the open block separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenBlock(const char8 * const blockName) = 0;

    /**
     * @brief Prints the close block separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseBlock(const char8 * const blockName) = 0;

    /**
     * @brief Prints the open assignment separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] varName the name of the variable
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenAssignment(const char8 * const varName) = 0;

    /**
     * @brief Prints the close assignment separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] varName the name of the variable
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseAssignment(const char8 * const varName) = 0;

    /**
     * @brief Prints the variable on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] var the variable to be printed.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintVariable(const AnyType &var) = 0;

    /**
     * @brief Very first print to be performed.
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintBegin();

    /**
     * @brief Very last print to be performed.
     * @return true if the print succeeds, false otherwise.
     * @pre
     *   stream != NULL
     */
    virtual bool PrintEnd();

    /**
     * @brief Prints the open array of nodes separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief Prints the close array of nodes separator on the stream
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief Prints a block inside an array of blocks.
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintOpenInsideArrayOfBlocks(const char8 * const blockName);

    /**
     * @brief Closes a block inside an array of blocks.
     * @return true if the print succeeds, false otherwise.
     * @param[in] blockName the name of the block
     * @pre
     *   stream != NULL
     */
    virtual bool PrintCloseInsideOfArrayOfBlocks(const char8 * const blockName);


    /**
     * @brief Prints a StructuredDataI to the output stream.
     * @param[in] sdi the StructuredDataI to print.
     * @param[in] collapseArraysOfNodes if true, arrays of nodes (e.g. A[0] = {}, A[1] {}, ...) will be collapsed into a single array (e.g. A = [{}, {}, ...)
     * @param[in] level current recursion level
     * @return true if the StructuredDataI is sucessfully printed.
     */
    bool PrintStructuredDataI(StructuredDataI &sdi, bool collapseArraysOfNodes, uint32 level = 0u);

protected:

    /**
     * Pointer to the stream where to write on
     */
    BufferedStreamI *stream;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PRINTERI_H_ */

