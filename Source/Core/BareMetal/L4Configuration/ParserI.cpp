/**
 * @file ParserI.cpp
 * @brief Source file for class ParserI
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

 * @details This source file contains the definition of all the methods for
 * the class ParserI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ParserI.h"
#include "CompositeErrorManagement.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


ParserI::ParserI(const MARTe::ParserData & constantsIn):
		constants(constantsIn){
}


ParserI::~ParserI() {
}

/*
uint32 ParserI::GetNextTokenType() {
    return TokenToCode(tokenProducer.GetToken());

}

uint32 ParserI::PeekNextTokenType(const uint32 position) {

    Token* tok = tokenProducer.PeekToken(position);

    return TokenToCode(tok);
}
*/


uint32 ParserI::TokenToCode(const Token *token){
    uint32 ret = 0;

    if (token != NULL_PTR(Token *)){
        CCString toCompare;

    	if (token->GetId() == TERMINAL_TOKEN) {
            toCompare = token->GetData();
        }
        else {
            toCompare = token->GetDescription();
        }
        for (uint32 i = 1u; (i < constants.StartSymbol) && (ret == 0); i++) {
            if (toCompare == constants.Terminal_name [i]) {
                ret = i;
            }
        }
    }
	return ret;
}


CCString ParserI::GetSymbolName(const uint32 symbol) const
{
	CCString symbolName("not a symbol");

	if ( symbol >= constants.StartAction  &&  symbol < constants.EndAction ) {
		symbolName = constants.Action_name [symbol-(constants.StartAction-1)];
	} else
	if ( symbol >= constants.StartSymbol  ) {
		symbolName = constants.Nonterminal_name [symbol-(constants.StartSymbol-1)];
	} else
	if ( symbol > 0   ) {
		symbolName = constants.Terminal_name [symbol];
	}
	return symbolName;
}


CCString ParserI::GetProductionName(const uint32 production) const{
	CCString ret = "???";
	if (production < constants.Production_name.GetNumberOfElements()){
		ret = constants.Production_name[production];
	} else {
		uint32 action = production - (constants.StartAction-1);
		if (action < constants.Action_name.GetNumberOfElements()){
			ret = constants.Action_name[action];
		}
	}
	return ret;
}


CCString ParserI::GetProductionNameWithConflicts(const uint32 production) const{
	CCString ret = "conflict";
	if (production < constants.Production_name.GetNumberOfElements()){
		ret = constants.Production_name[production];
	}
	return ret;
}


const uint32 &ParserI::GetProduction(const uint32 index)const  {
    return constants.Production[index];
}


uint32 ParserI::GetProductionRow(const uint32 index)const  {
    return constants.Production_row[index];
}


uint32 ParserI::GetParse(const uint32 index)const  {
    return constants.Parse[index];
}


uint32 ParserI::GetParseRow(const uint32 index)const  {
    return constants.Parse_row[index];
}


uint32 ParserI::GetConflict(const uint32 index)const  {
    return constants.Conflict[index];
}


uint32 ParserI::GetConflictRow(const uint32 index)const  {
    return constants.Conflict_row[index];
}



ErrorManagement::ErrorType ParserI::Parse(StreamI &stream,BufferedStreamI *	errorStream,uint32 debugLevel) {
	ErrorManagement::ErrorType ret;
    Token *currentToken = static_cast<Token*>(NULL);
    /**
     * The lexical analyzer reading the stream and providing the tokens.
     */
    LexicalAnalyzer tokenProducer(stream, constants.terminals, constants.separators, constants.beginOneLineComment,constants.beginMultipleLinesComment,constants.endMultipleLinesComment);

    bool isEOF = false;
    while ((ret) && (!isEOF)) {

        uint32 stackArray[constants.StackSize];
        uint32 *stack = &stackArray[0];

        uint32 *top = &stackArray[constants.StackSize - 1u];
        *top = 0u;
        uint32 start_symbol = constants.StartSymbol;

        StackPush(start_symbol, stack, top);

        PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Push %s (=%i)\n",GetSymbolName(start_symbol).GetList(),start_symbol);

		currentToken = tokenProducer.GetToken();
		uint32 token = TokenToCode(currentToken);
//        uint32 token = GetNextTokenType();
        uint32 new_token = token;
        PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Token= [%s](%s) \n",GetSymbolName(token).GetList(),currentToken->GetData().GetList());

		uint32 symbol = StackPop(top);
		PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol).GetList(),symbol,top);
        for (; (symbol > 0u) && (ret);) {

        	/// from StartAction to EndAction
            if (symbol >= constants.StartAction) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Execute %s \n",GetSymbolName(symbol).GetList());
                ret = Execute(symbol - (constants.StartAction - 1u),currentToken,errorStream);
            } /// from StartSymbol to EndSymbol

            else

            if (symbol >= constants.StartSymbol) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Rule %s \n",GetSymbolName(symbol).GetList());

				// used by Peek - depth of forward peeking
                uint32 level = 0u; // it is 1 in original code
                uint32 entry = 0u; // now initialised here

#if 0   // not generated
                /// CONDITIONAL PRODUCTION & PREDICTED ENTRY
                uint32 production_number = 0;
                production_number = GET_CONDITIONAL_PRODUCTION ( symbol );
                if ( production_number ) {
                    entry = get_predicted_entry ( tokens,
                                                  production_number, token,
                                                  level, 1 );
                }
#endif

                if ( entry == 0 ) {
					uint32 index = 0;
                	index = GetParseRow(symbol - (constants.StartSymbol - 1u));
                    index += token;
                    entry = GetParse(index);
                    PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Parse: [%s] + [%s] = [%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList(),GetProductionNameWithConflicts(entry).GetList());
                }  // entry = 0

                // the next entry is a conflict.
                // move ahead in the conflict tables
                while (entry >= constants.StartConflict) {
                	uint32 oldEntry = entry;
                    uint32 index = GetConflictRow(entry - (constants.StartConflict - 1u));
                    Token* tok = tokenProducer.PeekToken(level);
                    uint32 token = TokenToCode(tok);
//                    uint32 token = PeekNextTokenType(level);
                    PARSER_DIAGNOSTIC_REPORT(errorStream,2,"PeekToken(%i)= [%s] \n",level,GetSymbolName(token).GetList());
                    index += token;
                    entry = GetConflict(index);
                    PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Conflict: [%s] + [%s] = [%s] \n",GetProductionNameWithConflicts(oldEntry).GetList(),GetSymbolName(token).GetList(),GetProductionNameWithConflicts(entry).GetList());
                    ++level;
                } // while

                if (entry != 0u) {
                	const uint32 *production = &GetProduction(GetProductionRow(entry));
                    uint32 production_length = *production - 1u;
                    production++;
                    PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Try produce [%s]\n",GetProductionName(entry).GetList());

                    /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    if (*production == symbol) {
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Match %s\n",GetSymbolName(symbol).GetList());

                    	//action->predict(entry);  // predictive production for custom appications

                    	production += production_length;

                    	/*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        for (; production_length > 0u; production_length--) {
                            /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                            uint32 toPush = *production;
                            StackPush(toPush, stack, top);
                            PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Push %s (=%i)\n",GetSymbolName(toPush).GetList(),toPush);
                            production--;
                        } // for
                    } // production = symbol

                    else

                    {
                    	// new_token = no_entry ( error, symbol, token, level-1 );
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Unmatch symbol=[%s]  token= [%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
//                        GetNextTokenType();  // consume and throw away
                		currentToken = tokenProducer.GetToken();

                    } // production != symbol
                } // entry != 0

                else

                { // entry = 0
                	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"No-entry symbol=[%s]  token=[%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
//					GetNextTokenType();  // consume
            		currentToken = tokenProducer.GetToken(); // consume and throw away
                } // entry = 0
    	    } // symbol >= constants.StartSymbol

            else

            {
            	if (symbol > 0u) {
                    if (symbol == token) {
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Symbol %2 matches token \n",GetSymbolName(token).GetList());
                    	//token = GetNextTokenType();
                		currentToken = tokenProducer.GetToken();
                		token = TokenToCode(currentToken);
                		PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Token= [%s](%s) \n",GetSymbolName(token).GetList(),currentToken->GetData().GetList());
                        new_token = token;
                    }
                    else
                    {
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Expecting '%s' but found '%s' \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
						new_token = token;
                    }
                }
            }

            if (token != new_token) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Token != new_token(%s) \n",GetSymbolName(new_token).GetList());
                if (new_token > 0u) {
                    token = new_token;
                }

                if (token != constants.EndOfFile) {
                    continue;
                }
            }
            symbol = StackPop(top);
            PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol).GetList(),symbol,top);
        }

        if (token != constants.EndOfFile) {
            ret.internalSetupError = true;
            PARSER_ERROR_REPORT(errorStream,ret,"leftover token %s in stack",GetSymbolName(symbol).GetList());
//            PrintErrorOnStream("\nEOF found with tokens on internal parser stack! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        }

        PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Looping %s\n","around");

    } // for

    return ret;
}

}
