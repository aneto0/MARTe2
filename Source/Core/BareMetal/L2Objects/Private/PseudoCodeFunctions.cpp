/**
 * @file PseudoCodeFunctions.cpp
 * @brief Header file for class AnyType
 * @date 8 Apr 2020
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

#include "PseudoCodeFunctions.h"

namespace MARTe{
namespace PseudoCode{

uint32 availableFunctions = 0;

FunctionRecord functionRecords[maxFunctions];


bool FunctionRecord::TryConsume(CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize){
	bool ret = false;

	// match function name
	ret = (name == nameIn);

	// match first output if matchOutput is set
	uint32 index = 0U;
	if (ret && matchOutput){
		TypeDescriptor type;
		ret = typeStack.Peek(index++,type);
		if (ret){
			ret = (type.SameAs(types[numberOfInputs]));
		}
	}

	// match inputs types
	for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
		TypeDescriptor type;
		ret = typeStack.Peek(index++,type);
		if (ret){
			ret = (type.SameAs(types[i]));
		}
	}

	// found! commit changes
	if (ret){

		// remove first output type
		if (matchOutput){
			TypeDescriptor type;
			typeStack.Pop(type);
		}

		// remove inputs types
		for (uint32 i = 0U; ret && (i < numberOfInputs); i++){
			TypeDescriptor type;
			typeStack.Pop(type);
			dataStackSize -= ByteSizeToDataMemorySize(type.StorageSize());
		}

		// insert output types
		for (uint32 i = 0U; ret && (i < numberOfOutputs); i++){
			typeStack.Push(types[i+numberOfInputs]);
			dataStackSize += ByteSizeToDataMemorySize(types[i+numberOfInputs].StorageSize());
		}
	}

	return ret;
}

/**
 * to register a function
 */
void RegisterFunction(const FunctionRecord &record){
	if (availableFunctions < maxFunctions){
		functionRecords[availableFunctions++] = record;
	}
}

/**
 * find the correct PCode and updates the type in the typestack
 */
bool FindPCodeAndUpdateTypeStack(CodeMemoryElement &code, CCString nameIn,StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput,DataMemoryAddress &dataStackSize){

	CodeMemoryElement i = 0;
//printf("looking for %s within %i - %i funs\n",nameIn.GetList(),availableFunctions,maxFunctions);

	bool found = false;
	for (i=0; (!found) && (i < availableFunctions);i++ ){
		found = functionRecords[i].TryConsume(nameIn,typeStack,matchOutput,dataStackSize);
		if (found){
			code = i;
		}
	}

	return found;
}


template <typename T> void Read(Context &context){
	CodeMemoryElement index;
	index = context.GetPseudoCode();
	context.Push(context.Variable<T>(index));
}

template <typename T> void Write(Context &context){
	CodeMemoryElement index;
	index = context.GetPseudoCode();
	context.Pop(context.Variable<T>(index));
}

template <typename T> void Duplication(Context &context){
	T var;
	context.Peek(var);
	context.Push(var);
}

REGISTER_PCODE_FUNCTION(DUP,double,1,2,Duplication<float64>,Float64Bit,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(DUP,float,1,2,Duplication<float32>,Float32Bit,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(DUP,uint64,1,2,Duplication<uint64>,UnsignedInteger64Bit,UnsignedInteger64Bit,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(DUP,int64,1,2,Duplication<int64>,SignedInteger64Bit,SignedInteger64Bit,SignedInteger64Bit)
REGISTER_PCODE_FUNCTION(DUP,uint32,1,2,Duplication<uint32>,UnsignedInteger32Bit,UnsignedInteger32Bit,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(DUP,int32,1,2,Duplication<int32>,SignedInteger32Bit,SignedInteger32Bit,SignedInteger32Bit)
REGISTER_PCODE_FUNCTION(DUP,uint16,1,2,Duplication<uint16>,UnsignedInteger16Bit,UnsignedInteger16Bit,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(DUP,int16,1,2,Duplication<int16>,SignedInteger16Bit,SignedInteger16Bit,SignedInteger16Bit)
REGISTER_PCODE_FUNCTION(DUP,uint8,1,2,Duplication<uint8>,UnsignedInteger8Bit,UnsignedInteger8Bit,UnsignedInteger8Bit)
REGISTER_PCODE_FUNCTION(DUP,int8,1,2,Duplication<int8>,SignedInteger8Bit,SignedInteger8Bit,SignedInteger8Bit)

REGISTER_PCODE_FUNCTION(READ,double,0,1,Read<float64>,Float64Bit)
REGISTER_PCODE_FUNCTION(READ,float,0,1,Read<float32>,Float32Bit)
REGISTER_PCODE_FUNCTION(READ,uint64,0,1,Read<uint64>,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(READ,int64,0,1,Read<int64>,SignedInteger64Bit)
REGISTER_PCODE_FUNCTION(READ,uint32,0,1,Read<uint32>,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(READ,int32,0,1,Read<int32>,SignedInteger32Bit)
REGISTER_PCODE_FUNCTION(READ,uint16,0,1,Read<uint16>,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(READ,int16,0,1,Read<int16>,SignedInteger16Bit)
REGISTER_PCODE_FUNCTION(READ,uint8,0,1,Read<uint8>,UnsignedInteger8Bit)
REGISTER_PCODE_FUNCTION(READ,int8,0,1,Read<int8>,SignedInteger8Bit)

REGISTER_PCODE_FUNCTION(WRITE,double,1,0,Write<float64>,Float64Bit,Float64Bit)
REGISTER_PCODE_FUNCTION(WRITE,float,1,0,Write<float32> ,Float32Bit,Float32Bit)
REGISTER_PCODE_FUNCTION(WRITE,uint64,1,0,Write<uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit)
REGISTER_PCODE_FUNCTION(WRITE,int64,1,0,Write<int64>   ,SignedInteger64Bit  ,SignedInteger64Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint32,1,0,Write<uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit)
REGISTER_PCODE_FUNCTION(WRITE,int32,1,0,Write<int32>   ,SignedInteger32Bit  ,SignedInteger32Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint16,1,0,Write<uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit)
REGISTER_PCODE_FUNCTION(WRITE,int16,1,0,Write<int16>   ,SignedInteger16Bit  ,SignedInteger16Bit  )
REGISTER_PCODE_FUNCTION(WRITE,uint8,1,0,Write<uint8>   ,UnsignedInteger8Bit ,UnsignedInteger8Bit )
REGISTER_PCODE_FUNCTION(WRITE,int8,1,0,Write<int8>     ,SignedInteger8Bit   ,SignedInteger8Bit   )

#define REGISTER_CAST_FUNCTION(name,type1,type2,nInputs,nOutputs,function,...)\
	static const TypeDescriptor name ## type1 ## type2 ## _FunctionTypes[] = {__VA_ARGS__}; \
	static const FunctionRecord name ## type1 ## type2 ## _FunctionRecord={#name,nInputs,nOutputs,name ## type1 ## type2 ## _FunctionTypes,&function<type1,type2>}; \
	static class name ## type1 ## type2 ## RegisterClass { \
	public: name ## type1 ## type2 ## RegisterClass(){\
			RegisterFunction(name ## type1 ## type2 ## _FunctionRecord);\
		}\
	} name ## type1 ## type2 ## RegisterClassInstance;

template <typename T1,typename T2> void Casting(Context &context){
	T1 x1;
	T2 x2;
	bool ret;
	context.Pop(x1);
	ret = SafeNumber2Number(x1,x2);
	context.Push(x2);
	if (!ret){
		context.runtimeError.outOfRange = true;
	}
}

REGISTER_CAST_FUNCTION(CAST,float64,float32,1,1,Casting,Float64Bit          ,Float32Bit)
REGISTER_CAST_FUNCTION(CAST,float64,uint64 ,1,1,Casting,Float64Bit          ,UnsignedInteger64Bit)
REGISTER_CAST_FUNCTION(CAST,float64,int64  ,1,1,Casting,Float64Bit          ,SignedInteger64Bit  )
REGISTER_CAST_FUNCTION(CAST,float64,uint32 ,1,1,Casting,Float64Bit          ,UnsignedInteger32Bit)
REGISTER_CAST_FUNCTION(CAST,float64,int32  ,1,1,Casting,Float64Bit          ,SignedInteger32Bit  )
REGISTER_CAST_FUNCTION(CAST,float64,uint16 ,1,1,Casting,Float64Bit          ,UnsignedInteger16Bit)
REGISTER_CAST_FUNCTION(CAST,float64,int16  ,1,1,Casting,Float64Bit          ,SignedInteger16Bit  )
REGISTER_CAST_FUNCTION(CAST,float64,uint8  ,1,1,Casting,Float64Bit          ,UnsignedInteger8Bit )
REGISTER_CAST_FUNCTION(CAST,float64,int8   ,1,1,Casting,Float64Bit          ,SignedInteger8Bit   )

REGISTER_CAST_FUNCTION(CAST,float32,float64,1,1,Casting,Float32Bit          ,Float64Bit)
REGISTER_CAST_FUNCTION(CAST,float32,uint64 ,1,1,Casting,Float32Bit          ,UnsignedInteger64Bit)
REGISTER_CAST_FUNCTION(CAST,float32,int64  ,1,1,Casting,Float32Bit          ,SignedInteger64Bit  )
REGISTER_CAST_FUNCTION(CAST,float32,uint32 ,1,1,Casting,Float32Bit          ,UnsignedInteger32Bit)
REGISTER_CAST_FUNCTION(CAST,float32,int32  ,1,1,Casting,Float32Bit          ,SignedInteger32Bit  )
REGISTER_CAST_FUNCTION(CAST,float32,uint16 ,1,1,Casting,Float32Bit          ,UnsignedInteger16Bit)
REGISTER_CAST_FUNCTION(CAST,float32,int16  ,1,1,Casting,Float32Bit          ,SignedInteger16Bit  )
REGISTER_CAST_FUNCTION(CAST,float32,uint8  ,1,1,Casting,Float32Bit          ,UnsignedInteger8Bit )
REGISTER_CAST_FUNCTION(CAST,float32,int8   ,1,1,Casting,Float32Bit          ,SignedInteger8Bit   )

// TODO - implement without casting. promote all results between u/int16 and u/int8 to int32 as the compiler would do
#define REGISTER_OPERATOR(name,oper,fname)	    							        \
		template <typename T> void function ## fname ## ication (Context &context){ \
			T x1,x2,x3;													     		\
			context.Pop(x1);                                                        \
			context.Pop(x2);                                                        \
			x3 = static_cast<T>(x1 oper x2);                                        \
			context.Push(x3);                                                       \
		}                                                                           \
		REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit,Float64Bit)  \
		REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit,Float32Bit)  \
		REGISTER_PCODE_FUNCTION(name,uint64 ,2,1,function ## fname ## ication <uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit,UnsignedInteger64Bit) \
		REGISTER_PCODE_FUNCTION(name,int64  ,2,1,function ## fname ## ication <int64>  ,SignedInteger64Bit,SignedInteger64Bit,SignedInteger64Bit)       \
		REGISTER_PCODE_FUNCTION(name,uint32 ,2,1,function ## fname ## ication <uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit,UnsignedInteger32Bit) \
		REGISTER_PCODE_FUNCTION(name,int32  ,2,1,function ## fname ## ication <int32>  ,SignedInteger32Bit,SignedInteger32Bit,SignedInteger32Bit)       \
		REGISTER_PCODE_FUNCTION(name,uint16 ,2,1,function ## fname ## ication <uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit,UnsignedInteger16Bit) \
		REGISTER_PCODE_FUNCTION(name,int16  ,2,1,function ## fname ## ication <int16>  ,SignedInteger16Bit,SignedInteger16Bit,SignedInteger16Bit)       \
		REGISTER_PCODE_FUNCTION(name,uint8  ,2,1,function ## fname ## ication <uint8>  ,UnsignedInteger8Bit,UnsignedInteger8Bit,UnsignedInteger8Bit)    \
		REGISTER_PCODE_FUNCTION(name,int8   ,2,1,function ## fname ## ication <int8>   ,SignedInteger8Bit,SignedInteger8Bit,SignedInteger8Bit)

REGISTER_OPERATOR(ADD, + ,Addition)
REGISTER_OPERATOR(SUB, - ,Subtract)
REGISTER_OPERATOR(MUL, * ,Multipl)
REGISTER_OPERATOR(DIV, / ,Division)


#define REGISTER_1_FUNCTION(name,fname)	    							\
		template <typename T> void function ## fname ## ication (Context &context){ \
			T x,res;													     		\
			context.Pop(x);                                                         \
			res = fname (x);                                                        \
			context.Push(res);                                                      \
		}                                                                           \
		REGISTER_PCODE_FUNCTION(name,float32,1,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit)  \
		REGISTER_PCODE_FUNCTION(name,float64,1,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit)

REGISTER_1_FUNCTION(SIN,sin)
REGISTER_1_FUNCTION(COS,cos)
REGISTER_1_FUNCTION(TAN,tan)
REGISTER_1_FUNCTION(EXP,exp)
REGISTER_1_FUNCTION(LOG,log)
REGISTER_1_FUNCTION(LOG10,log10)

#define REGISTER_2_FUNCTION(name,fname)	    							\
		template <typename T> void function ## fname ## ication (Context &context){ \
			T x1,x2,res;	     										     		\
			context.Pop(x1);                                                        \
			context.Pop(x2);                                                        \
			res = fname (x1,x2);                                                    \
			context.Push(res);                                                      \
		}                                                                           \
		REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit,Float32Bit,Float32Bit)  \
		REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit,Float64Bit,Float64Bit)

REGISTER_2_FUNCTION(POW,pow)

#define REGISTER_COMPARE_OPERATOR(name,oper,fname)	    						    \
		template <typename T> void function ## fname ## ication (Context &context){ \
			T x1,x2;													     		\
			bool ret;													     		\
			context.Pop(x1);                                                        \
			context.Pop(x2);                                                        \
			ret = x1 oper x2;                                                       \
			context.Push(ret);                                                      \
		}                                                                           \
		REGISTER_PCODE_FUNCTION(name,float64,2,1,function ## fname ## ication <float64>,Float64Bit			,Float64Bit				,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,float32,2,1,function ## fname ## ication <float32>,Float32Bit			,Float32Bit				,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,uint64 ,2,1,function ## fname ## ication <uint64> ,UnsignedInteger64Bit,UnsignedInteger64Bit	,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,int64  ,2,1,function ## fname ## ication <int64>  ,SignedInteger64Bit	,SignedInteger64Bit		,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,uint32 ,2,1,function ## fname ## ication <uint32> ,UnsignedInteger32Bit,UnsignedInteger32Bit	,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,int32  ,2,1,function ## fname ## ication <int32>  ,SignedInteger32Bit	,SignedInteger32Bit		,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,uint16 ,2,1,function ## fname ## ication <uint16> ,UnsignedInteger16Bit,UnsignedInteger16Bit	,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,int16  ,2,1,function ## fname ## ication <int16>  ,SignedInteger16Bit	,SignedInteger16Bit		,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,uint8  ,2,1,function ## fname ## ication <uint8>  ,UnsignedInteger8Bit,UnsignedInteger8Bit		,UnsignedInteger8Bit) \
		REGISTER_PCODE_FUNCTION(name,int8   ,2,1,function ## fname ## ication <int8>   ,SignedInteger8Bit	,SignedInteger8Bit		,UnsignedInteger8Bit)

REGISTER_COMPARE_OPERATOR(EQ, == ,Equal)
REGISTER_COMPARE_OPERATOR(NEQ, != ,Different)
REGISTER_COMPARE_OPERATOR(GT, > ,Greater)
REGISTER_COMPARE_OPERATOR(LT, < ,Smaller)
REGISTER_COMPARE_OPERATOR(GTE, >= ,Great)
REGISTER_COMPARE_OPERATOR(LTE, <= ,Small)

#define REGISTER_LOGICAL_OPERATOR(name,oper,fname)	    						    \
		void function ## fname ## ication (Context &context){                       \
			bool x1,x2,ret;													        \
			context.Pop(x1);                                                        \
			context.Pop(x2);                                                        \
			ret = x1 oper x2;                                                       \
			context.Push(ret);                                                      \
		}                                                                           \
      	REGISTER_PCODE_FUNCTION(name,boolean,2,1,function ## fname ## ication,UnsignedInteger8Bit,UnsignedInteger8Bit,UnsignedInteger8Bit)

REGISTER_LOGICAL_OPERATOR(AND, && ,And)
REGISTER_LOGICAL_OPERATOR(OR, || ,Or)
REGISTER_LOGICAL_OPERATOR(XOR, ^ ,xor)




} // PseudoCode
} //MARTe
