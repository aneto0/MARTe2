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

#include "RegularExpression.h"
#include "Private/RegularExpressionEngine.h"

namespace MARTe{
namespace RegularExpression{

#if 0 //obsolete
class StreamIBinding{
public:
	inline StreamIBinding(StreamI &in,CStringTool     &out): stream(in),output(out){
		startPosition = in.Position();
		relativePosition = 0;
	}

	inline bool GetC(char8 & c){
		bool ret = true;
		uint32 size = 1;

		ret = stream.Read(&c,size);
		if (ret){
			relativePosition++;
			output.Append(c);
		}

		return ret;
	}
	inline uint32 Position(){
		return relativePosition;
//		return stream.Position();
	}
	bool Seek(uint32 position){
		bool ret = (position <= relativePosition);
		if (ret){
			ret = stream.Seek(position+startPosition);
		}
		if (ret){
			relativePosition = position;
			output.Truncate(position);
		}
		return ret;
	}

private:
	uint64 startPosition;
	uint32 relativePosition;
	StreamI &stream;
	CStringTool     &output;

};
#endif

/**
 * Provides an adequate interface over a stream to be able to use the RegularExpression::PatternMatch
 */
class BufferedStreamIBinding{
public:
	inline BufferedStreamIBinding(StreamI &in): stream(in),bufferT(buffer()){
		startPosition = in.Position();
		relativePosition = 0;
	}

	inline ~BufferedStreamIBinding(){
	}

	inline bool GetC(char8 & c){
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
			ret = false;
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

};

/**
 *
 */
class CStringBinding{
public:
	inline CStringBinding(CCString in): string(in){
		stringSize = string.GetSize();
		relativePosition = 0;
	}
	inline bool GetC(char8 & c){

		bool ret = true;
		if (relativePosition < stringSize){
			c = string[relativePosition];
			relativePosition++;
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

private:
	/**
	 * the string to parse
	 */
	CCString string;
	/**
	 * the size of the string to parse
	 */
	uint32 stringSize;
	/**
	 * the current position within the string
	 */
	uint32 relativePosition;
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
        	selectedRule = &ruleSet[ruleNo];
        	toContinue = false;
        } else {
        	ret.comparisonFailure = false; // reset this error to allow continuation
		REPORT_ERROR(ret,"Error while Matching a pattern");
        }
        ruleNo++;
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
        }
        ruleNo++;
    }
    return ret;
}



} //RegularExpression
} //MARTe
