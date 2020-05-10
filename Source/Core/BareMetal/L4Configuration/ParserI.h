/**
 * @file ParserI.h
 * @brief Header file for class ParserI
 * @date 09/12/2015
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

 * @details This header file contains the declaration of the class ParserI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PARSERI_H_
#define PARSERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "DynamicCString.h"
#include "ParserData.h"
#include "RegularExpression.h"
#include "StaticStack.h"
#include "List.h"
#include "StreamString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe {

#define PARSER_DIAGNOSTIC_REPORT(errorStream,level,format,...) 			\
if (debugLevel >=level){                                    \
	StreamString s;								 			\
	s.Printf(format,__VA_ARGS__);      						\
	if (errorStream) errorStream->Printf("%s",CCString(s.Buffer()));         \
	else {                          						\
		REPORT_ERROR(ErrorManagement::Debug,s.Buffer()); 	\
	}			 										 	\
}

#define PARSER_ERROR_REPORT(errorStream,ret,format,...)    	\
if (!ret){                                                  \
	StreamString s;								 			\
	s.Printf(format,__VA_ARGS__);      		        	    \
	if (errorStream) errorStream->Printf("%s",CCString(s.Buffer()) );        \
	REPORT_ERROR(ret,s.Buffer()); 	                        \
}



/**
 * @brief Abstract parser which allows to transform a stream of characters
 * into a structured data store, applying lexical rules set at instance level
 * and parsing rules implemented into subclasses.
 *
 * @details This class is a partial abstract class providing the generic
 * functionality of a parser while expecting subclasses to provide the specific
 * rules for each concrete language (each subclass must implement the pure
 * abstract functions).
 *
 * Each instance of the parser is bound when it is constructed, with all the
 * objects involved in the parsing analysis, as follows:
 * - An input stream of characters that contains the serialization of a
 * hierarchy of objects, encoded into a given language (e.g. XML, JSON, etc).
 * - An output structured data store where the parser will build the in-
 * memory objects defined into the input stream of characters.
 * - An output stream of characters where the parser will write all the errors
 * found on the input stream of characters.
 *
 * At construction time, too, the parser is initialized with the lexical
 * elements that the language used in the input stream of characters needs,
 * while the grammar of that language is expected to be implemented in
 * subclasses.
 *
 * After being properly created, each instance is ready to parse the input
 * stream of characters, whenever the user calls the method Parse().
 *
 * Notes about the input stream of characters:
 * - All the elements of a vector or matrix must be of the same token type
 * (NUMBER or STRING).
 * - Variables cannot be empty (i.e "scalar = " or vector = {}" or
 * "matrix = {{}}").
 * - If the type specified in the TYPE CAST expression is invalid, the value
 * will be saved in the database as a C-string (default), otherwise the token
 * will be converted to the specified type and then saved in the database.
 * - The error messages printed on the \a err stream are in the format
 * "error description [line number]".
 */
class DLL_API ParserI {

public:

    /**
     * @brief Constructor which initializes the instance with all the items
     * involved into the parsing (input, output, and grammar configuration).
     * @param[in] stream is the stream to be read and parsed.
     * @param[out] err is a stream where parse error messages are written into.
     * @param[in] grammarIn contains the comments patterns, the separator and
     * terminal characters.
     * @post
     *   GetGrammar() == grammarIn
     */
    ParserI(const ParserData & constantsIn);

    /**
     * @brief Destructor.
     */
    virtual ~ParserI();

    /**
     * @brief Parses the stream in input and builds the configuration database
     * accordingly.
     * @return true if the stream in input is parsed correctly, false otherwise.
     * In case of failure, the error causing the failure is printed on the
     * \a err stream in input (if it is not NULL).
     * @brief debugLevel  0 = onlyErrors 1=    2=
     */
    ErrorManagement::ErrorType Parse(StreamI &stream,BufferedStreamI *	errorStream=NULL_PTR(BufferedStreamI *),uint32 debugLevel = 0);

private:

    /**
     * @brief Retrieves the next expected token identifiers to be
     * pushed on the stack.
     * @param[in] index is the production row.
     * @return the next expected tokens identifiers to be pushed
     * on the stack.
     */
    const uint32 &GetProduction(const uint32 index) const;

    /**
     * @brief Retrieves the index of the productions array.
     * @param[in] index is the expected token identifier.
     * @return the index of the productions array.
     */
    uint32 GetProductionRow(const uint32 index) const;

    /**
     * @brief Retrieves the next token identifier.
     * @param[in] index is the parse row.
     * @return the next token identifier.
     */
    uint32 GetParse(const uint32 index) const;

    /**
     * @brief Retrieves the index of the parse array.
     * @param[in] index is the expected token identifier.
     * @return the index of the parse array.
     */
    uint32 GetParseRow(const uint32 index) const;

    /**
     * @brief Retrieves the conflict number.
     * @param[in] index is the conflict row.
     * @return the conflict number.
     */
    uint32 GetConflict(const uint32 index) const;

    /**
     * @brief Retrieves the conflict array index.
     * @param[in] index is the expected token identifier.
     */
    uint32 GetConflictRow(const uint32 index) const;

    /**
     * @brief Retrieves the name associated to the token.
     * @param[in] symbol is the expected token identifier.
     */
    CCString GetSymbolName(const uint32 symbol) const;

    /**
     * @brief Retrieves the name associated to the token.
     * @param[in] symbol is the expected token identifier.
     */
    CCString GetProductionName(const uint32 production) const ;

    /**
     * @brief Retrieves the name associated to the token.
     * @param[in] symbol is the expected token identifier.
     */
    CCString GetProductionNameWithConflicts(const uint32 production) const ;

    /**
     * @brief Pushes the expected token to the stack.
     * @param[in] symbol is the expected token.
     * @param[in] stack is the internal token stack.
     * @param[in, out] top is a pointer to the stack head.
     */
    inline void StackPush(const uint32 symbol,
                          const uint32 * const stack,
                          uint32* &top) const;

    /**
     * @brief Pops the expected token from the internal stack.
     * @param[out] top is a pointer to the stack head.
     */
    inline uint32 StackPop(uint32* &top) const;

    /**
     * @brief Consumes one token.
     * @return outOfRange  if there are no more tokens.
     */
	ErrorManagement::ErrorType NextToken();

protected:


	/**
	 *
	 */
	class  Token{
	public:// within the protected scope...
	    /**
	     * @brief Retrieves the token identifier.
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

	    /**
	     * @brief Retrieves the token line number.
	     * @return the token line number.
	     */
	    inline uint32 GetLineNumber() const;

		/**
		 * the matched rule
		 */
		const RegularExpression::PatternInformation *	matchedRule;

		/**
		 * the text matching the rule
		 */
		DynamicCString 									matchedText;
	};

    /**
     * @brief Peeks in the token stack produced by the lexical analyzer,
     * retrieves the identifier of the token in the next \a position index.
     * Note that the last token is at position 0
     * @return outOfRange  if there are no more tokens.
     */
    ErrorManagement::ErrorType PeekToken(StreamI &stream,const uint32 position, const Token *&token);

    /**
     * @brief Executes the specified function.
     * @param[in] number if the number of the callback to be executed.
     */
    virtual ErrorManagement::ErrorType Execute(const uint32 functionIndex,const ParserI::Token *currentToken,BufferedStreamI *errorStream)=0;

private:

    /**  CONSTANTS set by the PARSER generator*/
    /** to be correctly initalised by the initialiser of the specific specialisation */
    const ParserData &		constants;

    /**
     *
     */
    StaticStack<uint32>     symbolStack;

    /**
     * queue of tokens
     */
    List<Token>				tokenQueue;
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


void ParserI::StackPush(const uint32 symbol,
                        const uint32 * const stack,
                        uint32 *&top) const {
    /*lint -e{946} [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17]. Justification: stack implementation requires operational applied to pointer. */
    if (top > stack) {
        /*lint -e{165} , [MISRA C++ Rule 5-0-18]. Justification: stack implementation requires subtraction applied to pointer. */
        top--;
        *top = symbol;
    }
}

uint32 ParserI::StackPop(uint32 * &top) const {
    uint32 ret = 0u;
    if (*top != 0u) {
        ret = *top;
        top++;
    }
    return ret;
}

uint32 ParserI::Token::GetId() const{
	uint32 id = 0;
	if (matchedRule != NULL){
		id = matchedRule->ruleId;
	}
	return id;
}

CCString ParserI::Token::GetDescription() const {
	CCString description;
	if (matchedRule != NULL){
		description = matchedRule->ruleName;
	}
	return description;
}

CCString ParserI::Token::GetData() const {
	CCString data;
	data = matchedText;
	return data;
}

uint32 ParserI::Token::GetLineNumber() const{
	return 0;  // TODO
}



}

#endif /* PARSERI_H_ */

