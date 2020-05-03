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
	inline StreamIBinding(BufferedStreamI &in): stream(in){}

	inline bool GetC(char8 & c){
		bool ret = true;
		uint32 size = 1;

		ret = stream.Read(&c,size);

		return ret;
	}
	inline uint32 Position(){
		return stream.Position();
	}
	bool Seek(uint32 position){
		bool ret = true;
		ret = stream.Seek(position);
		return ret;
	}

private:
	BufferedStreamI & stream;

};

class CStringBinding{
public:
	inline CStringBinding(CCString in): string(in){
		stringSize = string.GetSize();
		position = 0;
	}
	inline bool GetC(char8 & c){
		bool ret = true;
		if (position < stringSize){
			c = string[position];
//printf("<%i:%c(%i)>\n",position,c,c);  // TODO remove
			position++;
		} else {
			ret = false;
		}
		return ret;
	}
	inline uint32 Position(){
//printf("<P:%i>\n",position);  // TODO remove
		return position;
	}
	bool Seek(uint32 newPosition){
		bool ret = true;
		if (newPosition <= stringSize){
			position = newPosition;
//printf("<S:%i>\n",position);  // TODO remove
		} else {
			ret = false;
		}
		return ret;
	}

private:
	CCString string;
	uint32 stringSize;
	uint32 position;

};


ErrorManagement::ErrorType  Match(BufferedStreamI &stream,CCString &pattern){
	ErrorManagement::ErrorType ret;

	StreamIBinding sib(stream);
	ret = PatternMatch(sib,pattern);

	return ret;
}

ErrorManagement::ErrorType  Match(CCString &string,CCString &pattern){
	ErrorManagement::ErrorType ret;

//printf ("START PatternMatch(%s,%s)\n",string.GetList(),pattern.GetList());
	CStringBinding cb(string);
	ret = PatternMatch(cb,pattern);
	string = string.GetList()+cb.Position();
	return ret;
}

} //RegularExpression
} //MARTe
