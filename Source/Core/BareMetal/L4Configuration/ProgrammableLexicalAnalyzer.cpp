/**
 * @file LexicalAnalyzer2.h
 * @brief Header file for class AnyType
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

#include "DynamicCString.h"
#include "ProgrammableLexicalAnalyzer.h"

namespace MARTe{


ErrorManagement::ErrorType ProgrammableLexicalAnalyzer::GetToken(const Token *&token) {
	ErrorManagement::ErrorType ret;

	if (tokenQueue.GetSize() > 0){
		ret.fatalError = !tokenQueue.Remove(0);
		REPORT_ERROR(ret, "Error renoving token");
	}

	if (ret){
		ret = PeekToken(0,token);
	}

    return ret;
}

ErrorManagement::ErrorType ProgrammableLexicalAnalyzer::PeekToken(const uint32 position,const Token *&token){
	ErrorManagement::ErrorType ret;

	while ((tokenQueue.GetSize() <= position) && ret){
		const RegularExpression::PatternInformation *selectedRule = NULL;
		DynamicCString matched;
		ret = RegularExpression::MatchRules(stream,ruleSet,selectedRule,matched);
		REPORT_ERROR(ret, "Error scanning input file for tokens");
		if (ret){
			Token token;
			token.matchedRule = selectedRule;
			token.matchedString = matched;
			ret.fatalError = !tokenQueue.Add(token);
			REPORT_ERROR(ret, "Error adding token");
		}
	}

	if (ret && (tokenQueue.GetSize() > position)){
		token = &tokenQueue.Access(position);
	}

	return ret;
}



} //MARTe
