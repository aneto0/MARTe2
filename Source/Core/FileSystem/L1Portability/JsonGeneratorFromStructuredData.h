/**
 * @file JsonGeneratorFromStructuredData.h
 * @brief Header file for class JsonGeneratorFromStructuredData
 * @date 08/03/2016
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class JsonGeneratorFromStructuredData
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef JSON_GENERATOR_FROM_STRUCTUREDDATA
#define JSON_GENERATOR_FROM_STRUCTUREDDATA

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief A class that generates JSON strings from StructuredData objects.
 *
 * @details This class offers an execute method that generates a StreamString
 * object from a StructuredData object. The resulting StreamString contains a
 * string which represents the contents of the StructuredData object in JSON
 * syntax.
 *
 * Grammar of the JSON syntax (compact version extracted from RFC 7159):
 * JSON-text = ws value ws
 * value = false / null / true / object / array / number / string
 * false = %x66.61.6c.73.65   ; false
 * null  = %x6e.75.6c.6c      ; null
 * true  = %x74.72.75.65      ; true
 * object = begin-object [ member *( value-separator member ) ] end-object
 * member = string name-separator value
 * array = begin-array [ value *( value-separator value ) ] end-array
 * begin-array     = ws %x5B ws  ; [ left square bracket
 * begin-object    = ws %x7B ws  ; { left curly bracket
 * end-array       = ws %x5D ws  ; ] right square bracket
 * end-object      = ws %x7D ws  ; } right curly bracket
 * name-separator  = ws %x3A ws  ; : colon
 * value-separator = ws %x2C ws  ; , comma
 * ws = *(
 *         %x20 /              ; Space
 *         %x09 /              ; Horizontal tab
 *         %x0A /              ; Line feed or New line
 *         %x0D )              ; Carriage return *
 * number = [ minus ] int [ frac ] [ exp ]
 * decimal-point = %x2E       ; .
 * digit1-9 = %x31-39         ; 1-9
 * e = %x65 / %x45            ; e E
 * exp = e [ minus / plus ] 1*DIGIT
 * frac = decimal-point 1*DIGIT
 * int = zero / ( digit1-9 *DIGIT )
 * minus = %x2D               ; -
 * plus = %x2B                ; +
 * zero = %x30                ; 0
 * string = quotation-mark *char quotation-mark
 * char = unescaped /
 *        escape (
 *            %x22 /          ; "    quotation mark  U+0022
 *            %x5C /          ; \    reverse solidus U+005C
 *            %x2F /          ; /    solidus         U+002F
 *            %x62 /          ; b    backspace       U+0008
 *            %x66 /          ; f    form feed       U+000C
 *            %x6E /          ; n    line feed       U+000A
 *            %x72 /          ; r    carriage return U+000D
 *            %x74 /          ; t    tab             U+0009
 *            %x75 4HEXDIG )  ; uXXXX                U+XXXX
 * escape = %x5C              ; \
 * quotation-mark = %x22      ; "
 * unescaped = %x20-21 / %x23-5B / %x5D-10FFFF
 *
 * Remark 1: The generated JSON does not contain any insignificant whitespace
 * (space, horizontal tab, line feed, carriage return) before or after any
 * of the six structural characters.
 *
 * Remark 2: The numbers produced by this generator never contain an exponent
 * part, but they always contain a fractional part when the number is a float.
 *
 * References:
 * + JSON's website [JSON|http://www.json.org]
 * + Wikipedia's JSON article [JSON|https://en.wikipedia.org/wiki/JSON]
 * + The JavaScript Object Notation (JSON) Data Interchange Format [Standard RFC 7159|https://tools.ietf.org/html/rfc7159]
 * + The JSON Data Interchange Format [Standard ECMA-404|http://www.ecma-international.org/publications/standards/Ecma-404.htm]
 *
 */
class DLL_API JsonGeneratorFromStructuredData {

public:

    /**
     * @brief Default constructor
     */
    JsonGeneratorFromStructuredData();

    /**
     * @brief Destructor
     */
    virtual ~JsonGeneratorFromStructuredData();

    /**
     * @brief Generates a StreamString object from a StructuredData object,
     * which contains a string representation of the contents of the
     * StructuredData object in JSON syntax.
     * @param[in] input The StructuredData object which contains the data to
     * be transformed into a JSON string.
     * @param[out] output The StreamString object which contains the data
     * inside input as a JSON string.
     * @return true if the generation succeed, false otherwise.
     *
     */
    bool Execute (MARTe::StructuredDataI& input, MARTe::StreamString& output);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* JSON_GENERATOR_FROM_STRUCTUREDDATA */

