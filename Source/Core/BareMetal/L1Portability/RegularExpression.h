/**
 * @file RegularExpression.h
 * @brief Header file for class AnyType
 * @date May 2, 2020
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

#ifndef REGULAREXPRESSION_H_
#define REGULAREXPRESSION_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "StreamI.h"
#include "CStringTool.h"
#include "DynamicCString.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace RegularExpression{

/**
 * @brief used in MatchRules
 */
struct PatternInformation{
	/**
	 *  @see RegularExpression::Match
	 */
	CCString  			pattern;

	/**
	 * @brief name of the rule
	 */
	CCString 			ruleName;

	/**
	 * @brief id of the rule
	 */
	uint32 				ruleId;

	/**
	 * @brief is this a separator to be skipped
	 */
	bool 				skip;
};

/**
 * to be used to terminate a Zero Terminated Array of rules
 */
static const PatternInformation emptyPattern = {emptyString,emptyString, 0,false};


/**
 * @brief callback used by Scan
 *
 */
typedef void (*PatternMatchCallBack)(CCString name, uint32 nameLength, CCString value, uint32 valueLength);


/**
 * @brief  This is a regular expression evaluator with a syntax related to that of PERL
 *         Changes are introduced to allow rapid direct matching
 * @details The Match function compares the data in the input with that of the pattern
 * 		   It will match 1 by 1 characters if are the same as in a standard comparison
 * 		   But some characters sequences enable introduction of regular expressions
 * 		   The following characters are in general reserved in the pattern string : ()[]{}!^*+-\?
 * 		   Inside the []  only reserved characters are ]^-\
 * 		   Inside the () or main sequence  only reserved characters are ()[]{}!*+\?
 * 		   In order to simply match one of these, the pattern must use a sequence \x where x is one of the above
 * 		   Example: \\ is the \ and \+ is the +.
 * 		   Note that when initialising a string in C additional escape characters must be used
 * 		   Example \\ becomes "\\\\" as in C \ is also a reserved character. so to match a \ one need to write 4 times \
 * 		   Let's indicate with <x> a matching atom.
 * 		   A matching atom can be a character, a block: (), a range: []
 * 		   A block is a sequence ( <A><B><C>... )
 * 		   A range is a sequence [ <char><char>.... <char>-<char> ... ^ <char><char> ...]
 * 		   A range is matched if either of its containing characters is matched
 * 		   Instead of specifying all possible characters, one can introduce a range <char> - <char>
 * 		   Or one can negate a set of characters: ^<char><char> or ^<char>-<char>
 * 		   Unlike PERL, negation ^ can be introduced multiple times. Each time the logic is reversed
 * 		   The [] try matching each character or range in order
 * 		   Special escapes substitute common ranges /d (all digits),  /w (all letters) and /a (all) (NB replaces .)
 * 		   It will finish when a match is found and will report a match or not match
 * 		   depending on the current status of the logic .
 * 		   A pattern containing a sequence  <A><B><C> is matched if all the atoms are matched in the correct order
 * 		   A pattern containing a sequence <A><B>|<C><D> is matched if either the first 2 or the second 2 atoms are matched
 * 		   A pattern containing a sequence <A>{n,m}<B><C> is matched if <A> is matched and then a sequence between n and m of <B> are matched followed by <C>
 * 		   Note that the {} associates to the right and not to the left as in  PERL
 * 		   Note that Match will match as many <B> as possible. This may not be desirable
 * 		   Note that Match, in case of failure of matching <C> will not backtrack and try with 1 less match of <B>
 * 		   A pattern containing a sequence <A>{n,m}?<B><C> is matched if <A> is matched and then a sequence between n and m of <B> are matched followed by <C>
 * 		   Note that in this case Match will match as few <B> as possible.
 * 		   The sequences {n,m} / {n,m}? can be shortened with * => {0,inf} + => {1,inf} ? => {0,1}
 * 		   Example <A>*?<B><C> means that first the sequence <A><C> will be tested,then <A><B><C> then <A><B><B><C>....
 * 		   A pattern containing a sequence <A><B>!<C><D> is matched if just <A><B> are matched, but is also not completed if <C><D> do not match
 *		   Note that this is a unique feature of this regexp. It allows indicating to the calling program that once
 *		   the <A><B> sequence is matched then not matching the following <C><D> is considered
 *		   an error and is not worth checking for any other match.
 * 		   A pattern containing a sequence <A>!<B>|<C><D> is matched if <A><B> or <C><D> are matched,
 * 		   but is also not matched <A> is matched and <B> is not
 * 		   The grammar allows also to name an atom using the syntax $text(<atom>)
 * 		   text is a sequence of any char terminated by the (
 * 		   *,+,?,{} cannot be used in front of a ?, but you can use *($text(<atom>))
 *
 * @param[in] pattern is a string containing the pattern to match. see function details.
 * @param[in,out] input is the stream/string to be parsed to check the match with the pattern.
 * 			If the pattern is matched all necessary characters are consumed from the stream/string
 * 			If the pattern is not matched, or in case of errors, the stream/string position is returned
 * 			to as before the call
 * @param[out] matched is were the matched string will be appended on success
 * @return  SyntaxError in case of a malformed expression
 *          OutOfRange  in case of failure in reading more characters
 *          ComparisonFailure if the pattern did not match
 *          NotCompleted if the pattern matched but was not completed
 *          not error on match
 */
ErrorManagement::ErrorType Match(StreamI &input,CCString &pattern,CStringTool &matched);

/**
 * @see Match(StreamI &input,CCString &pattern);
 */
ErrorManagement::ErrorType Match(CCString &input,CCString &pattern,CStringTool &matched);

/**
 * @brief Try matching one by one the rules in ruleSet until success or error.
 * @see Match(StreamI &input,CCString &pattern);
 * @param[in] ruleSet is a zero terminated array of PatterNInformation.
 * @param[out] selectedRule, upon success contains the rule that was matched.
 */
ErrorManagement::ErrorType MatchRules(StreamI &input,const ZeroTerminatedArray<const PatternInformation> ruleSet,const PatternInformation *&selectedRule,DynamicCString &matched);

/**
 * @see MatchRules(StreamI &input,const ZeroTerminatedArray<const PatternInformation> ruleSet,const PatternInformation *&selectedRule,DynamicCString &matched);
 */
ErrorManagement::ErrorType MatchRules(CCString &input,const ZeroTerminatedArray<const PatternInformation> ruleSet,const PatternInformation *&selectedRule,DynamicCString &matched);

/**
 * @brief allow extracting matched named atoms from a stream
 *        everytime one is matched the callback is called
 * @see Match(StreamI &input,CCString &pattern);
 * @param[in] pattern needs to have some named atoms
 */
ErrorManagement::ErrorType Scan(CCString &input,CCString &pattern,PatternMatchCallBack callBack);

/**
 * @see Scan(CCString &input,CCString &pattern,PatternMatchCallBack callBack)
 */
ErrorManagement::ErrorType Scan(StreamI &input,CCString &pattern,PatternMatchCallBack callBack);


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
} // Regex
} // MARTe

#endif /* REGULAREXPRESSION_H_ */
