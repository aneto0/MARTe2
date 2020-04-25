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
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {



static void PrintErrorOnStream(CCString const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }

//TODO FIX THIS
//        REPORT_ERROR(ErrorManagement::FatalError, format, lineNumber);
    }
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


ParserI::ParserI(StreamI &stream,BufferedStreamI * const err,const MARTe::ParserData & constantsIn):
		constants(constantsIn),
        tokenProducer(stream, constantsIn.terminals, constantsIn.separators, constantsIn.beginOneLineComment,constantsIn.beginMultipleLinesComment,constantsIn.endMultipleLinesComment){

	ok = ErrorManagement::NoError;

    errorStream = err;
    currentToken = static_cast<Token*>(NULL);
}

ParserI::~ParserI() {
    currentToken = static_cast<Token*>(NULL);
    errorStream=static_cast<BufferedStreamI*>(NULL);
}

uint32 ParserI::GetNextTokenType() {
//    uint32 ret = 0u;

    currentToken = tokenProducer.GetToken();

    return TokenToCode(currentToken);
/*
    CCString toCompare;

    // if it is a terminal use the data
    if (currentToken->GetId() == TERMINAL_TOKEN) {
        toCompare = currentToken->GetData();
    }
    // otherwise use the description
    else {
        toCompare = currentToken->GetDescription();
    }
    // return the slk token number
    for (uint32 i = 0u; i < constants.StartSymbol; i++) {
        if (toCompare == GetSymbolName(i)) {
            ret = i;
        }
    }
    return ret;
*/
}

uint32 ParserI::PeekNextTokenType(const uint32 position) {
//    uint32 ret = 0u;

    Token* tok = tokenProducer.PeekToken(position);

    return TokenToCode(tok);
/*
    CCString toCompare = static_cast<const char8 *>(NULL);

    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    }
    else {
        toCompare = tok->GetDescription();
    }
    for (uint32 i = 0u; i < constants.StartSymbol; i++) {
        if (toCompare == GetSymbolName(i)) {
            ret = i;
        }
    }

    return ret;
*/
}

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


ErrorManagement::ErrorType ParserI::Parse() {
	ok = ErrorManagement::NoError;

    bool isEOF = false;
    while ((ok) && (!isEOF)) {

        uint32 stackArray[constants.StackSize];
        uint32 *stack = &stackArray[0];

        uint32 *top = &stackArray[constants.StackSize - 1u];
        *top = 0u;
        uint32 start_symbol = constants.StartSymbol;

        StackPush(start_symbol, stack, top);
printf("Push %s (=%i)\n",GetSymbolName(start_symbol).GetList(),start_symbol); // TODO

        uint32 token = GetNextTokenType();
        uint32 new_token = token;
printf("Token= [%s](%s) \n",GetSymbolName(token).GetList(),currentToken->GetData().GetList()); // TODO

		uint32 symbol = StackPop(top);
printf("Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol).GetList(),symbol,top); // TODO
        for (; (symbol > 0u) && (ok);) {

        	/// from StartAction to EndAction
            if (symbol >= constants.StartAction) {
printf("Execute %s \n",GetSymbolName(symbol).GetList()); // TODO
                Execute(symbol - (constants.StartAction - 1u),currentToken,errorStream);
            } /// from StartSymbol to EndSymbol
            else
            if (symbol >= constants.StartSymbol) {
printf("Rule %s \n",GetSymbolName(symbol).GetList()); // TODO

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
printf("Parse: [%s] + [%s] = [%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList(),GetProductionNameWithConflicts(entry).GetList()); // TODO
                }  // entry = 0


                // the next entry is a conflict.
                // move ahead in the conflict tables
                while (entry >= constants.StartConflict) {
                	uint32 index = 0;
                	uint32 oldEntry = entry;
                    index = GetConflictRow(entry - (constants.StartConflict - 1u));
                    uint32 token = PeekNextTokenType(level);
printf("PeekToken(%i)= [%s] \n",level,GetSymbolName(token).GetList()); // TODO
                    index += token;
                    entry = GetConflict(index);
printf("Conflict: [%s] + [%s] = [%s] \n",GetProductionNameWithConflicts(oldEntry).GetList(),GetSymbolName(token).GetList(),GetProductionNameWithConflicts(entry).GetList()); // TODO
                    ++level;
                } // while

                if (entry != 0u) {
                	const uint32 *production = &GetProduction(GetProductionRow(entry));
                    uint32 production_length = *production - 1u;
                    production++;
printf("Try produce [%s]\n",GetProductionName(entry).GetList()); // TODO

                    /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    if (*production == symbol) {
printf("Match %s\n",GetSymbolName(symbol).GetList()); // TODO

                    	//action->predict(entry);  // predictive production for custom appications

                    	production += production_length;

                    	/*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        for (; production_length > 0u; production_length--) {
                            /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                            uint32 toPush = *production;
                            StackPush(toPush, stack, top);
printf("Push %s (=%i)\n",GetSymbolName(toPush).GetList(),toPush); // TODO
                            production--;
                        } // for
                    } // production = symbol
                    else {
                    	// new_token = no_entry ( error, symbol, token, level-1 );
printf ( "Unmatch symbol=[%s]  token= [%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
                        GetNextTokenType();  // consume

                    } // production != symbol
                } // entry != 0
                else
                { // entry = 0
printf ( "No-entry symbol=[%s]  token=[%s] \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
					GetNextTokenType();  // consume
                } // entry = 0
    	    } // symbol >= constants.StartSymbol
            else {
            	if (symbol > 0u) {
                    if (symbol == token) {
printf ("Symbol matches token \n");
                    	token = GetNextTokenType();
printf("Token= [%s](%s) \n",GetSymbolName(token).GetList(),currentToken->GetData().GetList()); // TODO
                        new_token = token;
                    }
                    else {
printf ( "Expecting '%s' but found '%s' \n",GetSymbolName(symbol).GetList(),GetSymbolName(token).GetList());
						new_token = token;
                    }
                }
            }

            if (token != new_token) {
printf ( "Token != new_token \n");
                if (new_token > 0u) {
                    token = new_token;
                }
                if (token != constants.EndOfFile) {
                    continue;
                }
            }
            symbol = StackPop(top);
printf("Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol).GetList(),symbol,top); // TODO
        }

        if (token != constants.EndOfFile) {
            PrintErrorOnStream("\nEOF found with tokens on internal parser stack! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            ok.internalSetupError = true;
        }

printf("Loop\n"); // TODO

    } // for

    return ok;
}

}
