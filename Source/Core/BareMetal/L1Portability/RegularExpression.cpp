/**
 * @file RegularExpression.cpp
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

#include <Private/RegularExpressionEngine.h>
#include "RegularExpression.h"

namespace MARTe{
namespace RegularExpression{

/**
 *
 */
class CStringBinding{
public:
	struct Status{
		uint32 position;
	};

	inline CStringBinding(CCString in,PatternMatchCallBackClass *callBackIn=NULL): string(in){
		stringSize = string.GetSize();
		relativePosition = 0;
		nameLength = 0;
		valueStart = 0;
		callBack = callBackIn;
	}

	/**
	 * returns error only in case of fault
	 * at the end of file returns 256
	 */
	inline bool GetC(int16 & c){

		if (relativePosition < stringSize){
			c = string[relativePosition];
			relativePosition++;
		} else {
			c = 256;
		}
		return true;
	}

	inline void SaveStatus(Status &status){
		status.position = relativePosition;
	}

	inline bool RestoreStatus(const Status &status){
		bool ret = true;
		if (status.position <= stringSize){
			relativePosition = status.position;
		} else {
			ret = false;
		}
		return ret;
	}

	inline uint32 Position(){
		return relativePosition;
	}

	inline bool Seek(uint32 newPosition){
		bool ret = true;
		if (newPosition <= stringSize){
			relativePosition = newPosition;
		} else {
			ret = false;
		}
		return ret;
	}

	inline void GetResult(CCString &modifiedInput,CStringTool &output){
		modifiedInput = string.GetList() + relativePosition;
		output.Append(string.GetList(),relativePosition);
	}

	inline void GetResult(CCString &modifiedInput){
		modifiedInput = string.GetList() + relativePosition;
		if (callBack){
			callBack->Matched(emptyString,0,string,relativePosition);
		}
	}

    inline void StartVariableName(CCString startVarName,CCString &oldVariableName){
        oldVariableName = nameStart;
        nameStart = startVarName;
    }

	inline void StartVariable(CCString endVarName,uint32 &oldNameLength,uint32 &oldValueStart){
        oldNameLength = nameLength;
		nameLength = (endVarName.GetList() - nameStart.GetList());
		if (nameLength > 0){
			nameLength--;
		}
		oldValueStart = valueStart;
		valueStart = relativePosition;
	}

    inline void EndVariable(CCString &oldVariableName,uint32 &oldNameLength,uint32 &oldValueStart,bool matched){
        if (callBack && matched){
            callBack->Matched(nameStart.GetList(),nameLength,string.GetList()+valueStart,relativePosition-valueStart);
        }
        nameStart  = oldVariableName;
        nameLength = oldNameLength;
        valueStart = oldValueStart;
    }


private:

	/**
	 * the string to parse
	 */
	CCString 		string;

	/**
	 * the size of the string to parse
	 */
	uint32 			stringSize;

	/**
	 * the current position within the string
	 */
	uint32 			relativePosition;

	/**
	 * reference to the start of the variable name inside the pattern string
	 */
	CCString 		nameStart;

	/**
	 * length of the segment of pattern that holds the name
	 */
	uint32 			nameLength;

	/**
	 * start position in the input string of the matched value
	 */
	uint32 			valueStart;

	/**
	 *
	 */
	PatternMatchCallBackClass *callBack;

};


/**
 * Provides an adequate interface over a stream to be able to use the RegularExpression::PatternMatch
 */
class BufferedStreamIBinding{
public:
	struct Status{
		uint32 position;
	};
	inline BufferedStreamIBinding(StreamI &in,PatternMatchCallBackClass *callBackIn=NULL): stream(in),bufferT(buffer()){
		startPosition 		= in.Position();
		relativePosition 	= 0;
		nameLength 			= 0;
		valueStart 			= 0;
		callBack 			= callBackIn;
	}

	inline ~BufferedStreamIBinding(){
	}

	inline bool GetC(int16 & c){
		bool ret = true;

		if (relativePosition >= bufferT.GetSize()){
			char8 block[33];
			uint32 size = sizeof(block)-1;
			stream.Read(block,size);
			block[size] = '\0';
			bufferT.Append(block,size);
		}

		if (relativePosition < bufferT.GetSize()){
			c = buffer[relativePosition];
			relativePosition++;
		} else {
			//ret = false;
			c = 256;
//printf("producing EOF token\n");
		}
		return ret;
	}

	inline void SaveStatus(Status &status){
		status.position = relativePosition;
	}

	inline bool RestoreStatus(const Status &status){
		bool ret = (status.position <= relativePosition);
		if (ret){
			relativePosition = status.position;
		}
		return ret;
	}

	inline uint32 Position(){
		return relativePosition;
	}
	inline bool Seek(uint32 position){
		bool ret = (position <= relativePosition);
		if (ret){
			relativePosition = position;
		}
		return ret;
	}
	inline void GetResult(CStringTool &output){
		stream.Seek(startPosition+relativePosition);
		output.Append(buffer.GetList(),relativePosition);
	}

	inline void GetResult(){
		stream.Seek(startPosition+relativePosition);
		if (callBack){
			callBack->Matched(emptyString,0,buffer,relativePosition);
		}
	}

    inline void StartVariableName(CCString startVarName,CCString &oldVariableName){
        oldVariableName = nameStart;
        nameStart = startVarName;
    }

    inline void StartVariable(CCString endVarName,uint32 &oldNameLength,uint32 &oldValueStart){
        oldNameLength = nameLength;
        nameLength = (endVarName.GetList() - nameStart.GetList());
        if (nameLength > 0){
            nameLength--;
        }
        oldValueStart = valueStart;
        valueStart = relativePosition;
    }

    inline void EndVariable(CCString &oldVariableName,uint32 &oldNameLength,uint32 &oldValueStart,bool matched){
        if (callBack && matched){
            callBack->Matched(nameStart.GetList(),nameLength,buffer.GetList()+valueStart,relativePosition-valueStart);
        }
        nameStart  = oldVariableName;
        nameLength = oldNameLength;
        valueStart = oldValueStart;
    }

private:

	/**
	 * stream start position
	 */
	uint64 				startPosition;

	/**
	 * relative position to the stream start
	 * position within the buffer string
	 */
	uint32 				relativePosition;

	/**
	 * the stream to get data from
	 */
	StreamI &		   	stream;

	/**
	 * buffer containing the data read from the stream
	 */
	DynamicCString     	buffer;

	/**
	 * tool to help use the buffer more efficiently
	 */
	CStringTool			bufferT;

	/**
	 * reference to the start of the variable name inside the pattern string
	 */
	CCString 			nameStart;

	/**
	 * length of the segment of pattern that holds the name
	 */
	uint32 				nameLength;

	/**
	 * start position in the input string of the matched value
	 */
	uint32 				valueStart;

	/**
	 *
	 */
	PatternMatchCallBackClass *callBack;


};


ErrorManagement::ErrorType  Match(StreamI &stream,CCString &pattern,CStringTool &matched){
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = !stream.CanSeek();
	REPORT_ERROR(ret,"Unsupported stream");
	if (ret){
		uint64 position = stream.Position();
		BufferedStreamIBinding sib(stream);
		ret = PatternMatch(sib,pattern);
		if (ret){
			sib.GetResult(matched);
		} else {
			stream.Seek(position);
		}
	}
	return ret;
}


ErrorManagement::ErrorType  Match(CCString &string,CCString &pattern,CStringTool &matched){
	ErrorManagement::ErrorType ret;

	CStringBinding cb(string);
	ret = PatternMatch(cb,pattern);
	if (ret){
		cb.GetResult(string,matched);
	}
	return ret;
}

ErrorManagement::ErrorType MatchRules(StreamI &line,const ZeroTerminatedArray<const PatternInformation> ruleSet,const PatternInformation *&selectedRule,DynamicCString &matched){
    int ruleNo = 0;
    bool toContinue = true;
    ErrorManagement::ErrorType ret;
    while((ruleSet[ruleNo].pattern.GetSize()> 0) && ret && toContinue){
        CCString pattern = ruleSet[ruleNo].pattern;
        CStringTool matchedT = matched();
        ret = RegularExpression::Match(line,pattern,matchedT);
        if (ret){
//printf("matched %s\n",ruleSet[ruleNo].ruleName.GetList());
        	selectedRule = &ruleSet[ruleNo];
        	toContinue = false;
        } else {
//printf("not matched %s\n",ruleSet[ruleNo].ruleName.GetList());

        	ret.comparisonFailure = false; // reset this error to allow continuation
        	REPORT_ERROR(ret,"Error while Matching a pattern");
            ruleNo++;
        }
    }
    if (ret){
        ret.comparisonFailure = (ruleSet[ruleNo].pattern.GetSize()== 0);
    }
    return ret;
}

ErrorManagement::ErrorType MatchRules(CCString &line,const ZeroTerminatedArray<const PatternInformation> ruleSet,const PatternInformation *&selectedRule,DynamicCString &matched){
    int ruleNo = 0;
    bool toContinue = true;
    ErrorManagement::ErrorType ret;

    while((ruleSet[ruleNo].pattern.GetSize()> 0) && ret && toContinue){
        CCString pattern = ruleSet[ruleNo].pattern;
        CStringTool matchedT = matched();
        ret = RegularExpression::Match(line,pattern,matchedT);
        if (ret){
        	selectedRule = &ruleSet[ruleNo];
        	toContinue = false;
        } else {
        	ret.comparisonFailure = false; // reset this error to allow continuation
    		REPORT_ERROR(ret,"Error while Matching a pattern");
            ruleNo++;
        }
    }
    if (ret){
//printf ("<%i> \n",ruleSet[ruleNo].pattern.GetSize()) ;
        ret.comparisonFailure = (ruleSet[ruleNo].pattern.GetSize()== 0);
    }
    return ret;
}

ErrorManagement::ErrorType Scan(StreamI &input,CCString &pattern,PatternMatchCallBackClass *callBack){
    ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = !input.CanSeek();
	REPORT_ERROR(ret,"Unsupported stream");
	if (ret){
		uint64 position = input.Position();
		BufferedStreamIBinding sib(input,callBack);
		ret = PatternMatch(sib,pattern);
		if (ret){
			sib.GetResult();
		} else {
			input.Seek(position);
		}
	}
    return ret;
}

ErrorManagement::ErrorType Scan(CCString &input,CCString &pattern,PatternMatchCallBackClass *callBack){
    ErrorManagement::ErrorType ret;

	CStringBinding cb(input,callBack);
	ret = PatternMatch(cb,pattern);
	if (ret){
		cb.GetResult(input);
	}

    return ret;
}


} //RegularExpression
} //MARTe
