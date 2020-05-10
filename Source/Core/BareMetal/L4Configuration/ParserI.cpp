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
#include "ProgressiveTypeCreator.h"
#include "StreamString.h"
#include "RegularExpression.h"

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




ErrorManagement::ErrorType ParserI::NextToken() {
	class TokenRemover: public GenericIterator<ParserI::Token>{
	public:
		inline TokenRemover(){
			isFirst = true;
		}
	    virtual IteratorAction Do(ParserI::Token &data,uint32 depth){
	    	IteratorAction ia;
	    	if (isFirst) {
//printf("removing %s\n",data.GetDescription().GetList());
	    		ia.SetActionCode(deleteNode);
	    		isFirst = false;
	    	} else {
	    		ia.SetActionCode(isCompleted);
	    	}

	    	return ia;
	    }
	private:
	    /**
	     * to know if this is the first
	     */
	    bool isFirst;
	};

	ErrorManagement::ErrorType ret;
	TokenRemover remover;

//printf("queue size = %i-->",tokenQueue.Size());

	if (tokenQueue.Size() > 0){
		ret.fatalError = !tokenQueue.Iterate(remover);
		REPORT_ERROR(ret, "Error removing token");
	}
//printf("%i\n",tokenQueue.Size());
	return ret;
}

ErrorManagement::ErrorType ParserI::PeekToken(StreamI &stream,const uint32 position, const Token *&token) {
	ErrorManagement::ErrorType ret;

	while ((tokenQueue.Size() <= position) && ret){
		Token token;
		ret = RegularExpression::MatchRules(stream,constants.ruleSet,token.matchedRule,token.matchedText);

		REPORT_ERROR(ret, "Error scanning input file for tokens");
		if (ret && !token.matchedRule->skip){
//printf("inserting %s\n",token.GetDescription().GetList());
			ret.fatalError = !tokenQueue.Insert(token,tokenQueue.Size());
			REPORT_ERROR(ret, "Error adding token");
		}
	}

	if (ret && (tokenQueue.Size() > position)){
		token = tokenQueue[position];
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
	const Token * currentToken;

//    bool isEOF = false;
//    while ((ret) && (!isEOF)) {

        uint32 stackArray[constants.StackSize];
        uint32 *stack = &stackArray[0];

        uint32 *top = &stackArray[constants.StackSize - 1u];
        *top = 0u;
        uint32 start_symbol = constants.StartSymbol;

        StackPush(start_symbol, stack, top);

        PARSER_DIAGNOSTIC_REPORT(errorStream,3,"Push %s (=%i)\n",GetSymbolName(start_symbol),start_symbol);

        if (ret){
        	ret = PeekToken(stream,0,currentToken);
            PARSER_ERROR_REPORT(errorStream,ret,"Failed reading token",0);
        }

        uint32 token = 0;
        if (currentToken != NULL){
            token = currentToken->GetId();
        }
        uint32 new_token = token;
		uint32 symbol = StackPop(top);

		PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Token= [%s](%s) \n",GetSymbolName(token),currentToken->GetData());
		PARSER_DIAGNOSTIC_REPORT(errorStream,3,"Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol),symbol,top);
        for (; (symbol > 0u) && (ret);) {

        	/// from StartAction to EndAction
            if (symbol >= constants.StartAction) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Execute %s \n",GetSymbolName(symbol));
                ret = Execute(symbol - (constants.StartAction - 1u),currentToken,errorStream);
                PARSER_ERROR_REPORT(errorStream,ret,"%s() failed",GetSymbolName(symbol));
            } /// from StartSymbol to EndSymbol

            else

            if (symbol >= constants.StartSymbol) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Rule %s \n",GetSymbolName(symbol));

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
                    PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Parse: [%s] + [%s] = [%s] \n",GetSymbolName(symbol),GetSymbolName(token),GetProductionNameWithConflicts(entry));
                }  // entry = 0

                // the next entry is a conflict.
                // move ahead in the conflict tables
                while ((entry >= constants.StartConflict)&&ret) {
                	uint32 oldEntry = entry;
                    uint32 index = GetConflictRow(entry - (constants.StartConflict - 1u));
                    uint32 token = 0;

                    const Token * temporaryToken;
                    ret = PeekToken(stream,level+1,temporaryToken);
                    PARSER_ERROR_REPORT(errorStream,ret,"Failed reading token \n",0);
                    if (temporaryToken){
                        token = temporaryToken->GetId();
                    }
                    index += token;
                    entry = GetConflict(index);
                    ++level;

                    PARSER_DIAGNOSTIC_REPORT(errorStream,2,"PeekToken(%i)= [%s] \n",level,GetSymbolName(token));
                    PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Conflict: [%s] + [%s] = [%s] \n",GetProductionNameWithConflicts(oldEntry),GetSymbolName(token),GetProductionNameWithConflicts(entry));
                } // while

                if (entry != 0u) {
                	const uint32 *production = &GetProduction(GetProductionRow(entry));
                    uint32 production_length = *production - 1u;
                    production++;
                    PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Try produce [%s]\n",GetProductionName(entry));

                    /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    if (*production == symbol) {
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Match %s\n",GetSymbolName(symbol));

                    	//action->predict(entry);  // predictive production for custom applications

                    	production += production_length;

                    	/*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        for (; production_length > 0u; production_length--) {
                            /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                            uint32 toPush = *production;
                            StackPush(toPush, stack, top);
                            PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Push %s (=%i)\n",GetSymbolName(toPush),toPush);
                            production--;
                        } // for
                    } // production = symbol

                    else

                    {
                    	// consume token
                    	NextToken();
                    	currentToken = NULL;
                    	ret.syntaxError = true;
                    	// new_token = no_entry ( error, symbol, token, level-1 );
                    	PARSER_ERROR_REPORT(errorStream,ret,"No-entry symbol=[%s]  token= [%s] \n",GetSymbolName(symbol),GetSymbolName(token));
                    	new_token = 0;
                    } // production != symbol
                } // entry != 0

                else

                { // entry = 0

                	NextToken();
                	currentToken = NULL;

                	ret.syntaxError = true;
                	PARSER_ERROR_REPORT(errorStream,ret,"No-entry symbol=[%s]  token=[%s] \n",GetSymbolName(symbol),GetSymbolName(token));
                	new_token = 0;

                } // entry = 0
    	    } // symbol >= constants.StartSymbol

            else

            {
            	if (symbol > 0u) {
                    if (symbol == token) {
                    	PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Symbol %2 matches token \n",GetSymbolName(token));

                        ret = NextToken();
                        PARSER_ERROR_REPORT(errorStream,ret,"Failed advancing on token Q\n",0);
                        currentToken = NULL;

                        if (ret){
                        	ret = PeekToken(stream,0,currentToken);
                            PARSER_ERROR_REPORT(errorStream,ret,"Failed reading token \n",0);
                        }

                        if (currentToken != NULL){
                            token = currentToken->GetId();
                    		PARSER_DIAGNOSTIC_REPORT(errorStream,2,"Token= [%s](%s) \n",GetSymbolName(token),currentToken->GetData());
                        }
                        new_token = token;

                    }
                    else
                    {
						new_token = 0;

						ret.syntaxError = true;
	                	PARSER_ERROR_REPORT(errorStream,ret,"Mismatch symbol=[%s]  token=[%s] \n",GetSymbolName(symbol),GetSymbolName(token));
                    }
                }
            }

            if (token != new_token) {
            	PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Token != new_token(%s) \n",GetSymbolName(new_token));
                if (new_token > 0u) {
                    token = new_token;
                }

                if (token != constants.EndOfFile) {
                    continue;
                }
            }

//            // exit
//          isEOF = (token == constants.EndOfFile) ;

            symbol = StackPop(top);
            PARSER_DIAGNOSTIC_REPORT(errorStream,3,"Pop %s (=%i) (top = %p)\n",GetSymbolName(symbol),symbol,top);
        }

        if (token != constants.EndOfFile) {
            ret.internalSetupError = true;
            PARSER_ERROR_REPORT(errorStream,ret,"leftover token %s in stack",GetSymbolName(symbol));
        }

//        PARSER_DIAGNOSTIC_REPORT(errorStream,4,"Looping %s\n","around");

//    } // for

    PARSER_ERROR_REPORT(errorStream,ret,"Failed",0);
    return ret;
}

}
