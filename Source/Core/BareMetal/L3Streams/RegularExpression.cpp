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

class BufferedStreamIBinding{
public:
	inline BufferedStreamIBinding(StreamI &in): stream(in),bufferT(buffer()){
		startPosition = in.Position();
		relativePosition = 0;
	}

	~BufferedStreamIBinding(){
		stream.Seek(startPosition+relativePosition);
	}

	inline bool GetC(char8 & c){
		bool ret = true;

		if (relativePosition >= bufferT.GetSize()){
			char8 block[33];
			uint32 size = sizeof(block)-1;
			stream.Read(block,size);
			block[size] = '\0';
			bufferT.Append(block,size);
//printf("(+%i> %i \"%s\")",size,bufferT.GetSize(),block);
		}

		if (relativePosition < bufferT.GetSize()){
			c = buffer[relativePosition];
//printf("([%i of %i]:%c)",relativePosition,bufferT.GetSize(),c);
			relativePosition++;
		} else {
			ret = false;
		}
		return ret;
	}
	inline uint32 Position(){
		return relativePosition;
	}
	bool Seek(uint32 position){
		bool ret = (position <= relativePosition);
		if (ret){
			relativePosition = position;
//printf("(%i<)",position);
		}
		return ret;
	}
	void GetResult(CStringTool &output){
		output.Append(buffer.GetList(),relativePosition);
	}

private:
	/**
	 * stream start position
	 */
	uint64 				startPosition;
	/**
	 * relative position to the stream start
	 */
	uint32 				relativePosition;
	StreamI &		   	stream;
	DynamicCString     	buffer;
	CStringTool			bufferT;

};


class CStringBinding{
public:
	inline CStringBinding(CCString in,CStringTool     &out): string(in),output(out){
		stringSize = string.GetSize();
		relativePosition = 0;
	}
	inline bool GetC(char8 & c){
//printf("DO"); //TODO

		bool ret = true;
		if (relativePosition < stringSize){
			c = string[relativePosition];
			relativePosition++;
			ErrorManagement::ErrorType ok = output.Append(c);
			if (!ok){
//printf("NOT"); //TODO
				ret = false;
			}
//printf("READ -> %c:%i",c,c);fflush(stdout); //TODO
		} else {
			ret = false;
		}
		return ret;
	}
	inline uint32 Position(){
		return relativePosition;
	}
	bool Seek(uint32 newPosition){
		bool ret = true;
		if (newPosition <= stringSize){
			relativePosition = newPosition;
			ErrorManagement::ErrorType ok = output.Truncate(newPosition);
			if (!ok) {
//printf("NOT");//TODO
				ret = false;
			}
//printf("SEEK -> %i",relativePosition);fflush(stdout); //TODO
		} else {
			ret = false;
		}
		return ret;
	}

private:
	CCString string;
	uint32 stringSize;
	uint32 relativePosition;
	CStringTool     &output;

};

#if 0
ErrorManagement::ErrorType  Match(StreamI &stream,CCString &pattern,CStringTool &matched){
	ErrorManagement::ErrorType ret;


	StreamIBinding sib(stream,matched);
	ret = PatternMatch(sib,pattern);

	return ret;
}
#else
ErrorManagement::ErrorType  Match(StreamI &stream,CCString &pattern,CStringTool &matched){
	ErrorManagement::ErrorType ret;


//printf ("\nSTART PatternMatch(%s)\n",pattern.GetList());
	BufferedStreamIBinding sib(stream);
	ret = PatternMatch(sib,pattern);
	if (ret){
		sib.GetResult(matched);
	}

	return ret;
}

#endif

ErrorManagement::ErrorType  Match(CCString &string,CCString &pattern,CStringTool &matched){
	ErrorManagement::ErrorType ret;


//printf ("START PatternMatch(%s,%s)\n",string.GetList(),pattern.GetList());
	CStringBinding cb(string,matched);
	ret = PatternMatch(cb,pattern);
	string = string.GetList()+cb.Position();
	return ret;
}

} //RegularExpression
} //MARTe
