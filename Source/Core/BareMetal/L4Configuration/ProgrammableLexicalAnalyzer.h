/**
 * @file ProgrammableLexicalAnalyzer.h
 * @brief Header file for class ProgrammableLexicalAnalyzer
 * @date Apr 29, 2020
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

#ifndef PROGRAMMABLE_LEXICALANALYZER_H_
#define PROGRAMMABLE_LEXICALANALYZER_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamI.h"
//#include "Token.h"
#include "CCString.h"
#include "StaticList.h"
#include "RegularExpression.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief Generic lexical analyzer (lexer) which allows to interpret a stream
 * of characters as a sequence of tokens, applying specific lexical rules set
 * at instance level.
 *
 * @details Each instance of the lexer is bound when it is constructed, with
 * the stream of characters to be analysed, which will remain the same during
 * the object lifetime. At construction time, too, the lexer is initialized
 * with the configuration of lexical elements that will rule the analysis
 * (terminals, separators, and comment markers).
 *
 * Once the lexer is created, it allows its users to read tokens from the
 * stream of characters one by one, being the lexer responsible of applying
 * the lexical rules of the configuration.
 *
 */
/*lint -e1712 . Justification: This class must be as per the only defined
 * constructor. No need for a default constructor.*/
class DLL_API ProgrammableLexicalAnalyzer {

public:
	/**
	 *
	 */
	struct Token {
		/**
		 *
		 */
		const RegularExpression::PatternInformation *matchedRule;

		/**
		 *
		 */
		DynamicCString 								 matchedString;

		/**
		 * constructor
		 */
		inline Token();

	    /**
	     * @brief Retrieves the token identifier.
	     * @return the token identifier.
	     */
	    inline uint32 GetId() const;

	    /**
	     * @brief Retrieves the token description.
	     * @return the token description.
	     */
	    inline CCString GetDescription() const ;

	    /**
	     * @brief Retrieves the token data.
	     * @return the token data.
	     */
	    inline CCString GetData() const ;

	};

    /**
     * @brief Constructor which initializes the instance with the stream of
     * characters to analyze and the configuration of the analyzer (terminals,
     * separators, and comment markers).
     * @param[in] rules is an array of LexicalAnalyzerRule terminated with an element made of zeroes
     */
    inline ProgrammableLexicalAnalyzer(const ZeroTerminatedArray<const RegularExpression::PatternInformation> &ruleSet, StreamI &streamIn);

    /**
     * @brief Destructor.
     */
    inline ~ProgrammableLexicalAnalyzer();

    /**
     * @brief deletes the old token. makes sure there is a new top token in the Q
     * @param[out] token is a reference to the start of the Q where the next token is present
     * @return a pointer to the next token found in the stream.
     */
    ErrorManagement::ErrorType GetToken(const Token *&token);

    /**
     * @brief Returns the nth token in the stream without consuming it.
     * @param[in] position is the position of the nth token in the stream (0 means the last token returned by GetToken).
     * @return the nth token found in the stream.
     */
    ErrorManagement::ErrorType PeekToken(const uint32 position,const Token *&token);

private:

    /**
     *
     */
    const ZeroTerminatedArray<const RegularExpression::PatternInformation> ruleSet;

    /**
     * Internal token queue
     */
    StaticList<const Token > 											    tokenQueue;

    /**
     *
     */
    StreamI &																stream;
};



/*---------------------------------------------------------------------------*/
/*                           Inline functions                                */
/*---------------------------------------------------------------------------*/

ProgrammableLexicalAnalyzer::ProgrammableLexicalAnalyzer(const ZeroTerminatedArray<const RegularExpression::PatternInformation> &ruleSetIn, StreamI &streamIn):ruleSet(ruleSetIn),stream(streamIn){
}


ProgrammableLexicalAnalyzer::~ProgrammableLexicalAnalyzer(){

}


ProgrammableLexicalAnalyzer::Token::Token(){
	matchedRule = NULL;
}

uint32 ProgrammableLexicalAnalyzer::Token::GetId() const{
	uint32 ret = 0;
	if (matchedRule != NULL){
		ret = matchedRule->ruleId;
	}
	return ret;
}

CCString ProgrammableLexicalAnalyzer::Token::GetDescription() const{
	CCString ret;
	if (matchedRule != NULL){
		ret = matchedRule->ruleName;
	}
	return ret;
}

CCString ProgrammableLexicalAnalyzer::Token::GetData() const{
	CCString ret;
	if (matchedRule != NULL){
		ret = this->matchedString;
	}
	return ret;
}

} // MARTe


#endif /*LEXICALANALYZER2_H_ */
