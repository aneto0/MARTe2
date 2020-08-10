/**
 * @file IOBufferPrintFormatter.h
 * @brief Header file for class AnyType
 * @date Aug 3, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef IOBUFFERPRINTFORMATTER_H_
#define IOBUFFERPRINTFORMATTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"
#include "IOBuffer.h"
#include "FormatDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * Groups of functions to help produce formatted prints of structures
 */
namespace PrintFormatter{

/*
 * @brief prints the characters needed to start a group of objects
 * @details CFG '{' XML ,JSON '{'.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenBlock(DesiredGrammar grammar,IOBuffer &out);

/*
 * @brief prints the characters needed to end a group of objects
 * @details CFG '}' XML "",JSON '}'.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseBlock(DesiredGrammar grammar,IOBuffer &out);

/*
 * @brief prints the characters needed to start an array of objects
 * @details  CFG '{' JSON,XML '['.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenArray(DesiredGrammar grammar,IOBuffer &out);

/*
 * @brief prints the characters needed to end an array of objects
 * @details  CFG '}' JSON XML ']'.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseArray(DesiredGrammar grammar,IOBuffer &out);

/*
 * @brief prints the characters needed to separate objects
 * @details CFG ',' XML ',' ,JSON ','. introduces also a space after the first element and a newline every ten elements
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] number is the element number before which to put the separator.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType Separator(DesiredGrammar grammar,uint32 number,IOBuffer &out);

/*
 * @brief prints the characters needed to start an assignment
 * @details CFG ' name =' XML '<name>' ,JSON '"name" :'.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] name is the name used in the assignment left side.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenAssignMent(DesiredGrammar grammar,CCString name,IOBuffer &out);

/*
 * @brief prints the characters needed to  end an assignment
 * @details CFG '' XML '<\name>' ,JSON ''.
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] name is the name used in the assignment left side.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseAssignMent(DesiredGrammar grammar,CCString name,IOBuffer &out);

#if 0
/*
 * @brief prints the characters needed to emit a string
 * @details CFG XML verbatim ,JSON adds ""
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] string is the string to print.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline bool StringField(DesiredGrammar grammar,CCString string,IOBuffer &out);

/*
 * @brief prints the characters needed to emit a single character string
 * @details CFG XML verbatim ,JSON adds ""
 * @param[in] grammar is the constant that defines the chosen grammar to use
 * @param[in] c is the charcter to print.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline bool CharField(DesiredGrammar grammar,char8 c,IOBuffer &out);
#endif

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ErrorManagement::ErrorType PrintFormatter::OpenBlock(DesiredGrammar grammar,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInXMLGrammar:{

    } break;
    case PrintInJsonGrammar:
    default:{
        ret.OSError = !out.PutC('{');
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::CloseBlock(DesiredGrammar grammar,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInXMLGrammar:{

    } break;
    case PrintInJsonGrammar:
    default:{
        ret.OSError = !out.PutC('}');
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::OpenArray(DesiredGrammar grammar,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInJsonGrammar:
    case PrintInXMLGrammar:{
        ret.OSError = !out.PutC('[');
    } break;
    default:{
        ret.OSError = !out.PutC('{');
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::CloseArray(DesiredGrammar grammar,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInJsonGrammar:
    case PrintInXMLGrammar:{
        ret.OSError = !out.PutC(']');
    } break;
    default:{
        ret.OSError = !out.PutS("}\n");
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::Separator(DesiredGrammar grammar,uint32 nEl,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    if (nEl > 0){
        ret.OSError = !out.PutC(',');
        if (ret && ((nEl % 10)==9)) {
            ret.OSError = !out.PutC('\n');
        }
     }
    return ret;
}

ErrorManagement::ErrorType PrintFormatter::OpenAssignMent(DesiredGrammar grammar,CCString name,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInJsonGrammar:{
        bool ret2 = out.PutC('"');
        ret2 = ret2 && out.PutS(name);
        ret2 = ret2 && out.PutS("\" : ");
        ret.OSError = !ret2;
    }break;
    case PrintInXMLGrammar:{
        bool ret2 = out.PutC('<');
        ret2 = ret2 && out.PutS(name);
        ret2 = ret2 &&out.PutC('>');
        ret.OSError = !ret2;
    } break;
    default:{
        bool ret2 = out.PutS(name);
        ret2 = ret2 && out.PutS(" = ");
        ret.OSError = !ret2;
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::CloseAssignMent(DesiredGrammar grammar,CCString name,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(grammar){
    case PrintInXMLGrammar:{
        bool ret2 = out.PutS("</");
        ret2 = ret2 && out.PutS(name);
        ret2 = ret2 && out.PutC('>');
        ret.OSError = !ret2;
    } break;
    case PrintInJsonGrammar:{

    }break;
    default:{
        ret.OSError = !out.PutC('\n');
    }
    };

    return ret;
}

#if 0
bool PrintFormatter::StringField(DesiredGrammar grammar,CCString string,IOBuffer &out){
    bool ret = true;

    switch(grammar){
    case PrintInJsonGrammar:{
        ret = ret && out.PutC('"');
        ret = ret && out.PutS(string);
        ret = ret && out.PutC('"');
    } break;
    case PrintInXMLGrammar:
    default:{
        ret = out.PutS(string);
    }
    };

    return ret;
}

bool PrintFormatter::CharField(DesiredGrammar grammar,char8 c,IOBuffer &out){
    bool ret = true;

    switch(grammar){
    case PrintInJsonGrammar:{
        ret = ret && out.PutC('"');
        ret = ret && out.PutC(c);
        ret = ret && out.PutC('"');
    } break;
    case PrintInXMLGrammar:
    default:{
        ret = out.PutC(c);
    }
    };

    return ret;
}
#endif

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_PRIVATE_IOBUFFERPRINTFORMATTER_H_ */
