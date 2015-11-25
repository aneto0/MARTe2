/**
 * @file Token.h
 * @brief Header file for class Token
 * @date 25/11/2015
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

 * @details This header file contains the declaration of the class Token
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TOKEN_H_
#define TOKEN_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief The element produced by the LexicalAnalyzer.
 */
class Token {

public:
    /** builds a token from a token description constant and the data content*/
    Token(uint32 id,
          const char8 *description,
          const char8 *data,
          uint32 lineNumber) {
        tokenId = id;
        tokenDescription = description;
        tokenData = data;
        tokenLineNumber = lineNumber;
    }

    /** builds a token from a simple character */
    Token(char8 c,
          uint32 lineNumber) {
        tokenId = c;
        tokenDescription = c;
        tokenData = c;
        tokenLineNumber = lineNumber;
    }

    /** destructor */
    virtual ~Token() {
    }

    /** returns the TokenDataType */
    uint32 Token() {
        return tokenId;
    }

    /** returns the line number */
    uint32 LineNumber() {
        return tokenLineNumber;
    }

    /** copy content */
    void operator=(LA_TokenData &td) {
        tokenId = td.token;
        tokenData = td.data;
        tokenDescription = td.description;
    }

    /** descrizione del contenuto */
    const char * Description() {
        return tokenDescription.Buffer();
    }

    /** il contenuto */
    const char * Data() {
        return tokenData.Buffer();
    }
private:
    /** the code identifying the lexical meaning of this part of the text */
    uint32 tokenId;

    /** copied from a LA_TokenInfo, the meaning of the token */
    StreamString tokenDescription;

    /** the parsed part of the text */
    StreamString tokenData;

    /** what line the token was found at*/
    uint32 tokenLineNumber;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TOKEN_H_ */

