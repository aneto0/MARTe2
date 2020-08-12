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
 * @param[in,out] fd indentation level is updated
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenBlock(FormatDescriptor & fd,IOBuffer &out);

/*
 * @brief prints the characters needed to end a group of objects
 * @details CFG '}' XML "",JSON '}'.
 * @param[in,out] fd indentation level is updated
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseBlock(FormatDescriptor & fd,IOBuffer &out);

/*
 * @brief prints the characters needed to start an array of objects
 * @details  CFG '{' JSON,XML '['.
 * @param[in] fd.desiredGrammar is the constant that defines the chosen fd.desiredGrammar to use
 * @param[in] out is the stream to print into.
 * @param[in] level 0 means the start of the row, 1 the start of the columns.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenArray(FormatDescriptor & fd,uint32 level,IOBuffer &out);

/*
 * @brief prints the characters needed to end an array of objects
 * @details  CFG '}' JSON XML ']'.
 * @param[in] fd.desiredGrammar is the constant that defines the chosen fd.desiredGrammar to use
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseArray(FormatDescriptor & fd,uint32 level,IOBuffer &out);

/*
 * @brief prints the characters needed to separate objects
 * @details CFG ',' XML ',' ,JSON ','. introduces also a space after the first element and a newline every ten elements
 * @param[in] fd.desiredGrammar is the constant that defines the chosen fd.desiredGrammar to use
 * @param[in] number is the element number before which to put the separator.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType Separator(FormatDescriptor fd,uint32 number,uint32 level,IOBuffer &out);

/*
 * @brief prints the characters needed to start an assignment
 * @details CFG ' name =' XML '<name>' ,JSON '"name" :'.
 * @param[in] fd.desiredGrammar is the constant that defines the chosen fd.desiredGrammar to use
 * @param[in] name is the name used in the assignment left side.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType OpenAssignMent(FormatDescriptor fd,CCString name,IOBuffer &out);

/*
 * @brief prints the characters needed to  end an assignment
 * @details CFG '' XML '<\name>' ,JSON ''.
 * @param[in] fd.desiredGrammar is the constant that defines the chosen fd.desiredGrammar to use
 * @param[in] name is the name used in the assignment left side.
 * @param[in] out is the stream to print into.
 * @return false in case of errors.
 */
inline ErrorManagement::ErrorType CloseAssignMent(FormatDescriptor fd,CCString name,IOBuffer &out);

inline void Indent(FormatDescriptor fd,IOBuffer &out);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void PrintFormatter::Indent(FormatDescriptor fd,IOBuffer &out){
    for (uint32 i = 0; i < fd.nOfIndentationCharacters(); i++){
        out.PutC(' ');
    }
}


ErrorManagement::ErrorType PrintFormatter::OpenBlock(FormatDescriptor & fd,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(fd.desiredGrammar){
    case PrintInXMLGrammar:{
    } break;
    case PrintInJsonGrammar:
    default:{
        ret.OSError = !out.PutC('{');
    }
    };
    ret.OSError = !out.PutC('\n');
    fd.IncreaseIndentation();
    Indent(fd,out);

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::CloseBlock(FormatDescriptor & fd,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    ret.OSError = !out.PutC('\n');
    fd.DecreaseIndentation();
    Indent(fd,out);

    switch(fd.desiredGrammar){
    case PrintInXMLGrammar:{

    } break;
    case PrintInJsonGrammar:
    default:{
        ret.OSError = !out.PutC('}');
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::OpenArray(FormatDescriptor & fd,uint32 level,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(fd.desiredGrammar){
    case PrintInJsonGrammar:
    case PrintInXMLGrammar:{
        ret.OSError = !out.PutC('[');
    } break;
    default:{
        ret.OSError = !out.PutC('{');
    }
    };

    if ((level != 0)&& ret){
        ret.OSError = !out.PutC('\n');
        fd.IncreaseIndentation();
        Indent(fd,out);
    }

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::CloseArray(FormatDescriptor & fd,uint32 level,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    if ((level != 0)&& ret){
        ret.OSError = !out.PutC('\n');
        fd.DecreaseIndentation();
        Indent(fd,out);
    }

    switch(fd.desiredGrammar){
    case PrintInJsonGrammar:
    case PrintInXMLGrammar:{
        ret.OSError = !out.PutC(']');
    } break;
    default:{
        ret.OSError = !out.PutC('}');
    }
    };

    return ret;
}

ErrorManagement::ErrorType PrintFormatter::Separator(FormatDescriptor fd,uint32 elementNumber,uint32 level,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    if (elementNumber > 0){
        ret.OSError = !out.PutC(',');
        if (ret && (((elementNumber % 20)== 0) || (level > 0))) {
            ret.OSError = !out.PutC('\n');
            Indent(fd,out);
        }
     }
    return ret;
}

ErrorManagement::ErrorType PrintFormatter::OpenAssignMent(FormatDescriptor fd,CCString name,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(fd.desiredGrammar){
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

ErrorManagement::ErrorType PrintFormatter::CloseAssignMent(FormatDescriptor fd,CCString name,IOBuffer &out){
    ErrorManagement::ErrorType ret;

    switch(fd.desiredGrammar){
    case PrintInXMLGrammar:{
        bool ret2 = out.PutS("</");
        ret2 = ret2 && out.PutS(name);
        ret2 = ret2 && out.PutC('>');
        ret.OSError = !ret2;
    } break;
    case PrintInJsonGrammar:{

    }break;
    default:{
    }
    };

    ret.OSError = !out.PutC('\n');
    Indent(fd,out);

    return ret;
}

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L3STREAMS_PRIVATE_IOBUFFERPRINTFORMATTER_H_ */
