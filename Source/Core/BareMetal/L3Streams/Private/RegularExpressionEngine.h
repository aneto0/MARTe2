/**
 * @file RegularExpressionEngine.h
 * @brief Header file for namespace RegExp
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

#ifndef REGULAREXPRESSIONENGINE_H_
#define REGULAREXPRESSIONENGINE_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace RegularExpression{


/**
 *  used to encode escape sequences
 */
static const int16 extendedCode = 512;

/**
 *
 */
static const int16 endOfPattern = 1024 ;

/**
 * indicates the range [0-9]
 */
static const int16 digitRange   = extendedCode  + 'd';


/**
 * 1dindicates the range [\0-\377]
 */
static const int16 allRange   = extendedCode  + 'a';

/**
 * \w indicates the range [A-Zaz]
 */
static const int16 wordRange   = extendedCode  + 'w';

/**
 * \e indicates the end of file
 */
static const int16 eof         = 256 ;

/**
 * returns logic if c is within the range [minAscii-maxAscii]
 * otherwise returns !logic
 */
static inline bool RangeCheck(char8 c, char8 minAscii, char8 maxAscii, bool logic );

/**
 *
 */
static inline int16 GetPatternElement(CCString &pattern);

// 0,1 reading start 2 reading end
enum RangeMatchStatus {
	PatternNotSet= 0,  // initial state - no character read from input
	PatternSet   = 1,  // start of the range set or unmatched individual element
	                   // also 1 character read from the input
	PatternRange = 2,  // ready for a range match
	Matched      = 3,  // range/individual match
	Completed    = 4,  // pattern fully consumed
	Error        = 5   // error somewhere
};

/**
 *
 */
static inline void HandleOpenRange(enum RangeMatchStatus &status,ErrorManagement::ErrorType &ret,int16 lastCharacter,char8 minAscii,bool logic);


/**
 * Produces comparisonFailure if pattern does not match
 * or 		outOfRange        if no more characters in input
 */
template <class T>
static inline ErrorManagement::ErrorType RangeMatch(T &input,CCString &pattern);

/**
 * Produces comparisonFailure if pattern does not match
 * or 		outOfRange        if no more characters in input
 * or       syntaxError       if a ) is encountered
 * or       notCompleted      if locked match failed
 */
template <class T>
static inline ErrorManagement::ErrorType MatchSingle(T &input,CCString &pattern);

/**
 * Produces comparisonFailure if pattern does not match
 * or 		outOfRange        if no more characters in input
 * or       syntaxError       if a ) is encountered
 * or       notCompleted      if locked match failed
 */
template <class T>
static inline ErrorManagement::ErrorType  PatternMatch(T &input,CCString &pattern);



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


bool RangeCheck(char8 c, char8 minAscii, char8 maxAscii, bool logic ){
	// chars are signed. get the unsigned interpretation
	uint32 uc =  reinterpret_cast<uint8 & >(c);
	uint32 umin =  reinterpret_cast<uint8 & >(minAscii);
	uint32 umax =  reinterpret_cast<uint8 & >(maxAscii);
	bool ret = !logic;
	if ((uc >= umin ) && (uc <= umax )){
		ret = logic;
	}
//printf("Checking %c(%i) in range [%i - %i]-> %i",c,uc,minAscii,maxAscii,ret);
	return ret;
}

int16 GetPatternElement(CCString &pattern){
	int16 patternElement = pattern[0];
	if (patternElement == '\\'){
		pattern++;
		patternElement = pattern[0]+extendedCode;
	}
	if (patternElement == 0) {
		patternElement = endOfPattern;
	} else if (patternElement == extendedCode){
		patternElement = eof;   // \ followed by \0
//printf("producing EOF matching token \n");
	} else {
		pattern++;
	}
	return 	patternElement;
}

static inline void HandleOpenRange(enum RangeMatchStatus &status,ErrorManagement::ErrorType &ret,int16 lastCharacter,char8 minAscii,bool logic){
	if (status == PatternRange){
		if (RangeCheck(lastCharacter,minAscii,255,true)){
			status = Matched;
	    	ret.comparisonFailure = !logic;
		}
	}
}

template <class T>
ErrorManagement::ErrorType RangeMatch(T &input,CCString &pattern){
	ErrorManagement::ErrorType ret;

//printf ("enter RAngeMatch(%s)\n",pattern.GetList());
    // positive or negative logic
    bool logic = true;

    // count open and closed square parentheses [ ]
    int parenCnt = 1;

    // store here the start of a range
    char8 minAscii = 0;

    // the status of the range matching algorithm
    enum RangeMatchStatus status = PatternNotSet;

    // the character to be matched
    int16 lastCharacter = 0;

    if (!input.GetC(lastCharacter)){
    	ret.fatalError = true;
		status = Error;
	}
    // check for NULL or empty
    if (pattern.GetSize() > 0){

        while (status < Completed) {

        	int16 patternElement = GetPatternElement(pattern);

//printf ("R(%i,%c:%i,%s)\n",status,patternElement,patternElement,pattern.GetList());fflush(stdout);

            switch(patternElement){
         		// end of pattern
        		// force exit from this matching
            	case endOfPattern:
            	case ']':{
                	// This is the end of the pattern so take a decision now!
                    if ((parenCnt <= 1) || (patternElement == endOfPattern)){
                    	// no match as yet and no open range ==> assign result as !logic
                        if (status < PatternRange) {
                        	ret.comparisonFailure = logic;
                        } else
                        // if open range n-] check against minAscii - 255
                        if (status == PatternRange){
                        	ret.comparisonFailure = !RangeCheck(lastCharacter,minAscii,255,logic);
                        }
                    	status = Completed;
                    } else {
                    	parenCnt--;
                    }
            	} break;
                case digitRange:
            	case wordRange:{
            		HandleOpenRange(status,ret,lastCharacter,minAscii,logic);
                	if (status == PatternNotSet){
                        minAscii = 0;
                		status = PatternSet;
                	}
                	if (status == PatternSet){
               			bool result = false;
               			if (patternElement == digitRange){
                    		result = RangeCheck(lastCharacter, '0', '9',true);
               			} else {
                    		result = RangeCheck(lastCharacter, 'A', 'Z',true) || RangeCheck(lastCharacter, 'a', 'z',true);
               			}
                		if (result){
                			status = Matched;
                		}
                	}
            	} break;

            	//  invert logic -> if matches now it is not matched!
            	case '^':{
            		// a sequence n-! indicates an open ended range n-255 and then a logic inversion
            		HandleOpenRange(status,ret,lastCharacter,minAscii,logic);


                	// process only if not match yet
                	if (status < Matched){
                        logic = !logic;
                        // restart sequence
                        if (status > PatternNotSet){
                        	status = PatternNotSet;
                        }
                        minAscii = 0;
                	}
                } break;
            	//  next character will be range end
                case '-':{
                	if (status == PatternSet){
                		status = PatternRange;
                	} else
                	// open ended range 0 - n
                	if (status == PatternNotSet){
                        minAscii = 0;
                    	status = PatternRange;
                    }
                } break;
                // any other character in the pattern
                default:{
                	if (status < Matched){
                        if (status == PatternNotSet){
                        	status = PatternSet;
                        }

                        char8 matchCharacter = static_cast<char8>(patternElement & 0xFF);

                		// if a range is not set
                		// set the start of a range
                		// set/change the individual match
                		// check compliance
                        if (status == PatternSet){
                            if (lastCharacter == matchCharacter) {
                            	status =  Matched;
                            	ret.comparisonFailure = !logic;
                            } else {
                                minAscii =  matchCharacter;
                            }
                        }
                        else
                        {
                        	HandleOpenRange(status,ret,lastCharacter,minAscii,logic);

                        	// if a range is set (the '-' character was just found)
                            // set the range extreme
                            // and check lastCharacter against the range
                            if (status == PatternRange){
                        		status = PatternNotSet;
                            }

                        }
                	}

                }
            }
        } // pattern size is not 0
    } // while loop

//printf ("exit RAngeMatch: %i \n",ret.ErrorsCleared());
    return ret;
}

static inline void GetBranchParameters(CCString &pattern,int &minMatches,int &maxMatches,bool &greedy,CCString &nextPattern){
    minMatches = 0;
    maxMatches = 1000000;
    greedy = true;

    if (pattern[0] == '*'){
        pattern++;
    } else
    if (pattern[0] == '+'){
        minMatches = 1;
        pattern++;
    } else
    if (pattern[0] == '?'){
        maxMatches = 1;
        pattern++;
    } else
    if (pattern[0] == '{'){
        pattern++;
        minMatches = atoi(pattern.GetList());
        char8 p = pattern[0];
        while((p!=0)&&(p!=',')&&(p!='}')){
        	pattern++;
            p = pattern[0];
        }
        if (p == ','){
            pattern++;
            maxMatches = atoi(pattern.GetList());
            while((p!=0)&&(p!='}')){
            	pattern++;
                p = pattern[0];
            }
        }
        if (p=='}') {
        	pattern++;
        }
    }
    if (pattern[0] == '?'){
    	greedy = false;
    	pattern++;
    }

    nextPattern = pattern;
    char p = nextPattern[0];
    switch(p){
        case 0: return;
        case '(':{
            int parenCnt = 1;
            bool escape = false;
            nextPattern++;
            p = nextPattern[0];
            while((p!=0)&&(parenCnt>0)){
                if (escape){
                    escape = false;
                } else
                if (p == '\\') {
                	escape = true;
                }
                else
                if (p == '(') {
                	parenCnt++;
                }
                else
                if (p == ')') {
                    parenCnt--;
                }
                nextPattern++;
                p = nextPattern[0];
            }
        }break;
        case '[':{
            int parenCnt = 1;
            bool escape = false;
            nextPattern++;
            p = nextPattern[0];
            while((p!=0)&&(parenCnt>0)){
                if (escape){
                    escape = false;
                } else
                if (p == '\\') escape = true;
                else
                if (p == '[') {
                	parenCnt++;
                }
                else
                if (p == ']') {
                    parenCnt--;
                }
                nextPattern++;
                p = nextPattern[0];
            }
        }break;
        case '\\':{
        	// consume the \ and the character following it
            nextPattern++;
            nextPattern++;
    	} break;
        default:{
            nextPattern++;
        }
    }
}

/** called by the *() */
template <class T>
ErrorManagement::ErrorType MatchSingle(T &input,CCString &pattern){
	ErrorManagement::ErrorType ret;

//printf ("enter MatchSingle(%s)\n",pattern.GetList());

    if (pattern.GetSize() != 0){

    	int16 patternElement = GetPatternElement(pattern);

        switch (patternElement){
            case '[':{
            	ret = RangeMatch(input,pattern);
            } break;
            case '(':{
            	ret = PatternMatch(input,pattern);
            } break;
            case endOfPattern:{
            	// considered an error after the *()
            	ret.syntaxError = true;
            } break;
            case allRange:{
            	int16 lastCharacter;
           		if (input.GetC(lastCharacter)){
           		} else {
            		ret.fatalError = true;
           		}
        	} break;
        	case wordRange:{
            	int16 lastCharacter;
           		if (input.GetC(lastCharacter)){
//printf("match \\w with %c",lastCharacter);
           			ret.comparisonFailure = !RangeCheck(lastCharacter, 'A', 'Z',true) && !RangeCheck(lastCharacter, 'a', 'z',true) ;
           		} else {
            		ret.fatalError = true;
           		}
        	} break;
        	case digitRange:{
            	int16 lastCharacter;
           		if (input.GetC(lastCharacter)){
           			ret.comparisonFailure = !RangeCheck(lastCharacter, '0', '9',true);
           		} else {
            		ret.fatalError = true;
           		}
        	} break;
            default:{
            	char8 matchCharacter = static_cast<char8>(patternElement && 0xFF);
            	int16 lastCharacter;
           		if (input.GetC(lastCharacter)){
           			ret.comparisonFailure = (lastCharacter != matchCharacter);
            	} else {
            		ret.fatalError = true;
            	}
            }
        }
    }
//printf ("exit MatchSingle%i \n",ret.ErrorsCleared());
    return ret;
}

static inline bool ScanVariableName(CCString &pattern){
	char8 c = pattern[0];
	pattern++;
	while((c != '\0') && (c != '(')){
		c = pattern[0];
		pattern++;
	}

	return (c == '(');
}

template <class T>
ErrorManagement::ErrorType PatternMatch(T &input,CCString &pattern){
	ErrorManagement::ErrorType ret;

//printf ("enter PatternMatch(%s)\n",pattern.GetList());  //TODO

    // count the round brackets after having given up()
    int parenCount = 0;

    enum {
		Matching     = 0,  // initial state: so far so good
		SectionFinish= 1,  // success and do not try other branches skip also |
		NotMatching  = 2,  // failed one step --> consume all the pattern, check for a possible branch |
		Matched      = 3,  // all done and successfully
		NotMatched   = 4,  // all done and not matched
		Error        = 5   // something went wrong
    } status = Matching;

    enum {
    	Normal       = 0,
    	Locked       = 1 // failed 	and must complete cannot try other branch
    } mode = Normal;

    typename T::Status globalInputStatus;
    input.SaveStatus(globalInputStatus);

    // if pattern is empty we have already finished
    // handles also NULL
    if (pattern.GetSize() == 0){
    	status = Matched;
    }

    // scan until maxCharacterMatch is 0 or ret becomes false
    while (status < Matched) {

    	int16 patternElement = GetPatternElement(pattern);

//printf ("M(%i,%i,%c:%i,%s)\n",status,mode,patternElement,patternElement,pattern.GetList());fflush(stdout);  //TODO

        switch (patternElement){
            case allRange:{
            	if (status == Matching){
                	int16 lastCharacter;
               		if (!input.GetC(lastCharacter)){
                		ret.fatalError = true;
                		status = Error;
               		}
            	}
        	} break;
            case digitRange:
        	case wordRange:{
            	int16 lastCharacter;
            	if (status == Matching){
               		if (!input.GetC(lastCharacter)){
                		ret.fatalError = true;
               			status = Error;
               		}
            	}
            	if (status == Matching){
           			bool result = false;
           			if (patternElement == digitRange){
                		result = RangeCheck(lastCharacter, '0', '9',true);
           			} else {
                		result = RangeCheck(lastCharacter, 'A', 'Z',true) || RangeCheck(lastCharacter, 'a', 'z',true);
           			}
            		if (!result){
        				status = NotMatching;
            		}
            	}
        	} break;
            case '[':{
            	if (status == Matching){
                	// consume pattern up to the closing ]
                	// match alternative sequences or ranges of characters
            		ErrorManagement::ErrorType check;
            		check = RangeMatch(input,pattern);

                	if (check.comparisonFailure){
                    	status = NotMatching;
                    	check.comparisonFailure = false;
                	}
                	if (!check){
                		status = Error;
                		ret = check;
                	}
            	}
            } break;
            case '$':{
            	if (status == Matching){

                	input.StartVariableName(pattern);
                	if (ScanVariableName(pattern)){
                    	input.StartVariable(pattern);

                		// rely on PatternMatch to balance the )
                		parenCount = 0;
                		// subpattern - need to match what lies between parenthesis
                		ErrorManagement::ErrorType check;
                		check = PatternMatch(input,pattern);

                    	if (check.comparisonFailure){
                        	status = NotMatching;
                    	}
                    	if (check.notCompleted){
            				mode = Locked;
                    	}
                    	if (check.outOfRange || check.syntaxError){
                    		status = Error;
                    		ret = check;
                    	}
                	} else {
                		status = Error;
                		ret.syntaxError = true;
                	}
            	}
            	if (status == Matching){
            		input.EndVariable();
            	}
            } break;
            case '(':{
            	if (status == Matching){
            		// rely on PatternMatch to balance the )
            		parenCount = 0;
            		// subpattern - need to match what lies between parenthesis
            		ErrorManagement::ErrorType check;
            		check = PatternMatch(input,pattern);

                	if (check.comparisonFailure){
                    	status = NotMatching;
                	}
                	if (check.notCompleted){
        				mode = Locked;
                	}
                	if (check.outOfRange || check.syntaxError){
                		status = Error;
                		ret = check;
                	}
            	} else {
            		// if we do PatternMatch we need to match the ()
            		parenCount++;
            	}
            } break;
            case endOfPattern:
            case ')':{
        		if ((parenCount == 0) || (patternElement == endOfPattern)){
//printf("end of pattern: %i \n",status); fflush(stdout);
        			if (status <= SectionFinish){
            			status = Matched;
                	} else {
//                   	if (status == NotMatching){
                   		ret.notCompleted = (mode == Locked);
                		ret.comparisonFailure = true;
            			status = NotMatched;
                   	}
        		} else {
        			parenCount --;
        		}

            } break;
            case '!':{
            	if (status == Matching ){
                	mode = Locked;
            	}
            } break;
            case '|':{
            	if (mode == Normal){
                	if (status == Matching){
                		status = SectionFinish;
                	} else
                	if (status == NotMatching){
            			status = Matching;
            			if (!input.RestoreStatus(globalInputStatus)){
                			ret.outOfRange = true;
                			status = Error;
                		}
                	}
            	}
            } break;
            case '*':
            case '+':
            case '?':
            case '{':
            {
            	CCString nextPattern = pattern;
                int maxMatches = 1000000;
                int minMatches = 0;
                bool greedy = true;

            	if (status == Matching){
                	pattern = (pattern.GetList()-1);  // put back the {
                    // consume the min number of matches and delegate the rest to RecursiveMatch
                    GetBranchParameters(pattern,minMatches,maxMatches,greedy,nextPattern);
                    if (patternElement == '?'){
                        minMatches = 0;
                        maxMatches = 1;
                    }
//printf ("nextPattern =  %s\n",nextPattern.GetList());fflush(stdout);  //TODO
            	}

                typename T::Status inputStatus;
                int  nMatches   = 0;
            	while ((status == Matching) && (nMatches < maxMatches)){

            		// try close earlier
            		if (!greedy && (nMatches >= minMatches)){
            			input.SaveStatus(inputStatus);
                    	CCString patternCopy  = nextPattern;

//printf ("Try early exit %s\n",patternCopy.GetList());  //TODO

                		ErrorManagement::ErrorType check;
                		check = PatternMatch(input,patternCopy);
                		if (check){
            				status = Matched;
            				nextPattern = patternCopy;
                		} else {
                        	if (check.outOfRange || check.syntaxError){
                        		status = Error;
                        		ret = check;
                        	} else
                           	if (check.notCompleted){
                           		mode = Locked;
                   				status = NotMatching;
                           	}
                			if (!input.RestoreStatus(inputStatus)){
                        		status = Error;
                        		ret.outOfRange = true;
            				}

                		}
            		}

            		if (status == Matching){
            			input.SaveStatus(inputStatus);
                    	CCString patternCopy  = pattern;
                    	// iteration of pattern matching
//printf ("Try loop %i match %s\n",nMatches,patternCopy.GetList());fflush(stdout);  //TODO

						ErrorManagement::ErrorType check;
						check = MatchSingle(input,patternCopy);
						if (check.outOfRange || check.syntaxError){
                    		status = Error;
                    		ret = check;
						} else
						if (check.notCompleted){
							mode = Locked;
            				status = NotMatching;
						} else
						if (check.comparisonFailure){

							if (nMatches < minMatches){
	                        	status = NotMatching;
							}

                			maxMatches = 0;// force exit the loop
                			// undo this trial
                			if (!input.RestoreStatus(inputStatus)){
                        		status = Error;
                        		ret.outOfRange = true;
            				}
//printf ("Failed * match \n");  //TODO
                        } else {
                        	nMatches++;
//printf ("match ok \n");  //TODO
                        }
            		}
            	}
            	pattern = nextPattern;

            } break;
            default:{
            	// 0 to 256 included!
            	int16 matchCharacter = patternElement & 0x1FF;
            	int16 lastCharacter;
            	if (status == Matching){
            		if (!input.GetC(lastCharacter)){
            			status = Error;
                		ret.fatalError = true;
            		}
            	}
            	if (status == Matching){
//printf("comparing %i(%c) with %i(%c)\n",lastCharacter,lastCharacter,matchCharacter,matchCharacter);
            		if (lastCharacter != matchCharacter){
                    	status = NotMatching;
            		}
            	}
            }
        }
    }


//printf ("exit PatternMatch: errcode = %x, ok=%i \n",ret.format_as_integer,ret.ErrorsCleared());   //TODO
    return ret;
}


} // Regexp
} // MARTe

#endif /* REGULAREXPRESSIONENGINE_H_ */
